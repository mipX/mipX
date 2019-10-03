/*=========================================================================
 
 Program:   mipx
 Module:    vmxGraph.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/


#include "vmxGraph.h"




vmxGraph3D::vmxGraph3D()
{
    m_graph = NULL;
    
    m_vtk_actor = vtkSmartPointer<vtkActor>::New();
	m_vtk_lines = vtkSmartPointer<vtkCellArray>::New();
    m_vtk_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_vtk_points = vtkSmartPointer<vtkPoints>::New();
    m_vtk_poly_data = vtkSmartPointer<vtkPolyData>::New();
    
    m_vtk_scalars = vtkSmartPointer<vtkFloatArray>::New();
}


vmxGraph3D::~vmxGraph3D()
{
	this->Reset();
}


void vmxGraph3D::Reset()
{
    m_vtk_lines->Initialize();
    m_vtk_points->Initialize();
    m_vtk_poly_data->Initialize();
}


int vmxGraph3D::IsVisible()
{
	return m_vtk_actor->GetVisibility();
}


void vmxGraph3D::SetVisibility(int is_visible)
{
	m_vtk_actor->SetVisibility(is_visible);
}



//---------------------------------------------------------------------------------------------------------------------------------------------



vmxGraph::vmxGraph()
{
    m_class_name.Assign("vmxGraph");
    
    m_vtk_graphs.SetNumberOfElements(1);
    m_vtk_graphs[0].m_graph = this;
    m_time_index = 0;
    m_vtk_interactor = NULL;
    m_vtk_renderer = NULL;
}


vmxGraph::~vmxGraph()
{
    this->Reset();
}


int vmxGraph::CurveUsingCardinalSpline(int number_of_output_points)//int interpolation_coefficient)
{
   // if(interpolation_coefficient<3 || interpolation_coefficient>10) interpolation_coefficient = 3;// we take 3 as minimum value, because links with only 1 position
    // will produce 2 positions. Empty links will remain empty (no need to interpolate straight lines).
    
    
    if(this->GetNumberOfVertices()<3) return 0;
    
    unsigned int t_index = 0;
    
    {
        
        //int number_of_output_points = this->GetNumberOfVertices() + this->GetNumberOfVertices() * interpolation_coefficient;
        
        //----- 1. Adjust positions -----
        vtkSmartPointer<vtkCardinalSpline> spline_x = vtkSmartPointer<vtkCardinalSpline>::New();
        vtkSmartPointer<vtkCardinalSpline> spline_y = vtkSmartPointer<vtkCardinalSpline>::New();
        vtkSmartPointer<vtkCardinalSpline> spline_z = vtkSmartPointer<vtkCardinalSpline>::New();
        
        mxListIterator<mxGraphVertex> it;
        int i=0;
        for(it.SetToBegin(this->GetGraph3D(t_index).GetVertices()), i=0; it.IsValid(); it.MoveToNext(), i++)
        {
            spline_x->AddPoint(i, it.GetElement().GetPosition()->C());
            spline_y->AddPoint(i, it.GetElement().GetPosition()->R());
            spline_z->AddPoint(i, it.GetElement().GetPosition()->S());
        }
            
        //Sampling parameter 't'
        double t;
        double dt = (spline_z->GetNumberOfPoints()-1.0)/((double)(number_of_output_points-1));
        
        //Interpolating
        mxList< mxPosition > new_position_list;
        new_position_list.AddToBegin(*(this->GetGraph3D(t_index).GetVertices()->GetBeginElementAddress()->GetPosition()));
        for(int j=1; j<number_of_output_points-1; j++)//we don't take positions with indexes 0 and number_of_output_points-1 into account (they are nodes).
        {
            t = dt*j;
            mxPosition *pp = new_position_list.AddNewToEnd();
            pp->C() = spline_x->Evaluate(t);
            pp->R() = spline_y->Evaluate(t);
            pp->S() = spline_z->Evaluate(t);
        }
        new_position_list.AddToEnd(*(this->GetGraph3D(t_index).GetVertices()->GetEndElementAddress()->GetPosition()));
        //-----------------------------
        
        
        this->GetGraph3D(t_index).Reset();
        
        mxListIterator< mxPosition > itpp;
        for(itpp.SetToBegin(new_position_list); itpp.IsValid(); itpp.MoveToNext())
        {
            this->GetGraph3D(t_index).AddVertex(itpp.GetElementAddress()->S(),itpp.GetElementAddress()->R(),itpp.GetElementAddress()->C());

            //!!!!!!!!!!!! MISSING HERE: ADDING EDGES BETWEEN VERTICES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        
//        //----- 2. Adjust scalars -----
//        //Do for each of the scalar values...
//        int noel_new = new_position_list.GetNumberOfElements();
//        int noel_old = it.GetElementPointer()->GetPositionsList()->GetNumberOfElements();
//        int ratio = noel_new / noel_old + 1;
//        for(unsigned int scalar_index=0; scalar_index<this->GetNumberOfScalars(); scalar_index++)
//        {
//            for(int q=0; q<noel_new; q++)
//            {
//                new_position_list[q].V(scalar_index) = (*(it.GetElementPointer()->GetPositionsList()))[q/ratio].V(scalar_index);
//            }
//        }
    
    }
    this->Update();
    
    return 1;
}



void vmxGraph::ExportMeshToSTL(const char *file_name)
{
    int t=0;
    
    vmxGraph3D *g = &(m_vtk_graphs[t]);
    
    vtkSmartPointer<vtkSTLWriter> w = vtkSmartPointer<vtkSTLWriter>::New();
    w->SetFileName(file_name);
    w->SetInputData(g->m_vtk_poly_data);
    w->Write();

    
    // Save STL
//    vtkSmartPointer< vtkPolyDataNormals > n = vtkSmartPointer<vtkPolyDataNormals>::New();
//    //n->SetInputConnection(g-> ->m_tube_filter->GetOutputPort());
//    n->SetInputData(g->m_vtk_poly_data);
//    n->SplittingOff();
//    n->ConsistencyOn();
//    n->Update();
//    vtkSmartPointer<vtkSTLWriter> w = vtkSmartPointer<vtkSTLWriter>::New();
//    w->SetFileName(file_name);
//    w->SetInputConnection(n->GetOutputPort());
//    w->Write();
}


void vmxGraph::ExportMeshToVTK(const char *file_name)
{
    int t=0;
    
    vmxGraph3D *g = &(m_vtk_graphs[t]);
    
    vtkSmartPointer<vtkGenericDataObjectWriter> w = vtkSmartPointer<vtkGenericDataObjectWriter>::New();
    w->SetFileName(file_name);
    w->SetInputData(g->m_vtk_poly_data);
    w->Write();
}


void vmxGraph::Reset()
{
    mxGraph::Reset();

    for(unsigned int i=0; i<m_vtk_graphs.GetNumberOfElements(); i++)
    {
        m_vtk_graphs[i].Reset();
    }
    
    m_vtk_graphs.SetNumberOfElements(1);
    m_vtk_graphs[0].m_graph = this;
    m_time_index = 0;
    
    if(m_vtk_interactor)
    {
        //m_vtk_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(m_picked_link_outline_actor);
    }
}


vmxGraph3D& vmxGraph::operator()(unsigned int t)
{
    return m_vtk_graphs[t];
}



vtkActor* vmxGraph::GetActor(unsigned int t)
{
    return m_vtk_graphs[t].m_vtk_actor;
}


int vmxGraph::HasActor(vtkActor *actor)
{
    if(!actor) return 0;
    for(unsigned int t=0; t<m_vtk_graphs.GetNumberOfElements(); t++)
    {
        if(this->GetActor(t)==actor)
        {
            return 1;
        }
    }
    return 0;
}


int vmxGraph::IsVisible()
{
    return m_is_visible;
}


void vmxGraph::SetRenderer(vtkRenderer *renderer)
{
    m_vtk_renderer = renderer;
    this->SetTimeIndexWithoutRendering(0);
}


void vmxGraph::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;

    //!!!!!!! SHOULD CHECK IF IT IS BETTER TO ADD ALL ACTORS TO INTERACTOR AND JUST CHANGE THEIR VISIBILITY OR
    //!!!!!!!! TO ADD (AND REMOVE) ONLY A SINGLE ACTOR EACH TIME A TIME INDEX IS CHANGED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    this->m_vtk_interactor = interactor;
    this->SetTimeIndex(0);
}


void vmxGraph::SetTimeIndex(unsigned int t)
{
    this->SetTimeIndexWithoutRendering(t);
    m_vtk_interactor->Render();
}


void vmxGraph::SetTimeIndexWithoutRendering(unsigned int t)
{
    if(t > m_vtk_graphs.GetNumberOfElements()-1) return;
    
    if(m_vtk_renderer)
    {
        m_vtk_renderer->RemoveActor(m_vtk_graphs[m_time_index].m_vtk_actor);
        m_time_index = t;
        m_vtk_graphs[m_time_index].m_vtk_actor->SetVisibility(m_is_visible);
        m_vtk_renderer->AddActor(m_vtk_graphs[m_time_index].m_vtk_actor);
    }
}



void vmxGraph::SetVisibility(int is_visible)
{
    m_is_visible = is_visible;
    m_vtk_graphs[m_time_index].SetVisibility(is_visible);
}


void vmxGraph::SetVisibilityOfComponent(int component_index, int is_visible)
{
    m_is_visible = is_visible;
    m_vtk_graphs[m_time_index].SetVisibility(is_visible);
}


//void vmxGraph::Update(unsigned int t)
//{
//    vmxGraph3D *graph = &(m_vtk_graphs[t]);
//
//    graph->m_vtk_lines->Initialize();
//    graph->m_vtk_points->Initialize();
//    graph->m_vtk_poly_data->Initialize();
//
//    //for(unsigned int i=0; i<skl3D->m_scalars_vtk.GetNumberOfElements(); i++) skl3D->m_scalars_vtk[i]->Initialize();
//
//    // Counts the number of added points. We use this when adding a new point to m_vtk_points.
//    int counter_for_number_of_added_points = 0;
//
//    mxListIterator< mxGraphEdge > ite;
//    for(ite.SetToBegin(this->GetEdges()); ite.IsValid(); ite.MoveToNext())
//    {
//        mxGraphEdge *edge = ite.GetElementAddress();
//
//        mxGraphVertex *v1 = edge->GetVertex1();
//        mxGraphVertex *v2 = edge->GetVertex2();
//
//        double world_CRS_1[3], world_CRS_2[3];
//        world_CRS_1[0] = this->GetOrigin_C()+ v1->GetPosition()->C() * this->GetSpacing_C();
//        world_CRS_1[1] = this->GetOrigin_R()+ v1->GetPosition()->R() * this->GetSpacing_R();
//        world_CRS_1[2] = this->GetOrigin_S()+ v1->GetPosition()->S() * this->GetSpacing_S();
//        world_CRS_2[0] = this->GetOrigin_C()+ v2->GetPosition()->C() * this->GetSpacing_C();
//        world_CRS_2[1] = this->GetOrigin_R()+ v2->GetPosition()->R() * this->GetSpacing_R();
//        world_CRS_2[2] = this->GetOrigin_S()+ v2->GetPosition()->S() * this->GetSpacing_S();
//
//        //graph->m_vtk_points->InsertPoint(counter_for_number_of_added_points, world_CRS_1[0], world_CRS_1[1], world_CRS_1[2]);
//        graph->m_vtk_points->InsertPoint(graph->m_vtk_points->GetNumberOfPoints(), world_CRS_1[0], world_CRS_1[1], world_CRS_1[2]);
//
////        for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
////        {
////            skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points,&(link_start_node_position.V(j)));
////        }
////
////        int n = psl->m_link_positions.GetNumberOfElements();
////        for(int i=1; i<n+1; i++)
////        {
////            //!!!!!!!!!! HERE INSTEAD OF INDEXING [] USE LIST ITERATOR - WILL BE MORE EFFICIENT !!!!!!!!!!
////            pos = psl->m_link_positions[i-1];
////            pos.S() = this->GetOrigin_S() + pos.S() * this->GetSpacing_S();
////            pos.R() = this->GetOrigin_R() + pos.R() * this->GetSpacing_R();
////            pos.C() = this->GetOrigin_C() + pos.C() * this->GetSpacing_C();
////
////            skl3D->m_points->InsertPoint(previous_number_of_points+i, pos.C(), pos.R(), pos.S());
////
////            for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
////            {
////                skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points+i,&(psl->m_link_positions[i-1].V(j)));
////            }
////        }
//
//        //graph->m_vtk_points->InsertPoint(counter_for_number_of_added_points+1, world_CRS_2[0], world_CRS_2[1], world_CRS_2[2]);
//        graph->m_vtk_points->InsertPoint(graph->m_vtk_points->GetNumberOfPoints(), world_CRS_2[0], world_CRS_2[1], world_CRS_2[2]);
//
//
////        for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
////        {
////            skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points+n+1,&(link_end_node_position.V(j)));
////        }
//
//        //The number of points that will be used for the cell is 'new_number_of_points - previous_number_of_points'.
//
//        graph->m_vtk_lines->InsertNextCell(2);
//        graph->m_vtk_lines->InsertCellPoint(graph->m_vtk_points->GetNumberOfPoints()-2);
//        graph->m_vtk_lines->InsertCellPoint(graph->m_vtk_points->GetNumberOfPoints()-1);
//
////
////        skl3D->m_lines->InsertNextCell(skl3D->m_points->GetNumberOfPoints() - previous_number_of_points);
////        for(int i=previous_number_of_points; i<skl3D->m_points->GetNumberOfPoints(); i++)
////        {
////            skl3D->m_lines->InsertCellPoint(i);
////        }
//
//        //Save the new 'previous_number_of_points' value.
//        counter_for_number_of_added_points = graph->m_vtk_points->GetNumberOfPoints();
//    }
//
//    graph->m_vtk_poly_data->SetPoints(graph->m_vtk_points);
//    graph->m_vtk_poly_data->SetLines(graph->m_vtk_lines);
//
////    //Add scalars with index 0 as current scalars
////    skl3D->m_poly_data->GetPointData()->SetScalars(skl3D->m_scalars_vtk[0]);
//
//
////    //Remove duplicates to avoid the vtkTubeFilter "can't create normals" error
////    skl3D->m_clean_poly_data->SetInputData(skl3D->m_poly_data);
////    //m_clean_poly_data->Update( );
//
//
//    graph->m_vtk_mapper->SetInputData(graph->m_vtk_poly_data);
////    double scalar_min_value, scalar_max_value;
////    this->GetScalarMinAndMaxValue(0,scalar_min_value,scalar_max_value);
////    if(scalar_min_value>=scalar_max_value){ scalar_max_value = scalar_min_value + 1; }
////    skl3D->m_mapper->SetScalarRange(scalar_min_value, scalar_max_value);//for scalars
//
//    // Set the color of the lines to vary with scalars.
//    graph->m_vtk_mapper->ScalarVisibilityOff();//graph->m_vtk_mapper->ScalarVisibilityOn();
//
//    graph->m_vtk_actor->SetMapper(graph->m_vtk_mapper);
//}



void vmxGraph::Update(unsigned int t)
{
    vmxGraph3D *graph = &(m_vtk_graphs[t]);
    
    graph->m_vtk_lines->Initialize();
    graph->m_vtk_points->Initialize();
    graph->m_vtk_poly_data->Initialize();
    
    //for(unsigned int i=0; i<skl3D->m_scalars_vtk.GetNumberOfElements(); i++) skl3D->m_scalars_vtk[i]->Initialize();
    
    // Counts the number of added points. We use this when adding a new point to m_vtk_points.
    int counter_for_number_of_added_points = 0;
    
    double scalar_min_value = 999999, scalar_max_value = 0;
    
    mxListIterator< mxGraphEdge > ite;
    for(ite.SetToBegin(this->GetEdges()); ite.IsValid(); ite.MoveToNext())
    {
        mxGraphEdge *edge = ite.GetElementAddress();
        
        mxGraphVertex *v1 = edge->GetVertex1();
        mxGraphVertex *v2 = edge->GetVertex2();
        
        double world_CRS_1[3], world_CRS_2[3];
        world_CRS_1[0] = this->GetOrigin_C()+ v1->GetPosition()->C() * this->GetSpacing_C();
        world_CRS_1[1] = this->GetOrigin_R()+ v1->GetPosition()->R() * this->GetSpacing_R();
        world_CRS_1[2] = this->GetOrigin_S()+ v1->GetPosition()->S() * this->GetSpacing_S();
        world_CRS_2[0] = this->GetOrigin_C()+ v2->GetPosition()->C() * this->GetSpacing_C();
        world_CRS_2[1] = this->GetOrigin_R()+ v2->GetPosition()->R() * this->GetSpacing_R();
        world_CRS_2[2] = this->GetOrigin_S()+ v2->GetPosition()->S() * this->GetSpacing_S();
        
        //graph->m_vtk_points->InsertPoint(counter_for_number_of_added_points, world_CRS_1[0], world_CRS_1[1], world_CRS_1[2]);
        graph->m_vtk_points->InsertPoint(graph->m_vtk_points->GetNumberOfPoints(), world_CRS_1[0], world_CRS_1[1], world_CRS_1[2]);
        
        graph->m_vtk_scalars->InsertTuple(graph->m_vtk_points->GetNumberOfPoints(),&(world_CRS_1[2]));
        
        if(scalar_min_value>world_CRS_1[2]) scalar_min_value = world_CRS_1[2];
        if(scalar_max_value<world_CRS_1[2]) scalar_max_value = world_CRS_1[2];
        
        //        for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
        //        {
        //            skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points,&(link_start_node_position.V(j)));
        //        }
        //
        //        int n = psl->m_link_positions.GetNumberOfElements();
        //        for(int i=1; i<n+1; i++)
        //        {
        //            //!!!!!!!!!! HERE INSTEAD OF INDEXING [] USE LIST ITERATOR - WILL BE MORE EFFICIENT !!!!!!!!!!
        //            pos = psl->m_link_positions[i-1];
        //            pos.S() = this->GetOrigin_S() + pos.S() * this->GetSpacing_S();
        //            pos.R() = this->GetOrigin_R() + pos.R() * this->GetSpacing_R();
        //            pos.C() = this->GetOrigin_C() + pos.C() * this->GetSpacing_C();
        //
        //            skl3D->m_points->InsertPoint(previous_number_of_points+i, pos.C(), pos.R(), pos.S());
        //
        //            for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
        //            {
        //                skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points+i,&(psl->m_link_positions[i-1].V(j)));
        //            }
        //        }
        
        //graph->m_vtk_points->InsertPoint(counter_for_number_of_added_points+1, world_CRS_2[0], world_CRS_2[1], world_CRS_2[2]);
        graph->m_vtk_points->InsertPoint(graph->m_vtk_points->GetNumberOfPoints(), world_CRS_2[0], world_CRS_2[1], world_CRS_2[2]);
        
        graph->m_vtk_scalars->InsertTuple(graph->m_vtk_points->GetNumberOfPoints(),&(world_CRS_2[2]));
        if(scalar_min_value>world_CRS_1[2]) scalar_min_value = world_CRS_1[2];
        if(scalar_max_value<world_CRS_1[2]) scalar_max_value = world_CRS_1[2];
        
        
        //        for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
        //        {
        //            skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points+n+1,&(link_end_node_position.V(j)));
        //        }
        
        //The number of points that will be used for the cell is 'new_number_of_points - previous_number_of_points'.
        
        graph->m_vtk_lines->InsertNextCell(2);
        graph->m_vtk_lines->InsertCellPoint(graph->m_vtk_points->GetNumberOfPoints()-2);
        graph->m_vtk_lines->InsertCellPoint(graph->m_vtk_points->GetNumberOfPoints()-1);
        
        //
        //        skl3D->m_lines->InsertNextCell(skl3D->m_points->GetNumberOfPoints() - previous_number_of_points);
        //        for(int i=previous_number_of_points; i<skl3D->m_points->GetNumberOfPoints(); i++)
        //        {
        //            skl3D->m_lines->InsertCellPoint(i);
        //        }
        
        //Save the new 'previous_number_of_points' value.
        counter_for_number_of_added_points = graph->m_vtk_points->GetNumberOfPoints();
    }
    
    graph->m_vtk_poly_data->SetPoints(graph->m_vtk_points);
    graph->m_vtk_poly_data->SetLines(graph->m_vtk_lines);
    
    //Add scalars with index 0 as current scalars
    graph->m_vtk_poly_data->GetPointData()->SetScalars(graph->m_vtk_scalars);
    
    
    //    //Remove duplicates to avoid the vtkTubeFilter "can't create normals" error
    //    skl3D->m_clean_poly_data->SetInputData(skl3D->m_poly_data);
    //    //m_clean_poly_data->Update( );
    
    
    graph->m_vtk_mapper->SetInputData(graph->m_vtk_poly_data);
    //    double scalar_min_value, scalar_max_value;
    //    this->GetScalarMinAndMaxValue(0,scalar_min_value,scalar_max_value);
    
    if(scalar_min_value>=scalar_max_value){ scalar_max_value = scalar_min_value + 1; }
    graph->m_vtk_mapper->SetScalarRange(scalar_min_value, scalar_max_value);//for scalars
    
    // Set the color of the lines to vary with scalars.
    graph->m_vtk_mapper->ScalarVisibilityOn(); //graph->m_vtk_mapper->ScalarVisibilityOff();
    
    graph->m_vtk_actor->SetMapper(graph->m_vtk_mapper);
}



void vmxGraph::Update()
{
    if(m_graphs.GetNumberOfElements() != m_vtk_graphs.GetNumberOfElements())
    {
        for(unsigned int t=0; t<m_vtk_graphs.GetNumberOfElements(); t++)
        {
            m_vtk_graphs[t].Reset();
        }
        m_vtk_graphs.SetNumberOfElements(m_graphs.GetNumberOfElements());
        for(unsigned int t=0; t<m_vtk_graphs.GetNumberOfElements(); t++)
        {
            m_vtk_graphs[t].m_graph = this;// SetUpObject(this);
        }
    }
    for(unsigned int t=0; t<m_vtk_graphs.GetNumberOfElements(); t++)
    {
        this->Update(t);
    }
}
