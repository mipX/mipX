/*=========================================================================
 
 Program:   mipx
 Module:    mxGraphProcessing.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxGraphProcessing_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxGraphProcessing_API
#else
    #if defined(_MSC_VER)
        #ifdef mxGraphProcessing_EXPORTS
            #define mxGraphProcessing_API __declspec(dllexport)
        #else
            #define mxGraphProcessing_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxGraphProcessing_EXPORTS
            #define mxGraphProcessing_API __attribute__((visibility("default")))
        #else
            #define mxGraphProcessing_API
        #endif
    #endif
#endif



#ifndef mxGraphProcessing_H
    #define mxGraphProcessing_H



#include "mxDataObjectFactory.h"
#include "mxFunctionObject.h"
#include "mxGeometry.h"
#include "mxGraph.h"
#include "mxImage.h"


/// Graph processing methods.

class mxGraphProcessing_API mxGraphProcessing : public mxFunctionObject
{
    
public:

//    /// Create a graph from an image. Each voxel will be a vertex and edges will be added between voxels of 26-neighborhood.
//    int CreateGraphFromImage(mxImage &image, mxGraph3D &output_graph3D, unsigned int t=0);
    
    /// Create a graph from an image. Each voxel will be a vertex and edges will be added between voxels of 26-neighborhood.
    int CreateGraphFromImage(mxImage &image, mxGraph &output_graph);

    
//    /// Create an image from the input graph.
//    int CreateImageFromGraph(mxGraph &graph, mxImage &output_image);
	
};



#endif
