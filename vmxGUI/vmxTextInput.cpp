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





//-----------------------------------------------------------------------------------------------------



//vmxTextInputInteractorLeftButtonDownCallback::vmxTextInputInteractorLeftButtonDownCallback()
//{
//    m_text_input = NULL;
//}
//
//
//vmxTextInputInteractorLeftButtonDownCallback::~vmxTextInputInteractorLeftButtonDownCallback()
//{
//    m_text_input = NULL;
//}
//
//
//void vmxTextInputInteractorLeftButtonDownCallback::SetTextInput(vmxTextInput *text_input)
//{
//    m_text_input = text_input;
//}
//
//
//vmxTextInputInteractorLeftButtonDownCallback* vmxTextInputInteractorLeftButtonDownCallback::New()
//{
//    return new vmxTextInputInteractorLeftButtonDownCallback;
//}
//
//
//void vmxTextInputInteractorLeftButtonDownCallback::Execute(vtkObject *caller, unsigned long, void *)
//{
//    cout<<" down ";
//    if(m_text_input)
//    {
//        if(m_text_input->m_interactor)
//        {
//            int pick_pos[2];
//            m_text_input->m_interactor->GetEventPosition(pick_pos);
//            
//            if(m_text_input->IsPicked(pick_pos[0], pick_pos[1]))
//            {
//                m_text_input->SetActive(1);
//            }
//            else
//            {
//                m_text_input->SetActive(0);
//            }
//        }
//    }
//}
//


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
                // if the data on the clipboard is valid...
                if(m_text_input->m_clip_board->m_is_valid)
                {
                    // ...and the input type is item input...
                    if(m_text_input->m_input_type == vmxTextInput::ITEM)
                    {
                        // ...copy it to the text input.
                        m_text_input->SetInputText(m_text_input->m_clip_board->m_data_text.Get_C_String());
                    }
                }
                else
                {
                    // If the data on clipboard is not valid...
                    
                    // ...and if the key press is not active...
                    if(!m_text_input->m_is_keypress_active)
                    {
                        //...reset text input and activate key press event (allow user to enter ext from keyboard).
//                        m_text_input->m_input.Clear();
                        if(m_text_input->m_input_type == vmxTextInput::DOUBLE)
                        {
                            m_text_input->m_interactor->AddObserver(vtkCommand::CharEvent, m_text_input->m_key_press_callback_input_double);
                            m_text_input->m_is_keypress_active = 1;
                        }
                        if(m_text_input->m_input_type == vmxTextInput::INTEGER)
                        {
                            m_text_input->m_interactor->AddObserver(vtkCommand::CharEvent, m_text_input->m_key_press_callback_input_integer);
                            m_text_input->m_is_keypress_active = 1;
                        }
                        if(m_text_input->m_input_type == vmxTextInput::TEXT)
                        {
                            m_text_input->m_interactor->AddObserver(vtkCommand::CharEvent, m_text_input->m_key_press_callback_input_text);
                            m_text_input->m_is_keypress_active = 1;
                        }
                    }
                }
                m_text_input->ShowInputText();
                m_text_input->RepositionAfterResizing();
                m_text_input->m_interactor->Render();
            }
            else
            {
                // if the object is not picked, turn off key press event (stop user from entring text from keyboard).
                m_text_input->m_interactor->RemoveObservers(vtkCommand::CharEvent, m_text_input->m_key_press_callback_input_text);
                m_text_input->m_interactor->RemoveObservers(vtkCommand::CharEvent, m_text_input->m_key_press_callback_input_integer);
                m_text_input->m_interactor->RemoveObservers(vtkCommand::CharEvent, m_text_input->m_key_press_callback_input_text);
                m_text_input->m_is_keypress_active = 0;
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxIntInputInteractorKeyPressCallback::vmxIntInputInteractorKeyPressCallback()
{
    m_text_input = NULL;
}


vmxIntInputInteractorKeyPressCallback::~vmxIntInputInteractorKeyPressCallback()
{
    m_text_input = NULL;
}


void vmxIntInputInteractorKeyPressCallback::SetTextInput(vmxTextInput *text_input)
{
    m_text_input = text_input;
}


vmxIntInputInteractorKeyPressCallback* vmxIntInputInteractorKeyPressCallback::New()
{
    return new vmxIntInputInteractorKeyPressCallback;
}


void vmxIntInputInteractorKeyPressCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_text_input)
    {
        if(m_text_input->m_interactor)
        {
            mxString key;
            key.Assign(m_text_input->m_interactor->GetKeySym());
            char c = m_text_input->m_interactor->GetKeyCode();
            
            for(int execute_once = 1; execute_once; execute_once = 0)
            {
                if(key=="Backspace")
                {
                    if(m_text_input->m_input.GetNumberOfCharacters()>0)
                    {
                        m_text_input->m_input.GetStdString().pop_back();
                        m_text_input->ShowInputText();
                        m_text_input->RepositionAfterResizing();
                        m_text_input->m_interactor->Render();
                    }
                    break;
                }
                
                // If a natural number is pressed...
                if(c>='0' && c<='9')
                {
                    m_text_input->m_input.Append(c);
                    m_text_input->ShowInputText();
                    m_text_input->RepositionAfterResizing();
                    m_text_input->m_interactor->Render();
                    break;
                }
                
                if(c=='+' || c=='-')
                {
                    if(m_text_input->m_input.GetNumberOfCharacters()==0)
                    {
                        m_text_input->m_input.Append(c);
                        m_text_input->ShowInputText();
                        m_text_input->RepositionAfterResizing();
                        m_text_input->m_interactor->Render();
                    }
                    break;
                }
            }
        }
    }
}




