/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIMenu.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIMenu.h"








vmxGUIMenuItem::vmxGUIMenuItem()
{
    m_is_selected = 0;
    m_slot = NULL;
    m_menu = NULL;
    m_sub_menu = NULL;
    m_data = NULL;
    
    m_number_of_check_boxes = 0;
    for(unsigned int i=0; i<m_number_of_check_boxes; i++)
    {
        m_checkboxes[i] = 0;
    }
}


vmxGUIMenuItem::~vmxGUIMenuItem()
{
}


void vmxGUIMenuItem::AttachSlot(vmxGUISlotFunction slot)
{
    m_slot = slot;
}


vmxGUISlotFunction vmxGUIMenuItem::GetSlot()
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
    
    m_picked_item = NULL;
    
    m_placement_relative_percentages[0] = m_placement_relative_percentages[1] = 0;

    m_interactor = NULL;
    m_font_size = 18;
    m_selection_text_character = '_';
    m_selection_text_line.Assign(m_selection_text_character);
    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_selected_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_checkboxes_actor = vtkSmartPointer<vtkTextActor>::New();
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
    }
    
    m_checkboxes_actor->SetInput(checkbox_text.Get_C_String());
    
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
        selection_character_actor->GetSize(m_main_widget->GetRenderer_GUI(),size); //selection_character_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(),size);
        if(size[0]>=actor_size[0]) break;
    }
    
    return 1;
}


int vmxGUIMenu::CopySelectedItemsToClipBoard()
{
    if(!m_clip_board) return 0;
    
    m_clip_board->m_data_text.Clear();
    for(int i=0; i<this->GetNumberOfItems(); i++)
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


vmxGUIMenuItem* vmxGUIMenu::GetItem(const char *item_name)
{
    mxListIterator<vmxGUIMenuItem> it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElementAddress()->m_text == item_name)
        {
            vmxGUIMenuItem *item = it.GetElementAddress();
            return item;
        }
    }
    return NULL;
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
    
    if(checkbox_index<0 || checkbox_index>=(int)m_items[picked_item_index].m_number_of_check_boxes) return -1;
    
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


vmxGUIMenu* vmxGUIMenu::GetPickedSubMenu(int pos1, int pos2)
{
    // Call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            if(m_item_pointers[i]->GetSubMenu()->IsVisible())
            {
                // find the picked submenu...
                if(m_item_pointers[i]->GetSubMenu()->IsPicked(pos1, pos2))
                {
                    return m_item_pointers[i]->GetSubMenu();
                }
                else
                {
                    return (m_item_pointers[i]->GetSubMenu()->GetPickedSubMenu(pos1, pos2));
                }
            }
        }
    }
    return NULL;
}


int vmxGUIMenu::GetSizeOfCheckBoxActor(int &output_size1, int &output_size2)
{
    if(!m_interactor) return 0;
    
    double size[2];
    m_checkboxes_actor->GetSize(m_main_widget->GetRenderer_GUI(), size);
    
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
    m_text_actor->GetSize(m_main_widget->GetRenderer_GUI(), size);
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
    for(int i=0; i<this->GetNumberOfItems(); i++)
    {
        if(m_item_pointers[i]->m_is_selected)
        {
            c++;
            if(c>1) return 1;
        }
    }
    return 0;
}


