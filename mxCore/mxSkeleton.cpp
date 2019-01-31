/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeleton.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxSkeleton.h"



mxSkeletonNode* mxSkeletonNode::GetAdjacentNode(mxSkeletonLink *link)
{
    if(!link) return NULL;
    
    if(link->GetNode1()==this) return link->GetNode2();
    return link->GetNode1();
}


int mxSkeletonNode::IsAdjacentTo(mxSkeletonNode *node)
{
    mxListIterator< mxSkeletonLink* > it;
    for(it.SetToBegin(m_links); it.IsValid(); it.MoveToNext())
    {
        mxSkeletonNode *n = it.GetElement()->GetNode1();
        if(n==this) n = it.GetElement()->GetNode2();
        if(n==node) return 1;
    }
    return 0;
}


//int mxSkeletonVertex::SendUpdates()
//{
//    if(!m_SP_is_updated) return 0;
//
//    for(int i=0; i<m_edges.GetNumberOfElements(); i++)
//    {
//        mxSkeletonVertex *v = this->GetAdjacentVertex(m_edges[i]);
//
//        // Adjacent vertices are udpated only if the new cost is smaller then the existing one (or if the cost is less than 0, which is a default non-valid number)
//        if(v->GetCost() > this->GetCost() + m_edges[i]->GetWeight()  || v->GetCost()<0)
//        {
//            v->SetCost(this->GetCost()+m_edges[i]->GetWeight());
//            v->m_SP_is_updated = 1;
//        }
//    }
//
//    m_SP_is_updated = 0;
//
//    return 1;
//}



//----------------------------------------------------------------------------------



mxSkeletonNode* mxSkeleton3D::AddNode(double s, double r, double c)
{
    mxSkeletonNode *v = m_nodes.AddNewToEnd();
    v->SetPosition(s,r,c);
    v->m_container = m_nodes.GetEndNodeAddress();
    return v;
}


mxSkeletonLink* mxSkeleton3D::AddLink(mxSkeletonNode *node1, mxSkeletonNode *node2)
{
    mxSkeletonLink *l = m_links.AddNewToEnd();
    l->m_node1 = node1;
    l->m_node2 = node2;
    l->m_container = m_links.GetEndNodeAddress();
    node1->m_links.AddToEnd(l);
    node2->m_links.AddToEnd(l);
    return l;
}


//int mxSkeleton3D::Backtrack(mxSkeletonVertex *start_vertex, mxSkeletonVertex *end_vertex, mxSkeleton3D *output_path)
//{
//    //    assert (m_is_converged);
//    //
//
//    output_path->Reset();
//    output_path->AddVertex(start_vertex->GetPosition()->S(),start_vertex->GetPosition()->R(),start_vertex->GetPosition()->C());
//
//    mxSkeletonVertex *current_vertex = start_vertex;
//    int is_new_step_found = 1;
//
//    while(current_vertex!=end_vertex && is_new_step_found)
//    {
//        is_new_step_found = 0;
//        mxSkeletonVertex *vertex_with_lowest_cost = current_vertex;
//        for(unsigned int i=0; i<current_vertex->GetNumberOfEdges(); i++)
//        {
//            mxSkeletonVertex *v = current_vertex->GetAdjacentVertex(current_vertex->m_edges[i]);
//            if(v->GetCost() < vertex_with_lowest_cost->GetCost())
//            {
//                vertex_with_lowest_cost = v;
//                is_new_step_found = 1;
//            }
//        }
//
//        if(is_new_step_found)
//        {
//            output_path->AddVertex(vertex_with_lowest_cost->GetPosition()->S(),vertex_with_lowest_cost->GetPosition()->R(),vertex_with_lowest_cost->GetPosition()->C());
//            output_path->AddEdge(current_vertex, vertex_with_lowest_cost);
//
//            current_vertex = vertex_with_lowest_cost;
//        }
//    }
//
//    return 1;
//}


//void mxSkeleton3D::ConvergeForShortestPath(mxSkeletonVertex *vertex)
//{
//    for(unsigned int i=0; i<this->GetNumberOfVertices(); i++)
//    {
//        m_vertices[i].SetCost(-1);
//    }
//
//    vertex->SetCost(0);
//    vertex->m_SP_is_updated = 1;
//
//    for(int is_converged = 0; !is_converged; )
//    {
//        is_converged = 1;
//        for(unsigned int i=0; i<this->GetNumberOfVertices(); i++)
//        {
//            if(m_vertices[i].SendUpdates()) is_converged = 0;
//        }
//    }
//}


