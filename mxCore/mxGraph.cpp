/*=========================================================================
 
 Program:   mipx
 Module:    mxGraph.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/


#include "mxGraph.h"



mxGraphVertex* mxGraphVertex::GetAdjacentVertex(mxGraphEdge *edge)
{
    if(!edge) return NULL;
    
    if(edge->GetVertex1()==this) return edge->GetVertex2();
    return edge->GetVertex1();
}


int mxGraphVertex::GetAdjacentVertices(mxArray<mxGraphVertex*> &adjacent_vertices)
{
    adjacent_vertices.SetNumberOfElements(this->GetNumberOfEdges());
    
    mxListIterator< mxGraphEdge* > ite;
    int i=0;
    for(ite.SetToBegin(m_edges), i=0; ite.IsValid() && i<adjacent_vertices.GetNumberOfElements(); ite.MoveToNext(), i++)
    {
        adjacent_vertices[i] = this->GetAdjacentVertex(ite.GetElement());
    }
    
    return 1;
}


int mxGraphVertex::IsAdjacentTo(mxGraphVertex *vertex)
{
    mxListIterator< mxGraphEdge* > it;
    for(it.SetToBegin(m_edges); it.IsValid(); it.MoveToNext())
    {
        mxGraphVertex *v = it.GetElement()->GetVertex1();
        if(v==this) v = it.GetElement()->GetVertex2();
        if(v==vertex) return 1;
    }
    return 0;
}


int mxGraphVertex::IsLinkVertex()
{
    if(this->GetNumberOfEdges()==2) return 1;
    return 0;
}


int mxGraphVertex::IsNodeVertex()
{
    return !(this->IsLinkVertex());
}


int mxGraphVertex::IsPortVertex()
{
    /// If this vertex is a stub (has only 1 incident edge), it is also a port vertex.
    if(this->IsStubVertex()) return 1;
    
    mxListIterator< mxGraphEdge* > it;
    int is_link_edge_found = 0;
    for(it.SetToBegin(m_edges); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->IsLinkEdge())
        {
            is_link_edge_found = 1;
            break;
        }
    }
    
    // if there is no link edge, return 0 (this vertex is not a port vertex)
    if(!is_link_edge_found) return 0;
    
    // if there was at least one link edge, check if it has at least one node edge and if so, return 1 (it is a port vertex).
    for(it.SetToBegin(m_edges); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->IsNodeEdge())
        {
            return 1;
        }
    }
    
    return 0;
}


int mxGraphVertex::IsStubVertex()
{
    if(this->GetNumberOfEdges()==1) return 1;
    return 0;
}


int mxGraphVertex::SendUpdates()
{
    if(!m_SP_is_updated) return 0;
    
    for(int i=0; i<m_edges.GetNumberOfElements(); i++)
    {
        mxGraphVertex *v = this->GetAdjacentVertex(m_edges[i]);
        
        // Adjacent vertices are udpated only if the new cost is smaller then the existing one (or if the cost is less than 0, which is a default non-valid number)
        if(v->GetCost() > this->GetCost() + m_edges[i]->GetWeight()  || v->GetCost()<0)
        {
            v->SetCost(this->GetCost()+m_edges[i]->GetWeight());
            v->m_SP_is_updated = 1;
        }
    }
    
    m_SP_is_updated = 0;
    
    return 1;
}



//----------------------------------------------------------------------------------



int mxGraphEdge::IsLinkEdge()
{
    if(m_vertex1->IsLinkVertex()) return 1;
    if(m_vertex2->IsLinkVertex()) return 1;
    return 0;
}


int mxGraphEdge::IsNodeEdge()
{
    return !(this->IsLinkEdge());
}


int mxGraphEdge::IsPortEdge()
{
    if(m_vertex1->IsPortVertex()) return 1;
    if(m_vertex2->IsPortVertex()) return 1;
    return 0;
}


int mxGraphEdge::IsStubEdge()
{
    if(m_vertex1->IsStubVertex()) return 1;
    if(m_vertex2->IsStubVertex()) return 1;
    return 0;
}



//----------------------------------------------------------------------------------



mxGraphVertex* mxGraph3D::AddVertex(double s, double r, double c)
{
    mxGraphVertex *v = m_vertices.AddNewToEnd();
    v->SetPosition(s,r,c);
    v->m_container = m_vertices.GetEndNodeAddress();
    return v;
}


mxGraphEdge* mxGraph3D::AddEdge(mxGraphVertex *vertex1, mxGraphVertex *vertex2)
{
    mxGraphEdge *e = m_edges.AddNewToEnd();
    e->m_vertex1 = vertex1;
    e->m_vertex2 = vertex2;
    e->m_container = m_edges.GetEndNodeAddress();
    vertex1->m_edges.AddToEnd(e);
    vertex2->m_edges.AddToEnd(e);
    return e;
}


int mxGraph3D::Backtrack(mxGraphVertex *start_vertex, mxGraphVertex *end_vertex, mxGraph3D *output_path)
{
    //    assert (m_is_converged);
    //
    
    output_path->Reset();
    output_path->AddVertex(start_vertex->GetPosition()->S(),start_vertex->GetPosition()->R(),start_vertex->GetPosition()->C());
    
    mxGraphVertex *current_vertex = start_vertex;
    int is_new_step_found = 1;
    
    while(current_vertex!=end_vertex && is_new_step_found)
    {
        is_new_step_found = 0;
        mxGraphVertex *vertex_with_lowest_cost = current_vertex;
        for(unsigned int i=0; i<current_vertex->GetNumberOfEdges(); i++)
        {
            mxGraphVertex *v = current_vertex->GetAdjacentVertex(current_vertex->m_edges[i]);
            if(v->GetCost() < vertex_with_lowest_cost->GetCost())
            {
                vertex_with_lowest_cost = v;
                is_new_step_found = 1;
            }
        }
        
        if(is_new_step_found)
        {
            output_path->AddVertex(vertex_with_lowest_cost->GetPosition()->S(),vertex_with_lowest_cost->GetPosition()->R(),vertex_with_lowest_cost->GetPosition()->C());
            output_path->AddEdge(current_vertex, vertex_with_lowest_cost);
            
            current_vertex = vertex_with_lowest_cost;
        }
    }
    
    return 1;
}


void mxGraph3D::ConvergeForShortestPath(mxGraphVertex *vertex)
{
    for(unsigned int i=0; i<this->GetNumberOfVertices(); i++)
    {
        m_vertices[i].SetCost(-1);
    }
    
    vertex->SetCost(0);
    vertex->m_SP_is_updated = 1;
    
    for(int is_converged = 0; !is_converged; )
    {
        is_converged = 1;
        for(unsigned int i=0; i<this->GetNumberOfVertices(); i++)
        {
            if(m_vertices[i].SendUpdates()) is_converged = 0;
        }
    }
}


int mxGraph3D::IsEmpty()
{
    if(m_vertices.GetNumberOfElements()<2) return 1;
    return 0;
}


mxGraphVertex* mxGraph3D::GetVertex(mxPosition &p)
{
    mxListIterator< mxGraphVertex > it;
    for(it.SetToBegin(m_vertices); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElementAddress()->GetPosition()->IsEqualPositionIn_3D(p))
        {
            return it.GetElementAddress();
        }
    }
    return NULL;
}


mxGraphVertex* mxGraph3D::GetVertex(double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return this->GetVertex(p);
}


mxGraphVertex* mxGraph3D::GetVertexClosestToPosition(mxPosition &p)
{
    mxListIterator< mxGraphVertex > it;
    mxGraphVertex *min_distance_vertex = NULL;
    double min_distance = -1;
    for(it.SetToBegin(m_vertices); it.IsValid(); it.MoveToNext())
    {
        double d = it.GetElementAddress()->GetPosition()->DistanceEuclidean(p);
        if(d<min_distance || min_distance<0)
        {
            min_distance = d;
            min_distance_vertex = it.GetElementAddress();
        }
    }
    return min_distance_vertex;
}


mxGraphVertex* mxGraph3D::GetVertexClosestToPosition(double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return this->GetVertexClosestToPosition(p);
}


void mxGraph3D::RemoveEdge(mxGraphEdge *edge)
{
    // Remove the edge from the lists_of_edges of its vertices.
    mxListIterator< mxGraphEdge* > it;
    for(it.SetToBegin(edge->GetVertex1()->GetEdges()); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()==edge)
        {
            edge->GetVertex1()->GetEdges()->DeleteNode(it.GetNode());
            break;
        }
    }
    for(it.SetToBegin(edge->GetVertex2()->GetEdges()); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()==edge)
        {
            edge->GetVertex2()->GetEdges()->DeleteNode(it.GetNode());
            break;
        }
    }
    
    // Remove the edge from the list of edges of the graph.
    m_edges.DeleteNode(edge->m_container);
}


void mxGraph3D::RemoveVertex(mxGraphVertex *vertex)
{
    // Remove all edges incident to this vertex.
    while(!vertex->GetEdges()->IsEmpty())
    {
        mxGraphEdge *edge = vertex->GetEdges()->GetEndElement();
        this->RemoveEdge(edge);
    }
    
    // Remove the vertex from the list of vertices of the graph.
    m_vertices.DeleteNode(vertex->m_container);
}



//----------------------------------------------------------------------------------



mxGraph::mxGraph()
{
    m_class_name.Assign("mxGraph");
    
    m_origin[0] = m_origin[1] = m_origin[2] = 0;
    m_spacing[0] = m_spacing[1] = m_spacing[2] = 1;
    
    // An empty graph must have one created 3D graph structure.
    m_graphs.SetNumberOfElements(1);
}


mxGraphVertex* mxGraph::AddVertex(unsigned int t, double s, double r, double c)
{
    return m_graphs[t].AddVertex(s,r,c);
}


mxGraphEdge* mxGraph::AddEdge(unsigned int t, mxGraphVertex *vertex1, mxGraphVertex *vertex2)
{
    return m_graphs[t].AddEdge(vertex1,vertex2);
}


int mxGraph::Backtrack(mxGraphVertex *start_vertex, mxGraphVertex *end_vertex, mxGraph *output_path, unsigned int t)
{
    return m_graphs[t].Backtrack(start_vertex,end_vertex,&(output_path->m_graphs[0]));
}


void mxGraph::ConvergeForShortestPath(mxGraphVertex *vertex, unsigned int t)
{
    return m_graphs[t].ConvergeForShortestPath(vertex);
}


int mxGraph::IsEmpty()
{
    if(m_graphs.IsEmpty()) return 1;
    for(unsigned int t=0; t<m_graphs.GetNumberOfElements(); t++)
    {
        if(!m_graphs[t].IsEmpty()) return 0;
    }
    return 1;
}


int mxGraph::GetLinkEdgeAndVertex(mxGraphVertex *output_vertex, mxGraphEdge *output_edge)
{
    mxGraphVertex *v = output_vertex;
    mxGraphEdge *e = output_edge;
    
    output_vertex = v->GetAdjacentVertex(e);
    if(output_vertex->GetNumberOfEdges() != 2)
    {
        output_edge = NULL;
        return 0;
    }
    else
    {
        output_edge = output_vertex->GetEdges()->GetBeginElement();
        if(output_edge == e)
        {
            output_edge = output_vertex->GetEdges()->GetEndElement();
        }
        return 1;
    }
}


mxGraphVertex* mxGraph::GetVertex(unsigned int t, mxPosition &p)
{
    return m_graphs[t].GetVertex(p);
}


mxGraphVertex* mxGraph::GetVertex(unsigned int t, double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return m_graphs[t].GetVertex(p);
}


mxGraphVertex* mxGraph::GetVertexClosestToPosition(unsigned int t, mxPosition &p)
{
    return m_graphs[t].GetVertexClosestToPosition(p);
}


mxGraphVertex* mxGraph::GetVertexClosestToPosition(unsigned int t, double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return m_graphs[t].GetVertexClosestToPosition(p);
}


void mxGraph::RemoveEdge(unsigned int t, mxGraphEdge *edge)
{
    m_graphs[t].RemoveEdge(edge);
}


void mxGraph::RemoveEdge(mxGraphEdge *edge)
{
    m_graphs[0].RemoveEdge(edge);
}


void mxGraph::RemoveVertex(unsigned int t, mxGraphVertex *vertex)
{
    m_graphs[t].RemoveVertex(vertex);
}


void mxGraph::RemoveVertex(mxGraphVertex *vertex)
{
    m_graphs[0].RemoveVertex(vertex);
}


void mxGraph::Reset()
{
    m_origin[0] = m_origin[1] = m_origin[2] = 0;
    m_spacing[0] = m_spacing[1] = m_spacing[2] = 1;
    
    // An empty graph must have one created 3D graph structure.
    m_graphs.SetNumberOfElements(1);
}
