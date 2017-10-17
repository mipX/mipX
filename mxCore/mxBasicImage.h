/*=========================================================================
 
 Program:   mipx
 Module:    mxBasicImage.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



//#if defined(mxBasicImage_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
//    #define mxBasicImage_API
//#else
//    #if defined(_MSC_VER)
//        #ifdef mxBasicImage_EXPORTS
//            #define mxBasicImage_API __declspec(dllexport)
//        #else
//            #define mxBasicImage_API __declspec(dllimport)
//        #endif
//    #else
//        #ifdef mxBasicImage_EXPORTS
//            #define mxBasicImage_API __attribute__((visibility("default")))
//        #else
//            #define mxBasicImage_API
//        #endif
//    #endif
//#endif



#ifndef mxBasicImage_H
    #define mxBasicImage_H



#include <stdint.h>
#include "mxArray.h"
#include "mxDataObject.h"
#include "mxString.h"


/// Image scalar definition. With this definition the size of the scalar can be changed (for future growth).

typedef uint64_t mxImageScalar;


/// Image template as a data object of a structured grid of elements.

class mxBasicImage : public mxDataObject
{
    
public:
	
    /// Attach existing (already created) data, i.e. sets the m_data_address to data_address and dimensions to given numbers.
    /// Notice: be sure that the data stored at data_address was allocated with the right data size.
    /// 'is_data_owned_by_mxImage' tells if the data will be owned by this object or if it will remain in possession of an external object.
    virtual int AttachData(void *data_address,  int is_data_owned_by_mxImage, unsigned int dimension_c, unsigned int dimension_r, unsigned int dimension_s, unsigned int dimension_t) = 0;

    /// Re-initialize dimensions of existing image. The method recalculates internal indexing pointers.
    /// Note: there is no re-allocation of data.  't*s*r*c' must be equal to existing total number of elements
    /// (i.e. the total number of voxels does not change).
    virtual int ConvertToDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c) = 0;
    
    /// Detach the array structures from the data (returns pointer to the data).
    virtual void* DetachData() = 0;
    
    /// Fill in the whole grid with input value.
    virtual void FillInWith(mxImageScalar value) = 0;
    
    /// Fill in the 3D image with index (t) with input value.
    virtual void FillIn_3D_ImageWith(unsigned int t, mxImageScalar value) = 0;
    
    /// Fill in the slice with indexes (t,s) with input value.
    virtual void FillInSliceWith(unsigned int t, unsigned int s, mxImageScalar value) = 0;
    
    /// Get value of voxel at given indexes.
    virtual mxImageScalar Get(unsigned int t, unsigned int s, unsigned int r, unsigned int c) = 0;
    virtual mxImageScalar Get(unsigned int s, unsigned int r, unsigned int c) = 0;
    virtual mxImageScalar Get(unsigned int r, unsigned int c) = 0;
    virtual mxImageScalar Get(unsigned int i) = 0;
    
    /// Get bounds of the image in 3D space.
    virtual int GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max) = 0;
    
    /// Get address of data elements.
    virtual void* GetData() = 0;
    
    /// Get image dimension of T indexes (number of "time series" of the image).
    virtual unsigned int GetDimension_T() = 0;
    
    /// Get image dimension of S indexes (number of "slices" of the image).
    virtual unsigned int GetDimension_S() = 0;
    
    /// Get image dimension of R indexes (number of rows of the image).
    virtual unsigned int GetDimension_R() = 0;
    
    /// Get image dimension of C indexes (number of columns of the image).
    virtual unsigned int GetDimension_C() = 0;
    
    /// Get number of data elements (time_series*slices*rows*columns) of the image.
    virtual unsigned int GetNumberOfDataElements() = 0;
    
    /// Get orientation.
    virtual double* GetOrientation() = 0;
    
    /// Get orientation Columns (C) vector.
    virtual void GetOrientation_C_Vector(double &Cx, double &Cy, double &Cz) = 0;
    
    /// Get orientation Rows (R) vector.
    virtual void GetOrientation_R_Vector(double &Rx, double &Ry, double &Rz) = 0;
    
    /// Get orientation Slices (S) vector.
    virtual void GetOrientation_S_Vector(double &Sx, double &Sy, double &Sz) = 0;
    
    /// Get origin for time series.
    virtual double GetOrigin_T() = 0;
    
    /// Get origin for slices (Z axis).
    virtual double GetOrigin_S() = 0;
    
    /// Get origin for rows (Y axis).
    virtual double GetOrigin_R() = 0;
    
    /// Get origin for columns (X axis).
    virtual double GetOrigin_C() = 0;
    
    /// Get the size of the scalar (in number of bytes) used to represent a single voxel in the image.
    virtual int GetScalarSizeInBytes() = 0;
    
    /// Get spacing for time series.
    virtual double GetSpacing_T() = 0;
    
    /// Get spacing for slices.
    virtual double GetSpacing_S() = 0;
    
    /// Get spacing for rows.
    virtual double GetSpacing_R() = 0;
    
    /// Get spacing for columns.
    virtual double GetSpacing_C() = 0;
    
    /// Get the minimum and maximum voxel value found in the image.
    virtual int GetVoxelMinAndMaxValues(mxImageScalar &min_value_included, mxImageScalar &max_value_included) = 0;
    
    /// Get the full range of voxel values [min_value_included,max_value_included].
    virtual void GetVoxelValueFullRange(mxImageScalar &min_value_included, mxImageScalar &max_value_included) = 0;
    
    /// Calculate index values for given world coordinates. If indexes fall outside the image range return fail 0.
    virtual int IndexFromWorldCoordinates(double w_t, double w_s, double w_r, double w_c, int &out_t, int &out_s, int &out_r, int &out_c) = 0;
    virtual int IndexFromWorldCoordinates(double w_s, double w_r, double w_c, int &out_s, int &out_r, int &out_c) = 0;
    
	/// Check if the image is empty.
	virtual int IsEmpty() = 0;

	/// Check if the image size is equal to input image for 4D (t,s,r,c).
	virtual int IsEqualInDimensions_4D(mxBasicImage &image) = 0;

    /// Check if the image size is equal to input image for 3D (s,r,c).
    virtual int IsEqualInDimensions_3D(mxBasicImage &image) = 0;
    
    /// Check if the image size is equal to input image for 2D (r,c).
    virtual int IsEqualInDimensions_2D(mxBasicImage &image) = 0;
    
    /// Reset the object.
    virtual void Reset() = 0;

    /// Set value of voxel at given indexes.
    virtual void Set(unsigned int t, unsigned int s, unsigned int r, unsigned int c, mxImageScalar value) = 0;
    virtual void Set(unsigned int s, unsigned int r, unsigned int c, mxImageScalar value) = 0;
    virtual void Set(unsigned int r, unsigned int c, mxImageScalar value) = 0;
    virtual void Set(unsigned int i, mxImageScalar value) = 0;
    
	/// Set the size of the grid with properties to DEFAULT values: spacing:(1,1,1,1), origin:(0,0,0,0)...
	/// If the grid is already the requested size, does not do anything.
    virtual int SetDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c) = 0;
	
	/// Set size and element type to be equal to size of the input image (properties like spacing, origin, etc. are not modified).
	virtual void SetDimensionsAs(mxBasicImage *image) = 0;
	
	/// Set size, element type and properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
	virtual void SetDimensionsAndPropertiesAs(mxBasicImage *image) = 0;
    
    /// Set orientation.
    virtual void SetOrientation(double Cx, double Cy, double Cz, double Rx, double Ry, double Rz) = 0;
    
    /// Set origin.
    virtual void SetOrigin(double t, double s, double r, double c) = 0;

    /// Set visual properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    /// Note: size is not modified.
	virtual void SetPropertiesAs(mxBasicImage *image) = 0;
    
    /// Set spacing.
    virtual void SetSpacing(double t, double s, double r, double c) = 0;
    
    /// Calculate world coordinates for given indexes.
    /// Notice: indexes are 'int' values on purpose: they do not have to fall inside the image index range.
    virtual int WorldCoordinatesFromIndexes(int t, int s, int r, int c, double &out_w_t, double &out_w_z, double &out_w_y, double &out_w_x) = 0;
    virtual int WorldCoordinatesFromIndexes(int s, int r, int c, double &out_w_z, double &out_w_y, double &out_w_x) = 0;
    
};


#endif