int mxSkeleton3D::IsEmpty()
{
    if(m_links.GetNumberOfElements()<2) return 1;
    return 0;
}


mxSkeletonNode* mxSkeleton3D::GetNode(mxPosition &p)
{
    mxListIterator< mxSkeletonNode > it;
    for(it.SetToBegin(m_nodes); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElementAddress()->GetPosition()->IsEqualPositionIn_3D(p))
        {
            return it.GetElementAddress();
        }
    }
    return NULL;
}


mxSkeletonNode* mxSkeleton3D::GetNode(double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return this->GetNode(p);
}


mxSkeletonNode* mxSkeleton3D::GetNodeClosestToPosition(mxPosition &p)
{
    mxListIterator< mxSkeletonNode > it;
    mxSkeletonNode *min_distance_node = NULL;
    double min_distance = -1;
    for(it.SetToBegin(m_nodes); it.IsValid(); it.MoveToNext())
    {
        double d = it.GetElementAddress()->GetPosition()->DistanceEuclidean(p);
        if(d<min_distance || min_distance<0)
        {
            min_distance = d;
            min_distance_node = it.GetElementAddress();
        }
    }
    return min_distance_node;
}


mxSkeletonNode* mxSkeleton3D::GetNodeClosestToPosition(double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return this->GetNodeClosestToPosition(p);
}


void mxSkeleton3D::RemoveLink(mxSkeletonLink *link)
{
    // Remove the edge from the lists_of_edges of its vertices.
    mxListIterator< mxSkeletonLink* > it;
    for(it.SetToBegin(link->GetNode1()->GetLinks()); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()==link)
        {
            link->GetNode1()->GetLinks()->DeleteNode(it.GetNode());
            break;
        }
    }
    for(it.SetToBegin(link->GetNode2()->GetLinks()); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()==link)
        {
            link->GetNode2()->GetLinks()->DeleteNode(it.GetNode());
            break;
        }
    }
    
    // Remove the link from the list of links of the Skeleton.
    m_links.DeleteNode(link->m_container);
}


void mxSkeleton3D::RemoveNode(mxSkeletonNode *node)
{
    // Remove all links incident to this node.
    while(!node->GetLinks()->IsEmpty())
    {
        mxSkeletonLink *link = node->GetLinks()->GetEndElement();
        this->RemoveLink(link);
    }
    
    // Remove the node from the list of nodes of the Skeleton.
    m_nodes.DeleteNode(node->m_container);
}



//----------------------------------------------------------------------------------



mxSkeleton::mxSkeleton()
{
    m_class_name.Assign("mxSkeleton");
    
//    m_origin[0] = m_origin[1] = m_origin[2] = 0;
//    m_spacing[0] = m_spacing[1] = m_spacing[2] = 1;
    
    // An empty skeleton must have one created 3D Skeleton structure.
    m_skeletons.SetNumberOfElements(1);
}


mxSkeletonNode* mxSkeleton::AddNode(unsigned int t, double s, double r, double c)
{
    return m_skeletons[t].AddNode(s,r,c);
}


mxSkeletonLink* mxSkeleton::AddLink(unsigned int t, mxSkeletonNode *node1, mxSkeletonNode *node2)
{
    return m_skeletons[t].AddLink(node1,node2);
}


