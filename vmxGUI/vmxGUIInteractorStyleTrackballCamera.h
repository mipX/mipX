/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIInteractorStyleTrackballCamera.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxGUIInteractorStyleTrackballCamera_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxGUIInteractorStyleTrackballCamera_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIInteractorStyleTrackballCamera_EXPORTS
            #define vmxGUIInteractorStyleTrackballCamera_API __declspec(dllexport)
        #else
            #define vmxGUIInteractorStyleTrackballCamera_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIInteractorStyleTrackballCamera_EXPORTS
            #define vmxGUIInteractorStyleTrackballCamera_API __attribute__((visibility("default")))
        #else
            #define vmxGUIInteractorStyleTrackballCamera_API
        #endif
    #endif
#endif



#ifndef vmxGUIInteractorStyleTrackballCamera_H
	#define vmxGUIInteractorStyleTrackballCamera_H



#include "mxObject.h"
#include "mxString.h"

#include <chrono>
//#include <stdint.h>

#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
//#include <vtkGenericDataObjectReader.h>
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




class vmxGUIInteractorStyleTrackballCamera : public vtkInteractorStyleTrackballCamera
{
    
public:
    
    vtkRenderer *m_registered_renderer_2D;
    
    // for detecting double click.
    int m_number_of_clicks;
    
    // for detecting double click.
    int m_left_button_down_previous_position[2];
    
    /// Indicator that will be set when the double click is detected.
    int m_is_double_clicked;
    
    /// This int value will be used to signal that double click has happened.
    static int EventDoubleClick;
    
    /// Indicates if the left button is down (used in move event to detect drag).
    int m_is_left_button_down;
    
    vtkActor2D *m_picked_2D_actor_on_left_button_down;
    
    //vmxImage *m_v_img;
    //vmxMenu *m_menu;
    //mxDataObjectTree *m_tree;
    
    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point < clock, std::chrono::milliseconds > ;
    
    time_point_type m_time_of_left_click_in_ms, m_time_of_left_click_in_ms2;
    
    
    /// Constructor.
    vmxGUIInteractorStyleTrackballCamera();

    /// Destructor.
    virtual ~vmxGUIInteractorStyleTrackballCamera();
    
    /// Check if double click was performed.
    int IsDoubleClicked(){ return m_is_double_clicked; };
    
    /// Create new object instance.
    static vmxGUIInteractorStyleTrackballCamera* New();
    
    
    //void SetDataObjectTree(mxDataObjectTree *tree) { m_tree = tree; };
    //void SetImage(vmxImage *im) { m_v_img = im; };
    //void SetMenu(vmxMenu *m) { m_menu = m; };
    
    
    /// Re-implemented to override the existing keypress char functionality.
    virtual void OnChar() { };//do nothing here.
    
    virtual void OnMouseMove();
    
    virtual void OnMouseWheelBackward();
    
    virtual void OnMouseWheelForward();

    /// Executes when left button is released.
    virtual void OnLeftButtonUp();
    
    /// Re-implemented to detect double-click.
    virtual void OnLeftButtonDown();
    
    /// Re-implemented to use specific keys.
    virtual void OnKeyPress();
    
    
};



#endif
