/*=========================================================================
 
 Program:   mipx
 Module:    PWVPaPd.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt

 Licensed under the BSD License 2.0.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.

 =========================================================================*/




#include "tinyfiledialogs.h"

//#include "mxProfileProcessing.h"
#include "mxPosition.h"
#include "mxBIP.h"
#include "mxGIP.h"
//#include "mxSkeletonization.h"
//#include "mxGraphProcessing.h"


#include "vmxAppFunctionInterface.h"
#include "vmxAppWidget.h"
#include "vmxAppImageViewer.h"
#include "vmxAppReportWidget.h"
#include "vmxImage.h"
#include "vmxMesh.h"
#include "vmxGraph.h"
#include "vmxCurve.h"
//#include "vmxProfile.h"


#include <vtkPNGReader.h>

#include <vtkImageExtractComponents.h>

#include <vtkMath.h>


#ifndef ImageProcessing_H
    #define ImageProcessing_H





//-------------------------------------------------------------------------------------------------------------------------




class PWVPaPd_LoadColorPNGImage: public vmxAppFunctionInterface
{
public:
    PWVPaPd_LoadColorPNGImage();
    virtual ~PWVPaPd_LoadColorPNGImage(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<PWVPaPd_LoadColorPNGImage> PWVPaPd_LoadColorPNGImageFactory;



//-------------------------------------------------------------------------------------------------------------------------




class PWVPaPd_CreatePaCurve: public vmxAppFunctionInterface
{
public:
    PWVPaPd_CreatePaCurve();
    virtual ~PWVPaPd_CreatePaCurve(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<PWVPaPd_CreatePaCurve> PWVPaPd_CreatePaCurveFactory;



//-------------------------------------------------------------------------------------------------------------------------



class PWVPaPd_CreatePdCurve: public vmxAppFunctionInterface
{
public:
    PWVPaPd_CreatePdCurve();
    virtual ~PWVPaPd_CreatePdCurve(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<PWVPaPd_CreatePdCurve> PWVPaPd_CreatePdCurveFactory;



//-------------------------------------------------------------------------------------------------------------------------







class PWVPaPdFunctionList : public vmxAppFunctionFactoryList
{
public:
    PWVPaPdFunctionList()
    {
        *(this->m_list_of_function_factories.AddNewToEnd()) = new PWVPaPd_LoadColorPNGImageFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new PWVPaPd_CreatePaCurveFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new PWVPaPd_CreatePdCurveFactory();
    };
    ~PWVPaPdFunctionList() {};
};



#endif
