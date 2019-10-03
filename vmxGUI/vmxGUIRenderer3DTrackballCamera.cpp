/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRenderer3DTrackballCamera.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxGUIRenderer3DTrackballCamera.h"




vmxGUIInteractorStyle3DTrackballCamera::vmxGUIInteractorStyle3DTrackballCamera()
{
    m_gui_renderer = NULL;
}


vmxGUIInteractorStyle3DTrackballCamera::~vmxGUIInteractorStyle3DTrackballCamera()
{
}


vtkStandardNewMacro(vmxGUIInteractorStyle3DTrackballCamera);


void vmxGUIInteractorStyle3DTrackballCamera::OnLeftButtonDown()
{
    //std::cout<<std::endl<<"vmxGUIInteractorStyle3DTrackballCamera::OnLeftButtonDown()";
    
    vtkRenderWindowInteractor *rwi = this->Interactor;
    m_double_click_detector.OnLeftButtonDown(rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], 0);
    
    // if double click is detected...
    if(m_double_click_detector.IsDoubleClicked())
    {
        //std::cout<<std::endl<<"m_double_click_detector.IsDoubleClicked()";
        //... perform picking.
        m_gui_renderer->m_cell_picker->Pick(rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], 0, m_gui_renderer->GetVTKRenderer());
        
        if(m_gui_renderer->m_cell_picker->GetCellId() != -1)
        {
            m_gui_renderer->m_pick_marker_actor->SetPosition(m_gui_renderer->m_cell_picker->GetPickPosition());
            m_gui_renderer->m_pick_marker_actor->SetVisibility(1);
        }
    }

    
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}


void vmxGUIInteractorStyle3DTrackballCamera::OnLeftButtonUp()
{
    vtkTextActor *ta = m_gui_renderer->GetCommand(this->Interactor->GetEventPosition()[0],this->Interactor->GetEventPosition()[1]);
    
    if(ta == m_gui_renderer->m_command_reset_view)
    {
        m_gui_renderer->GetVTKRenderer()->ResetCamera();
        m_gui_renderer->GetVTKRenderer()->Render();
        
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
        return;
    }
    if(ta == m_gui_renderer->m_command_seeds_add_new)
    {
        ////m_gui_renderer->m_picked_indexes;
        
        
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
        return;
    }
    if(ta == m_gui_renderer->m_command_seeds_clear_all)
    {
        m_gui_renderer->ClearAllPickedPositions();
        m_gui_renderer->UpdatePickedPositionsVisualization();
        this->Interactor->Render();
        
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
        return;
    }
    if(ta == m_gui_renderer->m_command_seeds_toggle_view)
    {
        int is_visible = m_gui_renderer->IsPickedPositionsActorVisible();
        m_gui_renderer->SetVisibilityOfPickedPositions(!is_visible);
        this->Interactor->Render();
        
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
        return;
    }
    
    // Don't forget to invoke event, otherwise connections won't work.
    this->m_gui_renderer->InvokeEvent(LeftButtonUpEvent, this->m_gui_renderer, NULL);
    
    //if(ta) std::cout<<std::endl<<ta->GetInput();
    //else std::cout<<std::endl<<"None";
    
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}



void vmxGUIInteractorStyle3DTrackballCamera::OnKeyPress()
{
    //get the keypress
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    
    
    if(key.compare("r") == 0)
    {
        m_gui_renderer->GetVTKRenderer()->ResetCamera();
        rwi->Render();
        return;
    }
    if(key.compare("k") == 0)
    {
        vtkSmartPointer< vtkCellPicker > picker = vtkSmartPointer< vtkCellPicker >::New();
        
        picker->SetTolerance(0.0005);
        
        // Pick from this location.
        picker->Pick(rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], 0, m_gui_renderer->GetVTKRenderer());
        
        double* pick_position = picker->GetPickPosition();
        
        
        if(picker->GetCellId() != -1)
        {
            m_gui_renderer->AddPickPosition(pick_position[0], pick_position[1], pick_position[2], 0);
            
            this->m_gui_renderer->InvokeEvent(SeedPointsModifiedEvent, this->m_gui_renderer, NULL);
            
            m_gui_renderer->UpdatePickedPositionsVisualization();
            
            rwi->Render();
        }
        
        return;
    }
    if(key.compare("l") == 0)
    {
        int is_visible = m_gui_renderer->IsPickedPositionsActorVisible();
        m_gui_renderer->SetVisibilityOfPickedPositions(!is_visible);
        rwi->Render();
        
        return;
    }
    if(key.compare("e") == 0)
    {
        m_gui_renderer->ClearAllPickedPositions();
        m_gui_renderer->UpdatePickedPositionsVisualization();
        rwi->Render();
        
        return;
    }
    
    // If the key press event has not been caught, invoke the event.
    m_gui_renderer->InvokeEvent(KeyPressEvent, this->m_gui_renderer, NULL);
}




//---------------------------------------------------------------------------



