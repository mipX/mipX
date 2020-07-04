/*=========================================================================
 
 Program:   mipx
 Module:    mxGIP.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
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



#include "mxCurve.h"
#include "mxImage.h"
#include "mxFunctionObject.h"



/// Grayscale Image Processing methods.

class mxGIP_API mxGIP : public mxFunctionObject
{
    
public:
    
    /// Constructor.
    mxGIP();
    
    /// Destructor.
    ~mxGIP();
    
    /// Analyses the histogram of the image and creates a new images with values scaled in such way that there are no empty values in the histogram.
    int CompactValues(mxImage &input, mxImage &output);

    /// Finds the absolute difference of pixel values from 2 inputs (the same as absolute value of subtraction result).
    int DifferenceAbsolute(mxImage &input, mxImage &input2, mxImage &output);
    
    /// Make the histogram of the input image.
    int Histogram(mxImage &input, mxArray<int> &output);
    int Histogram(mxImage &input, mxCurve &output);
    
    /// Histogram of a single slice (with indexes s,t).
    int Histogram(mxImage &input, unsigned int t, unsigned int s, mxCurve &output);
    int Histogram(mxImage &input, unsigned int t, unsigned int s, mxArray<int> &output);
    int Histogram(mxImage &input, mxImage &mask, unsigned int t, unsigned int s, mxCurve &output);
    
    /// Invert values of voxels (negative) based on found maximum and minimum voxel values.
    int InvertValues(mxImage &input, mxImage &output);
    
    /// Mask the input image with image mask (e.g. a segmented image with 0 value for background).
    int Mask(mxImage &input, mxImage &mask, mxImage &output);
    
    /// Rescale values to give input range [min_included, max_included].
    int RescaleValueRange(mxImage &input, mxImageScalar min_included, mxImageScalar max_included, mxImage &output);
    
    /// Threshold the input image with given value (all values equalt to and above it will be recorded in the output). The rest is background (zero value).
    int Threshold(mxImage &input, mxImageScalar threshold, mxImage &output);

    /// Threshold the input image with given range. All values in range [threshold1, threshold2] will be foreground in the output image.
    int ThresholdInRange(mxImage &input, mxImageScalar threshold1, mxImageScalar threshold2, mxImage &output);

};



#endif
