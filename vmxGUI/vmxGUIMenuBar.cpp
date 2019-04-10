/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIMenuBar.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIMenuBar.h"




//-----------------------------------------------------------------------------------------------------




vmxGUIMenuBarItem::vmxGUIMenuBarItem()
{
    m_menu_bar = NULL;
    m_text.Assign("Item");
    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
}


vmxGUIMenuBarItem::~vmxGUIMenuBarItem()
{
}


void vmxGUIMenuBarItem::GetOriginOfTextActor(int &output_origin1, int &output_origin2)
{
    double *org = m_text_actor->GetPosition();
    output_origin1 = org[0];
    output_origin2 = org[1];
}


void vmxGUIMenuBarItem::GetSizeOfTextActor(int &output_size1, int &output_size2)
{
    double size[2];
    m_text_actor->GetSize(m_menu_bar->m_interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer(), size);
    
    output_size1 = size[0];
    output_size2 = size[1];
}


int vmxGUIMenuBarItem::IsPicked(int pos1, int pos2)
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


int vmxGUIMenuBarItem::ShowInputText()
{
    mxString text;
    text.Append("  ");
    text.Append(m_text);
    text.Append("  ");
    m_text_actor->SetInput(text.Get_C_String());
    return 1;
}



//-----------------------------------------------------------------------------------------------------




vmxGUIMenuBar::vmxGUIMenuBar()
{
    m_clip_board = NULL;
    
    m_class_name.Assign("vmxGUIMenuBar");
    
    m_is_stretching_over_x_axis = 1;
    
    m_interactor = NULL;
    m_font_size = 18;
}


vmxGUIMenuBar::~vmxGUIMenuBar()
{
}


vmxGUIMenuBar* vmxGUIMenuBar::New()
{
    return new vmxGUIMenuBar;
}


vmxGUIMenuBarItem* vmxGUIMenuBar::AddItem(const char *item_name)
{
    vmxGUIMenuBarItem *item = m_items.AddNewToEnd();
    item->m_menu_bar = this;
    item->m_menu.SetMainWidget(m_main_widget);
    item->m_menu.SetColor(1,0,0);
    item->m_menu.SetFontSize(m_font_size);
    item->m_text.Assign(item_name);
    item->ShowInputText();
    return item;
}


void vmxGUIMenuBar::GetOrigin(int &origin1, int &origin2)
{
    if(m_items.IsEmpty())
    {
        origin1 = origin2 = 0;
        return;
    }
    m_items.GetBeginElement().GetOriginOfTextActor(origin1,origin2);
}


vmxGUIMenuBarItem* vmxGUIMenuBar::GetPickedItem(int pos1, int pos2)
{
    mxListIterator< vmxGUIMenuBarItem > it;
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


void vmxGUIMenuBar::GetSize(int &output_size1, int &output_size2)
{
    
    if(m_items.IsEmpty() || !m_interactor)
    {
        output_size1 = output_size2 = 0;
        return;
    }
    
    // size should be determined solely from the size parameter of each item
    output_size1 = output_size2 = 0;
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        output_size1 += size[0];
        if(output_size2<size[1]) output_size2 = size[1];
    }
}


void vmxGUIMenuBar::HideMenus()
{
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_menu.SetVisibility(0);
    }
}


int vmxGUIMenuBar::IsPicked(int pos1, int pos2)
{
    if(m_items.IsEmpty() || !m_interactor)
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


int vmxGUIMenuBar::IsVisible()
{
    if(m_items.IsEmpty())
    {
        return 0;
    }
    
    return m_items.GetBeginElement().m_text_actor->GetVisibility();
}


void vmxGUIMenuBar::OnLeftButtonUp()
{
    int pick_pos[2];
    this->m_interactor->GetEventPosition(pick_pos);
    
    this->HideMenus();
    
    vmxGUIMenuBarItem* item = this->GetPickedItem(pick_pos[0], pick_pos[1]);
    
    if(item)
    {
        int origin[2];
        item->GetOriginOfTextActor(origin[0],origin[1]);
        int size_of_menu[2];
        item->m_menu.GetSize(size_of_menu[0],size_of_menu[1]);
        item->m_menu.SetOrigin(origin[0],origin[1]-size_of_menu[1]);
        item->m_menu.SetVisibility(1);
        
        this->m_interactor->Render();
    }
    else
    {
    }
}


void vmxGUIMenuBar::SetColor(double r, double g, double b)
{
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->GetTextProperty()->SetColor(r,g,b);
    }
}


void vmxGUIMenuBar::SetFontSize(double font_size)
{
    if(font_size<=0) return;
    m_font_size = font_size;
    
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->GetTextProperty()->SetFontFamilyToArial();
        it.GetElement().m_text_actor->GetTextProperty()->SetFontSize(m_font_size);
    }
}


void vmxGUIMenuBar::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    if(!interactor) return;
    
    m_interactor = interactor;
    
    // No need to turn on the left button up listening, since it will be called when the bar menu is picked.
    
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_menu.SetInteractor(interactor);
        m_main_widget->GetRenderer_GUI()->AddActor2D(it.GetElement().m_text_actor);
    }
}


void vmxGUIMenuBar::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    this->vmxGUIWidget::SetMainWidget(main_widget);
    this->SetInteractor(main_widget->GetInteractor());
}


void vmxGUIMenuBar::SetMaximumSize(int max_size_x, int max_size_y)
{
    m_maximum_size[1] = 30;
    this->ShowInputText();
}


void vmxGUIMenuBar::SetOrigin(int origin1, int origin2)
{
    int text_actor_size[2];
    this->GetSize(text_actor_size[0], text_actor_size[1]);
    
    // all items need to be alligned with respect to their upper edges, hence we need to find the max y size
    int max_y_size = 0;
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        if(max_y_size<size[1]) max_y_size = size[1];
    }

    
    // origin x value will change for each item
    int origin_x = origin1;
    
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        int size[2];
        it.GetElement().GetSizeOfTextActor(size[0],size[1]);
        
        it.GetElement().m_text_actor->SetPosition(origin_x,origin2+max_y_size-size[1]);
        
        origin_x += size[0];
    }
    
    m_placement = FIXED;
}


void vmxGUIMenuBar::SetVisibility(int is_visible)
{
    this->ShowInputText();
    
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().m_text_actor->SetVisibility(is_visible);
    }
    
    this->HideMenus();

    this->RedoPlacement();
}


int vmxGUIMenuBar::ShowInputText()
{
    mxListIterator< vmxGUIMenuBarItem > it;
    for(it.SetToBegin(m_items); it.IsValid(); it.MoveToNext())
    {
        it.GetElement().ShowInputText();
    }
    
    return 1;
}








