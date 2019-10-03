/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeletonization.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxSkeletonization_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxSkeletonization_API
#else
    #if defined(_MSC_VER)
        #ifdef mxSkeletonization_EXPORTS
            #define mxSkeletonization_API __declspec(dllexport)
        #else
            #define mxSkeletonization_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxSkeletonization_EXPORTS
            #define mxSkeletonization_API __attribute__((visibility("default")))
        #else
            #define mxSkeletonization_API
        #endif
    #endif
#endif



#ifndef mxSkeletonization_H
    #define mxSkeletonization_H



#include "mxDataObjectFactory.h"
#include "mxFunctionObject.h"
#include "mxGeometry.h"
#include "mxImage.h"


/// Image skeletonization (centerline image extraction) methods.

class mxSkeletonization_API mxSkeletonization : public mxFunctionObject
{
    
public:

    /// For a converged skeletonized input image and given starting voxel 'start_index', make an output path as an array of indexes
    /// by backtracking (decrementing each step by 1) till a voxel with value 1 is found.
    int Backtrack(mxImage &input, mxIndex &higher_value_index, mxIndex &lower_value_index, mxList< mxIndex > &output);

    
    /// For a converged skeletonized input image and given starting voxel 'start_index', make an output path as an array of indexes
    /// by backtracking (decrementing each step by 1) till a voxel with value 1 is found.
    /// For this iterative version, output MUST BE set to right size and filled with 0 before iterative calling of the method.
    int Backtrack_Iterative(mxImage &input, mxIndex &start_index, mxImage &output);

    
//    /// For a converged skeletonized input image and given starting voxel 'start_index', make an output path as an array of indexes
//    /// by backtracking (decrementing each step by 1) till a voxel with value 1 is found.
//    int Backtrack(mxImage &input, mxIndex &start_index, mxArray< mxIndex > &output);
    
    /// For a skeletonized input image and a given non-zero voxel in the input with indexes start_index, makes an output
    /// image where each non-zero voxel has a value of step distance from the start_index.
    /// Combine this method with backtracking to get shortest paths.
    int ConvergeFromStartIndex(mxImage &input, mxIndex &start_index, mxImage &output);
    
    /// For a skeletonized input image and a given non-zero voxel in the input with indexes start_index, makes an output
    /// image where each non-zero voxel has a value of step distance from the start_index.
    /// Combine this method with backtracking to get shortest paths.
    /// For this iterative version, output MUST BE set to right size and filled with 0 before iterative calling of the method.
    int ConvergeFromStartIndex_Iterative(mxImage &input, mxIndex &start_index, mxImage &output);

    /// For a skeletonized input_output image and a given non-zero voxel in the input_output with indexes start_index,
    /// changes non-zero voxel values in input_output image such that each non-zero voxel has a value of step distance from the start_index.
    /// Combine this method with backtracking to get shortest paths.
    int ConvergeFromStartIndex(mxImage &input_output, mxIndex &start_index);
    
	/// Number of foreground 8-neighbors.
	unsigned int NumberOfNonZero_8_Neighbors(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c);

	/// Number of foreground 26-neighbors.
	unsigned int NumberOfNonZero_26_Neighbors(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c);

	int Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c, unsigned int slice2);
	
	int Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c, unsigned int slice2);
	
    // Checks if the voxels in the 8-neighborhood of the currently processed voxel belong to the same connected component.
    // If yes, the voxel is redundant, i.e. it can be removed without disconnecting the topology.
	int IsRedundantInNeighborhood8_Iterative(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c);

    // Checks if the voxels in the 26-neighborhood of the currently processed voxel belong to the same connected component.
    // If yes, the voxel is redundant, i.e. it can be removed without disconnecting the topology.
	int IsRedundantInNeighborhood26_Iterative(mxImage &image, unsigned int t, unsigned int s, unsigned int r, unsigned int c);

	/// Skeletonize a gray-scale image by making a histogram and checking binary connectivity criterion starting with lowest intensity values to highest. 
	/// When the highest gray values are to be skeletonized, it is performed in the order of the distance transform of only highest values in the object.
	/// 'repetition_scale' indicates how many values (intensities) before the current are being processed with the current.
	int OrderedSkeletonization(mxImage &input, mxImage &output, unsigned int t=0);

    /// Input is skeletonized image, output is skeletonized image where for each node connected component multiple paths within the component have been removed.
    int FilterMultipathsInNodeConnectedComponents(mxImage &input, mxImage &output, unsigned int t=0);


    
};



#endif
