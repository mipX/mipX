/*=========================================================================
 
 Program:   mipx
 Module:    ImageProcessing.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "ImageProcessing.h"





//-------------------------------------------------------------------------------------------------------------------------



ImageProcessing_Thresholding::ImageProcessing_Thresholding()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Threshold");
    this->GetConfig()->SetTitleLong("Thresholding");
}


void ImageProcessing_Thresholding::Execute()
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


void ImageProcessing_Thresholding::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------




ImageProcessing_CreateMesh::ImageProcessing_CreateMesh()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Create Mesh");
    this->GetConfig()->SetTitleLong("Create Mesh");
}


void ImageProcessing_CreateMesh::Execute()
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


void ImageProcessing_CreateMesh::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------




ImageProcessing_ExtractLargestCC::ImageProcessing_ExtractLargestCC()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Extract Largest CC");
    this->GetConfig()->SetTitleLong("Extract Largest CC");
}


void ImageProcessing_ExtractLargestCC::Execute()
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
            name.Assign("ext_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            
            
            vmxImage img_temp;
            vmxImage *extracted = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img_temp.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!extracted) return;
            
            extracted->SetPropertiesAs(image);
            
            
            mxBIP bip;
            if(!bip.ExtractLargestConnectedComponent26(*image,threshold,*extracted,0))
            {
                std::cout<<std::endl<<"ImageProcessing_ExtractLargestCC::Execute() error: mxBIP::ExtractLargestConnectedComponent26() returned 0 (fail)!";
                this->Abort();
                return;
            }
        }
    }
}


void ImageProcessing_ExtractLargestCC::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------




ImageProcessing_OrderedSkeletonization::ImageProcessing_OrderedSkeletonization()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Ordered Skeletonization");
    this->GetConfig()->SetTitleLong("Ordered Skeletonization");
}


void ImageProcessing_OrderedSkeletonization::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);

    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("skl_");
            name.Append(selected_data_objects[i]->GetObjectName());

            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;

            vmxImage *output_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!output_image) return;

            output_image->SetPropertiesAs(image);
            
            vmxImage pv;
            
            mxBIP bip;
            bip.ProfileVolumeTransformForSphere(*image, *image, pv);

            mxSkeletonization skl;
            skl.OrderedSkeletonization(pv, *output_image);
        }
    }
}


void ImageProcessing_OrderedSkeletonization::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------


