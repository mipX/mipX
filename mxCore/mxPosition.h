/*=========================================================================
 
 Program:   mipx
 Module:    mxPosition.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxPosition_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxPosition_API
#else
    #if defined(_MSC_VER)
        #ifdef mxPosition_EXPORTS
            #define mxPosition_API __declspec(dllexport)
        #else
            #define mxPosition_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxPosition_EXPORTS
            #define mxPosition_API __attribute__((visibility("default")))
        #else
            #define mxPosition_API
        #endif
    #endif
#endif



#ifndef mxPosition_H
    #define mxPosition_H



#include <iostream>
#include <math.h>



/// Position (real coordinates) class.

class mxPosition_API mxPosition
{
    
protected:

	/// Positions for C, R, S, T coordinates.
	double m_positions[4];

public:

	/// Constructor.
    mxPosition();
	
    /// Destructor.
    ~mxPosition();
    
    /// Copy of input positions.
    void CopyFrom(mxPosition &position);
    
    /// Calculate city block distance to input positions: d_city_block = d_slices + d_rows + d_columns.
    /// Note: time position is not included in distance calculation because it refers to time.
    unsigned int DistanceCityBlock(mxPosition &positions);
    
    /// Calculate Euclidean distance to input positions: d_euclidean = sqrt (d_slices^2 + d_rows^2 + d_columns^2).
    /// Note: time position is not included in distance calculation because it refers to time.
    double DistanceEuclidean(mxPosition &positions);
    
    /// Calculate maximum norm distance to input positions: d_max_norm = max {|d_slices|, |d_rows|, |d_columns|}.
    /// Note: time position is not included in distance calculation because it refers to time.
    unsigned int DistanceMaximumNorm(mxPosition &position);
    
    /// Calculate squared Euclidean distance to input positions: d_euclidean_squared = d_slices^2 + d_rows^2 + d_columns^2.
    /// Note: time position is not included in distance calculation because it refers to time.
    unsigned int DistanceSquaredEuclidean(mxPosition &position);

    /// Get pointer to position array.
    double* GetPositions();

    /// Check if the 2D positions (r,c) are equal to the given input positions.
    int IsEqualPositionIn_2D(mxPosition &position);
    
    /// Check if the 3D positions (s,r,c) are equal to the given input positions.
    int IsEqualPositionIn_3D(mxPosition &position);
    
    /// Check if the 4D positions (t,s,r,c) are equal to the given input positions.
    int IsEqualPositionIn_4D(mxPosition &position);
    
    /// Get column position.
	double& C();
    
    /// Get row position.
    double& R();
    
    /// Get slice position.
    double& S();

    /// Get time position.
    double& T();
    
    /// Reset the positions to 0 values.
    void Reset();
    
    /// Set 2D positions from the given array.
    int SetPositions2D(int *cr);
    
    /// Set 3D positions from the given array.
    int SetPositions3D(int *crs);
    
    /// Set 4D positions from the given array.
    int SetPositions4D(int *crst);
	
    /// Calls CopyFrom() method.
    mxPosition& operator =(mxPosition &position);

    /// Set positions.
    void SetPosition(int t_position, int s_position, int r_position, int c_position);
    void SetPosition(int s_position, int r_position, int c_position);
    void SetPosition(int r_position, int c_position);
    
	/// Print position.
    mxPosition_API friend std::ostream& operator << (std::ostream &o, mxPosition &position);
};



#endif