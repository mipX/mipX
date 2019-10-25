/*=========================================================================
 
 Program:   mipx
 Module:    LaaUI.h
 
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
#include "laaEDT.h"


#ifndef LaaUI_H
    #define LaaUI_H



//-------------------------------------------------------------------------------------------------------------------------

class LaaUI_Thresholding: public vmxAppFunctionInterface
{
public:
    
    LaaUI_Thresholding();
    virtual ~LaaUI_Thresholding(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<LaaUI_Thresholding> LaaUI_ThresholdingFactory;


//-------------------------------------------------------------------------------------------------------------------------

class LaaUI_CreateMesh: public vmxAppFunctionInterface
{
public:
    
    LaaUI_CreateMesh();
    virtual ~LaaUI_CreateMesh(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<LaaUI_CreateMesh> LaaUI_CreateMeshFactory;



//-------------------------------------------------------------------------------------------------------------------------

class LaaUI_DistanceTransform: public vmxAppFunctionInterface
{
public:
    
    LaaUI_DistanceTransform();
    virtual ~LaaUI_DistanceTransform(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<LaaUI_DistanceTransform> LaaUI_DistanceTransformFactory;


//-------------------------------------------------------------------------------------------------------------------------

class LaaUI_DecreasingRadiiSegmentation: public vmxAppFunctionInterface
{
public:
    
    LaaUI_DecreasingRadiiSegmentation();
    virtual ~LaaUI_DecreasingRadiiSegmentation(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<LaaUI_DecreasingRadiiSegmentation> LaaUI_DecreasingRadiiSegmentationFactory;


//-------------------------------------------------------------------------------------------------------------------------










class LaaUIFunctionList : public vmxAppFunctionFactoryList
{
public:
    LaaUIFunctionList()
    {
        *(this->m_list_of_function_factories.AddNewToEnd()) = new LaaUI_CreateMeshFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new LaaUI_DecreasingRadiiSegmentationFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new LaaUI_DistanceTransformFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new LaaUI_ThresholdingFactory();
    };
    ~LaaUIFunctionList() {};
};



#endif
