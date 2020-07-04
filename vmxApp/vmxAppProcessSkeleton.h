/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessSkeleton.h
 
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
#include "vmxCurve.h"
#include "vmxImage.h"
#include "vmxMesh.h"
#include "vmxGraph.h"
#include "vmxProfile.h"
#include "vmxPointList.h"
#include "vmxSkeleton.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


#include <vtkMath.h>


#ifndef vmxAppProcessSkeleton_H
    #define vmxAppProcessSkeleton_H



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessSkeleton_CreateFromImage: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessSkeleton_CreateFromImage();
    virtual ~vmxAppProcessSkeleton_CreateFromImage(){};
    //void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessSkeleton_CreateFromImage> vmxAppProcessSkeleton_CreateFromImageFactory;




//-------------------------------------------------------------------------------------------------------------------------





class vmxAppProcessSkeletonFunctionList : public vmxAppFunctionFactoryList
{
public:
    vmxAppProcessSkeletonFunctionList()
    {
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessSkeleton_CreateFromImageFactory();
        
    };
    ~vmxAppProcessSkeletonFunctionList() {};
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif


#endif
