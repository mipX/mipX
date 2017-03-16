/*=========================================================================
 
 Program:   mipx
 Module:    vmxInteractorStyleTrackballCamera.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxInteractorStyleTrackballCamera_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxInteractorStyleTrackballCamera_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxInteractorStyleTrackballCamera_EXPORTS
            #define vmxInteractorStyleTrackballCamera_API __declspec(dllexport)
        #else
            #define vmxInteractorStyleTrackballCamera_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxInteractorStyleTrackballCamera_EXPORTS
            #define vmxInteractorStyleTrackballCamera_API __attribute__((visibility("default")))
        #else
            #define vmxInteractorStyleTrackballCamera_API
        #endif
    #endif
#endif



#ifndef vmxInteractorStyleTrackballCamera_H
	#define vmxInteractorStyleTrackballCamera_H



#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)


#include "mxObject.h"
#include "mxString.h"

#include <chrono>
#include <stdint.h>

#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkInteractorStyleTrackballCamera.h>
//#include <vtkGenericDataObjectReader.h>
//#include <vtkImageData.h>
//#include <vtkImageCast.h>
//#include <vtkImageFlip.h>
//#include <vtkImagePlaneWidget.h>
//#include <vtkImageShiftScale.h>
//#include <vtkInformation.h>
//#include <vtkLookupTable.h>
//#include <vtkPNGReader.h>
//#include <vtkPNGWriter.h>
//#include <vtkPointData.h>
#include <vtkPicker.h>
#include <vtkProperty.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
//#include <vtkTextActor.h>
//#include <vtkTextProperty.h>




class vmxInteractorStyleTrackballCamera : public vtkInteractorStyleTrackballCamera
{
    
public:
    
    // for detecting double click.
    int m_number_of_clicks;
    
    // for detecting double click.
    int m_previous_position[2];
    
    /// Indicator that will be set when the double click is detected.
    int m_is_double_clicked;
    
    //vmxImage *m_v_img;
    //vmxMenu *m_menu;
    //mxDataObjectTree *m_tree;
    
    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point < clock, std::chrono::milliseconds > ;
    
    time_point_type m_time_of_left_click_in_ms,m_time_of_left_click_in_ms2;
    
    
    /// Constructor.
    vmxInteractorStyleTrackballCamera();

    /// Destructor.
    ~vmxInteractorStyleTrackballCamera();
    
    /// Check if double click was performed.
    int IsDoubleClicked(){ return m_is_double_clicked; };
    
    /// Create new object instance.
    static vmxInteractorStyleTrackballCamera* New();
    
    
    //void SetDataObjectTree(mxDataObjectTree *tree) { m_tree = tree; };
    //void SetImage(vmxImage *im) { m_v_img = im; };
    //void SetMenu(vmxMenu *m) { m_menu = m; };
    
    
    /// Re-implemented to override the existing keypress char functionality.
    virtual void OnChar() { };//do nothing here.
    
    /// Re-implemented to detect double-click.
    virtual void OnLeftButtonDown();
    
    /// Re-implemented to use specific keys.
    virtual void OnKeyPress();
    
    
};



#endif
