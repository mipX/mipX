/*=========================================================================
 
 Program:   mipx
 Module:    vmxImagePlanes.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/


// For VTK OpenGL errors see http://www.vtk.org/Wiki/VTK/OpenGL_Errors
// When transitioning existing code and applications to VTK 6.1 you may see reports for previously
// undetected OpenGL errors in your codes or ctest output which will need to be cleaned up.
// Apple Mac OSX
// On the Mac if you ask VTK to render into an "invalid drawable" you will find that all subsequent OpenGL
// calls fail (until the drawable becomes valid) with INVALID_FRAMEBUFFER_OPERATION reported. The "invalid drawable"
// issue is not new (http://vtk.markmail.org/search/?q=invalid%20drawable). However, until now one could ignore this
// as OpenGL errors were not detected and reported. Going forward you will need to fix your code or disable new gl
// error detection and reporting. There are three approaches that could be taken:
// - Restructure your code so that windows are visible prior to VTK rendering. This is the recommended approach.
// - Make use of the new vtkRenderWindow::IsDrawable method to detect invalid drawable and avoid asking VTK to render
// until the drawable is valid. This is useful if one can't ensure window visibility for some reason.
// - As a last resort, you may disable OpenGL error detection via the VTK_REPORT_OPENGL_ERRORS CMake variable, allowing you to ignore the bug.


#if defined(vmxImagePlanes_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxImagePlanes_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxImagePlanes_EXPORTS
            #define vmxImagePlanes_API __declspec(dllexport)
        #else
            #define vmxImagePlanes_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxImagePlanes_EXPORTS
            #define vmxImagePlanes_API __attribute__((visibility("default")))
        #else
            #define vmxImagePlanes_API
        #endif
    #endif
#endif



#ifndef vmxImagePlanes_H
	#define vmxImagePlanes_H



#include <stdint.h>

#include "mxDataObjectFactory.h"
#include "vmxImageData.h"
#include "vmxImagePlaneWidget.h"

#include <vtkGenericDataObjectReader.h>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkImageFlip.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageShiftScale.h>
#include <vtkInformation.h>
#include <vtkLookupTable.h>
#include <vtkPNGReader.h>
#include <vtkPNGWriter.h>
#include <vtkPointData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkUnsignedIntArray.h>
#include <vtkUnsignedLongArray.h>
#include <vtkTypeUInt64Array.h>
#include <vtkTypeUInt32Array.h>
#include <vtkTypeUInt16Array.h>
#include <vtkTypeUInt8Array.h>
#include <vtkWeakPointer.h>




/// VTK level image planes.

template<class T>
class vmxImagePlanes_API vmxImagePlanesT
{
    
protected:
    
    /// Pointer to the image being visualized.
    vmxImageDataT<T> *m_image;
    
    /// Indexes of planes currently being visualized in order c,r,s,t.
    unsigned int m_index[4];
    
    /// Plane for S direction.
    vtkSmartPointer<vmxImagePlaneWidget> m_plane_S;
    
    /// Plane for R direction.
    vtkSmartPointer<vmxImagePlaneWidget> m_plane_R;
    
    /// Plane for C direction.
    vtkSmartPointer<vmxImagePlaneWidget> m_plane_C;
    
    /// Lookup table for scalar mapping.
    vtkSmartPointer<vtkLookupTable> m_lookup_table;
    
    /// The interactor to which the object is assigned. Currently only a single interactor is supported.
    vtkWeakPointer<vtkRenderWindowInteractor> m_interactor;
    
    /// Visibility of each plane.
    int m_visibility_of_plane_S;
    int m_visibility_of_plane_R;
    int m_visibility_of_plane_C;
    
    
public:
    
    /// Constructor.
    vmxImagePlanesT();
    
    /// Destructor.
    ~vmxImagePlanesT();

    /// Build a color lookup table. Assumes linear ramp filter.
    void BuildLookupTable(double range0, double range1, double brightness0, double brightness1, double aplha0, double aplha1, double saturation0, double saturation1, double hue0, double hue1);
    
    /// Get column index.
    unsigned int GetIndexColumns();
    
    /// Get row index.
    unsigned int GetIndexRows();
    
    /// Get slice index.
    unsigned int GetIndexSlices();
    
    /// Get time index.
    unsigned int GetIndexTime();

    /// Get lookup table (e.g. if visualization parameters need to be set individually).
    vtkLookupTable* GetLookupTable();
    
    /// Get image plane widget for S (slices) direction.
    vmxImagePlaneWidget* GetPlaneWidget_S();

    /// Get image plane widget for R (rows) direction.
    vmxImagePlaneWidget* GetPlaneWidget_R();

    /// Get image plane widget for C (slices) direction.
    vmxImagePlaneWidget* GetPlaneWidget_C();
    
    /// Get plane visibility.
    int GetVisibilityOf_S_Plane() { return m_visibility_of_plane_S; };
    
    /// Get plane visibility.
    int GetVisibilityOf_R_Plane() { return m_visibility_of_plane_R; };

    /// Get plane visibility.
    int GetVisibilityOf_C_Plane() { return m_visibility_of_plane_C; };
    
    /// Set input image for visualization.
    int SetImage(vmxImageDataT<T> *input_image);
    
    /// Set Interactor.
    int SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set indexes of image planes with rendering the scene. Success 1, fail 0.
    int SetIndex_C(unsigned int index);

    /// Set indexes of image planes with rendering the scene. Success 1, fail 0.
    int SetIndex_R(unsigned int index);

    /// Set indexes of image planes with rendering the scene. Success 1, fail 0.
    int SetIndex_S(unsigned int index);
    
    /// Set indexes of image planes with rendering the scene. Success 1, fail 0.
    int SetIndex_T(unsigned int index);
    
    /// Color the opaque image plane. If you want to use lookup table with full scalar range, set
    /// 'is_using_full_range' to non zero, otherwise [minimim,maximum] range of pixel values is used.
    void SetMappingToOpaqueColor();
    
    /// Color the transparent image plane. If you want to use lookup table with full scalar range, set
    /// 'is_using_full_range' to non zero, otherwise [minimim,maximum] range of pixel values is used.
    void SetMappingToTransparentColor();
    
    /// Use grayscale opaque image plane. If you want to use lookup table with full scalar range, set
    /// 'is_using_full_range' to non zero, otherwise [minimim,maximum] range of pixel values is used.
    void SetMappingToOpaqueGrayScale();
    
    /// Use grayscale transparent image plane. If you want to use lookup table with full scalar range, set
    /// 'is_using_full_range' to non zero, otherwise [minimim,maximum] range of pixel values is used.
    void SetMappingToTransparentGrayScale();
    
    /// Color the transparent image plane so that evy non-zero voxel has the same color, zero voxels are transparent.
    void SetMappingToTransparentBinary();
    
    /// Set visibility of S (slice) plane on/off.
    void SetVisibilityOf_S_Plane(int is_visible);

    /// Set visibility of R (row) plane on/off.
    void SetVisibilityOf_R_Plane(int is_visible);

    /// Set visibility of C (column) plane on/off.
    void SetVisibilityOf_C_Plane(int is_visible);

};




template<class T> class vmxImagePlanesT;//pre-declare the template class itself


typedef vmxImagePlanesT<uint8_t> vmxImagePlanes8U;
typedef vmxImagePlanesT<uint16_t> vmxImagePlanes16U;
typedef vmxImagePlanesT<uint32_t> vmxImagePlanes32U;
//typedef vmxImagePlanesT<uint64_t> vmxImagePlanes64U;


typedef vmxImagePlanes16U vmxImagePlanes;





#endif
