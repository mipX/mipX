/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppDataLoader.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppDataLoader.h"



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadProfile::vmxAppDataLoader_LoadProfile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Profile");
    this->GetConfig()->SetTitleLong("Load Profile");
}


void vmxAppDataLoader_LoadProfile::StartInMainThread()
{
    char const * extension_filter[2] = { "*.prf", "*.PRF" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read Profile file(s)", "", 2, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadProfile::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    mxListIterator< mxString > it;
    for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString dir, name, ext;
        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        vmxProfile temp;
//        if(!temp.LoadFromFile(it.GetElementAddress()->Get_C_String()))
//        {
//
//            tinyfd_messageBox("Error", "Open Profile file failed", "ok", "error", 1);
//            return ;
//        }
        
        vmxProfile *pr = static_cast< vmxProfile* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
        //pr->Copy(&temp);
        //pr->Update();
        
        if(!pr->LoadFromFile(it.GetElementAddress()->Get_C_String()))
        {
            
            tinyfd_messageBox("Error", "Open Profile file failed", "ok", "error", 1);
            return ;
        }
        pr->Update();
        
    }
    
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadPointList::vmxAppDataLoader_LoadPointList()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>PointList");
    this->GetConfig()->SetTitleLong("Load PointList");
}


void vmxAppDataLoader_LoadPointList::StartInMainThread()
{
    char const * extension_filter[2] = { "*.psl", "*.PSL" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read PointList file(s)", "", 2, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadPointList::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    mxListIterator< mxString > it;
    for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString dir, name, ext;
        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        vmxPointList temp;
        if(!temp.LoadFromPSLFile(it.GetElementAddress()->Get_C_String()))
        {
            
            tinyfd_messageBox("Error", "Open PointList file failed", "ok", "error", 1);
            return ;
        }
        
        vmxPointList *pl = static_cast< vmxPointList* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
        pl->Copy(&temp);
        pl->Update();
    }
    
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadSkeleton::vmxAppDataLoader_LoadSkeleton()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Skeleton");
    this->GetConfig()->SetTitleLong("Load Skeleton");
}


void vmxAppDataLoader_LoadSkeleton::StartInMainThread()
{
    char const * extension_filter[4] = { "*.skl", "*.txt", "*.SKL", "*.TXT" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read Skeleton file(s)", "", 4, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadSkeleton::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    mxListIterator< mxString > it;
    for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString dir, name, ext;
        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        vmxSkeleton skl;
        if(!skl.LoadFromTextFileOfPositions(it.GetElementAddress()->Get_C_String()))
        {
            
            tinyfd_messageBox("Error", "Open Skeleton file failed", "ok", "error", 1);
            return ;
            
        }
        
        vmxSkeleton *skeleton = static_cast< vmxSkeleton* > (this->GetAppMainWidget()->GetDataListWidget()->Create(skl.GetClassName().Get_C_String(), name.Get_C_String()));
        skeleton->CopyFrom(&skl);
        skeleton->Update();
    }
    
}





//-------------------------------------------------------------------------------------------------------------------------


#if defined(MIPX_BUILD_LOADER_DICOM)

vmxAppDataLoader_LoadDicom::vmxAppDataLoader_LoadDicom()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Dicom");
    this->GetConfig()->SetTitleLong("Load Dicom");
}


void vmxAppDataLoader_LoadDicom::StartInMainThread()
{
    xmxLoaderDicom loader;

    //char const * extension_filter[2] = { "*.vtk", "*.VTK" };
    const char *folder_name_to_open = tinyfd_selectFolderDialog("Read DICOM folder", "");

    if (! folder_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadDicom::StartInMainThread(): folder_name_to_open is NULL";
        return ;
    }




    if(loader.DicomDirectoryToTextFileDirectoryTree(folder_name_to_open)){};


    char const *extension_filter[2] = { "*.mip", "*.MIP" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read MIP list file of dicom image series", loader.m_mipx_path_directory.Get_C_String(), 2, extension_filter, NULL, 0);

    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadDicom::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }

    mxString s, dir, name, ext;
    s.Assign(file_name_to_open);
    s.ExtractFilePathParts(dir, name, ext);


    vmxImage temp_image;

    if(!loader.LoadSeriesFromDicomFileListFile(file_name_to_open, &temp_image))
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadDicom::StartInMainThread(): error reading series.";
        return ;
    }
    vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp_image.GetClassName().Get_C_String(), name.Get_C_String()));
    image->Copy(&temp_image);
    
    image->SetMappingToOpaqueGrayScale();
    

    
    
