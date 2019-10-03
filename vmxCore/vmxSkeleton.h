/*=========================================================================
 
 Program:   mipx
 Module:    vmxSkeleton.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxSkeleton_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxSkeleton_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxSkeleton_EXPORTS
            #define vmxSkeleton_API __declspec(dllexport)
        #else
            #define vmxSkeleton_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxSkeleton_EXPORTS
            #define vmxSkeleton_API __attribute__((visibility("default")))
        #else
            #define vmxSkeleton_API
        #endif
    #endif
#endif



#ifndef vmxSkeleton_H
    #define vmxSkeleton_H


#include <vtkCommand.h>
#include <vtkSplineWidget.h>
#include <vtkRenderer.h>
#include <vtkTubeFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCardinalSpline.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkLookupTable.h>
#include <vtkRendererCollection.h>
#include <vtkSTLWriter.h>
#include <vtkGenericDataObjectWriter.h>
#include <vtkPicker.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>

#include "mxDataObjectFactory.h"
#include "mxSkeleton.h"


class vmxSkeleton;//predeclare the class;


/// Class for visualization of 3D skeleton structures on VTK level.

class vmxSkeleton_API vmxSkeleton3D
{
public:

	/// Pointer to the containing skeleton VTK object.
	vmxSkeleton *m_vtk_skeleton;

	/// VTK Actor/Points/Cells/PolyData/Tube_Filter/Mapper of the skeleton (needed to construct actor).
	vtkSmartPointer<vtkActor> m_actor; 
	vtkSmartPointer<vtkPoints> m_points;
	vtkSmartPointer<vtkCellArray> m_lines;
	vtkSmartPointer<vtkPolyData> m_poly_data;
	vtkSmartPointer<vtkCleanPolyData> m_clean_poly_data;
	vtkSmartPointer<vtkTubeFilter> m_tube_filter;
	vtkSmartPointer<vtkPolyDataMapper> m_mapper;

	/// Scalars loaded from scalar values in mxSkeleton (size is set to maximum number of scalar values in mxSkeleton).
	mxArray< vtkSmartPointer< vtkFloatArray > > m_scalars_vtk;



	/// Constructor.
	vmxSkeleton3D();

	/// Destructor.
	~vmxSkeleton3D();

	/// Set up the object. MUST BE CALLED AFTER CONSTRUCTOR.
	void SetUpObject(vmxSkeleton *skeleton_vtk);

	/// Reset the object to an initial state (just after construction).
	void Reset();

	/// Set/Get skeleton VTK actor visibility.
	int IsVisible();
	void SetVisibility(int is_visible);

};





//----------------------------------------------------------------------------------------------




/// Class for visualization of 4D skeleton structures (inherited from mxSkeleton) on VTK level.

class vmxSkeleton_API vmxSkeleton : public mxSkeleton
{
protected:

	/// Data.
	mxArray< vmxSkeleton3D > m_vtk_data;

	/// Time index.
	unsigned int m_index_time;

	/// Indicator of visibility.
	int m_is_visible;
    
    /// Indicator if scalar coloring is on. If not a single color is used.
    int m_is_coloring_scalars;

	/// Interactor to which we bind the object.
	vtkRenderWindowInteractor *m_interactor;
    
    /// Renderer to which the VTK actors are added.
    vtkRenderer *m_renderer;

	/// Minimum value of tube radius (because radius can be changing).
	double m_tube_radius_min;

	/// Spline widget objects. Spline widget is used to interacively modify skeleton.
	vtkSmartPointer<vtkSplineWidget> m_spline_widget;//Spline widget with possibility to modify positions
	vtkSmartPointer<vtkPoints> m_spline_widget_points;//Points used to create spline widget
	
	/// We will allow only one link to be represented with spline widget at a time (easier to use and 
	/// less computationally extensive), and this is the pointer to that link
	mxSkeletonLink* m_link_represented_with_spline_widget;

	/// Picked link indicator VTK objects (outline (box) actor to show the picked link).
	vtkSmartPointer<vtkPoints> m_picked_link_outline_points;
	vtkSmartPointer<vtkCellArray> m_picked_link_outline_lines;
	vtkSmartPointer<vtkPolyData> m_picked_link_outline_poly_data;
	vtkSmartPointer<vtkPolyDataMapper> m_picked_link_outline_mapper;
	vtkSmartPointer<vtkActor> m_picked_link_outline_actor;

	/// The picked link.
	mxSkeletonLink* m_picked_link;


public:

	/// Constructor.
	vmxSkeleton();

	/// Destructor.
	~vmxSkeleton();

	/// Reset the object to an initial state (just after construction).
	void Reset();
    
    /// Copy entire object from input.
    virtual int Copy(vmxSkeleton *p_skel);
    
    /// Copy data members of input object if the input object and this object are of the same type.
    virtual int CopyFromDataObject(mxDataObject *data_object);
    
    /// Exact copy.
    int CopyFrom(vmxSkeleton *p_skel);
    int CopyFrom(vmxSkeleton &skel);
    
	/// Indexing for Time.
	vmxSkeleton3D& operator()(unsigned int t);

	/// Get the skeleton VTK actor.
	vtkActor* GetActor(unsigned int t=0);

	/// Get the actor of the outline of the picked link.
	vtkActor* GetPickedLinkOutlineActor();
    
    /// Set the visibility of the object component for VTK visualization.
    /// E.g. a 3D image has 3 visualization components: axial, sagittal and coronal planes.
    virtual int GetVisibilityOfComponent(int component_index) { return m_is_visible; };

	/// Check if the skeleton contains the input actor.
	int HasActor(vtkActor *actor);

	/// Set/Get skeleton VTK actor visibility.
	int IsVisible();
	void SetVisibility(int is_visible);

	/// Set/Get VTK Interactor.
	void SetInteractor(vtkRenderWindowInteractor *interactor);
	vtkRenderWindowInteractor* GetInteractor();

	/// Check if the VTK interactor has been set (pointer to interactor is not NULL).
	int IsInteractorSet();

	/// Set time index with rendering the scene.
	void SetTimeIndex(unsigned int t);

	/// Set time index WITHOUT rendering the scene (use when multiple objects need to be updated before the scene is rendered).
	void SetTimeIndex_NoRender(unsigned int t);

	/// Update the VTK structures of the skeleton. This is needed if the underlying mxSkeleton was 
	/// changed in processing or created the first time.
	void Update(unsigned int t);
	void Update();

	/// Update only the VTK objects refering to scalars. Use this if only coloring of the skeleton is changed.
	int UpdateScalars(unsigned int scalar_index, unsigned int t);
	int UpdateScalars(unsigned int scalar_index);

	/// Export mesh (tube filter made poly data) to STL file.
	/// Warning: STL cannot save scalar values of the mesh, only its geometry.
	void ExportMeshToSTLFile(const char *file_name, unsigned int t=0);

	/// Export mesh (tube filter made poly data) to VTK files.
	void ExportMeshToVTKFile(const char *file_name, unsigned int t=0);
	
	/// Converts the VTK WORLD mxPoint (in case of picking, for example) to local Skeleton positions (for calculation 
	/// of distances, best paths...). This is done by subtracting the origin of the VTK Skeleton and dividing by spacing.
	void ConvertWorldPositionToSkeletonPosition(mxPoint &input_world_position, mxPoint &output_skeleton_position);

	/// Adds the 'spline_widget' to the previously set interactor with SetInteractor() method.
	/// WARNING!!! Initialize the interactor before calling this function (interactor->Initialize()) 
	/// in order for this function to plot the widget right!!! 
	int AddSplineWidgetToInteractor();

	/// Goes through all the SkeletonLinks until it finds the link that has the position closest to the picked position and markes 
	/// it with outline box. CHECKS IF THE PICKED 'input_actor' BELONGS TO THIS vmxSkeleton. Fail 0, success 1.
	int MarkSkeletonLinkForGivenActorAndPickWorldPosition(vtkActor *input_actor, mxPoint &pick_position);

	/// Calculates pruning order for metrics given by stub link removal order number.
	void PruningOrder();

	/// For the given input node calculates the "distance" (as number of steps) from the given node.
	void HierarchyFromRoot(mxSkeletonLink* root);

	/// Performs HierarchyFromRoot(mxSkeletonLink*) using picked link as input.
	void HierarchyFromRoot();

	/// Represent the picked Skeleton Link (defined from the picked input actor) as spline widget in the input interactor. If the link is found
	/// the pointer to it 'link_vtk_represented_with_spline_widget' is set indicating that it has a spline widget representation (normal actor 
	/// is no longer visible). WARNING!!! Initialize the interactor before calling this function (interactor->Initialize()) 
	/// in order for this function to plot the widget right!!! 
	int RepresentPickedLinkAsSplineWidget();

	/// For the SkeletonLink 'm_link_represented_with_spline_widget' removes its representation as spline widget and puts back 
	/// its normal gray actor. The pointer 'link_vtk_represented_with_spline_widget' is set to NULL.
	int TurnOffRepresentationOfMemorizedLinkAsSplineWidget();

	/// Turn On/Off the representation of a link with the spline widget.
	void ToggleLinkAsSplineWidgetRepresentation();
	
	/// Set coloring of skeleton to scalars defined by input tag. 
	void SetColoringScalars(mxString &tag);

	/// Set a single color to skeleton. 
	void SetColoringToSingleColor(double red=1, double green=1, double blue=1);
    
    /// Set the renderer for VTK visualization.
    virtual void SetRenderer(vtkRenderer *renderer);
    
    /// Set the range of the visualization lookup table for scalars with given index.
    int SetScalarVisualizationRange(unsigned int scalar_index, double range_lower_value, double range_upper_value);
    
    /// Set the visibility of the object component for VTK visualization.
    /// E.g. a 3D image has 3 visualization components: axial, sagittal and coronal planes.
    virtual void SetVisibilityOfComponent(int component_index, int is_visible);

	/// If the points are entered, creates the path with cardinal splines by inserting 'interpolation_coefficient' positions between every 
	/// two existing positions in every link (2<interpolation_coefficient<11). If no points were entered returns 0, else returns 1.
	int InterpolateWithCardinalSpline(int interpolation_coefficient);

};




typedef mxDataObjectFactoryT< vmxSkeleton > vmxSkeletonFactory;




#endif