int vmxGUIMenu::IsSubMenuPicked(int pos1, int pos2)
{
    // Call this method for each submenu
    for(unsigned int i=0; i<m_item_pointers.GetNumberOfElements(); i++)
    {
        if(m_item_pointers[i]->GetSubMenu())
        {
            if(m_item_pointers[i]->GetSubMenu()->IsVisible())
            {
                if(m_item_pointers[i]->GetSubMenu()->IsPicked(pos1, pos2)) return 1;
                else
                {
                    return (m_item_pointers[i]->GetSubMenu()->IsSubMenuPicked(pos1, pos2));
                }
            }
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


void vmxGUIMenu::OnLeftButtonUp()
{
    //cout<<" vmxGUIMenu::OnLeftButtonUp() ";
    
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);
    
    int index = this->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
    
    
    vmxGUIMenuItem *item = this->GetItem(index);
    if(item)
    {
        this->HideAllSubMenus();
        
        m_picked_item = item;
        
        if(item->m_sub_menu)
        {
            int *window_size = this->m_interactor->GetRenderWindow()->GetSize();
            
            int origin[2];
            this->GetOrigin(origin[0], origin[1]);
            int menu_size[2];
            this->GetSize(menu_size[0],menu_size[1]);
            item->m_sub_menu->SetVisibility(1);
            int new_origin[2];
            new_origin[0] = origin[0]+menu_size[0];
            new_origin[1] = origin[1] + (menu_size[1]*index)/this->GetNumberOfItems();
            
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
            this->InvokeEvent(LeftButtonUpEvent, this, item);
            this->SetVisibility(0);
        }
    }
    else
    {
        this->SetVisibility(0);
    }
    
    this->m_interactor->Render();
}


void vmxGUIMenu::OnLeftButtonDown()
{
    //cout<<" vmxGUIMenu::OnLeftButtonDown() ";
    
    if(this->m_clip_board) this->m_clip_board->m_is_valid = 0;
    
    
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);
    
    int index = this->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
    if(index<0)
    {
        vmxGUIMenu* picked_submenu = this->GetPickedSubMenu(pick_pos[0], pick_pos[1]);
        
        if(picked_submenu)
        {
            int submenu_item_index = picked_submenu->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
            if(submenu_item_index<0)
            {
                this->SetVisibility(0);
                this->m_interactor->Render();
                return;
            }
            
            vmxGUIMenuItem *submenu_item = picked_submenu->GetItem(submenu_item_index);
            if(submenu_item)
            {
                if(!submenu_item->GetSubMenu())// if the picked item does not contain a submenu, hide the current menu.
                {
                    this->SetVisibility(0);
                    this->m_interactor->Render();
                    return;
                }
            }
        }
        else
        {
            this->SetVisibility(0);
            this->m_interactor->Render();
            return;
        }
    }
}


void vmxGUIMenu::OnMouseMove()
{
    //cout<<" vmxGUIMenu::OnMouseMove() ";
    
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);
    
    int index = this->GetPickedItemIndex(pick_pos[0], pick_pos[1]);
    
    if(index<0) return; //ADDED!!!!!!

    this->SelectSingleItem(index);
    
    this->ShowSelectedItems();
    
    this->m_interactor->Render();
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
    
    m_text_actor->GetTextProperty()->SetFontFamilyToArial();
    m_checkboxes_actor->GetTextProperty()->SetFontFamilyToArial();
    m_selected_text_actor->GetTextProperty()->SetFontFamilyToArial();
    
    m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    m_checkboxes_actor->GetTextProperty()->SetFontSize(m_font_size);
    m_selected_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    
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
    
    m_main_widget->GetRenderer_GUI()->AddActor2D(m_text_actor);
    m_main_widget->GetRenderer_GUI()->AddActor2D(m_checkboxes_actor);
    m_main_widget->GetRenderer_GUI()->AddActor2D(m_selected_text_actor);
    
    m_selected_text_actor->SetVisibility(0);
    
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


void vmxGUIMenu::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    vmxGUIWidget::SetMainWidget(main_widget);
    this->SetClipBoard(main_widget->GetClipBoard());
    this->SetInteractor(m_main_widget->GetInteractor());
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
    this->m_selected_text_actor->SetVisibility(0);
    
    // If the menu is not visible, it should not observe interaction events
    if(!is_visible)
    {
        this->SetListeningFor_LeftButtonUp_Event(0);
        this->SetListeningFor_LeftButtonDown_Event(0);
        this->SetListeningFor_MouseMove_Event(0);
    }
    else
    {

        this->SetListeningFor_MouseMove_Event(1);
        this->SetListeningFor_LeftButtonDown_Event(1);
    }
    
    this->RedoPlacement();
}


int vmxGUIMenu::ShowSelectedItems()
{
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


int vmxGUIMenu::ShowTextItems()
{
    mxString text;
    mxString checkbox_text;
    
    for(int i=m_item_pointers.GetNumberOfElements()-1; i>=0; i--)
    {
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
    
    return 1;
}








