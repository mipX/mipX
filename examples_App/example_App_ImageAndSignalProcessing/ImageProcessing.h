/*=========================================================================
 
 Program:   mipx
 Module:    ImageProcessing.h
 
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



#include <vtkMath.h>


#ifndef ImageProcessing_H
    #define ImageProcessing_H





//-------------------------------------------------------------------------------------------------------------------------




class ImageProcessing_Thresholding: public vmxAppFunctionInterface
{
public:
    
    ImageProcessing_Thresholding();
    virtual ~ImageProcessing_Thresholding(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<ImageProcessing_Thresholding> ImageProcessing_ThresholdingFactory;



//-------------------------------------------------------------------------------------------------------------------------



class ImageProcessing_CreateMesh: public vmxAppFunctionInterface
{
public:
    
    ImageProcessing_CreateMesh();
    virtual ~ImageProcessing_CreateMesh(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<ImageProcessing_CreateMesh> ImageProcessing_CreateMeshFactory;



//-------------------------------------------------------------------------------------------------------------------------



class ImageProcessing_ExtractLargestCC: public vmxAppFunctionInterface
{
public:
    
    ImageProcessing_ExtractLargestCC();
    virtual ~ImageProcessing_ExtractLargestCC(){};
    //void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<ImageProcessing_ExtractLargestCC> ImageProcessing_ExtractLargestCCFactory;



//-------------------------------------------------------------------------------------------------------------------------



class ImageProcessing_OrderedSkeletonization: public vmxAppFunctionInterface
{
public:
    
    ImageProcessing_OrderedSkeletonization();
    virtual ~ImageProcessing_OrderedSkeletonization(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<ImageProcessing_OrderedSkeletonization> ImageProcessing_OrderedSkeletonizationFactory;



//-------------------------------------------------------------------------------------------------------------------------





class ImageProcessingFunctionList : public vmxAppFunctionFactoryList
{
public:
    ImageProcessingFunctionList()
    {
        
        *(this->m_list_of_function_factories.AddNewToEnd()) = new ImageProcessing_OrderedSkeletonizationFactory();
        
        *(this->m_list_of_function_factories.AddNewToEnd()) = new ImageProcessing_ExtractLargestCCFactory();
        
        *(this->m_list_of_function_factories.AddNewToEnd()) = new ImageProcessing_ThresholdingFactory();
        
        *(this->m_list_of_function_factories.AddNewToEnd()) = new ImageProcessing_CreateMeshFactory();
        
    };
    ~ImageProcessingFunctionList() {};
};



#endif