//    vtkSmartPointer< vtkStringArray > files = vtkSmartPointer< vtkStringArray >::New();
//
//    // Copy the list of files into vtk array
//    //files->SetNumberOfValues(1);
//    files->InsertNextValue("/Users/danilobabin/Dropbox/-DIP_IMAGES/Aorta/From_Laura_and_Dan/PWV/MFS002/12DEC83_064A45_1.MR.0062.0012.2018.03.22.09.59.03.666074.148779588.IMA");
//    //files->SetValue(1, "");
//
//
//
//
//
//    vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create("vmxImage16U", "try"));
//
//    {
//        //vtkSmartPointer<vtkGDCMImageReader2> vtk_reader = vtkSmartPointer<vtkGDCMImageReader2>::New();
//
//        vtkSmartPointer<vtkDICOMImageReader> vtk_reader = vtkSmartPointer<vtkDICOMImageReader>::New();
//
//        //vtk_reader->GetFileNames()->SetNumberOfValues(1);
//        //vtk_reader->GetFileNames()->SetValue(0, "/Users/danilobabin/Dropbox/-DIP_IMAGES/Aorta/From_Laura_and_Dan/PWV/MFS002/12DEC83_064A45_1.MR.0062.0012.2018.03.22.09.59.03.666074.148779588.IMA");
//
//        //vtk_reader->SetFileNames(files);
//        vtk_reader->SetFileName("/Users/danilobabin/-Dip_Images/test coregist ghent/case 107/Coronary angio/CD1-DVD from radiology  TEST QUALITY/DICOM/00000001/00000001/00000001");
//        //vtk_reader->SetFileName("/Users/danilobabin/-Dip_Images/prostate.IMG");
//        vtk_reader->Update();
//
//        std::cout<<std::endl<<" dims= "<<vtk_reader->GetOutput()->GetDimensions()[0]<<", "<<vtk_reader->GetOutput()->GetDimensions()[1]<<", "<<vtk_reader->GetOutput()->GetDimensions()[2];
//
//
//        // Copy image
//        image->SetDimensions(1, vtk_reader->GetOutput()->GetDimensions()[2], vtk_reader->GetOutput()->GetDimensions()[1], vtk_reader->GetOutput()->GetDimensions()[0]);
////        double *ptr = static_cast<double *>(vtk_reader->GetOutput()->GetScalarPointer(0, 0, 0));
////        for(int i=0; i<image->GetNumberOfDataElements(); i++)
////        {
////            (*image)[i] = ptr[i];
////        }
//
//        //vtk_reader->GetOutput()->GetScalar
//
//        double min = 999999, max=-999999;
//        //int min = 999999, max=-999999;
//        for(int s=0; s<image->GetDimension_S(); s++)
//        {
//            for(int r=0; r<image->GetDimension_R(); r++)
//            {
//                for(int c=0; c<image->GetDimension_C(); c++)
//                {
//                    double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));
//                    //int v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));
//                    //double v =  *(static_cast<double *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));
//                    if(min > v) min = v;
//                    if(max < v) max = v;
//                }
//            }
//        }
//
//        double range = max - min + 1;
//        //int range = max - min + 1;
//
//        std::cout<<std::endl<<"  min = "<<min<<",   max = "<<max<<",  range = "<<range;
//
//        for(int s=0; s<image->GetDimension_S(); s++)
//        {
//            for(int r=0; r<image->GetDimension_R(); r++)
//            {
//                for(int c=0; c<image->GetDimension_C(); c++)
//                {
//                    double v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));;
//                    //int v =  *(static_cast<int *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));
//                    //double v =  *(static_cast<double *>(vtk_reader->GetOutput()->GetScalarPointer(c, r, s)));;
//                    image->operator()(s,r,c) = ((v-min)/range)*65535;
//                }
//            }
//        }
//
//
//        // Set image properties
//        image->SetOrigin(0, vtk_reader->GetOutput()->GetOrigin()[2], vtk_reader->GetOutput()->GetOrigin()[1], vtk_reader->GetOutput()->GetOrigin()[0]);
//        image->SetSpacing(1, vtk_reader->GetOutput()->GetSpacing()[2], vtk_reader->GetOutput()->GetSpacing()[1], vtk_reader->GetOutput()->GetSpacing()[0]);
//
//
//
//
////        // Copy tags
////        mxListIterator< mxImageSliceTag > it;
////        int i=0;
////        for(it.SetToBegin(list_of_tags), i=0; it.IsValid() && i<image->GetTag()->m_tags.GetNumberOfDataElements(); it.MoveToNext(), i++)
////        {
////            image->GetTag()->GetTag(i) = it.GetElement();
////        }
//
//    }
//    image->SetMappingToOpaqueGrayScale();
    
    

  
}