vmxGUIRenderer3DTrackballCamera::vmxGUIRenderer3DTrackballCamera(vmxGUIMainWidget *main_widget)
{
    m_class_name.Assign("vmxGUIRenderer3DTrackballCamera");
    
    m_clip_board = NULL;
    
    m_is_capturing_event = 1;
    
    m_main_widget = main_widget;
    
    if(m_main_widget)
    {
        m_clip_board = main_widget->GetClipBoard();
    }
    
    
    m_cell_picker = vtkSmartPointer< vtkCellPicker >::New();
    m_cell_picker->SetTolerance(0.0005);
    
    m_interactor_style = vtkSmartPointer<vmxGUIInteractorStyle3DTrackballCamera>::New();
    m_interactor_style->m_gui_renderer = this;
    
    m_index_slice = 0;
    m_index_time = 0;
    
    
    
    
    // VTK structures for creating pick marker.
    m_pick_marker_actor = vtkSmartPointer<vtkActor>::New();
    m_pick_marker_lines = vtkSmartPointer<vtkCellArray>::New();
    m_pick_marker_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_pick_marker_points = vtkSmartPointer<vtkPoints>::New();
    m_pick_marker_poly_data = vtkSmartPointer<vtkPolyData>::New();
    
    
    this->internal_CreateCrossHairs();
    
    m_renderer->AddActor(m_pick_marker_actor);
    
    
    // VTK structures for visualization of picked points.
    m_pp_points = vtkSmartPointer<vtkPoints>::New();
    m_pp_poly_data = vtkSmartPointer<vtkPolyData>::New();
    m_pp_sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    m_pp_glyph = vtkSmartPointer<vtkGlyph3D>::New();
    m_pp_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_pp_actor = vtkSmartPointer<vtkActor>::New();
    
    m_renderer->AddActor(m_pp_actor);
    
    
    m_command_index_slice_time = this->AddCommand(" ");
    
    m_command_reset_view = this->AddCommand("(R) Reset view");
    m_command_seeds_add_new = this->AddCommand("(K) Add seed");
    m_command_seeds_clear_all = this->AddCommand("(E) Clear seeds");
    m_command_seeds_toggle_view = this->AddCommand("(L) Seeds toggle view");
}


vmxGUIRenderer3DTrackballCamera::~vmxGUIRenderer3DTrackballCamera()
{
    
}


void vmxGUIRenderer3DTrackballCamera::DisplayProperties()
{
    mxString st;
    st.Assign("S: ");
    st.AppendNumber((int)m_index_slice);
    st.Append("  T: ");
    st.AppendNumber((int)m_index_time);
    m_command_index_slice_time->SetInput(st.Get_C_String());
}


void vmxGUIRenderer3DTrackballCamera::GetPickedWorldPosition(double &x, double &y, double &z)
{
    x = m_pick_marker_actor->GetPosition()[0];
    y = m_pick_marker_actor->GetPosition()[1];
    z = m_pick_marker_actor->GetPosition()[2];
}


void vmxGUIRenderer3DTrackballCamera::internal_CreateCrossHairs(int size)
{
    m_pick_marker_lines->Initialize();
    m_pick_marker_points->Initialize();
    m_pick_marker_poly_data->Initialize();
    
    double factor = (double)size;

    double pos[3], direction[3];
    
    //----- point (-1,0,0) -----
    pos[0] = -1.0; pos[1] = 0.0; pos[2] = 0.0;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = 1.0; direction[1] = 0.0; direction[2] = 0.0;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----- point (0,-1,0) -----
    pos[0] = 0.0; pos[1] = -1.0; pos[2] = 0.0;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = 0.0; direction[1] = 1.0; direction[2] = 0.0;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----- point (0,0,-1) -----
    pos[0] = 0.0; pos[1] = 0.0; pos[2] = -1.0;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = 0.0; direction[1] = 0.0; direction[2] = 1.0;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---

    m_pick_marker_poly_data->SetPoints(m_pick_marker_points);
    m_pick_marker_poly_data->SetLines(m_pick_marker_lines);

    m_pick_marker_mapper->SetInputData(m_pick_marker_poly_data);
    m_pick_marker_mapper->ScalarVisibilityOff();
    
    m_pick_marker_actor->SetMapper(m_pick_marker_mapper);
    m_pick_marker_actor->GetProperty()->SetColor(1,0,0);
    m_pick_marker_actor->SetVisibility(0);
}


