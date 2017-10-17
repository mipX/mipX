 /*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIMainWidget.h
 
  Authors: Danilo Babin.
  Copyright (c) Danilo Babin.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxGUIMainWidget_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIMainWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIMainWidget_EXPORTS
            #define vmxGUIMainWidget_API __declspec(dllexport)
        #else
            #define vmxGUIMainWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIMainWidget_EXPORTS
            #define vmxGUIMainWidget_API __attribute__((visibility("default")))
        #else
            #define vmxGUIMainWidget_API
        #endif
    #endif
#endif


#ifndef vmxGUIMainWidget_H
	#define vmxGUIMainWidget_H



#include "mxString.h"
#include "vmxGUIObject.h"

#include <iostream>

#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>



class vmxGUIMainWidget_API vmxGUIMainWidgetRenderWindowModifiedCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxGUIMainWidget *m_widget;
    
    int m_previous_window_size[2];
    
    /// Constructor.
    vmxGUIMainWidgetRenderWindowModifiedCallback();
    
    /// Destructor.
    ~vmxGUIMainWidgetRenderWindowModifiedCallback();
    
    /// Create new object instance.
    static vmxGUIMainWidgetRenderWindowModifiedCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};




//----------------------------------------------------------------------------------------------------------------------




class vmxGUIMainWidget_API vmxGUIMainWidget
{

protected:

    /// Class (type) unique name string.
    mxString m_class_name;//static mxString m_class_name;
    
    /// Pointer to the containing render window.
    vtkRenderWindow *m_render_window;
    
    /// Available extent of x values, sequentially [x_min,x_max] for the left side of the main widget.
    int m_left_x_extent[2];

    /// Available extent of y values, sequentially [y_min,y_max] for the left side of the main widget.
    int m_left_y_extent[2];

    /// Available extent of x values, sequentially [x_min,x_max] for the center of the main widget.
    int m_center_x_extent[2];
    
    /// Available extent of y values, sequentially [y_min,y_max] for the center of the main widget.
    int m_center_y_extent[2];

    /// Available extent of x values, sequentially [x_min,x_max] for the right side of the main widget.
    int m_right_x_extent[2];
    
    /// Available extent of y values, sequentially [y_min,y_max] for the right side of the main widget.
    int m_right_y_extent[2];

    /// Spacing between objects.
    int m_spacing;
    
    /// Callback regulating the positioning of objects when the render window is resized.
    vtkSmartPointer<vmxGUIMainWidgetRenderWindowModifiedCallback> m_window_modified_callback;

    
public:
    
    /// List of contained objects.
    mxList<vmxGUIObject*> m_objects;

    
    
    /// Constructor.
    vmxGUIMainWidget();
    
    /// Destructor.
    virtual ~vmxGUIMainWidget();
    
    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForLeftSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);

    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForCenter(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);

    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForRightSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);
    
    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForLeftSide(int &x_size, int &y_size, int is_stretching_over_x_axis);
    
    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForCenter(int &x_size, int &y_size, int is_stretching_over_x_axis);

    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForRightSide(int &x_size, int &y_size, int is_stretching_over_x_axis);
    
    /// Get pointer to the render window.
    vtkRenderWindow* GetRenderWindow();
    
    /// Get the size of the render window. If no render window is attached return fail 0.
    int GetRenderWindowSize(int &x_size, int &y_size);
    
    /// Based on existing placement preference, repositions the object.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Reset the object.
    void Reset();
    
    /// Set the render window.
    void SetRenderWindow(vtkRenderWindow *render_window);
    
};





#endif