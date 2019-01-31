/*=========================================================================
 
 Program:   mipx
 Module:    vmxCurve.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxCurve_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxCurve_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxCurve_EXPORTS
            #define vmxCurve_API __declspec(dllexport)
        #else
            #define vmxCurve_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxCurve_EXPORTS
            #define vmxCurve_API __attribute__((visibility("default")))
        #else
            #define vmxCurve_API
        #endif
    #endif
#endif



#ifndef vmxCurve_H
    #define vmxCurve_H


#include "mxCurve.h"
#include "mxDataObjectFactory.h"
#include "mxList.h"
#include "mxMatrix.h"

#include <vtkDataArray.h>
#include <vtkFieldData.h>
#include <vtkXYPlotActor.h>
#include <vtkProperty2D.h>
#include <vtkDataObject.h>
#include <vtkSmartPointer.h>


#include <vtkVersion.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>


#include <vtkDoubleArray.h>



/// Helper class for curve color assignent.

class vmxCurveColor
{
public:
    
    double m_red;
    double m_green;
    double m_blue;
};




/// VTK level 2D curve (xy signal).

class vmxCurve : public mxCurve
{
    
public:
    
    /// Array of y values.
    vtkSmartPointer< vtkDoubleArray > m_array_y;

    /// Array of x values.
    vtkSmartPointer< vtkDoubleArray > m_array_x;

    /// Visibility indicator.
    int m_is_visible;
    
    /// Array of colors that will be assigned to different curves.
    static mxArray< vmxCurveColor > m_colors;
    
    /// Index that controls which color should be assigned to the curve.
    static int m_color_index;

    

    /// Constructor.
    vmxCurve();
    
    /// Destructor.
    virtual ~vmxCurve();
    
    /// Get VTK double array for x values.
    vtkDoubleArray* Get_X_DoubleArray() { return m_array_x; };

    /// Get VTK double array for y values.
    vtkDoubleArray* Get_Y_DoubleArray() { return m_array_y; };
    
    /// Set the visibility of the object component for VTK visualization.
    /// In this case just a single plot of a curve is shown so number of components is 1.
    void SetVisibilityOfComponent(int component_index, int is_visible) { m_is_visible = is_visible; };
    
    /// Get the visibility of the object component for VTK visualization.
    int GetVisibilityOfComponent(int component_index) { return m_is_visible; };

    /// Update visual structures for the curve after modification.
    void Update();
};



typedef mxDataObjectFactoryT<vmxCurve> vmxCurveFactory;






#endif
