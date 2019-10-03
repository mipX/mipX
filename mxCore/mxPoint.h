/*=========================================================================
 
 Program:   mipx
 Module:    mxPoint.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxPoint_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxPoint_API
#else
    #if defined(_MSC_VER)
        #ifdef mxPoint_EXPORTS
            #define mxPoint_API __declspec(dllexport)
        #else
            #define mxPoint_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxPoint_EXPORTS
            #define mxPoint_API __attribute__((visibility("default")))
        #else
            #define mxPoint_API
        #endif
    #endif
#endif



#ifndef mxPoint_H
    #define mxPoint_H


#include "mxArray.h"
#include "mxString.h"
#include "mxPosition.h"


#define mxPoint_NUMBER_OF_VALUES 15

/// Point class representing positions with a value.

class mxPoint_API mxPoint : public mxPosition
{
    
protected:
    
    /// Value assigned to this point.
	double m_value[mxPoint_NUMBER_OF_VALUES];

public:

	/// Constructor.
	mxPoint();
    
    /// Destructor.
	~mxPoint();

    /// Copy from input point.
    void CopyFrom(mxPoint &point);
    
    /// Get (maximum) number of values that can be stored in the point.
    int GetNumberOfValues() { return mxPoint_NUMBER_OF_VALUES; };
    
    /// Check if the 2D positions (r,c) are equal to the given input positions.
    int IsEqualValueAndPositionIn_2D(mxPoint &point);
    
    /// Check if the 3D positions (s,r,c) are equal to the given input positions.
    int IsEqualValueAndPositionIn_3D(mxPoint &point);
    
    /// Check if the 4D positions (t,s,r,c) are equal to the given input positions.
    int IsEqualValueAndPositionIn_4D(mxPoint &point);
    
    /// Print out the point and only values for indexes that are in the input array.
    void Print(std::ostream &o, mxArray<int> &indexes_of_values_for_printing);
    
    /// Print out the point and only values for indexes that are in the input array.
    void Print(std::stringstream &o, mxArray<int> &indexes_of_values_for_printing);
    
    /// Read the point from an input string.
    /// The string should be of type: '(0,1.1,3,2.1){7.7,5,0,0,0,0,0,0,0,0,0}'
    int ReadFromString(const char *position_string);
    
    /// Set point value.
    void SetValue(double value, unsigned int index = 0);
    
	/// Get point value.
    double& V(unsigned int index = 0);
	
	/// Copy from input point.
	mxPoint& operator =(mxPoint &point);
    
    /// Copy from input position.
	mxPoint& operator =(mxPosition &position);
	
	/// Print point.
    mxPoint_API friend std::ostream& operator << (std::ostream &o, mxPoint &point);
};





#endif
