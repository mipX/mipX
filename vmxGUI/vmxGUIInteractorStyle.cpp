/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIInteractorStyle.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxGUIInteractorStyle.h"


int vmxGUIInteractorStyle::event_DoubleClick = vtkCommand::UserEvent + 1;


vmxGUIInteractorStyle::vmxGUIInteractorStyle()
{
}


vmxGUIInteractorStyle::~vmxGUIInteractorStyle()
{
    
}


vmxGUIInteractorStyle* vmxGUIInteractorStyle::New()
{
    return new vmxGUIInteractorStyle;
}


void vmxGUIInteractorStyle::AddGUIWidgetToTheListOfListeners(vmxGUIBaseObject *listener_widget, mxList<vmxGUIBaseObject*> *list_of_listeners)
{
    if(!mxListFunctions::HasElement(*list_of_listeners, listener_widget))
    {
        list_of_listeners->AddToEnd(listener_widget);
    }
}


void vmxGUIInteractorStyle::RemoveGUIWidgetFromTheListOfListeners(vmxGUIBaseObject *listener_widget, mxList<vmxGUIBaseObject*> *list_of_listeners)
{
    mxListNode<vmxGUIBaseObject*> *node = mxListFunctions::GetNodeWithElement(*list_of_listeners, listener_widget);
    
    if(node)
    {
        list_of_listeners->DeleteNode(node);
    }
}


void vmxGUIInteractorStyle::AddRenderer(vmxGUIRenderer *renderer)
{
    if(renderer) m_renderers.AddToEnd(renderer);
}


void vmxGUIInteractorStyle::OnMouseMove()
{
    //cout<<" OnMouseMove ";
    
    vtkRenderWindowInteractor *interactor = this->GetInteractor();
    int* click_position = this->GetInteractor()->GetEventPosition();
    this->FindPokedRenderer(click_position[0], click_position[1]);
    
    this->m_left_button_drag_detector.OnMouseMove(click_position[0],click_position[1]);
    
    //If drag is detected
    if(this->m_left_button_drag_detector.IsDragging())
    {
        //cout<<" drag ";
        
        int left_button_drag_start_position[2];
        this->m_left_button_drag_detector.GetLeftButtonDragStartPosition(left_button_drag_start_position[0],left_button_drag_start_position[1]);
        
        // The drag event is forwarded only to the object over which the event started.
        vmxGUIBaseObject *picked_gui_object = m_main_widget->GetGUIObjectForScreenCoordinates(left_button_drag_start_position[0], left_button_drag_start_position[1]);
        if(picked_gui_object)
        {
            if(picked_gui_object->IsAccepting_LeftButtonDrag_Event())
            {
                picked_gui_object->OnLeftButtonDrag();
            }
        }
    }
    
    // Event is forwarded to all of its listeners.
    mxListIterator<vmxGUIBaseObject*> itgo;
    
    // Copy the list of listeners to a temp list - needed in case listeners are removed from the original list during callbacks.
    mxList<vmxGUIBaseObject*> l;
    l = m_listeners_for_MouseMove_event;
    
    for(itgo.SetToBegin(l); itgo.IsValid(); itgo.MoveToNext())
    {
        if(itgo.GetElement()->IsListeningFor_MouseMove_Event())
        {
            itgo.GetElement()->OnMouseMove();
        }
    }
    
    // Find the picked scene renderer.
    vmxGUIRenderer *scene_renderer = NULL;
    mxListIterator<vmxGUIRenderer*> it;
    for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->GetVTKRenderer() == this->CurrentRenderer)
        {
            scene_renderer = it.GetElement();
            break;
        }
    }
    
    // If the picked renderer was correctly found...
    if(scene_renderer)
    {
        //... and if it has a specified interactor style, use it.
        if(scene_renderer->GetVTKInteractorStyle())
        {
            this->SetInteractor(NULL);
            mxListIterator<vmxGUIRenderer*> it;
            for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
            {
                if(it.GetElement()->GetVTKInteractorStyle())
                {
                    it.GetElement()->GetVTKInteractorStyle()->SetInteractor(NULL);
                }
            }
            
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(interactor);
            scene_renderer->GetVTKInteractorStyle()->OnMouseMove();
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(NULL);
            this->SetInteractor(interactor);
            
        }
        //... if there is no interactor style, call the method from the renderer
        else
        {
            scene_renderer->OnMouseMove();
        }
    }
}


