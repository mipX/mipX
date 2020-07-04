/*=========================================================================
 
 Program:   mipx
 Module:    mxGraph.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxGraph_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxGraph_API
#else
    #if defined(_MSC_VER)
        #ifdef mxGraph_EXPORTS
            #define mxGraph_API __declspec(dllexport)
        #else
            #define mxGraph_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxGraph_EXPORTS
            #define mxGraph_API __attribute__((visibility("default")))
        #else
            #define mxGraph_API
        #endif
    #endif
#endif



#ifndef mxGraph_H
    #define mxGraph_H


#include "mxArray.h"
#include "mxDataObject.h"
#include "mxList.h"
#include "mxMatrix.h"
#include "mxPosition.h"
#include "mxString.h"

//#include <strstream>
#include <fstream>
//#include <iomanip>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




/// Pre-declare class of graph edges.
class mxGraphEdge;


/// Vertex of a graph.

class mxGraph_API mxGraphVertex
{
    
public:
    
    /// Maintain the pointer to the list node (container) for this vertex. It will allow us to remove the vertex in a fast way (if needed).
    mxListNode < mxGraphVertex >* m_container;
    
    /// Cost of the vertex ("distance" from a source/start vertex in the graph).
    double m_cost;
    
    /// List of edges that are connected to this vertex.
    mxList< mxGraphEdge* > m_edges;
    
    /// ID number used for iterating over vertices in a fast way.
    int m_ID;
    
    /// Coordinates of the vertex.
    mxPosition m_position;
    
     /// Indicates if the vertex was recently updated in shortest path calculation. This is used for cost propagation in shortest path calculations.
    int m_SP_is_updated;

    
    
    /// Constructor.
    mxGraphVertex() { m_cost = 0; m_SP_is_updated = 0; m_container = NULL; m_ID = -1; };
    
    /// Destructor.
    ~mxGraphVertex() {};
    
    /// Get adjacent vertex over the given edge.
    mxGraphVertex* GetAdjacentVertex(mxGraphEdge *edge);
    
    /// Get adjacent vertices of this vertex.
    int GetAdjacentVertices(mxArray<mxGraphVertex*> &adjacent_vertices);

    /// Get cost ("distance" from a source/start vertex in the graph) of the vertex.
    double GetCost() { return m_cost; };
    
    /// Get edges incident to this vertex.
    mxList< mxGraphEdge* >* GetEdges() { return &m_edges; };
    
    /// GetNumber of incident edges.
    int GetNumberOfEdges() { return m_edges.GetNumberOfElements(); };
    
    /// Get position.
    mxPosition* GetPosition() { return &m_position; };
    
    /// Check if the this vertex is adjacent to the input vertex.
    int IsAdjacentTo(mxGraphVertex *vertex);
    
    /// Check if the vertex is a link vertex (has only 2 edges, i.e. is a part of a path/link).
    int IsLinkVertex();
    
    /// Check if the vertex is a node vertex (part of skeleton node, has 1 or more than 2 edges), i.e. if the vertex is not a link vetex.
    int IsNodeVertex();
    
    /// Check if the vertex is a port vertex (has at least one link edge and at least one node edge (non-link edge)), i.e. it is a vertex that connects a node to a link.
    /// Note: port vertex set is a subset of node vertex set.
    int IsPortVertex();
    
    /// Check if this vertex is a stub, i.e. if it has only 1 incidet edge.
    int IsStubVertex();
    
    /// Set cost ("distance" from a source/start vertex in the graph) of the vertex.
    void SetCost(double cost) { m_cost = cost; };

    /// Set position of vertex.
    void SetPosition(double s, double r, double c) { m_position.SetPosition(s,r,c); };

    /// Sends updates to adjacent vertices. The method will be run in case this vertex was updated in the previous iteration (m_SP_is_updated is set) od shortest path calculation.
    /// This is used for cost propagation in shortest path calculations.
    int SendUpdates();
    
//    friend ostream& operator << (ostream &o, mxGraphVertex &v);
};


/// Edge of a graph.

class mxGraph_API mxGraphEdge
{
    
public:
    
    /// Maintain the pointer to the list node (container) for this edge. It will allow us to remove the edge in a fast way (if needed).
    mxListNode < mxGraphEdge >* m_container;
    
    /// ID number used for iterating over edges in a fast way.
    int m_ID;
    
    /// The first (initial) vertex that this edge connects.
    mxGraphVertex *m_vertex1;
    
    /// The second (ending) vertex that this edge connects.
    mxGraphVertex *m_vertex2;
    
    /// Weight of this edge.
    double m_weight;
    
    /// Constructor.
    mxGraphEdge() { m_vertex1 = m_vertex2 = NULL; m_weight = 1; m_container = NULL; m_ID = -1; };

    /// Destructor.
    ~mxGraphEdge() {};
    
    /// Get the first (initial) vertex that this edge connects.
    mxGraphVertex* GetVertex1() { return m_vertex1; };
    
    /// Get the second (ending) vertex that this edge connects.
    mxGraphVertex* GetVertex2() { return m_vertex2; };
    
    /// Get weight of the edge.
    double GetWeight() { return m_weight; };
    
    /// Check if the edge is a link edge (i.e. is a part of a path/link, which means that at least one of its vertices has number of neighbors <=2).
    int IsLinkEdge();

    /// Check if the edge is a node edge (i.e. is a part of a skeleton node, which means that it is not a link edge).
    int IsNodeEdge();
    
    /// Check if the edge is a port edge (has at least one port vertex), i.e. it is an edge that connects a node to a link.
    /// Note: port edge set is a subset of link edge set.
    int IsPortEdge();
    
    /// Check if this edge is a stub, i.e. if it has at least one stub vertex.
    int IsStubEdge();
    
    /// Set weight of the edge.
    void SetWeight(double weight) { m_weight = weight; };
};



/// Graph (undirectional). This structure exists only in space.

class mxGraph_API mxGraph3D
{
    
private:
    
    /// List of vertices.
    mxList< mxGraphVertex > m_vertices;
    
    /// List of edges.
    mxList< mxGraphEdge > m_edges;

    //int converged; //has the graph converged - is the best path found

public:
    
    /// Constructor.
    mxGraph3D() {};
    
    /// Destructor.
    ~mxGraph3D() {};
    
    /// Add a new vertex to the graph with the specified position.
    mxGraphVertex* AddVertex(double s=0, double r=0, double c=0);
    
    /// Add an edge between existing vertices.
    mxGraphEdge* AddEdge(mxGraphVertex *vertex1, mxGraphVertex *vertex2);
    
    /// Perform backtracking from the start_vertex all the way to the end_vertex (if end vertex is not found, performs backtracking to maximum extent).
    int Backtrack(mxGraphVertex *start_vertex, mxGraphVertex *end_vertex, mxGraph3D *output_path);
    
    /// Converge the shortest paths to the given input vertex.
    void ConvergeForShortestPath(mxGraphVertex *vertex);

    /// Check if graph is empty (number of vertices less than 2).
    int IsEmpty();

    /// Get number of edges in the graph.
    int GetNumberOfEdges() { return m_edges.GetNumberOfElements(); };
    
    /// Get number of vertices in the graph.
    int GetNumberOfVertices() { return m_vertices.GetNumberOfElements(); };
    
    /// Get list of edges.
    mxList< mxGraphEdge >* GetEdges() { return &m_edges; };
    
    /// Get list of vertices.
    mxList< mxGraphVertex >* GetVertices() { return &m_vertices; };
    
    /// Get vertex with the given position.
    mxGraphVertex* GetVertex(mxPosition &p);
    mxGraphVertex* GetVertex(double s, double r, double c);

    /// Get vertex that is the closest to the input position.
    mxGraphVertex* GetVertexClosestToPosition(mxPosition &p);
    mxGraphVertex* GetVertexClosestToPosition(double s, double r, double c);

    /// Remove the edge from the graph.
    void RemoveEdge(mxGraphEdge *edge);
    
    /// Remove the vertex from the graph along with all its edges.
    void RemoveVertex(mxGraphVertex *vertex);
    
    /// Reset the objectto state just after construction.
    void Reset() { m_vertices.Reset(); m_edges.Reset(); };
};



/// Graph (undirectional). Consists of an array of space graphs (mxGraph3D) for each time instance.

class mxGraph_API mxGraph : public mxDataObject
{
    
protected:
    
    /// Array of graphs (3D), one for each time instance.
    mxArray< mxGraph3D > m_graphs;
    
    /// Origin c,r,s.
    double m_origin[3];
    
    /// Spacing c,r,s.
    double m_spacing[3];
    
public:
    
    /// Constructor.
    mxGraph();
    
    /// Destructor.
    virtual ~mxGraph() {};
    
    /// Add a new vertex to the graph with the specified position.
    mxGraphVertex* AddVertex(unsigned int t, double s, double r, double c);
    mxGraphVertex* AddVertex(double s, double r, double c) { return this->AddVertex(0, s,r,c); };
    mxGraphVertex* AddVertex(unsigned int t=0) { return this->AddVertex(t, 0,0,0); };
    
    /// Add an edge between existing vertices.
    mxGraphEdge* AddEdge(unsigned int t, mxGraphVertex *vertex1, mxGraphVertex *vertex2);
    mxGraphEdge* AddEdge(mxGraphVertex *vertex1, mxGraphVertex *vertex2) { return this->AddEdge(0, vertex1,vertex2); };
    
    /// Perform backtracking from the start_vertex all the way to the end_vertex (if end vertex is not found, performs backtracking to maximum extent).
    int Backtrack(mxGraphVertex *start_vertex, mxGraphVertex *end_vertex, mxGraph *output_path, unsigned int t=0);
    
    /// Converge the shortest paths to the given input vertex.
    void ConvergeForShortestPath(mxGraphVertex *vertex, unsigned int t=0);
    
    /// Check if graph is empty (number of vertices less than 2).
    int IsEmpty();
    
    /// Use this method to iterate over a path (link - composed of vertices that have only 2 links. For input vertex and edge this method
    /// outputs the next vertex and edge on the link. If there are no more edges on the link the return value is 0 and output edge will be NULL.
    /// Use: for(v=start_vertex, e=start_edge; v!=NULL || e!=NULL; this->GetLinkEdgeAndVertex(v,e) ) {...}
    int GetLinkEdgeAndVertex(mxGraphVertex *output_vertex, mxGraphEdge *output_edge);
    
    /// Get number of edges in the graph.
    int GetNumberOfEdges(unsigned int t=0) { return m_graphs[t].GetNumberOfEdges(); };
    
    /// Get number of time instances.
    int GetNumberOfTimeSeries() { return m_graphs.GetNumberOfElements(); };
    
    /// Get number of vertices in the graph.
    int GetNumberOfVertices(unsigned int t=0) { return m_graphs[t].GetNumberOfVertices(); };
    
    /// Get list of edges.
    mxList< mxGraphEdge >* GetEdges(unsigned int t=0) { return m_graphs[t].GetEdges(); };
    
    /// Get 3D graph with the given index.
    mxGraph3D& GetGraph3D(unsigned int t=0) { return m_graphs[t]; };
    
    /// Get origin S (z).
    double GetOrigin_S() { return m_origin[2]; };
    
    /// Get origin R (y).
    double GetOrigin_R() { return m_origin[1]; };

    /// Get origin C (x).
    double GetOrigin_C() { return m_origin[0]; };

    /// Get spacing S (z).
    double GetSpacing_S() { return m_spacing[2]; };
    
    /// Get spacing R (y).
    double GetSpacing_R() { return m_spacing[1]; };
    
    /// Get spacing C (x).
    double GetSpacing_C() { return m_spacing[0]; };
    
    /// Get list of vertices.
    mxList< mxGraphVertex >* GetVertices(unsigned int t=0) { return m_graphs[t].GetVertices(); };
    
    /// Get vertex with the given position.
    mxGraphVertex* GetVertex(unsigned int t, double s, double r, double c);
    mxGraphVertex* GetVertex(unsigned int t, mxPosition &p);
    mxGraphVertex* GetVertex(double s, double r, double c) { return this->GetVertex(0, s,r,c); };
    mxGraphVertex* GetVertex(mxPosition &p) { return this->GetVertex(0, p); };
    
    /// Get vertex that is the closest to the input position.
    mxGraphVertex* GetVertexClosestToPosition(unsigned int t, double s, double r, double c);
    mxGraphVertex* GetVertexClosestToPosition(unsigned int t, mxPosition &p);
    mxGraphVertex* GetVertexClosestToPosition(double s, double r, double c) { return this->GetVertexClosestToPosition(0, s,r,c); };
    mxGraphVertex* GetVertexClosestToPosition(mxPosition &p) { return this->GetVertexClosestToPosition(0, p); };
    
    /// Remove the edge from the graph.
    void RemoveEdge(unsigned int t, mxGraphEdge *edge);
    void RemoveEdge(mxGraphEdge *edge);
    
    /// Remove the vertex from the graph along with all its edges.
    void RemoveVertex(unsigned int t, mxGraphVertex *vertex);
    void RemoveVertex(mxGraphVertex *vertex);

    /// Reset the objectto state just after construction.
    void Reset();
    
    ///// Save to file.
    //void SaveToFile(const char *file_name);
    
    /// Set number of time instances.
    int SetNumberOfTimeSeries(int number_of_time_instances) { return m_graphs.SetNumberOfElements(number_of_time_instances); };
    
    /// Set origin.
    void SetOrigin(double s, double r, double c) { m_origin[2] = s; m_origin[1] = r; m_origin[0] = c; };
    
    /// Set spacing.
    void SetSpacing(double s, double r, double c) { m_spacing[2] = s; m_spacing[1] = r; m_spacing[0] = c; };
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif



#endif
