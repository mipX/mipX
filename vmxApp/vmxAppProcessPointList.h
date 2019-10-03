/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppProcessPointList.h
 
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




#include <vtkMath.h>


#ifndef vmxAppProcessPointList_H
    #define vmxAppProcessPointList_H







//-------------------------------------------------------------------------------------------------------------------------



class vmxAppProcessPointList_InsertValuesFromCurve: public vmxAppFunctionInterface
{
public:
    
    vmxAppProcessPointList_InsertValuesFromCurve();
    virtual ~vmxAppProcessPointList_InsertValuesFromCurve(){};
    void Step_0();
    void Execute();
    void StartInMainThread();
};

typedef vmxAppFunctionFactoryInterfaceT<vmxAppProcessPointList_InsertValuesFromCurve> vmxAppProcessPointList_InsertValuesFromCurveFactory;




//-------------------------------------------------------------------------------------------------------------------------





class vmxAppProcessPointListFunctionList : public vmxAppFunctionFactoryList
{
public:
    vmxAppProcessPointListFunctionList()
    {
        *(this->m_list_of_function_factories.AddNewToEnd()) = new vmxAppProcessPointList_InsertValuesFromCurveFactory();
        
    };
    ~vmxAppProcessPointListFunctionList() {};
};



#endif
