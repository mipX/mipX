/*=========================================================================
 
 Program:   mipx
 Module:    vmxButtonGroup.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIButtonGroup.h"




//-----------------------------------------------------------------------------------------------------



vmxButtonGroupInteractorLeftButtonUpCallback::vmxButtonGroupInteractorLeftButtonUpCallback()
{
    m_button_group = NULL;
}


vmxButtonGroupInteractorLeftButtonUpCallback::~vmxButtonGroupInteractorLeftButtonUpCallback()
{
    m_button_group = NULL;
}


void vmxButtonGroupInteractorLeftButtonUpCallback::SetTextInput(vmxButtonGroup *menu_bar)
{
    m_button_group = menu_bar;
}


vmxButtonGroupInteractorLeftButtonUpCallback* vmxButtonGroupInteractorLeftButtonUpCallback::New()
{
    return new vmxButtonGroupInteractorLeftButtonUpCallback;
}


void vmxButtonGroupInteractorLeftButtonUpCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_button_group)
    {
        if(m_button_group->m_interactor)
        {
            int pick_pos[2];
            m_button_group->m_interactor->GetEventPosition(pick_pos);
            vmxButtonGroupItem* item = m_button_group->GetPickedItem(pick_pos[0], pick_pos[1]);

            if(item)
            {
                if(m_button_group->m_button_type == vmxButtonGroup::RADIO_BUTTON)
                {
                    m_button_group->UncheckAllItems();
                    item->SetChecked(1);
                    m_button_group->ShowInputText();
                }
                if(m_button_group->m_button_type == vmxButtonGroup::CHECK_BOX)
                {
                    if(item->m_is_checked) item->SetChecked(0);
                    else item->SetChecked(1);
                    m_button_group->ShowInputText();
                }
                if(m_button_group->m_button_type == vmxButtonGroup::PUSH_BUTTON)
                {
                    if(item->m_slot)
                    {
                        item->m_slot->Execute(m_button_group);
                    }
                }

            }
            
//            int pick_pos[2];
//            m_button_group->m_interactor->GetEventPosition(pick_pos);
//            
//            m_button_group->HideMenus();
//            
//            vmxButtonGroupItem* item = m_button_group->GetPickedItem(pick_pos[0], pick_pos[1]);
//            
//            if(item)
//            {
//                int origin[2];
//                item->GetOriginOfTextActor(origin[0],origin[1]);
//                int size_of_menu[2];
//                item->m_menu.GetSize(size_of_menu[0],size_of_menu[1]);
//                item->m_menu.SetOrigin(origin[0],origin[1]-size_of_menu[1]);
//                item->m_menu.SetVisibility(1);
//            }
//            else
//            {
//                //m_button_group->HideMenus();
//            }
        }
    }
}



//-----------------------------------------------------------------------------------------------------



vmxButtonGroupItem::vmxButtonGroupItem()
{
    m_slot = NULL;
    m_is_checked = 0;
    m_button_group = NULL;
    m_text.Assign("Item");
    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxButtonGroupItem::~vmxButtonGroupItem()
{
}


void vmxButtonGroupItem::GetOriginOfTextActor(int &output_origin1, int &output_origin2)
{
    double *org = m_text_actor->GetPosition();
    output_origin1 = org[0];
    output_origin2 = org[1];
}


void vmxButtonGroupItem::GetSizeOfTextActor(int &output_size1, int &output_size2)
{
    double size[2];
    m_text_actor->GetSize(m_button_group->m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
}


int vmxButtonGroupItem::IsPicked(int pos1, int pos2)
{
    int size[2];
    this->GetSizeOfTextActor(size[0],size[1]);
    
    int origin[2];
    this->GetOriginOfTextActor(origin[0],origin[1]);
    
    int end_pos1 = origin[0] + size[0];
    if(pos1<origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = origin[1] + size[1];
    if(pos2<origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


int vmxButtonGroupItem::ShowInputText()
{
    mxString text;
    if(m_button_group->m_button_type == vmxButtonGroup::CHECK_BOX)
    {
        text.Append(" [");
        if(m_is_checked) text.Append("x] ");
        else text.Append("_] ");
        text.Append(m_text);
        text.Append(" ");
    }
    if(m_button_group->m_button_type == vmxButtonGroup::RADIO_BUTTON)
    {
        text.Append(" (");
        if(m_is_checked) text.Append("x) ");
        else text.Append("_) ");
        text.Append(m_text);
        text.Append(" ");
    }
    if(m_button_group->m_button_type == vmxButtonGroup::PUSH_BUTTON)
    {
        text.Append(" |");
        text.Append(m_text);
        text.Append("| ");
    }

    m_text_actor->SetInput(text.Get_C_String());
    return 1;
}



//-----------------------------------------------------------------------------------------------------




vmxButtonGroup::vmxButtonGroup()
{
//    m_clip_board = NULL;
    
    m_class_name.Assign("vmxButtonGroup");
    
    m_button_type = vmxButtonGroup::PUSH_BUTTON;
    
    m_description_text.Assign(" ");
    
    m_description_text_actor = vtkSmartPointer<vtkTextActor>::New();
    
    m_left_button_up_callback = vtkSmartPointer<vmxButtonGroupInteractorLeftButtonUpCallback>::New();
    m_left_button_up_callback->SetTextInput(this);
    
    m_is_stretching_over_x_axis = 0;
    
    m_interactor = NULL;
    m_font_size = 18;
}


vmxButtonGroup::~vmxButtonGroup()
{
}


vmxButtonGroup* vmxButtonGroup::New()
{
    return new vmxButtonGroup;
}


vmxButtonGroupItem* vmxButtonGroup::AddItem(const char *item_name, int is_checked)
{
    vmxButtonGroupItem *item = m_items.AddNewToEnd();
    item->m_button_group = this;
    item->m_is_checked = is_checked;
    item->m_text.Assign(item_name);
    item->ShowInputText();
    return item;
}


void vmxButtonGroup::GetOrigin(int &origin1, int &origin2)
{
    //m_items.GetBeginElement().GetOriginOfTextActor(origin1,origin2);
    
    this->GetOriginOfDescriptionText(origin1,origin2);
}


void vmxButtonGroup::GetOriginOfDescriptionText(int &origin1, int &origin2)
{
    double *org = m_description_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
}


vmxButtonGroupItem* vmxButtonGroup::GetPickedItem(int pos1, int pos2)
{
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        
        int origin[2];
        it.GetElement().GetOriginOfTextActor(origin[0],origin[1]);
        
        int end_pos1 = origin[0] + size[0];
        if(pos1>=origin[0] && pos1<=end_pos1)
        {
            int end_pos2 = origin[1] + size[1];
            if(pos2>=origin[1] && pos2<=end_pos2) return it.GetElementAddress();
        }
    }
    
    return NULL;
}


void vmxButtonGroup::GetSize(int &output_size1, int &output_size2)
{
    if(!m_interactor)
    {
        output_size1 = output_size2 = 0;
        return;
    }
    
    // size should be determined solely from the size parameter of each item.
    this->GetSizeOfDescriptionText(output_size1,output_size2);
    
    //output_size1 = output_size2 = 0;
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        output_size1 += size[0];
        if(output_size2<size[1]) output_size2 = size[1];
    }
}


void vmxButtonGroup::GetSizeOfDescriptionText(int &output_size1, int &output_size2)
{
    if(!m_interactor)
    {
        output_size1 = output_size2 = 0;
        return;
    }
    double size_description_text[2];
    m_description_text_actor->GetSize(m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size_description_text);

    output_size1 = size_description_text[0];
    output_size2 = size_description_text[1];
}


int vmxButtonGroup::IsPicked(int pos1, int pos2)
{
    if(!m_interactor)
    {
        return 0;
    }
    
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


int vmxButtonGroup::IsVisible()
{
//    if(m_items.IsEmpty())
//    {
//        return 0;
//    }
    
    return m_description_text_actor->GetVisibility();
}


void vmxButtonGroup::SetButtonTypeToCheckBox()
{
    m_button_type = vmxButtonGroup::CHECK_BOX;
}


void vmxButtonGroup::SetButtonTypeToRadioButton()
{
    m_button_type = vmxButtonGroup::RADIO_BUTTON;
}


void vmxButtonGroup::SetButtonTypeToPushButton()
{
    m_button_type = vmxButtonGroup::PUSH_BUTTON;
}


void vmxButtonGroup::SetColor(double r, double g, double b)
{
    m_description_text_actor->GetTextProperty()->SetColor(r,g,b);
    
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->GetTextProperty()->SetColor(r,g,b);
    }
}


void vmxButtonGroup::SetDescriptionText(const char *description_text)
{
    m_description_text.Assign(description_text);
}


void vmxButtonGroup::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;
    
    m_description_text_actor->GetTextProperty()->SetFontFamilyToArial();
    m_description_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        //    m_text_actor->GetTextProperty()->SetFontFamilyToCourier();
        //    m_text_actor->GetTextProperty()->SetFontFamilyToTimes();
        it.GetElement().m_text_actor->GetTextProperty()->SetFontFamilyToArial();
        it.GetElement().m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    }
}


void vmxButtonGroup::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
    
    m_interactor->AddObserver(vtkCommand::EndInteractionEvent, m_left_button_up_callback);
    
    m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(m_description_text_actor);
    
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(it.GetElement().m_text_actor);
    }
}


void vmxButtonGroup::SetMaximumSize(int max_size_x, int max_size_y)
{
    m_maximum_size[1] = 30;
    
//    // Should check if the new size is different than the old one...
//    //if(max_size_x!=m_maximum_size[0] || max_size_y!=m_maximum_size[1])
//    if(max_size_y!=m_maximum_size[1])
//    {
//        m_maximum_size[0] = max_size_x;
//        m_maximum_size[1] = max_size_y;
//        
////        // compute the max number of items member variable
////        this->ComputeMaxNumberOfItemsInTextActor();
//        
//        // now perform resizing of the object.
//        
        this->ShowInputText();
////        this->ShowSelectedItems();
//    }
}

void vmxButtonGroup::SetOrigin(int origin1, int origin2)
{
//    int text_actor_size[2];
//    this->GetSize(text_actor_size[0], text_actor_size[1]);
    
    // all items need to be alligned with respect to their upper edges, hence we need to find the max y size
    
    int size_of_description_text_actor[2];
    this->GetSizeOfDescriptionText(size_of_description_text_actor[0], size_of_description_text_actor[1]);
    
    int max_y_size = size_of_description_text_actor[1]; //int max_y_size = 0;
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        if(max_y_size<size[1]) max_y_size = size[1];
    }

    
    // origin x value will change for each item
    int origin_x = origin1;
    
    // first "iteration" takes into account the description text.
    {
        int size[2];
        this->GetSizeOfDescriptionText(size[0],size[1]);
        
        m_description_text_actor->SetPosition(origin_x,origin2+max_y_size-size[1]);
        
        origin_x += size[0];
    }
    
    //mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        
        it.GetElement().m_text_actor->SetPosition(origin_x,origin2+max_y_size-size[1]);
        
        origin_x += size[0];
        //cout<<endl<<"origin_x="<<origin_x<<endl;
    }
    
    m_placement = FIXED;
}


void vmxButtonGroup::SetVisibility(int is_visible)
{
    this->ShowInputText();
    
    m_description_text_actor->SetVisibility(is_visible);
    
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->SetVisibility(is_visible);
    }

    this->RedoPlacement();
    
    //vmxGUIObject::SetVisibility(is_visible);
}


int vmxButtonGroup::ShowInputText()
{
    m_description_text_actor->SetInput(m_description_text.Get_C_String());
    
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().ShowInputText();
    }
    
    return 1;
}


void vmxButtonGroup::UncheckAllItems()
{
    mxListIterator< vmxButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().SetChecked(0);
    }
}








