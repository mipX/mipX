 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppDataLoader.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppDataLoader_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppDataLoader_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppDataLoader_EXPORTS
            #define vmxAppDataLoader_API __declspec(dllexport)
        #else
            #define vmxAppDataLoader_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppDataLoader_EXPORTS
            #define vmxAppDataLoader_API __attribute__((visibility("default")))
        #else
            #define vmxAppDataLoader_API
        #endif
    #endif
#endif


#ifndef vmxAppDataLoader_H
	#define vmxAppDataLoader_H

#include "tinyfiledialogs.h"

#include "vmxAppWidget.h"
#include "vmxAppFunctionInterface.h"
#include "vmxAppImageViewer.h"
#include "vmxCurve.h"
#include "vmxImage.h"
#include "vmxPointList.h"
#include "vmxProfile.h"
#include "vmxSkeleton.h"



#if defined(MIPX_BUILD_LOADER_DICOM)
    #include "xmxLoaderDicom.h"
#endif


#if defined(MIPX_BUILD_LOADER_NIFTI)
    #include "xmxLoaderNifti.h"
#endif


#if defined(MIPX_BUILD_LOADER_MATLAB)
    #include "xmxLoaderMatlab.h"
#endif


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



class vmxAppDataLoader_API vmxAppDataLoader_LoadProfile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadProfile();
    virtual ~vmxAppDataLoader_LoadProfile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadProfile> vmxAppDataLoader_LoadProfileFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_LoadPointList: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadPointList();
    virtual ~vmxAppDataLoader_LoadPointList() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadPointList> vmxAppDataLoader_LoadPointListFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_LoadSkeleton: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadSkeleton();
    virtual ~vmxAppDataLoader_LoadSkeleton() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadSkeleton> vmxAppDataLoader_LoadSkeletonFactory;



//-------------------------------------------------------------------------------------------------------------------------


#if defined(MIPX_BUILD_LOADER_DICOM)

class vmxAppDataLoader_API vmxAppDataLoader_LoadDicom: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadDicom();
    virtual ~vmxAppDataLoader_LoadDicom() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadDicom> vmxAppDataLoader_LoadDicomFactory;

#endif


//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_LoadVTKImage: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadVTKImage();
    virtual ~vmxAppDataLoader_LoadVTKImage() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadVTKImage> vmxAppDataLoader_LoadVTKImageFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_LoadPNGImages: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadPNGImages();
    virtual ~vmxAppDataLoader_LoadPNGImages() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadPNGImages> vmxAppDataLoader_LoadPNGImagesFactory;



//-------------------------------------------------------------------------------------------------------------------------


class vmxAppDataLoader_API vmxAppDataLoader_LoadColorPNGImages: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadColorPNGImages();
    virtual ~vmxAppDataLoader_LoadColorPNGImages() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadColorPNGImages> vmxAppDataLoader_LoadColorPNGImagesFactory;



//-------------------------------------------------------------------------------------------------------------------------




#if defined(MIPX_BUILD_LOADER_MATLAB)

class vmxAppDataLoader_API vmxAppDataLoader_LoadMatlabImage: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadMatlabImage();
    virtual ~vmxAppDataLoader_LoadMatlabImage() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadMatlabImage> vmxAppDataLoader_LoadMatlabImageFactory;

#endif


//-------------------------------------------------------------------------------------------------------------------------


#if defined(MIPX_BUILD_LOADER_NIFTI)

class vmxAppDataLoader_API vmxAppDataLoader_LoadNiftiImage: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadNiftiImage();
    virtual ~vmxAppDataLoader_LoadNiftiImage() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadNiftiImage> vmxAppDataLoader_LoadNiftiImageFactory;

#endif

//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_LoadCurveFromRaw: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadCurveFromRaw();
    virtual ~vmxAppDataLoader_LoadCurveFromRaw() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadCurveFromRaw> vmxAppDataLoader_LoadCurveFromRawFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_LoadCurveFromMatlab: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_LoadCurveFromMatlab();
    virtual ~vmxAppDataLoader_LoadCurveFromMatlab() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_LoadCurveFromMatlab> vmxAppDataLoader_LoadCurveFromMatlabFactory;



//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_SaveImageToVTKFile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SaveImageToVTKFile();
    virtual ~vmxAppDataLoader_SaveImageToVTKFile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SaveImageToVTKFile> vmxAppDataLoader_SaveImageToVTKFileFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_SaveImageToVTIFile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SaveImageToVTIFile();
    virtual ~vmxAppDataLoader_SaveImageToVTIFile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SaveImageToVTIFile> vmxAppDataLoader_SaveImageToVTIFileFactory;



//-------------------------------------------------------------------------------------------------------------------------


#if defined(MIPX_BUILD_LOADER_MATLAB)

class vmxAppDataLoader_API vmxAppDataLoader_SaveImageToMATFile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SaveImageToMATFile();
    virtual ~vmxAppDataLoader_SaveImageToMATFile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SaveImageToMATFile> vmxAppDataLoader_SaveImageToMATFileFactory;

#endif


//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_SaveImageToPNGFiles: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SaveImageToPNGFiles();
    virtual ~vmxAppDataLoader_SaveImageToPNGFiles() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SaveImageToPNGFiles> vmxAppDataLoader_SaveImageToPNGFilesFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_SavePointListToPSLFile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SavePointListToPSLFile();
    virtual ~vmxAppDataLoader_SavePointListToPSLFile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SavePointListToPSLFile> vmxAppDataLoader_SavePointListToPSLFileFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_SaveSkeletonToSKLFile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SaveSkeletonToSKLFile();
    virtual ~vmxAppDataLoader_SaveSkeletonToSKLFile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SaveSkeletonToSKLFile> vmxAppDataLoader_SaveSkeletonToSKLFileFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_SaveCurveToMatlabFile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SaveCurveToMatlabFile();
    virtual ~vmxAppDataLoader_SaveCurveToMatlabFile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SaveCurveToMatlabFile> vmxAppDataLoader_SaveCurveToMatlabFileFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppDataLoader_API vmxAppDataLoader_SaveProfileToPRFFile: public vmxAppFunctionInterface
{
public:
    
    vmxAppDataLoader_SaveProfileToPRFFile();
    virtual ~vmxAppDataLoader_SaveProfileToPRFFile() {};
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppDataLoader_SaveProfileToPRFFile> vmxAppDataLoader_SaveProfileToPRFFileFactory;




//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------




class vmxAppDataLoader_API vmxAppDataLoader : public vmxAppFunctionFactoryList
{
public:
    vmxAppDataLoader()
    {
        // Loading
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadProfileFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadPointListFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadSkeletonFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadCurveFromRawFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadCurveFromMatlabFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadVTKImageFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadPNGImagesFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadColorPNGImagesFactory();

#if defined(MIPX_BUILD_LOADER_DICOM)
    *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadDicomFactory();
#endif
#if defined(MIPX_BUILD_LOADER_MATLAB)
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadMatlabImageFactory();
#endif
#if defined(MIPX_BUILD_LOADER_NIFTI)
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_LoadNiftiImageFactory();
#endif
      
        

        // Saving
#if defined(MIPX_BUILD_LOADER_MATLAB)
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SaveImageToMATFileFactory();
#endif
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SaveImageToVTKFileFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SaveImageToVTIFileFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SaveImageToPNGFilesFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SavePointListToPSLFileFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SaveSkeletonToSKLFileFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SaveCurveToMatlabFileFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppDataLoader_SaveProfileToPRFFileFactory();
        
        
    };
    ~vmxAppDataLoader() {};
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif



#endif
