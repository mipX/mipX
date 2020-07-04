/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessSkeleton.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppProcessSkeleton.h"






//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessSkeleton_CreateFromImage::vmxAppProcessSkeleton_CreateFromImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Skeleton>>Create From Image");
    this->GetConfig()->SetTitleLong("Create From Image");
}


//void vmxAppProcessSkeleton_CreateFromImage::Step_0()
//{
//    this->GetConfig()->AddInputDataObjects("Point list and curve: ");
//
//}


void vmxAppProcessSkeleton_CreateFromImage::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);

    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("skl_");
            name.Append(selected_data_objects[i]->GetObjectName());

            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;

            vmxSkeleton skl_temp;
            vmxSkeleton *skel = static_cast< vmxSkeleton* > (this->GetAppMainWidget()->GetDataListWidget()->Create(skl_temp.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!skel) return;


            skel->CreateFromSkeletonizedImageOfDisconnectedComponents(*image, "Skeletonized");
            
            skel->Update();
            skel->SetTimeIndex(0);

        }
    }
}


void vmxAppProcessSkeleton_CreateFromImage::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------



