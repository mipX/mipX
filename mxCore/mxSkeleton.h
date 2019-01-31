/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeleton.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxSkeleton_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxSkeleton_API
#else
    #if defined(_MSC_VER)
        #ifdef mxSkeleton_EXPORTS
            #define mxSkeleton_API __declspec(dllexport)
        #else
            #define mxSkeleton_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxSkeleton_EXPORTS
            #define mxSkeleton_API __attribute__((visibility("default")))
        #else
            #define mxSkeleton_API
        #endif
    #endif
#endif



#ifndef mxSkeleton_H
    #define mxSkeleton_H


#include "mxArray.h"
#include "mxDataObject.h"
#include "mxGraph.h"
#include "mxList.h"
#include "mxMatrix.h"
#include "mxPosition.h"
#include "mxString.h"

#include <strstream>
#include <fstream>
#include <iomanip>


/// Pre-declare class of Skeleton links.
class mxSkeletonLink;


/// Node of the Skeleton.

class mxSkeletonNode
{
    
public:
    
    /// List of graph edges that belong to this node.
    mxList< mxGraphEdge* > m_graph_edges;
    
    /// List of graph vertices that belong to this node.
    mxList< mxGraphVertex* > m_graph_vertices;
    
    /// Maintain the pointer to the list node (container) for this node. It will allow us to remove the node in a fast way (if needed).
    mxListNode < mxSkeletonNode >* m_container;

    /// Coordinates of the node.
    mxPosition m_position;

    /// List of links that are connected to this node.
    mxList< mxSkeletonLink* > m_links;
    
    /// Cost of the vertex ("distance" from a source/start vertex in the Skeleton).
    double m_cost;
    
//    /// Indicates if the vertex was recently updated in shortest path calculation. This is used for cost propagation in shortest path calculations.
//    int m_SP_is_updated;

    
    
    /// Constructor.
    mxSkeletonNode() { m_cost = 0; m_container = NULL; }; //m_SP_is_updated = 0; };
    
    /// Destructor.
    ~mxSkeletonNode() {};
    
    /// Get adjacent node over the given link.
    mxSkeletonNode* GetAdjacentNode(mxSkeletonLink *link);

    /// Get cost ("distance" from a source/start vertex in the Skeleton) of the vertex.
    double GetCost() { return m_cost; };

    /// Get links incident to this vertex.
    mxList< mxSkeletonLink* >* GetLinks() { return &m_links; };

    /// GetNumber of incident links.
    int GetNumberOfLinks() { return m_links.GetNumberOfElements(); };

    /// Get position.
    mxPosition* GetPosition() { return &m_position; };

    /// Check if the this node is adjacent to the input node.
    int IsAdjacentTo(mxSkeletonNode *node);

    /// Set cost ("distance" from a source/start node in the Skeleton) of the node.
    void SetCost(double cost) { m_cost = cost; };

    /// Set position of vertex.
    void SetPosition(double s, double r, double c) { m_position.SetPosition(s,r,c); };

    /// Sends updates to adjacent vertices. The method will be run in case this vertex was updated in the previous iteration (m_SP_is_updated is set) od shortest path calculation.
//    /// This is used for cost propagation in shortest path calculations.
//    int SendUpdates();
};


/// Link of the Skeleton.

class mxSkeletonLink
{
    
public:
    
    /// List of graph edges that belong to this link.
    mxList< mxGraphEdge* > m_graph_edges;
    
    /// List of graph vertices that belong to this link.
    mxList< mxGraphVertex* > m_graph_vertices;
    
    /// The first port edge of this link (in case link has only one edge it can be the same as the second one).
    mxGraphEdge* m_graph_port_edge1;
    
    /// The second port edge of this link (in case link has only one edge it can be the same as the first one).
    mxGraphEdge* m_graph_port_edge2;
    
    /// The first port vertex of this link.
    mxGraphVertex* m_graph_port_vertex1;

    /// The second port vertex of this link.
    mxGraphVertex* m_graph_port_vertex2;
    
    /// Maintain the pointer to the list node (container) for this link. It will allow us to remove the link in a fast way (if needed).
    mxListNode < mxSkeletonLink >* m_container;
    
    /// The first (initial) vertex that this edge connects.
    mxSkeletonNode *m_node1;
    
    /// The second (ending) vertex that this edge connects.
    mxSkeletonNode *m_node2;
    
