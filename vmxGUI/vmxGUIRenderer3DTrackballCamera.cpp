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
    vtkRenderWindowInteractor *rwi = this->Interactor;
    m_double_click_detector.OnLeftButtonDown(rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], 0);
    
    // if double click is detected...
    if(m_double_click_detector.IsDoubleClicked())
    {
        //... perform picking.
        m_gui_renderer->m_cell_picker->Pick(rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], 0, m_gui_renderer->GetVTKRenderer());
        
        if(m_gui_renderer->m_cell_picker->GetCellId() != -1)
        {
            m_gui_renderer->m_pick_marker_actor->SetPosition(m_gui_renderer->m_cell_picker->GetPickPosition());
        }
    }

    
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
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
    
    
    
//    m_index_slice = 0;
//    m_index_time = 0;
    
    
    
    
    // VTK structures for creating pick marker.
    m_pick_marker_actor = vtkSmartPointer<vtkActor>::New();
    m_pick_marker_lines = vtkSmartPointer<vtkCellArray>::New();
    m_pick_marker_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_pick_marker_points = vtkSmartPointer<vtkPoints>::New();
    m_pick_marker_poly_data = vtkSmartPointer<vtkPolyData>::New();
    
    
    this->internal_CreateCrossHairs();
    
    m_renderer->AddActor(m_pick_marker_actor);
    
    
    
}


vmxGUIRenderer3DTrackballCamera::~vmxGUIRenderer3DTrackballCamera()
{
    
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