#endif




//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadVTKImage::vmxAppDataLoader_LoadVTKImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>VTK image");
    this->GetConfig()->SetTitleLong("Load VTK image");
}


void vmxAppDataLoader_LoadVTKImage::StartInMainThread()
{
    char const * extension_filter[2] = { "*.vtk", "*.VTK" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read VTK image file", "", 2, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadVTKImage::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    mxListIterator< mxString > it;
    for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString dir, name, ext;
        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        vmxImage temp;
        if(!temp.LoadVTKFile(it.GetElementAddress()->Get_C_String()))
        {
            tinyfd_messageBox("Error", "Open VTK file failed", "ok", "error", 1);
            return ;
        }
        
        vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
        image->Copy(&temp);
        
//        if(!image->LoadVTKFile(it.GetElementAddress()->Get_C_String()))
//        {
//            tinyfd_messageBox("Error", "Open VTK file failed", "ok", "error", 1);
//            image->SetDimensions(1,1,1,1);
//            return ;
//        }
        
        image->SetMappingToOpaqueGrayScale();
    }
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadPNGImages::vmxAppDataLoader_LoadPNGImages()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>PNG image");
    this->GetConfig()->SetTitleLong("Load PNG image");
}


void vmxAppDataLoader_LoadPNGImages::StartInMainThread()
{
    char const * extension_filter[2] = { "*.png", "*.PNG" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read PNG image file", "", 2, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadPNGImages::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    mxListIterator< mxString > it;
    for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString dir, name, ext;
        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        vtkSmartPointer<vtkStringArray> file_names = vtkSmartPointer<vtkStringArray>::New();
        file_names->InsertNextValue(it.GetElementAddress()->Get_C_String());
        
        vmxImage temp;
        if(!temp.LoadPNGImages(file_names))
        {
            tinyfd_messageBox("Error", "Open PNG file failed", "ok", "error", 1);
            return ;
        }
        
        vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
        image->Copy(&temp);
        
        //image->Update();
        image->SetMappingToOpaqueGrayScale();
        
    }
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadColorPNGImages::vmxAppDataLoader_LoadColorPNGImages()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Color (RGB) PNG image");
    this->GetConfig()->SetTitleLong("Load Color (RGB) PNG image");
}


void vmxAppDataLoader_LoadColorPNGImages::StartInMainThread()
{
    char const * extension_filter[2] = { "*.png", "*.PNG" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read Color (RGB) PNG image file", "", 2, extension_filter, NULL, 0);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadColorPNGImages::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    //mxList< mxString > list_of_File_names;
    //mxString s;
    //s.Assign(file_name_to_open);
    //s.ExtractStrings('|', list_of_File_names);
    
    //mxListIterator< mxString > it;
    //for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString st, dir, name, ext;
        st.Assign(file_name_to_open);
        st.ExtractFilePathParts(dir, name, ext);
        //it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        //vtkSmartPointer<vtkStringArray> file_names = vtkSmartPointer<vtkStringArray>::New();
        //file_names->InsertNextValue(it.GetElementAddress()->Get_C_String());
        
        vmxImage temp;
        if(!temp.LoadColorPNGImage(file_name_to_open))
        {
            tinyfd_messageBox("Error", "Open PNG file failed", "ok", "error", 1);
            return ;
        }
        
        vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
        image->Copy(&temp);
        
        //image->Update();
        image->SetMappingToOpaqueGrayScale();
        
    }
}



