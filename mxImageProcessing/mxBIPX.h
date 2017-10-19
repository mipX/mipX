/*=========================================================================
 
 Program:   mipx
 Module:    mxBIPX.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxBIPX_USE_SOURCE_CODE) || defined(mxImageProcessing_USE_SOURCE_CODE)
    #define mxBIPX_API
#else
    #if defined(_MSC_VER)
        #ifdef mxBIPX_EXPORTS
            #define mxBIPX_API __declspec(dllexport)
        #else
            #define mxBIPX_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxBIPX_EXPORTS
            #define mxBIPX_API __attribute__((visibility("default")))
        #else
            #define mxBIPX_API
        #endif
    #endif
#endif



#ifndef mxBIPX_H
    #define mxBIPX_H


#include "mxDataObjectFactory.h"
#include "mxBasicImage.h"
#include "mxFunctionObject.h"
#include "mxGeometry.h"



/// Voxel class representing indexes with a value.

class mxBIPX_API mxBIPX : public mxFunctionObject
{
    
public:
    
    /// Constructor.
    mxBIPX();
    
    /// Destructor.
    ~mxBIPX();
    
    /// Morphological closing (dilation followed by erosion) of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int CloseWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
    
    /// Morphological dilation of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int DilateWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
    
    /// Squared distance transform - calculates the distance of the voxels in masked image to the nearest background voxel in input image.
    int DistanceTransformForSphere(mxBasicImage &input, mxBasicImage &mask, mxBasicImage &output, unsigned int t=0);
    
    /// Morphological erosion of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int ErodeWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
    
    /// Extract the connected component starting from the given seed indexes by 26-neighborgood propagation on value above the given threshold value.
    int ExtractConnectedComponent26(mxBasicImage &input, mxBasicImage &output, unsigned int seed_s, unsigned int seed_r, unsigned int seed_c, mxImageScalar threshold, unsigned int t=0);
    
    /// Get the number of foreground (non-zero) voxels in 8 neighborhood of the given voxel indexes.
    int NumberOfForegroundVoxelsInNeighborhood8(mxBasicImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Get the number of foreground (non-zero) voxels in 8 neighborhood of the given voxel indexes.
    /// Warning: This is a function for iterative use (faster than non-iterative implementation),
    /// but data validity check is NOT performed and mxGeometry has to have pre-set dimensions.
    int NumberOfForegroundVoxelsInNeighborhood8_Iterative(mxBasicImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Get the number of foreground (non-zero) voxels in 26 neighborhood of the given voxel indexes.
    int NumberOfForegroundVoxelsInNeighborhood26(mxBasicImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Get the number of foreground (non-zero) voxels in 26 neighborhood of the given voxel indexes.
    /// Warning: This is a function for iterative use (faster than non-iterative implementation),
    /// but data validity check is NOT performed and mxGeometry has to have pre-set dimensions.
    int NumberOfForegroundVoxelsInNeighborhood26_Iterative(mxBasicImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c);

    /// Morphological opening (erosion followed by dilation) of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int OpenWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
};



#endif