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
                for(int i=0; i<ito.GetElement()->GetNumberOfVisualizationComponents(); i++)
                {
                    item->m_checkboxes[i] = ito.GetElement()->GetVisibilityOfComponent(i);
                }
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
    
    m_menu_bar = new vmxGUIMenuBar();
    m_menu_bar->SetMainWidget(this);
    //vmxGUIMenuBarItem *file_menu_item = m_menu_bar->AddItem("File");
    m_menu_bar->SetColor(1,1,0);
    m_menu_bar->SetFontSize(20);
    m_menu_bar->SetInteractor(this->GetInteractor());
    m_menu_bar->SetPlacementToUpperLeft();
    //m_menu_bar->SetPlacementOrderToAbove();
    m_menu_bar->SetVisibility(1);
    
    
    m_function_widget = new vmxAppFunctionWidget();
    m_function_widget->SetMainWidget(this);
    m_function_widget->SetAppMainWidget(this);
    //m_function_widget.SetColor(1,1,0);
    //m_function_widget.SetFontSize(20);
    //m_function_widget.SetInteractor(this->GetInteractor());
    m_function_widget->SetPlacementToLowerLeft();
    m_function_widget->SetPlacementOrderToAbove();
    m_function_widget->SetVisibility(1);
    
    

}


vmxAppMainWidget::~vmxAppMainWidget()
{
}


int vmxAppMainWidget::LoadFunctionFactory(vmxAppFunctionFactoryInterface *function_factory)
{
    if(!function_factory) return 0;
    
    // Try creating a function from given factory.
    vmxAppFunctionInterface *function = function_factory->Create();
    
    if(!function)
    {
        std::cout<<std::endl<<"vmxAppMainWidget::LoadFunctionFactory() error: function_factory->Create() returned NULL!";
        return 0;
    }
    
    mxString name_in_menu = function->m_function_config.GetTitleInMenu();
    
    function_factory->Release(function);
    
    //---------- add to the menu bar ----------
    mxString s; s.Append(">"); s.Append(name_in_menu); s.Append(">");
    mxList< mxString > list_of_option_strings;
    s.ExtractStrings('>', '>', list_of_option_strings);
    
    // there must be at least 2 items - one for the menu bar and at least one for the drop down menu.
    if(list_of_option_strings.GetNumberOfElements()<2)
    {
        return 0;
    }
    
    // if the menu bar item with the given title does not exist, create it.
    vmxGUIMenuBarItem *menu_bar_item = m_menu_bar->GetItem(list_of_option_strings.GetBeginElementAddress()->Get_C_String());
    if(!menu_bar_item)
    {
        //std::cout<<std::endl<<"Adding item to menu bar: "<<list_of_option_strings.GetBeginElementAddress()->Get_C_String();
        menu_bar_item = m_menu_bar->AddItem(list_of_option_strings.GetBeginElementAddress()->Get_C_String());
    }
    list_of_option_strings.DeleteBegin();
    
    
    vmxGUIMenuItem *new_menu_item = NULL; //this is the result of item adding
    vmxGUIMenu *current_menu = menu_bar_item->GetMenu();//this is the menu containing the new item.
    {
        // for all titles
        for( ; !list_of_option_strings.IsEmpty(); list_of_option_strings.DeleteBegin())
        {
            vmxGUIMenuItem *current_menu_item = current_menu->GetItem(list_of_option_strings.GetBeginElementAddress()->Get_C_String());
            
            // if the menu item does not exist, we need to add it...
            if(!current_menu_item)
            {
                //... as a new menu in case the list_of_option_strings has more than 1 element (so we have not yet come to the ctual final item)
                if((list_of_option_strings.GetNumberOfElements()>1))
                {
                    current_menu = (current_menu->AddSubMenu(list_of_option_strings.GetBeginElementAddress()->Get_C_String()))->GetSubMenu();
                }
                //... or as a the final item (no menu attached to it).
                else
                {
                    new_menu_item = current_menu->AddItem(list_of_option_strings.GetBeginElementAddress()->Get_C_String());
                    break;
                }
            }
            // if the menu item exists, just change the current_menu.
            else
            {
                current_menu = current_menu_item->GetSubMenu();
                
                // should add here code to address the case when adding an item with existing name.
            }
        }
    }
    //-----------------------------------------
    
    if(!new_menu_item) return 0;
    
    new_menu_item->m_data = static_cast<void*> (function_factory);
    
    {
        //vmxGUIConnection *connection =
        vmxGUIConnection::New(current_menu, LeftButtonUpEvent, current_menu, this, vmxAppMainWidget::Slot_InitiateFunction);
    }
    
    current_menu->BuildMenu();
    m_menu_bar->BuildMenuBar();
    m_menu_bar->SetVisibility(1);
   
    
    //this->m_batch_call_mapper.RegisterBatchCall(function->m_batch_object,function->m_batch_command,function->m_batch_command_parameters, function_factory);
    
    return 1;
}


