/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessProfile.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppProcessProfile.h"




//-------------------------------------------------------------------------------------------------------------------------





vmxAppProcessProfile_Create_RC_Profile::vmxAppProcessProfile_Create_RC_Profile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    mxString menu;
    menu.Append(vmxAppProcessProfileMenuString);
    menu.Append(">>Create RC Profile");
    this->GetConfig()->SetTitleInMenu(menu.Get_C_String());
    //this->GetConfig()->SetTitleInMenu("Image>>Create RC Profile");
    this->GetConfig()->SetTitleLong("Create RC Profile");
}



void vmxAppProcessProfile_Create_RC_Profile::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
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
            
            vmxImage mask;
            mxGIP gip;
            gip.Threshold(*image, threshold, mask);
    
            char const *input_string2 = tinyfd_inputBox("Enter Radius limit as INT number", "", "");
            if (!input_string2) return ;
            
            int radius_limit = std::stoi(input_string2);
            if(radius_limit<2 || radius_limit > 20) radius_limit = 10;
            
            vmxProfile prf_temp;
            vmxProfile *prf = static_cast< vmxProfile* > (this->GetAppMainWidget()->GetDataListWidget()->Create(prf_temp.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!prf) return;
            
            mxProfileProcessing prprc;
            prprc.Make_R_Profile_CircleDSE(*image, mask, *prf, radius_limit);
            
            //mesh->CreatePolyData(image, threshold);
            //mesh->CreateActorByLookupTableScalarColoring(0,255);
        }
    }

    
    
}


void vmxAppProcessProfile_Create_RC_Profile::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessProfile_Create_RS_Profile::vmxAppProcessProfile_Create_RS_Profile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    mxString menu;
    menu.Append(vmxAppProcessProfileMenuString);
    menu.Append(">>Create RS Profile");
    this->GetConfig()->SetTitleInMenu(menu.Get_C_String());
    //this->GetConfig()->SetTitleInMenu("Image>>Create RS Profile");
    this->GetConfig()->SetTitleLong("Create RS Profile");
}



void vmxAppProcessProfile_Create_RS_Profile::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
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
            
            vmxImage mask;
            mxGIP gip;
            gip.Threshold(*image, threshold, mask);
            
            char const *input_string2 = tinyfd_inputBox("Enter Radius limit as INT number", "", "");
            if (!input_string2) return ;
            
            int radius_limit = std::stoi(input_string2);
            if(radius_limit<2 || radius_limit > 20) radius_limit = 10;
            
            vmxProfile prf_temp;
            vmxProfile *prf = static_cast< vmxProfile* > (this->GetAppMainWidget()->GetDataListWidget()->Create(prf_temp.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!prf) return;
            
            mxProfileProcessing prprc;
            prprc.Make_R_Profile_SphereDSE(*image, mask, *prf, radius_limit);
            
            //mesh->CreatePolyData(image, threshold);
            //mesh->CreateActorByLookupTableScalarColoring(0,255);
        }
    }
    
    
    
}


void vmxAppProcessProfile_Create_RS_Profile::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessProfile_ProfileMeasure::vmxAppProcessProfile_ProfileMeasure()
{
    this->GetConfig()->SetNumberOfSteps(1);
    mxString menu;
    menu.Append(vmxAppProcessProfileMenuString);
    menu.Append(">>Profile Measure");
    this->GetConfig()->SetTitleInMenu(menu.Get_C_String());
    this->GetConfig()->SetTitleLong("Profile Measure");
}


void vmxAppProcessProfile_ProfileMeasure::Step_0()
{
    this->GetConfig()->AddInputDataObjects("Input profile:");
    mxList<mxString> list_of_operators;
    list_of_operators.AddNewToEnd()->Assign("min");
    list_of_operators.AddNewToEnd()->Assign("max");
    list_of_operators.AddNewToEnd()->Assign("avr");
    list_of_operators.AddNewToEnd()->Assign("med(-)");
    list_of_operators.AddNewToEnd()->Assign("med(+)");
    list_of_operators.AddNewToEnd()->Assign("med_avr");
    list_of_operators.AddNewToEnd()->Assign("minmax_avr");
    list_of_operators.AddNewToEnd()->Assign("med_root");
    list_of_operators.AddNewToEnd()->Assign("minmax_root");
    
    this->GetConfig()->AddInputCheckBoxOptions("Operators:", &list_of_operators);
}


