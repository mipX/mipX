/*=========================================================================
 
 Program:   mipx
 Module:    mxGraphProcessing.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "mxGraphProcessing.h"



//int mxGraphProcessing::CreateGraphFromImage(mxImage &image, mxGraph3D &output_graph3D, unsigned int t)
//{
//    if(image.IsEmpty()) return 0;
//    if(t>=image.GetDimension_T()) return 0;
//
//    output_graph3D.Reset();
//
//    // Go through the image and put every non-zero value voxel as a vertex in the graph.
//    for(unsigned int s=0; s<image.GetDimension_S(); s++)
//    {
//        for(unsigned int r=0; r<image.GetDimension_R(); r++)
//        {
//            for(unsigned int c=0; c<image.GetDimension_C(); c++)
//            {
//                if(image(t,s,r,c)!=0)
//                {
//                    output_graph3D.AddVertex(s,r,c);
//                }
//            }
//        }
//    }
//
//    // Go through the list of graph vertices and create edges between adjacent vertices (voxels in 26-neighborhood go up to distances of sqrt(3), but surely less than 2, so 1.9 is safe).
//    {
//        mxListIterator< mxGraphVertex > it, it2;
//        for(it.SetToBegin(output_graph3D.GetVertices()); it.IsValid(); it.MoveToNext())
//        {
//            for(it2.SetToNode(it.GetNode()->GetNextNode()); it2.IsValid(); it.MoveToNext())
//            {
//                double distance = it.GetElementAddress()->GetPosition()->DistanceEuclidean(it2.GetElementAddress()->GetPosition());
//                if(distance<1.9)
//                {
//                    output_graph3D.AddEdge(it.GetElementAddress(), it2.GetElementAddress());
//                }
//            }
//        }
//    }
//
//    // Go through the list of graph vertices and remove those vertices that are isolated (do not have any adjacent vertices, ie. no edges).
//    {
//        mxList< mxGraphVertex* > vertices_to_delete;
//        for(it.SetToBegin(output_graph3D.GetVertices()); it.IsValid(); it.MoveToNext())
//        {
//            if(it.GetElementAddress()->GetNumberOfEdges()==0)
//            {
//                vertices_to_delete.AddToEnd(it.GetElementAddress());
//            }
//        }
//
//        mxListIterator< mxGraphVertex* > it;
//        for(it.SetToBegin(vertices_to_delete); it.IsValid(); it.MoveToNext())
//        {
//            output_graph3D.RemoveVertex(it.GetElement());
//        }
//    }
//
//    return 1;
//}



int mxGraphProcessing::CreateGraphFromImage(mxImage &image, mxGraph &output_graph)
{
    if(image.IsEmpty()) return 0;
    
    output_graph.SetNumberOfTimeSeries(image.GetDimension_T());
    output_graph.SetOrigin(image.GetOrigin_S(),image.GetOrigin_R(),image.GetOrigin_C());
    output_graph.SetSpacing(image.GetSpacing_S(),image.GetSpacing_R(),image.GetSpacing_C());
    
    for(unsigned int t=0; t<image.GetDimension_T(); t++)
    {
        output_graph.GetGraph3D(t).Reset();
        
        // Go through the image and put every non-zero value voxel as a vertex in the graph.
        for(unsigned int s=0; s<image.GetDimension_S(); s++)
        {
            for(unsigned int r=0; r<image.GetDimension_R(); r++)
            {
                for(unsigned int c=0; c<image.GetDimension_C(); c++)
                {
                    if(image(t,s,r,c)!=0)
                    {
                        output_graph.GetGraph3D(t).AddVertex(s,r,c);
                    }
                }
            }
        }
        
        // Go through the list of graph vertices and create edges between adjacent vertices (voxels in 26-neighborhood go up to distances of sqrt(3), but surely less than 2, so 1.9 is safe).
        {
            mxListIterator< mxGraphVertex > it, it2;
            for(it.SetToBegin(output_graph.GetGraph3D(t).GetVertices()); it.IsValid(); it.MoveToNext())
            {
                for(it2.SetToNode(it.GetNode()->GetNextNode()); it2.IsValid(); it2.MoveToNext())
                {
                    //mxPosition *p = it2.GetElementAddress()->GetPosition();
                    //double distance = it.GetElementAddress()->GetPosition()->DistanceEuclidean(*p);
                    double distance = it.GetElementAddress()->GetPosition()->DistanceEuclidean(*(it2.GetElementAddress()->GetPosition()));
                    if(distance<1.9)
                    {
                        output_graph.GetGraph3D(t).AddEdge(it.GetElementAddress(), it2.GetElementAddress());
                    }
                }
            }
        }
        
        // Go through the list of graph vertices and remove those vertices that are isolated (do not have any adjacent vertices, ie. no edges).
        {
            mxListIterator< mxGraphVertex > it;
            mxList< mxGraphVertex* > vertices_to_delete;
            for(it.SetToBegin(output_graph.GetGraph3D(t).GetVertices()); it.IsValid(); it.MoveToNext())
            {
                if(it.GetElementAddress()->GetNumberOfEdges()==0)
                {
                    mxGraphVertex *v = it.GetElementAddress() ;
                    vertices_to_delete.AddToEnd(v);
                }
            }
            
            mxListIterator< mxGraphVertex* > it2;
            for(it2.SetToBegin(vertices_to_delete); it2.IsValid(); it2.MoveToNext())
            {
                output_graph.GetGraph3D(t).RemoveVertex(it2.GetElement());
            }
        }
    }
    
    return 1;
}



//int mxGraphProcessing::CreateImageFromGraph(mxGraph &graph, mxImage &output_image)
//{
//
//}