//-------------------------------------------------------------------------------------------------------------------------



#if defined(MIPX_BUILD_LOADER_MATLAB)


vmxAppDataLoader_LoadMatlabImage::vmxAppDataLoader_LoadMatlabImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Matlab image");//"File>>Load>>Matlab image");
    this->GetConfig()->SetTitleLong("Load Matlab image");
}


void vmxAppDataLoader_LoadMatlabImage::StartInMainThread()
{
    xmxLoaderMatlab loader_matlab;
    
    char const * extension_filter[2] = { "*.mat", "*.MAT" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read Matlab image file", "", 2, extension_filter, NULL, 0);
    
    if (! file_name_to_open)
    {
        //tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 1);
        std::cout<<std::endl<<"vmxAppDataLoader_LoadMatlabImage::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxString s, dir, name, ext;
    s.Assign(file_name_to_open);
    s.ExtractFilePathParts(dir, name, ext);
    
    vmxImage temp_image;
    
    if(!loader_matlab.LoadImage(file_name_to_open, NULL, &temp_image))
    {
        tinyfd_messageBox("Error", "Open Matlab file failed", "ok", "error", 1);
        return ;
        
    }

    
//    if(!loader_matlab.LoadImage(file_name_to_open, "I", &temp_image))
//    {
//        if(!loader_matlab.LoadImage(file_name_to_open, "II", &temp_image))
//        {
//            tinyfd_messageBox("Error", "Open Matlab file failed", "ok", "error", 1);
//            return ;
//        }
//    }
    
    
    vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp_image.GetClassName().Get_C_String(), name.Get_C_String()));
    image->Copy(&temp_image);
    
    
    image->SetMappingToOpaqueGrayScale();
    
}

#endif



//-------------------------------------------------------------------------------------------------------------------------

#if defined(MIPX_BUILD_LOADER_NIFTI)

vmxAppDataLoader_LoadNiftiImage::vmxAppDataLoader_LoadNiftiImage()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Nifti image");//"File>>Load>>Nifti image");
    this->GetConfig()->SetTitleLong("Load Nifti image");
}


void vmxAppDataLoader_LoadNiftiImage::StartInMainThread()
{
    xmxLoaderNifti loader_nifti;
    
    char const * extension_filter[2] = { "*.nii", "*.gz" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read Nifti image file", "", 2, extension_filter, NULL, 0);
    
    if (! file_name_to_open)
    {
        //tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 1);
        std::cout<<std::endl<<"vmxAppDataLoader_LoadNiftiImage::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxString s, dir, name, ext;
    s.Assign(file_name_to_open);
    s.ExtractFilePathParts(dir, name, ext);
    
    vmxImage temp_image;
    
    if(!loader_nifti.LoadImage(file_name_to_open, &temp_image))
    {
        {
            tinyfd_messageBox("Error", "Open Nifti file failed", "ok", "error", 1);
            return ;
        }
    }
    
    vmxImage *image = static_cast< vmxImage* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp_image.GetClassName().Get_C_String(), name.Get_C_String()));
    image->Copy(&temp_image);
    
    image->SetMappingToOpaqueGrayScale();
}

#endif



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadCurveFromRaw::vmxAppDataLoader_LoadCurveFromRaw()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Curve from raw");//"File>>Load>>Nifti image");
    this->GetConfig()->SetTitleLong("Load curve from raw");
}


void vmxAppDataLoader_LoadCurveFromRaw::StartInMainThread()
{
    char const * extension_filter[2] = { "*.dat", "*.DAT" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read curve raw file", "", 2, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadCurveFromRaw::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    mxListIterator< mxString > it;
    for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString dir, name, ext;
        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        vmxCurve temp;
        if(!temp.LoadRawFile(it.GetElementAddress()->Get_C_String()))
        {
            {
                tinyfd_messageBox("Error", "Open curve raw file failed", "ok", "error", 1);
                return ;
            }
        }
        
        vmxCurve *curve = static_cast< vmxCurve* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
        curve->Copy(&temp);
    }
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_LoadCurveFromMatlab::vmxAppDataLoader_LoadCurveFromMatlab()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Load>>Curve from Matlab");
    this->GetConfig()->SetTitleLong("Load curve from Matlab");
}


