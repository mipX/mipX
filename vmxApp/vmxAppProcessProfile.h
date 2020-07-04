/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessProfile.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt

 Licensed under the BSD License 2.0.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.

 =========================================================================*/




#include "tinyfiledialogs.h"

#include "mxProfileProcessing.h"
#include "mxPosition.h"
#include "mxBIP.h"
#include "mxGIP.h"
#include "mxSkeletonization.h"
#include "mxGraphProcessing.h"


#include "vmxAppFunctionInterface.h"
#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxAppReportWidget.h"
#include "vmxImage.h"
#include "vmxMesh.h"
#include "vmxGraph.h"
#include "vmxProfile.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


#include <vtkMath.h>


#ifndef vmxAppProcessProfile_H
    #define vmxAppProcessProfile_H



#define vmxAppProcessProfileMenuString "Profile"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessProfile_Create_RC_Profile: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessProfile_Create_RC_Profile();
    virtual ~vmxAppProcessProfile_Create_RC_Profile(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessProfile_Create_RC_Profile> vmxAppProcessProfile_Create_RC_ProfileFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessProfile_Create_RS_Profile: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessProfile_Create_RS_Profile();
    virtual ~vmxAppProcessProfile_Create_RS_Profile(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessProfile_Create_RS_Profile> vmxAppProcessProfile_Create_RS_ProfileFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessProfile_ProfileMeasure: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessProfile_ProfileMeasure();
    virtual ~vmxAppProcessProfile_ProfileMeasure(){};
    void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessProfile_ProfileMeasure> vmxAppProcessProfile_ProfileMeasureFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_ThresholdProfileMeasureImage: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_ThresholdProfileMeasureImage();
    virtual ~vmxAppProcessImage_ThresholdProfileMeasureImage(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_ThresholdProfileMeasureImage> vmxAppProcessImage_ThresholdProfileMeasureImageFactory;




//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------




class vmxAppProcessProfileFunctionList : public vmxAppFunctionFactoryList
{
public:
    vmxAppProcessProfileFunctionList()
    {
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_ThresholdProfileMeasureImageFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessProfile_ProfileMeasureFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessProfile_Create_RC_ProfileFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessProfile_Create_RS_ProfileFactory();
    };
    ~vmxAppProcessProfileFunctionList() {};
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif


#endif
