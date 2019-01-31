/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppTableViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppTableViewer.h"


//-------------------------------------------------------------------------------------------




void vmxAppTableViewer::SetInputTable(vmxTable *table)
{
    if(m_table==table) return;
    
    m_table_renderer->RemoveData();
    
    m_table = table;
    
    m_table_renderer->SetTitle(m_table->GetObjectName().Get_C_String());
    
    for(unsigned int s=0; s<m_table->GetNumberOfSheets(); s++)
    {
        m_table_renderer->AddInputData(m_table->GetSheet(s)->GetTitle().Get_C_String(), m_table->GetSheet(s)->GetSheetAsString().Get_C_String(),
                                       m_table->GetSheet(s)->GetNumberOfRows()+1, m_table->GetSheet(s)->GetNumberOfColumns()+1);
    }
}


//void vmxAppTableViewer::RemoveAllTables()
//{
//    //m_table_renderer->RemoveAllData();
//}


void vmxAppTableViewer::Setup(vmxAppMainWidget *main_widget)
{
    m_main_widget = main_widget;
    
    m_table_renderer = new vmxGUIRendererTableViewer(m_main_widget);
    this->SetNumberOfRenderers(1);
    this->RegisterRenderer(0,m_table_renderer);
    
    //vmxGUIConnection *connection =
//    vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), m_main_widget->GetDataListWidget(), ItemSelectedEvent, //ItemModifiedEvent,
//                                                         m_main_widget->GetDataListWidget()->GetListWidget(), this, vmxAppTableViewer::Slot_UpdateViewer);
    
    
    vmxGUIConnection *connection = vmxGUIConnection::New(m_main_widget->GetDataListWidget()->GetListWidget(), ItemSelectedEvent,
                                                         this->GetRenderer(), this, vmxAppTableViewer::Slot_UpdateViewer);
    
    connection->SetPassedAddress(m_main_widget->GetDataListWidget()->GetListWidget()->GetLastSelectedItemData(), 0);
 
//    cout<<endl<<m_main_widget->GetDataListWidget()->GetListWidget()->GetLastSelectedItemData()<<endl;
//    cout<<endl<<(*(m_main_widget->GetDataListWidget()->GetListWidget()->GetLastSelectedItemData()))<<endl;
}


void vmxAppTableViewer::Update()
{
    //m_table_renderer->Update();
}


int vmxAppTableViewer::Slot_UpdateViewer(vmxGUIConnection *connection)
{
    cout<<" vmxAppTableViewer::Slot_UpdateViewer() ";
    vmxAppTableViewer *viewer = static_cast<vmxAppTableViewer *>(connection->GetSlotCallerObject());
    
//    cout<<endl<<connection->GetPassedAddress(0)<<endl;
//    cout<<endl<<(*(connection->GetPassedAddress(0)))<<endl;
    
    mxDataObject *obj = static_cast<mxDataObject *>(*(connection->GetPassedAddress(0)));
    
    if(!obj) return 0;
    
//    cout<<"  "<<obj->GetClassName().Get_C_String()<<"  ";
    
    vmxTable table;
    if(obj->GetClassName() == table.GetClassName())
    {
//        cout<<" # ";
        vmxTable *tbl = static_cast<vmxTable *>(obj);//*(connection->GetPassedAddress(0)));
        viewer->SetInputTable(tbl);
    }
    
    
    

    
    
    
//    //cout<<" vmxAppTableViewer::Slot_UpdateViewer() ";
//    vmxAppDataListWidget *data_list = static_cast<vmxAppDataListWidget *>(connection->GetSenderSubObject());
//    vmxAppTableViewer *viewer = static_cast<vmxAppTableViewer *>(connection->GetReceiverSubObject());
//
//    vmxTable table;
//
//    mxArray<mxDataObject *> selected_data_objects;
//    data_list->GetSelectedDataObjects(selected_data_objects);
//
//    for(unsigned int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
//    {
//        if(selected_data_objects[i]->GetClassName() == table.GetClassName())
//        {
//            vmxTable *t = static_cast<vmxTable *> (selected_data_objects[i]);
//            viewer->SetInputTable(t);
//        }
//    }
    
    viewer->Update();
    
    return 1;
}
                                                         
