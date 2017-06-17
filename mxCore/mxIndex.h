/*=========================================================================
 
 Program:   mipx
 Module:    mxIndex.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxIndex_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxIndex_API
#else
    #if defined(_MSC_VER)
        #ifdef mxIndex_EXPORTS
            #define mxIndex_API __declspec(dllexport)
        #else
            #define mxIndex_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxIndex_EXPORTS
            #define mxIndex_API __attribute__((visibility("default")))
        #else
            #define mxIndex_API
        #endif
    #endif
#endif



#ifndef mxIndex_H
    #define mxIndex_H



#include <iostream>
#include <math.h>



/// Index (discrete coordinates) class.
/// Note: supports negative indexes.

class mxIndex_API mxIndex
{
    
protected:

	/// Indexes for C, R, S, T coordinates.
	int m_indexes[4];

public:

	/// Constructor.
    mxIndex();
	
    /// Destructor.
    ~mxIndex();
    
    /// Copy of input indexes.
    void CopyFrom(mxIndex &index);
    
    /// Calculate city block distance to input indexes: d_city_block = d_slices + d_rows + d_columns.
    /// Note: time index is not included in distance calculation because it refers to time.
    unsigned int DistanceCityBlock(mxIndex &indexes);
    
    /// Calculate Euclidean distance to input indexes: d_euclidean = sqrt (d_slices^2 + d_rows^2 + d_columns^2).
    /// Note: time index is not included in distance calculation because it refers to time.
    double DistanceEuclidean(mxIndex &indexes);
    
    /// Calculate maximum norm distance to input indexes: d_max_norm = max {|d_slices|, |d_rows|, |d_columns|}.
    /// Note: time index is not included in distance calculation because it refers to time.
    unsigned int DistanceMaximumNorm(mxIndex &index);
    
    /// Calculate squared Euclidean distance to input indexes: d_euclidean_squared = d_slices^2 + d_rows^2 + d_columns^2.
    /// Note: time index is not included in distance calculation because it refers to time.
    unsigned int DistanceSquaredEuclidean(mxIndex &index);

    /// Get pointer to index array.
    int* GetIndexes();

    /// Check if the 2D indexes (r,c) are equal to the given input indexes.
    int IsEqualIndexIn_2D(mxIndex &index);
    
    /// Check if the 3D indexes (s,r,c) are equal to the given input indexes.
    int IsEqualIndexIn_3D(mxIndex &index);
    
    /// Check if the 4D indexes (t,s,r,c) are equal to the given input indexes.
    int IsEqualIndexIn_4D(mxIndex &index);
    
    /// Get column index.
	int& C();
    
    /// Get row index.
    int& R();
    
    /// Get slice index.
    int& S();

    /// Get time index.
    int& T();
    
    /// Reset the indexes to 0 values.
    void Reset();
    
    /// Set 2D indexes from the given array.
    int SetIndexes2D(int *cr);
    
    /// Set 3D indexes from the given array.
    int SetIndexes3D(int *crs);
    
    /// Set 4D indexes from the given array.
    int SetIndexes4D(int *crst);
	
    /// Calls CopyFrom() method.
    mxIndex& operator =(mxIndex &index);

    /// Set indexes.
    void SetIndex(int t_index, int s_index, int r_index, int c_index);
    void SetIndex(int s_index, int r_index, int c_index);
    void SetIndex(int r_index, int c_index);
    
	/// Print index.
    mxIndex_API friend std::ostream& operator << (std::ostream &o, mxIndex &index);
};



#endif