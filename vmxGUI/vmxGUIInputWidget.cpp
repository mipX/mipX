/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIInputWidget.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIInputWidget.h"




vmxGUIInputWidget::vmxGUIInputWidget()
{
    m_clip_board = NULL;
    
    m_input_type = vmxGUIInputWidget::TEXT;
    
    m_class_name.Assign("vmxGUIInputWidget");
    
    m_is_keypress_active = 0;
    
    m_interactor = NULL;
    
    m_renderer = NULL;
    
    m_font_size = 18;

    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxGUIInputWidget::~vmxGUIInputWidget()
{
}


vmxGUIInputWidget* vmxGUIInputWidget::New()
{
    return new vmxGUIInputWidget;
}


void vmxGUIInputWidget::GetInputItems(mxList< mxString > &items_list)
{
    items_list.Reset();
    if(m_input_type != vmxGUIInputWidget::ITEM) return;
    
    mxString s;
    s.Assign(vmxGUIItemSeparationChar2); s.Append(m_input);
    s.ExtractStrings(vmxGUIItemSeparationChar2, vmxGUIItemSeparationChar1, items_list);
}


void vmxGUIInputWidget::GetOrigin(int &origin1, int &origin2)
{
    double *org = m_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    //return 1;
}


void vmxGUIInputWidget::GetSize(int &output_size1, int &output_size2)
{
    if(!m_interactor) return;// 0;
    
    double size[2];
    m_text_actor->GetSize(m_main_widget->GetRenderer_GUI(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
}


int vmxGUIInputWidget::HasVTKActor2D(vtkActor2D *actor_2D)
{
    vtkActor2D *text_actor = (vtkActor2D*) m_text_actor;
    if(text_actor==actor_2D) return 1;
    return 0;
}


int vmxGUIInputWidget::IsDotEntered()
{
    for(unsigned int i=0; i<m_input.GetNumberOfCharacters(); i++)
    {
        if(m_input[i]=='.') { return 1; }
    }
    return 0;
}


int vmxGUIInputWidget::IsVisible()
{
    return this->m_text_actor->GetVisibility();
}


void vmxGUIInputWidget::OnKeyPress()
{
    if(this->GetMainWidget())
    {
        if(this->GetMainWidget()->GetInteractor())
        {
            mxString key;
            key.Assign(this->GetMainWidget()->GetInteractor()->GetKeySym());
            char c = this->GetMainWidget()->GetInteractor()->GetKeyCode();

            
            // for INT
            if(this->m_input_type == vmxGUIInputWidget::TEXT)
            {
                for(int execute_once = 1; execute_once; execute_once = 0)
                {
                    if(key=="Backspace")
                    {
                        if(this->m_input.GetNumberOfCharacters()>0)
                        {
                            this->m_input.PopBack(); //this->m_input.GetStdString()->pop_back();
                            this->ShowInputText();
                            this->RepositionAfterResizing();
                            this->GetMainWidget()->GetInteractor()->Render();
                        }
                        break;
                    }
                    
                    if(key=="Left" || key=="Right" || key=="Up" || key=="Down")
                    {
                        break;
                    }
                    
                    // if any other key is pressed...
                    {
                        this->m_input.Append(c);
                        this->ShowInputText();
                        this->RepositionAfterResizing();
                        this->GetMainWidget()->GetInteractor()->Render();
                    }
                }
            }
            
            // for DOUBLE
            if(this->m_input_type == vmxGUIInputWidget::DOUBLE)
            {
                for(int execute_once = 1; execute_once; execute_once = 0)
                {
                    if(key=="Backspace")
                    {
                        if(this->m_input.GetNumberOfCharacters()>0)
                        {
                            this->m_input.PopBack(); //this->m_input.GetStdString()->pop_back();
                            this->ShowInputText();
                            this->RepositionAfterResizing();
                            this->GetMainWidget()->GetInteractor()->Render();
                        }
                        break;
                    }
                    
                    // If a natural number is pressed...
                    if(c>='0' && c<='9')
                    {
                        this->m_input.Append(c);
                        this->ShowInputText();
                        this->RepositionAfterResizing();
                        this->GetMainWidget()->GetInteractor()->Render();
                        break;
                    }
                    if(c=='+' || c=='-')
                    {
                        if(this->m_input.GetNumberOfCharacters()==0)
                        {
                            this->m_input.Append(c);
                            this->ShowInputText();
                            this->RepositionAfterResizing();
                            this->GetMainWidget()->GetInteractor()->Render();
                        }
                        break;
                    }
                    if(c=='.')
                    {
                        if(this->m_input.GetNumberOfCharacters()>0)
                        {
                            if(this->m_input[0]=='+' || this->m_input[0]=='-')
                            {
                                if(this->m_input.GetNumberOfCharacters()>1)
                                {
                                    if(!this->IsDotEntered())
                                    {
                                        this->m_input.Append(c);
                                        this->ShowInputText();
                                        this->RepositionAfterResizing();
                                        this->GetMainWidget()->GetInteractor()->Render();
                                    }
                                }
                            }
                            else
                            {
                                if(!this->IsDotEntered())
                                {
                                    this->m_input.Append(c);
                                    this->ShowInputText();
                                    this->RepositionAfterResizing();
                                    this->GetMainWidget()->GetInteractor()->Render();
                                }
                            }
                        }
                        break;
                    }
                }
            }
            
            // for TEXT
            if(this->m_input_type == vmxGUIInputWidget::INTEGER)
            {
                for(int execute_once = 1; execute_once; execute_once = 0)
                {
                    if(key=="Backspace")
                    {
                        if(this->m_input.GetNumberOfCharacters()>0)
                        {
                            this->m_input.PopBack(); //this->m_input.GetStdString()->pop_back();
                            this->ShowInputText();
                            this->RepositionAfterResizing();
                            this->GetMainWidget()->GetInteractor()->Render();
                        }
                        break;
                    }
                    
                    // If a natural number is pressed...
                    if(c>='0' && c<='9')
                    {
                        this->m_input.Append(c);
                        this->ShowInputText();
                        this->RepositionAfterResizing();
                        this->GetMainWidget()->GetInteractor()->Render();
                        break;
                    }
                    
                    if(c=='+' || c=='-')
                    {
                        if(this->m_input.GetNumberOfCharacters()==0)
                        {
                            this->m_input.Append(c);
                            this->ShowInputText();
                            this->RepositionAfterResizing();
                            this->GetMainWidget()->GetInteractor()->Render();
                        }
                        break;
                    }
                }
            }
        }
    }
}


void vmxGUIInputWidget::OnLeftButtonUp()
{
    if(this->GetMainWidget())
    {
        if(this->GetMainWidget()->GetInteractor())
        {
            int pick_pos[2];
            this->GetMainWidget()->GetInteractor()->GetEventPosition(pick_pos);
            
            if(this->IsPicked(pick_pos[0], pick_pos[1]))
            {
                // if the data on the clipboard is valid...
                if(this->m_clip_board->m_is_valid)
                {
                    // ...and the input type is item input...
                    if(this->m_input_type == vmxGUIInputWidget::ITEM)
                    {
                        // ...copy it to the text input.
                        this->SetInputText(this->m_clip_board->m_data_text.Get_C_String());
                    }
                }
                else
                {
                    // If the data on clipboard is not valid...
                    
                    // ...and if the key press is not active...
                    if(!this->m_is_keypress_active)
                    {
                        //...reset text input and activate key press event (allow user to enter ext from keyboard).
                        this->SetListeningFor_KeyPress_Event(1);
                        this->m_is_keypress_active = 1;
                    }
                }
                this->ShowInputText();
                this->RepositionAfterResizing();
                this->m_interactor->Render();
            }
            else
            {                
                // if the object is not picked, turn off key press event (stop user from entring text from keyboard).
                this->SetListeningFor_KeyPress_Event(0);
                this->m_is_keypress_active = 0;
            }
        }
    }
}


void vmxGUIInputWidget::SetColor(double r, double g, double b)
{
    m_text_actor->GetTextProperty()->SetColor(r,g,b);
}


void vmxGUIInputWidget::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;
    
    m_text_actor->GetTextProperty()->SetFontFamilyToArial();
    m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
}


void vmxGUIInputWidget::SetInputTypeToDoubleNumber(double default_value)
{
    m_input.AssignNumber(default_value);
    m_input_type = vmxGUIInputWidget::DOUBLE;
}


void vmxGUIInputWidget::SetInputTypeToIntegerNumber(int default_value)
{
    m_input.AssignNumber(default_value);
    m_input_type = vmxGUIInputWidget::INTEGER;
}


void vmxGUIInputWidget::SetInputTypeToItem()
{
    m_input_type = vmxGUIInputWidget::ITEM;
}


void vmxGUIInputWidget::SetInputTypeToText(const char *default_text)
{
    m_input.Assign(default_text);
    m_input_type = vmxGUIInputWidget::TEXT;
}


void vmxGUIInputWidget::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
}


void vmxGUIInputWidget::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    this->vmxGUIWidget::SetMainWidget(main_widget);
    
    this->SetClipBoard(main_widget->GetClipBoard());
    this->SetRenderer(main_widget->GetRenderer_GUI());
    this->SetInteractor(main_widget->GetInteractor());
}


void vmxGUIInputWidget::SetOrigin(int origin1, int origin2)
{
    int text_actor_size[2];
    this->GetSize(text_actor_size[0], text_actor_size[1]);
    
    m_text_actor->SetPosition(origin1,origin2);
    m_placement = FIXED;
}


void vmxGUIInputWidget::SetRenderer(vtkRenderer *renderer)
{
    m_renderer = renderer;
    
    if(m_renderer) m_renderer->AddActor2D(m_text_actor);
}


void vmxGUIInputWidget::SetVisibility(int is_visible)
{
    this->ShowInputText();
    this->m_text_actor->SetVisibility(is_visible);
    this->RedoPlacement();
}


int vmxGUIInputWidget::ShowInputText()
{
    mxString input_text;
    input_text.Assign(m_description);
    input_text.Append(m_input);
    
    m_text_actor->SetInput(input_text.Get_C_String());
    
    return 1;
}








