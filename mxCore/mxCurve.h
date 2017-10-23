/*=========================================================================
 
 Program:   mipx
 Module:    mxCurve.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxCurve_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxCurve_API
#else
    #if defined(_MSC_VER)
        #ifdef mxCurve_EXPORTS
            #define mxCurve_API __declspec(dllexport)
        #else
            #define mxCurve_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxCurve_EXPORTS
            #define mxCurve_API __attribute__((visibility("default")))
        #else
            #define mxCurve_API
        #endif
    #endif
#endif



#ifndef mxCurve_H
    #define mxCurve_H



#include "mxArray.h"
#include "mxDataObject.h"
#include "mxString.h"

#include <strstream>
#include <fstream>


/// 2D curve (x and y=f(x) values) as pair of arrays of x and corresponding y values.

class mxCurve_API mxCurve : public mxDataObject
{
    
protected:
    
    /// Label for x axis (describes what the x values represent).
    mxString m_label_for_X_axis;
    
    /// Label for y axis (describes what the y values represent).
    mxString m_label_for_Y_axis;
    
    /// Array of x values.
    mxArray< double > m_X_values;
    
    /// Array of y = f(x) values.
    mxArray< double > m_Y_values;

    

public:

	/// Constructor.
	mxCurve();

	/// Destructor.
	virtual ~mxCurve();
    
    /// Copy from input curve.
    int Copy(mxCurve *curve);
    
    /// Assuming that the number of samples is non-zero, fills in the X array with values x_{i+1} = x_{i} + delta_x,
    /// where starting value is x0_included. E.g.: x0 = 0, delta_x = 1: X = [1,2,3,4,5,6,7....].
    int FillIn_X_ValuesAscending(double x0_included = 0, double delta_x = 1);
    
    /// Get array of X values.
    mxArray< double >* GetArrayOf_X_Values();
    
    /// Get array of Y values.
    mxArray< double >* GetArrayOf_Y_Values();
   
    /// Get label for X axis.
    mxString GetLabelFor_X_Axis();
    
    /// Get label for Y axis.
    mxString GetLabelFor_Y_Axis();
    
    /// Get the maximum Y value and its index.
    int GetMaximum_Y_Value(double &output_max_y_value, unsigned int &output_index_of_max_y_value);
    
    /// Get the minimum Y value and its index.
    int GetMinimum_Y_Value(double &output_min_y_value, unsigned int &output_index_of_min_y_value);

	/// Get number of samples of the curve (equals the number of elements in the x or y array).
	unsigned int GetNumberOfSamples();
    
    /// Get range of values [x_min, x_max] for x axis.
    int GetRangeOf_X_Values(double &x_min, double &x_max);
    
    /// Get range of values [y_min, y_max] for y axis.
    int GetRangeOf_Y_Values(double &y_min, double &y_max);
    
    /// Assuming X array is a sorted array in ascending order of values, finds the index of the given input x value.
    /// If the exact value cannot be found, returns the first value larger than x_value. If already the fist value
    /// in the array is larger than x_value, returns index value 0.
    unsigned int IndexOf_X_ValueInAscendingOrder(double x_value);

    /// Check if the curve is empty (if number of samples is 0).
    int IsEmpty();
    
    /// Load curve from a Matlab M file.
    int LoadMatlabFile(const char *file_name);
    
    /// Leacy load functions to allow loading of earlier file types.
    int Load_Legacy_SingleArrayFile_v3(const char *file_name);
    int Load_Legacy_SingleArrayFile_v2(const char *file_name);
    
    /// Reset (empty the x and y arrays).
    virtual void Reset();
    
    /// Save to a Matlab M file.
    void SaveToMatlabFile(const char *file_name);
    
    ///  For a given index set x and y values of the curve.
    void Set(unsigned int index, double x_value, double y_value);

    /// Set label for X axis.
    void SetLabelFor_X_Axis(const char *label);
    
    /// Set label for Y axis.
    void SetLabelFor_Y_Axis(const char *label);

	/// Set the number of samples of the curve.
    virtual void SetNumberOfSamples(unsigned int s);

    /// Sort value arrays of the curve (both X and Y) by sorting X values in scending order (and copying their corresponding Y values).
    void Sort_X_Ascending();

    /// Returns X value for given input index.
    double& X(unsigned int index);
    
    /// Returns Y value for given input index.
	double& Y(unsigned int index);

    /// Returns Y value for given input X value (so returns f(x)).
    /// Caution: assumes that x values are sorted in ascending order. Interpolates to find correct f(x) value.
    double Y_FX(double x_value);
    
};


#endif