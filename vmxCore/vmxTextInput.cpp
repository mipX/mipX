/*=========================================================================
 
 Program:   mipx
 Module:    vmxTextInput.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxTextInput.h"




vmxTextInputRenderWindowModifiedCallback::vmxTextInputRenderWindowModifiedCallback()
{
    m_text_input = NULL;
    m_previous_window_size[0] = m_previous_window_size[1] = 0;
}


vmxTextInputRenderWindowModifiedCallback::~vmxTextInputRenderWindowModifiedCallback()
{
    m_text_input = NULL;
}


void vmxTextInputRenderWindowModifiedCallback::SetTextInput(vmxTextInput *text_input)
{
    m_text_input = text_input;
}


vmxTextInputRenderWindowModifiedCallback* vmxTextInputRenderWindowModifiedCallback::New()
{
    return new vmxTextInputRenderWindowModifiedCallback;
}


void vmxTextInputRenderWindowModifiedCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_text_input)
    {
        if(m_text_input->m_interactor)
        {
            //cout<<"*";
            int *size = m_text_input->m_interactor->GetRenderWindow()->GetSize();
            if(size[0]!=m_previous_window_size[0] || size[1]!=m_previous_window_size[1])
            {
                //cout<<"-";
                m_previous_window_size[0] = size[0];
                m_previous_window_size[1] = size[1];
                m_text_input->RedoPlacement();
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxTextInputInteractorLeftButtonDownCallback::vmxTextInputInteractorLeftButtonDownCallback()
{
    m_text_input = NULL;
}


vmxTextInputInteractorLeftButtonDownCallback::~vmxTextInputInteractorLeftButtonDownCallback()
{
    m_text_input = NULL;
}


void vmxTextInputInteractorLeftButtonDownCallback::SetTextInput(vmxTextInput *text_input)
{
    m_text_input = text_input;
}


vmxTextInputInteractorLeftButtonDownCallback* vmxTextInputInteractorLeftButtonDownCallback::New()
{
    return new vmxTextInputInteractorLeftButtonDownCallback;
}


void vmxTextInputInteractorLeftButtonDownCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_text_input)
    {
        if(m_text_input->m_interactor)
        {
            int pick_pos[2];
            m_text_input->m_interactor->GetEventPosition(pick_pos);
            
            if(m_text_input->IsPicked(pick_pos[0], pick_pos[1]))
            {
                m_text_input->SetActive(1);
            }
            else
            {
                m_text_input->SetActive(0);
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxTextInputInteractorLeftButtonUpCallback::vmxTextInputInteractorLeftButtonUpCallback()
{
    m_text_input = NULL;
}


vmxTextInputInteractorLeftButtonUpCallback::~vmxTextInputInteractorLeftButtonUpCallback()
{
    m_text_input = NULL;
}


void vmxTextInputInteractorLeftButtonUpCallback::SetTextInput(vmxTextInput *text_input)
{
    m_text_input = text_input;
}


vmxTextInputInteractorLeftButtonUpCallback* vmxTextInputInteractorLeftButtonUpCallback::New()
{
    return new vmxTextInputInteractorLeftButtonUpCallback;
}


void vmxTextInputInteractorLeftButtonUpCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_text_input)
    {
        if(m_text_input->m_interactor)
        {
            int pick_pos[2];
            m_text_input->m_interactor->GetEventPosition(pick_pos);
            
            if(m_text_input->IsPicked(pick_pos[0], pick_pos[1]))
            {
                if(m_text_input->m_clip_board->m_is_valid)
                {
                    m_text_input->m_text_actor->SetInput(m_text_input->m_clip_board->m_data_text.Get_C_String());
                    m_text_input->m_interactor->Render();
                }
                m_text_input->SetActive(1);
            }
            else
            {
                m_text_input->SetActive(0);
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------





vmxTextInputInteractorKeyPressCallback::vmxTextInputInteractorKeyPressCallback()
{
    m_text_input = NULL;
}


vmxTextInputInteractorKeyPressCallback::~vmxTextInputInteractorKeyPressCallback()
{
    m_text_input = NULL;
}


void vmxTextInputInteractorKeyPressCallback::SetTextInput(vmxTextInput *text_input)
{
    m_text_input = text_input;
}


vmxTextInputInteractorKeyPressCallback* vmxTextInputInteractorKeyPressCallback::New()
{
    return new vmxTextInputInteractorKeyPressCallback;
}


void vmxTextInputInteractorKeyPressCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_text_input)
    {
        if(m_text_input->m_interactor)
        {
            if(m_text_input->m_is_active)
            {
            
                //char *key = m_text_input->m_interactor->GetKeySym();
                mxString key;
                key.Assign(m_text_input->m_interactor->GetKeySym());
                //cout<<key;
                char c = key[0];
                
                
                //if(c==8 || c==127)//value 8 is backspace
                if(key=="Backspace")
                {
                    if(m_text_input->m_number_of_entered_characters>0)
                    {
                        m_text_input->m_number_of_entered_characters--;
                        m_text_input->ShowInputText();
                        m_text_input->m_interactor->Render();
                    }
                }
                else
                {
                    if(m_text_input->m_number_of_entered_characters<99)
                    {
                        m_text_input->m_input[m_text_input->m_number_of_entered_characters] = c;
                        m_text_input->m_number_of_entered_characters++;
                        m_text_input->ShowInputText();
                        m_text_input->m_interactor->Render();

                    }
                }
                
                //cout<<c;
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------





vmxTextInput::vmxTextInput()
{
    m_clip_board = NULL;
    
    m_placement = FREE;
    m_placement_relative_percentages[0] = m_placement_relative_percentages[1] = 0;
    m_window_modified_callback = vtkSmartPointer<vmxTextInputRenderWindowModifiedCallback>::New();
    m_window_modified_callback->SetTextInput(this);
    m_left_button_down_callback = vtkSmartPointer<vmxTextInputInteractorLeftButtonDownCallback>::New();
    m_left_button_down_callback->SetTextInput(this);
    m_key_press_callback = vtkSmartPointer<vmxTextInputInteractorKeyPressCallback>::New();
    m_key_press_callback->SetTextInput(this);
    m_left_button_up_callback = vtkSmartPointer<vmxTextInputInteractorLeftButtonUpCallback>::New();
    m_left_button_up_callback->SetTextInput(this);
    
    
    m_number_of_entered_characters = 0;
    
    m_is_active = 0;
    m_interactor = NULL;
    m_font_size = 18;

    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxTextInput::~vmxTextInput()
{
}


vmxTextInput* vmxTextInput::New()
{
    return new vmxTextInput;
}


int vmxTextInput::GetOrigin(int &origin1, int &origin2)
{
    double *org = m_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    return 1;
}


int vmxTextInput::GetSize(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_text_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
    
    return 1;
}


int vmxTextInput::IsPicked(int pos1, int pos2)
{
    int size[2];
    this->GetSize(size[0],size[1]);
    
    int origin[2];
    this->GetOrigin(origin[0],origin[1]);
    
    int end_pos1 = origin[0] + size[0];
    if(pos1<origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = origin[1] + size[1];
    if(pos2<origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


void vmxTextInput::RedoPlacement()
{
    //    if(m_placement==FREE)
    //    {
    //
    //    }
    if(m_placement==RELATIVE)
    {
//        this->SetPlacementToRelative(m_placement_relative_percentages[0],m_placement_relative_percentages[1]);
    }
    if(m_placement==LOWER_LEFT)
    {
//        this->SetPlacementToLowerLeft();
    }
    if(m_placement==LOWER_CENTER)
    {
//        this->SetPlacementToLowerCenter();
    }
    if(m_placement==LOWER_RIGHT)
    {
//        this->SetPlacementToLowerRight();
    }
    if(m_placement==CENTER_LEFT)
    {
//        this->SetPlacementToCenterLeft();
    }
    //    if(m_placement==CENTER_CENTER)
    //    {
    //
    //    }
    if(m_placement==CENTER_RIGHT)
    {
//        this->SetPlacementToCenterRight();
    }
    if(m_placement==UPPER_LEFT)
    {
//        this->SetPlacementToUpperLeft();
    }
    if(m_placement==UPPER_CENTER)
    {
//        this->SetPlacementToUpperCenter();
    }
    if(m_placement==UPPER_RIGHT)
    {
//        this->SetPlacementToUpperRight();
    }
}


void vmxTextInput::SetColor(double r, double g, double b)
{
    m_text_actor->GetTextProperty()->SetColor(r,g,b);
}


void vmxTextInput::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;
    
    //    m_text_actor->GetTextProperty()->SetFontFamilyToCourier();
    //
    //    m_text_actor->GetTextProperty()->SetFontFamilyToTimes();
    
    m_text_actor->GetTextProperty()->SetFontFamilyToArial();
    
    m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
}


void vmxTextInput::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
    
    m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
    m_interactor->AddObserver(vtkCommand::LeftButtonPressEvent, m_left_button_down_callback);
    m_interactor->AddObserver(vtkCommand::EndInteractionEvent, m_left_button_up_callback);
    
    m_interactor->AddObserver(vtkCommand::CharEvent, m_key_press_callback);

    
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_text_actor);
}


void vmxTextInput::SetOrigin(int origin1, int origin2)
{
    int text_actor_size[2];
    this->GetSize(text_actor_size[0], text_actor_size[1]);
    
    m_text_actor->SetPosition(origin1,origin2);
    m_placement = FREE;
}

//
//void vmxTextInput::SetPlacementToCenterLeft()
//{
//    this->SetPlacementToRelative(0,50);
//    m_placement = CENTER_LEFT;
//}
//
//
//void vmxTextInput::SetPlacementToCenterRight()
//{
//    this->SetPlacementToRelative(100,50);
//    m_placement = CENTER_RIGHT;
//}
//
//
//void vmxTextInput::SetPlacementToLowerCenter()
//{
//    this->SetPlacementToRelative(50,0);
//    m_placement = LOWER_CENTER;
//}
//
//
//void vmxTextInput::SetPlacementToLowerLeft()
//{
//    this->SetOrigin(0,0);
//    m_placement = LOWER_LEFT;
//}
//
//
//void vmxTextInput::SetPlacementToLowerRight()
//{
//    int *window_size = m_interactor->GetRenderWindow()->GetSize();
//    int menu_size[2];
//    this->GetSizeOfMenu(menu_size[0], menu_size[1]);
//    
//    int origin[2];
//    origin[0] = window_size[0] - menu_size[0];
//    origin[1] = 0;
//    
//    this->SetOrigin(origin[0],origin[1]);
//    m_placement = LOWER_RIGHT;
//}
//
//
//void vmxTextInput::SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent)
//{
//    m_placement_relative_percentages[0] = x_percent;
//    m_placement_relative_percentages[1] = y_percent;
//    
//    int *window_size = m_interactor->GetRenderWindow()->GetSize();
//    int origin[2];
//    origin[0] = (window_size[0] * x_percent) / 100;
//    origin[1] = (window_size[1] * y_percent) / 100;
//    
//    int menu_size[2];
//    this->GetSizeOfMenu(menu_size[0], menu_size[1]);
//    
//    if(origin[0]+menu_size[0]>window_size[0])
//    {
//        origin[0] = window_size[0] - menu_size[0];
//    }
//    
//    if(origin[1]+menu_size[1]>window_size[1])
//    {
//        origin[1] = window_size[1] - menu_size[1];
//    }
//    
//    this->SetOrigin(origin[0],origin[1]);
//    m_placement = RELATIVE;
//}
//
//
//void vmxTextInput::SetPlacementToUpperCenter()
//{
//    this->SetPlacementToRelative(50,100);
//    m_placement = UPPER_CENTER;
//}
//
//
//void vmxTextInput::SetPlacementToUpperLeft()
//{
//    int *window_size = m_interactor->GetRenderWindow()->GetSize();
//    int menu_size[2];
//    this->GetSizeOfMenu(menu_size[0], menu_size[1]);
//    
//    int origin[2];
//    origin[0] = 0;
//    origin[1] = window_size[1] - menu_size[1];
//    
//    this->SetOrigin(origin[0],origin[1]);
//    m_placement = UPPER_LEFT;
//}
//
//
//void vmxTextInput::SetPlacementToUpperRight()
//{
//    int *window_size = m_interactor->GetRenderWindow()->GetSize();
//    int menu_size[2];
//    this->GetSizeOfMenu(menu_size[0], menu_size[1]);
//    
//    int origin[2];
//    origin[0] = window_size[0] - menu_size[0];
//    origin[1] = window_size[1] - menu_size[1];
//    
//    this->SetOrigin(origin[0],origin[1]);
//    m_placement = UPPER_RIGHT;
//}


void vmxTextInput::SetVisibility(int is_visible)
{
    this->m_text_actor->SetVisibility(is_visible);
}


int vmxTextInput::ShowInputText()
{
    //cout<<"#";
    
    mxString input_text;
    input_text.Assign(m_description);
    for(unsigned int i=0; i<m_number_of_entered_characters; i++)
    {
        input_text.Append(m_input[i]);
    }
    
    m_text_actor->SetInput(input_text.Get_C_String());//"Input: ");
    
    return 1;
}