void vmxGUIRenderer3DTrackballCamera::UpdatePickedPositionsVisualization()
{
    m_pp_points->Initialize();
    m_pp_poly_data->Initialize();
    
    for(int n=0; n<this->m_number_of_picked_positions; n++)
    {
        
        //        if(slice_index==m_index_slice)
        {
            //cout<<" P ";
            double lp[3];
            lp[0] = this->m_picked_poisitions[n].m_x;
            lp[1] = this->m_picked_poisitions[n].m_y;
            lp[2] = this->m_picked_poisitions[n].m_z;
            m_pp_points->InsertPoint(n, lp[0], lp[1], lp[2]);
        }
    }
    
    
    
    //    if(!m_picked_positions_per_index[m_index_slice].IsEmpty())
    //    {
    //        cout<<" P ";
    //        mxListIterator< vmxGUIRendererPosition* > it;
    //        int n = 0;
    //        for(it.SetToBegin(m_picked_positions_per_index[m_index_slice]); it.IsValid(); it.MoveToNext())
    //        {
    //            double lp[3];
    //            this->GetLocalCoordinatesForWorldCoordinates(it.GetElement()->m_z, it.GetElement()->m_y, it.GetElement()->m_x, lp[2], lp[1], lp[0]);
    //            //m_pp_points->InsertPoint(n, this->m_picked_poisitions[n].m_x, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_z);
    //            m_pp_points->InsertPoint(n, lp[0], lp[1], lp[2]);
    //            n++;
    //        }
    //    }
    
    //    //for(unsigned int n=0; n<this->m_number_of_picked_positions; n++)
    //    int n=0;
    //    for(unsigned int i=0; i<this->m_number_of_picked_positions; i++)
    //    {
    //
    //        int index[3];
    //        this->GetIndexesForWorldCoordinates(this->m_picked_poisitions[n].m_z, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_x, index[2], index[1], index[0]);
    //        int slice_index;
    //
    //        if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    //        {
    //            slice_index = index[2];
    //        }
    //        else
    //        {
    //            if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    //            {
    //                slice_index = index[1];
    //            }
    //            else //if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    //            {
    //                slice_index = index[0];
    //            }
    //        }
    //
    //        cout<<"  slice_index="<<slice_index<<", m_index_slice="<<m_index_slice<<"  ";
    //        //cout<<" index[2]="<<index[2]<<" index[1]="<<index[1]<<" index[0]="<<index[0]<<", m_index_slice="<<m_index_slice<<"  ";
    //
    //        if(slice_index==m_index_slice)
    //        {
    //            cout<<" P ";
    //            double lp[3];
    //            this->GetLocalCoordinatesForWorldCoordinates(this->m_picked_poisitions[n].m_z, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_x, lp[2], lp[1], lp[0]);
    //            //m_pp_points->InsertPoint(n, this->m_picked_poisitions[n].m_x, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_z);
    //            m_pp_points->InsertPoint(n, lp[0], lp[1], lp[2]);
    //            n++;
    //        }
    //    }
    
    
    
    m_pp_poly_data->SetPoints(m_pp_points);
    
    //    //Add scalars with index 0 as current scalars
    //    m_poly_data->GetPointData()->SetScalars(m_scalars_vtk[0]);
    
    m_pp_sphere_source->SetRadius(2);
    //m_pp_glyph->SetColorModeToColorByScalar();
    m_pp_glyph->SetSourceConnection(m_pp_sphere_source->GetOutputPort());
    m_pp_glyph->SetInputData(m_pp_poly_data);
    m_pp_glyph->ScalingOff();
    m_pp_glyph->Update();
    
    m_pp_mapper->SetInputConnection(m_pp_glyph->GetOutputPort());
    //    double scalar_min_value, scalar_max_value;
    //    this->GetScalarMinAndMaxValue(0,scalar_min_value,scalar_max_value);
    //    this->m_mapper->SetScalarRange(scalar_min_value, scalar_max_value);//for scalars
    
    this->m_pp_mapper->ScalarVisibilityOff();
    
    m_pp_actor->SetMapper(m_pp_mapper);
    //-----
    
    //    //----- LABELS -----
    //    m_labels->Initialize();
    //    m_labels->SetName("Labels");
    //    m_labels->SetNumberOfValues(m_points->GetNumberOfPoints());
    //    number_of_points = 0;
    //    for(it.SetLeftEnd(m_positions); it.IsValid(); it.MoveRight())
    //    {
    //        m_labels->SetValue(number_of_points,it.GetElementPointer()->GetDescription().C_String());
    //        number_of_points++;
    //    }
    //
    //    m_labels_point_source->SetNumberOfPoints(m_points->GetNumberOfPoints());
    //    m_labels_point_source->Update();
    //    m_labels_point_source->GetOutput()->GetPointData()->AddArray(m_labels);
    //    m_labels_point_source->GetOutput()->SetPoints(m_points);
    //
    //    m_labels_hierarchy_filter->SetInputConnection(m_labels_point_source->GetOutputPort());
    //    m_labels_hierarchy_filter->SetLabelArrayName("Labels");
    //    m_labels_hierarchy_filter->Update();
    //
    //    //m_labels_mapper->UseDepthBufferOn();
    //    m_labels_mapper->SetMaximumLabelFraction(0.5);
    //    m_labels_mapper->SetInputConnection(m_labels_hierarchy_filter->GetOutputPort());
    //    m_labels_actor->SetMapper(m_labels_mapper);
    //    //-----
}
