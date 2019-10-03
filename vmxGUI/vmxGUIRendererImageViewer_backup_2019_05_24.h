/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRendererImageViewer.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxGUIRendererImageViewer_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIRendererImageViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIRendererImageViewer_EXPORTS
            #define vmxGUIRendererImageViewer_API __declspec(dllexport)
        #else
            #define vmxGUIRendererImageViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIRendererImageViewer_EXPORTS
            #define vmxGUIRendererImageViewer_API __attribute__((visibility("default")))
        #else
            #define vmxGUIRendererImageViewer_API
        #endif
    #endif
#endif



#ifndef vmxGUIRendererImageViewer_H
    #define vmxGUIRendererImageViewer_H


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
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
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


class vmxGUIRendererImageViewer;


/// Interactor style for 2D image viewing.

class vmxGUIInteractorStyleImageViewer : public vtkInteractorStyleImage
{
    
public:
    
    /// Pointer to the renderer.
    vmxGUIRendererImageViewer *m_gui_renderer;
    
    /// Constructor.
    vmxGUIInteractorStyleImageViewer();
    
    /// Destructor.
    virtual ~vmxGUIInteractorStyleImageViewer();
    
    /// Create new object instance.
    static vmxGUIInteractorStyleImageViewer* New();
    
    /// Re-implemented to override the existing keypress char functionality.
    virtual void OnChar()
    {
        //do nothing here.
    };
    
    /// Executes on mouse move.
    virtual void OnMouseMove();
    
    /// Executes when left button is released.
    virtual void OnLeftButtonUp();
    
    /// Re-implemented to detect double-click.
    virtual void OnLeftButtonDown();
    
    /// Re-implemented to use specific keys.
    virtual void OnKeyPress();
    
};





/// Renderer with an interactor style for image viewing.

class vmxGUIRendererImageViewer_API vmxGUIRendererImageViewer : public vmxGUIRenderer
{

public:
    
    /// Image being visualized.
    vtkImageData *m_image_data;
    
    /// Interactor style assigned to this renderer.
    vtkSmartPointer< vmxGUIInteractorStyleImageViewer > m_interactor_style;
    
    /// Image actor.
    vtkSmartPointer< vtkImageActor > m_image_actor;
    
    /// Image reslice.
    vtkSmartPointer< vtkImageReslice > m_reslice;
    
    /// Current index of plane.
    unsigned int m_index_time;
    unsigned int m_index_slice;
    
    /// Direction matrix for transversal cut plane.
    double m_transversal_direction_matrix[16];
    
    /// Direction matrix for coronal cut plane.
    double m_coronal_direction_matrix[16];
    
    /// Direction matrix for sagittal cut plane.
    double m_sagittal_direction_matrix[16];
    
    /// VTK direction matrix
    vtkSmartPointer< vtkMatrix4x4 > m_reslice_axes;
    
    /// VTK Mapper to map colors to image using a lookup table.
    vtkSmartPointer< vtkImageMapToColors > m_image_map_to_colors;
    
    /// Lookup table.
    vtkSmartPointer<vtkLookupTable> m_lookup_table;
        
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

//    /// Array that stores picked positions per index (for easy visualization and access).
//    mxArray< mxList< vmxGUIRendererPosition* > > m_picked_positions_per_index;
    
    /// Orientation types of viewer plane.
    enum vmxGUIRendererImageViewerPlaneOrientation
    {
        ORIENTATION_TRANSVERSAL,
        ORIENTATION_CORONAL,
        ORIENTATION_SAGITTAL
    };
    
    
    /// Image being overlayed on the first image.
    vtkImageData *m_overlay_image_data;
    
    /// Image actor of overlayed image.
    vtkSmartPointer< vtkImageActor > m_overlay_image_actor;
    
    /// Image reslice for overlayed image.
    vtkSmartPointer< vtkImageReslice > m_overlay_reslice;
    
    /// VTK direction matrix for overlayed image.
    vtkSmartPointer< vtkMatrix4x4 > m_overlay_reslice_axes;
    
    /// VTK Mapper to map colors to overlayed image using a lookup table.
    vtkSmartPointer< vtkImageMapToColors > m_overlay_image_map_to_colors;
    
    /// Lookup table for overlayed image.
    vtkSmartPointer<vtkLookupTable> m_overlay_lookup_table;

    
    /// Color mapping types.
    enum vmxGUIRendererImageViewerPlaneColorMapping
    {
        COLOR_OPAQUE,
        GRAY_OPAQUE,
        COLOR_TRANSPARENT,
        GRAY_TRANSPARENT,
        BINARY_TRANSPARENT
    };

    // Current mapping of image planes
    vmxGUIRendererImageViewerPlaneColorMapping m_mapping;
    
    // Current mapping of overlayed image planes
    vmxGUIRendererImageViewerPlaneColorMapping m_mapping_of_overlayed_image;

    
    /// Current orientation of the cut plane.
    vmxGUIRendererImageViewerPlaneOrientation m_plane_orientation;
    
    
    /// Constructor.
    vmxGUIRendererImageViewer();
    