int mxSkeleton::BuildSkeleton()
{
    if(this->mxGraph::IsEmpty()) return 0;
    
    unsigned int t=0;
    
    /// Create an array of vertices for faster access.
    mxArray< mxGraphVertex* > array_of_vertices;
    {
        array_of_vertices.SetNumberOfElements(m_graphs[t].GetNumberOfVertices());
        mxListIterator< mxGraphVertex > itv;
        int i=0;
        for(itv.SetToBegin(m_graphs[t].GetVertices()), i=0; itv.IsValid() && i<array_of_vertices.GetNumberOfElements(); itv.MoveToNext(), i++)
        {
            array_of_vertices[i] = itv.GetElementAddress();
            array_of_vertices[i]->m_ID = i;// set the id to correspond to its index in the array - this will allow fast mapping.
        }
    }
    
    /// Create an array of edges for faster access.
    mxArray< mxGraphEdge* > array_of_edges;
    {
        array_of_edges.SetNumberOfElements(m_graphs[t].GetNumberOfEdges());
        mxListIterator< mxGraphEdge > ite;
        int i=0;
        for(ite.SetToBegin(m_graphs[t].GetEdges()), i=0; ite.IsValid() && i<array_of_edges.GetNumberOfElements(); ite.MoveToNext(), i++)
        {
            array_of_edges[i] = ite.GetElementAddress();
        }
    }
    
    
    
    mxArray< int > vertex_indicators;
    vertex_indicators.SetNumberOfElements(array_of_vertices.GetNumberOfElements());
    vertex_indicators.FillInWith(0);
    
    //--- Create skeleton nodes ---
    {
//        mxArray< int > vertex_indicators;
//        vertex_indicators.SetNumberOfElements(array_of_vertices.GetNumberOfElements());
//        vertex_indicators.FillInWith(0);
//
        for(unsigned int i=0; i<array_of_vertices.GetNumberOfElements(); i++)
        {
            //if the current vertex is a node vertex and if it has not been processed yet...
            if(array_of_vertices[i]->IsNodeVertex() && vertex_indicators[i]==0)
            {
                //... create a new node, add the vertex to it...
                mxSkeletonNode *node = this->AddNode(array_of_vertices[i]->GetPosition()->S(), array_of_vertices[i]->GetPosition()->R(), array_of_vertices[i]->GetPosition()->C());
                node->m_graph_vertices.AddToEnd(array_of_vertices[i]);
//Maybe: set here the pointer in the vertex to the skeleton node!!!
                
                mxList< mxGraphVertex* > l;
                l.AddToEnd(array_of_vertices[i]);
                vertex_indicators[i] = 1;
                
                //...and all other node vertices that are adjacently connected.
                while(!l.IsEmpty())
                {
                    mxGraphVertex *v = l.GetBeginElement();
                    
                    mxArray<mxGraphVertex *> adjacent_vertices;
                    v->GetAdjacentVertices(adjacent_vertices);
                    
                    for(unsigned int j=0; j<adjacent_vertices.GetNumberOfElements(); j++)
                    {
                        if(adjacent_vertices[j]->IsNodeVertex() && vertex_indicators[adjacent_vertices[j]->m_ID]==0)
                        {
                            node->m_graph_vertices.AddToEnd(adjacent_vertices[j]);
                            
//Maybe: set here the pointer in the vertex to the skeleton node!!!
                            vertex_indicators[adjacent_vertices[j]->m_ID] = 1;
                            l.AddToEnd(adjacent_vertices[j]);
                        }
                    }
                    
                    l.DeleteBegin();
                }
                
                // Maybe: adjust the position of the node!
            }
        }
    }
    //------------------------------

    
    //--- Create skeleton links ---
    {
        mxArray< int > indicators;
        indicators.SetNumberOfElements(array_of_vertices.GetNumberOfElements());
        indicators.FillInWith(0);

        for(unsigned int i=0; i<array_of_edges.GetNumberOfElements(); i++)
        {
            //if the current vertex is a node vertex and if it has not been processed yet...
            if(array_of_edges[i]->IsPortEdge() && indicators[i]==0)
            {
                //--- determine the fist port vertex ---
                mxGraphVertex *port1 = array_of_edges[i]->GetVertex1();
                if(!port1->IsPortVertex()) port1 = array_of_edges[i]->GetVertex2();
                //---
                
                //--- determine the second port vertex ---
                mxGraphVertex *port2;
                {
                    mxGraphEdge *e;
                    mxGraphVertex *v;
                    for(e=array_of_edges[i], v=port1; v!=NULL && e!=NULL; this->GetLinkEdgeAndVertex(v, e))
                    {
                            
                    }
                }
                
//                mxGraphVertex *port2 = array_of_edges[i]->GetVertex1();
//                if(!port1->IsPortVertex()) port1 = array_of_edges[i]->GetVertex2();
//                //---
//
//                
//                
//                //... create a new link, add the vertex to it...
//                mxSkeletonLink *link = this->AddLink(array_of_vertices[i]->GetPosition()->S(), array_of_vertices[i]->GetPosition()->R(), array_of_vertices[i]->GetPosition()->C());
//                node->m_graph_vertices.AddToEnd(array_of_vertices[i]);
//                //Maybe: set here the pointer in the vertex to the skeleton node!!!
//                
//                mxList< mxGraphVertex* > l;
//                l.AddToEnd(array_of_vertices[i]);
//                vertex_indicators[i] = 1;
//                
//                //...and all other node vertices that are adjacently connected.
//                while(!l.IsEmpty())
//                {
//                    mxGraphVertex *v = l.GetBeginElement();
//                    
//                    mxArray<mxGraphVertex *> adjacent_vertices;
//                    v->GetAdjacentVertices(adjacent_vertices);
//                    
//                    for(unsigned int j=0; j<adjacent_vertices.GetNumberOfElements(); j++)
//                    {
//                        if(adjacent_vertices[j]->IsNodeVertex() && vertex_indicators[adjacent_vertices[j]->m_ID]==0)
//                        {
//                            node->m_graph_vertices.AddToEnd(adjacent_vertices[j]);
//                            //Maybe: set here the pointer in the vertex to the skeleton node!!!
//                            vertex_indicators[adjacent_vertices[j]->m_ID] = 1;
//                            l.AddToEnd(adjacent_vertices[j]);
//                        }
//                    }
//                    
//                    l.DeleteBegin();
//                }
            }
        }
    }
    //------------------------------

    
    return 1;
}