void vmxAppProcessProfile_ProfileMeasure::Execute()
{
    vmxProfile prf_temp;
    mxArray< mxDataObject* > ia = this->GetData()->GetDataObjectsOfType(prf_temp.GetClassName().Get_C_String());
    
    if(ia.IsEmpty())
    {
        std::cout<<std::endl<<"vmxAppProcessProfile_ProfileMeasure::Execute() error: array of images is empty.";
        this->Abort();
        return;
    }
    
    vmxProfile *prf = static_cast<vmxProfile*>(ia[0]);
    if(!prf)
    {
        std::cout<<std::endl<<"vmxAppProcessProfile_ProfileMeasure::Execute() error: pointer to profile is NULL.";
        this->Abort();
        return;
    }
    
    //prf->GetVa
    
    mxString name_string;
    name_string.Assign("pm_");
    mxArray< mxString > operators =  this->GetData()->GetSelectedCheckBoxOptions("Operators:");
    
    if(operators.IsEmpty())
    {
        std::cout<<std::endl<<"vmxAppProcessProfile_ProfileMeasure::Execute() error: array of operators is empty.";
        this->Abort();
        return;
    }
    std::cout<<std::endl<<"Operators:";
    for(int i=0; i< (int)operators.GetNumberOfElements(); i++)
    {
        std::cout<<"  '"<<operators[i].Get_C_String()<<"'";
        name_string.Append("_");
        name_string.Append( operators[i].Get_C_String() );
    }
    

    
    vmxImage image;
    
    mxProfileProcessing prprc;
    prprc.ProfileMeasure(*prf, image, operators, mxProfileProcessing::GREATER);
    
//    uint64_t min_value_included, max_value_included;
//    image.GetVoxelMinAndMaxValues(min_value_included, max_value_included);
    
//    int threshold = max_value_included - 1;
    
//    char const *input_string = tinyfd_inputBox("Enter Threshold value as INT number", "", "");
//    if (!input_string) return ;
//
//    int threshold = std::stoi(input_string);
//
//    uint64_t min_value_included, max_value_included;
//    image.GetVoxelValueFullRange(min_value_included, max_value_included);
//    if(threshold<min_value_included || threshold>max_value_included) return;

    
    //image.SetVisibility(1);
    //image.SetMappingToOpaqueGrayScale();
    
    
    vmxImage *image_segmented = static_cast<vmxImage*> (this->GetAppMainWidget()->GetDataListWidget()->Create(image.GetClassName().Get_C_String(),name_string.Get_C_String()));
    
    image_segmented->Copy(&image);
    
//    mxGIP gip;
//    gip.Threshold(image, threshold, *image_segmented);

    image_segmented->SetMappingToOpaqueGrayScale();
    
}


void vmxAppProcessProfile_ProfileMeasure::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessImage_ThresholdProfileMeasureImage::vmxAppProcessImage_ThresholdProfileMeasureImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    mxString menu;
    menu.Append(vmxAppProcessProfileMenuString);
    menu.Append(">>Max Threshold Profile Measure Image");
    this->GetConfig()->SetTitleInMenu(menu.Get_C_String());
    this->GetConfig()->SetTitleLong("Max Threshold Profile Measure Image");
}


void vmxAppProcessImage_ThresholdProfileMeasureImage::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            uint64_t min_value_included, max_value_included;
            image->GetVoxelMinAndMaxValues(min_value_included, max_value_included);
        
            int threshold = max_value_included - 1;
            
            mxString name;
            name.Assign("thr");
            name.AppendNumber(threshold);
            name.Append("_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *thresholded_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!thresholded_image) return;
            
            thresholded_image->SetPropertiesAs(image);
            
            mxGIP gip;
            gip.Threshold(*image, threshold, *thresholded_image);
            
            thresholded_image->SetMappingToOpaqueGrayScale();
        }
    }
}


void vmxAppProcessImage_ThresholdProfileMeasureImage::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------
