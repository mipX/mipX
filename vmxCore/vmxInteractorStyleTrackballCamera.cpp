/*=========================================================================
 
 Program:   mipx
 Module:    vmxInteractorStyleTrackballCamera.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxInteractorStyleTrackballCamera.h"




vmxInteractorStyleTrackballCamera::vmxInteractorStyleTrackballCamera()
{
    m_is_double_clicked = 0;
}


vmxInteractorStyleTrackballCamera::~vmxInteractorStyleTrackballCamera()
{
    
}


vmxInteractorStyleTrackballCamera* vmxInteractorStyleTrackballCamera::New()
{
    return new vmxInteractorStyleTrackballCamera;
}


void vmxInteractorStyleTrackballCamera::OnLeftButtonDown()
{
    m_time_of_left_click_in_ms2 = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    
    long time_diff = (m_time_of_left_click_in_ms2 - m_time_of_left_click_in_ms).count();
    m_time_of_left_click_in_ms = m_time_of_left_click_in_ms2;
    
    //cout<<" time_diff="<<time_diff<<"  ";
    
    //std::cout << "Pressed left mouse button." << std::endl;
    this->m_number_of_clicks++;
    //std::cout << "NumberOfClicks = " << this->NumberOfClicks << std::endl;
    
    
    vtkRenderWindowInteractor *rwi = this->Interactor;
    
    int pickPosition[2];
    rwi->GetEventPosition(pickPosition);
    
    int xdist = pickPosition[0] - this->m_previous_position[0];
    int ydist = pickPosition[1] - this->m_previous_position[1];
    
    this->m_previous_position[0] = pickPosition[0];
    this->m_previous_position[1] = pickPosition[1];
    
    int moveDistance = (int)sqrt((double)(xdist*xdist + ydist*ydist));
    
    // Reset numClicks - If mouse moved further than resetPixelDistance
    if(moveDistance > 5 || time_diff>700)//this->ResetPixelDistance)
    {
        this->m_number_of_clicks = 1;
        m_is_double_clicked = 0;
    }
    
    
    if(this->m_number_of_clicks == 2)
    {
        //std::cout << "Double clicked." << std::endl;
        this->m_number_of_clicks = 0;
        //rwi->GetPicker()->
        m_is_double_clicked = 1;
        //m_menu->GetPickedItem(rwi->GetEventPosition()[0],rwi->GetEventPosition()[1]);
    }
    // forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}


void vmxInteractorStyleTrackballCamera::OnKeyPress()
{
    //get the keypress
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    
    if(key.compare("a") == 0)
    {
        //cout<<" a ";
        //m_v_img->SetIndexTime(m_v_img->GetIndexTime()+1);
        rwi->Render();
    }
    if(key.compare("z") == 0)
    {
        //cout<<" z ";
        //m_v_img->SetIndexTime(m_v_img->GetIndexTime()-1);
        rwi->Render();
    }
    if(key.compare("s") == 0)
    {
        //cout<<" s ";
        //m_v_img->SetIndexSlices(m_v_img->GetIndexSlices()+1);
        rwi->Render();
    }
    if(key.compare("x") == 0)
    {
        //cout<<" x ";
        //m_v_img->SetIndexSlices(m_v_img->GetIndexSlices()-1);
        rwi->Render();
    }
    if(key.compare("q") == 0)
    {
        //m_tree->ReleaseAllDataObjects();
        rwi->GetRenderWindow()->Finalize();
        
        // Stop the interactor
        rwi->TerminateApp();
    }
    if(key.compare("p") == 0)
    {
        cout<<"event_pos="<<rwi->GetEventPosition()[0]<<","<< rwi->GetEventPosition()[1]<<"   ";
        //m_menu->GetPickedItem(rwi->GetEventPosition()[0],rwi->GetEventPosition()[1]);
        
        //            m_tree->ReleaseAllDataObjects();
        //            rwi->GetRenderWindow()->Finalize();
        //            //
        //            //    // Stop the interactor
        //            rwi->TerminateApp();
    }
}

