/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessImage.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppProcessImage.h"




//-------------------------------------------------------------------------------------------------------------------------



vmxAppProcessImage_CompactValues::vmxAppProcessImage_CompactValues()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Compact values");
    this->GetConfig()->SetTitleLong("Compact values");
}


void vmxAppProcessImage_CompactValues::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("cmp_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxImage *compacted_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!compacted_image) return;
            
            compacted_image->SetPropertiesAs(image);
            
            mxGIP gip;
            gip.CompactValues(*image, *compacted_image);
            
            compacted_image->SetMappingToOpaqueGrayScale();
            
        }
    }
    
    
    
}


void vmxAppProcessImage_CompactValues::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------



vmxAppProcessImage_Thresholding::vmxAppProcessImage_Thresholding()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Threshold");
    this->GetConfig()->SetTitleLong("Thresholding");
}


void vmxAppProcessImage_Thresholding::Execute()
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
            
            thresholded_image->SetMappingToOpaqueGrayScale();
            
        }
    }
    
    
    
}


void vmxAppProcessImage_Thresholding::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------








vmxAppProcessImage_CreateMesh::vmxAppProcessImage_CreateMesh()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Create Mesh");
    this->GetConfig()->SetTitleLong("Create Mesh");
}


void vmxAppProcessImage_CreateMesh::Execute()
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
            
            vmxMesh m;
            vmxMesh *mesh = static_cast< vmxMesh* > (this->GetAppMainWidget()->GetDataListWidget()->Create(m.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!mesh) return;
            
            mesh->CreatePolyData(image, threshold);
            mesh->CreateActorByLookupTableScalarColoring(0,255);
        }
    }
    
    
    
}


void vmxAppProcessImage_CreateMesh::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessImage_Subtract::vmxAppProcessImage_Subtract()
{
    this->GetConfig()->SetNumberOfSteps(1);
    this->GetConfig()->SetTitleInMenu("Image>>Absolute difference");//Subtract Images");
    this->GetConfig()->SetTitleLong("Subtract Images");
}


void vmxAppProcessImage_Subtract::Step_0()
{
    this->GetConfig()->AddInputDataObjects("Image:");
    this->GetConfig()->AddInputDataObjects("Image to subtract:");
//    mxList<mxString> list_of_options;
//    list_of_options.AddNewToEnd()->Assign("Absolute");
//    list_of_options.AddNewToEnd()->Assign("Leveled to Zero for negative values");
//
//    this->GetConfig()->AddInputRadioButtonOptions("Options:", &list_of_options);
}


void vmxAppProcessImage_Subtract::Execute()
{
    vmxImage img_temp;
    mxArray< mxDataObject* > ia = this->GetData()->GetDataObjectsOfType(img_temp.GetClassName().Get_C_String());
    
    if(ia.GetNumberOfElements()!=2)
    {
        std::cout<<std::endl<<"vmxAppProcessImage_Subtract::Execute() error: array of images is empty.";
        this->Abort();
        return;
    }
    
    vmxImage *img1 = static_cast<vmxImage*>(ia[0]);
    vmxImage *img2 = static_cast<vmxImage*>(ia[1]);
    
    if(!img1 || !img2)
    {
        std::cout<<std::endl<<"vmxAppProcessImage_Subtract::Execute() error: pointer to image is NULL.";
        this->Abort();
        return;
    }
    
    mxString name_string;
    name_string.Assign("diff_");
    
//    mxArray< mxString > operators =  this->GetData()->GetSelectedCheckBoxOptions("Options:");
//
//    if(operators.IsEmpty())
//    {
//        std::cout<<std::endl<<"vmxAppProcessImage_Subtract::Execute() error: array of operators is empty.";
//        this->Abort();
//        return;
//    }
//    std::cout<<std::endl<<"Operators:";
//    for(int i=0; i<operators.GetNumberOfElements(); i++)
//    {
//        std::cout<<"  '"<<operators[i].Get_C_String()<<"'";
//        name_string.Append("_");
//        name_string.Append( operators[i].Get_C_String() );
//    }
    
   
    name_string.Append(img1->GetObjectName());
    vmxImage *image_diff = static_cast<vmxImage*> (this->GetAppMainWidget()->GetDataListWidget()->Create(img_temp.GetClassName().Get_C_String(),name_string.Get_C_String()));
    
    mxGIP gip;
    gip.DifferenceAbsolute(*img1, *img2, *image_diff);
    
    image_diff->SetMappingToOpaqueGrayScale();
}


