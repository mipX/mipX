/*=========================================================================
 
 Program:   mipx
 Module:    LaaUI.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "LaaUI.h"


LaaUI_Thresholding::LaaUI_Thresholding()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("LaaUI>>(1) Threshold");
    this->GetConfig()->SetTitleLong("Thresholding");
}


void LaaUI_Thresholding::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            char const *input_string = tinyfd_inputBox("Enter Threshold value as INT number", "", "");
            if (!input_string) return ;
            
            int threshold = std::stoi(input_string);//0;
        
            
            uint64_t min_value_included, max_value_included;
            img.GetVoxelValueFullRange(min_value_included, max_value_included);
            if(threshold<min_value_included && threshold>max_value_included) return;
            
            mxString name;
            name.Assign("thr_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxImage *thresholded_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!thresholded_image) return;
            
            thresholded_image->SetPropertiesAs(image);

            mxGIP gip;
            gip.Threshold(*image, threshold, *thresholded_image);
            
        }
    }
}


void LaaUI_Thresholding::StartInMainThread()
{
    this->Execute();
}






//-------------------------------------------------------------------------------------------------------------------------


LaaUI_CreateMesh::LaaUI_CreateMesh()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Create Mesh");
    this->GetConfig()->SetTitleLong("Create Mesh");
}


void LaaUI_CreateMesh::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            char const *input_string = tinyfd_inputBox("Enter Threshold value as INT number", "", "");
            if (!input_string) return ;
            
            int threshold = std::stoi(input_string);
            
            uint64_t min_value_included, max_value_included;
            img.GetVoxelValueFullRange(min_value_included, max_value_included);
            if(threshold<min_value_included || threshold>max_value_included) return;
            
            mxString name;
            //name.Assign("m_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxMesh m;
            vmxMesh *mesh = static_cast< vmxMesh* > (this->GetAppMainWidget()->GetDataListWidget()->Create(m.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!mesh) return;
            
            mesh->CreatePolyData(image, threshold);
            mesh->CreateActorByLookupTableScalarColoring(0,255);
        }
    }
    
    
    
}


void LaaUI_CreateMesh::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------



LaaUI_DistanceTransform::LaaUI_DistanceTransform()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("LaaUI>>(2) Distance Transform ");
    this->GetConfig()->SetTitleLong("Euclidean Distance Transform");
}


void LaaUI_DistanceTransform::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            laaEDT edt;

            mxString name;
            name.Assign("edt_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxImage *edt_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!edt_image) return;
            
            edt_image->SetPropertiesAs(image);

            edt.createMaxRadiusImageSITK(*image, *edt_image);

            // Set properties again because the edt.create reads the image from
            // a temp file
            edt_image->SetPropertiesAs(image);

        }
    }
    
    
    
}


void LaaUI_DistanceTransform::StartInMainThread()
{
    this->Execute();
}

//-------------------------------------------------------------------------------------------------------------------------



LaaUI_DecreasingRadiiSegmentation::LaaUI_DecreasingRadiiSegmentation()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("LaaUI>>(3) Decreasing Radii Segmentation ");
    this->GetConfig()->SetTitleLong("Decreasing Radii Segmentation");
}


void LaaUI_DecreasingRadiiSegmentation::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    vmxImage radius_image;
    int seed_slice, seed_row, seed_column;
    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        vmxPointList list;
        std::cout << "Analyzing: " << selected_data_objects[i]->GetClassName() << std::endl;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;

            radius_image.Copy(image);
            std::cout << "DRS-> " << "Loaded radius image" << std::endl;
        }
        else if(selected_data_objects[i]->GetClassName()==list.GetClassName())
        {
            mxPointList *in_list = static_cast<mxPointList*>(selected_data_objects[i]);
            if(!in_list) return;

            mxList<mxPointListElement> *seed_points;
            seed_points = in_list->GetListOfPoints();

            seed_slice  = seed_points->GetBeginElement().GetPoint()->S();
            seed_row    = seed_points->GetBeginElement().GetPoint()->R();
            seed_column = seed_points->GetBeginElement().GetPoint()->C();

            std::cout << "DRS -> " << "Loaded seeds" << std::endl;
            std::cout << "DRS -> " << "coords: " << seed_slice << " " <<
              seed_row << " " << seed_column << std::endl;

        }
    }
    
    std::cout << "DRS -> " << "Converted seeds" << std::endl;
    radius_image.IndexFromWorldCoordinates((double)seed_slice, (double)seed_row, (double)seed_column, seed_slice, seed_row, seed_column);
    std::cout << "DRS -> " << "coords: " << seed_slice << " " <<
      seed_row << " " << seed_column << std::endl;
    
    
}


void LaaUI_DecreasingRadiiSegmentation::StartInMainThread()
{
    this->Execute();
}

