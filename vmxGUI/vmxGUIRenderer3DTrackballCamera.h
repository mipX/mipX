/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRenderer3DTrackballCamera.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxGUIRenderer3DTrackballCamera_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIRenderer3DTrackballCamera_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIRenderer3DTrackballCamera_EXPORTS
            #define vmxGUIRenderer3DTrackballCamera_API __declspec(dllexport)
        #else
            #define vmxGUIRenderer3DTrackballCamera_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIRenderer3DTrackballCamera_EXPORTS
            #define vmxGUIRenderer3DTrackballCamera_API __attribute__((visibility("default")))
        #else
            #define vmxGUIRenderer3DTrackballCamera_API
        #endif
    #endif
#endif



#ifndef vmxGUIRenderer3DTrackballCamera_H
    #define vmxGUIRenderer3DTrackballCamera_H


#include "mxString.h"
#include "vmxGUIWidget.h"
#include "vmxGUIEventDetector.h"

#include <vtkCellArray.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyle.h>
#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>


class vmxGUIRenderer3DTrackballCamera;



/// Interactor style trackball camera in 3D space.

class vmxGUIInteractorStyle3DTrackballCamera : public vtkInteractorStyleTrackballCamera
{
    
public:
    
    /// Pointer to the renderer.
    vmxGUIRenderer3DTrackballCamera *m_gui_renderer;
    
    /// Detector of double click event.
    vmxDoubleClickDetector m_double_click_detector;
    
    /// Constructor.
    vmxGUIInteractorStyle3DTrackballCamera();
    
    /// Destructor.
    virtual ~vmxGUIInteractorStyle3DTrackballCamera();
    
    /// Create new object instance.
    static vmxGUIInteractorStyle3DTrackballCamera* New();
    
    /// Re-implemented to override the existing keypress char functionality.
    virtual void OnChar()
    {
        //do nothing here.
    };
    
    /// Re-imaplemented.
    virtual void OnKeyPress();
    
    /// Re-implemented to detect double-click.
    virtual void OnLeftButtonDown();
    
    /// Re-implemented to check commands first.
    virtual void OnLeftButtonUp();
    
};





/// Renderer with an interactor style trackball camera.

class vmxGUIRenderer3DTrackballCamera_API vmxGUIRenderer3DTrackballCamera : public vmxGUIRenderer
{

public:
    
    /// Interactor style assigned to this renderer.
    vtkSmartPointer< vmxGUIInteractorStyle3DTrackballCamera > m_interactor_style;
    
    
    vtkSmartPointer< vtkCellPicker > m_cell_picker;
    
    /// Current indexes for temporal and slice view.
    unsigned int m_index_time;
    unsigned int m_index_slice;
    
    /// Picked indexes in order C,R,S,T.
    int m_picked_indexes[4];
    
    /// VTK structures to construct pick marker.
    vtkSmartPointer<vtkActor> m_pick_marker_actor;
    vtkSmartPointer<vtkCellArray> m_pick_marker_lines;
    vtkSmartPointer<vtkPolyDataMapper> m_pick_marker_mapper;
    vtkSmartPointer<vtkPoints> m_pick_marker_points;
    vtkSmartPointer<vtkPolyData> m_pick_marker_poly_data;
    
    /// VTK structures for Glyph visualization of picked positions.
    vtkSmartPointer<vtkPoints> m_pp_points;
    vtkSmartPointer<vtkPolyData> m_pp_poly_data;
    vtkSmartPointer<vtkSphereSource> m_pp_sphere_source;
    vtkSmartPointer<vtkGlyph3D> m_pp_glyph;
    vtkSmartPointer<vtkPolyDataMapper> m_pp_mapper;
    vtkSmartPointer<vtkActor> m_pp_actor;
    
    
    /// Command text actor for reset of view.
    vtkTextActor *m_command_reset_view;
    
    /// Command text actor for adding seed point.
    vtkTextActor *m_command_seeds_add_new;
    
    /// Command text actor for clearing seed points.
    vtkTextActor *m_command_seeds_clear_all;
    
    /// Command text actor for toggle view of seed points.
    vtkTextActor *m_command_seeds_toggle_view;
    
    /// Command text actor for slice and time index display.
    vtkTextActor *m_command_index_slice_time;

    
    
    /// Constructor.
    vmxGUIRenderer3DTrackballCamera(vmxGUIMainWidget *main_widget);
    
    /// Destructor.
    virtual ~vmxGUIRenderer3DTrackballCamera();
    
    /// Show indexing properties.
    virtual void DisplayProperties();
    
    /// Get picked indexes.
    int* GetPickedIndexes() { return m_picked_indexes; };
    
    /// Get VTK interactor style for this renderer (if none specified, leave to return value NULL).
    virtual vtkInteractorStyle* GetVTKInteractorStyle() { return m_interactor_style;} ;
    
    /// Internal method to create the cross-hairs for marking the pick positions.
    void internal_CreateCrossHairs(int size=20);
    
    /// Check the visibility of position actors.
    int IsPickedPositionsActorVisible() { return m_pp_actor->GetVisibility(); };
    
    /// Executed on mouse move event.
    virtual void OnMouseMove() {};
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelBackward() {};
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelForward() {};
    
    /// Executed when left button is released.
    virtual void OnLeftButtonUp() {};
    
    /// Executed when left button is released. Re-implemented to detect double-click.
    virtual void OnLeftButtonDown() {};
    
    /// Executed when key is pressed.
    virtual void OnKeyPress() {};
    
//    /// Perform picking from input world coordinates and indexes. In general, picking can be performed either only from world coordinates (and time) or only from
//    /// indexes, but we use both here in order to avoid recomputing when pick is called from a function or object which already calculated both world coordinates
//    /// and indexes. This way the method is suitable for being used in slot methods.
//    void Pick(int index_c, int index_r, int index_s, int t_index = 0);
    
    
//    /// Set index for slice.
//    void SetIndexSlice(unsigned int slice_index);
    
    /// Set visibility of the pick marker.
    void SetPickMarkerVisibility(int is_visible) { m_pick_marker_actor->SetVisibility(is_visible); };
    
    /// Set visibility of picked positions.
    void SetVisibilityOfPickedPositions(int is_visible) { m_pp_actor->SetVisibility(is_visible); }
    
    /// GetPicked position in world coordinates.
    void GetPickedWorldPosition(double &x, double &y, double &z);

//    /// Predefined slot method to set the slice index of the receiver object from the int value passed as data in the connection.
//    static int Slot_SetIndexSlice(vmxGUIConnection *connection);
//
//    /// Predefined slot method to pick the position on image and set the pick marker to it.
//    /// The method takes int values for picked indexes and double for the given world coordinates (to avoid recomputation).
//    static int Slot_Pick(vmxGUIConnection *connection);
    
    
    /// Update visualization of picked positions.
    void UpdatePickedPositionsVisualization();
    
};



#endif
