/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIMenu.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIMenu.h"



//vmxGUIMenuRenderWindowModifiedCallback::vmxGUIMenuRenderWindowModifiedCallback()
//{
//    m_menu = NULL;
//    m_previous_window_size[0] = m_previous_window_size[1] = 0;
//}
//
//
//vmxGUIMenuRenderWindowModifiedCallback::~vmxGUIMenuRenderWindowModifiedCallback()
//{
//    m_menu = NULL;
//}
//
//
//void vmxGUIMenuRenderWindowModifiedCallback::SetMenu(vmxGUIMenu *menu)
//{
//    m_menu = menu;
//}
//
//
//vmxGUIMenuRenderWindowModifiedCallback* vmxGUIMenuRenderWindowModifiedCallback::New()
//{
//    return new vmxGUIMenuRenderWindowModifiedCallback;
//}
//
//
//void vmxGUIMenuRenderWindowModifiedCallback::Execute(vtkObject *caller, unsigned long, void *)
//{
//    if(m_menu)
//    {
//        if(m_menu->m_interactor)
//        {
//            //cout<<"*";
//            int *size = m_menu->m_interactor->GetRenderWindow()->GetSize();
//            if(size[0]!=m_previous_window_size[0] || size[1]!=m_previous_window_size[1])
//            {
//                //cout<<"-";
//                m_previous_window_size[0] = size[0];
//                m_previous_window_size[1] = size[1];
//                m_menu->RedoPlacement();
//            }
//        }
//    }
//}



//-----------------------------------------------------------------------------------------------------



vmxGUIMenuInteractorLeftButtonDownCallback::vmxGUIMenuInteractorLeftButtonDownCallback()
{
    m_menu = NULL;
    m_previous_left_button_down_position[0] = m_previous_left_button_down_position[1] = 0;
}


vmxGUIMenuInteractorLeftButtonDownCallback::~vmxGUIMenuInteractorLeftButtonDownCallback()
{
    m_menu = NULL;
}


void vmxGUIMenuInteractorLeftButtonDownCallback::SetMenu(vmxGUIMenu *menu)
{
    m_menu = menu;
}


vmxGUIMenuInteractorLeftButtonDownCallback* vmxGUIMenuInteractorLeftButtonDownCallback::New()
{
    return new vmxGUIMenuInteractorLeftButtonDownCallback;
}