void vmxAppProcessImage_Subtract::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppProcessImage_Mask::vmxAppProcessImage_Mask()
{
    this->GetConfig()->SetNumberOfSteps(1);
    this->GetConfig()->SetTitleInMenu("Image>>Mask");
    this->GetConfig()->SetTitleLong("Mask Images");
}


void vmxAppProcessImage_Mask::Step_0()
{
    this->GetConfig()->AddInputDataObjects("Image to mask:");
    this->GetConfig()->AddInputDataObjects("Mask image:");
}


void vmxAppProcessImage_Mask::Execute()
{
    vmxImage img_temp;
    mxArray< mxDataObject* > ia = this->GetData()->GetDataObjectsOfType(img_temp.GetClassName().Get_C_String());
    
    if(ia.GetNumberOfElements()!=2)
    {
        std::cout<<std::endl<<"vmxAppProcessImage_Mask::Execute() error: array of images is empty.";
        this->Abort();
        return;
    }
    
    vmxImage *img1 = static_cast<vmxImage*>(ia[0]);
    vmxImage *img2 = static_cast<vmxImage*>(ia[1]);
    
    if(!img1 || !img2)
    {
        std::cout<<std::endl<<"vmxAppProcessImage_Mask::Execute() error: pointer to image is NULL.";
        this->Abort();
        return;
    }
    
    mxString name_string;
    name_string.Assign("msk_");
    
    name_string.Append(img1->GetObjectName());
    vmxImage *image_out = static_cast<vmxImage*> (this->GetAppMainWidget()->GetDataListWidget()->Create(img_temp.GetClassName().Get_C_String(),name_string.Get_C_String()));
    
    mxGIP gip;
    gip.Mask(*img1, *img2, *image_out);
    
    image_out->SetMappingToOpaqueGrayScale();
}


void vmxAppProcessImage_Mask::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessImage_ExtractCenterlines::vmxAppProcessImage_ExtractCenterlines()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Extract Centerlines");
    this->GetConfig()->SetTitleLong("Extract Centerlines");
}


void vmxAppProcessImage_ExtractCenterlines::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
//            char const *input_string = tinyfd_inputBox("Enter Threshold value as INT number", "", "");
//            if (!input_string) return ;
//
//            int threshold = std::stoi(input_string);
//
//            uint64_t min_value_included, max_value_included;
//            img.GetVoxelValueFullRange(min_value_included, max_value_included);
//            if(threshold<min_value_included || threshold>max_value_included) return;
            
            mxString name;
            //name.Assign("m_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            
            mxBIP bip;
            //if(!bip.DistanceTransformForSphere(*vimg16, *vimg16, *vimg16_2)) cout<<"mxBIP::DistanceTransformForSphere() returned 0 (fail)!"<<endl;
            
            vmxImage pv;
            if(!bip.ProfileVolumeTransformForSphere(*image, *image, pv))
            {
                std::cout<<std::endl<<"vmxAppProcessImage_ExtractCenterlines::Execute() error: mxBIP::ProfileVolumeTransformForSphere() returned 0 (fail)!";
                this->Abort();
                return;
            }
            
            mxSkeletonization skeletonization;
            vmxImage skl;
            skeletonization.OrderedSkeletonization(pv, skl);
            
            
            
            
            vmxGraph g_temp;
            vmxGraph *graph = static_cast< vmxGraph* > (this->GetAppMainWidget()->GetDataListWidget()->Create(g_temp.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!graph) return;
            
            mxGraphProcessing gp;
            gp.CreateGraphFromImage(skl, (*graph));
            
            
            graph->Update();
            graph->SetTimeIndex(0);
            //graph->SetVisibility(1);
            
        }
    }

}


