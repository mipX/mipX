/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRendererSignalViewer.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxGUIRendererSignalViewer_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIRendererSignalViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIRendererSignalViewer_EXPORTS
            #define vmxGUIRendererSignalViewer_API __declspec(dllexport)
        #else
            #define vmxGUIRendererSignalViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIRendererSignalViewer_EXPORTS
            #define vmxGUIRendererSignalViewer_API __attribute__((visibility("default")))
        #else
            #define vmxGUIRendererSignalViewer_API
        #endif
    #endif
#endif



#ifndef vmxGUIRendererSignalViewer_H
    #define vmxGUIRendererSignalViewer_H


#include "mxString.h"
#include "vmxGUIWidget.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif

#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkContextActor.h>
#include <vtkContextScene.h>
#include <vtkAbstractArray.h>
#include <vtkPointData.h>

#include <vtkVersion.h>

#include <vtkChartXY.h>
#include <vtkContextScene.h>
#include <vtkContextActor.h>
#include <vtkContextInteractorStyle.h>
#include <vtkFloatArray.h>
#include <vtkPlotPoints.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTable.h>

#include <vtkContextMouseEvent.h>
#include <vtkTooltipItem.h>
#include <vtkAxis.h>
#include <vtkBrush.h>
#include <vtkPen.h>
#include <vtkChartLegend.h>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



/// Container for a single signal.

class vmxGUIRendererSignalViewerSignalContainer
{
public:
    mxString m_name;
    mxString m_X_label;
    mxString m_Y_label;
    
    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
    
    vtkDoubleArray *m_X_array;
    vtkDoubleArray *m_Y_array;
    
    vmxGUIRendererSignalViewerSignalContainer()
    {
        m_name.Assign("signal");
        m_X_label.Assign("X");
        m_Y_label.Assign("Y");
        m_X_array = NULL;
        m_Y_array = NULL;
        
        m_red = 255;
        m_green = 255;
        m_blue = 255;
    };
    
    ~vmxGUIRendererSignalViewerSignalContainer() { };
};



/// VTK GUI signal (xy curve) viewer.

class vmxGUIRendererSignalViewer_API vmxGUIRendererSignalViewer : public vmxGUIRenderer
{

public:
    
    /// Indicator if the left button is down.
    int m_is_left_button_down;
    
    /// Last screen position of the mouse.
    int m_last_mouse_screen_position_xy[2];
    
    vtkSmartPointer<vtkContextScene> m_chart_scene;
    vtkSmartPointer<vtkContextActor> m_chart_actor;

    
    vtkSmartPointer<vtkChartXY> m_chart;
    
    
    
    // Create a table with some points in it...
    vtkSmartPointer<vtkTable> m_table;

   

    
    /// List of singal containers.
    mxList< vmxGUIRendererSignalViewerSignalContainer > m_signals;
    
 
//    /// Setup the text and add it to the renderer
//    vtkSmartPointer<vtkTextActor> m_coordinate_text_actor;
    
    
    
    /// Default array to plot in case there is nothing else to plot.
    vtkSmartPointer<vtkDoubleArray> m_default_array_x;
    vtkSmartPointer<vtkDoubleArray> m_default_array_y;
    
    
//    /// VTK structures to construct pick marker.
//    vtkSmartPointer<vtkActor> m_pick_marker_actor;
//    vtkSmartPointer<vtkCellArray> m_pick_marker_lines;
//    vtkSmartPointer<vtkPolyDataMapper> m_pick_marker_mapper;
//    vtkSmartPointer<vtkPoints> m_pick_marker_points;
//    vtkSmartPointer<vtkPolyData> m_pick_marker_poly_data;

    
    
    /// Constructor.
    vmxGUIRendererSignalViewer(vmxGUIMainWidget *main_widget);
    
    /// Destructor.
    virtual ~vmxGUIRendererSignalViewer();
    
    /// Helper method to enter data to vtkContextMouseEvent class.
    void CreateMouseEvent(vtkContextMouseEvent &event, int button);
    
    /// Set the input data.
    void AddInputData(vtkDoubleArray *x_array, vtkDoubleArray *y_array, const char *name, const char *x_label, const char *y_label, unsigned char red, unsigned char green, unsigned char blue);
        
    /// Get VTK interactor style for this renderer (if none specified, leave to return value NULL).
    virtual vtkInteractorStyle* GetVTKInteractorStyle() { return NULL; };
    
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
    
    /// Remove all added signals.
    void RemoveAllData();

    /// Updates display of the signals. Use this to actually render signal to screen (when adding signals is done).
    void Update();
    
    /// Convert event position (e.g. mouse click position) to coordinates (y and y value) in the plot.
    void ViewportPositionToPlotCoordinates(int viewport_pos_x, int viewport_pos_y, double &output_plot_coordinate_x, double &output_plot_coordinate_y);
    
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
