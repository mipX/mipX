/*=========================================================================
 
 Program:   mipx
 Module:    vmxImagePlaneWidget.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxImagePlaneWidget.h"



vmxImagePlaneWidget::vmxImagePlaneWidget()
{
    m_is_double_clicked = 0;
    m_number_of_clicks = 0;
}


vmxImagePlaneWidget::~vmxImagePlaneWidget()
{
}


vmxImagePlaneWidget* vmxImagePlaneWidget::New()
{
    return new vmxImagePlaneWidget;
}


void vmxImagePlaneWidget::OnLeftButtonDown()
{
    m_time_of_left_click_in_ms2 = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    
    long time_diff = (m_time_of_left_click_in_ms2 - m_time_of_left_click_in_ms).count();
    m_time_of_left_click_in_ms = m_time_of_left_click_in_ms2;
    
    m_number_of_clicks++;
    
   
    
    vtkRenderWindowInteractor *rwi = this->Interactor;
    
    
    int pickPosition[2];
    rwi->GetEventPosition(pickPosition);
    
    int xdist = pickPosition[0] - m_left_button_down_previous_position[0];
    int ydist = pickPosition[1] - m_left_button_down_previous_position[1];
    
    m_left_button_down_previous_position[0] = pickPosition[0];
    m_left_button_down_previous_position[1] = pickPosition[1];
    
    int squared_move_distance = xdist*xdist + ydist*ydist;
    
    // Reset numClicks - If mouse moved further than resetPixelDistance
    if(squared_move_distance > 25 || time_diff>700)//this->ResetPixelDistance)
    {
        this->m_number_of_clicks = 1;
        m_is_double_clicked = 0;
    }
    
    
    if(this->m_number_of_clicks == 2)
    {
        this->m_number_of_clicks = 0;
        m_is_double_clicked = 1;
    }
    
    
    if(m_is_double_clicked)
    {
        vtkImagePlaneWidget::OnLeftButtonDown();
        //rwi->LeftButtonPressEvent();
    }
}

