/*=========================================================================
 
 Program:   mipx
 Module:    mxGIP.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxGIP_USE_SOURCE_CODE) || defined(mxImageProcessing_USE_SOURCE_CODE)
    #define mxGIP_API
#else
    #if defined(_MSC_VER)
        #ifdef mxGIP_EXPORTS
            #define mxGIP_API __declspec(dllexport)
        #else
            #define mxGIP_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxGIP_EXPORTS
            #define mxGIP_API __attribute__((visibility("default")))
        #else
            #define mxGIP_API
        #endif
    #endif
#endif



#ifndef mxGIP_H
    #define mxGIP_H



#include "mxImage.h"
#include "mxFunctionObject.h"



/// Voxel class representing indexes with a value.

class mxGIP_API mxGIP : public mxFunctionObject
{
    
public:
    
    /// Constructor.
    mxGIP();
    
    /// Destructor.
    ~mxGIP();
    
    /// Make the histogram of the input image.
    int Histogram(mxImage &input, mxArray<int> &output);
    
    /// Invert values of voxels (negative) based on found maximum and minimum voxel values.
    int InvertValues(mxImage &input, mxImage &output);
    
    /// Mask the input image with image mask (e.g. a segmented image with 0 value for background).
    int Mask(mxImage &input, mxImage &mask, mxImage &output);
    
    /// Threshold the input image with given value (all values equalt to and above it will be recorded in the output). The rest is background (zero value).
    int Threshold(mxImage &input, mxImageScalar threshold, mxImage &output);

    /// Threshold the input image with given range. All values in range [threshold1, threshold2] will be foreground in the output image.
    int ThresholdInRange(mxImage &input, mxImageScalar threshold1, mxImageScalar threshold2, mxImage &output);

};



#endif