void vmxGUIInteractorStyle::OnMouseWheelBackward()
{
 
    int* click_pos = this->GetInteractor()->GetEventPosition();
    vtkSmartPointer<vtkPropPicker>  picker = vtkSmartPointer<vtkPropPicker>::New();
    
    // The event is forwarded only to the object over which the mouse is positioned.
    vmxGUIBaseObject *picked_gui_object = m_main_widget->GetGUIObjectForScreenCoordinates(click_pos[0], click_pos[1]);
    if(picked_gui_object)
    {
        if(picked_gui_object->IsListeningFor_MouseWheelBackward_Event())
        {
            picked_gui_object->OnMouseWheelBackward();
            return;
        }
    }


    vtkRenderWindowInteractor *interactor = this->GetInteractor();
    int* click_position = this->GetInteractor()->GetEventPosition();
    this->FindPokedRenderer(click_position[0], click_position[1]);
    
    
    // Find the picked scene renderer.
    vmxGUIRenderer *scene_renderer = NULL;
    mxListIterator<vmxGUIRenderer*> it;
    for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->GetVTKRenderer() == this->CurrentRenderer)
        {
            scene_renderer = it.GetElement();
            break;
        }
    }
    
    
    // If the picked renderer was correctly found...
    if(scene_renderer)
    {
        //... and if it has a specified interactor style, use it.
        if(scene_renderer->GetVTKInteractorStyle())
        {
            this->SetInteractor(NULL);
            mxListIterator<vmxGUIRenderer*> it;
            for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
            {
                if(it.GetElement()->GetVTKInteractorStyle())
                {
                    it.GetElement()->GetVTKInteractorStyle()->SetInteractor(NULL);
                }
            }
            
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(interactor);
            scene_renderer->GetVTKInteractorStyle()->OnMouseWheelBackward();
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(NULL);
            this->SetInteractor(interactor);
            
        }
        //... if there is no interactor style, call the method from the renderer
        else
        {
            scene_renderer->OnMouseWheelBackward();
        }
    }
}


void vmxGUIInteractorStyle::OnMouseWheelForward()
{
    
    int* click_pos = this->GetInteractor()->GetEventPosition();
    vtkSmartPointer<vtkPropPicker>  picker = vtkSmartPointer<vtkPropPicker>::New();

    // The event is forwarded only to the object over which the mouse is positioned.
    vmxGUIBaseObject *picked_gui_object = m_main_widget->GetGUIObjectForScreenCoordinates(click_pos[0], click_pos[1]);
    if(picked_gui_object)
    {
        if(picked_gui_object->IsListeningFor_MouseWheelForward_Event())
        {
            picked_gui_object->OnMouseWheelForward();
            return;
        }
    }
    
    vtkRenderWindowInteractor *interactor = this->GetInteractor();
    int* click_position = this->GetInteractor()->GetEventPosition();
    this->FindPokedRenderer(click_position[0], click_position[1]);
    
    
    // Find the picked scene renderer.
    vmxGUIRenderer *scene_renderer = NULL;
    mxListIterator<vmxGUIRenderer*> it;
    for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->GetVTKRenderer() == this->CurrentRenderer)
        {
            scene_renderer = it.GetElement();
            break;
        }
    }
    
    // If the picked renderer was correctly found...
    if(scene_renderer)
    {
        //... and if it has a specified interactor style, use it.
        if(scene_renderer->GetVTKInteractorStyle())
        {
            this->SetInteractor(NULL);
            mxListIterator<vmxGUIRenderer*> it;
            for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
            {
                if(it.GetElement()->GetVTKInteractorStyle())
                {
                    it.GetElement()->GetVTKInteractorStyle()->SetInteractor(NULL);
                }
            }
            
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(interactor);
            scene_renderer->GetVTKInteractorStyle()->OnMouseWheelForward();
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(NULL);
            this->SetInteractor(interactor);
            
        }
        //... if there is no interactor style, call the method from the renderer
        else
        {
            scene_renderer->OnMouseWheelForward();
        }
    }
}



