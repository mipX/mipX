/*=========================================================================
 
 Program:   mipx
 Module:    mxPoint.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
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



#include "mxPosition.h"



/// Point class representing positions with a value.

class mxPoint_API mxPoint : public mxPosition
{
    
protected:
    
    /// Value assigned to this point.
	double m_value;

public:

	/// Constructor.
	mxPoint();
    
    /// Destructor.
	~mxPoint();

    /// Copy from input point.
    void CopyFrom(mxPoint &point);
    
    /// Check if the 2D positions (r,c) are equal to the given input positions.
    int IsEqualValueAndPositionIn_2D(mxPoint &point);
    
    /// Check if the 3D positions (s,r,c) are equal to the given input positions.
    int IsEqualValueAndPositionIn_3D(mxPoint &point);
    
    /// Check if the 4D positions (t,s,r,c) are equal to the given input positions.
    int IsEqualValueAndPositionIn_4D(mxPoint &point);
    
    /// Set point value.
    void SetValue(double value);
    
	/// Get point value.
    double& V();
	
	/// Copy from input point.
	mxPoint& operator =(mxPoint &point);
    
    /// Copy from input position.
	mxPoint& operator =(mxPosition &position);
	
	/// Print point.
    mxPoint_API friend std::ostream& operator << (std::ostream &o, mxPoint &point);
};





#endif