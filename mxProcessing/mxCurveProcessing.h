/*=========================================================================
 
 Program:   mipx
 Module:    mxCurveProcessing.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxCurveProcessing_USE_SOURCE_CODE) || defined(mxProcessing_USE_SOURCE_CODE)
    #define mxCurveProcessing_API
#else
    #if defined(_MSC_VER)
        #ifdef mxCurveProcessing_EXPORTS
            #define mxCurveProcessing_API __declspec(dllexport)
        #else
            #define mxCurveProcessing_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxCurveProcessing_EXPORTS
            #define mxCurveProcessing_API __attribute__((visibility("default")))
        #else
            #define mxCurveProcessing_API
        #endif
    #endif
#endif



#ifndef mxCurveProcessing_H
    #define mxCurveProcessing_H



#include "mxCurve.h"
#include "mxDataObjectFactory.h"
#include "mxFunctionObject.h"

#include <math.h>


/// Curve processing methods (xy signals).

class mxCurveProcessing_API mxCurveProcessing : public mxFunctionObject
{
    
public:

	/// Constructor.
	mxCurveProcessing();

	/// Destructor.
	~mxCurveProcessing();
    
    /// Use this method to compare 2 double numbers. Defauls number of decimals for equality is 5 (tolerated error: 0.00001).
    int internalAreDoublesEqual_5(double d1, double d2);
    
    /// Find the first slope X value of an impulse.
    int FirstSlope_X_Value_ForSingleImpuls(mxCurve &input, double &output_x_value);
    
    /// Find the most prominent rising slopes by pronouncing their peaks.
    int RisingSlopePeaks(mxCurve &input, mxCurve &output);
    
    /// Find the most prominent rising slopes by pronouncing their peaks.
    int RisingSlopePeaksWeightedBySlopeHight(mxCurve &input, mxCurve &output);

  
};


#endif
