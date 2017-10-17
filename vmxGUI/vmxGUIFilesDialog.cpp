/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIFilesDialog.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIFilesDialog.h"



vmxGUIFilesDialogRenderWindowModifiedCallback::vmxGUIFilesDialogRenderWindowModifiedCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
    m_previous_window_size[0] = m_previous_window_size[1] = 0;
}


vmxGUIFilesDialogRenderWindowModifiedCallback::~vmxGUIFilesDialogRenderWindowModifiedCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
}


void vmxGUIFilesDialogRenderWindowModifiedCallback::SetDialog(vmxGUIFilesDialog *dialog)
{
    m_is_active = 0;
    m_dialog = dialog;
}


vmxGUIFilesDialogRenderWindowModifiedCallback* vmxGUIFilesDialogRenderWindowModifiedCallback::New()
{
    return new vmxGUIFilesDialogRenderWindowModifiedCallback;
}


void vmxGUIFilesDialogRenderWindowModifiedCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_dialog->m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, this);
    }
    else
    {
        if(!m_is_active) return;
                m_dialog->m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIFilesDialogRenderWindowModifiedCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_dialog)
    {
        if(m_dialog->m_interactor)
        {
            //cout<<"*";
            int *size = m_dialog->m_interactor->GetRenderWindow()->GetSize();
            if(size[0]!=m_previous_window_size[0] || size[1]!=m_previous_window_size[1])
            {
                //cout<<"-";
                m_previous_window_size[0] = size[0];
                m_previous_window_size[1] = size[1];
                m_dialog->RedoPlacement();
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIFilesDialogInteractorLeftButtonDownCallback::vmxGUIFilesDialogInteractorLeftButtonDownCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
}


vmxGUIFilesDialogInteractorLeftButtonDownCallback::~vmxGUIFilesDialogInteractorLeftButtonDownCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
}


void vmxGUIFilesDialogInteractorLeftButtonDownCallback::SetDialog(vmxGUIFilesDialog *dialog)
{
    m_is_active = 0;
    m_dialog = dialog;
}


vmxGUIFilesDialogInteractorLeftButtonDownCallback* vmxGUIFilesDialogInteractorLeftButtonDownCallback::New()
{
    return new vmxGUIFilesDialogInteractorLeftButtonDownCallback;
}


void vmxGUIFilesDialogInteractorLeftButtonDownCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_dialog->m_interactor->AddObserver(vtkCommand::LeftButtonPressEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_dialog->m_interactor->RemoveObservers(vtkCommand::LeftButtonPressEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIFilesDialogInteractorLeftButtonDownCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_dialog)
    {
        if(m_dialog->m_interactor)
        {
            //cout<<endl<<" down ";
            
            if(m_dialog->m_clip_board) m_dialog->m_clip_board->m_is_valid = 0;
            
            
            int pick_pos[2];
            m_dialog->m_interactor->GetEventPosition(pick_pos);
            
            m_dialog->m_double_click_detector.OnLeftButtonDown(pick_pos[0], pick_pos[1],m_dialog->m_interactor->GetControlKey());
 
            int index = m_dialog->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(index<0)
            {
                m_dialog->ListenForKeyPressEvent(0);
                return;
            }
            
            m_dialog->m_item_selection_detector.OnLeftButtonDown(m_dialog->m_interactor->GetControlKey(), m_dialog->m_item_pointers[index]->m_is_selected, m_dialog->IsMultipleItemsSelected());

            
            // If double click was performed on an item representing a folder, open that folder
            if(m_dialog->m_double_click_detector.IsDoubleClicked())
            {
                vmxGUIFilesDialogItem *item = m_dialog->GetItem(index);
                if(item->m_text[item->m_text.GetNumberOfCharacters()-1]==mxString::PathSeparator())//if this item is a folder
                {
                    mxString name;
                    name.Assign(m_dialog->m_path_directory);
                    if(item->m_text=="../")
                    {
                        mxString name2;
                        name2.Assign(m_dialog->m_path_directory);
                        if(!name2.PathLevelUp(name))
                        {
                            //cout<<" name="<<name<<endl;
                            return;
                        }
                        else
                        {
                            //cout<<" name="<<name<<endl;
                            m_dialog->m_path_directory.Assign(name);
                        }
                    }
                    else
                    {
                        //cout<<" name="<<name<<endl;
                        name.Append(item->m_text);
                    }
                    m_dialog->OpenFiles(name.Get_C_String());
                    m_dialog->RedoPlacement();
                    //cout<<" "<<name.Get_C_String()<<" ";
                }
                
                //cout<<" "<<item->m_text.Get_C_String()<<" ";
                return;
            }
            
            
            if(m_dialog->m_item_selection_detector.IsMultipleSelectionDetected())
            {
                // if multiple selection is detected (CTRL key is pressed), jus add/remove items from selection
                m_dialog->GetItem(index)->m_is_selected = !(m_dialog->GetItem(index)->m_is_selected);
            }
            else
            {
                // If CTRL is not pressed, we need to check if a single selection is performed or a drag event of multiple selected items.
                //if(m_dialog->m_item_selection_detector.IsSingleSelectionDetected())
                //{
                    // If there is no drag for sure, just perform single selection
                    m_dialog->SelectSingleItem(index);
                //}
            }
            
            m_dialog->ListenForKeyPressEvent(1);
            
            m_dialog->ShowTextItems(m_dialog->m_text_index_offset);//m_dialog->ShowSelectedItems();
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIFilesDialogInteractorMouseWheelBackwardCallback::vmxGUIFilesDialogInteractorMouseWheelBackwardCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
}


vmxGUIFilesDialogInteractorMouseWheelBackwardCallback::~vmxGUIFilesDialogInteractorMouseWheelBackwardCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
}


void vmxGUIFilesDialogInteractorMouseWheelBackwardCallback::SetDialog(vmxGUIFilesDialog *dialog)
{
    m_is_active = 0;
    m_dialog = dialog;
}


vmxGUIFilesDialogInteractorMouseWheelBackwardCallback* vmxGUIFilesDialogInteractorMouseWheelBackwardCallback::New()
{
    return new vmxGUIFilesDialogInteractorMouseWheelBackwardCallback;
}


void vmxGUIFilesDialogInteractorMouseWheelBackwardCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_dialog->m_interactor->AddObserver(vtkCommand::MouseWheelBackwardEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_dialog->m_interactor->RemoveObservers(vtkCommand::MouseWheelBackwardEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIFilesDialogInteractorMouseWheelBackwardCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_dialog)
    {
        if(m_dialog->m_interactor)
        {
            //cout<<" wheel_backward ";
            
            int pick_pos[2];
            m_dialog->m_interactor->GetEventPosition(pick_pos);
            
            int index = m_dialog->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(index<0) return;
            
            int step = 4;
            
            m_dialog->ShowTextItems(m_dialog->m_text_index_offset+step);
            m_dialog->RedoPlacement();
            
            m_dialog->m_interactor->Render();
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIFilesDialogInteractorMouseWheelForwardCallback::vmxGUIFilesDialogInteractorMouseWheelForwardCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
}


vmxGUIFilesDialogInteractorMouseWheelForwardCallback::~vmxGUIFilesDialogInteractorMouseWheelForwardCallback()
{
    m_is_active = 0;
    m_dialog = NULL;
}


void vmxGUIFilesDialogInteractorMouseWheelForwardCallback::SetDialog(vmxGUIFilesDialog *dialog)
{
    m_is_active = 0;
    m_dialog = dialog;
}


vmxGUIFilesDialogInteractorMouseWheelForwardCallback* vmxGUIFilesDialogInteractorMouseWheelForwardCallback::New()
{
    return new vmxGUIFilesDialogInteractorMouseWheelForwardCallback;
}


void vmxGUIFilesDialogInteractorMouseWheelForwardCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_dialog->m_interactor->AddObserver(vtkCommand::MouseWheelForwardEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_dialog->m_interactor->RemoveObservers(vtkCommand::MouseWheelForwardEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIFilesDialogInteractorMouseWheelForwardCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_dialog)
    {
        if(m_dialog->m_interactor)
        {
            //cout<<" wheel_forward ";
            
            int pick_pos[2];
            m_dialog->m_interactor->GetEventPosition(pick_pos);
            
            int index = m_dialog->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(index<0) return;
            
            int step = 4;
            
            m_dialog->ShowTextItems(m_dialog->m_text_index_offset-step);
            m_dialog->RedoPlacement();
            
            m_dialog->m_interactor->Render();
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIFilesDialogInteractorKeyPressCallback::vmxGUIFilesDialogInteractorKeyPressCallback()
{
    m_dialog = NULL;
    m_is_active = 0;
}


vmxGUIFilesDialogInteractorKeyPressCallback::~vmxGUIFilesDialogInteractorKeyPressCallback()
{
    m_dialog = NULL;
    m_is_active = 0;
}


void vmxGUIFilesDialogInteractorKeyPressCallback::SetDialog(vmxGUIFilesDialog *dialog)
{
    m_dialog = dialog;
    m_is_active = 0;
}


vmxGUIFilesDialogInteractorKeyPressCallback* vmxGUIFilesDialogInteractorKeyPressCallback::New()
{
    return new vmxGUIFilesDialogInteractorKeyPressCallback;
}


void vmxGUIFilesDialogInteractorKeyPressCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_dialog->m_interactor->AddObserver(vtkCommand::KeyPressEvent, this);//m_dialog->m_key_press_callback);
        m_is_active = is_active;
    }
    else
    {
        if(!m_is_active) return;
        m_dialog->m_interactor->RemoveObservers(vtkCommand::KeyPressEvent, this);//m_dialog->m_key_press_callback);
        m_is_active = is_active;
    }
}


void vmxGUIFilesDialogInteractorKeyPressCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    //cout<<" key ";
    if(m_dialog)
    {
        if(m_dialog->m_interactor)
        {
            mxString key;
            key.Assign(m_dialog->m_interactor->GetKeySym());
            //char c = key[0];
            
            if(key=="Return")
            {
                cout<<"Return";
            }
            else
            {
                if(key=="Escape")
                {
                    cout<<"Escape";
                }
            }
        }
    }
}





//-----------------------------------------------------------------------------------------------------



vmxGUIFilesDialogItem::vmxGUIFilesDialogItem()
{
    m_is_selected = 0;
    m_dialog = NULL;
}


vmxGUIFilesDialogItem::~vmxGUIFilesDialogItem()
{
}



//-----------------------------------------------------------------------------------------------------



vmxGUIFilesDialog::vmxGUIFilesDialog()
{
    m_clip_board = NULL;
    
    
    m_placement = FREE;
    m_placement_relative_percentages[0] = m_placement_relative_percentages[1] = 0;
    
    m_selection_type = OPEN_FILES;

    m_window_modified_callback = vtkSmartPointer<vmxGUIFilesDialogRenderWindowModifiedCallback>::New();
    m_window_modified_callback->SetDialog(this);
    m_left_button_down_callback = vtkSmartPointer<vmxGUIFilesDialogInteractorLeftButtonDownCallback>::New();
    m_left_button_down_callback->SetDialog(this);
    m_mouse_wheel_backward_callback = vtkSmartPointer<vmxGUIFilesDialogInteractorMouseWheelBackwardCallback>::New();
    m_mouse_wheel_backward_callback->SetDialog(this);
    m_mouse_wheel_forward_callback = vtkSmartPointer<vmxGUIFilesDialogInteractorMouseWheelForwardCallback>::New();
    m_mouse_wheel_forward_callback->SetDialog(this);
    m_key_press_callback = vtkSmartPointer<vmxGUIFilesDialogInteractorKeyPressCallback>::New();
    m_key_press_callback->SetDialog(this);

    

    m_interactor = NULL;
    m_font_size = 18;
    m_text_index_offset = 0;
    m_text_actor_max_number_of_items = 50;
    m_number_of_items_in_text_actor = 0;
    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_user_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_path_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxGUIFilesDialog::~vmxGUIFilesDialog()
{
}


vmxGUIFilesDialog* vmxGUIFilesDialog::New()
{
    return new vmxGUIFilesDialog;
}


vmxGUIFilesDialogItem* vmxGUIFilesDialog::AddItem(const char *item_text)
{
    if(!item_text) return NULL;
    vmxGUIFilesDialogItem *item = this->m_items.AddNewToEnd();
    item->m_dialog = this;
    item->m_text.Assign(item_text);
    return item;
}


void vmxGUIFilesDialog::BuildDialog()
{
    m_item_pointers.Reset();
    m_item_pointers.SetNumberOfElements(m_items.GetNumberOfElements());
    
    mxString text;
    
    int i = m_item_pointers.GetNumberOfElements()-1;
    mxListIterator<vmxGUIFilesDialogItem> it;
    for(it.SetToEnd(this->m_items), i = m_item_pointers.GetNumberOfElements()-1; it.IsValid() && i>=0; it.MoveToPrevious(), i--)
    {
        m_item_pointers[i] = it.GetElementAddress();
    }
    
    this->ShowTextItems();
    m_user_text_actor->SetInput("Open File(s):");
    m_path_actor->SetInput("Path:");

}



void vmxGUIFilesDialog::GetColor(double &r, double &g, double &b)
{
    m_text_actor->GetTextProperty()->GetColor(r,g,b);
}


int vmxGUIFilesDialog::GetOriginOfDialog(int &origin1, int &origin2)
{
    //currently the text actor is the first one in the dialog
    return this->GetOriginOfTextActor(origin1, origin2);
}


int vmxGUIFilesDialog::GetOriginOfTextActor(int &origin1, int &origin2)
{
    double *org = m_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    return 1;
}


int vmxGUIFilesDialog::GetPickedItemIndex(int pos1, int pos2)
{
    if(!this->IsDialogPicked(pos1,pos2)) return -1;
    
    int dialog_size[2];
    this->GetSizeOfTextActor(dialog_size[0], dialog_size[1]);
    
    int dialog_origin[2];
    this->GetOriginOfTextActor(dialog_origin[0], dialog_origin[1]);
    
    int item_index;
    double a1 = (pos2-dialog_origin[1]);
    if(a1<0) return -1;
    
    int number_of_items_in_text_actor = m_number_of_items_in_text_actor;

    double a2 = ((double)dialog_size[1])/((double)number_of_items_in_text_actor);
    item_index = a1/a2;
    item_index += this->GetNumberOfItems()-number_of_items_in_text_actor-m_text_index_offset;
    
//    cout<<endl;
//    cout<<"  "<<"pick_pos="<<pos1<<","<<pos2;
//    cout<<"  "<<"dialog_origin="<<dialog_origin[0]<<","<<dialog_origin[1];
//    //cout<<"  "<<"pos2-orig[1]="<<pos2-dialog_origin[1];
//    cout<<"  "<<"a1="<<a1;
//    cout<<"  "<<"a2="<<a2;
//    cout<<"  "<<"a1/a2="<<a1/a2;
//    cout<<"  "<<"dialog_size="<<dialog_size[0]<<","<<dialog_size[1];
//    cout<<"  "<<"number_of_items_in_text_actor="<<(number_of_items_in_text_actor);
//    cout<<"  "<<"total_number_of_items="<<(this->GetNumberOfItems());
//    cout<<"  "<<"item_index="<<item_index;
//    cout<<endl;
    
    if(item_index<0 || item_index>=this->GetNumberOfItems()) return -1;
    return item_index;
}


int vmxGUIFilesDialog::GetSizeOfDialog(int &output_size1, int &output_size2)
{
    int size_of_text_actor[2];
    int size_of_user_text_actor[2];
    int size_of_path_actor[2];
    
    if(!this->GetSizeOfTextActor(size_of_text_actor[0], size_of_text_actor[1])) return 0;
    if(!this->GetSizeOfUserTextActor(size_of_user_text_actor[0], size_of_user_text_actor[1])) return 0;
    if(!this->GetSizeOfPathActor(size_of_path_actor[0], size_of_path_actor[1])) return 0;

    
    
    output_size1 = size_of_text_actor[0];
    if(output_size1<size_of_user_text_actor[0]) output_size1 = size_of_user_text_actor[0];
    if(output_size1<size_of_path_actor[0]) output_size1 = size_of_path_actor[0];
    output_size2 = size_of_text_actor[1] + size_of_user_text_actor[1] + size_of_path_actor[1];
    return 1;
}


int vmxGUIFilesDialog::GetSizeOfTextActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_text_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    output_size1 = size[0];
    output_size2 = size[1];
    
    return 1;
}


int vmxGUIFilesDialog::GetSizeOfUserTextActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_user_text_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    output_size1 = size[0];
    output_size2 = size[1];
    
    return 1;
}


int vmxGUIFilesDialog::GetSizeOfPathActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_path_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    output_size1 = size[0];
    output_size2 = size[1];
    
    return 1;
}


int vmxGUIFilesDialog::GetRenderWindowAvailableSize(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    int *window_size = m_interactor->GetRenderWindow()->GetSize();
    output_size1 = window_size[0];
    output_size2 = window_size[1];
    return 1;
}


int vmxGUIFilesDialog::GetVisibility()
{
    return m_text_actor->GetVisibility();
}


int vmxGUIFilesDialog::IsDialogPicked(int pos1, int pos2)
{
    int dialog_size[2];
    this->GetSizeOfDialog(dialog_size[0],dialog_size[1]);
    
    int dialog_origin[2];
    this->GetOriginOfDialog(dialog_origin[0],dialog_origin[1]);
    
    
    int end_pos1 = dialog_origin[0] + dialog_size[0];
    if(pos1<dialog_origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = dialog_origin[1] + dialog_size[1];
    if(pos2<dialog_origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


int vmxGUIFilesDialog::IsMultipleItemsSelected()
{
    int c = 0;
    for(unsigned int i=0; i<this->GetNumberOfItems(); i++)
    {
        if(m_item_pointers[i]->m_is_selected)
        {
            c++;
            if(c>1) return 1;
        }
    }
    return 0;
}


int vmxGUIFilesDialog::IsTextActorPicked(int pos1, int pos2)
{
    int actor_size[2];
    this->GetSizeOfTextActor(actor_size[0],actor_size[1]);
    
    int actor_origin[2];
    this->GetOriginOfTextActor(actor_origin[0],actor_origin[1]);
    
    
    int end_pos1 = actor_origin[0] + actor_size[0];
    if(pos1<actor_origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = actor_origin[1] + actor_size[1];
    if(pos2<actor_origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


void vmxGUIFilesDialog::ListenForLeftButtonDownEvent(int is_listening)
{
    m_left_button_down_callback->Activate(is_listening);
}


void vmxGUIFilesDialog::ListenForRenderWindowModifiedEvent(int is_listening)
{
    m_window_modified_callback->Activate(is_listening);
}


void vmxGUIFilesDialog::ListenForMouseWheelBackwardEvent(int is_listening)
{
    m_mouse_wheel_backward_callback->Activate(is_listening);
}


void vmxGUIFilesDialog::ListenForMouseWheelForwardEvent(int is_listening)
{
    m_mouse_wheel_forward_callback->Activate(is_listening);
}


void vmxGUIFilesDialog::ListenForKeyPressEvent(int is_listening)
{
    m_key_press_callback->Activate(is_listening);
}


void vmxGUIFilesDialog::RedoPlacement()
{
//    if(m_placement==FREE)
//    {
//        
//    }
    if(m_placement==RELATIVE)
    {
        this->SetPlacementToRelative(m_placement_relative_percentages[0],m_placement_relative_percentages[1]);
    }
    if(m_placement==LOWER_LEFT)
    {
        this->SetPlacementToLowerLeft();
    }
    if(m_placement==LOWER_CENTER)
    {
        this->SetPlacementToLowerCenter();
    }
    if(m_placement==LOWER_RIGHT)
    {
        this->SetPlacementToLowerRight();
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
//         this->SetPlacementToCenterRight();
    }
    if(m_placement==UPPER_LEFT)
    {
        this->SetPlacementToUpperLeft();
    }
    if(m_placement==UPPER_CENTER)
    {
        this->SetPlacementToUpperCenter();
    }
    if(m_placement==UPPER_RIGHT)
    {
        this->SetPlacementToUpperRight();
    }
}


void vmxGUIFilesDialog::SelectSingleItem(unsigned int item_index)
{
    if(item_index>=m_item_pointers.GetNumberOfElements()) return;
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        m_item_pointers[i]->m_is_selected = 0;
    }
    m_item_pointers[item_index]->m_is_selected = 1;
}


void vmxGUIFilesDialog::SetColor(double r, double g, double b)
{
    m_text_actor->GetTextProperty()->SetColor(r,g,b);
    m_user_text_actor->GetTextProperty()->SetColor(r,g,b);
    m_path_actor->GetTextProperty()->SetColor(r,g,b);
}


void vmxGUIFilesDialog::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;

//    m_text_actor->GetTextProperty()->SetFontFamilyToCourier();
//    m_user_text_actor->GetTextProperty()->SetFontFamilyToCourier();
//    m_path_actor->GetTextProperty()->SetFontFamilyToCourier();
//    
    
//    m_text_actor->GetTextProperty()->SetFontFamilyToTimes();
//    m_user_text_actor->GetTextProperty()->SetFontFamilyToTimes();
//    m_path_actor->GetTextProperty()->SetFontFamilyToTimes();
    
    m_text_actor->GetTextProperty()->SetFontFamilyToArial();
    m_user_text_actor->GetTextProperty()->SetFontFamilyToArial();
    m_path_actor->GetTextProperty()->SetFontFamilyToArial();
    
    m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    m_user_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    m_path_actor->GetTextProperty()->SetFontSize(m_font_size);
    


}


void vmxGUIFilesDialog::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
    
    this->ListenForRenderWindowModifiedEvent(1);
    this->ListenForLeftButtonDownEvent(1);
    
    //m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
    //m_interactor->AddObserver(vtkCommand::LeftButtonPressEvent, m_left_button_down_callback);

    
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_text_actor);
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_user_text_actor);
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_path_actor);
}


void vmxGUIFilesDialog::SetOrigin(int origin1, int origin2)
{
    int text_actor_size[2];
    this->GetSizeOfTextActor(text_actor_size[0], text_actor_size[1]);
    
    int path_actor_size[2];
    this->GetSizeOfPathActor(path_actor_size[0], path_actor_size[1]);


    m_text_actor->SetPosition(origin1,origin2);
    m_path_actor->SetPosition(origin1,origin2+text_actor_size[1]);
    m_user_text_actor->SetPosition(origin1,m_path_actor->GetPosition()[1]+path_actor_size[1]);
    
    m_placement = FREE;
}


//void vmxGUIFilesDialog::SetPlacementToCenterLeft()
//{
//    this->SetPlacementToRelative(0,50);
//    m_placement = CENTER_LEFT;
//}


//void vmxGUIFilesDialog::SetPlacementToCenterRight()
//{
//    this->SetPlacementToRelative(100,50);
//    m_placement = CENTER_RIGHT;
//}


void vmxGUIFilesDialog::SetPlacementToLowerCenter()
{
    this->SetPlacementToRelative(50,0);
    m_placement = LOWER_CENTER;
}


void vmxGUIFilesDialog::SetPlacementToLowerLeft()
{
    this->SetOrigin(0,0);
    m_placement = LOWER_LEFT;
}


void vmxGUIFilesDialog::SetPlacementToLowerRight()
{
    int *window_size = m_interactor->GetRenderWindow()->GetSize();
    int dialog_size[2];
    this->GetSizeOfDialog(dialog_size[0], dialog_size[1]);
    
    int origin[2];
    origin[0] = window_size[0] - dialog_size[0];
    origin[1] = 0;
    
    this->SetOrigin(origin[0],origin[1]);
    m_placement = LOWER_RIGHT;
}


void vmxGUIFilesDialog::SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent)
{
    m_placement_relative_percentages[0] = x_percent;
    m_placement_relative_percentages[1] = y_percent;
    
    int *window_size = m_interactor->GetRenderWindow()->GetSize();
    int origin[2];
    origin[0] = (window_size[0] * x_percent) / 100;
    origin[1] = (window_size[1] * y_percent) / 100;
    
    int dialog_size[2];
    this->GetSizeOfDialog(dialog_size[0], dialog_size[1]);

    if(origin[0]+dialog_size[0]>window_size[0])
    {
        origin[0] = window_size[0] - dialog_size[0];
    }
    
    if(origin[1]+dialog_size[1]>window_size[1])
    {
        origin[1] = window_size[1] - dialog_size[1];
    }

    this->SetOrigin(origin[0],origin[1]);
    m_placement = RELATIVE;
}


void vmxGUIFilesDialog::SetPlacementToUpperCenter()
{
    this->SetPlacementToRelative(50,100);
    m_placement = UPPER_CENTER;
}


void vmxGUIFilesDialog::SetPlacementToUpperLeft()
{
    int *window_size = m_interactor->GetRenderWindow()->GetSize();
    int dialog_size[2];
    this->GetSizeOfDialog(dialog_size[0], dialog_size[1]);

    int origin[2];
    origin[0] = 0;
    origin[1] = window_size[1] - dialog_size[1];
    
    this->SetOrigin(origin[0],origin[1]);
    m_placement = UPPER_LEFT;
}


void vmxGUIFilesDialog::SetPlacementToUpperRight()
{
    int *window_size = m_interactor->GetRenderWindow()->GetSize();
    int dialog_size[2];
    this->GetSizeOfDialog(dialog_size[0], dialog_size[1]);
    
    int origin[2];
    origin[0] = window_size[0] - dialog_size[0];
    origin[1] = window_size[1] - dialog_size[1];
    
    this->SetOrigin(origin[0],origin[1]);
    m_placement = UPPER_RIGHT;
}


void vmxGUIFilesDialog::SetVisibility(int is_visible)
{
    if(this->GetVisibility() && is_visible) return;
    if(!this->GetVisibility() && !is_visible) return;
    
    this->m_text_actor->SetVisibility(is_visible);
    this->m_user_text_actor->SetVisibility(is_visible);
    this->m_path_actor->SetVisibility(is_visible);
    
    // If the dialog is not visible, it should not observe interaction events
    if(!is_visible)
    {
        this->ListenForRenderWindowModifiedEvent(0);
        this->ListenForLeftButtonDownEvent(0);
    }
    else
    {
        this->ListenForRenderWindowModifiedEvent(1);
        this->ListenForLeftButtonDownEvent(1);
    }
}


int vmxGUIFilesDialog::ShowTextItems(int offset)
{
    if(offset>this->GetNumberOfItems()-2)
    {
        m_text_index_offset = this->GetNumberOfItems()-2;
        if(m_text_index_offset<0) m_text_index_offset = 0;
    }
    else
    {
        if(offset<0)
        {
            m_text_index_offset = 0;
        }
        else
        {
            m_text_index_offset = offset;
        }
    }
    
    mxString text;
    
    m_number_of_items_in_text_actor = 0;
    int m = 0;
    //for(int i=this->GetNumberOfItems()-1-m_text_index_offset; i>=0; i--)
    for(int i=this->GetNumberOfItems()-1-m_text_index_offset; m<m_text_actor_max_number_of_items && i>=0; i--, m++)
    {
        m_number_of_items_in_text_actor++;
        
        text.Append(" ");
        if(m_item_pointers[i]->m_is_selected)
        {
            text.Append(">");
        }
        else
        {
            text.Append(" ");
        }
        
        text.Append(m_item_pointers[i]->m_text);
        
        if(i!=0 && m!=m_text_actor_max_number_of_items-1)
        {
            text.Append("\n");//selected_text.Append("_\n");
        }
    }
   
    m_text_actor->SetInput(text.Get_C_String());
    
    return 1;
}



int vmxGUIFilesDialog::OpenFiles(const char *directory_path)
{
   // m_selection_type = OPEN_FILES;
    
    mxString path_directory;
    
    if(!directory_path)
    {
        char *home;
#if defined(_MSC_VER)
        home = getenv("USERPROFILE");// for Microsoft Windows use environment variable: USERPROFILE
#else
        home = getenv("HOME");// for Unix-type systems use environment variable: HOME
#endif
        path_directory.Assign(home);
    }
    else
    {
        path_directory.Assign(directory_path);
    }
    
    if(path_directory[(path_directory.GetNumberOfCharacters()-1)]!=mxString::PathSeparator())
    path_directory.Append(mxString::PathSeparator());
    
    //cout<<home;
    
    
    tinydir_dir dir;
    if(tinydir_open_sorted(&dir,path_directory.Get_C_String()) == -1)
    {
        cout<<"vmxGUIFilesDialog::Start(): Error opening file";
        tinydir_close(&dir);
        return 0;
    }
    
    this->m_items.Reset();
    m_path_directory.Assign(path_directory);
    m_text_index_offset = 0;
    
  
    for(int i=dir.n_files-1; i>=0; i--)
    {
        tinydir_file file;
        if(tinydir_readfile_n(&dir, &file, i) == -1)
        {
            cout<<endl<<"vmxGUIFilesDialog::Start(): Error getting file";
            tinydir_close(&dir);
            return 0;
        }
        else
        {
        
            if(file.is_dir)
            {
                mxString test;
                test.Assign(file.name);
                if(test != "." && test != "..")
                {
                    mxString name;
                    //name.Assign("<DIR> ");
                    name.Append(file.name);
                    name.Append(mxString::PathSeparator());
                    this->AddItem(name.Get_C_String());
                }
            }
            else
            {
                this->AddItem(file.name);
            }
            //cout<<file.path<<endl;
        }
    }
    
    this->AddItem("../");

    this->BuildDialog();
    
    mxString path_text;
    path_text.Assign("Path: ");
    path_text.Append(m_path_directory);
    this->m_path_actor->SetInput(path_text.Get_C_String());
    
    if(m_number_of_items_in_text_actor<this->GetNumberOfItems())
    {
        this->ListenForMouseWheelBackwardEvent(1);
        this->ListenForMouseWheelForwardEvent(1);
    }
    else
    {
        this->ListenForMouseWheelBackwardEvent(0);
        this->ListenForMouseWheelForwardEvent(0);
    }

    return 1;
}







