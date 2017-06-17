/*=========================================================================
 
 Program:   mipx
 Module:    mxVoxel.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxVoxel_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxVoxel_API
#else
    #if defined(_MSC_VER)
        #ifdef mxVoxel_EXPORTS
            #define mxVoxel_API __declspec(dllexport)
        #else
            #define mxVoxel_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxVoxel_EXPORTS
            #define mxVoxel_API __attribute__((visibility("default")))
        #else
            #define mxVoxel_API
        #endif
    #endif
#endif



#ifndef mxVoxel_H
    #define mxVoxel_H



#include "mxIndex.h"



/// Voxel class representing indexes with a value.

class mxVoxel_API mxVoxel : public mxIndex
{
    
protected:
    
    /// Value assigned to this pixel.
	unsigned int m_value;

public:

	/// Constructor.
	mxVoxel();
    
    /// Destructor.
	~mxVoxel();

    /// Copy from input voxel.
    void CopyFrom(mxVoxel &voxel);
    
    /// Check if the 2D indexes (r,c) are equal to the given input indexes.
    int IsEqualValueAndIndexIn_2D(mxVoxel &voxel);
    
    /// Check if the 3D indexes (s,r,c) are equal to the given input indexes.
    int IsEqualValueAndIndexIn_3D(mxVoxel &voxel);
    
    /// Check if the 4D indexes (t,s,r,c) are equal to the given input indexes.
    int IsEqualValueAndIndexIn_4D(mxVoxel &voxel);
    
    /// Set voxel value.
    void SetValue(unsigned int value);
    
	/// Get voxel value.
    unsigned int& V();
	
	/// Copy from input voxel.
	mxVoxel& operator =(mxVoxel &voxel);
    
    /// Copy from input index.
	mxVoxel& operator =(mxIndex &index);
	
	/// Print voxel.
    mxVoxel_API friend std::ostream& operator << (std::ostream &o, mxVoxel &voxel);
};





#endif