void vmxAppMainWidget::LoadFunctionFactoryList(vmxAppFunctionFactoryList *list_of_function_factories)
{
    mxListIterator< vmxAppFunctionFactoryInterface* > it;
    for(it.SetToBegin(list_of_function_factories->m_list_of_function_factories); it.IsValid(); it.MoveToNext())
    {
        this->LoadFunctionFactory(it.GetElement());
    }
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
    
    // Register factories with the data list widget.
    // The widget will later use these factories to construct objects of the given type.
    m_data_list_widget->AddFactory(new vmxCurveFactory);
    m_data_list_widget->AddFactory(new vmxImage16UFactory);
    m_data_list_widget->AddFactory(new vmxMeshFactory);
    m_data_list_widget->AddFactory(new vmxProfileFactory);
    m_data_list_widget->AddFactory(new vmxGraphFactory);
    m_data_list_widget->AddFactory(new vmxPointListFactory);
    m_data_list_widget->AddFactory(new vmxSkeletonFactory);
    m_data_list_widget->AddFactory(new vmxTableFactory);

}


void vmxAppMainWidget::SetRenderer3DToDefaultTrackballCamera()
{
    m_default_renderer_3D = new vmxGUIRenderer3DTrackballCamera(this);
    m_renderer_3D = m_default_renderer_3D;
    this->AddRenderer(m_renderer_3D);
    
    m_command_seeds_to_point_list = m_default_renderer_3D->AddCommand("Seeds to PointList");
    
    vmxGUIConnection::New(m_renderer_3D, KeyPressEvent, m_renderer_3D, this, vmxAppMainWidget::Slot_SetIndexTimeAndSliceForDefaultRenderer3D);
    vmxGUIConnection::New(m_renderer_3D, LeftButtonUpEvent, m_renderer_3D, this, vmxAppMainWidget::Slot_OnLeftButtonUpForDefaultRenderer3D);
    
    
}


int vmxAppMainWidget::Slot_InitiateFunction(vmxGUIConnection *connection)
{
    //std::cout<<std::endl<<"vmxAppMainWidget::Slot_InitiateFunction()";
    
    vmxGUIMenu *current_menu = static_cast<vmxGUIMenu *>(connection->GetSenderObject());
    if(!current_menu) return 0;
    
    vmxGUIMenuItem *item = current_menu->GetPickedItem();
    if(!item) return 0;
    
    vmxAppMainWidget *app_main_widget = static_cast< vmxAppMainWidget* > (connection->GetSlotCallerObject());
    if(!app_main_widget) return 0;

    vmxAppFunctionFactoryInterface *function_factory = static_cast< vmxAppFunctionFactoryInterface* > (item->m_data);
    if(!function_factory) return 0;

    vmxAppFunctionInterface *function = function_factory->Create();
    if(!function) return 0;
    function->m_app_main_widget = app_main_widget;

    //std::cout<<std::endl<<function->GetConfig()->GetTitleInMenu().Get_C_String();
    
    function->GetConfig()->SetIndexOfCurrentStep(0);
    function->GetConfig()->ResetCurrentStep();

    if(function->GetConfig()->GetNumberOfSteps()==0)
    {
        //std::cout<<std::endl<<"function->GetConfig()->GetNumberOfSteps()==0";
        function->StartInMainThread();
        app_main_widget->RedoPlacement();
    }
    else
    {
        function->Step_0();
        app_main_widget->GetFunctionWidget()->Setup(function_factory, function);
        app_main_widget->GetFunctionWidget()->LoadConfiguration(function->GetConfig());
        app_main_widget->GetFunctionWidget()->SetVisibility(1);
    }


//        plugin->m_function_config.m_function_object->ConnectProgressCounter(function_tab->m_progress_tab->m_progress_bar);
//
//    connect(function_tab, SIGNAL( SignalStartOrNextButtonPressed() ) , this, SLOT( SlotFunctionStartOrNext() ) );
//    connect(function_tab, SIGNAL( SignalBackButtonPressed() ) , this, SLOT( SlotFunctionBack() ) );
//    connect(function_tab, SIGNAL( SignalCloseButtonPressed() ) , this, SLOT( SlotFunctionClose() ) );
//    connect(function_tab, SIGNAL( SignalCancelButtonPressed() ) , this, SLOT( SlotFunctionCancel() ) );
//
//    function_tab->LoadConfiguration(&(plugin->m_function_config));
//    plugin->m_data_container.Reset();


    
    
    return 1;
}