void vmxAppProcessImage_ExtractCenterlines::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessImage_ExtractLargestCC::vmxAppProcessImage_ExtractLargestCC()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Extract Largest CC");
    this->GetConfig()->SetTitleLong("Extract Largest CC");
}


void vmxAppProcessImage_ExtractLargestCC::Execute()
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
                std::cout<<std::endl<<"vmxAppProcessImage_ExtractLargestCC::Execute() error: mxBIP::ExtractLargestConnectedComponent26() returned 0 (fail)!";
                this->Abort();
                return;
            }
            
            extracted->SetMappingToOpaqueGrayScale();
        }
    }
}


void vmxAppProcessImage_ExtractLargestCC::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------



vmxAppProcessImage_Filter_4_Neighbors::vmxAppProcessImage_Filter_4_Neighbors()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Filter 4 neighbors");
    this->GetConfig()->SetTitleLong("Filter 4 neighbors");
}


void vmxAppProcessImage_Filter_4_Neighbors::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("f4_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxImage *output_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!output_image) return;
            
            output_image->SetPropertiesAs(image);
            
            mxBIP bip;
            bip.FilterWith_4_Neghborhood(*image, *output_image);
            
            output_image->SetMappingToOpaqueGrayScale();
        }
    }
}


void vmxAppProcessImage_Filter_4_Neighbors::StartInMainThread()
{
    this->Execute();
}






//-------------------------------------------------------------------------------------------------------------------------



vmxAppProcessImage_ProcessPhaseContrast::vmxAppProcessImage_ProcessPhaseContrast()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Process PC");
    this->GetConfig()->SetTitleLong("Process PC");
}