void vmxGUIInteractorStyle::OnLeftButtonUp()
{
    //cout<<" OnLeftButtonUp ";
    
    vtkRenderWindowInteractor *interactor = this->GetInteractor();
    int* click_position = this->GetInteractor()->GetEventPosition();
    this->FindPokedRenderer(click_position[0], click_position[1]);
    
    
    // Update info for detection of drag event.
    this->m_left_button_drag_detector.OnLeftButtonUp(click_position[0], click_position[1]);
    
    // Update info for detection of single click event.
    this->m_left_button_single_click_detector.OnLeftButtonUp(click_position[0], click_position[1]);

    
    
    // The event is forwarded to the object over which the mouse is positioned and...
    int is_event_picked_by_an_object = 0;
    vmxGUIBaseObject *picked_gui_object = m_main_widget->GetGUIObjectForScreenCoordinates(click_position[0], click_position[1]);
    if(picked_gui_object)
    {
        is_event_picked_by_an_object = 1;
        if(this->m_left_button_drag_detector.IsDropped() && picked_gui_object->IsAccepting_LeftButtonDrop_Event())
        {
            picked_gui_object->OnLeftButtonDrop();
            
            vmxGUIWidgetCollection *collection = picked_gui_object->GetCollection();
            if(collection)
            {
                collection->SetEventCallerObjectAndEventPosition(picked_gui_object, click_position[0], click_position[1]);
                collection->OnLeftButtonDrop();
            }
        }
        else
        {
            if(this->m_left_button_single_click_detector.IsSingleClicked() && picked_gui_object->IsAccepting_LeftButtonSingleClick_Event())
            {
                picked_gui_object->OnLeftButtonSingleClick();
                
                vmxGUIWidgetCollection *collection = picked_gui_object->GetCollection();
                if(collection)
                {
                    collection->SetEventCallerObjectAndEventPosition(picked_gui_object, click_position[0], click_position[1]);
                    collection->OnLeftButtonSingleClick();
                }
            }
            else
            {
                picked_gui_object->OnLeftButtonUp();
                
                vmxGUIWidgetCollection *collection = picked_gui_object->GetCollection();
                if(collection)
                {
                    collection->SetEventCallerObjectAndEventPosition(picked_gui_object, click_position[0], click_position[1]);
                    collection->OnLeftButtonUp();
                }
            }
        }
    }
    
    // ... the event is forwarded to all of its listeners.
    mxListIterator<vmxGUIBaseObject*> itgo;
    
    // Copy the list of listeners to a temp list - needed in case listeners are removed from the original list during callbacks.
    mxList<vmxGUIBaseObject*> l;
    l = m_listeners_for_LeftButtonUp_event;
    
    for(itgo.SetToBegin(l); itgo.IsValid(); itgo.MoveToNext())
    {
        if(itgo.GetElement())
        {
            if(itgo.GetElement()->IsListeningFor_LeftButtonUp_Event())
            {
                itgo.GetElement()->OnLeftButtonUp();
            }
        }
    }
    
    if(is_event_picked_by_an_object) return;
    
    // Find the picked scene renderer.
    vmxGUIRenderer *scene_renderer = NULL;
    mxListIterator<vmxGUIRenderer*> it;
    for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->GetVTKRenderer() == this->CurrentRenderer)
        {
            scene_renderer = it.GetElement();
            break;
        }
    }
    
    // If the picked renderer was correctly found...
    if(scene_renderer)
    {
        //... and if it has a specified interactor style, use it.
        if(scene_renderer->GetVTKInteractorStyle())
        {
            this->SetInteractor(NULL);
            mxListIterator<vmxGUIRenderer*> it;
            for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
            {
                if(it.GetElement()->GetVTKInteractorStyle())
                {
                    it.GetElement()->GetVTKInteractorStyle()->SetInteractor(NULL);
                }
            }
            
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(interactor);
            scene_renderer->GetVTKInteractorStyle()->OnLeftButtonUp();
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(NULL);
            this->SetInteractor(interactor);
            
        }
        //... if there is no interactor style, call the method from the renderer
        else
        {
            scene_renderer->OnLeftButtonUp();
        }
    }
}