    /// Coordinates of the link positions - these are NOT graph vertices. They are used for representing the links.
    mxList< mxPosition > m_positions;
    
    /// Weight of this link.
    double m_weight;
    
    
    /// Constructor.
    mxSkeletonLink() { m_node1 = m_node2 = NULL; m_weight = 0; m_container = NULL; };

    /// Destructor.
    ~mxSkeletonLink() {};
    
    /// Get the first (initial) node that this link connects.
    mxSkeletonNode* GetNode1() { return m_node1; };
    
    /// Get the second (ending) node that this link connects.
    mxSkeletonNode* GetNode2() { return m_node2; };
    
    /// Get weight of the link.
    double GetWeight() { return m_weight; };

    /// Set weight of the link.
    void SetWeight(double weight) { m_weight = weight; };
};



/// Skeleton (undirectional). This structure exists only in space.

class mxSkeleton3D
{
    
private:
    
    /// List of vertices.
    mxList< mxSkeletonNode > m_nodes;
    
    /// List of edges.
    mxList< mxSkeletonLink > m_links;

    //int converged; //has the Skeleton converged - is the best path found

public:
    
    /// Constructor.
    mxSkeleton3D() {};
    
    /// Destructor.
    ~mxSkeleton3D() {};
    
    /// Add a new node to the skeleton with the specified position.
    mxSkeletonNode* AddNode(double s=0, double r=0, double c=0);
    
    /// Add an edge between existing vertices.
    mxSkeletonLink* AddLink(mxSkeletonNode *node1, mxSkeletonNode *node2);
    
//    /// Perform backtracking from the start_vertex all the way to the end_vertex (if end vertex is not found, performs backtracking to maximum extent).
//    int Backtrack(mxSkeletonNode *start_vertex, mxSkeletonNode *end_vertex, mxSkeleton3D *output_path);
    
//    /// Converge the shortest paths to the given input vertex.
//    void ConvergeForShortestPath(mxSkeletonNode *vertex);

    /// Check if Skeleton is empty (number of vertices less than 2).
    int IsEmpty();

    /// Get number of links in the skeleton.
    int GetNumberOfLinks() { return m_links.GetNumberOfElements(); };
    
    /// Get number of nodes in the skeleton.
    int GetNumberOfNodes() { return m_nodes.GetNumberOfElements(); };
    
    /// Get list of links.
    mxList< mxSkeletonLink >* GetLinks() { return &m_links; };
    
    /// Get list of nodes.
    mxList< mxSkeletonNode >* GetNodes() { return &m_nodes; };
    
    /// Get node with the given position.
    mxSkeletonNode* GetNode(mxPosition &p);
    mxSkeletonNode* GetNode(double s, double r, double c);

    /// Get node that is the closest to the input position.
    mxSkeletonNode* GetNodeClosestToPosition(mxPosition &p);
    mxSkeletonNode* GetNodeClosestToPosition(double s, double r, double c);

    /// Remove the link from the skeleton.
    void RemoveLink(mxSkeletonLink *link);
    
    /// Remove the node from the skeleton along with all its edges.
    void RemoveNode(mxSkeletonNode *node);
    
    /// Reset the object to state just after construction.
    void Reset() { m_nodes.Reset(); m_links.Reset(); };
};



/// Skeleton (undirectional). Consists of an array of space Skeletons (mxSkeleton3D) for each time instance.

class mxSkeleton : public mxGraph //mxDataObject
{
    
protected:
    
//    /// Skeleton is built on top of this graph structure.
//    mxGraph m_graph;
    
    /// Array of Skeletons (3D), one for each time instance.
    mxArray< mxSkeleton3D > m_skeletons;
    
//    /// Origin c,r,s.
//    double m_origin[3];
    
//    /// Spacing c,r,s.
//    double m_spacing[3];
    
public:
    
    /// Constructor.
    mxSkeleton();
    
    /// Destructor.
    virtual ~mxSkeleton() {};
    
    /// Add a new node to the skeleton with the specified position.
    mxSkeletonNode* AddNode(unsigned int t, double s, double r, double c);
    mxSkeletonNode* AddNode(double s, double r, double c) { return this->AddNode(0, s,r,c); };
    mxSkeletonNode* AddNode(unsigned int t=0) { return this->AddNode(t, 0,0,0); };
    