//-----------------------------------------------------------------------------------------------------



vmxDoubleInputInteractorKeyPressCallback::vmxDoubleInputInteractorKeyPressCallback()
{
    m_text_input = NULL;
}


vmxDoubleInputInteractorKeyPressCallback::~vmxDoubleInputInteractorKeyPressCallback()
{
    m_text_input = NULL;
}


void vmxDoubleInputInteractorKeyPressCallback::SetTextInput(vmxTextInput *text_input)
{
    m_text_input = text_input;
}


vmxDoubleInputInteractorKeyPressCallback* vmxDoubleInputInteractorKeyPressCallback::New()
{
    return new vmxDoubleInputInteractorKeyPressCallback;
}


void vmxDoubleInputInteractorKeyPressCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_text_input)
    {
        if(m_text_input->m_interactor)
        {
            mxString key;
            key.Assign(m_text_input->m_interactor->GetKeySym());
            char c = m_text_input->m_interactor->GetKeyCode();
            
            for(int execute_once = 1; execute_once; execute_once = 0)
            {
                if(key=="Backspace")
                {
                    if(m_text_input->m_input.GetNumberOfCharacters()>0)
                    {
                        m_text_input->m_input.GetStdString().pop_back();
                        m_text_input->ShowInputText();
                        m_text_input->RepositionAfterResizing();
                        m_text_input->m_interactor->Render();
                    }
                    break;
                }
                
                // If a natural number is pressed...
                if(c>='0' && c<='9')
                {
                    m_text_input->m_input.Append(c);
                    m_text_input->ShowInputText();
                    m_text_input->RepositionAfterResizing();
                    m_text_input->m_interactor->Render();
                    break;
                }
                if(c=='+' || c=='-')
                {
                    if(m_text_input->m_input.GetNumberOfCharacters()==0)
                    {
                        m_text_input->m_input.Append(c);
                        m_text_input->ShowInputText();
                        m_text_input->RepositionAfterResizing();
                        m_text_input->m_interactor->Render();
                    }
                    break;
                }
                if(c=='.')
                {
                    if(m_text_input->m_input.GetNumberOfCharacters()>0)
                    {
                        if(m_text_input->m_input[0]=='+' || m_text_input->m_input[0]=='-')
                        {
                            if(m_text_input->m_input.GetNumberOfCharacters()>1)
                            {
                                if(!m_text_input->IsDotEntered())
                                {
                                    m_text_input->m_input.Append(c);
                                    m_text_input->ShowInputText();
                                    m_text_input->RepositionAfterResizing();
                                    m_text_input->m_interactor->Render();
                                }
                            }
                        }
                        else
                        {
                            if(!m_text_input->IsDotEntered())
                            {
                                m_text_input->m_input.Append(c);
                                m_text_input->ShowInputText();
                                m_text_input->RepositionAfterResizing();
                                m_text_input->m_interactor->Render();
                            }
                        }
                    }
                    break;
                }
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
    //cout<<" key ";
    if(m_text_input)
    {
        if(m_text_input->m_interactor)
        {
            mxString key;
            key.Assign(m_text_input->m_interactor->GetKeySym());
            char c = m_text_input->m_interactor->GetKeyCode();
            
            cout<<key;
            
            for(int execute_once = 1; execute_once; execute_once = 0)
            {
                if(key=="Backspace")
                {
                    if(m_text_input->m_input.GetNumberOfCharacters()>0)
                    {
                        m_text_input->m_input.GetStdString().pop_back();
                        m_text_input->ShowInputText();
                        m_text_input->RepositionAfterResizing();
                        m_text_input->m_interactor->Render();
                    }
                    break;
                }
                
                if(key=="Left" || key=="Right" || key=="Up" || key=="Down")
                {
                    break;
                }
                
                // if any other key is pressed...
                {
                    m_text_input->m_input.Append(c);
                    m_text_input->ShowInputText();
                    m_text_input->RepositionAfterResizing();
                    m_text_input->m_interactor->Render();
                }
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxTextInput::vmxTextInput()
{
    m_clip_board = NULL;
    
    m_input_type = vmxTextInput::TEXT;
    
    m_class_name.Assign("vmxTextInput");
    
//    m_left_button_down_callback = vtkSmartPointer<vmxTextInputInteractorLeftButtonDownCallback>::New();
//    m_left_button_down_callback->SetTextInput(this);
    m_key_press_callback_input_text = vtkSmartPointer<vmxTextInputInteractorKeyPressCallback>::New();
    m_key_press_callback_input_text->SetTextInput(this);
    m_left_button_up_callback = vtkSmartPointer<vmxTextInputInteractorLeftButtonUpCallback>::New();
    m_left_button_up_callback->SetTextInput(this);
    m_key_press_callback_input_double = vtkSmartPointer<vmxDoubleInputInteractorKeyPressCallback>::New();
    m_key_press_callback_input_double->SetTextInput(this);
    m_key_press_callback_input_integer = vtkSmartPointer<vmxIntInputInteractorKeyPressCallback>::New();
    m_key_press_callback_input_integer->SetTextInput(this);

    m_is_keypress_active = 0;
    
    
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


void vmxTextInput::GetOrigin(int &origin1, int &origin2)
{
    double *org = m_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    //return 1;
}


void vmxTextInput::GetSize(int &output_size1, int &output_size2)
{
    if(!m_interactor) return;// 0;
    
    double size[2];
    m_text_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
    
    //return 1;
}


int vmxTextInput::IsDotEntered()
{
    for(unsigned int i=0; i<m_input.GetNumberOfCharacters(); i++)
    {
        if(m_input[i]=='.') { return 1; }
    }
    return 0;
}


//int vmxTextInput::IsPicked(int pos1, int pos2)
//{
//    int size[2];
//    this->GetSize(size[0],size[1]);
//    
//    int origin[2];
//    this->GetOrigin(origin[0],origin[1]);
//    
//    int end_pos1 = origin[0] + size[0];
//    if(pos1<origin[0] || pos1>end_pos1) return 0;
//    
//    int end_pos2 = origin[1] + size[1];
//    if(pos2<origin[1] || pos2>end_pos2) return 0;
//    
//    return 1;
//}


int vmxTextInput::IsVisible()
{
    return this->m_text_actor->GetVisibility();
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


void vmxTextInput::SetInputTypeToDoubleNumber(double default_value)
{
    m_input.AssignNumber(default_value);
    m_input_type = vmxTextInput::DOUBLE;
}


void vmxTextInput::SetInputTypeToIntegerNumber(int default_value)
{
    m_input.AssignNumber(default_value);
    m_input_type = vmxTextInput::INTEGER;
}


void vmxTextInput::SetInputTypeToItem()
{
    m_input_type = vmxTextInput::ITEM;
}


void vmxTextInput::SetInputTypeToText(const char *default_text)
{
    m_input.Assign(default_text);
    m_input_type = vmxTextInput::TEXT;
}


void vmxTextInput::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
    
    m_interactor->AddObserver(vtkCommand::EndInteractionEvent, m_left_button_up_callback);
    
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_text_actor);
}


void vmxTextInput::SetOrigin(int origin1, int origin2)
{
    int text_actor_size[2];
    this->GetSize(text_actor_size[0], text_actor_size[1]);
    
    m_text_actor->SetPosition(origin1,origin2);
    m_placement = FIXED;
}


void vmxTextInput::SetVisibility(int is_visible)
{
    this->ShowInputText();
    this->m_text_actor->SetVisibility(is_visible);
    this->RedoPlacement();
    //vmxGUIObject::SetVisibility(is_visible);
}


int vmxTextInput::ShowInputText()
{
    mxString input_text;
    input_text.Assign(m_description);
    input_text.Append(m_input);
    
    m_text_actor->SetInput(input_text.Get_C_String());
    
    return 1;
}








