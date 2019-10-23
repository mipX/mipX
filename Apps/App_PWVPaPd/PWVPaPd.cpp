/*=========================================================================
 
 Program:   mipx
 Module:    PWVPaPd.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "PWVPaPd.h"





//-------------------------------------------------------------------------------------------------------------------------



PWVPaPd_LoadColorPNGImage::PWVPaPd_LoadColorPNGImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("PWVPaPd>>Load PNG PaPd FFR recording");
    this->GetConfig()->SetTitleLong("Load PNG PaPd FFR recording");
}


void PWVPaPd_LoadColorPNGImage::Execute()
{
    vmxImage temp;
    mxDataObject *obj1 = this->GetAppMainWidget()->GetDataListWidget()->GetDataObject("Image1",temp.GetClassName().Get_C_String());
    mxDataObject *obj2 = this->GetAppMainWidget()->GetDataListWidget()->GetDataObject("Image2",temp.GetClassName().Get_C_String());
    if(!obj1 || !obj2)
    {
        this->Abort();
        return;
    }
    
    vmxAppViewer *v1 = this->GetAppMainWidget()->GetViewer("ImageViewer1");
    vmxAppViewer *v2 = this->GetAppMainWidget()->GetViewer("ImageViewer2");
    if(!v1 || !v2)
    {
        this->Abort();
        return;
    }
    
    vmxAppImageViewer *iv1 = static_cast< vmxAppImageViewer* >(v1);
    vmxAppImageViewer *iv2 = static_cast< vmxAppImageViewer* >(v2);
    if(!iv1 || !iv2)
    {
        this->Abort();
        return;
    }
    
    char const * extension_filter[2] = { "*.png", "*.PNG" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read PNG PaPd FFR image file", "", 2, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"PWVPaPd_LoadColorPNGImage::StartInMainThread(): file_name_to_open is NULL.";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    if (list_of_File_names.IsEmpty())
    {
        std::cout<<std::endl<<"PWVPaPd_LoadColorPNGImage::StartInMainThread(): list_of_File_names is empty.";
        return ;
    }
    
    //mxListIterator< mxString > it;
    //for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
//    {
//        mxString dir, name, ext;
//        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
//
//        vmxImage temp;
//        if(!temp.LoadVTKFile(it.GetElementAddress()->Get_C_String()))
//        {
//            tinyfd_messageBox("Error", "Open VTK file failed", "ok", "error", 1);
//            return ;
//        }
//
//        vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
//        image->Copy(&temp);
//    }

    
    vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetFileName(list_of_File_names[0].Get_C_String());
    reader->Update();
    
    vtkSmartPointer<vtkImageExtractComponents> extract_red_filter = vtkSmartPointer<vtkImageExtractComponents>::New();
    extract_red_filter->SetInputConnection(reader->GetOutputPort());
    extract_red_filter->SetComponents(0);
    extract_red_filter->Update();
    
    
    vtkSmartPointer<vtkImageExtractComponents> extract_green_filter = vtkSmartPointer<vtkImageExtractComponents>::New();
    extract_green_filter->SetInputConnection(reader->GetOutputPort());
    extract_green_filter->SetComponents(1);
    extract_green_filter->Update();
    
    
    vtkSmartPointer< vtkImageData > image_red = vtkSmartPointer< vtkImageData >::New();
    image_red->DeepCopy(extract_red_filter->GetOutput());
    
    vtkSmartPointer< vtkImageData > image_green = vtkSmartPointer< vtkImageData >::New();
    image_green->DeepCopy(extract_green_filter->GetOutput());
    

    
    vmxImage *img1 = static_cast<vmxImage*>(obj1);
    vmxImage *img2 = static_cast<vmxImage*>(obj2);
    
    img1->SetDimensions(1,1,image_red->GetDimensions()[1], image_red->GetDimensions()[0]);
    {
        int i=0;
        for(int r=0; r<img1->GetDimension_R(); r++)
        {
            for(int c=0; c<img1->GetDimension_C(); c++)
            {
                img1->operator()(r,c) = *((unsigned short*) image_red->GetScalarPointer(c, r, 0));
                i++;
            }
        }
    }
    
    img2->SetDimensions(1,1,image_green->GetDimensions()[1], image_green->GetDimensions()[0]);
    {
        int i=0;
        for(int r=0; r<img2->GetDimension_R(); r++)
        {
            for(int c=0; c<img2->GetDimension_C(); c++)
            {
                img2->operator()(r,c) = *((unsigned short*) image_green->GetScalarPointer(c, r, 0));
                i++;
            }
        }
    }
    
    iv1->GetRenderer()->SetImageData(img1->GetVTKImageData(0));// image_red);
    iv1->GetRenderer()->SetDirectionToTransversal();
    iv1->GetRenderer()->SetMappingToGrayScale();
    iv1->GetRenderer()->FitImageToScreen();

    iv2->GetRenderer()->SetImageData(img2->GetVTKImageData(0));// image_red);
    iv2->GetRenderer()->SetDirectionToTransversal();
    iv2->GetRenderer()->SetMappingToGrayScale();
    iv2->GetRenderer()->FitImageToScreen();

}


void PWVPaPd_LoadColorPNGImage::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------




PWVPaPd_CreatePaCurve::PWVPaPd_CreatePaCurve()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("PWVPaPd>>Create Pa Curve");
    this->GetConfig()->SetTitleLong("Create Pa Curve");
}


void PWVPaPd_CreatePaCurve::Execute()
{
    vmxImage temp;
    mxDataObject *obj = this->GetAppMainWidget()->GetDataListWidget()->GetDataObject("Image1",temp.GetClassName().Get_C_String());
    if(!obj)
    {
        this->Abort();
        return;
    }
    
    vmxImage *img = static_cast<vmxImage*>(obj);
    
    vmxAppViewer *v = this->GetAppMainWidget()->GetViewer("ImageViewer1");
    if(!v)
    {
        this->Abort();
        return;
    }
    
    vmxAppImageViewer *iv = static_cast< vmxAppImageViewer* >(v);
    if(!iv)
    {
        this->Abort();
        return;
    }
    
    
    std::cout<<" "<<iv->GetRenderer()->GetPickedIndexes()[0]<<", "<<iv->GetRenderer()->GetPickedIndexes()[1]<<", "<<iv->GetRenderer()->GetPickedIndexes()[2];
    
    mxGeometry g;
    g.SetDimensions(img->GetDimension_S(), img->GetDimension_R(), img->GetDimension_C());
    int value = 0;
    for(unsigned int radius = 0; radius<300 && !value; radius++)
    {
        int rn,cn;
        for(g.ForCircle(iv->GetRenderer()->GetPickedIndexes()[1], iv->GetRenderer()->GetPickedIndexes()[0],radius); g.GetCircle(radius+1, rn,cn); )
        {
            if(img->operator()(0,rn,cn) >= 100 )
            {
                value = img->operator()(0,rn,cn);
                break;
            }
        }
    }
    
    std::cout<<std::endl<<" value = "<<value<<std::endl;
    
    if(!value)
    {
        this->Abort();
        return;
    }
    
    vmxCurve temp_curve;
    vmxCurve *curve = static_cast<vmxCurve*> (this->GetAppMainWidget()->GetDataListWidget()->Create(temp_curve.GetClassName().Get_C_String(),"Pa"));
    
    mxList<int> list_c, list_r;
    for( int c=0; c<img->GetDimension_C(); c++)
    {
        for( int r=0; r<img->GetDimension_R(); r++)
        {
            if(img->operator()(r,c) == value )
            {
                list_c.AddToEnd(c);
                list_r.AddToEnd(r);
            }
        }
    }
    
    curve->SetNumberOfSamples(list_c.GetNumberOfElements());
    mxListIterator<int> itc, itr;
    int i=0;
    for(itc.SetToBegin(list_c), itr.SetToBegin(list_r), i=0; itc.IsValid() && itr.IsValid(); itc.MoveToNext(), itr.MoveToNext(), i++)
    {
        curve->X(i) = itc.GetElement();
        curve->Y(i) = itr.GetElement();
    }
    
    curve->SetColor(255, 0,0,1);
    
    curve->Update();
}


void PWVPaPd_CreatePaCurve::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------




PWVPaPd_CreatePdCurve::PWVPaPd_CreatePdCurve()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("PWVPaPd>>Create Pd Curve");
    this->GetConfig()->SetTitleLong("Create Pd Curve");
}


void PWVPaPd_CreatePdCurve::Execute()
{
    vmxImage temp;
    mxDataObject *obj = this->GetAppMainWidget()->GetDataListWidget()->GetDataObject("Image2",temp.GetClassName().Get_C_String());
    if(!obj)
    {
        this->Abort();
        return;
    }
    
    vmxImage *img = static_cast<vmxImage*>(obj);
    
    vmxAppViewer *v = this->GetAppMainWidget()->GetViewer("ImageViewer2");
    if(!v)
    {
        this->Abort();
        return;
    }
    
    vmxAppImageViewer *iv = static_cast< vmxAppImageViewer* >(v);
    if(!iv)
    {
        this->Abort();
        return;
    }
    
    
    std::cout<<" "<<iv->GetRenderer()->GetPickedIndexes()[0]<<", "<<iv->GetRenderer()->GetPickedIndexes()[1]<<", "<<iv->GetRenderer()->GetPickedIndexes()[2];
    
    mxGeometry g;
    g.SetDimensions(img->GetDimension_S(), img->GetDimension_R(), img->GetDimension_C());
    int value = 0;
    for(unsigned int radius = 0; radius<300 && !value; radius++)
    {
        int rn,cn;
        for(g.ForCircle(iv->GetRenderer()->GetPickedIndexes()[1], iv->GetRenderer()->GetPickedIndexes()[0],radius); g.GetCircle(radius+1, rn,cn); )
        {
            if(img->operator()(0,rn,cn) >= 100 )
            {
                value = img->operator()(0,rn,cn);
                break;
            }
        }
    }
    
    std::cout<<std::endl<<" value = "<<value<<std::endl;
    
    if(!value)
    {
        this->Abort();
        return;
    }
    
    vmxCurve temp_curve;
    vmxCurve *curve = static_cast<vmxCurve*> (this->GetAppMainWidget()->GetDataListWidget()->Create(temp_curve.GetClassName().Get_C_String(),"Pd"));
    
    mxList<int> list_c, list_r;
    for( int c=0; c<img->GetDimension_C(); c++)
    {
        for( int r=0; r<img->GetDimension_R(); r++)
        {
            if(img->operator()(r,c) == value )
            {
                list_c.AddToEnd(c);
                list_r.AddToEnd(r);
            }
        }
    }
    
    curve->SetNumberOfSamples(list_c.GetNumberOfElements());
    mxListIterator<int> itc, itr;
    int i=0;
    for(itc.SetToBegin(list_c), itr.SetToBegin(list_r), i=0; itc.IsValid() && itr.IsValid(); itc.MoveToNext(), itr.MoveToNext(), i++)
    {
        curve->X(i) = itc.GetElement();
        curve->Y(i) = itr.GetElement();
    }
    
    curve->SetColor(0,255,0, 1);
    
    curve->Update();
}


void PWVPaPd_CreatePdCurve::StartInMainThread()
{
    this->Execute();
}




//-------------------------------------------------------------------------------------------------------------------------