    /// Constructor.
    vmxGUIRendererImageViewer(vmxGUIMainWidget *main_widget);
    
    void SetMainWidget(vmxGUIMainWidget *main_widget)
    {
        m_main_widget = main_widget;
        m_clip_board = main_widget->GetClipBoard();
    };
    
    /// Destructor.
    virtual ~vmxGUIRendererImageViewer();
    
//    /// Re-implemented adding of pick positions.
//    virtual void AddPickPosition(double x, double y, double z, double v);
    
    /// Fit the image actor to screen.
    void FitImageToScreen();
    
    /// Get picked indexes.
    int* GetPickedIndexes() { return m_picked_indexes; };
    
    /// Get image indexes for given local coordinates.
    /// IMPORTANT: one of the indexes will always be equal to m_index_slice, the other 2 are calculated from local positions.
    /// This is because one input local position value will always be 0.
    void GetIndexesForLocalCoordinates(double ws, double wr, double wc, int &output_s, int &output_r, int &output_c);

    /// Get image indexes for given local coordinates.
    /// IMPORTANT: one of the indexes will always be equal to m_index_slice, the other 2 are calculated from local positions.
    /// This is because one input local position value will always be 0.
    void GetIndexesForWorldCoordinates(double ws, double wr, double wc, int &output_s, int &output_r, int &output_c);

    /// Get local coordinates for given world coordinates.
    void GetLocalCoordinatesForWorldCoordinates(double ws, double wr, double wc, double &output_ls, double &output_lr, double &output_lc);
    
    /// Get world coordinates for given local coordinates.
    /// IMPORTANT: one of the world coordinates is calculated from an indexe m_index_slice, the other 2 are calculated from input local positions.
    /// This is because one input local position value will always be 0.
    void GetWorldCoordinatesForLocalCoordinates(double ls, double lr, double lc, double &output_ws, double &output_wr, double &output_wc);
    
    /// Get VTK interactor style for this renderer (if none specified, leave to return value NULL).
    virtual vtkInteractorStyle* GetVTKInteractorStyle() { return m_interactor_style;} ;
    
    //    virtual void OnChar() { };//do nothing here.
    
    /// Internal method to create the cross-hairs for marking the pick positions.
    void internal_CreateCrossHairs(int size=20);
    
    /// Check is the picked positions are visible.
    int IsPickedPositionsActorVisible();
    
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
    
    /// Executed when left button is released.
    virtual void OnLeftButtonDrop() { this->GetConnectionManager()->Execute(LeftButtonDropEvent, this); };// std::cout<<"  "<<this->GetClipBoard()->m_data_text.Get_C_String()<<" dropped! "; };
    
    /// Executed when key is pressed.
    virtual void OnKeyPress() {};
    
    /// Perform picking from input world coordinates and indexes. In general, picking can be performed either only from world coordinates (and time) or only from
    /// indexes, but we use both here in order to avoid recomputing when pick is called from a function or object which already calculated both world coordinates
    /// and indexes. This way the method is suitable for being used in slot methods.
    void Pick(int index_c, int index_r, int index_s, int t_index = 0);
    
    /// Set slicing direction to transversal.
    void SetDirectionToTransversal();
    
    /// Set slicing direction to coronal.
    void SetDirectionToCoronal();
    
    /// Set slicing direction to sagittal.
    void SetDirectionToSagittal();
    
    /// Set the image data.
    void SetImageData(vtkImageData *image_data);
    
    /// Set index for slice.
    void SetIndexSlice(unsigned int slice_index);
    
    /// Color the opaque image plane. If you want to use lookup table with full scalar range, set
    /// 'is_using_full_range' to non zero, otherwise [minimim,maximum] range of pixel values is used.
    void SetMappingToColor(int is_full_value_range_used = 0);
    
    /// Use grayscale opaque image plane. If you want to use lookup table with full scalar range, set
    /// 'is_using_full_range' to non zero, otherwise [minimim,maximum] range of pixel values is used.
    void SetMappingToGrayScale(int is_full_value_range_used = 0);
    
    /// Set the overlayed image data.
    void SetOverlayedImageData(vtkImageData *overlayed_image_data);

    /// Set the mapping of the overlayed image to show segmented parts of the image.
    void SetOverlayedImageMappingToBinary(unsigned int threshold = 1);
    
    /// Set the mapping of the overlayed image to transparent color.
    void SetOverlayedImageMappingToColor(double opacity);

    /// Set the mapping of the overlayed image to transparent grayscale.
    void SetOverlayedImageMappingToGrayScale(double opacity);
    
    /// Set visibility of picked positions.
    void SetVisibilityOfPickedPositions(int is_visible);
    
    /// Update visualization of picked positions.
    void UpdatePickedPositionsVisualization();

    /// Predefined slot method to set the slice index of the receiver object from the int value passed as data in the connection.
    static int Slot_SetIndexSlice(vmxGUIConnection *connection);
    
    /// Predefined slot method to pick the position on image and set the pick marker to it.
    /// The method takes int values for picked indexes and double for the given world coordinates (to avoid recomputation).
    static int Slot_Pick(vmxGUIConnection *connection);
    
};



#endif