void vmxGUIMenuInteractorLeftButtonDownCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_menu)
    {
        if(m_menu->m_interactor)
        {
            cout<<" down ";
            
            if(m_menu->m_clip_board) m_menu->m_clip_board->m_is_valid = 0;
            
            
            int pick_pos[2];
            m_menu->m_interactor->GetEventPosition(pick_pos);
            
            m_previous_left_button_down_position[0] = pick_pos[0];
            m_previous_left_button_down_position[1] = pick_pos[1];
            
            int index = m_menu->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(index<0) return;
            
            if(m_menu->m_interactor->GetControlKey())
            {
                // if CTRL key is pressed, jus add/remove items from selection
                m_menu->GetItem(index)->m_is_selected = !(m_menu->GetItem(index)->m_is_selected);
            }
            else
            {
                // If CTRL is not pressed, we need to check is a single selection is performed or a drag event of multiple selected items.
                if(m_menu->m_item_pointers[index]->m_is_selected && m_menu->IsMultipleItemsSelected())
                {
                    // If the picked item was already selected and multiple items are selected in the list, this could be a drag,
                    // and for now consider it as such by setting m_left_button_up_callback->m_is_selecting that will perform selection
                    // in case drag was not done after all.
                    m_menu->m_left_button_up_callback->m_is_selecting = 1;
                }
                else
                {
                    // If there is no drag for sure, just perform single selection
                    m_menu->SelectSingleItem(index);
                }
            }
            
            //m_menu->ListenForMouseMoveEvent(1);
            //m_menu->m_mouse_move_callback->SetPreviousLeftButtonDownPosition(pick_pos[0], pick_pos[1]);
            m_menu->ListenForLeftButtonUpEvent(1);
            
            m_menu->ShowSelectedItems();
            
            int check_index = m_menu->GetPickedCheckBoxIndex(index,pick_pos[0],pick_pos[1]);
            if(check_index>=0)
            {
                if(m_menu->m_items[index].m_checkboxes[check_index])
                {
                    m_menu->m_items[index].m_checkboxes[check_index] = 0;
                }
                else
                {
                    m_menu->m_items[index].m_checkboxes[check_index] = 1;
                }
                m_menu->BuildMenu();//NO NEED TO BUILD THE WHOLE MENU, INSTEAD BUILD JUST CHECK BOXES...
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIMenuInteractorLeftButtonUpCallback::vmxGUIMenuInteractorLeftButtonUpCallback()
{
    m_menu = NULL;
    m_is_selecting = 0;
}


vmxGUIMenuInteractorLeftButtonUpCallback::~vmxGUIMenuInteractorLeftButtonUpCallback()
{
    m_menu = NULL;
}


void vmxGUIMenuInteractorLeftButtonUpCallback::SetMenu(vmxGUIMenu *menu)
{
    m_menu = menu;
}


vmxGUIMenuInteractorLeftButtonUpCallback* vmxGUIMenuInteractorLeftButtonUpCallback::New()
{
    return new vmxGUIMenuInteractorLeftButtonUpCallback;
}


void vmxGUIMenuInteractorLeftButtonUpCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_menu)
    {
        if(m_menu->m_interactor)
        {
            cout<<" up ";
            
            int pick_pos[2];
            m_menu->m_interactor->GetEventPosition(pick_pos);
            
            int index = m_menu->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
//            if(index<0)
//            {
//                // reset the indicator.
//                //m_is_selecting = 0;
//                return;
//            }
            
            // if the button down event set the is_selecting indicator, it means that is was suspecting a drag event
            // but was not sure until mouse move event plays out.
            if(m_is_selecting)
            {
                // if is_selecting is set, this means that mouse move event did not reset it, so we perform single
                // item selection.
//                int pick_pos[2];
//                m_menu->m_interactor->GetEventPosition(pick_pos);
//                
//                int index = m_menu->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
                if(index<0) return;
                
                m_menu->SelectSingleItem(index);
                m_menu->ShowSelectedItems();
                
                
                // reset the indicator.
                m_is_selecting = 0;
            }
            
            
            
            
            
            vmxGUIMenuItem *item = m_menu->GetItem(index);
            if(item)
            {
                m_menu->HideAllSubMenus();
                if(item->m_sub_menu)
                {
                    int *window_size = m_menu->m_interactor->GetRenderWindow()->GetSize();
                    
                    int origin[2];
                    m_menu->GetOrigin(origin[0], origin[1]);
                    int menu_size[2];
                    m_menu->GetSize(menu_size[0],menu_size[1]);
                    item->m_sub_menu->SetVisibility(1);
                    int new_origin[2];
                    new_origin[0] = origin[0]+menu_size[0];
                    new_origin[1] = origin[1] + (menu_size[1]*index)/m_menu->GetNumberOfItems();
                    
                    int sub_menu_size[2];
                    item->m_sub_menu->GetSize(sub_menu_size[0],sub_menu_size[1]);
                    if(new_origin[1]+sub_menu_size[1]>window_size[1])
                    {
                        new_origin[1] = window_size[1] - sub_menu_size[1];
                    }
                    
                    
                    item->m_sub_menu->SetOrigin(new_origin[0], new_origin[1]);
                }
                else
                {
                    if(item->m_slot)
                    {
                        item->m_slot->Execute(m_menu);
                    }
                }
            }
            else
            {
                m_menu->HideAllSubMenus();
            }
            
            
            // Turn off listening for mouse move and button up events. They will be turned on in button down
            // event (as soon as left button is pressed).
            //m_menu->ListenForMouseMoveEvent(0);
            m_menu->ListenForLeftButtonUpEvent(0);
            m_menu->SetVisibilityOfDragEventActor(0);
            m_menu->m_interactor->Render();
        }
    }
}



//-----------------------------------------------------------------------------------------------------



//vmxGUIMenuInteractorMouseMoveCallback::vmxGUIMenuInteractorMouseMoveCallback()
//{
//    m_menu = NULL;
//    m_previous_left_button_down_position[0] = m_previous_left_button_down_position[1] = 0;
//}
//
//
//vmxGUIMenuInteractorMouseMoveCallback::~vmxGUIMenuInteractorMouseMoveCallback()
//{
//    m_menu = NULL;
//}
//
//
//void vmxGUIMenuInteractorMouseMoveCallback::SetMenu(vmxGUIMenu *menu)
//{
//    m_menu = menu;
//}
//
//
//vmxGUIMenuInteractorMouseMoveCallback* vmxGUIMenuInteractorMouseMoveCallback::New()
//{
//    return new vmxGUIMenuInteractorMouseMoveCallback;
//}
//
//
//void vmxGUIMenuInteractorMouseMoveCallback::Execute(vtkObject *caller, unsigned long, void *)
//{
//    if(m_menu)
//    {
//        if(m_menu->m_interactor)
//        {
//            cout<<" move ";
//            
//            int pick_pos[2];
//            m_menu->m_interactor->GetEventPosition(pick_pos);
//            
//            // Get the distance between the last left button down evenr and current position of the mouse.
//            int d0 = pick_pos[0] - m_previous_left_button_down_position[0];
//            if(d0<0) d0 = -d0;
//            int d1 = pick_pos[1] - m_previous_left_button_down_position[1];
//            if(d1<0) d1 = -d1;
//            
//            // if the distance is larger than the bellow predefined value, we have detected a drag. We know that at
//            // this moment the left button is still down because the left button up would stop this interaction observation.
//            if(d0>10 || d1>10)//If drag is detected
//            {
//                //cout<<" drag ";
//                
//                // tell left button up cllback not to perform any selection by reseting m_left_button_up_callback->m_is_selecting.
//                m_menu->m_left_button_up_callback->m_is_selecting = 0;
//                
//                // perform copying to clipboard of selected data.
//                m_menu->CopySelectedItemsToClipBoard();
//                m_menu->SetVisibilityOfDragEventActor(1);
//                m_menu->m_drag_actor->SetPosition(pick_pos[0],pick_pos[1]);
//                m_menu->m_interactor->Render();
//                
//            }
//        }
//    }
//}



//-----------------------------------------------------------------------------------------------------



vmxGUIMenuItem::vmxGUIMenuItem()
{
    m_is_selected = 0;
    m_slot = NULL;
    m_menu = NULL;
    m_sub_menu = NULL;
    
    m_number_of_check_boxes = 0;
    for(unsigned int i=0; i<m_number_of_check_boxes; i++)
    {
        m_checkboxes[i] = 0;
    }
}


vmxGUIMenuItem::~vmxGUIMenuItem()
{
    delete m_slot;
}


void vmxGUIMenuItem::AttachSlot(vmxGUIMenuSlot *slot)
{
    m_slot = slot;
}


vmxGUIMenuSlot* vmxGUIMenuItem::GetSlot()
{
    return m_slot;
}


vmxGUIMenu* vmxGUIMenuItem::GetSubMenu()
{
    return m_sub_menu;
}




//-----------------------------------------------------------------------------------------------------



vmxGUIMenu::vmxGUIMenu()
{
    m_class_name.Assign("vmxGUIMenu");
    
    m_clip_board = NULL;
    
    m_menu = this;
    
    //m_placement = FREE;
    m_placement_relative_percentages[0] = m_placement_relative_percentages[1] = 0;

//    m_window_modified_callback = vtkSmartPointer<vmxGUIMenuRenderWindowModifiedCallback>::New();
//    m_window_modified_callback->SetMenu(this);
    m_left_button_down_callback = vtkSmartPointer<vmxGUIMenuInteractorLeftButtonDownCallback>::New();
    m_left_button_down_callback->SetMenu(this);
    m_left_button_up_callback = vtkSmartPointer<vmxGUIMenuInteractorLeftButtonUpCallback>::New();
    m_left_button_up_callback->SetMenu(this);
//    m_mouse_move_callback = vtkSmartPointer<vmxGUIMenuInteractorMouseMoveCallback>::New();
//    m_mouse_move_callback->SetMenu(this);

    
    m_max_number_of_check_boxes = 0;
    m_interactor = NULL;
    m_font_size = 18;
    m_selection_text_character = '_';
    m_selection_text_line.Assign(m_selection_text_character);
    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_selected_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_checkboxes_actor = vtkSmartPointer<vtkTextActor>::New();
    m_drag_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxGUIMenu::~vmxGUIMenu()
{
}


vmxGUIMenu* vmxGUIMenu::New()
{
    return new vmxGUIMenu;
}


vmxGUIMenuItem* vmxGUIMenu::AddItem(const char *item_text, unsigned int number_of_check_boxes)
{
    if(!item_text) return NULL;
    if(number_of_check_boxes>10) return NULL;
    vmxGUIMenuItem *item = this->m_items.AddNewToEnd();
    item->m_menu = this;
    item->m_text.Assign(item_text);
    item->m_number_of_check_boxes = number_of_check_boxes;
    if(number_of_check_boxes>m_max_number_of_check_boxes) m_max_number_of_check_boxes = number_of_check_boxes;
    return item;
}


vmxGUIMenuItem* vmxGUIMenu::AddSubMenu(const char *item_text)
{
    if(!item_text) return NULL;
    
    vmxGUIMenuItem *item = this->m_items.AddNewToEnd();
    item->m_menu = this;
    item->m_text.Assign(item_text);
    
    item->m_sub_menu = new vmxGUIMenu;
    
    item->m_sub_menu->m_menu = this;
    item->m_sub_menu->SetClipBoard(m_clip_board);
    item->m_sub_menu->SetMainWidget(m_main_widget);

    
    double r,g,b;
    this->GetColor(r,g,b);
    item->m_sub_menu->SetColor(r,g,b);
    
    item->m_sub_menu->SetFontSize(this->m_font_size);
    
    return item;
}


void vmxGUIMenu::BuildMenu()
{
    m_item_pointers.Reset();
    m_item_pointers.SetNumberOfElements(m_items.GetNumberOfElements());
    
    mxString text;
    mxString checkbox_text;
    
    int i = m_item_pointers.GetNumberOfElements()-1;
    mxListIterator<vmxGUIMenuItem> it;
    for(it.SetToEnd(this->m_items), i = m_item_pointers.GetNumberOfElements()-1; it.IsValid() && i>=0; it.MoveToPrevious(), i--)
    {
        m_item_pointers[i] = it.GetElementAddress();
//        text.Append(" ");
//        text.Append(it.GetElement().m_text);
//
//        if(it.GetElement().GetSubMenu())
//        {
//             checkbox_text.Append(">>");
//        }
//        
//        if(it.GetNode()!=m_items.GetBeginNodeAddress())
//        {
//            text.Append("\n");
//            checkbox_text.Append("\n");
//        }
    }
    
//    m_text_actor->SetInput(text.Get_C_String());
//    m_checkboxes_actor->SetInput(checkbox_text.Get_C_String());
    
    this->ShowTextItems();
    this->ComputeSelectionTextLine();
    
    // Now that this menu is built, call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            m_item_pointers[i]->GetSubMenu()->BuildMenu();
        }
    }
}


