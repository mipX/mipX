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
#include "vmxGUIWidget.h"

#include <iostream>

#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>




/// Callback method class executed when render window is resized.

class vmxGUIMainWidgetRenderWindowModifiedCallback : public vtkCommand
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



/// Main widget of vtk GUI. Each widget is bound to this one.

class vmxGUIMainWidget_API vmxGUIMainWidget
{
    
protected:
    
    /// Class (type) unique name string.
    mxString m_class_name;
    
    /// Pointer to the containing render window. We keep a separate pointer to allow a possibility of use of external render window.
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
    
    /// Renderer that will contain GUI objects.
    vtkSmartPointer<vtkRenderer> m_renderer_GUI;
    
    /// Renderer that will contain 3D scene.
    vmxGUIRenderer3DTrackBallCamera m_renderer_3D;
    
    /// List of user defined renderers.
    mxList< vmxGUIRenderer* > m_renderers_list;
    
    /// Render window that contains all the renderers and the interactor.
    vtkSmartPointer<vtkRenderWindow> m_render_window_internal; //Maybe the render window should not be included here, but externally. To be determined.
    
    /// Render window interactor associated with the render window.
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    
    /// Style assigned to the interactor.
    vtkSmartPointer<vmxGUIInteractorStyle> m_interactor_style;
    
    /// Clipboard owned by the main widget.
    vmxGUIClipBoard m_clipboard;
    
    
    
    /// List of contained objects for particular placement.
    //    mxList<vmxGUIWidget*> m_bottom_objects;
    //    mxList<vmxGUIWidget*> m_top_objects;
    
    mxList<vmxGUIWidget*> m_lower_left_objects;
    mxList<vmxGUIWidget*> m_lower_right_objects;
    mxList<vmxGUIWidget*> m_lower_center_objects;
    mxList<vmxGUIWidget*> m_upper_left_objects;
    mxList<vmxGUIWidget*> m_upper_right_objects;
    mxList<vmxGUIWidget*> m_upper_center_objects;
    
    // This is how the distribution of object placements (according to object lists) looks like:
    // ----t---
    // ul uc ur
    // ll lc lr
    // ----b---
    
    
public:
    
    /// List of contained objects.
    mxList<vmxGUIBaseObject*> m_objects;
    
    
    
    /// Constructor.
    vmxGUIMainWidget();
    
    /// Destructor.
    virtual ~vmxGUIMainWidget();
    
    /// Add a new user defined renderer.
    int AddRenderer(vmxGUIRenderer *renderer); //vmxGUIRenderer* AddRenderer(const char *renderer_name = "Renderer");
    
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
    
    /// Get pointer to the clipboard.
    vmxGUIClipBoard* GetClipBoard() { return &m_clipboard; };
    
    /// Get pointer to the interactor.
    vtkRenderWindowInteractor* GetInteractor() { return m_interactor; };
    
    /// Get pointer to the interactor style.
    vmxGUIInteractorStyle* GetInteractorStyle() { return m_interactor_style; };
    
    /// Get pointer to the renderer of GUI.
    vtkRenderer* GetRenderer_GUI() { return m_renderer_GUI; };
    
    /// Get pointer to the 3D renderer.
    vtkRenderer* GetRenderer_3D() { return m_renderer_3D.GetVTKRenderer(); };
    
    /// Get pointer to the render window.
    vtkRenderWindow* GetRenderWindow();
    
    /// Get the size of the render window. If no render window is attached return fail 0.
    int GetRenderWindowSize(int &x_size, int &y_size);
    
    /// Get the object that contains the given 2D vtk actor. If none found, return NULL.
    vmxGUIBaseObject* GetGUIObjectForVTKActor2D(vtkActor2D *actor);
    
    /// Get the object falling under the given (mouse click) screen coordinates. If none found, return NULL.
    vmxGUIBaseObject* GetGUIObjectForScreenCoordinates(int pos1, int pos2);
    
    /// Based on existing placement preference, repositions the object.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Reset the object.
    virtual void Reset();
    
    /// Set the render window.
    void SetRenderWindow(vtkRenderWindow *render_window);
    
    /// Render and start the interaction.
    void StartInteraction()
    {
        this->GetRenderWindow()->Render();
        this->GetInteractor()->Start();
    };
};






#endif
