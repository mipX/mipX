/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppWidget.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppWidget.h"



//-------------------------------------------------------------------------------------------



void vmxAppDataListWidgetMouseLeftClickSlot::Execute(vmxGUIListWidgetItem *list_widget_item, int picked_checkbox_index)
{
    if(!list_widget_item->GetData()) return;
    
    mxDataObject *data_object = static_cast<mxDataObject*>(list_widget_item->GetData());
    
    if(picked_checkbox_index>=0 && picked_checkbox_index<data_object->GetNumberOfVisualizationComponents())
    {
        data_object->SetVisibilityOfComponent(picked_checkbox_index, list_widget_item->IsChecked(picked_checkbox_index));
    }
}


vmxAppDataListWidget::vmxAppDataListWidget(vmxAppMainWidget *app_main_widget)
{
    m_app_main_widget = app_main_widget;
    m_list_widget.SetMainWidget(app_main_widget);
}


vmxAppDataListWidget::~vmxAppDataListWidget()
{
    this->ReleaseAllDataObjects();
    this->m_data_object_factories.Reset();
}


void vmxAppDataListWidget::UpdateListWidget()
{
    //cout<<"vmxAppDataListWidget::UpdateListWidget: no of factories="<<m_data_object_factories.GetNumberOfElements()<<endl;
    //cout<<endl<<" vmxAppDataListWidget::UpdateListWidget() "<<endl;
    
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
                item->AttachSlot(new vmxAppDataListWidgetMouseLeftClickSlot);
                item->SetData(ito.GetElement());
                //item->SetNumberOfCheckBoxes(ito.GetElement()->GetNumberOfVisualizationComponents());
            }
        }
    }
    
    m_list_widget.BuildListWidget();
}


int vmxAppDataListWidget::AddExistingObject(mxDataObject* data_object)
{
    if(!mxDataObjectTree::AddExistingObject(data_object)) return 0;
    
    this->UpdateListWidget();
    
    return 1;
}


//int vmxAppDataListWidget::AddFactory(mxDataObjectFactory *factory)
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


//mxString vmxAppDataListWidget::CheckObjectName(mxString desired_name)
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


mxDataObject* vmxAppDataListWidget::Create(const char *class_or_type_name, const char *data_object_name)
{
    mxDataObject *data_object = mxDataObjectTree::Create(class_or_type_name, data_object_name);
    this->UpdateListWidget();
    data_object->SetInteractor(m_list_widget.GetMainWidget()->GetInteractor());
    
    if(m_app_main_widget->GetRenderer_3D())
    {
        data_object->SetRenderer(m_app_main_widget->GetRenderer_3D()->GetVTKRenderer());
    }
    //data_object->SetRenderer(m_list_widget.GetMainWidget()->GetRenderer_3D());
    return data_object;
}


int vmxAppDataListWidget::GetSelectedDataObjects(mxArray< mxDataObject* > &selected_data_objects)
{
    mxArray<vmxGUIListWidgetItem *> selected_items;
    this->GetListWidget()->GetSelectedItems(selected_items);
    
    int number_of_selected_data_objects = 0;
    for(int i=0; i<selected_items.GetNumberOfElements(); i++)
    {
        if(!selected_items[i]->IsController())
        {
            number_of_selected_data_objects++;
        }
    }
    
    
    selected_data_objects.SetNumberOfElements(number_of_selected_data_objects);
    int j=0;
    for(int i=0; i<selected_items.GetNumberOfElements() && j<selected_data_objects.GetNumberOfElements(); i++)
    {
        if(!selected_items[i]->IsController())
        {
            selected_data_objects[j] = static_cast<mxDataObject*> (selected_items[i]->GetData());
            j++;
        }
    }
    
    
    return number_of_selected_data_objects;
}


//mxDataObject* vmxAppDataListWidget::GetDataObject(const char *data_object_name)
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


mxList<mxDataObject*> vmxAppDataListWidget::Load(const char *file_names, const char *loading_class_name)
{
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NOT IMPLEMENTED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    mxList<mxDataObject*> l;
    return l;
}


void vmxAppDataListWidget::Release(mxDataObject* data_object)
{
    mxDataObjectTree::Release(data_object);
    this->UpdateListWidget();
}


void vmxAppDataListWidget::ReleaseAllDataObjects()
{
    mxDataObjectTree::ReleaseAllDataObjects();
    this->UpdateListWidget();
}



//-------------------------------------------------------------------------------------------



//vmxGUIRenderer* vmxAppViewer::GetRenderer()
//{
//    return m_renderers[0];
//}


void vmxAppViewer::RegisterRenderer(unsigned int renderer_index, vmxGUIRenderer *renderer)
{
    m_renderers[renderer_index] = renderer;
}



//-------------------------------------------------------------------------------------------



vmxAppMainWidget::vmxAppMainWidget()
{
    vmxGUIConnection::internal_SetMainWidget(this);
    m_renderer_3D = NULL;
}


vmxAppMainWidget::~vmxAppMainWidget()
{
}


void vmxAppMainWidget::RegisterViewer(vmxAppViewer *viewer)
{
    viewer->Setup(this);
    mxArray< vmxGUIRenderer* > renderers = viewer->GetRenderers();
    for(unsigned int i=0; i<renderers.GetNumberOfElements(); i++)
    {
        this->AddRenderer(renderers[i]);
    }
}


void vmxAppMainWidget::SetDataListWidgetToDefault()
{
    m_default_data_list_widget = new vmxAppDataListWidget(this);
    m_data_list_widget = m_default_data_list_widget;
    m_data_list_widget->GetListWidget()->SetColor(1,1,0);
    m_data_list_widget->GetListWidget()->SetFontSize(18);
    m_data_list_widget->GetListWidget()->SetVisibility(1);
    m_data_list_widget->GetListWidget()->SetPlacementToUpperRight();
}


void vmxAppMainWidget::SetRenderer3DToDefaultTrackballCamera()
{
    m_default_renderer_3D = new vmxGUIRenderer3DTrackballCamera(this);
    m_renderer_3D = m_default_renderer_3D;
    this->AddRenderer(m_renderer_3D);
}
