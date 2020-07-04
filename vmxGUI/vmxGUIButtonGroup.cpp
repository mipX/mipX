/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIButtonGroup.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIButtonGroup.h"




//-----------------------------------------------------------------------------------------------------



vmxGUIButtonGroupItem::vmxGUIButtonGroupItem()
{
    m_slot = NULL;
    m_is_checked = 0;
    m_button_group = NULL;
    m_text.Assign("Item");
    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxGUIButtonGroupItem::~vmxGUIButtonGroupItem()
{
}


void vmxGUIButtonGroupItem::GetOriginOfTextActor(int &output_origin1, int &output_origin2)
{
    double *org = m_text_actor->GetPosition();
    output_origin1 = org[0];
    output_origin2 = org[1];
}


void vmxGUIButtonGroupItem::GetSizeOfTextActor(int &output_size1, int &output_size2)
{
    double size[2];
    m_text_actor->GetSize(m_button_group->GetMainWidget()->GetRenderer_GUI(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
}


int vmxGUIButtonGroupItem::IsPicked(int pos1, int pos2)
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


int vmxGUIButtonGroupItem::ShowInputText()
{
    mxString text;
    if(m_button_group->m_button_type == vmxGUIButtonGroup::CHECK_BOX)
    {
        text.Append(" [");
        if(m_is_checked) text.Append("x] ");
        else text.Append("_] ");
        text.Append(m_text);
        text.Append(" ");
    }
    if(m_button_group->m_button_type == vmxGUIButtonGroup::RADIO_BUTTON)
    {
        text.Append(" (");
        if(m_is_checked) text.Append("x) ");
        else text.Append("_) ");
        text.Append(m_text);
        text.Append(" ");
    }
    if(m_button_group->m_button_type == vmxGUIButtonGroup::PUSH_BUTTON)
    {
        text.Append(" |");
        text.Append(m_text);
        text.Append("| ");
    }

    m_text_actor->SetInput(text.Get_C_String());
    return 1;
}



//-----------------------------------------------------------------------------------------------------




vmxGUIButtonGroup::vmxGUIButtonGroup()
{
    m_class_name.Assign("vmxGUIButtonGroup");
    
    m_button_type = vmxGUIButtonGroup::PUSH_BUTTON;
    
    m_description_text.Assign(" ");
    
    m_description_text_actor = vtkSmartPointer<vtkTextActor>::New();
    
    m_interactor = NULL;
    m_font_size = 18;
    
    m_last_pushed_item = NULL;
}


vmxGUIButtonGroup::~vmxGUIButtonGroup()
{
}


vmxGUIButtonGroup* vmxGUIButtonGroup::New()
{
    return new vmxGUIButtonGroup;
}


vmxGUIButtonGroupItem* vmxGUIButtonGroup::AddItem(const char *item_name, int is_checked)
{
    vmxGUIButtonGroupItem *item = m_items.AddNewToEnd();
    item->m_button_group = this;
    item->m_is_checked = is_checked;
    item->m_text.Assign(item_name);
    item->ShowInputText();
    m_main_widget->GetRenderer_GUI()->AddActor2D(item->m_text_actor);

    return item;
}


void vmxGUIButtonGroup::GetCheckedItems(mxArray< vmxGUIButtonGroupItem* > &array_of_checked_items)
{
    int n=0;
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement().m_is_checked) n++;
    }
    
    array_of_checked_items.SetNumberOfElements(n);
    
    int i=0;
    for(it.SetToBegin(m_items), i=0; it.IsValid() && i<(int)array_of_checked_items.GetNumberOfElements(); it.MoveToNext())
    {
        if(it.GetElement().m_is_checked)
        {
            array_of_checked_items[i] = it.GetElementAddress();
            i++;
        }
    }
}


void vmxGUIButtonGroup::GetOrigin(int &origin1, int &origin2)
{
    this->GetOriginOfDescriptionText(origin1,origin2);
    
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int org[2];
        it.GetElement().GetOriginOfTextActor(org[0],org[1]);
        if(origin1>org[0]) origin1 = org[0];
        if(origin2>org[1]) origin2 = org[1];
    }
    
}


void vmxGUIButtonGroup::GetOriginOfDescriptionText(int &origin1, int &origin2)
{
    double *org = m_description_text_actor->GetPosition();
    origin1 = org[0];
    origin2 = org[1];
}


vmxGUIButtonGroupItem* vmxGUIButtonGroup::GetPickedItem(int pos1, int pos2)
{
    mxListIterator< vmxGUIButtonGroupItem > it;
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


void vmxGUIButtonGroup::GetSize(int &output_size1, int &output_size2)
{
    if(!m_interactor)
    {
        output_size1 = output_size2 = 0;
        return;
    }
    
    // size should be determined solely from the size parameter of each item.
    this->GetSizeOfDescriptionText(output_size1,output_size2);
    
    //cout<<"vmxGUIButtonGroup::GetSize(): description text size (x,y): "<<output_size1<<","<<output_size2<<endl;
    //cout<<"vmxGUIButtonGroup::GetSize(): number of items: "<<m_items.GetNumberOfElements()<<endl;

    
    //output_size1 = output_size2 = 0;
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        output_size1 += size[0];
        if(output_size2<size[1]) output_size2 = size[1];
    }
}


