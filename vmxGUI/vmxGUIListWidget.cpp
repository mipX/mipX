/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIListWidget.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIListWidget.h"





vmxGUIListWidgetItem::vmxGUIListWidgetItem()
{
    /// By default all items are controllers, i.e. they are all on level 0.
    m_is_exclusively_checked = 0;
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



//-----------------------------------------------------------------------------------------------------



vmxGUIListWidget::vmxGUIListWidget()
{
    m_clip_board = NULL;
    
    m_class_name.Assign("vmxGUIListWidget");
    
//    this->SetAccepting_LeftButtonDoubleClick_Event(1); // ADDED THIS !!!!!!!!!!!!!!!!!!!!!!!!!
    this->SetAccepting_LeftButtonDrag_Event(1); // ADDED THIS !!!!!!!!!!!!!!!!!!!!!!!!!
//    this->SetAccepting_LeftButtonDrop_Event(1); // ADDED THIS !!!!!!!!!!!!!!!!!!!!!!!!!
    
    
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
    
    m_last_selected_item = NULL;
    m_last_selected_item_data = NULL;


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
    
    vtkSmartPointer<vtkTextActor> test_actor = vtkSmartPointer<vtkTextActor>::New();
    test_actor->GetTextProperty()->SetFontSize(m_font_size);
    
    double size[2];
    mxString text;
    text.Assign(" Xg ");
    
    test_actor->SetInput(text.Get_C_String());
    
    test_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(),size);
    
    for(m_text_actor_max_number_of_items = 0; m_text_actor_max_number_of_items<500; m_text_actor_max_number_of_items++)
    {
        if(size[1]>m_maximum_size[1]) break;
        
        text.Append("\n Xg ");
        test_actor->SetInput(text.Get_C_String());
        
        test_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(),size);
    }
    
    return 1;
}


int vmxGUIListWidget::ComputeSelectionTextLine()
{
    if(!this->m_interactor) return 0;
    if(!this->m_interactor->GetRenderWindow()) return 0;
    if(!this->m_interactor->GetRenderWindow()->GetRenderers()) return 0;
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



void vmxGUIListWidget::GetSelectedItems(mxArray<vmxGUIListWidgetItem*> &array_of_selected_items)
{
    int number_of_selected_items = 0;
    for(int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->IsSelected())
        {
            number_of_selected_items++;
        }
    }
    
    array_of_selected_items.SetNumberOfElements(number_of_selected_items);
    int n=0;
    for(int i=0; i<m_item_pointers.GetNumberOfElements() && n<number_of_selected_items; i++)
    {
        if(m_item_pointers[i]->IsSelected())
        {
            array_of_selected_items[n] = m_item_pointers[i];
            n++;
        }
    }
}


void vmxGUIListWidget::GetSelectedItemsNames(mxArray<mxString> &array_of_selected_items_names)
{
    int number_of_selected_items = 0;
    for(int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->IsSelected())
        {
            number_of_selected_items++;
        }
    }
    
    array_of_selected_items_names.SetNumberOfElements(number_of_selected_items);
    int n=0;
    for(int i=0; i<m_item_pointers.GetNumberOfElements() && n<number_of_selected_items; i++)
    {
        if(m_item_pointers[i]->IsSelected())
        {
            array_of_selected_items_names[n].Assign(m_item_pointers[i]->m_text);
            n++;
        }
    }
}


int vmxGUIListWidget::GetSizeOfCheckBoxActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_checkboxes_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(), size);
    
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


