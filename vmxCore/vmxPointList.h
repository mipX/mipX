/*=========================================================================
 
 Program:   mipx
 Module:    vmxPointList.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxPointList_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define vmxPointList_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxPointList_EXPORTS
            #define vmxPointList_API __declspec(dllexport)
        #else
            #define vmxPointList_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxPointList_EXPORTS
            #define vmxPointList_API __attribute__((visibility("default")))
        #else
            #define vmxPointList_API
        #endif
    #endif
#endif



#ifndef vmxPointList_H
    #define vmxPointList_H



#include "mxArray.h"
#include "mxDataObject.h"
#include "mxDataObjectFactory.h"
#include "mxList.h"
#include "mxPoint.h"
#include "mxPointList.h"
#include "mxString.h"

#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkCardinalSpline.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkLine.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkPointSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkStringArray.h>



/// VTK level calss for visualization of point list.

class vmxPointList_API vmxPointList : public mxPointList
{
    
public:

	/// VTK objects for 3D visualization of points.
	vtkSmartPointer< vtkPoints > m_vtk_points;
    vtkSmartPointer< vtkPolyData > m_vtk_poly_data;
	vtkSmartPointer< vtkSphereSource > m_vtk_sphere_source;
    vtkSmartPointer< vtkGlyph3D > m_vtk_glyph;
	vtkSmartPointer< vtkPolyDataMapper > m_vtk_mapper;
    vtkSmartPointer< vtkActor > m_vtk_actor;
    
    /// Scalars values taken from mxPointList.
    mxArray< vtkSmartPointer< vtkFloatArray > > m_vtk_scalars;

	/// VTK objects for visualization of labels (of points in 3D).
	vtkSmartPointer< vtkStringArray > m_vtk_label_labels;
    vtkSmartPointer< vtkPointSource > m_vtk_label_point_source;
	vtkSmartPointer< vtkPointSetToLabelHierarchy > m_vtk_label_hierarchy_filter;
	vtkSmartPointer< vtkLabelPlacementMapper > m_vtk_label_mapper;
	vtkSmartPointer< vtkActor2D > m_vtk_label_actor;

	/// VTK objects for outline box of the picked point.
	vtkSmartPointer< vtkPoints > m_vtk_pick_outline_points;
	vtkSmartPointer< vtkCellArray > m_vtk_pick_outline_lines;
	vtkSmartPointer< vtkPolyData > m_vtk_pick_outline_poly_data;
	vtkSmartPointer< vtkPolyDataMapper > m_vtk_pick_outline_mapper;
	vtkSmartPointer< vtkActor > m_vtk_pick_outline_actor;

	/// Picked point.
	mxPointListElement *m_picked_list_element;
	
    /// VTK renderer for this object.
    vtkRenderer *m_renderer;
    
    /// Visibility indicator.
    int m_is_visible;

    /// Visibility indicator for the outline of the picked point.
    int m_is_visible_picked_point;

    /// Visibility indicator for the labels.
    int m_is_visible_labels;
    
//    /// Defines the lookup table scalar range, first value as min and second as max.
//    /// If min>max, coloring will be done by min and max values of scalars. This is default.
//    double m_lookup_table_scalar_range[2];

    

	/// Constructor.
	vmxPointList();
    
    /// Destructor.
	virtual ~vmxPointList();
    
    /// Copy from input object.
    int Copy(vmxPointList *l);
    
    /// Get VTK renderer.
    vtkRenderer* GetRenderer() { return m_renderer; };

	/// Get VTK actor.
    vtkActor* GetActor() { return m_vtk_actor; };
    
    /// Get last picked point list element.
    mxPointListElement* GetPickedPointListElement() { return m_picked_list_element; };
    
    /// Re-implemented from mxDataObject.
    int GetVisibilityOfComponent(int component_index);
    
    /// Check if input actor belongs to this object.
    int HasActor(vtkActor *actor);
    
    /// Interpolates the existing point positions to get the given 'new_number_of_points' along the path of the list.
    int InterpolateUsingCardinalSpline(int new_number_of_points);

	/// Check if the point list is visible.
    int IsVisible() { return m_is_visible; };
    
    /// Check if the labels of points are visible.
    int IsVisibleLabels() { return m_is_visible_labels; };
    
    /// Check if the picked point outline is visible.
    int IsVisiblePickedPoint() { return m_is_visible_picked_point; };

    /// Mark the closes point in the list as picked for the input poisition of the vtkActor.
    /// If the input_actor belongs to this object return 1, if not return 0.
    int MarkPickedPoint(vtkActor *input_actor, mxPosition *pick_position);
    
    /// Delete picked position.
    void RemovePickedPoint();
    
    /// Reset the object to its initial state (just after default construction).
    void Reset();
    
    /// Set color mapping to scalars defined by input tag string.
    void SetColorMappingToScalars(const char *tag);
    
    /// Set color mapping ot a single color defined by input RGB values.
    void SetColorMappingToSingleColor(double red=1, double green=1, double blue=1);
    
    /// Set VTK renderer for this object.
    virtual void SetRenderer(vtkRenderer *renderer);
    
    /// Set the scalar visualization range (lookup table will color values in this range).
    /// If lower value is equal or higher than upper value, the whole range of scalars will be used for visualization (so range [scalar_min, scalar_max]).
    int SetScalarVisualizationRange(unsigned int scalar_index, double range_lower_value, double range_upper_value);
    
    /// Set the visibility of the point list.
	void SetVisibility(int is_visible);
    
    /// Re-implemented from mxDataObject.
    void SetVisibilityOfComponent(int component_index, int is_visible);
    
    /// Set visibility of the picked point outline.
	void SetVisibilityOfPickedPoint(int is_visible);

    /// Set visibility of the labels of points.
	void SetVisibilityOfLabels(int is_visible);

	/// Update VTK point structures - use when the points in the list change.
	void Update();

	/// Update VTK scalar objects only. Use this method to perform recoloring of points based on different scalars.
	int UpdateScalars(unsigned int scalar_index);


};



typedef mxDataObjectFactoryT<vmxPointList> vmxPointListFactory;



#endif