void vmxGUIButtonGroup::GetSizeOfDescriptionText(int &output_size1, int &output_size2)
{
    if(!m_interactor)
    {
        output_size1 = output_size2 = 0;
        return;
    }
    double size_description_text[2];
    m_description_text_actor->GetSize(this->GetMainWidget()->GetRenderer_GUI(), size_description_text);

    output_size1 = size_description_text[0];
    output_size2 = size_description_text[1];
}


int vmxGUIButtonGroup::IsPicked(int pos1, int pos2)
{
    if(!m_interactor)
    {
        return 0;
    }
    
    //cout<<"vmxGUIButtonGroup::IsPicked() pos="<<pos1<<","<<pos2<<endl;
    
    int size[2];
    this->GetSize(size[0],size[1]);
    
    //cout<<"vmxGUIButtonGroup::IsPicked() size="<<size[0]<<","<<size[1]<<endl;
    
    int origin[2];
    this->GetOrigin(origin[0],origin[1]);
    
    //cout<<"vmxGUIButtonGroup::IsPicked() origin="<<origin[0]<<","<<origin[1]<<endl;
    
    int end_pos1 = origin[0] + size[0];
    if(pos1<origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = origin[1] + size[1];
    if(pos2<origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


int vmxGUIButtonGroup::IsVisible()
{
    return m_description_text_actor->GetVisibility();
}


void vmxGUIButtonGroup::OnLeftButtonUp()
{
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);
    vmxGUIButtonGroupItem* item = this->GetPickedItem(pick_pos[0], pick_pos[1]);
    
    if(item)
    {
        if(this->m_button_type == vmxGUIButtonGroup::RADIO_BUTTON)
        {
            this->UncheckAllItems();
            item->SetChecked(1);
            this->ShowInputText();
            this->m_main_widget->GetInteractor()->Render();
        }
        if(this->m_button_type == vmxGUIButtonGroup::CHECK_BOX)
        {
            if(item->m_is_checked) item->SetChecked(0);
            else item->SetChecked(1);
            this->ShowInputText();
            this->m_main_widget->GetInteractor()->Render();
        }
        if(this->m_button_type == vmxGUIButtonGroup::PUSH_BUTTON)
        {
            m_last_pushed_item = item;
            if(item->m_slot)
            {
                item->m_slot->Execute(this);
            }
        }
        //std::cout<<endl<<" vmxGUIButtonGroup::OnLeftButtonUp() called this->InvokeEvent()";
        this->InvokeEvent(ButtonClickedEvent, this, item);
    }
}


void vmxGUIButtonGroup::SetButtonTypeToCheckBox()
{
    m_button_type = vmxGUIButtonGroup::CHECK_BOX;
}


void vmxGUIButtonGroup::SetButtonTypeToRadioButton()
{
    m_button_type = vmxGUIButtonGroup::RADIO_BUTTON;
}


void vmxGUIButtonGroup::SetButtonTypeToPushButton()
{
    m_button_type = vmxGUIButtonGroup::PUSH_BUTTON;
}


void vmxGUIButtonGroup::SetColor(double r, double g, double b)
{
    m_description_text_actor->GetTextProperty()->SetColor(r,g,b);
    
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->GetTextProperty()->SetColor(r,g,b);
    }
}


void vmxGUIButtonGroup::SetDescriptionText(const char *description_text)
{
    m_description_text.Assign(description_text);
}


void vmxGUIButtonGroup::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;
    
    m_description_text_actor->GetTextProperty()->SetFontFamilyToArial();
    m_description_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->GetTextProperty()->SetFontFamilyToArial();
        it.GetElement().m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    }
}


void vmxGUIButtonGroup::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
}


void vmxGUIButtonGroup::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    this->vmxGUIWidget::SetMainWidget(main_widget);

    this->SetInteractor(main_widget->GetInteractor());
    
    main_widget->GetRenderer_GUI()->AddActor2D(this->m_description_text_actor);
 
}


void vmxGUIButtonGroup::SetMaximumSize(int max_size_x, int max_size_y)
{
    m_maximum_size[1] = 30;
    
    this->ShowInputText();
}

void vmxGUIButtonGroup::SetOrigin(int origin1, int origin2)
{
//    int text_actor_size[2];
//    this->GetSize(text_actor_size[0], text_actor_size[1]);
    
    // all items need to be alligned with respect to their upper edges, hence we need to find the max y size
    
    int size_of_description_text_actor[2];
    this->GetSizeOfDescriptionText(size_of_description_text_actor[0], size_of_description_text_actor[1]);
    
    int max_y_size = size_of_description_text_actor[1]; //int max_y_size = 0;
    mxListIterator< vmxGUIButtonGroupItem > it;
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
    
    //mxListIterator< vmxGUIButtonGroupItem > it;
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


void vmxGUIButtonGroup::SetVisibility(int is_visible)
{
    this->ShowInputText();
    
    m_description_text_actor->SetVisibility(is_visible);
    
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->SetVisibility(is_visible);
    }

    this->RedoPlacement();
    
    //vmxGUIObject::SetVisibility(is_visible);
}


int vmxGUIButtonGroup::ShowInputText() // THIS IS LIKE AN UPDATE METHOD!!!!!!!!!!!!!
{
    m_description_text_actor->SetInput(m_description_text.Get_C_String());
    
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().ShowInputText();
    }
    
    return 1;
}


void vmxGUIButtonGroup::UncheckAllItems()
{
    mxListIterator< vmxGUIButtonGroupItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().SetChecked(0);
    }
}