int vmxGUIMenu::ComputeSelectionTextLine()
{
    if(!this->m_interactor) return 0;
    if(!this->m_interactor->GetRenderWindow()) return 0;
    if(!this->m_interactor->GetRenderWindow()->GetRenderers()) return 0;
    if(!this->m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()) return 0;
    if(this->m_item_pointers.IsEmpty()) return 0;//menu not built yet.
    
    vtkSmartPointer<vtkTextActor> selection_character_actor = vtkSmartPointer<vtkTextActor>::New();
    selection_character_actor->GetTextProperty()->SetFontSize(m_font_size);
    
    int actor_size[2];
    this->GetSizeOfTextActor(actor_size[0],actor_size[1]);
    
    m_selection_text_line.Clear();
    for(int n = 1; n<200; n++)
    {
        m_selection_text_line.Append(m_selection_text_character);
        selection_character_actor->SetInput(m_selection_text_line.Get_C_String());
        double size[2];
        selection_character_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(),size);
        if(size[0]>=actor_size[0]) break;
    }
    
    return 1;
}


int vmxGUIMenu::CopySelectedItemsToClipBoard()
{
    if(!m_clip_board) return 0;
    
    m_clip_board->m_data_text.Clear();
    for(unsigned int i=0; i<this->GetNumberOfItems(); i++)
    {
        if(m_item_pointers[i]->m_is_selected)
        {
            m_clip_board->m_data_text.Append(m_item_pointers[i]->m_text);
            m_clip_board->m_data_text.Append(";");
        }
    }
    m_clip_board->m_is_valid = 1;
    m_clip_board->m_owner_of_data = (void*)this;
    
    return 1;
}



