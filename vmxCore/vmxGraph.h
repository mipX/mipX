/*=========================================================================

Program:   mipx
Module:    vmxGraph.h

Authors: Danilo Babin, Hrvoje Leventic.
Copyright (c) Danilo Babin, Hrvoje Leventic.
All rights reserved.
See Copyright.txt

Licensed under the BSD License 2.0.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the above copyright notice for more information.

=========================================================================*/



#if defined(vmxGraph_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxGraph_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGraph_EXPORTS
            #define vmxGraph_API __declspec(dllexport)
        #else
            #define vmxGraph_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGraph_EXPORTS
            #define vmxGraph_API __attribute__((visibility("default")))
        #else
            #define vmxGraph_API
        #endif
    #endif
#endif



#ifndef vmxGraph_H
    #define vmxGraph_H


#include "mxDataObjectFactory.h"
#include "mxGraph.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


#include <vtkActor.h>
#include <vtkCardinalSpline.h>
#include <vtkCellArray.h>
#include <vtkCleanPolyData.h>
#include <vtkCommand.h>
#include <vtkFloatArray.h>
#include <vtkGenericDataObjectWriter.h>
#include <vtkLine.h>
#include <vtkLookupTable.h>
#include <vtkPicker.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkPropPicker.h>
#include <vtkSmartPointer.h>
#include <vtkSplineWidget.h>
#include <vtkSTLWriter.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTubeFilter.h>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


// Pre-declaration.
class vmxGraph;


/// VTK level class of 3D graphs.

class vmxGraph_API vmxGraph3D
{
public:

	/// Address of the container of this object (vmxGraph).
	vmxGraph *m_graph;

	/// VTK Actor/Points/Cells/PolyData/Tube_Filter/Mapper of the graph (needed to construct actor).
	vtkSmartPointer<vtkActor> m_vtk_actor;
	vtkSmartPointer<vtkCellArray> m_vtk_lines;
    vtkSmartPointer<vtkPolyDataMapper> m_vtk_mapper;
    vtkSmartPointer<vtkPoints> m_vtk_points;
	vtkSmartPointer<vtkPolyData> m_vtk_poly_data;
//	vtkSmartPointer<vtkCleanPolyData> m_vtk_clean_poly_data;
	

//	/// Scalars loaded from scalar values in mxGraph (size is set to maximum number of scalar values in mxGraph).
    vtkSmartPointer<vtkFloatArray> m_vtk_scalars; //	mxArray< vtkSmartPointer<vtkFloatArray> > m_scalars_vtk;



	/// Constructor.
	vmxGraph3D();

	/// Destructor.
	~vmxGraph3D();

	/// Check if te object is visible.
	int IsVisible();

    /// Reset the object to an initial state (just after construction).
    void Reset();
    
    /// Set visibility of the object (VTK actor).
    void SetVisibility(int is_visible);
};





//----------------------------------------------------------------------------------------------





/// VTK level class of 4D graphs

class vmxGraph_API vmxGraph : public mxGraph
{
    
protected:

    /// Array of 3D vtk graphs for each time index.
    mxArray< vmxGraph3D > m_vtk_graphs;

    /// Time index.
    unsigned int m_time_index;

    /// Indicator of visibility.
    int m_is_visible;

    /// Interactor to which we bind the object.
    vtkRenderWindowInteractor *m_vtk_interactor;
    
    /// Renderer to which we add the object.
    vtkRenderer *m_vtk_renderer;

//    /// Picked edge bounding box VTK objects (outline (box) actor shows picked edge).
//    vtkSmartPointer<vtkPoints> m_picked_edge_outline_points;
//    vtkSmartPointer<vtkCellArray> m_picked_edge_outline_lines;
//    vtkSmartPointer<vtkPolyData> m_picked_edge_outline_poly_data;
//    vtkSmartPointer<vtkPolyDataMapper> m_picked_edge_outline_mapper;
//    vtkSmartPointer<vtkActor> m_picked_edge_outline_actor;

//    /// The picked link.
//    mxGraphLink* m_picked_edge;


public:

    /// Constructor.
    vmxGraph();

    /// Destructor.
    ~vmxGraph();
    
    /// Use cardinal spline (of VTK) to curve the PATH and interpolate using a given coefficient.
    /// Notice: Considers that the graph is a PATH (one skeleton link).
    /// CAUTION: Assumes that vertices have been entered as a path sequence (so NOT raster scan order). This is usually so it the path is
    /// created from a point list.
    int CurveUsingCardinalSpline(int interpolation_coefficient);
    
    /// Export the mesh data of the graph to an STL file.
    /// Note: STL file type does not save any scalar values.
    void ExportMeshToSTL(const char *file_name);
    
    /// Export the mesh data of the graph to a VTK file.
    void ExportMeshToVTK(const char *file_name);

    /// Get VTK actor for the specified time index (default 0).
    vtkActor* GetActor(unsigned int t=0);

    /// Ceck if the input actor is contained in this obejct.
    int HasActor(vtkActor *actor);

    /// Check if te object is visible.
    int IsVisible();

    /// Time indexing.
    vmxGraph3D& operator()(unsigned int t);
    
    /// Reset the object to an initial state (just after construction).
    void Reset();
    
    /// Set the interactor for VTK visualization.
    virtual void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set the renderer for VTK visualization.
    virtual void SetRenderer(vtkRenderer *renderer);
    
    /// Set time index and render the scene.
    void SetTimeIndex(unsigned int t);

    /// Set time index without rendering the scene. Use this to avoid needless rendering during processing of multiple objects.
    void SetTimeIndexWithoutRendering(unsigned int t);

    /// Set visibility of the object (VTK actor).
    void SetVisibility(int is_visible);
    
    /// Set the visibility of the object component for VTK visualization.
    /// E.g. a 3D image has 3 visualization components: axial, sagittal and coronal planes.
    virtual void SetVisibilityOfComponent(int component_index, int is_visible);
    
    /// Update the graph VTK objects for the given time instance. Use this method to update the visualization when the underlaying mxGraph has changed.
    void Update(unsigned int t);
    
    /// Update the graph VTK objects for all time instances. Use this method to update the visualization when the underlaying mxGraph has changed.
    void Update();

    
};


typedef mxDataObjectFactoryT< vmxGraph > vmxGraphFactory;


#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif


#endif