void vmxAppDataLoader_LoadCurveFromMatlab::StartInMainThread()
{
    char const * extension_filter[2] = { "*.m", "*.M" };
    const char *file_name_to_open = tinyfd_openFileDialog("Read curve Matlab file", "", 2, extension_filter, NULL, 1);
    
    if (! file_name_to_open)
    {
        std::cout<<std::endl<<"vmxAppDataLoader_LoadCurveFromMatlab::StartInMainThread(): file_name_to_open is NULL";
        return ;
    }
    
    mxList< mxString > list_of_File_names;
    mxString s;
    s.Assign(file_name_to_open);
    s.ExtractStrings('|', list_of_File_names);
    
    mxListIterator< mxString > it;
    for(it.SetToBegin(list_of_File_names); it.IsValid(); it.MoveToNext())
    {
        mxString dir, name, ext;
        it.GetElementAddress()->ExtractFilePathParts(dir, name, ext);
        
        vmxCurve temp;
        if(!temp.LoadMatlabFile(it.GetElementAddress()->Get_C_String()))
        {
            {
                tinyfd_messageBox("Error", "Open curve Matlab file failed", "ok", "error", 1);
                return ;
            }
        }
        
        vmxCurve *curve = static_cast< vmxCurve* > (this->GetAppMainWidget()->GetDataListWidget()->Create(temp.GetClassName().Get_C_String(), name.Get_C_String()));
        curve->Copy(&temp);
    }
}



//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_SaveImageToVTKFile::vmxAppDataLoader_SaveImageToVTKFile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>Image to VTK file");
    this->GetConfig()->SetTitleLong("Save image to VTK file");
}


void vmxAppDataLoader_SaveImageToVTKFile::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            char const * extension_filter[2] = { "*.vtk", "*.VTK" };
            char const *save_file_name = tinyfd_saveFileDialog("Save image to legacy VTK file", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SaveImageToVTKFile::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(image)
            {
                image->SaveToVTK16UFile(save_file_name);
                return;// MAKE SURE WE SAVE ONE IMAGE AT A TIME.
            }
        }
    }
}




//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_SaveImageToVTIFile::vmxAppDataLoader_SaveImageToVTIFile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>Image to VTI file");
    this->GetConfig()->SetTitleLong("Save image to VTI file");
}


void vmxAppDataLoader_SaveImageToVTIFile::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            char const * extension_filter[2] = { "*.vti", "*.VTI" };
            char const *save_file_name = tinyfd_saveFileDialog("Save image to VTI file", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SaveImageToVTIFile::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
    
            if(image)
            {
                image->SaveToVTIFile(save_file_name, image->GetScalarSizeInBytes());
                return;// MAKE SURE WE SAVE ONE IMAGE AT A TIME.
            }
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_SaveImageToPNGFiles::vmxAppDataLoader_SaveImageToPNGFiles()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>Image to PNG file(s)");
    this->GetConfig()->SetTitleLong("Save image to PNG file(s)");
}


void vmxAppDataLoader_SaveImageToPNGFiles::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            char const * extension_filter[2] = { "*.png", "*.PNG" };
            char const *save_file_name = tinyfd_saveFileDialog("Save image to PNG file(s)", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SaveImageToPNGFiles::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(image)
            {
                //std::cout<<std::endl<<"image->GetScalarSizeInBytes() = "<<image->GetScalarSizeInBytes();
                image->SaveToPNGImages(save_file_name, image->GetScalarSizeInBytes());// SaveToPNG8UImages(save_file_name);//, image->GetScalarSizeInBytes());
                return;// MAKE SURE WE SAVE ONE IMAGE AT A TIME.
            }
        }
    }
}




//-------------------------------------------------------------------------------------------------------------------------







#if defined(MIPX_BUILD_LOADER_MATLAB)


vmxAppDataLoader_SaveImageToMATFile::vmxAppDataLoader_SaveImageToMATFile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>Image to MAT file");
    this->GetConfig()->SetTitleLong("Save image to MAT file");
}


