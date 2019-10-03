/*=========================================================================
 
 Program:   mipx
 Module:    mxBIP.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxBIP_USE_SOURCE_CODE) || defined(mxImageProcessing_USE_SOURCE_CODE)
    #define mxBIP_API
#else
    #if defined(_MSC_VER)
        #ifdef mxBIP_EXPORTS
            #define mxBIP_API __declspec(dllexport)
        #else
            #define mxBIP_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxBIP_EXPORTS
            #define mxBIP_API __attribute__((visibility("default")))
        #else
            #define mxBIP_API
        #endif
    #endif
#endif



#ifndef mxBIP_H
    #define mxBIP_H


#include "mxDataObjectFactory.h"
#include "mxImage.h"
#include "mxFunctionObject.h"
#include "mxGeometry.h"



/// Binary Image Processing methods.

class mxBIP_API mxBIP : public mxFunctionObject
{
    
public:
    
    /// Constructor.
    mxBIP();
    
    /// Destructor.
    ~mxBIP();
    
    /// Morphological closing (dilation followed by erosion) of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int CloseWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
    
    /// Morphological dilation of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int DilateWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
    
    /// Squared distance transform - calculates the distance of the voxels in masked image to the nearest background voxel in input image.
    int DistanceTransformForSphere(mxImage &input, mxImage &mask, mxImage &output, unsigned int t=0);
    
    /// Morphological erosion of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int ErodeWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
    
    /// Filter out voxels that have no non-zero 4-neighbors.
    int FilterWith_4_Neghborhood(mxImage &input, mxImage &output, unsigned int t=0);
    
    /// Extract the connected component starting from the given seed indexes by 26-neighborgood propagation on value above the given threshold value.
    int ExtractConnectedComponent26(mxImage &input, mxImage &output, unsigned int seed_s, unsigned int seed_r, unsigned int seed_c, mxImageScalar threshold, unsigned int t=0);
    
    /// Extract the largest 26-connected component in the input image above the given threshold value.
    int ExtractLargestConnectedComponent26(mxImage &input_image, int threshold, mxImage &output_image, unsigned int t);
    
    /// Get the number of foreground (non-zero) voxels in 8 neighborhood of the given voxel indexes.
    int NumberOfForegroundVoxelsInNeighborhood8(mxImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Get the number of foreground (non-zero) voxels in 8 neighborhood of the given voxel indexes.
    /// Warning: This is a function for iterative use (faster than non-iterative implementation),
    /// but data validity check is NOT performed and mxGeometry has to have pre-set dimensions.
    int NumberOfForegroundVoxelsInNeighborhood8_Iterative(mxImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Get the number of foreground (non-zero) voxels in 26 neighborhood of the given voxel indexes.
    int NumberOfForegroundVoxelsInNeighborhood26(mxImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c);
    
    /// Get the number of foreground (non-zero) voxels in 26 neighborhood of the given voxel indexes.
    /// Warning: This is a function for iterative use (faster than non-iterative implementation),
    /// but data validity check is NOT performed and mxGeometry has to have pre-set dimensions.
    int NumberOfForegroundVoxelsInNeighborhood26_Iterative(mxImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c);

    /// Morphological opening (erosion followed by dilation) of the binary input image using a spherical structuring element (SE) of given squared radius size.
    int OpenWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t=0);
    
    /// Assigns each output voxel a value equal to the number of foregroung voxels found in the smallest sphere that contains at least one background voxel
    /// (i.e. the first sphere size that goes out of the segmented input image).
    int ProfileVolumeTransformForSphere(mxImage &input, mxImage &mask, mxImage &output, unsigned int t=0, unsigned int offset_percent=0);

};



#endif
