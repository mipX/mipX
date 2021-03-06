/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessImage.h
 
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


#ifndef vmxAppProcessImage_H
    #define vmxAppProcessImage_H



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




class vmxAppProcessImage_CompactValues: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_CompactValues();
    virtual ~vmxAppProcessImage_CompactValues(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_CompactValues> vmxAppProcessImage_CompactValuesFactory;



//-------------------------------------------------------------------------------------------------------------------------




class vmxAppProcessImage_Thresholding: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_Thresholding();
    virtual ~vmxAppProcessImage_Thresholding(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_Thresholding> vmxAppProcessImage_ThresholdingFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_CreateMesh: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_CreateMesh();
    virtual ~vmxAppProcessImage_CreateMesh(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_CreateMesh> vmxAppProcessImage_CreateMeshFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_Subtract: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_Subtract();
    virtual ~vmxAppProcessImage_Subtract(){};
    void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_Subtract> vmxAppProcessImage_SubtractFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_Mask: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_Mask();
    virtual ~vmxAppProcessImage_Mask(){};
    void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_Mask> vmxAppProcessImage_MaskFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_ExtractCenterlines: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_ExtractCenterlines();
    virtual ~vmxAppProcessImage_ExtractCenterlines(){};
    //void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_ExtractCenterlines> vmxAppProcessImage_ExtractCenterlinesFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_ExtractLargestCC: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_ExtractLargestCC();
    virtual ~vmxAppProcessImage_ExtractLargestCC(){};
    //void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_ExtractLargestCC> vmxAppProcessImage_ExtractLargestCCFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_Filter_4_Neighbors: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_Filter_4_Neighbors();
    virtual ~vmxAppProcessImage_Filter_4_Neighbors(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_Filter_4_Neighbors> vmxAppProcessImage_Filter_4_NeighborsFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_ProcessPhaseContrast: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_ProcessPhaseContrast();
    virtual ~vmxAppProcessImage_ProcessPhaseContrast(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_ProcessPhaseContrast> vmxAppProcessImage_ProcessPhaseContrastFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_GraphReportXTK: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_GraphReportXTK();
    virtual ~vmxAppProcessImage_GraphReportXTK(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_GraphReportXTK> vmxAppProcessImage_GraphReportXTKFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_SkeletonReportXTK: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_SkeletonReportXTK();
    virtual ~vmxAppProcessImage_SkeletonReportXTK(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_SkeletonReportXTK> vmxAppProcessImage_SkeletonReportXTKFactory;



//-------------------------------------------------------------------------------------------------------------------------




class vmxAppProcessImage_OrderedSkeletonization: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_OrderedSkeletonization();
    virtual ~vmxAppProcessImage_OrderedSkeletonization(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_OrderedSkeletonization> vmxAppProcessImage_OrderedSkeletonizationFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_CreateGraphFromImage: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_CreateGraphFromImage();
    virtual ~vmxAppProcessImage_CreateGraphFromImage(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_CreateGraphFromImage> vmxAppProcessImage_CreateGraphFromImageFactory;



//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessImage_FilterNodeMultiPahts: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessImage_FilterNodeMultiPahts();
    virtual ~vmxAppProcessImage_FilterNodeMultiPahts(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessImage_FilterNodeMultiPahts> vmxAppProcessImage_FilterNodeMultiPahtsFactory;



//-------------------------------------------------------------------------------------------------------------------------




class vmxAppProcessImageFunctionList : public vmxAppFunctionFactoryList
{
public:
    vmxAppProcessImageFunctionList()
    {
        
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_FilterNodeMultiPahtsFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_CreateGraphFromImageFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_OrderedSkeletonizationFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_ProcessPhaseContrastFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_Filter_4_NeighborsFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_ExtractLargestCCFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_SubtractFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_MaskFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_ExtractCenterlinesFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_CompactValuesFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_ThresholdingFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_CreateMeshFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_GraphReportXTKFactory();
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessImage_SkeletonReportXTKFactory();
        
    };
    ~vmxAppProcessImageFunctionList() {};
};




#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif



#endif