int vmxGUIListWidget::HasVTKActor2D(vtkActor2D *actor_2D)
{
    void *i = (void*) actor_2D;

    void *text_actor = (void *) (this->m_text_actor);
    if(text_actor==i) return 1;
    
    void *checkboxes_actor = (void *) (this->m_checkboxes_actor);
    if(checkboxes_actor==i) return 1;
    
    return 0;
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


void vmxGUIListWidget::OnKeyPress()
{
    // do nothing.
}

void vmxGUIListWidget::OnMouseMove()
{
    //do nothing.
}


void vmxGUIListWidget::OnLeftButtonUp()
{
    //cout<<" vmxGUIListWidget::OnLeftButtonUp() ";
    
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);

    this->m_item_selection_detector.OnLeftButtonUp();
    
    
    // Turn off listening for button up events. They will be turned on in button down
    // event (as soon as left button is pressed).
    this->SetListeningFor_LeftButtonUp_Event(0);
    this->SetVisibilityOfDragEventActor(0);
    this->m_interactor->Render();

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ADDED THIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //if(this->m_item_selection_detector.IsSingleSelectionDetected())
    {
        //this->GetConnectionManager();
        this->GetConnectionManager()->Execute(ItemSelectedEvent, this);//, NULL);
        
//!!!!!!!! THERE SHOULD BE A SEPARATE CHECKING IF AN ITEM WAS ACTUALLY MODIFIED AND THE EVENT SHOULD ONLY THEN BE CALLED !!!!!!!!!!
        this->GetConnectionManager()->Execute(ItemModifiedEvent, this);//, NULL);
    }
}


void vmxGUIListWidget::OnLeftButtonDown()
{
    //cout<<" vmxGUIListWidget::OnLeftButtonDown() ";
    
    if(this->GetClipBoard()) this->GetClipBoard()->m_is_valid = 0;
    
    /// Reset the picked indexes to non-valid values.
    this->m_picked_item_index = -1;
    this->m_picked_check_box_index = -1;
    
    
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);
    
    
    
    int index = this->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
    if(index<0)
    {
        return;
    }

    
    this->m_item_selection_detector.OnLeftButtonDown(this->m_interactor->GetControlKey(), this->m_item_pointers[index]->m_is_selected, this->IsMultipleItemsSelected());
    
    this->m_picked_item_index = index;
    
    
    
    if(this->m_item_selection_detector.IsMultipleSelectionDetected())
    {
        // if multiple selection is detected (CTRL key is pressed), jus add/remove items from selection
        this->GetItem(index)->m_is_selected = !(this->GetItem(index)->m_is_selected);
    }
    else
    {
        // If CTRL is not pressed, we need to check if a single selection is performed or a drag event of multiple selected items.
        if(this->m_item_selection_detector.IsSingleSelectionDetected())
        {
            // If there is no drag for sure, just perform single selection
            this->SelectSingleItem(index);
            this->m_last_selected_item = this->GetItem(index);
            this->m_last_selected_item_data = this->GetItem(index)->GetData();
        }
    }
    
    this->SetListeningFor_LeftButtonUp_Event(1);
    
    this->ShowSelectedItems();
    
    int check_index = this->GetPickedCheckBoxIndex(index,pick_pos[0],pick_pos[1]);
    if(check_index>=0)
    {
        if(this->m_items[index].m_checkboxes[check_index])
        {
            this->m_items[index].m_checkboxes[check_index] = 0;
        }
        else
        {
            this->m_items[index].m_checkboxes[check_index] = 1;
        }
        this->m_picked_check_box_index = check_index;
        this->ShowTextItems();
    }
    
    
    // Execute the mouse left click slot (if attached).
    if(this->GetItem(index)->m_mouse_left_click_slot)
    {
        this->GetItem(index)->m_mouse_left_click_slot->Execute(this->GetItem(index), check_index);
    }

    
    // Call the on item picked event.
    this->OnItemPicked();

}



void vmxGUIListWidget::OnLeftButtonDoubleClick()
{
    //            // If double click was performed execute double click slot...
    //            if(m_list_widget->m_double_click_detector.IsDoubleClicked())
    //            {
    //                // this part executes on detected double click.
    //            }
    
    // do nothing.
}


void vmxGUIListWidget::OnLeftButtonDrag()
{
    //cout<<" vmxGUIListWidget::OnLeftButtonDrag() ";
    
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);
    
   this->SetListeningFor_LeftButtonUp_Event(1);
    
    // perform copying to clipboard of selected data.
    this->CopySelectedItemsToClipBoard();
    this->SetVisibilityOfDragEventActor(1);
    this->m_drag_actor->SetPosition(pick_pos[0],pick_pos[1]);
    this->m_interactor->Render();

}


