/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIInteractorStyle.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxGUIInteractorStyle_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIInteractorStyle_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIInteractorStyle_EXPORTS
            #define vmxGUIInteractorStyle_API __declspec(dllexport)
        #else
            #define vmxGUIInteractorStyle_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIInteractorStyle_EXPORTS
            #define vmxGUIInteractorStyle_API __attribute__((visibility("default")))
        #else
            #define vmxGUIInteractorStyle_API
        #endif
    #endif
#endif



#ifndef vmxGUIInteractorStyle_H
#define vmxGUIInteractorStyle_H



#include "mxString.h"

#include "vmxGUIEventDetector.h"
//#include "vmxGUIRenderer.h"
#include "vmxGUIWidget.h"

#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPicker.h>
#include <vtkProperty.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>



// forward declaration.
class vmxGUIBaseObject;
class vmxGUIMainWidget;
class vmxGUIRenderer;


/// Interactor style used by the interactor of the main widget to call individual interaction styles for each renderer.

class vmxGUIInteractorStyle : public vtkInteractorStyleTrackballCamera
{
    
public:
    
    
    /// Pointer to the main widget contining the interactor that uses this interactor style.
    vmxGUIMainWidget *m_main_widget;
    
    /// Event for double click. The int value will be used to signal that double click has happened.
    static int event_DoubleClick;
    
    /// List of assigned renderers.
    mxList<vmxGUIRenderer*> m_renderers;
    
    /// Detects if left mouse button double click is performed.
    vmxDoubleClickDetector m_left_button_double_click_detector;
    
    /// Detects if left mouse button single click is performed.
    vmxSingleClickDetector m_left_button_single_click_detector;
    
    /// Detects if left mouse button drag is performed.
    vmxMouseDragDetector m_left_button_drag_detector;

//    /// Indicates if the left button is down.
//    int m_is_left_button_down;
//    
//    /// Indicates if the right button is down.
//    int m_is_right_button_down;
//
//    /// Indicates if the middle button is down.
//    int m_is_middle_button_down;


    /// List of widgets listenting for the given event. These objects listen for the specified events at all times and places (not just when the mouse hovers ovet them).
    /// Each event has its own list of listeners to speed up callback calling.
    mxList<vmxGUIBaseObject*> m_listeners_for_MouseMove_event;
    mxList<vmxGUIBaseObject*> m_listeners_for_MouseWheelBackward_event;
    mxList<vmxGUIBaseObject*> m_listeners_for_MouseWheelForward_event;
    mxList<vmxGUIBaseObject*> m_listeners_for_LeftButtonUp_event;
    mxList<vmxGUIBaseObject*> m_listeners_for_LeftButtonDown_event;
    mxList<vmxGUIBaseObject*> m_listeners_for_KeyPress_event;


    /// Constructor.
    vmxGUIInteractorStyle();

    /// Destructor.
    virtual ~vmxGUIInteractorStyle();
    
    /// General method used to add the given widget to the given list of listeners.
    void AddGUIWidgetToTheListOfListeners(vmxGUIBaseObject *listener_widget, mxList<vmxGUIBaseObject*> *list_of_listeners);
    
    /// General method used to remove the given widget from the given list of listeners.
    void RemoveGUIWidgetFromTheListOfListeners(vmxGUIBaseObject *listener_widget, mxList<vmxGUIBaseObject*> *list_of_listeners);

    /// Add a renderer to the list of maintined renderers by this interactor style.
    void AddRenderer(vmxGUIRenderer *renderer);
    
//    /// Register which renderer will be used for displaying GUI widgets.
//    void RegisterRendererForGUIWidgets(vtkRenderer *renderer_for_GUI_widgets);
    
//    /// Check if double click was performed.
//    int IsDoubleClicked(){ return m_is_double_clicked; };
    
    /// Create new object instance.
    static vmxGUIInteractorStyle* New();
    
    /// Re-implemented to override the existing keypress char functionality.
    virtual void OnChar()
    {
        //do nothing here.
    };
    
    /// Executed on mouse move event.
    virtual void OnMouseMove();

    /// Executed on mouse move event.
    virtual void OnMouseWheelBackward();
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelForward();

    /// Executed when left button is released.
    virtual void OnLeftButtonUp();
    
    /// Executed when left button is released. Re-implemented to detect double-click.
    virtual void OnLeftButtonDown();
    
    /// Executed when key is pressed.
    virtual void OnKeyPress();

//    /// Executed for the object which is double clicked on left button double click.
//    virtual void OnLeftButtonDoubleClick();
//
//    /// Executed for the object which is dragged on left button dragging.
//    virtual void OnLeftButtonDrag();

    /// Set the pointer to the main widget.
    void SetMainWidget(vmxGUIMainWidget *main_widget)
    {
        m_main_widget = main_widget;
    };
    
};



#endif
