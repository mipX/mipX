/*=========================================================================
 
 Program:   mipx
 Module:    mxProfileProcessing.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxProfileProcessing_USE_SOURCE_CODE) || defined(mxProcessing_USE_SOURCE_CODE)
    #define mxProfileProcessing_API
#else
    #if defined(_MSC_VER)
        #ifdef mxProfileProcessing_EXPORTS
            #define mxProfileProcessing_API __declspec(dllexport)
        #else
            #define mxProfileProcessing_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxProfileProcessing_EXPORTS
            #define mxProfileProcessing_API __attribute__((visibility("default")))
        #else
            #define mxProfileProcessing_API
        #endif
    #endif
#endif



#ifndef mxProfileProcessing_H
    #define mxProfileProcessing_H



#include "mxGeometry.h"
#include "mxImage.h"
#include "mxProfile.h"
#include "mxDataObjectFactory.h"
#include "mxFunctionObject.h"

#include <math.h>



/// Profile processing methods (multidimensional vector field).

class mxProfileProcessing_API mxProfileProcessing : public mxFunctionObject
{
    mxString m_s_min;
    mxString m_s_max;
    mxString m_s_med_minus;
    mxString m_s_med_plus;
    mxString m_s_avr;
    mxString m_s_med_avr;
    mxString m_s_minmax_avr;
    mxString m_s_med_root;
    mxString m_s_minmax_root;
    
public:
    
    enum mxComparisonOperator
    {
        SMALLER,
        SMALLER_OR_EQUAL,
        GREATER,
        GREATER_OR_EQUAL
    };


	/// Constructor.
	mxProfileProcessing();

	/// Destructor.
	~mxProfileProcessing();
    

    /// Profile can contain different functions, so this method maps desired functions to their indexes in the profile according to the strings found
    /// in the profile itself. If the profile does not contain the desired function, the method returns 0, oterwise returns 1.
    int GetIndexArrayForOperatorStringArray(mxProfile &input, mxArray< mxString> &operators, mxArray<int> &output_index_array);
    
    /// Profile can contain different functions, so this method maps desired functions to their indexes in the profile according to the strings found
    /// in the profile itself. If the profile does not contain the desired function, the method returns 0, oterwise returns 1.
    int GetIndexArrayForProfileFunction(mxProfile &input, unsigned int profile_function, mxArray<int> &output_index_array);
    
    /// Function bits(from LSB to MSB): min, max, med-, med+, avr, med_avr, minmax_avr, med_root, minmax_root.
    /// Examines the image in all 3D directions (sphere).
    int Make_R_Profile_SphereDSE(mxImage &input_image, mxImage &mask, mxProfile &output_profile, int radius_limit, unsigned int functions = 511);
    
    /// Function bits(from LSB to MSB): min, max, med-, med+, avr, med_avr, minmax_avr, med_root, minmax_root.
    /// Examines the image in transversal direction (uses Geometry::circle).
    int Make_R_Profile_CircleDSE(mxImage &input_image, mxImage &mask, mxProfile &output_profile, int radius_limit, unsigned int functions = 511);
    
    /// Calculates profile measure for a given profile, stores it into output image.
    int ProfileMeasure(mxProfile &input, mxImage &output, unsigned int profile_function, mxComparisonOperator comparison_operator, int start_r_or_n_value = 1);

    /// Calculates profile measure for a given profile, stores it into output image.
    int ProfileMeasure(mxProfile &input, mxImage &output, mxArray<mxString> &array_of_operators, mxComparisonOperator comparison_operator, int start_r_or_n_value = 1);
    
    /// Calculates profile measure for a given profile, stores it into output image.
    int ProfileMeasure(mxProfile &input, mxImage &output, mxArray<int> &index_array, mxComparisonOperator comparison_operator, int start_r_or_n_value = 1);

    
//    ///Automatically determines profile function by smallest absolute deviation of each function from average pixel value in the slice.
//    int ProfileMeasure_ProfileFunctionBySmallestDeviation(mxProfile &input, mxImage &output, mxComparisonOperator comparison_operator, int start_r_or_n_value = 1);
//
    
//    //Finds the average value in the (transversal) slice with given index.
//    //NOTICE: THE AVERAGE IS CALCULATED ONLY USING POSITIONS WHERE THE PROFILE VECTOR EXISTS. THIS MEANS THAT SLICES CAN HAVE UNEQUAL NUMBER
//    // OF POINTS TAKEN INTO ACCOUNT!
//    int _AverageValueInSlice(mxProfile &input, int slice_index, double &output_average);
//
//    //For all basic functions existing in input profile for a SINGLE SLICE, the average values are calculated for DSEs ('r' or 'n' values) up to 'r_or_n_max_size'.
//    // The average values are stored in a matrix, where the rows are the basic functions: min, max, med-, med+, avr, med_avr, minmax_avr, med_root, minmax_root.
//    // This is called BASIC AVERAGE PROFILE. If any of these functions is not in the profile, the matix values for it will be -1.
//    int _BasicAverageProfileInSlice(mxProfile &input, int slice_index, int r_or_n_max_size, mxMatrix<double> &basic_average_profile);
//
//    //From the calculated BASIC AVERAGE PROFILE for slice and average slice value, calculates absolute deviation for given 'profile_function' combined profile
//    // function. The 'abs_deviation' represents function's absolute deviation. If the combined profile function for a given index does not exist,
//    // returns 0, else return 1.
//    int _AbsoluteDeviationForFunctionFromBasicAverageProfile(unsigned int profile_function, mxMatrix<double> &basic_average_profile, double average_value_in_slice, double &abs_deviation);
//
//    //From the calculated BASIC AVERAGE PROFILE for slice 'slice_index' and average slice value, calculates absolute deviation for all possible combined profile
//    // functions. The 'output_deviation_array' has 2^9 values, where each index represents a combined function and the value in the array at that index represents
//    // function's absolute deviation. If the combined profile function for a given index does not exist, its value in the array will be -1. Notice that the value
//    // at index 0 will always be -1, since profile function at that value does not exist.
//    // Repeatedly calls _AbsoluteDeviationForFunctionFromBasicAverageProfile() for all profile functions.
//    int _AbsoluteDeviationArrayFromBasicAverageProfile(mxMatrix<double> &basic_average_profile, double average_value_in_slice, mxArray<double> &output_deviation_array);

    
    
    
    
    

    //'index_matrix' is of dimensions (n,3) with indexes of the mxProfile vector to be used. The columns indexes are: [0] is S,
    // [1] is R and [2] is C component of mxProfile vector. Comparison is one of the following: "<", ">", "<=" or ">=".
    int Threshold(mxProfile &profile, mxImage &output_image, mxMatrix<int> &index_matrix, const char *comparison);
    
  
};


#endif