void vmxGUIListWidget::OnLeftButtonDrop()
{
    // do nothing.
}


void vmxGUIListWidget::OnMouseWheelForward()
{
    if(this->m_interactor)
    {
        int pick_pos[2];
        this->m_interactor->GetEventPosition(pick_pos);
        
        int index = this->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
        if(index<0) return;
        
        int step = 4;
        
        this->SetIndexOffset(this->m_text_index_offset-step);
        this->ShowTextItems();
        this->ShowSelectedItems();
        this->RepositionAfterResizing();
        
        this->m_interactor->Render();
    }
}


void vmxGUIListWidget::OnMouseWheelBackward()
{
    if(this->m_interactor)
    {
        int pick_pos[2];
        this->m_interactor->GetEventPosition(pick_pos);
        
        int index = this->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
        if(index<0) return;
        
        int step = 4;
        
        this->SetIndexOffset(this->m_text_index_offset+step);
        this->ShowTextItems();
        this->ShowSelectedItems();
        this->RepositionAfterResizing();
        
        this->m_interactor->Render();
    }
}




void vmxGUIListWidget::Reset()
{
    m_number_of_items_in_text_actor = 0;
    m_max_number_of_check_boxes = 0;
    
    m_picked_item_index = -1;
    m_picked_check_box_index = -1;
    m_text_index_offset = 0;
    
    m_items.Reset();
    m_item_pointers.Reset();
    m_text_actor->SetInput("");
    m_checkboxes_actor->SetInput("");
    
    m_last_selected_item = NULL;
    m_last_selected_item_data = NULL;


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
    
    
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_text_actor);
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_checkboxes_actor);
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_selected_text_actor);
    this->GetMainWidget()->GetRenderer_GUI()->AddActor2D(m_drag_actor);
    
    m_selected_text_actor->SetVisibility(0);
    m_drag_actor->SetVisibility(0);
    
    this->ComputeSelectionTextLine();
}


void vmxGUIListWidget::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    this->vmxGUIWidget::SetMainWidget(main_widget);

    this->SetClipBoard(main_widget->GetClipBoard());
    this->SetInteractor(main_widget->GetInteractor());
    
}


void vmxGUIListWidget::SetMaximumSize(int max_size_x, int max_size_y)
{
    // Should check if the new size is different than the old one...
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
    
    // If the list_widget is not visible, it should not observe interaction events
    if(!is_visible)
    {
        this->SetListeningFor_MouseMove_Event(0);
        this->SetListeningFor_LeftButtonUp_Event(0);
        this->SetListeningFor_LeftButtonDown_Event(0);
        this->SetListeningFor_MouseWheelBackward_Event(0);
        this->SetListeningFor_MouseWheelForward_Event(0);
    }
    else
    {
        this->SetListeningFor_LeftButtonDown_Event(1);
        
        if(m_number_of_items_in_text_actor<this->GetNumberOfItems())
        {
            this->SetListeningFor_MouseWheelBackward_Event(1);
            this->SetListeningFor_MouseWheelForward_Event(1);
        }
        else
        {
            this->SetListeningFor_MouseWheelBackward_Event(0);
            this->SetListeningFor_MouseWheelForward_Event(0);
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
            if(i!=this->GetNumberOfItems()-1  && m!=m_text_actor_max_number_of_items-1)
            {
                selected_text.Append("\n");
            }
        }
        else
        {
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
        this->SetListeningFor_MouseWheelBackward_Event(1);
        this->SetListeningFor_MouseWheelForward_Event(1);
    }
    else
    {
        this->SetListeningFor_MouseWheelBackward_Event(0);
        this->SetListeningFor_MouseWheelForward_Event(0);
    }
    
    m_text_actor->SetInput(text.Get_C_String());
    m_checkboxes_actor->SetInput(checkbox_text.Get_C_String());

    return 1;
}