void vmxGUIMenu::GetColor(double &r, double &g, double &b)
{
    m_text_actor->GetTextProperty()->GetColor(r,g,b);
}


void vmxGUIMenu::GetOrigin(int &origin1, int &origin2)
{
    //currently the text actor is the first one in the menu
    this->GetOriginOfTextActor(origin1, origin2);
}


int vmxGUIMenu::GetOriginOfTextActor(int &origin1, int &origin2)
{
    double *org = m_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    return 1;
}


int vmxGUIMenu::GetOriginOfCheckBoxActor(int &origin1, int &origin2)
{
    double *org = m_checkboxes_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    return 1;
}


int vmxGUIMenu::GetPickedCheckBoxIndex(int picked_item_index, int pos1, int pos2)
{
    if(picked_item_index<0 || picked_item_index>=this->GetNumberOfItems()) return -1;
    if(m_items[picked_item_index].m_number_of_check_boxes==0) return -1;
    if(this->GetMaxNumberOfCheckBoxes()==0) return -1;//this condition is not really needed, but better safe than sorry.
    if(!this->IsCheckBoxActorPicked(pos1,pos2)) return -1;
    
    int actor_size[2];
    this->GetSizeOfCheckBoxActor(actor_size[0], actor_size[1]);
    
    int actor_origin[2];
    this->GetOriginOfCheckBoxActor(actor_origin[0],actor_origin[1]);
    
    double a1 = (pos1-actor_origin[0]);
    double a2 = ((double)actor_size[0])/((double)this->GetMaxNumberOfCheckBoxes());
    int checkbox_index = a1/a2;;
    
    if(checkbox_index<0 || checkbox_index>=m_items[picked_item_index].m_number_of_check_boxes) return -1;
    
    //cout<<" "<<checkbox_index<<" ";
    
    return checkbox_index;
}


