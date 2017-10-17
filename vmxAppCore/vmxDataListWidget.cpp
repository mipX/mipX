/*=========================================================================
 
 Program:   mipx
 Module:    vmxDataListWidget.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxDataListWidget.h"


void vmxDataListWidgetMouseLftClickSlot::Execute(vmxGUIListWidgetItem *list_widget_item, int picked_checkbox_index)
{
    if(!list_widget_item->GetData()) return;
    
    mxDataObject *data_object = static_cast<mxDataObject*>(list_widget_item->GetData());
    
    if(picked_checkbox_index>=0 && picked_checkbox_index<data_object->GetNumberOfVisualizationComponents())
    {
        data_object->SetVisibilityOfComponent(picked_checkbox_index, list_widget_item->IsChecked(picked_checkbox_index));
    }
}




vmxDataListWidget::vmxDataListWidget()
{
}


vmxDataListWidget::~vmxDataListWidget()
{
    this->ReleaseAllDataObjects();
    this->m_data_object_factories.Reset();
}


void vmxDataListWidget::UpdateListWidget()
{
    //cout<<"vmxDataListWidget::UpdateListWidget: no of factories="<<m_data_object_factories.GetNumberOfElements()<<endl;
    //cout<<endl<<" vmxDataListWidget::UpdateListWidget() "<<endl;
    m_list_widget.Reset();
    
    //cout<<"no of factories="<<m_data_object_factories.GetNumberOfElements()<<endl;
    
    mxListIterator<mxDataObjectFactory*> itf;
    for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid(); itf.MoveToNext())
    {
        if(!itf.GetElement()->GetCreatedDataObjects()->IsEmpty())
        {
            vmxGUIListWidgetItem *controller_item = m_list_widget.AddItem(itf.GetElement()->GetClassName().Get_C_String());
            
            mxListIterator<mxDataObject*> ito;
            for(ito.SetToBegin(itf.GetElement()->GetCreatedDataObjects()); ito.IsValid(); ito.MoveToNext())
            {
                vmxGUIListWidgetItem *item = m_list_widget.AddItem(ito.GetElement()->GetObjectName().Get_C_String(), controller_item, ito.GetElement()->GetNumberOfVisualizationComponents());
                item->AttachSlot(new vmxDataListWidgetMouseLftClickSlot);
                item->SetData(ito.GetElement());
                //item->SetNumberOfCheckBoxes(ito.GetElement()->GetNumberOfVisualizationComponents());
            }
        }
    }
    
    m_list_widget.BuildListWidget();
}


int vmxDataListWidget::AddExistingObject(mxDataObject* data_object)
{
    if(!mxDataObjectTree::AddExistingObject(data_object)) return 0;
    
    this->UpdateListWidget();
    
    return 1;
}


//int vmxDataListWidget::AddFactory(mxDataObjectFactory *factory)
//{
//    if(!factory) return 0;
//    if(mxListFunctions::HasElement(this->m_data_object_factories, factory)) return 1;
//    else
//    {
//        this->m_data_object_factories.AddToEnd(factory);
//        factory->SetDataObjectTree(this);
//    }
//    return 1;
//}


//mxString vmxDataListWidget::CheckObjectName(mxString desired_name)
//{
//    mxString desired_name_modified;
//    desired_name_modified.Assign(desired_name);
//    
//    for(int i=1; i<32768; i++)
//    {
//        int is_equivalent_name_found = 0;
//        
//        mxListIterator<mxDataObjectFactory*> itf;
//        for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid() && !is_equivalent_name_found; itf.MoveToNext())
//        {
//            mxListIterator<mxDataObject*> ito;
//            for(ito.SetToBegin(itf.GetElement()->GetCreatedDataObjects()); ito.IsValid(); ito.MoveToNext())
//            {
//                if(ito.GetElement()->GetObjectName()==desired_name_modified)
//                {
//                    is_equivalent_name_found = 1;
//                    break;
//                }
//            }
//        }
//        if(!is_equivalent_name_found)
//        {
//            return desired_name_modified;
//        }
//        else
//        {
//            desired_name_modified.Assign(desired_name);
//            desired_name_modified.Append("_");
//            desired_name_modified.AppendNumber(i);
//        }
//    }
//    return desired_name;
//}


mxDataObject* vmxDataListWidget::Create(const char *class_or_type_name, const char *data_object_name)
{
    mxDataObject *data_object = mxDataObjectTree::Create(class_or_type_name, data_object_name);
    this->UpdateListWidget();
    data_object->SetInteractor(m_list_widget.GetMainWidget()->GetInteractor());
    data_object->SetRenderer(m_list_widget.GetMainWidget()->GetRenderer_3D());
    return data_object;
}


//mxDataObject* vmxDataListWidget::GetDataObject(const char *data_object_name)
//{
//    mxListIterator<mxDataObjectFactory*> itf;
//    for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid(); itf.MoveToNext())
//    {
//        mxListIterator<mxDataObject*> ito;
//        for(ito.SetToBegin(itf.GetElement()->GetCreatedDataObjects()); ito.IsValid(); ito.MoveToNext())
//        {
//            if(ito.GetElement()->GetObjectName()==data_object_name)
//            {
//                return ito.GetElement();
//            }
//        }
//    }
//    return NULL;
//}


mxList<mxDataObject*> vmxDataListWidget::Load(const char *file_names, const char *loading_class_name)
{
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NOT IMPLEMENTED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    mxList<mxDataObject*> l;
    return l;
}


void vmxDataListWidget::Release(mxDataObject* data_object)
{
    mxDataObjectTree::Release(data_object);
    this->UpdateListWidget();
}


void vmxDataListWidget::ReleaseAllDataObjects()
{
    mxDataObjectTree::ReleaseAllDataObjects();
    this->UpdateListWidget();
}

