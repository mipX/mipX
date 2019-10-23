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



// PROBLEMS:
// SLOT is called twice with one event, check that it's called only once.
// malloc error in destructor when exiting app.å


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

#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageReslice.h>
#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkMatrix4x4.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkTransform.h>
#include <vtkPolyDataMapper.h>
#include <vtkCursor3D.h>
#include <vtkCamera.h>
#include <vtkXYPlotActor.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkDataObject.h>
#include <vtkContextActor.h>
#include <vtkContextScene.h>
#include <vtkAbstractArray.h>
#include <vtkPointData.h>



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
    
    
    double xmin = 0.;
    double ymin = 0.;
    double xmax = 0.;
    double ymax = 0.;
    
    
    double scale = 1;
    
    int int_scale = 1;
    int max_int_scale = 10;
    
    double visible_range_x[2], visible_range_y[2];
    
    /// List of singal containers.
    mxList< vmxGUIRendererSignalViewerSignalContainer > m_signals;
    
    /// VTK plot actor.
    vtkSmartPointer<vtkXYPlotActor> m_plot_actor;

    /// Setup the text and add it to the renderer
    vtkSmartPointer<vtkTextActor> m_coordinate_text_actor;
    
    // VTK data object.
    vtkSmartPointer<vtkDataObject> m_data_object;
    
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
    
    /// Set the input data.
    void AddInputData(vtkDoubleArray *x_array, vtkDoubleArray *y_array, const char *name, const char *x_label, const char *y_label, unsigned char red, unsigned char green, unsigned char blue);
        
    /// Get VTK interactor style for this renderer (if none specified, leave to return value NULL).
    virtual vtkInteractorStyle* GetVTKInteractorStyle() { return NULL;} ;
    
    /// Executed on mouse move event.
    virtual void OnMouseMove();
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelBackward();
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelForward();
    
    /// Executed when left button is released.
    virtual void OnLeftButtonUp() {};
    
    /// Executed when left button is released. Re-implemented to detect double-click.
    virtual void OnLeftButtonDown() {};
    
    /// Executed when key is pressed.
    virtual void OnKeyPress();
    
    /// Remove all added signals.
    void RemoveAllData();

    /// Updates display of the signals. Use this to actually render signal to screen (when adding signals is done).
    void Update();
    
    /// Convert event position (e.g. mouse click position) to coordinates (y and y value) in the plot.
    void ViewportPositionToPlotCoordinates(int viewport_pos_x, int viewport_pos_y, double &output_plot_coordinate_x, double &output_plot_coordinate_y);
    
    /// Zoom method - this version resets the range of values. The problem is that some lines will not be plotted because their points fall outside the range.
    void Zoom1(double plot_value_x, double plot_value_y, double x_min, double x_max, double y_min, double y_max, double zoom_scale);

    /// Zoom method - this version really zooms in/out on the scene. The problem here is that the axis and labels are not visible at all scales. However,
    /// there is no problem with line visibility as in the version 1.
    void Zoom2(double plot_value_x, double plot_value_y, int renderer_size_x, int renderer_size_y, int click_pos_x,int click_pos_y, double int_zoom_scale);
    
};



#endif
