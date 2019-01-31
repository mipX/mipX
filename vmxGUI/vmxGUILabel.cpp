/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUILabel.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUILabel.h"




vmxGUILabel::vmxGUILabel()
{
    m_clip_board = NULL;
    
    m_class_name.Assign("vmxGUILabel");
    
    m_interactor = NULL;
    
    m_renderer = NULL;
    
    m_font_size = 18;

    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxGUILabel::~vmxGUILabel()
{
}


vmxGUILabel* vmxGUILabel::New()
{
    return new vmxGUILabel;
}


void vmxGUILabel::GetOrigin(int &origin1, int &origin2)
{
    double *org = m_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    //return 1;
}


void vmxGUILabel::GetSize(int &output_size1, int &output_size2)
{
    if(!m_interactor) return;// 0;
    
    double size[2];
    m_text_actor->GetSize(m_main_widget->GetRenderer_GUI(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
}


int vmxGUILabel::HasVTKActor2D(vtkActor2D *actor_2D)
{
    vtkActor2D *text_actor = (vtkActor2D*) m_text_actor;
    if(text_actor==actor_2D) return 1;
    return 0;
}


int vmxGUILabel::IsVisible()
{
    return this->m_text_actor->GetVisibility();
}


void vmxGUILabel::SetColor(double r, double g, double b)
{
    m_text_actor->GetTextProperty()->SetColor(r,g,b);
}


void vmxGUILabel::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;
    
    m_text_actor->GetTextProperty()->SetFontFamilyToArial();    
    m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
}


void vmxGUILabel::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
}


void vmxGUILabel::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    this->vmxGUIWidget::SetMainWidget(main_widget);
    
    this->SetClipBoard(main_widget->GetClipBoard());
    this->SetRenderer(main_widget->GetRenderer_GUI());
    this->SetInteractor(main_widget->GetInteractor());
}


void vmxGUILabel::SetOrigin(int origin1, int origin2)
{
    int text_actor_size[2];
    this->GetSize(text_actor_size[0], text_actor_size[1]);
    
    m_text_actor->SetPosition(origin1,origin2);
    m_placement = FIXED;
}


void vmxGUILabel::SetRenderer(vtkRenderer *renderer)
{
    m_renderer = renderer;
    
    if(m_renderer) m_renderer->AddActor2D(m_text_actor);
}


void vmxGUILabel::SetVisibility(int is_visible)
{
    this->ShowInputText();
    this->m_text_actor->SetVisibility(is_visible);
    this->RedoPlacement();
}


int vmxGUILabel::ShowInputText()
{
    m_text_actor->SetInput(m_description.Get_C_String());
    return 1;
}








