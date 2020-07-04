/*=========================================================================
 
 Program:   mipx
 Module:    vmxImageData.h
 
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


#if defined(vmxImageData_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxImageData_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxImageData_EXPORTS
            #define vmxImageData_API __declspec(dllexport)
        #else
            #define vmxImageData_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxImageData_EXPORTS
            #define vmxImageData_API __attribute__((visibility("default")))
        #else
            #define vmxImageData_API
        #endif
    #endif
#endif



#ifndef vmxImageData_H
	#define vmxImageData_H



#include <stdint.h>
#include "mxImage.h"


#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


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
#include <vtkStructuredGridReader.h>
#include <vtkStructuredPointsReader.h>
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
#include <vtkXMLImageDataWriter.h>
#include <vtkXMLImageDataReader.h>

#include <vtkXMLImageDataReader.h>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


/// VTK level image template as a data object of a structured grid of elements.

template<class T>
class vmxImageData_API vmxImageDataT : public mxImageT<T>
{
    
protected:

    /// This vtk image data will own the loaded data (e.g. when loading VTK or PNG files).
    vtkSmartPointer<vtkImageData> m_vtk_image_data;
    
    /// Array of vtk image data used for visualization purposes.
    mxArray< vtkSmartPointer<vtkImageData> > m_vtk_image_data_array;

    /// The method that will assign values to m_vtk_image_data_array from existing data at m_data_address (inherited from mxMatrix).
    /// This method is internal, to be used only by other member methods or methods of child classes.
    int internalAttachToPreset_mxImage();
    
public:
    
	/// Constructor.
	vmxImageDataT();

	/// Destructor.
	virtual ~vmxImageDataT();
    
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
    int Copy(vmxImageDataT<T> *image);
    
    /// Copy entire image from input image.
    int Copy(mxImageT<T> *image);
    
    /// Detach the array structures from the data (returns pointer to the data).
    virtual void* DetachData();
    
    /// For a given "time" index return pointer to vtkImageData.
    vtkImageData* GetVTKImageData(unsigned int t_index);
    
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

    /// Save to VTI (vtk image) file, where 'scalar_size_in_bytes' is scalar size defined
    /// in number of bytes (e.g. if scalar_size_in_bytes=2 we are recording to vtk 16 unsigned file).
    void SaveToVTIFile(const char *file_name, unsigned int scalar_size_in_bytes);
    
    /// Save to VTK structured points file, where 'scalar_size_in_bytes' is scalar size defined
    /// in number of bytes (e.g. if scalar_size_in_bytes=2 we are recording to vtk 16 unsigned file).
    void SaveToVTKFile(const char *file_name, unsigned int scalar_size_in_bytes);
    
    /// Save to VTK structured points file with scalar size 8 bits unsigned,
    void SaveToVTK8UFile(const char *file_name);
    
    /// Save to VTK structured points file with scalar size 16 bits unsigned,
    void SaveToVTK16UFile(const char *file_name);
    
    /// Save to VTK structured points file with scalar size 32 bits unsigned,
    void SaveToVTK32UFile(const char *file_name);
    
    /// Save to PNG image series, where 'scalar_size_in_bytes' is scalar size defined
    /// in number of bytes (e.g. if scalar_size_in_bytes=2 we are recording to png 16 unsigned file).
    void SaveToPNGImages(const char *file_name_root, unsigned int scalar_size_in_bytes);
    
    /// Save to PNG 8 bit unsigned image series
    void SaveToPNG8UImages(const char *file_name_root);
    
    /// Save to PNG 16 bit unsigned image series
    void SaveToPNG16UImages(const char *file_name_root);
    
    /// Set origin.
    virtual void SetOrigin(double t, double s, double r, double c);

    /// Set spacing.
    virtual void SetSpacing(double t, double s, double r, double c);
	
    /// Set the size of the grid with properties to DEFAULT values: spacing:(1,1,1,1), origin:(0,0,0,0)...
    /// If the grid is already the requested size, does not do anything.
    virtual int SetDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Set size and element type to be equal to size of the input image (properties like spacing, origin, etc. are not modified).
    virtual void SetDimensionsAs(mxImageT<T> *image);
    
    /// Set size and properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    virtual void SetDimensionsAndPropertiesAs(mxImageT<T> *image);
    
    /// Set visual properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    /// Note: size is not modified.
    virtual void SetPropertiesAs(mxImageT<T> *image);
    
    /// Take over data from an existing vtk image data. Shallow copy is made and this object will be the owner of the data.
    /// This means that the input vtk image data is left without any data elements. Input parameters 'number_of_time_series' and
    /// 'number_of_slices' are used to convert the input vtk image data to the right dimensions (because vtk image data is 3D only).
    virtual int TakeOverVTKImageData(vtkImageData *vtk_image, unsigned int number_of_time_series, unsigned int number_of_slices);

};


template<class T> class vmxImageDataT;


// Declare types of images.
typedef vmxImageDataT<uint8_t> vmxImageData8U;
typedef vmxImageDataT<uint16_t> vmxImageData16U;
typedef vmxImageDataT<uint32_t> vmxImageData32U;
//typedef vmxImageDataT<uint64_t> vmxImageData64U;


/// Defualt image class will have 16U (16-bit unsigned) type of elements.
typedef vmxImageData16U vmxImageData;



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


#endif
