/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIListWidget.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIListWidget.h"






//-----------------------------------------------------------------------------------------------------



vmxGUIListWidgetInteractorLeftButtonDownCallback::vmxGUIListWidgetInteractorLeftButtonDownCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


vmxGUIListWidgetInteractorLeftButtonDownCallback::~vmxGUIListWidgetInteractorLeftButtonDownCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


void vmxGUIListWidgetInteractorLeftButtonDownCallback::SetListWidget(vmxGUIListWidget *list_widget)
{
    m_is_active = 0;
    m_list_widget = list_widget;
}


vmxGUIListWidgetInteractorLeftButtonDownCallback* vmxGUIListWidgetInteractorLeftButtonDownCallback::New()
{
    return new vmxGUIListWidgetInteractorLeftButtonDownCallback;
}


void vmxGUIListWidgetInteractorLeftButtonDownCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_list_widget->m_interactor->AddObserver(vtkCommand::LeftButtonPressEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_list_widget->m_interactor->RemoveObservers(vtkCommand::LeftButtonPressEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIListWidgetInteractorLeftButtonDownCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_list_widget)
    {
        if(m_list_widget->m_interactor)
        {
            //cout<<endl<<" down ";
            
            if(m_list_widget->GetClipBoard()) m_list_widget->GetClipBoard()->m_is_valid = 0;

            /// Reset the picekd indexes to non-valid values.
            m_list_widget->m_picked_item_index = -1;
            m_list_widget->m_picked_check_box_index = -1;

            
            int pick_pos[2];
            m_list_widget->m_interactor->GetEventPosition(pick_pos);
            
            m_list_widget->m_double_click_detector.OnLeftButtonDown(pick_pos[0], pick_pos[1],m_list_widget->m_interactor->GetControlKey());
            
            
            int index = m_list_widget->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(index<0)
            {
                return;
            }
            m_list_widget->m_mouse_drag_detector.OnLeftButtonDown(pick_pos[0], pick_pos[1]);
            
            m_list_widget->m_item_selection_detector.OnLeftButtonDown(m_list_widget->m_interactor->GetControlKey(), m_list_widget->m_item_pointers[index]->m_is_selected, m_list_widget->IsMultipleItemsSelected());
            
            m_list_widget->m_picked_item_index = index;

            
            
            if(m_list_widget->m_item_selection_detector.IsMultipleSelectionDetected())
            {
                // if multiple selection is detected (CTRL key is pressed), jus add/remove items from selection
                m_list_widget->GetItem(index)->m_is_selected = !(m_list_widget->GetItem(index)->m_is_selected);
            }
            else
            {
                // If CTRL is not pressed, we need to check if a single selection is performed or a drag event of multiple selected items.
                if(m_list_widget->m_item_selection_detector.IsSingleSelectionDetected())
                {
                    // If there is no drag for sure, just perform single selection
                    m_list_widget->SelectSingleItem(index);
                }
            }
            
            m_list_widget->ListenForMouseMoveEvent(1);
            m_list_widget->ListenForLeftButtonUpEvent(1);
            
            m_list_widget->ShowSelectedItems();
            
            int check_index = m_list_widget->GetPickedCheckBoxIndex(index,pick_pos[0],pick_pos[1]);
            //cout<<"check_index = "<<check_index<<endl;
            if(check_index>=0)
            {
                if(m_list_widget->m_items[index].m_checkboxes[check_index])
                {
                    m_list_widget->m_items[index].m_checkboxes[check_index] = 0;
                }
                else
                {
                    m_list_widget->m_items[index].m_checkboxes[check_index] = 1;
                }
                m_list_widget->m_picked_check_box_index = check_index;
                m_list_widget->ShowTextItems();
            }
            
//            // If double click was performed execute double click slot...
//            if(m_list_widget->m_double_click_detector.IsDoubleClicked())
//            {
//                // this part executes on detected double click.
//            }
//            else
//            {
                // Execute the single mouse left click slot (if attached).
                if(m_list_widget->GetItem(index)->m_mouse_left_click_slot)
                {
                    m_list_widget->GetItem(index)->m_mouse_left_click_slot->Execute(m_list_widget->GetItem(index), check_index);
                }
//            }
            
            m_list_widget->OnItemPicked();
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIListWidgetInteractorLeftButtonUpCallback::vmxGUIListWidgetInteractorLeftButtonUpCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


vmxGUIListWidgetInteractorLeftButtonUpCallback::~vmxGUIListWidgetInteractorLeftButtonUpCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


void vmxGUIListWidgetInteractorLeftButtonUpCallback::SetListWidget(vmxGUIListWidget *list_widget)
{
    m_is_active = 0;
    m_list_widget = list_widget;
}


vmxGUIListWidgetInteractorLeftButtonUpCallback* vmxGUIListWidgetInteractorLeftButtonUpCallback::New()
{
    return new vmxGUIListWidgetInteractorLeftButtonUpCallback;
}


void vmxGUIListWidgetInteractorLeftButtonUpCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_list_widget->m_interactor->AddObserver(vtkCommand::EndInteractionEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_list_widget->m_interactor->RemoveObservers(vtkCommand::EndInteractionEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIListWidgetInteractorLeftButtonUpCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_list_widget)
    {
        if(m_list_widget->m_interactor)
        {
            //cout<<endl<<" up ";
            
            int pick_pos[2];
            m_list_widget->m_interactor->GetEventPosition(pick_pos);
            
            m_list_widget->m_mouse_drag_detector.OnLeftButtonUp(pick_pos[0], pick_pos[1]);
            m_list_widget->m_item_selection_detector.OnLeftButtonUp();
            m_list_widget->m_item_selection_detector.OnLeftButtonUp();
            
            
            
            //--- Execute slot or open submenu ---
            //            vmxOpenFilesDialogItem *item = m_menu->GetItem(index);
            //            if(item)
            //            {
            //                m_menu->HideAllSubMenus();
            //                if(item->m_sub_menu)
            //                {
            //                    int *window_size = m_menu->m_interactor->GetRenderWindow()->GetSize();
            //
            //                    int origin[2];
            //                    m_menu->GetOriginOfMenu(origin[0], origin[1]);
            //                    int menu_size[2];
            //                    m_menu->GetSizeOfMenu(menu_size[0],menu_size[1]);
            //                    item->m_sub_menu->SetVisibility(1);
            //                    int new_origin[2];
            //                    new_origin[0] = origin[0]+menu_size[0];
            //                    new_origin[1] = origin[1] + (menu_size[1]*index)/m_menu->GetNumberOfItems();
            //
            //                    int sub_menu_size[2];
            //                    item->m_sub_menu->GetSizeOfMenu(sub_menu_size[0],sub_menu_size[1]);
            //                    if(new_origin[1]+sub_menu_size[1]>window_size[1])
            //                    {
            //                        new_origin[1] = window_size[1] - sub_menu_size[1];
            //                    }
            //
            //
            //                    item->m_sub_menu->SetOrigin(new_origin[0], new_origin[1]);
            //                }
            //                else
            //                {
            //                    if(item->m_slot)
            //                    {
            //                        item->m_slot->Execute(m_menu);
            //                    }
            //                }
            //            }
            //            else
            //            {
            //                m_menu->HideAllSubMenus();
            //            }
            //------
            
            
            // Turn off listening for mouse move and button up events. They will be turned on in button down
            // event (as soon as left button is pressed).
            m_list_widget->ListenForMouseMoveEvent(0);
            m_list_widget->ListenForLeftButtonUpEvent(0);
            m_list_widget->SetVisibilityOfDragEventActor(0);
            m_list_widget->m_interactor->Render();

        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIListWidgetInteractorMouseMoveCallback::vmxGUIListWidgetInteractorMouseMoveCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


vmxGUIListWidgetInteractorMouseMoveCallback::~vmxGUIListWidgetInteractorMouseMoveCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


void vmxGUIListWidgetInteractorMouseMoveCallback::SetListWidget(vmxGUIListWidget *list_widget)
{
    m_is_active = 0;
    m_list_widget = list_widget;
}


vmxGUIListWidgetInteractorMouseMoveCallback* vmxGUIListWidgetInteractorMouseMoveCallback::New()
{
    return new vmxGUIListWidgetInteractorMouseMoveCallback;
}


void vmxGUIListWidgetInteractorMouseMoveCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_list_widget->m_interactor->AddObserver(vtkCommand::ModifiedEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_list_widget->m_interactor->RemoveObservers(vtkCommand::ModifiedEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIListWidgetInteractorMouseMoveCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_list_widget)
    {
        if(m_list_widget->m_interactor)
        {
            //cout<<endl<<" move2 ";
            
            int pick_pos[2];
            m_list_widget->m_interactor->GetEventPosition(pick_pos);
            
            m_list_widget->m_mouse_drag_detector.OnMouseMove(pick_pos[0],pick_pos[1]);
            m_list_widget->m_item_selection_detector.OnMouseMove(m_list_widget->m_mouse_drag_detector.IsDragging());
            
            //If drag is detected
            if(m_list_widget->m_mouse_drag_detector.IsDragging())
            {
                //cout<<" drag ";
                
                // perform copying to clipboard of selected data.
                m_list_widget->CopySelectedItemsToClipBoard();
                m_list_widget->SetVisibilityOfDragEventActor(1);
                m_list_widget->m_drag_actor->SetPosition(pick_pos[0],pick_pos[1]);
                m_list_widget->m_interactor->Render();
            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIListWidgetInteractorMouseWheelBackwardCallback::vmxGUIListWidgetInteractorMouseWheelBackwardCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


vmxGUIListWidgetInteractorMouseWheelBackwardCallback::~vmxGUIListWidgetInteractorMouseWheelBackwardCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


void vmxGUIListWidgetInteractorMouseWheelBackwardCallback::SetListWidget(vmxGUIListWidget *list_widget)
{
    m_is_active = 0;
    m_list_widget = list_widget;
}


vmxGUIListWidgetInteractorMouseWheelBackwardCallback* vmxGUIListWidgetInteractorMouseWheelBackwardCallback::New()
{
    return new vmxGUIListWidgetInteractorMouseWheelBackwardCallback;
}


void vmxGUIListWidgetInteractorMouseWheelBackwardCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_list_widget->m_interactor->AddObserver(vtkCommand::MouseWheelBackwardEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_list_widget->m_interactor->RemoveObservers(vtkCommand::MouseWheelBackwardEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIListWidgetInteractorMouseWheelBackwardCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    //cout<<" wheel_backward ";
    if(m_list_widget)
    {
        if(m_list_widget->m_interactor)
        {
            int pick_pos[2];
            m_list_widget->m_interactor->GetEventPosition(pick_pos);
            
            int index = m_list_widget->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(index<0) return;
            
            int step = 4;
            
            m_list_widget->SetIndexOffset(m_list_widget->m_text_index_offset+step);
            m_list_widget->ShowTextItems();
            m_list_widget->ShowSelectedItems();
//            m_list_widget->RedoPlacement();
            m_list_widget->RepositionAfterResizing();
            
            m_list_widget->m_interactor->Render();
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIListWidgetInteractorMouseWheelForwardCallback::vmxGUIListWidgetInteractorMouseWheelForwardCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


vmxGUIListWidgetInteractorMouseWheelForwardCallback::~vmxGUIListWidgetInteractorMouseWheelForwardCallback()
{
    m_is_active = 0;
    m_list_widget = NULL;
}


void vmxGUIListWidgetInteractorMouseWheelForwardCallback::SetListWidget(vmxGUIListWidget *list_widget)
{
    m_is_active = 0;
    m_list_widget = list_widget;
}


vmxGUIListWidgetInteractorMouseWheelForwardCallback* vmxGUIListWidgetInteractorMouseWheelForwardCallback::New()
{
    return new vmxGUIListWidgetInteractorMouseWheelForwardCallback;
}


void vmxGUIListWidgetInteractorMouseWheelForwardCallback::Activate(int is_active)
{
    if(is_active)
    {
        if(m_is_active) return;
        m_list_widget->m_interactor->AddObserver(vtkCommand::MouseWheelForwardEvent, this);
    }
    else
    {
        if(!m_is_active) return;
        m_list_widget->m_interactor->RemoveObservers(vtkCommand::MouseWheelForwardEvent, this);
    }
    m_is_active = is_active;
}


void vmxGUIListWidgetInteractorMouseWheelForwardCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    //cout<<" wheel_forward ";
    if(m_list_widget)
    {
        if(m_list_widget->m_interactor)
        {
            int pick_pos[2];
            m_list_widget->m_interactor->GetEventPosition(pick_pos);
            
            int index = m_list_widget->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(index<0) return;
            
            int step = 4;
            
            m_list_widget->SetIndexOffset(m_list_widget->m_text_index_offset-step);
            m_list_widget->ShowTextItems();
            m_list_widget->ShowSelectedItems();
//            m_list_widget->RedoPlacement();
            m_list_widget->RepositionAfterResizing();
            
            m_list_widget->m_interactor->Render();
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxGUIListWidgetItem::vmxGUIListWidgetItem()
{
    /// By default all items are controllers, i.e. they are all on level 0.
    m_is_controller = 1;
    m_is_selected = 0;
    m_mouse_left_click_slot = NULL;
    m_list_widget = NULL;
    m_sub_list_widget = NULL;
    m_data = NULL;
    
    m_number_of_check_boxes = 0;
    for(unsigned int i=0; i<m_number_of_check_boxes; i++)
    {
        m_checkboxes[i] = 0;
    }
}


vmxGUIListWidgetItem::~vmxGUIListWidgetItem()
{
    delete m_mouse_left_click_slot;
}


void vmxGUIListWidgetItem::AttachSlot(vmxGUIListWidgetMouseLeftClickSlot *slot)
{
    m_mouse_left_click_slot = slot;
}


vmxGUIListWidgetMouseLeftClickSlot* vmxGUIListWidgetItem::GetMouseLeftClickSlot()
{
    return m_mouse_left_click_slot;
}


vmxGUIListWidget* vmxGUIListWidgetItem::GetSubListWidget()
{
    return m_sub_list_widget;
}


int vmxGUIListWidgetItem::IsChecked(int checkbox_index)
{
    if(checkbox_index<0) return 0;
    if(checkbox_index>=vmxGUIListWidgetItem_max_number_of_checkboxes) return 0;
    
    return m_checkboxes[checkbox_index];
}


//void vmxGUIListWidgetItem::SetNumberOfCheckBoxes(unsigned int number_of_check_boxes)
//{
//    if(number_of_check_boxes > vmxGUIListWidgetItem_max_number_of_checkboxes)
//    {
//        m_number_of_check_boxes = vmxGUIListWidgetItem_max_number_of_checkboxes;
//    }
//    else
//    {
//        m_number_of_check_boxes = number_of_check_boxes;
//    }
//}




//-----------------------------------------------------------------------------------------------------



vmxGUIListWidget::vmxGUIListWidget()
{
    m_clip_board = NULL;
    
    m_class_name.Assign("vmxGUIListWidget");
    
    m_left_button_down_callback = vtkSmartPointer<vmxGUIListWidgetInteractorLeftButtonDownCallback>::New();
    m_left_button_down_callback->SetListWidget(this);
    m_left_button_up_callback = vtkSmartPointer<vmxGUIListWidgetInteractorLeftButtonUpCallback>::New();
    m_left_button_up_callback->SetListWidget(this);
    m_mouse_move_callback = vtkSmartPointer<vmxGUIListWidgetInteractorMouseMoveCallback>::New();
    m_mouse_move_callback->SetListWidget(this);
    m_mouse_wheel_backward_callback = vtkSmartPointer<vmxGUIListWidgetInteractorMouseWheelBackwardCallback>::New();
    m_mouse_wheel_backward_callback->SetListWidget(this);
    m_mouse_wheel_forward_callback = vtkSmartPointer<vmxGUIListWidgetInteractorMouseWheelForwardCallback>::New();
    m_mouse_wheel_forward_callback->SetListWidget(this);
    
    
    m_number_of_items_in_text_actor = 0;
    m_text_actor_max_number_of_items = 50;
    
    m_max_number_of_check_boxes = 0;
    m_picked_item_index = -1;
    m_picked_check_box_index = -1;
    m_interactor = NULL;
    m_font_size = 18;
    m_text_index_offset = 0;
    m_selection_text_character = '_';
    m_selection_text_line.Assign(m_selection_text_character);
    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_selected_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_checkboxes_actor = vtkSmartPointer<vtkTextActor>::New();
   
    m_drag_actor = vtkSmartPointer<vtkTextActor>::New();

}


vmxGUIListWidget::~vmxGUIListWidget()
{
}


vmxGUIListWidget* vmxGUIListWidget::New()
{
    return new vmxGUIListWidget;
}


vmxGUIListWidgetItem* vmxGUIListWidget::AddItem(const char *item_text, unsigned int number_of_check_boxes)
{
    if(!item_text) return NULL;
    if(number_of_check_boxes>10) return NULL;
    vmxGUIListWidgetItem *item = this->m_items.AddNewToEnd();
    item->m_list_widget = this;
    item->m_text.Assign(item_text);
    item->m_number_of_check_boxes = number_of_check_boxes;
    if(number_of_check_boxes>m_max_number_of_check_boxes) m_max_number_of_check_boxes = number_of_check_boxes;
    return item;
}


vmxGUIListWidgetItem* vmxGUIListWidget::AddItem(const char *item_text, vmxGUIListWidgetItem *controller, unsigned int number_of_check_boxes)
{
    if(!controller) return NULL;
    if(!controller->m_is_controller) return NULL;
    if(number_of_check_boxes>10) return NULL;
    
    mxListNode<vmxGUIListWidgetItem> *controller_node = this->m_items.GetNodeWithElementAddress(controller);

    vmxGUIListWidgetItem *item = this->m_items.AddNewAfterNode(controller_node);
    item->m_is_controller = 0;
    item->m_list_widget = this;
    item->m_text.Assign(item_text);
    item->m_number_of_check_boxes = number_of_check_boxes;
    if(number_of_check_boxes>m_max_number_of_check_boxes) m_max_number_of_check_boxes = number_of_check_boxes;
    return item;
    
}


void vmxGUIListWidget::BuildListWidget()
{
    m_item_pointers.Reset();
    m_item_pointers.SetNumberOfElements(m_items.GetNumberOfElements());
    
    //Find out how many controllers there are in the list.
    mxListIterator<vmxGUIListWidgetItem> it;
    int n_of_controllers = 0;
    for(it.SetToEnd(this->m_items); it.IsValid(); it.MoveToPrevious())
    {
        if(it.GetElement().m_is_controller) n_of_controllers++;
    }
    
    m_controller_pointers.Reset();
    m_controller_pointers.SetNumberOfElements(n_of_controllers);
    
    
    int i = m_item_pointers.GetNumberOfElements()-1;
    int j = m_controller_pointers.GetNumberOfElements()-1;
    for(it.SetToEnd(this->m_items), i = m_item_pointers.GetNumberOfElements()-1, j = m_controller_pointers.GetNumberOfElements()-1; it.IsValid() && i>=0 && j>=0; it.MoveToPrevious(), i--)
    {
        m_item_pointers[i] = it.GetElementAddress();
        if(it.GetElement().m_is_controller)
        {
            m_controller_pointers[j] = it.GetElementAddress();
            j--;
        }
    }
    
    this->ShowTextItems();
    
    this->ComputeSelectionTextLine();
}


int vmxGUIListWidget::ComputeMaxNumberOfItemsInTextActor()
{
    if(!this->m_interactor) return 0;
    if(!this->m_interactor->GetRenderWindow()) return 0;
    if(!this->m_interactor->GetRenderWindow()->GetRenderers()) return 0;
//    if(!this->m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()) return 0;
    
    vtkSmartPointer<vtkTextActor> test_actor = vtkSmartPointer<vtkTextActor>::New();
    test_actor->GetTextProperty()->SetFontSize(m_font_size);
    
    double size[2];
    mxString text;
    text.Assign(" Xg ");
    
    test_actor->SetInput(text.Get_C_String());
    
    test_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(),size);
//    test_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(),size);
    
    for(m_text_actor_max_number_of_items = 0; m_text_actor_max_number_of_items<500; m_text_actor_max_number_of_items++)
    {
        if(size[1]>m_maximum_size[1]) break;
        
        text.Append("\n Xg ");
        test_actor->SetInput(text.Get_C_String());
        
        test_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(),size);
//        test_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(),size);
    }
    
    
    return 1;
}


int vmxGUIListWidget::ComputeSelectionTextLine()
{
    if(!this->m_interactor) return 0;
    if(!this->m_interactor->GetRenderWindow()) return 0;
    if(!this->m_interactor->GetRenderWindow()->GetRenderers()) return 0;
//    if(!this->m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()) return 0;
    if(this->m_item_pointers.IsEmpty()) return 0;//list_widget not built yet.
    
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
        
        selection_character_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(),size);
//        selection_character_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(),size);
        if(size[0]>=actor_size[0]) break;
    }
    
    return 1;
}


int vmxGUIListWidget::CopySelectedItemsToClipBoard()
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



void vmxGUIListWidget::GetColor(double &r, double &g, double &b)
{
    m_text_actor->GetTextProperty()->GetColor(r,g,b);
}


void vmxGUIListWidget::GetOrigin(int &origin1, int &origin2)
{
    //currently the text actor is the first one in the list_widget
    this->GetOriginOfTextActor(origin1, origin2);
}


int vmxGUIListWidget::GetOriginOfTextActor(int &origin1, int &origin2)
{
    double *org = m_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    return 1;
}


int vmxGUIListWidget::GetOriginOfCheckBoxActor(int &origin1, int &origin2)
{
    double *org = m_checkboxes_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
    return 1;
}


int vmxGUIListWidget::GetPickedCheckBoxIndex(int picked_item_index, int pos1, int pos2)
{
//    cout<<"pos=("<<pos1<<","<<pos2<<")"<<endl;
    
    if(picked_item_index<0 || picked_item_index>=this->GetNumberOfItems()) return -1;
    if(m_items[picked_item_index].m_number_of_check_boxes==0) return -1;
//    cout<<"GetMaxNumberOfCheckBoxes="<<this->GetMaxNumberOfCheckBoxes()<<endl;
    if(this->GetMaxNumberOfCheckBoxes()==0) return -1;//this condition is not really needed, but better safe than sorry.
    if(!this->IsCheckBoxActorPicked(pos1,pos2)) return -1;
    
//    cout<<"passed_conditions"<<endl;
    int actor_size[2];
    this->GetSizeOfCheckBoxActor(actor_size[0], actor_size[1]);
    
    int actor_origin[2];
    this->GetOriginOfCheckBoxActor(actor_origin[0],actor_origin[1]);
    
    double a1 = (pos1-actor_origin[0]);
    double a2 = ((double)actor_size[0])/((double)this->GetMaxNumberOfCheckBoxes());
    int checkbox_index = a1/a2;;
    
    if(checkbox_index<0 || checkbox_index>=m_items[picked_item_index].m_number_of_check_boxes) return -1;
    
    return checkbox_index;
}


int vmxGUIListWidget::GetPickedItemIndex(int pos1, int pos2)
{
    if(!this->IsListWidgetPicked(pos1,pos2)) return -1;
    
    int list_widget_size[2];
    this->GetSizeOfTextActor(list_widget_size[0], list_widget_size[1]);
    
    int list_widget_origin[2];
    this->GetOriginOfTextActor(list_widget_origin[0], list_widget_origin[1]);
    
    int item_index;
    double a1 = (pos2-list_widget_origin[1]);
    if(a1<0) return -1;
    
    //int number_of_items_in_text_actor = m_number_of_items_in_text_actor;
    
    double a2 = ((double)list_widget_size[1])/((double)m_number_of_items_in_text_actor);
    item_index = a1/a2;
    
    item_index += this->GetNumberOfItems()-m_number_of_items_in_text_actor-m_text_index_offset;
//    cout<<"  intm_index="<<item_index<<"  ";
    
    item_index = this->GetNumberOfItems()-1-item_index;
    
//    cout<<endl;
//    cout<<"  "<<"pick_pos="<<pos1<<","<<pos2;
//    cout<<"  "<<"list_widget_origin="<<list_widget_origin[0]<<","<<list_widget_origin[1];
//    //cout<<"  "<<"pos2-orig[1]="<<pos2-dialog_origin[1];
//    cout<<"  "<<"a1="<<a1;
//    cout<<"  "<<"a2="<<a2;
//    cout<<"  "<<"a1/a2="<<a1/a2;
//    cout<<"  "<<"list_widget_size="<<list_widget_size[0]<<","<<list_widget_size[1];
//    cout<<"  "<<"number_of_items_in_text_actor="<<(number_of_items_in_text_actor);
//    cout<<"  "<<"total_number_of_items="<<(this->GetNumberOfItems());
//    cout<<"  "<<"item_index="<<item_index;
//    cout<<endl;

    
    
    if(item_index<0 || item_index>=this->GetNumberOfItems()) return -1;
    return item_index;
}


int vmxGUIListWidget::GetSizeOfCheckBoxActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
//!!!!!!!!!!!!!!! REPLACE THE ...->GetFirstRenderer() WITH THE RENDERED FOR GUI WITHIN THE MAIN WIDGET (ALSO FOR MULTIPLE PLACES IN THI FILE) !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    double size[2];
    m_checkboxes_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(), size);
//    m_checkboxes_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    
    if(size[0]==0) output_size1 = 1;
    else output_size1 = size[0];
    if(size[1]==0) output_size2 = 1;
    else output_size2 = size[1];

    
    return 1;
}


void vmxGUIListWidget::GetSize(int &output_size1, int &output_size2)
{
    int size_of_text_actor[2];
    int size_of_checkbox_actor[2];
    if(!this->GetSizeOfTextActor(size_of_text_actor[0], size_of_text_actor[1]))
    {
        output_size1 = 0;
        output_size2 = 0;
        return;
    }
    if(!this->GetSizeOfCheckBoxActor(size_of_checkbox_actor[0], size_of_checkbox_actor[1]))
    {
        output_size1 = 0;
        output_size2 = 0;
        return;
    }
    output_size1 = size_of_text_actor[0] + size_of_checkbox_actor[0];
    output_size2 = size_of_text_actor[1];
}


int vmxGUIListWidget::GetSizeOfTextActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_text_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(), size);
//    m_text_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    
    if(size[0]==0) output_size1 = 1;
    else output_size1 = size[0];
    if(size[1]==0) output_size2 = 1;
    else output_size2 = size[1];
    
    return 1;
}


int vmxGUIListWidget::GetVisibility()
{
    return m_text_actor->GetVisibility();
}


int vmxGUIListWidget::IsCheckBoxActorPicked(int pos1, int pos2)
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


int vmxGUIListWidget::IsListWidgetPicked(int pos1, int pos2)
{
    int list_widget_size[2];
    this->GetSize(list_widget_size[0],list_widget_size[1]);
    
    int list_widget_origin[2];
    this->GetOrigin(list_widget_origin[0],list_widget_origin[1]);
    
    
    int end_pos1 = list_widget_origin[0] + list_widget_size[0];
    if(pos1<list_widget_origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = list_widget_origin[1] + list_widget_size[1];
    if(pos2<list_widget_origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


int vmxGUIListWidget::IsMultipleItemsSelected()
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


int vmxGUIListWidget::IsTextActorPicked(int pos1, int pos2)
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


int vmxGUIListWidget::IsVisible()
{
    return m_text_actor->GetVisibility();
}


//void vmxGUIListWidget::ListenForRenderWindowModifiedEvent(int is_listening)
//{
//    m_window_modified_callback->Activate(is_listening);
//}


void vmxGUIListWidget::ListenForLeftButtonDownEvent(int is_listening)
{
    m_left_button_down_callback->Activate(is_listening);
}


void vmxGUIListWidget::ListenForLeftButtonUpEvent(int is_listening)
{
    m_left_button_up_callback->Activate(is_listening);
}


void vmxGUIListWidget::ListenForMouseMoveEvent(int is_listening)
{
    m_mouse_move_callback->Activate(is_listening);
}


void vmxGUIListWidget::ListenForMouseWheelBackwardEvent(int is_listening)
{
    m_mouse_wheel_backward_callback->Activate(is_listening);
}


void vmxGUIListWidget::ListenForMouseWheelForwardEvent(int is_listening)
{
    m_mouse_wheel_forward_callback->Activate(is_listening);
}


void vmxGUIListWidget::Reset()
{
    m_number_of_items_in_text_actor = 0;
    m_max_number_of_check_boxes = 0;
    
    m_picked_item_index = -1;
    m_picked_check_box_index = -1;
    m_text_index_offset = 0;
    
//    m_selection_text_character = '_';
//    m_selection_text_line.Assign(m_selection_text_character);
    
    m_items.Reset();
    m_item_pointers.Reset();
    m_text_actor->SetInput("");
    m_checkboxes_actor->SetInput("");

}


void vmxGUIListWidget::SelectSingleItem(unsigned int item_index)
{
    if(item_index>=m_item_pointers.GetNumberOfElements()) return;
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        m_item_pointers[i]->m_is_selected = 0;
    }
    m_item_pointers[item_index]->m_is_selected = 1;
}


void vmxGUIListWidget::SetColor(double r, double g, double b)
{
    m_text_actor->GetTextProperty()->SetColor(r,g,b);
    m_checkboxes_actor->GetTextProperty()->SetColor(r,g,b);
    m_selected_text_actor->GetTextProperty()->SetColor(1,1,1);
    m_selected_text_actor->GetTextProperty()->SetOpacity(0.7);
    m_drag_actor->GetTextProperty()->SetOpacity(0.5);

}


void vmxGUIListWidget::SetFontSize(double font_size)
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

}


void vmxGUIListWidget::SetIndexOffset(int offset)
{
    if(offset>this->GetNumberOfItems()-m_text_actor_max_number_of_items)
    {
        m_text_index_offset = this->GetNumberOfItems()-m_text_actor_max_number_of_items;
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
}


void vmxGUIListWidget::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
    
    //m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
    m_interactor->AddObserver(vtkCommand::LeftButtonPressEvent, m_left_button_down_callback);
    
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_text_actor);
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_checkboxes_actor);
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_selected_text_actor);
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_drag_actor);
    
//    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_text_actor);
//    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_checkboxes_actor);
//    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_selected_text_actor);
//    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_drag_actor);
//    
    m_selected_text_actor->SetVisibility(0);
    m_drag_actor->SetVisibility(0);
    
    this->ComputeSelectionTextLine();

}


void vmxGUIListWidget::SetMaximumSize(int max_size_x, int max_size_y)
{
    // Should check if the new size is different than the old one...
    //if(max_size_x!=m_maximum_size[0] || max_size_y!=m_maximum_size[1])
    if(max_size_y!=m_maximum_size[1])
    {
        m_maximum_size[0] = max_size_x;
        m_maximum_size[1] = max_size_y;
        
        // compute the max number of items member variable
        this->ComputeMaxNumberOfItemsInTextActor();
        
        // now perform resizing of the object.
        
        this->ShowTextItems();
        this->ShowSelectedItems();
    }
}


void vmxGUIListWidget::SetOrigin(int origin1, int origin2)
{
    int text_actor_size[2];
    this->GetSizeOfTextActor(text_actor_size[0], text_actor_size[1]);

    m_text_actor->SetPosition(origin1,origin2);
    m_checkboxes_actor->SetPosition(origin1+text_actor_size[0],origin2);
    m_selected_text_actor->SetPosition(origin1,origin2);
    m_placement = FIXED;
}


void vmxGUIListWidget::SetVisibility(int is_visible)
{
    if(this->GetVisibility() && is_visible) return;
    if(!this->GetVisibility() && !is_visible) return;
    
    this->m_text_actor->SetVisibility(is_visible);
    this->m_checkboxes_actor->SetVisibility(is_visible);
    this->m_selected_text_actor->SetVisibility(0);
    //    this->m_drag_actor->SetVisibility(is_visible);
    
    // If the list_widget is not visible, it should not observe interaction events
    if(!is_visible)
    {
        //m_interactor->GetRenderWindow()->RemoveObservers(vtkCommand::ModifiedEvent, m_window_modified_callback);
        this->ListenForMouseMoveEvent(0);
        this->ListenForLeftButtonUpEvent(0);
        this->ListenForLeftButtonDownEvent(0);
        this->ListenForMouseWheelBackwardEvent(0);
        this->ListenForMouseWheelForwardEvent(0);
    }
    else
    {
        //m_interactor->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
        this->ListenForLeftButtonDownEvent(1);
        
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
    }
    
    this->RedoPlacement();
}


void vmxGUIListWidget::SetVisibilityOfDragEventActor(int is_visible)
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


int vmxGUIListWidget::ShowSelectedItems()
{
    mxString selected_text;
    
    int m = 0;
    for(int i=m_text_index_offset; i<=this->GetNumberOfItems()-1 && m<m_text_actor_max_number_of_items; i++, m++)
    {
        if(m_item_pointers[i]->m_is_selected)
        {
            selected_text.Append(m_selection_text_line);
            //if(i!=0)
            if(i!=this->GetNumberOfItems()-1  && m!=m_text_actor_max_number_of_items-1)
            {
                selected_text.Append("\n");
            }
        }
        else
        {
            //if(i!=0)
            if(i!=this->GetNumberOfItems()-1  && m!=m_text_actor_max_number_of_items-1)
            {
                selected_text.Append("\n");
            }
            else
            {
                selected_text.Append(" ");
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


int vmxGUIListWidget::ShowTextItems()
{
    mxString text, checkbox_text;
    
    m_number_of_items_in_text_actor = 0;
    int m = 0;
    for(int i=m_text_index_offset; i<=this->GetNumberOfItems()-1 && m<m_text_actor_max_number_of_items; i++, m++)
    {
        m_number_of_items_in_text_actor++;
        
        text.Append(" ");
        if(!m_item_pointers[i]->m_is_controller)
        {
            text.Append("   ");
        }
        
        text.Append(m_item_pointers[i]->m_text);
        for(unsigned int k=0; k<m_item_pointers[i]->m_number_of_check_boxes; k++)
        {
            if(m_item_pointers[i]->m_checkboxes[k]==0)
            {
                checkbox_text.Append("[_]");
            }
            else
            {
                checkbox_text.Append("[x]");
            }
        }
        
        if(i!=this->GetNumberOfItems()-1 && m!=m_text_actor_max_number_of_items-1)
        {
            text.Append("\n");
            checkbox_text.Append("\n");
        }
    }
    
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
    
    m_text_actor->SetInput(text.Get_C_String());
    m_checkboxes_actor->SetInput(checkbox_text.Get_C_String());

    return 1;
}