int vmxGUIMenu::GetPickedItemIndex(int pos1, int pos2)
{
    if(!this->IsMenuPicked(pos1,pos2)) return -1;
    
    int menu_size[2];
    this->GetSize(menu_size[0], menu_size[1]);
    
    int menu_origin[2];
    this->GetOrigin(menu_origin[0],menu_origin[1]);
    
    int item_index;
    double a1 = (pos2-menu_origin[1]);
    if(a1<0) item_index = -1;
    else
    {
        double a2 = ((double)menu_size[1])/((double)this->GetNumberOfItems());
        item_index = a1/a2;
    }
    
//    cout<<endl;
//    cout<<"  "<<"pick_pos="<<pos1<<","<<pos2;
//    cout<<"  "<<"menu_origin="<<menu_origin[0]<<","<<menu_origin[1];
//    cout<<"  "<<"pos2-orig[1]="<<pos2-menu_origin[1];
//    cout<<"  "<<"a1="<<a1;
//    cout<<"  "<<"menu_origin="<<menu_origin[0]<<","<<menu_origin[1];
//    cout<<"  "<<"menu_size="<<menu_size[0]<<","<<menu_size[1];
//    cout<<"  "<<"number_of_items="<<(this->GetNumberOfItems());
//    cout<<"  "<<"item_index="<<item_index;
//    cout<<endl;
    
    if(item_index<0 || item_index>=this->GetNumberOfItems()) return -1;
    return item_index;
}


