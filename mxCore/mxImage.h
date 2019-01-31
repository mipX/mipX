/*=========================================================================
 
 Program:   mipx
 Module:    mxImage.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxImage_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxImage_API
#else
    #if defined(_MSC_VER)
        #ifdef mxImage_EXPORTS
            #define mxImage_API __declspec(dllexport)
        #else
            #define mxImage_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxImage_EXPORTS
            #define mxImage_API __attribute__((visibility("default")))
        #else
            #define mxImage_API
        #endif
    #endif
#endif



#ifndef mxImage_H
    #define mxImage_H



#include <stdint.h>
#include "mxArray.h"
#include "mxBasicImage.h"
#include "mxDataObject.h"
#include "mxRegularGrid.h"
#include "mxString.h"



template class mxImage_API mxArray<int>;
	


/// Image template as a data object of a structured grid of elements.

template<class T>
class mxImage_API mxImageT : public mxBasicImage
{

protected:
    
    /// Data is stored in a regular grid.
    mxRegularGrid<T> m_grid;
    
    /// Indicates if the data in regular grid is owned by this object. If it is it has to be released on destruction.
    /// If it is not, the data mamory is not deallocated (some external object is in charge of that).
    int m_is_data_owned_by_mxImage;
    
public:
	
	/// Constructor.
	mxImageT();

	/// Destructor.
	virtual ~mxImageT();

    /// Attach existing (already created) data, i.e. sets the m_data_address to data_address and dimensions to given numbers.
    /// Notice: be sure that the data stored at data_address was allocated with the right data size.
    /// 'is_data_owned_by_mxImage' tells if the data will be owned by this object or if it will remain in possession of an external object.
    virtual int AttachData(void *data_address, int is_data_owned_by_mxImage,  unsigned int dimension_c, unsigned int dimension_r, unsigned int dimension_s, unsigned int dimension_t);
    
    /// Re-initialize dimensions of existing image. The method recalculates internal indexing pointers.
    /// Note: there is no re-allocation of data.  't*s*r*c' must be equal to existing total number of elements
    /// (i.e. the total number of voxels does not change).
    virtual int ConvertToDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c);

	/// Copy entire image from input image.
    int Copy(mxImageT<T> *image);
    
    /// Copy data members of input object if the input object and this object are of the same type.
    virtual int CopyFromDataObject(mxDataObject *data_object);
    
    /// Detach the array structures from the data (returns pointer to the data).
    virtual void* DetachData();
    
    /// Fill in the whole grid with input value.
    void FillInWith(mxImageScalar value);
    
    /// Fill in the 3D image with index (t) with input value.
    void FillIn_3D_ImageWith(unsigned int t, mxImageScalar value);
    
    /// Fill in the slice with indexes (t,s) with input value.
    void FillInSliceWith(unsigned int t, unsigned int s, mxImageScalar value);

    /// Get value of voxel at given indexes.
    mxImageScalar Get(unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    mxImageScalar Get(unsigned int s, unsigned int r, unsigned int c);
    mxImageScalar Get(unsigned int r, unsigned int c);
    mxImageScalar Get(unsigned int i);
    
    /// Get bounds of the image in 3D space.
    int GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max);
    
    /// Get address of data elements.
    void* GetData();
    
    /// Get number of "time series" of the image.
    unsigned int GetDimension_T();
    
    /// Get number of "slices" of the image.
    unsigned int GetDimension_S();
    
    /// Get number of rows of the image.
    unsigned int GetDimension_R();
    
    /// Get number of columns of the image.
    unsigned int GetDimension_C();
    
    /// Get number of data elements (time_series*slices*rows*columns) of the image.
    unsigned int GetNumberOfDataElements();
    
    /// Get orientation.
    double* GetOrientation();
    
    /// Get orientation Columns (C) vector.
    void GetOrientation_C_Vector(double &Cx, double &Cy, double &Cz);
    
    /// Get orientation Rows (R) vector.
    void GetOrientation_R_Vector(double &Rx, double &Ry, double &Rz);
    
    /// Get orientation Slices (S) vector.
    void GetOrientation_S_Vector(double &Sx, double &Sy, double &Sz);
    
    /// Get origin for time series.
    double GetOrigin_T();
    
    /// Get origin for slices (Z axis).
    double GetOrigin_S();
    
    /// Get origin for rows (Y axis).
    double GetOrigin_R();
    
    /// Get origin for columns (X axis).
    double GetOrigin_C();
    
    /// Get the size of the scalar (in number of bytes) used to represent a single voxel in the image.
    int GetScalarSizeInBytes();
    
    /// Get spacing for time series.
    double GetSpacing_T();
    
    /// Get spacing for slices.
    double GetSpacing_S();
    
    /// Get spacing for rows.
    double GetSpacing_R();
    
    /// Get spacing for columns.
    double GetSpacing_C();
    
    /// Get the minimum and maximum voxel value found in the image.
    int GetVoxelMinAndMaxValues(uint64_t &min_value_included, uint64_t &max_value_included);
    
    /// Get the full range of voxel values [min_value_included,max_value_included].
    void GetVoxelValueFullRange(uint64_t &min_value_included, uint64_t &max_value_included);
    //void GetVoxelValueFullRange(T &min_value_included, T &max_value_included);


    /// Calculate index values for given world coordinates. If indexes fall outside the image range return fail 0.
    int IndexFromWorldCoordinates(double w_t, double w_s, double w_r, double w_c, int &out_t, int &out_s, int &out_r, int &out_c);
    int IndexFromWorldCoordinates(double w_s, double w_r, double w_c, int &out_s, int &out_r, int &out_c);

	/// Check if the image is empty.
	int IsEmpty();
    
    /// Check if the image size is equal to input image for 4D (t,s,r,c).
    int IsEqualInDimensions_4D(mxBasicImage &image);
    
    /// Check if the image size is equal to input image for 3D (s,r,c).
    int IsEqualInDimensions_3D(mxBasicImage &image);
    
    /// Check if the image size is equal to input image for 2D (r,c).
    int IsEqualInDimensions_2D(mxBasicImage &image);
    
    /// Indexing operator.
    T& operator()(unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    T& operator()(unsigned int s, unsigned int r, unsigned int c);
    T& operator()(unsigned int r, unsigned int c);
    
    /// Array indexing operator.
    T& operator[](unsigned int i);
    
    
    /// Reset the object.
    virtual void Reset();
    
    /// Set value of voxel at given indexes.
    void Set(unsigned int t, unsigned int s, unsigned int r, unsigned int c, mxImageScalar value);
    void Set(unsigned int s, unsigned int r, unsigned int c, mxImageScalar value);
    void Set(unsigned int r, unsigned int c, mxImageScalar value);
    void Set(unsigned int i, mxImageScalar value);
    
	/// Set the size of the grid with properties to DEFAULT values: spacing:(1,1,1,1), origin:(0,0,0,0)...
	/// If the grid is already the requested size, does not do anything.
    virtual int SetDimensions(unsigned int t, unsigned int s, unsigned int r, unsigned int c);
	
	/// Set size and element type to be equal to size of the input image (properties like spacing, origin, etc. are not modified).
	virtual void SetDimensionsAs(mxBasicImage *image);
	
	/// Set size, element type and properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
	virtual void SetDimensionsAndPropertiesAs(mxBasicImage *image);
    
    /// Set orientation.
    virtual void SetOrientation(double Cx, double Cy, double Cz, double Rx, double Ry, double Rz);
    
    /// Set origin.
    virtual void SetOrigin(double t, double s, double r, double c);
    
    /// Set visual properties (spacing, origin, orientation, etc.) to be equal to those of the input image.
    /// Note: size is not modified.
	virtual void SetPropertiesAs(mxBasicImage *image);
    
    /// Set spacing.
    virtual void SetSpacing(double t, double s, double r, double c);
    
    /// Calculate world coordinates for given indexes.
    /// Notice: indexes are 'int' values on purpose: they do not have to fall inside the image index range.
    int WorldCoordinatesFromIndexes(int t, int s, int r, int c, double &out_w_t, double &out_w_z, double &out_w_y, double &out_w_x);
    int WorldCoordinatesFromIndexes(int s, int r, int c, double &out_w_z, double &out_w_y, double &out_w_x);

};


template<class T> class mxImageT;


// Declare types of images.
typedef mxImageT<uint8_t> mxImage8U;
typedef mxImageT<uint16_t> mxImage16U;
typedef mxImageT<uint32_t> mxImage32U;
//typedef mxImageT<uint64_t> mxImage64U;


/// Defualt image class will have 16U (16-bit unsigned) type of elements.
//typedef mxImage16U mxImage;
typedef mxImageT<mxImageScalar> mxImage;




#endif