void vmxAppDataLoader_SaveImageToMATFile::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxImage img;
        if(selected_data_objects[i]->GetClassName()==img.GetClassName())
        {
            char const * extension_filter[2] = { "*.mat", "*.MAT" };
            char const *save_file_name = tinyfd_saveFileDialog("Save image to Matlab MAT file", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SaveImageToMATFile::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxImage *image = static_cast<vmxImage*>(selected_data_objects[i]);
            if(image)
            {
                xmxLoaderMatlab l;
                l.SaveImage(save_file_name,image,"I");
                return;// MAKE SURE WE SAVE ONE IMAGE AT A TIME.
            }
        }
    }
}

#endif


//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_SavePointListToPSLFile::vmxAppDataLoader_SavePointListToPSLFile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>PointList to PSL file");
    this->GetConfig()->SetTitleLong("Save PointList to PSL file");
}


void vmxAppDataLoader_SavePointListToPSLFile::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxPointList pl;
        if(selected_data_objects[i]->GetClassName() == pl.GetClassName())
        {
            char const * extension_filter[2] = { "*.psl", "*.PSL" };
            char const *save_file_name = tinyfd_saveFileDialog("Save PointList to PSL file", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SavePointListToPSLFile::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxPointList *p = static_cast<vmxPointList*>(selected_data_objects[i]);
            if(p)
            {
                p->SaveToPSLFile(save_file_name);
                return;
            }
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_SaveSkeletonToSKLFile::vmxAppDataLoader_SaveSkeletonToSKLFile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>Skeleton to SKL file");
    this->GetConfig()->SetTitleLong("Save Skeleton to SKL file");
}


void vmxAppDataLoader_SaveSkeletonToSKLFile::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxSkeleton skl;
        if(selected_data_objects[i]->GetClassName() == skl.GetClassName())
        {
            char const * extension_filter[2] = { "*.skl", "*.SKL" };
            char const *save_file_name = tinyfd_saveFileDialog("Save Skeleton to SKL file", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SaveSkeletonToSKLFile::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxSkeleton *p = static_cast< vmxSkeleton* >(selected_data_objects[i]);
            if(p)
            {
                p->SaveToTextFileOfPositions(save_file_name);
                return;
            }
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_SaveCurveToMatlabFile::vmxAppDataLoader_SaveCurveToMatlabFile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>Curve to Matlab file");
    this->GetConfig()->SetTitleLong("Save Curve to Matlab file");
}


void vmxAppDataLoader_SaveCurveToMatlabFile::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxCurve crv;
        if(selected_data_objects[i]->GetClassName() == crv.GetClassName())
        {
            char const * extension_filter[2] = { "*.m", "*.M" };
            char const *save_file_name = tinyfd_saveFileDialog("Save Curve to Matlab file", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SaveCurveToMatlabFile::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxCurve *p = static_cast< vmxCurve* >(selected_data_objects[i]);
            if(p)
            {
                p->SaveToMatlabFile(save_file_name);
                return;
            }
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------



vmxAppDataLoader_SaveProfileToPRFFile::vmxAppDataLoader_SaveProfileToPRFFile()
{
    this->GetConfig()->SetNumberOfSteps(0);
    this->GetConfig()->SetTitleInMenu("Save>>Profile to PRF file");
    this->GetConfig()->SetTitleLong("Save Profile to PRF file");
}


void vmxAppDataLoader_SaveProfileToPRFFile::StartInMainThread()
{
    mxArray<mxDataObject *> selected_data_objects;
    this->GetAppMainWidget()->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
    
    for(int i=0; i< (int)selected_data_objects.GetNumberOfElements(); i++)
    {
        vmxProfile prf;
        if(selected_data_objects[i]->GetClassName() == prf.GetClassName())
        {
            char const * extension_filter[2] = { "*.prf", "*.PRF" };
            char const *save_file_name = tinyfd_saveFileDialog("Save Profile to PRF file", selected_data_objects[i]->GetObjectName().Get_C_String(), 2, extension_filter, NULL);
            
            if (!save_file_name)
            {
                std::cout<<std::endl<<"vmxAppDataLoader_SaveProfileToPRFFile::StartInMainThread(): save_file_name is NULL";
                return ;
            }
            
            vmxProfile *p = static_cast< vmxProfile* >(selected_data_objects[i]);
            if(p)
            {
                p->SaveToFile(save_file_name);
                return;
            }
        }
    }
}


