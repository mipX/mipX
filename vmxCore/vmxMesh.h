/*=========================================================================
 
 Program:   mipx
 Module:    vmxMesh.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxMesh_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxMesh_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxMesh_EXPORTS
            #define vmxMesh_API __declspec(dllexport)
        #else
            #define vmxMesh_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxMesh_EXPORTS
            #define vmxMesh_API __attribute__((visibility("default")))
        #else
            #define vmxMesh_API
        #endif
    #endif
#endif



#ifndef vmxMesh_H
    #define vmxMesh_H



#include "mxArray.h"
#include "mxImage.h"
#include "mxDataObject.h"
#include "mxDataObjectFactory.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



#include <vtkActor.h>
#include <vtkContourFilter.h>
#include <vtkGenericDataObjectWriter.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkMatrix4x4.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkTransform.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkXMLPolyDataReader.h>






#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




/// Class for 3D mesh manipulation. It is a time unit element for vmxMesh.

class vmxMesh_API vmxMesh3D
{
    
public:

	/// VTK mesh actor.
    vtkSmartPointer<vtkActor> m_actor;
        
	/// VTK poly data (actual mesh data).
    vtkSmartPointer<vtkPolyData> m_poly_data;
    
    /// Filter to get disjoint components of the poly data (mesh).
    vtkSmartPointer<vtkPolyDataConnectivityFilter> m_poly_data_connectivity_filter;

	/// VTK mapper to map poly data to actor.
	vtkSmartPointer<vtkPolyDataMapper> m_poly_data_mapper;

    
	/// Constructor.
	vmxMesh3D();

	/// Destructor.
	~vmxMesh3D();

	/// Create the actor from the existing poly data by mapping scalars from the lookup table. The table
    /// is re-generated to be in range [scalar_min_value,scalar_max_value] which should correspond to range of
    /// voxel gray values in the image from which the poly data was created from.
	void CreateActorByLookupTableScalarColoring(vtkLookupTable *lookup_table);

	/// Create the actor from the existing poly data by coloring each disconnected component of th emesh in different color.
    void CreateActorByDifferentlyColoringDisjointComponents();
    
    /// Create the polydata from the input VTK image data. The poly data is created from voxel values above the input threshold value.
    /// The Cx,Cy,Cz,Rx,Ry,Rz,Sx,Sy,Sz input values define the orientation of the mesh.
    void CreatePolyData(vtkImageData *image, int threshold, double Cx, double Cy, double Cz, double Rx, double Ry, double Rz, double Sx, double Sy, double Sz);

	/// Get the color of the mesh (when scalar visibility is off).
	void GetColor(double &red, double &green, double &blue);

	/// Get opacity (inverted transparency) of the mesh.
	double GetOpacity();
    
    /// Reset the object (return to initial state like after construction).
    void Reset();
    
    /// Set the color of the mesh (scalar visibility is turned off).
    void SetColor(double red, double green, double blue);
    
    /// Set opacity (inverted transparency) of the mesh.
    void SetOpacity(double opacity);

	/// Set visibility of the mesh.
    void SetVisibility(int is_visible);
};



/// Class for 4D mesh manipulation. It uses an array of vmxMesh3D to represent 4D surfaces.

class vmxMesh_API vmxMesh : public mxDataObject
{
    
protected:
	
	/// Data.
	mxArray< vmxMesh3D > m_data;

	/// Index 'T' (time).
	unsigned int m_index_T;
    
    /// Lookup table for scalar color mapping.
    vtkSmartPointer<vtkLookupTable> m_lookup_table;
    
    /// VTK renderer for this object.
    vtkRenderer *m_renderer;
    
	/// Visibility indicator.
	int m_is_visible;

    
public:

	/// Constructor.
	vmxMesh();

	/// Destructor.
	virtual ~vmxMesh();
    
    /// Copy data members of input object if the input object nad this object are of the same type.
    virtual int CopyFromDataObject(mxDataObject *data_object);
    
    /// Copy from input mesh.
    int Copy(vmxMesh *mesh);
    
    /// Create the actor from the existing poly data by coloring each disconnected component of th emesh in different color.
    void CreateActorByDifferentlyColoringDisjointComponents();
    
    /// Create the actor from the existing poly data by mapping scalars from the lookup table. The table
    /// is re-generated to be in range [scalar_min_value,scalar_max_value] which should correspond to range of
    /// voxel gray values in the image from which the poly data was created from.
    void CreateActorByLookupTableScalarColoring(double scalar_min_value, double scalar_max_value);
    
    /// Create the polydata from the input mxBasicImage image data. The poly data is created from voxel values above the input threshold value.
    void CreatePolyData(mxImage *image, int threshold);

    /// Get bounds of the object in 3D space. If not applicable (e.g. object is a 2D signal) return fail 0.
    virtual int GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max);
    
    /// Get the color of the mesh (when scalar visibility is off).
    void GetColor(double &red, double &green, double &blue);

//    /// Get the context menu of the object.
//    virtual qmxDataObjectContextMenu* GetContextMenu();
    
    /// Get 'T' dimension (number of time series).
    unsigned int GetDimension_T();
    
    /// Get opacity (inverted transparency) of the mesh.
    double GetOpacity();

//    /// Get the properties widget of the object.
//    virtual qmxDataObjectPropertiesWidget* GetPropertiesWidget();
    
    /// Get VTK renderer for this object.
    vtkRenderer* GetRenderer();
    
    /// Check if the actor belongs to the object.
    virtual int HasActor(vtkActor *actor);
    
    /// Check if the object is empty.
    int IsEmpty();

    /// Check visibility of the mesh.
    int IsVisible();
    
    /// Load mesh from STL file.
    int LoadSTLFile(const char *file_name);
    
    /// Load mesh from VTP file.
    int LoadVTPFile(const char *file_name);

    /// Indexing for 'T' index (time).
    vmxMesh3D& operator()(unsigned int t);

    /// Reset the object.
    virtual void Reset();
    
    /// Save mesh to STL files. Each time instance will be saved as a separate file.
    void SaveToSTLFiles(const char *file_name);
    
    /// Save mesh to VTK files. Each time instance will be saved as a separate file.
    void SaveToVTKFiles(const char *file_name);

    /// Set the color of the mesh (scalar visibility is turned off).
    void SetColor(double red, double green, double blue);
    
    /// Set T (time) index fot the mesh. Rendering of the scene will be performed.
    void SetIndex_T(unsigned int t);
    
    /// Set T (time) index fot the mesh. Rendering of the scene will NOT be performed.
    void SetIndex_T_WithoutRendering(unsigned int t);

    /// Set opacity (inverted transparency) of the mesh.
    void SetOpacity(double opacity);
    
    /// Set VTK renderer for this object.
    virtual void SetRenderer(vtkRenderer *renderer);

    /// Set visibility of the mesh.
    void SetVisibility(int is_visible);
    
    /// Set the visibility of the object component for VTK visualization (in case more components need to be visualized).
    virtual void SetVisibilityOfComponent(int component_index, int is_visible);

};



typedef mxDataObjectFactoryT<vmxMesh> vmxMeshFactory;




#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