int vmxGUIMenu::GetSizeOfCheckBoxActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_checkboxes_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
    
    return 1;
}


void vmxGUIMenu::GetSize(int &output_size1, int &output_size2)
{
    int size_of_text_actor[2];
    int size_of_checkbox_actor[2];
    if(!this->GetSizeOfTextActor(size_of_text_actor[0], size_of_text_actor[1]))
    {
        output_size1 = output_size2 = 0;
        return ;
    }
    if(!this->GetSizeOfCheckBoxActor(size_of_checkbox_actor[0], size_of_checkbox_actor[1]))
    {
        output_size1 = output_size2 = 0;
        return ;
    }
    output_size1 = size_of_text_actor[0] + size_of_checkbox_actor[0];
    output_size2 = size_of_text_actor[1];
}


int vmxGUIMenu::GetSizeOfTextActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_text_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    output_size1 = size[0];
    output_size2 = size[1];
    
    return 1;
}


int vmxGUIMenu::GetVisibility()
{
    return m_text_actor->GetVisibility();
}


void vmxGUIMenu::HideAllSubMenus()
{
    // Call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            m_item_pointers[i]->GetSubMenu()->SetVisibility(0);
            m_item_pointers[i]->GetSubMenu()->HideAllSubMenus();
        }
    }
}


int vmxGUIMenu::IsCheckBoxActorPicked(int pos1, int pos2)
{
    int actor_size[2];
    this->GetSizeOfCheckBoxActor(actor_size[0],actor_size[1]);
    
    int actor_origin[2];
    this->GetOriginOfCheckBoxActor(actor_origin[0],actor_origin[1]);
    
    
    int end_pos1 = actor_origin[0] + actor_size[0];
    if(pos1<actor_origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = actor_origin[1] + actor_size[1];
    if(pos2<actor_origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


int vmxGUIMenu::IsMenuPicked(int pos1, int pos2)
{
    int menu_size[2];
    this->GetSize(menu_size[0],menu_size[1]);
    
    int menu_origin[2];
    this->GetOrigin(menu_origin[0],menu_origin[1]);
    
    
    int end_pos1 = menu_origin[0] + menu_size[0];
    if(pos1<menu_origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = menu_origin[1] + menu_size[1];
    if(pos2<menu_origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


int vmxGUIMenu::IsMultipleItemsSelected()
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


int vmxGUIMenu::IsTextActorPicked(int pos1, int pos2)
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


int vmxGUIMenu::IsVisible()
{
    return m_text_actor->GetVisibility();
}


void vmxGUIMenu::ListenForLeftButtonDownEvent(int is_listening)
{
    if(is_listening)
    {
        m_interactor->AddObserver(vtkCommand::LeftButtonPressEvent, m_left_button_down_callback);
    }
    else
    {
        m_interactor->RemoveObservers(vtkCommand::LeftButtonPressEvent, m_left_button_down_callback);
    }
}


void vmxGUIMenu::ListenForLeftButtonUpEvent(int is_listening)
{
//    if(!m_interactor) return;
    
    if(is_listening)
    {
        //cout<<" m_left_button_up_callback(1) ";
        m_interactor->AddObserver(vtkCommand::EndInteractionEvent, m_left_button_up_callback);
        //m_interactor->AddObserver(vtkCommand::LeftButtonReleaseEvent, m_left_button_up_callback);
    }
    else
    {
        //cout<<" m_left_button_up_callback(0) ";
        m_interactor->RemoveObservers(vtkCommand::EndInteractionEvent, m_left_button_up_callback);
        //m_interactor->RemoveObservers(vtkCommand::LeftButtonReleaseEvent, m_left_button_up_callback);
    }
}


void vmxGUIMenu::SelectSingleItem(unsigned int item_index)
{
    if(item_index>=m_item_pointers.GetNumberOfElements()) return;
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        m_item_pointers[i]->m_is_selected = 0;
    }
    m_item_pointers[item_index]->m_is_selected = 1;
}


void vmxGUIMenu::SetColor(double r, double g, double b)
{
    m_text_actor->GetTextProperty()->SetColor(r,g,b);
    m_checkboxes_actor->GetTextProperty()->SetColor(r,g,b);
    m_selected_text_actor->GetTextProperty()->SetColor(1,1,1);
    m_selected_text_actor->GetTextProperty()->SetOpacity(0.7);
    m_drag_actor->GetTextProperty()->SetOpacity(0.5);
    
    // Call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            m_item_pointers[i]->GetSubMenu()->SetColor(r,g,b);
        }
    }

}


void vmxGUIMenu::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;

//    m_text_actor->GetTextProperty()->SetFontFamilyToCourier();
//    m_checkboxes_actor->GetTextProperty()->SetFontFamilyToCourier();
//    m_selected_text_actor->GetTextProperty()->SetFontFamilyToCourier();
//    
    
//    m_text_actor->GetTextProperty()->SetFontFamilyToTimes();
//    m_checkboxes_actor->GetTextProperty()->SetFontFamilyToTimes();
//    m_selected_text_actor->GetTextProperty()->SetFontFamilyToTimes();
    
    m_text_actor->GetTextProperty()->SetFontFamilyToArial();
    m_checkboxes_actor->GetTextProperty()->SetFontFamilyToArial();
    m_selected_text_actor->GetTextProperty()->SetFontFamilyToArial();
    
    m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    m_checkboxes_actor->GetTextProperty()->SetFontSize(m_font_size);
    m_selected_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    m_drag_actor->GetTextProperty()->SetFontSize(m_font_size);
    
    this->ComputeSelectionTextLine();
    
    // Call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            m_item_pointers[i]->GetSubMenu()->SetFontSize(font_size);
        }
    }

}