//int mxSkeleton::Backtrack(mxSkeletonVertex *start_vertex, mxSkeletonVertex *end_vertex, mxSkeleton *output_path, unsigned int t)
//{
//    return m_Skeletons[t].Backtrack(start_vertex,end_vertex,&(output_path->m_Skeletons[0]));
//}
//
//
//void mxSkeleton::ConvergeForShortestPath(mxSkeletonVertex *vertex, unsigned int t)
//{
//    return m_Skeletons[t].ConvergeForShortestPath(vertex);
//}


//int mxSkeleton::IsEmpty()
//{
//    if(m_skeletons.IsEmpty()) return 1;
//    for(unsigned int t=0; t<m_skeletons.GetNumberOfElements(); t++)
//    {
//        if(!m_skeletons[t].IsEmpty()) return 0;
//    }
//    return 1;
//}


mxSkeletonNode* mxSkeleton::GetNode(unsigned int t, mxPosition &p)
{
    return m_skeletons[t].GetNode(p);
}


mxSkeletonNode* mxSkeleton::GetNode(unsigned int t, double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return m_skeletons[t].GetNode(p);
}


mxSkeletonNode* mxSkeleton::GetNodeClosestToPosition(unsigned int t, mxPosition &p)
{
    return m_skeletons[t].GetNodeClosestToPosition(p);
}


mxSkeletonNode* mxSkeleton::GetNodeClosestToPosition(unsigned int t, double s, double r, double c)
{
    mxPosition p;
    p.SetPosition(s,r,c);
    return m_skeletons[t].GetNodeClosestToPosition(p);
}


void mxSkeleton::RemoveLink(unsigned int t, mxSkeletonLink *link)
{
    m_skeletons[t].RemoveLink(link);
}


void mxSkeleton::RemoveLink(mxSkeletonLink *link)
{
    m_skeletons[0].RemoveLink(link);
}


void mxSkeleton::RemoveNode(unsigned int t, mxSkeletonNode *node)
{
    m_skeletons[t].RemoveNode(node);
}


void mxSkeleton::RemoveNode(mxSkeletonNode *node)
{
    m_skeletons[0].RemoveNode(node);
}


void mxSkeleton::Reset()
{
    this->mxGraph::Reset();
//    m_origin[0] = m_origin[1] = m_origin[2] = 0;
//    m_spacing[0] = m_spacing[1] = m_spacing[2] = 1;
    
    // An empty skeleton must have one created 3D Skeleton structure.
    m_skeletons.SetNumberOfElements(1);
}


int mxSkeleton::SetNumberOfTimeSeries(int number_of_time_instances)
{
    if(!this->mxGraph::SetNumberOfTimeSeries(number_of_time_instances)) return 0;
    return m_skeletons.SetNumberOfElements(number_of_time_instances);
}
