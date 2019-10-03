/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessGraph.h
 
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
#include "vmxPointList.h"



#include <vtkMath.h>


#ifndef vmxAppProcessGraph_H
    #define vmxAppProcessGraph_H







//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessGraph_CreateGraphFromImage: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessGraph_CreateGraphFromImage();
    virtual ~vmxAppProcessGraph_CreateGraphFromImage(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessGraph_CreateGraphFromImage> vmxAppProcessGraph_CreateGraphFromImageFactory;




//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessGraph_CreateGraphFromPointList: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessGraph_CreateGraphFromPointList();
    virtual ~vmxAppProcessGraph_CreateGraphFromPointList(){};
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessGraph_CreateGraphFromPointList> vmxAppProcessGraph_CreateGraphFromPointListFactory;




//-------------------------------------------------------------------------------------------------------------------------




class vmxAppProcessGraphFunctionList : public vmxAppFunctionFactoryList
{
public:
    vmxAppProcessGraphFunctionList()
    {
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessGraph_CreateGraphFromImageFactory();
        
        //*(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessGraph_CreateGraphFromPointListFactory();
        
    };
    ~vmxAppProcessGraphFunctionList() {};
};



#endif