void vmxGUIMenu::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
    
//    m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
    m_interactor->AddObserver(vtkCommand::LeftButtonPressEvent, m_left_button_down_callback);
    
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_text_actor);
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_checkboxes_actor);
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_selected_text_actor);
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_drag_actor);
    
    m_selected_text_actor->SetVisibility(0);
    m_drag_actor->SetVisibility(0);
    
    this->ComputeSelectionTextLine();
    
    // Call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            m_item_pointers[i]->GetSubMenu()->SetInteractor(interactor);
            m_item_pointers[i]->GetSubMenu()->SetVisibility(0);
        }
    }

}


void vmxGUIMenu::SetOrigin(int origin1, int origin2)
{    
    int text_actor_size[2];
    this->GetSizeOfTextActor(text_actor_size[0], text_actor_size[1]);

    m_text_actor->SetPosition(origin1,origin2);
    m_checkboxes_actor->SetPosition(origin1+text_actor_size[0],origin2);
    m_selected_text_actor->SetPosition(origin1,origin2);
    m_placement = FIXED;
}


void vmxGUIMenu::SetVisibility(int is_visible)
{
    if(this->GetVisibility() && is_visible) return;
    if(!this->GetVisibility() && !is_visible) return;
    
    this->m_text_actor->SetVisibility(is_visible);
    this->m_checkboxes_actor->SetVisibility(is_visible);
    this->m_selected_text_actor->SetVisibility(0);//    this->m_selected_text_actor->SetVisibility(is_visible);
//    this->m_drag_actor->SetVisibility(is_visible);
    
    // If the menu is not visible, it should not observe interaction events
    if(!is_visible)
    {
        //m_interactor->GetRenderWindow()->RemoveObservers(vtkCommand::ModifiedEvent, m_window_modified_callback);
        //this->ListenForMouseMoveEvent(0);
        this->ListenForLeftButtonUpEvent(0);
        this->ListenForLeftButtonDownEvent(0);
    }
    else
    {
        //m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
        this->ListenForLeftButtonDownEvent(1);
    }
}