void vmxAppProcessImage_ProcessPhaseContrast::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("ppc_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxImage *output_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!output_image) return;
            
            output_image->SetPropertiesAs(image);
            

            // BODY
            vmxImage output_body_img;
            {
                // Set the size of output;
                output_body_img.SetDimensions(1,image->GetDimension_S(),image->GetDimension_R(),image->GetDimension_C());
                output_body_img.SetPropertiesAs(image);
                output_body_img.FillInWith(255);
                
                // Calculate venc zelo level pixel value and range of values
                //unsigned int pixel_value_for_venc_value_zero = 2048;
                //this->VencZeroLevelPixelValueForPhaseContrastImage(pc, pixel_value_for_venc_value_zero);
                uint64_t min, max;
                image->GetVoxelMinAndMaxValues(min,max);
                
                //int range = max - min;
                //if(range<=0) return 0;
                
                
                for(unsigned int s=0; s<image->GetDimension_S(); s++)
                {
                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
                    {
                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
                        {
                            for(unsigned int t=1; t<image->GetDimension_T(); t++)
                            {
                                int value_t0 = image->operator()(t-1,s,r,c);
                                int value_t1 = image->operator()(t,s,r,c);
                                
                                // Calculate existing difference in voxel value between consequitive time moments
                                int diff_old = value_t1 - value_t0;
                                if(diff_old<0) diff_old = -diff_old;
                                
                                // Calculate possible difference in voxel value between consequitive time moments if the second value is corrected
                                int value_t1_corrected;
                                //--- 1.case ---
                                if((value_t1-min) < max-value_t1) { value_t1_corrected = max + (value_t1-min); }
                                //--- 2.case ---
                                else { value_t1_corrected = min - (max - value_t1); }
                                
                                // Calculate corrected value difference in voxel value between consequitive time moments
                                int diff_corrected = value_t1_corrected - value_t0;
                                if(diff_corrected<0) diff_corrected = -diff_corrected;
                                
                                //Finally, if the corrected difference is smaller than the old one, assign the thresholded image pixel value 0.
                                if(diff_corrected < diff_old)
                                {
                                    output_body_img(0,s,r,c) = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            //output_image->Copy(&output_body_img);
            
            
            
            vmxImage magnitude_image;
            
            {
                //bdImage body_img;
                //if(!this->BodyMaskFromPC_Multiple2DTime(pc, body_img)) return 0;

                unsigned int pixel_value_for_venc_value_zero = 2048;
                //this->VencZeroLevelPixelValueForPhaseContrastImage(pc, pixel_value_for_venc_value_zero);

                // Set the size of output;
                magnitude_image.SetDimensionsAndPropertiesAs(image);
                magnitude_image.FillInWith(0);


                for(unsigned int s=0; s<image->GetDimension_S(); s++)
                {
                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
                    {
                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
                        {
                            for(unsigned int t=0; t<image->GetDimension_T(); t++)
                            {
                                if(output_body_img(s,r,c)!=0)
                                {
                                    int diff = image->operator()(t,s,r,c) - pixel_value_for_venc_value_zero;
                                    if(diff<0) diff = -diff;
                                    //if()
                                    magnitude_image(t,s,r,c) = diff;
                                }
                                else { magnitude_image(t,s,r,c)=0; }
                            }
                        }
                    }
                }
                
                int threshold = 600;
                int squared_radius_of_SE = 4;//-1;//4;

                
                
                
                if(threshold>0)
                {
                    vmxImage thr_img;
                    mxGIP gip;
                    gip.Threshold(magnitude_image, threshold, thr_img);
                    //mxBIP bip;
                    //bip.Threshold(magnitude_image, threshold, thr_img);

                    if(squared_radius_of_SE>0)
                    {
                        vmxImage dil_img, dil_er_img, dil_er_dil_img;
                        
                        mxBIP bip;
                        bip.DilateWithSphereSE(thr_img, dil_img, squared_radius_of_SE);
                        bip.ErodeWithSphereSE(dil_img, dil_er_img, squared_radius_of_SE*4);
                        bip.DilateWithSphereSE(dil_er_img, dil_er_dil_img, squared_radius_of_SE);

                        for(unsigned int s=0; s<image->GetDimension_S(); s++)
                        {
                            for(unsigned int r=0; r<image->GetDimension_R(); r++)
                            {
                                for(unsigned int c=0; c<image->GetDimension_C(); c++)
                                {
                                    for(unsigned int t=0; t<image->GetDimension_T(); t++)
                                    {
                                        if(dil_er_dil_img(t,s,r,c)!=0)
                                        {
                                            magnitude_image(t,s,r,c) = thr_img(t,s,r,c);
                                        }
                                        else { magnitude_image(t,s,r,c)=0; }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        magnitude_image.Copy(&thr_img);
                    }
                }
            }
            
            //output_image->Copy(&magnitude_image);

            
            
            vmxImage output_img;
            vmxImage seg_3d_img;
            {
                

                output_img.SetDimensionsAndPropertiesAs(image);
                output_img.FillInWith(0);

                for(unsigned int t=0; t<image->GetDimension_T(); t++)
                {
                    for(unsigned int s=0; s<image->GetDimension_S(); s++)
                    {
                        for(unsigned int r=0; r<image->GetDimension_R(); r++)
                        {
                            for(unsigned int c=0; c<image->GetDimension_C(); c++)
                            {
                                if(magnitude_image(t,s,r,c)!=0)
                                {
                                    unsigned int value = 1;
                                    for(unsigned int t2=0; t2<t; t2++)
                                    {
                                        if(output_img(t2,s,r,c)!=0)
                                        {
                                            value = t2+1;
                                            break;
                                        }
                                    }
                                    output_img(t,s,r,c) = value;
                                }
                            }
                        }
                    }
                }

                // Create the 3D segmented image incorporating voxles in space over all times
                
                seg_3d_img.SetDimensions(1,image->GetDimension_S(),image->GetDimension_R(),image->GetDimension_C());
                seg_3d_img.SetPropertiesAs(image);
                seg_3d_img.FillInWith(0);

                for(unsigned int t=0; t<image->GetDimension_T(); t++)
                {
                    for(unsigned int s=0; s<image->GetDimension_S(); s++)
                    {
                        for(unsigned int r=0; r<image->GetDimension_R(); r++)
                        {
                            for(unsigned int c=0; c<image->GetDimension_C(); c++)
                            {
                                if(output_img(t,s,r,c)!=0)
                                {
                                    seg_3d_img(s,r,c) = output_img(t,s,r,c);
                                }
                            }
                        }
                    }
                }
            }
            
            
            vmxImage steps_img;
            {
                // Create a 3D image (corresponding to the seg_3d_img) with voxel values indicating earliest time of appearance of a given voxel region.
                
                steps_img.SetDimensions(1,image->GetDimension_S(),image->GetDimension_R(),image->GetDimension_C());
                steps_img.SetPropertiesAs(image);
                steps_img.FillInWith(0);

                mxGeometry g;
                g.SetDimensions(image->GetDimension_S(),image->GetDimension_R(),image->GetDimension_C());

                for(unsigned int t=0; t<image->GetDimension_T(); t++)
                {
                    for(unsigned int s=0; s<image->GetDimension_S(); s++)
                    {
                        for(unsigned int r=0; r<image->GetDimension_R(); r++)
                        {
                            for(unsigned int c=0; c<image->GetDimension_C(); c++)
                            {
                                if(seg_3d_img(s,r,c)==t+1)
                                {
                                    int is_border_found = 0;
                                    for(int radius=0; radius<g.GetMaxSphereSquaredRadius() && (!is_border_found); radius++)
                                    {
                                        int sn,rn,cn;
                                        for(g.ForSphere(s,r,c, radius); g.GetSphere(radius+1, sn,rn,cn); )
                                        {
                                            if(seg_3d_img(sn,rn,cn)!=0 && steps_img(sn,rn,cn)==0)
                                            {
                                                steps_img(sn,rn,cn) = t+1;
                                            }
                                            else is_border_found = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            output_image->Copy(&steps_img);

            output_image->SetMappingToOpaqueGrayScale();
            
        }
    }
}


void vmxAppProcessImage_ProcessPhaseContrast::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessImage_GraphReportXTK::vmxAppProcessImage_GraphReportXTK()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Report Graph");
    this->GetConfig()->SetTitleLong("Report Graph");
}


void vmxAppProcessImage_GraphReportXTK::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);

    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxGraph graph_temp;
        if(selected_data_objects[i]->GetClassName()==graph_temp.GetClassName())
        {
            mxString name;
            name.Assign("f4_");
            name.Append(selected_data_objects[i]->GetObjectName());

            vmxGraph *graph = static_cast<vmxGraph*>(selected_data_objects[i]);
            if(!graph) return;
            
            char const * extension_filter[2] = { "*.htm", "*.html" };
            char const *save_file_name = tinyfd_saveFileDialog("Save Report", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppProcessImage_GraphReportXTK::Execute(): save_file_name is NULL";
                return ;
            }

            
            vmxAppReportGenerator rg;
            
            rg.Create_XTK_ReportForGraph(graph, save_file_name);

        }
    }
    
//    vmxAppReportGenerator rg;
//    vmxGraph graph;
//
//    mxGraphVertex *v1 =  graph.AddVertex(10, 0,0);
//    mxGraphVertex *v2 =  graph.AddVertex(0, 0,10);
//    mxGraphVertex *v3 =  graph.AddVertex(0, 10,0);
//    mxGraphVertex *v4 =  graph.AddVertex(0, 10,10);
//    graph.AddEdge(v1, v2);
//    graph.AddEdge(v1, v4);
//    graph.AddEdge(v2, v3);
//    graph.AddEdge(v3, v4);
//    graph.AddEdge(v1, v3);
//    graph.Update();
//
//
//    graph.GetObjectName().Assign("Centerline");
//    rg.Create_XTK_ReportForGraph(&graph, "/Users/danilobabin/try_report");
}


void vmxAppProcessImage_GraphReportXTK::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppProcessImage_SkeletonReportXTK::vmxAppProcessImage_SkeletonReportXTK()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Report Skeleton");
    this->GetConfig()->SetTitleLong("Report Skeleton");
}


void vmxAppProcessImage_SkeletonReportXTK::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxSkeleton temp;
        if(selected_data_objects[i]->GetClassName()==temp.GetClassName())
        {
            vmxSkeleton *skl = static_cast<vmxSkeleton*>(selected_data_objects[i]);
            if(!skl) return;
            
            char const * extension_filter[2] = { "*.htm", "*.html" };
            char const *save_file_name = tinyfd_saveFileDialog("Save Report", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if(!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppProcessImage_SkeletonReportXTK::Execute(): save_file_name is NULL";
                return ;
            }
            
            vmxAppReportGenerator rg;
            
            rg.Create_XTK_ReportForSkeleton(skl, save_file_name);
            
        }
    }
}


void vmxAppProcessImage_SkeletonReportXTK::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------





vmxAppProcessImage_OrderedSkeletonization::vmxAppProcessImage_OrderedSkeletonization()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Ordered Skeletonization");
    this->GetConfig()->SetTitleLong("Ordered Skeletonization");
}


void vmxAppProcessImage_OrderedSkeletonization::Execute()
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

            vmxImage *output_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!output_image) return;

            output_image->SetPropertiesAs(image);
            
            vmxImage pv;
            
            mxBIP bip;
            bip.ProfileVolumeTransformForSphere(*image, *image, pv);

            mxSkeletonization skl;
            skl.OrderedSkeletonization(pv, *output_image);
            
            output_image->SetMappingToOpaqueGrayScale();
        }
    }
}


void vmxAppProcessImage_OrderedSkeletonization::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessImage_CreateGraphFromImage::vmxAppProcessImage_CreateGraphFromImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Create Graph From Image");
    this->GetConfig()->SetTitleLong("Create Graph From Image");
}


void vmxAppProcessImage_CreateGraphFromImage::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("grp_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxGraph g_temp;
            vmxGraph *graph = static_cast< vmxGraph* > (this->GetAppMainWidget()->GetDataListWidget()->Create(g_temp.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!graph) return;
            
            
            mxGraphProcessing gp;
            gp.CreateGraphFromImage(*image, (*graph));
            
            graph->Update();
            graph->SetTimeIndex(0);

        }
    }
}


void vmxAppProcessImage_CreateGraphFromImage::StartInMainThread()
{
    this->Execute();
}



//-------------------------------------------------------------------------------------------------------------------------




vmxAppProcessImage_FilterNodeMultiPahts::vmxAppProcessImage_FilterNodeMultiPahts()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Image>>Filter Graph Node Multi Pahts");
    this->GetConfig()->SetTitleLong("Filter Graph Node Multi Pahts");
}


void vmxAppProcessImage_FilterNodeMultiPahts::Execute()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            mxString name;
            name.Assign("fmp_");
            name.Append(selected_data_objects[i]->GetObjectName());
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(!image) return;
            
            vmxImage *output_image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(img.GetClassName().Get_C_String(), name.Get_C_String()));
            if(!output_image) return;
            
            output_image->SetPropertiesAs(image);
            
            mxSkeletonization skl;
            skl.FilterMultipathsInNodeConnectedComponents(*image, *output_image);
            
            output_image->SetMappingToOpaqueGrayScale();
        }
    }
}


void vmxAppProcessImage_FilterNodeMultiPahts::StartInMainThread()
{
    this->Execute();
}