int vmxAppMainWidget::Slot_SetIndexTimeAndSliceForDefaultRenderer3D(vmxGUIConnection *connection)
{
    vmxGUIRenderer3DTrackballCamera *renderer = static_cast< vmxGUIRenderer3DTrackballCamera* >(connection->GetSenderObject());
    if(!renderer) return 0;
    
    std::string key = renderer->GetVTKInteractorStyle()->GetInteractor()->GetKeySym();
    
    if(key.compare("s") == 0)
    {
        vmxAppMainWidget *app_main_widget = static_cast< vmxAppMainWidget* > (connection->GetSlotCallerObject());
        if(!app_main_widget) return 0;
        
        mxArray< mxDataObject* > selected_data_objects;
        app_main_widget->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
        
        if(!selected_data_objects.IsEmpty())
        {
            unsigned int index_t = selected_data_objects[0]->GetIndex_T();
            
            if(index_t == selected_data_objects[0]->GetDimension_T()-1)
            {
                selected_data_objects[0]->SetIndex_T(0);
                renderer->m_index_time = 0;
            }
            else
            {
                selected_data_objects[0]->SetIndex_T(index_t+1);
                renderer->m_index_time = index_t+1;
            }
        }
        
        for(unsigned int i=1; i<selected_data_objects.GetNumberOfElements(); i++)
        {
            unsigned int index_t = selected_data_objects[i]->GetIndex_T();
            
            if(index_t == selected_data_objects[i]->GetDimension_T()-1) selected_data_objects[i]->SetIndex_T(0);
            else selected_data_objects[i]->SetIndex_T(index_t+1);
        }
        
        renderer->DisplayProperties();
        
//        // Don't forget to invoke event, otherwise connections won't work.
//        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
        
        renderer->GetVTKRenderer()->Render();
        
        return 1;
    }
    if(key.compare("x") == 0)
    {
        vmxAppMainWidget *app_main_widget = static_cast< vmxAppMainWidget* > (connection->GetSlotCallerObject());
        if(!app_main_widget) return 0;
        
        mxArray< mxDataObject* > selected_data_objects;
        app_main_widget->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
        
        if(!selected_data_objects.IsEmpty())
        {
            unsigned int index_t = selected_data_objects[0]->GetIndex_T();
            
            if(index_t == 0)
            {
                selected_data_objects[0]->SetIndex_T(selected_data_objects[0]->GetDimension_T()-1);
                renderer->m_index_time = selected_data_objects[0]->GetDimension_T()-1;
            }
            else
            {
                selected_data_objects[0]->SetIndex_T(index_t-1);
                renderer->m_index_time = index_t-1;
            }
        }
        
        for(unsigned int i=1; i<selected_data_objects.GetNumberOfElements(); i++)
        {
            unsigned int index_t = selected_data_objects[i]->GetIndex_T();
            
            if(index_t == 0) selected_data_objects[i]->SetIndex_T(selected_data_objects[i]->GetDimension_T()-1);
            else selected_data_objects[i]->SetIndex_T(index_t-1);
        }
        
        renderer->DisplayProperties();
        
        //        // Don't forget to invoke event, otherwise connections won't work.
        //        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
        
        renderer->GetVTKRenderer()->Render();
        
        return 1;
    }
    
    if(key.compare("a") == 0)
    {
        vmxAppMainWidget *app_main_widget = static_cast< vmxAppMainWidget* > (connection->GetSlotCallerObject());
        if(!app_main_widget) return 0;
        
        mxArray< mxDataObject* > selected_data_objects;
        app_main_widget->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
        
        if(selected_data_objects.IsEmpty()) return 0;
        
        for(unsigned int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
        {
            unsigned int index = selected_data_objects[i]->GetIndex_S();
            
            if(index == selected_data_objects[i]->GetDimension_S()-1) selected_data_objects[i]->SetIndex_S(0);
            else selected_data_objects[i]->SetIndex_S(index+1);
        }
        
        renderer->m_index_slice = selected_data_objects[0]->GetIndex_S();
        
        renderer->DisplayProperties();
        
        //        // Don't forget to invoke event, otherwise connections won't work.
        //        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
        
        renderer->GetVTKRenderer()->Render();
        
        return 1;
    }
    if(key.compare("z") == 0)
    {
        vmxAppMainWidget *app_main_widget = static_cast< vmxAppMainWidget* > (connection->GetSlotCallerObject());
        if(!app_main_widget) return 0;
        
        mxArray< mxDataObject* > selected_data_objects;
        app_main_widget->GetDataListWidget()->GetSelectedDataObjects(selected_data_objects);
        
        if(selected_data_objects.IsEmpty()) return 0;
        
        for(unsigned int i=0; i<selected_data_objects.GetNumberOfElements(); i++)
        {
            unsigned int index = selected_data_objects[i]->GetIndex_S();
            
            if(index == 0) selected_data_objects[i]->SetIndex_S(selected_data_objects[i]->GetDimension_S()-1);
            else selected_data_objects[i]->SetIndex_S(index-1);
            
        }
        
        renderer->m_index_slice = selected_data_objects[0]->GetIndex_S();
        
        renderer->DisplayProperties();
        
        //        // Don't forget to invoke event, otherwise connections won't work.
        //        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
        
        renderer->GetVTKRenderer()->Render();
        
        return 1;
    }

    
    
    return 1;
}



int vmxAppMainWidget::Slot_OnLeftButtonUpForDefaultRenderer3D(vmxGUIConnection *connection)
{
    vmxGUIRenderer3DTrackballCamera *renderer = static_cast< vmxGUIRenderer3DTrackballCamera* >(connection->GetSenderObject());
    if(!renderer) return 0;
    
    vmxAppMainWidget *app_mw = static_cast<vmxAppMainWidget *>(renderer->GetMainWidget());
    
    //vmxAppImageViewer *app_viewer = static_cast<vmxAppImageViewer *>(connection->GetSlotCallerObject());
    //vmxGUIRendererImageViewer *gui_viewer = static_cast<vmxGUIRendererImageViewer *>(connection->GetSenderObject());
    
    vtkRenderWindowInteractor *rwi = renderer->GetMainWidget()->GetInteractor();
    
    vtkTextActor *ta = renderer->GetCommand(rwi->GetEventPosition()[0],rwi->GetEventPosition()[1]);
    
    if(ta == app_mw->m_command_seeds_to_point_list)
    {
        if(renderer->GetNumberOfPickedPositions()>0)
        {
            vmxPointList *pl = static_cast< vmxPointList* >( app_mw->GetDataListWidget()->Create("vmxPointList", "Seeds") );
            
            if(pl)
            {
                pl->Reset();
                double c,r,s,v;
                for(int i=0; i<renderer->GetNumberOfPickedPositions(); i++)
                {
                    renderer->GetPickedPosition(i, c,r,s,v);
                    pl->AddPosition(s, r, c);
                }
                pl->Update();
            }
            
            app_mw->RedoPlacement();
        }
    }
    
    return 1;
}