void vmxGUIInteractorStyle::OnLeftButtonDown()
{
    //cout<<" vmxGUIInteractorStyle::OnLeftButtonDown() ";
 
    vtkRenderWindowInteractor *interactor = this->GetInteractor();
    int* click_position = this->GetInteractor()->GetEventPosition();
    this->FindPokedRenderer(click_position[0], click_position[1]);
    
    
    // Update info for detection of double click event.
    this->m_left_button_double_click_detector.OnLeftButtonDown(click_position[0], click_position[1],interactor->GetControlKey());
    
    // Update info for detection of single click event.
    this->m_left_button_single_click_detector.OnLeftButtonDown(click_position[0], click_position[1]);
    
    // Update info for detection of drag event.
    this->m_left_button_drag_detector.OnLeftButtonDown(click_position[0], click_position[1]);
    

    //----- Examine the event from the GUI object side -----
    
    // The event is forwarded to the object over which the mouse is positioned and...
    int is_event_picked_by_an_object = 0;
    vmxGUIBaseObject *picked_gui_object = m_main_widget->GetGUIObjectForScreenCoordinates(click_position[0], click_position[1]);
    if(picked_gui_object)
    {
        is_event_picked_by_an_object = 1;
        {
            //If double click was performed and the object accepts double click event execute double click slot...
            if(this->m_left_button_double_click_detector.IsDoubleClicked() && picked_gui_object->IsAccepting_LeftButtonDoubleClick_Event())
            {
                picked_gui_object->OnLeftButtonDoubleClick();
                
                vmxGUIWidgetCollection *collection = picked_gui_object->GetCollection();
                if(collection)
                {
                    collection->SetEventCallerObjectAndEventPosition(picked_gui_object, click_position[0], click_position[1]);
                    collection->OnLeftButtonDoubleClick();
                }
                
            }
            else
            {
                picked_gui_object->OnLeftButtonDown();
                
                // If the object belongs to a collection, we call the callback commands of the collection.
                vmxGUIWidgetCollection *collection = picked_gui_object->GetCollection();
                if(collection)
                {
                    collection->SetEventCallerObjectAndEventPosition(picked_gui_object, click_position[0], click_position[1]);
                    collection->OnLeftButtonDown();
                }
            }
        }
    }
    
    // ... the event is forwarded to all of its listeners.
    mxListIterator<vmxGUIBaseObject*> itgo;
    
    // Copy the list of listeners to a temp list - needed in case listeners are removed from the original list during callbacks.
    mxList<vmxGUIBaseObject*> l;
    l = m_listeners_for_LeftButtonDown_event;
    
    for(itgo.SetToBegin(l); itgo.IsValid(); itgo.MoveToNext())
    {
        if(itgo.GetElement()->IsListeningFor_LeftButtonDown_Event())
        {
            itgo.GetElement()->OnLeftButtonDown();
        }
    }
    
    
    // If the event is picked by a GUI object, we will not forward the event to renderers for the scenes.
    if(is_event_picked_by_an_object) return;
    
    //---------------------------------------------------------

    
    //----- Examine the event from the scene renderers side -----

    // Find the picked scene renderer.
    vmxGUIRenderer *scene_renderer = NULL;
    mxListIterator<vmxGUIRenderer*> it;
    for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->GetVTKRenderer() == this->CurrentRenderer)
        {
            scene_renderer = it.GetElement();
            break;
        }
    }
    
    // If the picked renderer was correctly found...
    if(scene_renderer)
    {
        //... and if it has a specified interactor style, use it.
        if(scene_renderer->GetVTKInteractorStyle())
        {
            this->SetInteractor(NULL);
            mxListIterator<vmxGUIRenderer*> it;
            for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
            {
                if(it.GetElement()->GetVTKInteractorStyle())
                {
                    it.GetElement()->GetVTKInteractorStyle()->SetInteractor(NULL);
                }
            }
            
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(interactor);
            scene_renderer->GetVTKInteractorStyle()->OnLeftButtonDown();
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(NULL);
            this->SetInteractor(interactor);
            
        }
        //... if there is no interactor style, call the method from the renderer
        else
        {
            scene_renderer->OnLeftButtonDown();
        }
    }
    //---------------------------------------------------------
}


void vmxGUIInteractorStyle::OnKeyPress()
{
    // The KeyPress event is caught by GUI ONLY in case the mouse is positioned over the gui object that is listening for KeyPress.
    // Otherwise, the event is transfered to other vtk observers.
    
    int* click_position = this->GetInteractor()->GetEventPosition();
    this->FindPokedRenderer(click_position[0], click_position[1]);
    
    vmxGUIBaseObject *picked_gui_object = m_main_widget->GetGUIObjectForScreenCoordinates(click_position[0], click_position[1]);
    if(picked_gui_object)
    {
        picked_gui_object->OnKeyPress();
        return;
    }
    
    vtkRenderWindowInteractor *interactor = this->GetInteractor();
    
    // Find the picked scene renderer.
    vmxGUIRenderer *scene_renderer = NULL;
    mxListIterator<vmxGUIRenderer*> it;
    for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->GetVTKRenderer() == this->CurrentRenderer)
        {
            scene_renderer = it.GetElement();
            break;
        }
    }
    
    // If the picked renderer was correctly found...
    if(scene_renderer)
    {
        //... and if it has a specified interactor style, use it.
        if(scene_renderer->GetVTKInteractorStyle())
        {
            this->SetInteractor(NULL);
            mxListIterator<vmxGUIRenderer*> it;
            for(it.SetToBegin(m_renderers); it.IsValid(); it.MoveToNext())
            {
                if(it.GetElement()->GetVTKInteractorStyle())
                {
                    it.GetElement()->GetVTKInteractorStyle()->SetInteractor(NULL);
                }
            }
            
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(interactor);
            scene_renderer->GetVTKInteractorStyle()->OnKeyPress();
            scene_renderer->GetVTKInteractorStyle()->SetInteractor(NULL);
            this->SetInteractor(interactor);
            
        }
        //... if there is no interactor style, call the method from the renderer
        else
        {
            scene_renderer->OnKeyPress();
        }
    }
}



