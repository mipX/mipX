/*=========================================================================
 
 Program:   mipx
 Module:    mxGIPX.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxGIPX_USE_SOURCE_CODE) || defined(mxImageProcessing_USE_SOURCE_CODE)
    #define mxGIPX_API
#else
    #if defined(_MSC_VER)
        #ifdef mxGIPX_EXPORTS
            #define mxGIPX_API __declspec(dllexport)
        #else
            #define mxGIPX_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxGIPX_EXPORTS
            #define mxGIPX_API __attribute__((visibility("default")))
        #else
            #define mxGIPX_API
        #endif
    #endif
#endif



#ifndef mxGIPX_H
    #define mxGIPX_H



#include "mxBasicImage.h"
#include "mxFunctionObject.h"



/// Voxel class representing indexes with a value.

class mxGIPX_API mxGIPX : public mxFunctionObject
{
    
public:
    
    /// Constructor.
    mxGIPX();
    
    /// Destructor.
    ~mxGIPX();
    
    /// Make the histogram of the input image.
    int Histogram(mxBasicImage &input, mxArray<int> &output);
    
    /// Invert values of voxels (negative) based on found maximum and minimum voxel values.
    int InvertValues(mxBasicImage &input, mxBasicImage &output);
    
    /// Threshold the input image with given value (all values equalt to and above it will be recorded in the output). The rest is background (zero value).
    int Threshold(mxBasicImage &input, mxImageScalar threshold, mxBasicImage &output);

    /// Threshold the input image with given range. All values in range [threshold1, threshold2] will be foreground in the output image.
    int ThresholdInRange(mxBasicImage &input, mxImageScalar threshold1, mxImageScalar threshold2, mxBasicImage &output);

};



#endif