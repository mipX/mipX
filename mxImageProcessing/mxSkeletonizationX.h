/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeletonizationX.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxSkeletonizationX_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxSkeletonizationX_API
#else
    #if defined(_MSC_VER)
        #ifdef mxSkeletonizationX_EXPORTS
            #define mxSkeletonizationX_API __declspec(dllexport)
        #else
            #define mxSkeletonizationX_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxSkeletonizationX_EXPORTS
            #define mxSkeletonizationX_API __attribute__((visibility("default")))
        #else
            #define mxSkeletonizationX_API
        #endif
    #endif
#endif



#ifndef mxSkeletonizationX_H
    #define mxSkeletonizationX_H



#include "mxDataObjectFactory.h"
#include "mxBasicImage.h"
#include "mxBIPX.h"
#include "mxFunctionObject.h"
#include "mxGeometry.h"


/// Image skeletonization (centerline image extraction) methods.

class mxSkeletonizationX_API mxSkeletonizationX : public mxFunctionObject
{

protected:
    
//    int Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(mxBasicImage &input, mxGeometry &g, mxBasicImage &temp_image_2x3x3, unsigned int t, unsigned int s_curent_slice, unsigned int r, unsigned int c, unsigned int s_second_slice);
    
    int Is_3x3_SlicePartiallyConnectedToCurrentSlice_Iterative(mxBasicImage &current3x3, mxBasicImage &second3x3, mxGeometry &g3x3);
    
    
//    int Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(mxBasicImage &input, mxGeometry &g, mxBasicImage &temp_slice_3x3, unsigned int t, unsigned int s_curent_slice, unsigned int r, unsigned int c, unsigned int s_second_slice);
    
    int Is_3x3_SliceCompletelyConnectedToCurrentSlice_Iterative(mxBasicImage &current3x3, mxBasicImage &second3x3, mxGeometry &g3x3);

    
    // Checks if the voxels in the 8-neighborhood of the currently processed voxel belong to the same connected component.
    // If yes, the voxel is redundant, i.e. it can be removed without disconnecting the topology.
    int IsRedundantInNeighborhood8_Iterative(mxBasicImage &temp_slice_3x3);
    
    
//    // Checks if the voxels in the 26-neighborhood of the currently processed voxel belong to the same connected component.
//    // If yes, the voxel is redundant, i.e. it can be removed without disconnecting the topology.
//    int IsRedundantInNeighborhood26_Iterative(mxBasicImage &temp_slice_3x3x3);

    
    // Checks if the voxels in the 26-neighborhood of the currently processed voxel belong to the same connected component.
    // If yes, the voxel is redundant, i.e. it can be removed without disconnecting the topology.
    int IsRedundantInNeighborhood26_Iterative(mxBasicImage &previous3x3, mxBasicImage &current3x3, mxBasicImage &next3x3, mxGeometry &g3x3);

    
    
public:
    
   
    /// Skeletonize the input segmented image in ascending order of foreground voxel gray values (from lowest values to highest).
    /// The range of the highest gray values found in the input image is extended by performing a distance transform only on the voxels
    /// with the highest gray value.
	int OrderedSkeletonization(mxBasicImage &input, mxBasicImage &output, unsigned int t=0);


};



#endif