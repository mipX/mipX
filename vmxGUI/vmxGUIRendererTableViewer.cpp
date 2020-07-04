/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRendererTableViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxGUIRendererTableViewer.h"






vtkStandardNewMacro(vmxGUIRendererTableViewerSheet);


bool vmxGUIRendererTableViewerSheet::Paint(vtkContext2D *painter)
{
    // set up painter properties.
    painter->GetTextProp()->SetVerticalJustificationToCentered();
    painter->GetTextProp()->SetJustificationToCentered();
    painter->GetTextProp()->SetColor(1.0, 1.0, 0.0);// notice: this method takes doubles (range 0 to 1)!
    painter->GetTextProp()->SetFontSize(m_renderer->m_font_size);
    painter->GetBrush()->SetColor(100, 100, 100, 255);
    painter->GetPen()->SetColor(255,255,0,255);
    
    
    
    int available_window_size[2];
    available_window_size[0] = m_renderer->GetVTKRenderer()->GetSize()[0];
    available_window_size[1] = m_renderer->GetVTKRenderer()->GetSize()[1];
    
    // field size for the titles.
    int title_field_size = m_renderer->m_font_size + 4;
    
    
    
    
    //--- calculate positions of table title, index numbers, sheet title... ---
    int old_available_window_size_y = available_window_size[1];
    
    int table_title_y = old_available_window_size_y - title_field_size;
    
    int tab_index_y = table_title_y - title_field_size;
    
    int sheet_title_y = tab_index_y - title_field_size;
    
    available_window_size[1] = sheet_title_y;
    //------
    
    //--- paint the actual table data ---
    int field_size[2];
    field_size[0] = available_window_size[0]/ m_number_of_columns;
    field_size[1] = available_window_size[1]/ m_number_of_rows;
    
    mxMatrix< mxString > table;
    table.SetDimensions(m_number_of_rows, m_number_of_columns);
    
    mxList< mxString > list;
    m_data.ExtractStrings('{', '}', list);
    mxListIterator<mxString> it;
    it.SetToBegin(list);
    for(int r=m_number_of_rows-1; r>=0 && it.IsValid(); r--)
    {
        for(int c=0; c<m_number_of_columns && it.IsValid(); c++, it.MoveToNext())
        {
            table(r,c).Assign(it.GetElement());
        }
    }
    
    for(int r=0; r<(int)table.GetNumberOfRows(); r++)
    {
        for(int c=0; c<(int)table.GetNumberOfColumns(); c++)
        {
            painter->DrawRect(c*field_size[0], r*field_size[1], (c+1)*field_size[0], (r+1)*field_size[1]);
            painter->DrawString( ((2*c+1)*field_size[0])/2, ((2*r+1)*field_size[1])/2, table(r,c).Get_C_String());
        }
    }
    //------
    
    //--- paint sheet title ---
    painter->DrawRect(0, sheet_title_y, available_window_size[0], sheet_title_y + title_field_size);
    painter->DrawString( available_window_size[0]/2, (2*sheet_title_y+title_field_size)/2, this->m_title.Get_C_String());
    //------

    //--- paint sheet index ---
    for(int i=0; i<(int)m_renderer->m_sheets.GetNumberOfElements(); i++)
    {
        mxString index_str;
        index_str.AssignNumber(i);
        
        if(i == m_renderer->m_current_sheet_index)
        {
            painter->GetTextProp()->SetBold(1);
        }
        else
        {
            painter->GetTextProp()->SetBold(0);
        }
        
        painter->DrawRect(i*title_field_size, tab_index_y, available_window_size[0], tab_index_y + title_field_size);
        painter->DrawString( ((2*i+1)*title_field_size)/2, (2*tab_index_y+title_field_size)/2, index_str.Get_C_String());
    }
    painter->GetTextProp()->SetBold(0);
    //------

    //--- paint table title ---
    painter->DrawRect(0, table_title_y, available_window_size[0], table_title_y + title_field_size);
    painter->DrawString( available_window_size[0]/2, (2*table_title_y+title_field_size)/2, this->m_renderer->m_title.Get_C_String());
    //------

    
    return true;
}





//---------------------------------------------------------------------------



vmxGUIRendererTableViewer::vmxGUIRendererTableViewer(vmxGUIMainWidget *main_widget)
{
    m_class_name.Assign("vmxGUIRendererTableViewer");
    
    m_clip_board = NULL;
    
    m_is_capturing_event = 1;
    
    m_font_size = 24;
    
    m_main_widget = main_widget;
    
    if(m_main_widget)
    {
        m_clip_board = main_widget->GetClipBoard();
    }
    
    m_current_sheet_index = 0;
    
    
    m_context_actor = vtkSmartPointer<vtkContextActor>::New();
    
    this->GetVTKRenderer()->AddActor(m_context_actor);
}


vmxGUIRendererTableViewer::~vmxGUIRendererTableViewer()
{
    
}


void vmxGUIRendererTableViewer::AddInputData(const char *sheet_title, const char *sheet_data, int number_of_rows, int number_of_columns)
{
    if(!sheet_data) return;
    if(!sheet_title) return;
    
    vtkSmartPointer< vmxGUIRendererTableViewerSheet > sheet;
    m_sheets.AddToEnd(sheet);
    
    m_sheets.GetEndElement() = vtkSmartPointer< vmxGUIRendererTableViewerSheet >::New();
    m_sheets.GetEndElement()->m_renderer = this;
    m_sheets.GetEndElement()->m_title.Assign(sheet_title);
    m_sheets.GetEndElement()->m_data.Assign(sheet_data);
    m_sheets.GetEndElement()->m_number_of_rows = number_of_rows;
    m_sheets.GetEndElement()->m_number_of_columns = number_of_columns;
    
    this->SetSheetIndex(m_sheets.GetNumberOfElements()-1);
    
}



void vmxGUIRendererTableViewer::OnMouseWheelBackward()
{
    this->SetSheetIndex(m_current_sheet_index-1);
    this->GetVTKRenderer()->GetRenderWindow()->Render();
}


void vmxGUIRendererTableViewer::OnMouseWheelForward()
{
    this->SetSheetIndex(m_current_sheet_index+1);
    this->GetVTKRenderer()->GetRenderWindow()->Render();
}


void vmxGUIRendererTableViewer::RemoveData()
{
    m_context_actor->GetScene()->ClearItems();
    
    m_sheets.Reset();
    
    m_current_sheet_index = -1;
}


void vmxGUIRendererTableViewer::SetSheetIndex(int sheet_index)
{
    if(sheet_index<0 || sheet_index>=(int)m_sheets.GetNumberOfElements()) return;

    m_current_sheet_index = sheet_index;
    
    m_context_actor->GetScene()->ClearItems();
    m_context_actor->GetScene()->AddItem(m_sheets[m_current_sheet_index]);
    

}