void vmxGUIMenu::SetVisibilityOfDragEventActor(int is_visible)
{
    if(!is_visible)
    {
        m_drag_actor->SetVisibility(0);
    }
    else
    {
        if(m_drag_actor->GetVisibility()) return;
        
        int counter = 0;
        for(int i=this->GetNumberOfItems()-1; i>=0; i--)
        {
            if(m_item_pointers[i]->m_is_selected)
            {
                counter++;
            }
        }
        mxString text;
        text.AssignNumber(counter);
        m_drag_actor->SetInput(text.Get_C_String());
        m_drag_actor->SetVisibility(1);
    }
}


int vmxGUIMenu::ShowSelectedItems()
{
    //cout<<"#";
    
    mxString selected_text;
    for(int i=this->GetNumberOfItems()-1; i>=0; i--)
    {
        if(m_item_pointers[i]->m_is_selected)
        {
            selected_text.Append(m_selection_text_line);
            if(i!=0)
            {
                selected_text.Append("\n");
            }
        }
        else
        {
            if(i!=0)
            {
                selected_text.Append("\n");//selected_text.Append("_\n");
            }
            else
            {
                selected_text.Append(" ");//selected_text.Append("_");
            }
        }
    }
    
    m_selected_text_actor->SetInput(selected_text.Get_C_String());
    
    int actor_origin[2];
    this->GetOriginOfTextActor(actor_origin[0],actor_origin[1]);
    m_selected_text_actor->SetPosition(actor_origin[0],actor_origin[1]);
    
    m_selected_text_actor->SetVisibility(1);
    
    return 1;
}


int vmxGUIMenu::ShowTextItems()
{
    mxString text;
    mxString checkbox_text;
    
    for(int i=m_item_pointers.GetNumberOfElements()-1; i>=0; i--)
    {
        //m_item_pointers[i] = it.GetElementAddress();
        text.Append(" ");
        text.Append(m_item_pointers[i]->m_text);
        
        if(m_item_pointers[i]->GetSubMenu())
        {
            checkbox_text.Append("  >>");
        }
        
        if(i!=0)
        {
            text.Append("\n");
            checkbox_text.Append("\n");
        }
    }
    
    m_text_actor->SetInput(text.Get_C_String());
    m_checkboxes_actor->SetInput(checkbox_text.Get_C_String());
    
    // Now that this menu is built, call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            m_item_pointers[i]->GetSubMenu()->ShowTextItems();
        }
    }

    
    
//    mxString text, checkbox_text;
//    
//    m_number_of_items_in_text_actor = 0;
//    int m = 0;
//    for(int i=m_text_index_offset; i<=this->GetNumberOfItems()-1 && m<m_text_actor_max_number_of_items; i++, m++)
//    {
//        m_number_of_items_in_text_actor++;
//        
//        text.Append(" ");
//        if(!m_item_pointers[i]->m_is_controller)
//        {
//            text.Append("   ");
//        }
//        
//        text.Append(m_item_pointers[i]->m_text);
//        for(unsigned int k=0; k<m_item_pointers[i]->m_number_of_check_boxes; k++)
//        {
//            if(m_item_pointers[i]->m_checkboxes[k]==0)
//            {
//                checkbox_text.Append("[_]");
//            }
//            else
//            {
//                checkbox_text.Append("[x]");
//            }
//        }
//        
//        if(i!=this->GetNumberOfItems()-1 && m!=m_text_actor_max_number_of_items-1)
//        {
//            text.Append("\n");
//            checkbox_text.Append("\n");
//        }
//    }
//    
//    if(m_number_of_items_in_text_actor<this->GetNumberOfItems())
//    {
//        this->ListenForMouseWheelBackwardEvent(1);
//        this->ListenForMouseWheelForwardEvent(1);
//    }
//    else
//    {
//        this->ListenForMouseWheelBackwardEvent(0);
//        this->ListenForMouseWheelForwardEvent(0);
//    }
//    
//    m_text_actor->SetInput(text.Get_C_String());
//    m_checkboxes_actor->SetInput(checkbox_text.Get_C_String());
    
    return 1;
}