    /// Add a link between existing nodes.
    mxSkeletonLink* AddLink(unsigned int t, mxSkeletonNode *node1, mxSkeletonNode *node2);
    mxSkeletonLink* AddLink(mxSkeletonNode *node1, mxSkeletonNode *node2) { return this->AddLink(0, node1,node2); };
    
    /// Build skeleton from the exsting graph structures.
    int BuildSkeleton();
    
//    /// Perform backtracking from the start_node all the way to the end_vertex (if end vertex is not found, performs backtracking to maximum extent).
//    int Backtrack(mxSkeletonNode *start_vertex, mxSkeletonNode *end_vertex, mxSkeleton *output_path, unsigned int t=0);
    
//    /// Converge the shortest paths to the given input vertex.
//    void ConvergeForShortestPath(mxSkeletonNode *vertex, unsigned int t=0);
    
//    /// Check if Skeleton is empty (number of vertices less than 2).
//    int IsEmpty();
    
    /// Get number of links in the Skeleton.
    int GetNumberOfLinks(unsigned int t=0) { return m_skeletons[t].GetNumberOfLinks(); };
    
//    /// Get number of time instances.
//    int GetNumberOfTimeSeries() { return m_skeletons.GetNumberOfElements(); };
    
    /// Get number of nodes in the skeleton.
    int GetNumberOfNodes(unsigned int t=0) { return m_skeletons[t].GetNumberOfNodes(); };
    
    /// Get list of links.
    mxList< mxSkeletonLink >* GetLinks(unsigned int t=0) { return m_skeletons[t].GetLinks(); };
    
    /// Get 3D Skeleton with the given index.
    mxSkeleton3D& GetSkeleton3D(unsigned int t=0) { return m_skeletons[t]; };
    
//    /// Get origin S (z).
//    double GetOrigin_S() { return m_origin[2]; };
//
//    /// Get origin R (y).
//    double GetOrigin_R() { return m_origin[1]; };
//
//    /// Get origin C (x).
//    double GetOrigin_C() { return m_origin[0]; };
//
//    /// Get spacing S (z).
//    double GetSpacing_S() { return m_spacing[2]; };
//
//    /// Get spacing R (y).
//    double GetSpacing_R() { return m_spacing[1]; };
//
//    /// Get spacing C (x).
//    double GetSpacing_C() { return m_spacing[0]; };
    
    /// Get list of nodes.
    mxList< mxSkeletonNode >* GetNodes(unsigned int t=0) { return m_skeletons[t].GetNodes(); };
    
    /// Get node with the given position.
    mxSkeletonNode* GetNode(unsigned int t, double s, double r, double c);
    mxSkeletonNode* GetNode(unsigned int t, mxPosition &p);
    mxSkeletonNode* GetNode(double s, double r, double c) { return this->GetNode(0, s,r,c); };
    mxSkeletonNode* GetNode(mxPosition &p) { return this->GetNode(0, p); };
    
    /// Get node that is the closest to the input position.
    mxSkeletonNode* GetNodeClosestToPosition(unsigned int t, double s, double r, double c);
    mxSkeletonNode* GetNodeClosestToPosition(unsigned int t, mxPosition &p);
    mxSkeletonNode* GetNodeClosestToPosition(double s, double r, double c) { return this->GetNodeClosestToPosition(0, s,r,c); };
    mxSkeletonNode* GetNodeClosestToPosition(mxPosition &p) { return this->GetNodeClosestToPosition(0, p); };
    
    /// Remove the link from the Skeleton.
    void RemoveLink(unsigned int t, mxSkeletonLink *link);
    void RemoveLink(mxSkeletonLink *link);
    
    /// Remove the node from the Skeleton along with all its links.
    void RemoveNode(unsigned int t, mxSkeletonNode *node);
    void RemoveNode(mxSkeletonNode *node);

    /// Reset the object to state just after construction.
    void Reset();
    
    /// Set number of time instances.
    int SetNumberOfTimeSeries(int number_of_time_instances);// { return m_skeletons.SetNumberOfElements(number_of_time_instances); };
    
//    /// Set origin.
//    void SetOrigin(double s, double r, double c) { m_origin[2] = s; m_origin[1] = r; m_origin[0] = c; };
//
//    /// Set spacing.
//    void SetSpacing(double s, double r, double c) { m_spacing[2] = s; m_spacing[1] = r; m_spacing[0] = c; };
};


#endif
