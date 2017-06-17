/*=========================================================================
 
 Program:   mipx
 Module:    mxGeometry.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxGeometry_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxGeometry_API
#else
    #if defined(_MSC_VER)
        #ifdef mxGeometry_EXPORTS
            #define mxGeometry_API __declspec(dllexport)
        #else
            #define mxGeometry_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxGeometry_EXPORTS
            #define mxGeometry_API __attribute__((visibility("default")))
        #else
            #define mxGeometry_API
        #endif
    #endif
#endif



#ifndef mxGeometry_H
    #define mxGeometry_H



#include "mxArray.h"
#include "mxIndex.h"
#include "mxList.h"



template class mxGeometry_API mxArray<unsigned int>;



//-----------------------------------------------------------------------------------------------------------


/// Geometric shapes and objects for 2D and 3D images. Note: the mxGeometry object is in fact an iterator. This means
/// that a single object is used for a single FOR loop. If another loop needs to be entered within the first loop,
/// a new mxGeometry object needs to be used.
///
/// Example for iterating over sphere neighborhood:
///
/// mxGeometry g;
/// for(g.ForSphere(s,r,c,squared_radius_included1); g.GetSphere(squared_radius_included2,sn,rn,cn); )
/// {... do something...}
///

class mxGeometry_API mxGeometry
{
    
protected:
    
	/// Current value of index that iterates over arrays of indexes (the value of index is either size
    /// in case of square and cube or squared radiu in case of sphere and circle.
    int m_current_array_index;
    
    /// Current value of index that iterates over indexes in one of the size/squared_radius arrays.
    int m_current_local_index;

	/// Central pixel indexes used in FOR loop.
	mxIndex m_current_central_indexes;
    
    /// Spacial dimensions of the corresponding image.
    int m_dimensions[3];
    
    /// Circle index positions. The main array contains arrays of indexes, each array for the same squared radius.
    static mxArray<mxArray<mxIndex> > m_indexes_circle;
    
    /// Cube index positions. The main array contains arrays of indexes, each array for the same size of cube.
    static mxArray<mxArray<mxIndex> > m_indexes_cube;
    
    /// Sphere index positions. The main array contains arrays of indexes, each array for the same squared radius.
    static mxArray<mxArray<mxIndex> > m_indexes_sphere;
    
    /// Square index positions. The main array contains arrays of indexes, each array for the same size of square.
    static mxArray<mxArray<mxIndex> > m_indexes_square;
    
    /// Indexes marking the starting position of (half) line.
    mxIndex m_line_begin_indexes;
    
    /// Indexes marking the direction of (half) line.
    mxIndex m_line_direction_indexes;
    
    /// Current value of index used to iterate over position of (half) line.
    int m_line_current_local_index;
    
    /// Unit of difference in s,r,c coordinates used to calculate the new position indexes on (half)line.
    double m_line_delta_s;
    double m_line_delta_r;
    double m_line_delta_c;
    
    /// Initiate looking for 4 neighbors of given input indexes. It is used by other For_ methods.
    void ForIndexes(int s, int r, int c, unsigned int start_array_index_included);
    
    
public:

	/// Constructor.
	mxGeometry();

	/// Destructor.
	~mxGeometry();
	
	/// Checks if the given coordinates are valid in the set dimensions of the object (values set with SetDimensions() method).
	int IsIndexValid(int s, int r, int c);

    /// Initiate looking for 4 neighbors of given input indexes. Use as the second argument in FOR loop.
	void For_4_Neighborhood(int r, int c);
    
    /// Initiate looking for 6 neighbors of given input indexes. Use as the second argument in FOR loop.
    void For_6_Neighborhood(int s, int r, int c);
    
    /// Initiate looking for 8 neighbors of given input indexes. Use as the second argument in FOR loop.
    void For_8_Neighborhood(int r, int c);
    
    /// Initiate looking for 9 neighbors of given input indexes. Use as the second argument in FOR loop.
	void For_9_Neighborhood(int r, int c);
    
    /// Initiate looking for 26 neighbors of given input indexes. Use as the second argument in FOR loop.
	void For_26_Neighborhood(int s, int r, int c);
    
    /// Initiate looking for 27 neighbors of given input indexes. Use as the second argument in FOR loop.
	void For_27_Neighborhood(int s, int r, int c);
    
    /// Initiate looking for circle neighbors of given input indexes. Use as the second argument in FOR loop.
	void ForCircle(int r, int c, unsigned int start_squared_radius_included);
    
    /// Initiate looking for cube neighbors of given input indexes. Use as the second argument in FOR loop.
    void ForCube(int s, int r, int c, unsigned int start_size_included);
    
    /// Initiate looking for line neighbors of given input indexes. Use as the second argument in FOR loop.
    void ForLine(int s_start_included, int r_start_included, int c_start_included, int direction_s, int direction_r, int direction_c);
    
    /// Initiate looking for sphere neighbors of given input indexes. Use as the second argument in FOR loop.
    void ForSphere(int s, int r, int c, unsigned int start_squared_radius_included);
    
    /// Initiate looking for square neighbors of given input indexes. Use as the second argument in FOR loop.
    void ForSquare(int r, int c, unsigned int start_size_included);
    
	/// Get 4 neighbors that fall within the preset dimensions. Use as the second argument in FOR loop.
	int Get_4_Neighborhood(int &r, int &c);
    int Get_4_Neighborhood(mxIndex &output_indexes);
    
    /// Get 6 neighbors that fall within the preset dimensions. Use as the second argument in FOR loop.
    int Get_6_Neighborhood(int &s, int &r, int &c);
    int Get_6_Neighborhood(mxIndex &output_indexes);
    
    /// Get 8 neighbors that fall within the preset dimensions. Use as the second argument in FOR loop.
    int Get_8_Neighborhood(int &r, int &c);
	int Get_8_Neighborhood(mxIndex &output_indexes);
    
    /// Get 9 neighbors that fall within the preset dimensions. Use as the second argument in FOR loop.
	int Get_9_Neighborhood(int &r, int &c);
	int Get_9_Neighborhood(mxIndex &output_indexes);
    
    /// Get 26 neighbors that fall within the preset dimensions. Use as the second argument in FOR loop.
	int Get_26_Neighborhood(int &s, int &r, int &c);
	int Get_26_Neighborhood(mxIndex &output_indexes);
    
    /// Get 27 neighbors that fall within the preset dimensions. Use as the second argument in FOR loop.
	int Get_27_Neighborhood(int &s, int &r, int &c);
	int Get_27_Neighborhood(mxIndex &output_indexes);
    
    /// Get circle neighbors that fall within the preset dimensions and given end_square_radius_included. Use as the second argument in FOR loop.
	int GetCircle(unsigned int end_square_radius_included, int &r, int &c);
	int GetCircle(unsigned int end_square_radius_included, mxIndex &output_indexes);
    
    /// Get cube neighbors that fall within the preset dimensions and given end_size_included. Use as the second argument in FOR loop.
    int GetCube(unsigned int end_size_included, int &s, int &r, int &c);
    int GetCube(unsigned int end_size_included, mxIndex &output_indexes);
    
    /// Get (half)line indexes that fall within the preset dimensions. Use as the second argument in FOR loop.
    int GetLine(int &s, int &r, int &c);
    int GetLine(mxIndex &output_indexes);
    
    /// Get sphere neighbors that fall within the preset dimensions and given end_square_radius_included. Use as the second argument in FOR loop.
	int GetSphere(unsigned int end_square_radius_included, int &s, int &r, int &c);
	int GetSphere(unsigned int end_square_radius_included, mxIndex &output_indexes);
    
    /// Get square neighbors that fall within the preset dimensions and given end_size_included. Use as the second argument in FOR loop.
    int GetSquare(unsigned int end_size_included, int &r, int &c);
    int GetSquare(unsigned int end_size_included, mxIndex &output_indexes);
   
    /// Generate arrays of indexes for circle neighborhood.
    void PopulateCircleIndexArray(unsigned int maximum_radius = 50);
    
    /// Generate arrays of indexes for cube neighborhood.
    void PopulateCubeIndexArray(unsigned int maximum_size = 30);
    
    /// Generate arrays of indexes for sphere neighborhood.
    void PopulateSphereIndexArray(unsigned int maximum_radius = 30);
    
    /// Generate arrays of indexes for square neighborhood.
    void PopulateSquareIndexArray(unsigned int maximum_size = 50);
    
    /// Set dimensions (e.g. from dimensions of corresponding image).
    void SetDimensions(unsigned int slices, unsigned int rows, unsigned int columns);
    void SetDimensions(unsigned int rows, unsigned int columns);


};


#endif