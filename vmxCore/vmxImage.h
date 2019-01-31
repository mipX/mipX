/*=========================================================================
 
 Program:   mipx
 Module:    vmxImage.h
 
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


#if defined(vmxImage_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxImage_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxImage_EXPORTS
            #define vmxImage_API __declspec(dllexport)
        #else
            #define vmxImage_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxImage_EXPORTS
            #define vmxImage_API __attribute__((visibility("default")))
        #else
            #define vmxImage_API
        #endif
    #endif
#endif



#ifndef vmxImage_H
	#define vmxImage_H



#include <stdint.h>
#include "mxImage.h"
#include "mxDataObjectFactory.h"
#include "vmxImageData.h"
#include "vmxImagePlaneWidget.h"
#include "vmxImagePlanes.h"

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





/// VTK level 4D image.

template<class T>
class vmxImage_API vmxImageT : public vmxImageDataT<T>
{
    
public:
    
    /// Image planes - VTK visualization of orthogonal cut planes.
    vmxImagePlanesT<T> m_image_planes;
    
    /// Renderer to which this image is assigned.
    vtkRenderer *m_renderer;
    
    /// Constructor.
    vmxImageT();
    
    /// Destructor.
    virtual ~vmxImageT();
    
    /// Attach existing (already created) data, i.e. sets the m_data_address to data_address and dimensions to given numbers.
    /// Notice: be sure that the data stored at data_address was allocated with the right data size.
    /// 'is_data_owned_by_mxImage' tells if the data will be owned by this object or if it will remain in possession of an external object.
    virtual int AttachData(void *data_address, int is_data_owned_by_mxImage,  unsigned int dimension_c, unsigned int dimension_r, unsigned int dimension_s, unsigned int dimension_t);
    
    /// Re-initialize dimensions of existing image. The method recalculates internal indexing pointers.
    /// Note: there is no re-allocation of data.  't*s*r*c' must be equal to existing total number of elements
    /// (i.e. the total number of voxels does not change).
    virtual int ConvertToDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c);

    /// Copy data members of input object if the input object nad this object are of the same type.
    virtual int CopyFromDataObject(mxDataObject *data_object);
    
    /// Copy entire image from input image.
    int Copy(vmxImageT<T> *image);
    
    /// Copy entire image from input image.
    int Copy(vmxImageDataT<T> *image);
    
    /// Copy entire image from input image.
    int Copy(mxImageT<T> *image);
    
    /// Detach the array structures from the data (returns pointer to the data).
    virtual void* DetachData();
    
    /// Get pointer to image planes.
    vmxImagePlanesT<T>* GetImagePlanes() { return (&m_image_planes); };
    
    /// Get column index.
    unsigned int GetIndex_C();
    
    /// Get row index.
    unsigned int GetIndex_R();
    
    /// Get slice index.
    unsigned int GetIndex_S();
    
    /// Get time index.
    unsigned int GetIndex_T();
    
//    /// Checks if the given vtk actor belongs to this image (checks image planes).
//    int HasActor(vtkActor *actor);
    
    //    /// Check visibility of the obejct.
    //    int IsVisible();
    
    /// Load PNG image series from the array of given filen names.
    virtual int LoadPNGImages(vtkStringArray *file_names);
    
    /// Load PNG image series from file name root, the number of slices (starting from index 0) and number of digits
    /// marking the image index in the file name (e.g. for number_of_digits=4, index 7 will be '0007').
    virtual int LoadPNGImages(const char *file_name_root, unsigned int number_of_digits, unsigned int number_of_slices);
    
    /// Load PNG image series from file name root, the start and end index of the images and number of digits
    /// marking the image index in the file name (e.g. for number_of_digits=4, index 7 will be '0007').
    virtual int LoadPNGImages(const char *file_name_root, unsigned int number_of_digits, unsigned int slice_start_index, unsigned int slice_end_index);
    
    /// Load LEGACY VTK structured points file.
    virtual int LoadVTKFile(const char *file_name);
    
    /// Reset the object.
    virtual void Reset();

    /// Set index 's' (slices).
    int SetIndex_S(unsigned int index);
    
    /// Set index 't' (time series).
    int SetIndex_T(unsigned int index);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *rwi);
        
    /// Set the size of the grid with properties to DEFAULT values: spacing:(1,1,1,1), origin:(0,0,0,0)...
    /// If the grid is already the requested size, does not do anything.
    virtual int SetDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Set size and element type to be equal to size of the input image (properties like spacing, origin, etc. are not modified).
    virtual void SetDimensionsAs(mxBasicImage *image);
    
    /// Set size and properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    virtual void SetDimensionsAndPropertiesAs(mxBasicImage *image);
    
    /// Color the opaque image plane.
    void SetMappingToOpaqueColor();
    
    /// Color the transparent image plane.
    void SetMappingToTransparentColor();
    
    /// Use grayscale opaque image plane.
    void SetMappingToOpaqueGrayScale();
    
    /// Use grayscale transparent image plane.
    void SetMappingToTransparentGrayScale();
    
    /// Color the transparent image plane so that evy non-zero voxel has the same color, zero voxels are transparent.
    void SetMappingToTransparentBinary();
    
    //    /// Set the orientation of the image.
    //    void SetOrientationCxCyCzRxRyRz(double cx, double cy, double cz, double rx, double ry, double rz);

    /// Set origin from input coordinates.
    void SetOrigin(double t, double s, double r, double c);
    
    /// Set visual properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    /// Note: size is not modified.
    virtual void SetPropertiesAs(mxBasicImage *image);
    
    /// Set the renderer to which this object is assigned. The image planes will have this renderer as default independently of the default renderer of the interactor.
    void SetRenderer(vtkRenderer *renderer);
    
    /// Set spacing.
    virtual void SetSpacing(double t, double s, double r, double c);
    
    /// Set the visibility of planes. Should be called before destruction of the object
    /// if the vtk level app terminates at the same level as this object.
    void SetVisibility(int is_visible);
    
    /// Set the visibility of the object component for VTK visualization.
    /// This is a 3D image and has 3 visualization components: axial, sagittal and coronal planes.
    void SetVisibilityOfComponent(int component_index, int is_visible);

    /// Take over data from an existing vtk image data. Shallow copy is made and this object will be the owner of the data.
    /// This means that the input vtk image data is left without any data elements. Input parameters 'number_of_time_series' and
    /// 'number_of_slices' are used to convert the input vtk image data to the right dimensions (because vtk image data is 3D only).
    virtual int TakeOverVTKImageData(vtkImageData *vtk_image, unsigned int number_of_time_series, unsigned int number_of_slices);

    
};



template<class T> class vmxImageDataT;


// Declare types of images.
typedef vmxImageT<uint8_t> vmxImage8U;
typedef vmxImageT<uint16_t> vmxImage16U;
typedef vmxImageT<uint32_t> vmxImage32U;
//typedef vmxImageT<uint64_t> vmxImage64U;


/// Defualt image class will have 16U (16-bit unsigned) type of elements.
typedef vmxImage16U vmxImage;

typedef mxDataObjectFactoryT<vmxImage8U> vmxImage8UFactory;
typedef mxDataObjectFactoryT<vmxImage16U> vmxImage16UFactory;
typedef mxDataObjectFactoryT<vmxImage32U> vmxImage32UFactory;
//typedef mxDataObjectFactoryT<vmxImage64U> vmxImage64UFactory;

typedef mxDataObjectFactoryT<vmxImage> vmxImageFactory;




#endif
