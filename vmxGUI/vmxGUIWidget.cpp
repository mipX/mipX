/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIWidget.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxGUIWidget.h"



// Initialize the static member value.
vmxGUIMainWidget* vmxGUIConnection::m_main_widget = NULL;



void vmxGUIConnection::PrintSelf()
{
    cout<<endl;
    cout<<" Sender object: type = "<<m_sender_object->GetClassName().Get_C_String()<<", name = "<<m_sender_object->GetObjectName().Get_C_String()<<endl;
    cout<<" Sender non-GUI object: address = "<<m_sender_non_gui_object<<endl;
    cout<<" Trigger event ID: "<<m_trigger_event_id<<endl;
    cout<<" Receiver object: type = "<<m_receiver_object->GetClassName().Get_C_String()<<", name = "<<m_receiver_object->GetObjectName().Get_C_String()<<endl;
    cout<<" Slot caller object: address = "<<m_slot_caller_object<<endl;
    cout<<" Slot: address = "<<((void*)m_slot)<<endl;
    cout<<" Passed data: address = "<<((void*)m_passed_data)<<endl;
    cout<<" Passed data int: address = "<<((void*)m_passed_data_int)<<endl;
}


vmxGUIConnection* vmxGUIConnection::New(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, void *sender_non_gui_object, vmxGUIBaseObject *receiver, void *slot_caller_object, vmxGUISlotFunction slot)
{
    if(!sender) return NULL;
    if(!receiver) return NULL;
    
    // Check if this connection already exists and if it does, just return pointer to it (no need to create another one).
    vmxGUIConnection *c = sender->GetConnectionManager()->GetConnection(sender,trigger_event_id,receiver,slot);
    if(c) return c;
    
    vmxGUIConnection* connection = new vmxGUIConnection();
    connection->m_sender_object = sender;
    connection->m_sender_non_gui_object = sender_non_gui_object;
    connection->m_receiver_object = receiver;
    connection->m_trigger_event_id = trigger_event_id;
    connection->m_slot_caller_object = slot_caller_object;
    connection->m_slot = slot;
    
    sender->GetConnectionManager()->AddConnection(connection);
    if(sender!=receiver) receiver->GetConnectionManager()->AddConnection(connection);
    
    return connection;
}


vmxGUIConnection* vmxGUIConnection::New(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, void *slot_caller_object, vmxGUISlotFunction slot)
{
    return vmxGUIConnection::New(sender, trigger_event_id, NULL, receiver, slot_caller_object, slot);
}


int vmxGUIConnection::Delete(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, vmxGUISlotFunction slot)
{
    if(!sender) return 0;
    
    vmxGUIConnection c;
    c.m_sender_object = sender;
    c.m_trigger_event_id = trigger_event_id;
    c.m_receiver_object = receiver;
    c.m_slot = slot;
    
    vmxGUIConnection *connection_to_delete = sender->GetConnectionManager()->RemoveConnection(c);
    receiver->GetConnectionManager()->RemoveConnection(c);
    
    if(!connection_to_delete) { return 0; }
    
    delete connection_to_delete;
    return 1;
}


int vmxGUIConnection::Delete(vmxGUIConnection *connection)
{
    if(!connection) return 0;
    
    vmxGUIBaseObject *sender = connection->m_sender_object;
    vmxGUIBaseObject *receiver = connection->m_receiver_object;
    
    connection->m_sender_object->GetConnectionManager()->RemoveConnection(*connection);
    if(sender!=receiver) connection->m_receiver_object->GetConnectionManager()->RemoveConnection(*connection);

    delete connection;
    return 1;
}


int vmxGUIConnection::IsEqualTo(vmxGUIConnection &connection)
{
    return this->IsEqualTo(connection.m_sender_object, connection.m_trigger_event_id, connection.m_receiver_object, connection.m_slot);
}


int vmxGUIConnection::IsEqualTo(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, vmxGUISlotFunction slot)
{
    if(m_receiver_object != receiver) return 0;
    if(m_sender_object != sender) return 0;
    if(m_trigger_event_id != trigger_event_id) return 0;
    if(m_slot != slot) return 0;
    return 1;
}



//void vmxGUIConnection::Set(vmxGUIBaseObject *sender, void *sender_subobject, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, void *receiver_subobject, vmxGUISlotFunction slot)
//{
//    this->m_sender_object = sender;
//    this->m_sender_subobject = sender_subobject;
//    this->m_receiver_object = receiver;
//    this->m_receiver_subobject = receiver_subobject;
//    this->m_trigger_event_id = trigger_event_id;
//    this->m_slot = slot;
//}



//------------------------------------------------------------------------------------------------------




vmxGUIConnectionManager::vmxGUIConnectionManager()
{
    m_connections_by_event_id.SetNumberOfElements(VMX_GUI_USER_EVENT_START_INDEX*2);
}



vmxGUIConnectionManager::~vmxGUIConnectionManager()
{
    for(unsigned int i=0; i<m_connections_by_event_id.GetNumberOfElements(); i++)
    {
        while(!m_connections_by_event_id[i].IsEmpty())
        {
            vmxGUIConnection::Delete(m_connections_by_event_id[i].GetBeginElement());
        }
    }
}


void vmxGUIConnectionManager::AddConnection(vmxGUIConnection *connection)
{
    m_connections_by_event_id[connection->GetEventID()].AddToEnd(connection);
}


vmxGUIConnection* vmxGUIConnectionManager::GetConnection(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, vmxGUISlotFunction slot)
{
    mxListIterator< vmxGUIConnection* > it;
    for(it.SetToBegin(m_connections_by_event_id[trigger_event_id]); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->IsEqualTo(sender,trigger_event_id,receiver,slot))
        {
            vmxGUIConnection* c = it.GetElement();
            return c;
        }
    }
    return NULL;
}


vmxGUIConnection* vmxGUIConnectionManager::RemoveConnection(vmxGUIConnection &connection)
{
    vmxGUIConnection* c = NULL;
    mxListIterator< vmxGUIConnection* > it;
    mxListNode< vmxGUIConnection* > *node_to_delete = NULL;
    for(it.SetToBegin(m_connections_by_event_id[connection.GetEventID()]); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->IsEqualTo(connection))
        {
            node_to_delete = it.GetNode();
            c = it.GetElement();
            break;
        }
    }
    
    if(node_to_delete)
    {
        m_connections_by_event_id[connection.GetEventID()].DeleteNode(node_to_delete);
        return c;
    }
    
    return NULL;
}


int vmxGUIConnectionManager::Execute(vmxGUIEventID event_id, vmxGUIBaseObject *sender_object)//, void *sender_subobject)
{
    //cout<<endl<<" vmxGUIConnectionManager::Execute() "<<endl;
    //cout<<" event_id="<<event_id<<", sender_object="<<((void*)sender_object)<<", sender_subobject="<<sender_subobject<<endl;
    
    vmxGUIConnection *last_valid_connection = NULL;
    
    int return_value = 1;
    mxListIterator< vmxGUIConnection* > it;
    for(it.SetToBegin(m_connections_by_event_id[event_id]); it.IsValid(); it.MoveToNext())
    {
        //cout<<"   sender_object="<<((void*)it.GetElement()->GetSenderObject())<<", sender_subobject="<<it.GetElement()->GetSenderSubObject()<<endl;
        
        // considering that the subobject is of void* type, it is questionable if it shoudl be used to invoke event. Maybe not.
        
        //if(it.GetElement()->GetSenderObject()==sender_object && it.GetElement()->GetSenderSubObject()==sender_subobject)
        if(it.GetElement()->GetSenderObject()==sender_object)// && it.GetElement()->GetSenderSubObject()==sender_subobject)
        {
            last_valid_connection = it.GetElement();
            if(!it.GetElement()->Execute())
            {
                return_value = 0;
            }
        }
    }
    
    if(last_valid_connection)
    {
        if(last_valid_connection->GetMainWidget())
        {
            if(last_valid_connection->GetMainWidget()->GetRenderWindow())
            {
                last_valid_connection->GetMainWidget()->GetRenderWindow()->Render();
            }
        }
    }
    
    return return_value;
}





//------------------------------------------------------------------------------------------------------






vmxGUIBaseObject::vmxGUIBaseObject()
{
    m_clip_board = NULL;
    m_main_widget = NULL;
    m_class_name.Assign("vmxGUIBaseObject");
    m_object_name.Assign("Noname");
    
    m_is_accepting_LeftButtonDoubleClick_event = 0;
    m_is_accepting_LeftButtonSingleClick_event = 0;
    m_is_accepting_LeftButtonDrag_event = 0;
    m_is_accepting_LeftButtonDrop_event = 0;
    
    m_is_listening_for_KeyPress_event = 0;
    m_is_listening_for_MouseMove_event = 0;
    m_is_listening_for_LeftButtonUp_event = 0;
    m_is_listening_for_LeftButtonDown_event = 0;
    m_is_listening_for_MouseWheelForward_event = 0;
    m_is_listening_for_MouseWheelBackward_event = 0;
}


vmxGUIBaseObject::~vmxGUIBaseObject()
{
}


mxString vmxGUIBaseObject::GetClassName()
{
    return m_class_name;
}


mxString vmxGUIBaseObject::GetObjectName()
{
    return m_object_name;
}


void vmxGUIBaseObject::SetClassName(const char *class_name)
{
    if(class_name) m_class_name.Assign(class_name);
}


void vmxGUIBaseObject::SetListeningFor_KeyPress_Event(int is_listening)
{
    m_is_listening_for_KeyPress_event = is_listening;
    if(is_listening)
    {
        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_KeyPress_event));
    }
    else
    {
        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_KeyPress_event));
    }
}


void vmxGUIBaseObject::SetListeningFor_LeftButtonDown_Event(int is_listening)
{
    m_is_listening_for_LeftButtonDown_event = is_listening;
    if(is_listening)
    {
        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonDown_event));
    }
    else
    {
        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonDown_event));
    }
}


void vmxGUIBaseObject::SetListeningFor_LeftButtonUp_Event(int is_listening)
{
    m_is_listening_for_LeftButtonUp_event = is_listening;
    if(is_listening)
    {
        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonUp_event));
    }
    else
    {
        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonUp_event));
    }
}


void vmxGUIBaseObject::SetListeningFor_MouseMove_Event(int is_listening)
{
    m_is_listening_for_MouseMove_event = is_listening;
    if(is_listening)
    {
        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseMove_event));
    }
    else
    {
        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseMove_event));
    }
}


void vmxGUIBaseObject::SetListeningFor_MouseWheelBackward_Event(int is_listening)
{
    m_is_listening_for_MouseWheelBackward_event = is_listening;
    if(is_listening)
    {
        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelBackward_event));
    }
    else
    {
        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelBackward_event));
    }
}


void vmxGUIBaseObject::SetListeningFor_MouseWheelForward_Event(int is_listening)
{
    m_is_listening_for_MouseWheelForward_event = is_listening;
    if(is_listening)
    {
        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelForward_event));
    }
    else
    {
        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelForward_event));
    }
}


void vmxGUIBaseObject::SetObjectName(const char *object_name)
{
    if(object_name) m_object_name.Assign(object_name);
}


void vmxGUIBaseObject::SetVisibility(int is_visible)
{
//    if(!m_is_occupying_renderer) return;
//
//    if(is_visible)
//    {
//        if(m_main_widget->m_renderer_GUI_occupant==NULL)
//        {
//            m_main_widget->m_renderer_GUI_occupant = this;
//        }
//    }
//    else
//    {
//        if(m_main_widget->m_renderer_GUI_occupant==this)
//        {
//            m_main_widget->m_renderer_GUI_occupant = this;
//        }
//    }
}



//------------------------------------------------------------------------------------------------------






vmxGUIWidgetBase::vmxGUIWidgetBase()
{
    m_collection = NULL;
//    m_clip_board = NULL;
//    m_main_widget = NULL;
    m_class_name.Assign("vmxGUIWidgetBase");
    m_object_name.Assign("Noname");
    m_is_stretching_over_x_axis = 0;
    m_is_occupying_renderer = 0;
    m_placement_order = ABOVE;
    m_placement = RELATIVE;
    
//    m_is_accepting_LeftButtonDoubleClick_event = 0;
//    m_is_accepting_LeftButtonSingleClick_event = 0;
//    m_is_accepting_LeftButtonDrag_event = 0;
//    m_is_accepting_LeftButtonDrop_event = 0;
//
//    m_is_listening_for_KeyPress_event = 0;
//    m_is_listening_for_MouseMove_event = 0;
//    m_is_listening_for_LeftButtonUp_event = 0;
//    m_is_listening_for_LeftButtonDown_event = 0;
//    m_is_listening_for_MouseWheelForward_event = 0;
//    m_is_listening_for_MouseWheelBackward_event = 0;
}


vmxGUIWidgetBase::~vmxGUIWidgetBase()
{
}


//mxString vmxGUIWidgetBase::GetClassName()
//{
//    return m_class_name;
//}
//
//
//mxString vmxGUIWidgetBase::GetObjectName()
//{
//    return m_object_name;
//}


vmxGUIWidgetBase::vmxGUIWidgetPlacement vmxGUIWidgetBase::GetPlacement()
{
    return m_placement;
}


vmxGUIWidgetBase::vmxGUIWidgetPlacementOrder vmxGUIWidgetBase::GetPlacementOrder()
{
    return m_placement_order;
}


int vmxGUIWidgetBase::IsStretchingOver_X_Axis()
{
    return m_is_stretching_over_x_axis;
}


//void vmxGUIWidgetBase::SetListeningFor_KeyPress_Event(int is_listening)
//{
//    m_is_listening_for_KeyPress_event = is_listening;
//    if(is_listening)
//    {
//        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_KeyPress_event));
//    }
//    else
//    {
//        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_KeyPress_event));
//    }
//}
//
//
//void vmxGUIWidgetBase::SetListeningFor_LeftButtonDown_Event(int is_listening)
//{
//    m_is_listening_for_LeftButtonDown_event = is_listening;
//    if(is_listening)
//    {
//        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonDown_event));
//    }
//    else
//    {
//        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonDown_event));
//    }
//}
//
//
//void vmxGUIWidgetBase::SetListeningFor_LeftButtonUp_Event(int is_listening)
//{
//    m_is_listening_for_LeftButtonUp_event = is_listening;
//    if(is_listening)
//    {
//        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonUp_event));
//    }
//    else
//    {
//        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_LeftButtonUp_event));
//    }
//}
//
//
//void vmxGUIWidgetBase::SetListeningFor_MouseMove_Event(int is_listening)
//{
//    m_is_listening_for_MouseMove_event = is_listening;
//    if(is_listening)
//    {
//        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseMove_event));
//    }
//    else
//    {
//        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseMove_event));
//    }
//}
//
//
//void vmxGUIWidgetBase::SetListeningFor_MouseWheelBackward_Event(int is_listening)
//{
//    m_is_listening_for_MouseWheelBackward_event = is_listening;
//    if(is_listening)
//    {
//        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelBackward_event));
//    }
//    else
//    {
//        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelBackward_event));
//    }
//}
//
//
//void vmxGUIWidgetBase::SetListeningFor_MouseWheelForward_Event(int is_listening)
//{
//    m_is_listening_for_MouseWheelForward_event = is_listening;
//    if(is_listening)
//    {
//        this->m_main_widget->GetInteractorStyle()->AddGUIWidgetToTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelForward_event));
//    }
//    else
//    {
//        this->m_main_widget->GetInteractorStyle()->RemoveGUIWidgetFromTheListOfListeners(this, &(this->m_main_widget->GetInteractorStyle()->m_listeners_for_MouseWheelForward_event));
//    }
//}
//
//
//void vmxGUIWidgetBase::SetObjectName(const char *object_name)
//{
//    if(object_name) m_object_name.Assign(object_name);
//}


void vmxGUIWidgetBase::SetVisibility(int is_visible)
{
    if(!m_is_occupying_renderer) return;
    
    if(is_visible)
    {
        if(m_main_widget->m_renderer_GUI_occupant==NULL)
        {
            m_main_widget->m_renderer_GUI_occupant = this;
        }
    }
    else
    {
        if(m_main_widget->m_renderer_GUI_occupant==this)
        {
            m_main_widget->m_renderer_GUI_occupant = this;
        }
    }
}




//------------------------------------------------------------------------------------------------------




vmxGUIWidget::vmxGUIWidget()
{
    m_class_name.Assign("vmxGUIWidget");
    m_is_stretching_over_x_axis = 0;
    m_object_bounds[0] = m_object_bounds[1] = m_object_bounds[2] = m_object_bounds[3] = 0;
    m_placement_order = ABOVE;
    m_placement = RELATIVE;
}


vmxGUIWidget::~vmxGUIWidget()
{
}


int vmxGUIWidget::CopyFrom(vmxGUIWidget *object)
{
    if(!object) return 0;
    
    // Do not copy object name. Depending on higher level app, it could be a unique name.
    // Also, no need to copy class names, as they are the same for each object of the same class.
    
    return 1;
}


mxArray<vmxGUIWidget*> vmxGUIWidget::GetWidgets()
{
    mxArray<vmxGUIWidget*> a;
    a.SetNumberOfElements(1);
    a[0] = this;
    return a;
}


int vmxGUIWidget::IsPicked(int pos1, int pos2)
{
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


void vmxGUIWidget::RedoPlacement()
{
    if(m_placement==RELATIVE)
    {
        this->SetPlacementToRelative(m_placement_relative_percentages[0],m_placement_relative_percentages[1]);
        return;
    }
    if(m_placement==LOWER_LEFT)
    {
        this->SetPlacementToLowerLeft();
        return;
    }
    if(m_placement==LOWER_CENTER)
    {
        this->SetPlacementToLowerCenter();
        return;
    }
    if(m_placement==LOWER_RIGHT)
    {
        this->SetPlacementToLowerRight();
        return;
    }
    if(m_placement==CENTER_LEFT)
    {
        this->SetPlacementToCenterLeft();
        return;
    }
    if(m_placement==CENTER_CENTER)
    {
        this->SetPlacementToCenterCenter();
        return;
    }
    if(m_placement==CENTER_RIGHT)
    {
        this->SetPlacementToCenterRight();
        return;
    }
    if(m_placement==UPPER_LEFT)
    {
        this->SetPlacementToUpperLeft();
        return;
    }
    if(m_placement==UPPER_CENTER)
    {
        this->SetPlacementToUpperCenter();
        return;
    }
    if(m_placement==UPPER_RIGHT)
    {
        this->SetPlacementToUpperRight();
        return;
    }
}


void vmxGUIWidget::RepositionAfterResizing()
{
    // (0,3) (1,3)
    // (0,2) (1,2)
    
    if(m_placement==LOWER_LEFT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);
        
        int new_origin_x = m_object_bounds[0];
        int new_origin_y = m_object_bounds[2];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = LOWER_LEFT;
        
        return;
    }
//    if(m_placement==LOWER_CENTER)
//    {
//        this->SetPlacementToLowerCenter();
//    }
    if(m_placement==LOWER_RIGHT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);
        
        int new_origin_x = m_object_bounds[1] - new_object_size[0];
        int new_origin_y = m_object_bounds[2];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = LOWER_RIGHT;
        
        return;
    }
//    if(m_placement==CENTER_LEFT)
//    {
//        this->SetPlacementToCenterLeft();
//    }
//    if(m_placement==CENTER_CENTER)
//    {
//        this->SetPlacementToCenterCenter();
//    }
//    if(m_placement==CENTER_RIGHT)
//    {
//        this->SetPlacementToCenterRight();
//    }
    if(m_placement==UPPER_LEFT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);
        
        int new_origin_x = m_object_bounds[0];
        int new_origin_y = m_object_bounds[3] - new_object_size[1];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = UPPER_LEFT;
        
        return;
    }
//    if(m_placement==UPPER_CENTER)
//    {
//        this->SetPlacementToUpperCenter();
//    }
    if(m_placement==UPPER_RIGHT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);

        int new_origin_x = m_object_bounds[1] - new_object_size[0];
        int new_origin_y = m_object_bounds[3] - new_object_size[1];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = UPPER_RIGHT;
        
        return;
    }
}


void vmxGUIWidget::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    m_main_widget = main_widget;
    if(m_main_widget)
    {
        vmxGUIWidgetBase *obj = static_cast<vmxGUIWidgetBase*>(this); //vmxGUIBaseObject *obj = static_cast<vmxGUIBaseObject*>(this);
        
        m_main_widget->m_objects.AddToEnd(obj);
        m_clip_board = main_widget->GetClipBoard();
        this->SetInteractor(main_widget->GetInteractor());

    }
}


void vmxGUIWidget::SetPlacementOrderToAbove()
{
    m_placement_order = ABOVE;
}


void vmxGUIWidget::SetPlacementOrderToBelow()
{
    m_placement_order = BELOW;
}


void vmxGUIWidget::SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent)
{
    m_placement_relative_percentages[0] = x_percent;
    m_placement_relative_percentages[1] = y_percent;
    
    int x_min, x_max, y_min, y_max;
    
    if(x_percent<34)
    {
        m_main_widget->GetAvailableExtentForLeftSide(x_min, x_max, y_min, y_max, this->IsStretchingOver_X_Axis());
    }
    else
    {
        if(x_percent<67)
        {
            m_main_widget->GetAvailableExtentForCenter(x_min, x_max, y_min, y_max, this->IsStretchingOver_X_Axis());
        }
        else
        {
            m_main_widget->GetAvailableExtentForRightSide(x_min, x_max, y_min, y_max, this->IsStretchingOver_X_Axis());
        }
    }
    
    
    //cout<<this->m_class_name.Get_C_String()<<": x_min="<<x_min<<", x_max="<<x_max<<", y_min="<<y_min<<", y_max="<<y_max<<endl;

    
    int window_size[2];
    window_size[0] = x_max - x_min + 1;
    window_size[1] = y_max - y_min + 1;
    
    this->SetMaximumSize(window_size[0],window_size[1]);
    
    int origin[2];
    origin[0] = x_min + (window_size[0] * x_percent) / 100;
    origin[1] = y_min + (window_size[1] * y_percent) / 100;
    
    //cout<<"temp: origin[0]="<<origin[0]<<", origin[1]="<<origin[1]<<endl;

    
    int widget_size[2];
    this->GetSize(widget_size[0], widget_size[1]);
    
    if(origin[0]+widget_size[0]>window_size[0])
    {
        origin[0] = window_size[0] - widget_size[0] + x_min;
    }

    if(y_percent==100)
    {
        if(origin[1]+widget_size[1]>window_size[1])
        {
            origin[1] = window_size[1] - widget_size[1] + y_min;
        }
    }

    //cout<<this->m_class_name.Get_C_String()<<": widget_size[0]="<<widget_size[0]<<", widget_size[1]="<<widget_size[1]<<endl;
    //cout<<this->m_class_name.Get_C_String()<<": origin[0]="<<origin[0]<<", origin[1]="<<origin[1]<<endl;
    
    this->SetOrigin(origin[0],origin[1]);
    
    m_object_bounds[0] = origin[0];
    m_object_bounds[1] = origin[0] + widget_size[0];
    m_object_bounds[2] = origin[1];
    m_object_bounds[3] = origin[1] + widget_size[1];
    
    m_placement = RELATIVE;
}


void vmxGUIWidget::SetPlacementToCenterCenter()
{
    this->SetPlacementToRelative(50,50);
    m_placement = CENTER_CENTER;
}


void vmxGUIWidget::SetPlacementToCenterLeft()
{
    this->SetPlacementToRelative(0,50);
    m_placement = CENTER_LEFT;
}


void vmxGUIWidget::SetPlacementToCenterRight()
{
    this->SetPlacementToRelative(100,50);
    m_placement = CENTER_RIGHT;
}


void vmxGUIWidget::SetPlacementToLowerCenter()
{
    this->SetPlacementToRelative(50,0);
    m_placement = LOWER_CENTER;
}


void vmxGUIWidget::SetPlacementToLowerLeft()
{
    this->SetPlacementToRelative(0,0);
    m_placement = LOWER_LEFT;
}


void vmxGUIWidget::SetPlacementToLowerRight()
{
    this->SetPlacementToRelative(100,0);
    m_placement = LOWER_RIGHT;
}


void vmxGUIWidget::SetPlacementToUpperCenter()
{
    this->SetPlacementToRelative(50,100);
    m_placement = UPPER_CENTER;
}


void vmxGUIWidget::SetPlacementToUpperLeft()
{
    this->SetPlacementToRelative(0,100);
    m_placement = UPPER_LEFT;
}


void vmxGUIWidget::SetPlacementToUpperRight()
{
    this->SetPlacementToRelative(100,100);
    m_placement = UPPER_RIGHT;
}


void vmxGUIWidget::SetStretchingOver_X_Axis(int is_stretching_over_x_axis)
{
    m_is_stretching_over_x_axis = is_stretching_over_x_axis;
}


//void vmxGUIWidget::SetVisibility(int is_visible)
//{
//    if(!m_is_occupying_renderer) return;
//
//    if(is_visible)
//    {
//        if(m_main_widget->m_renderer_GUI_occupant==NULL)
//        {
//            m_main_widget->m_renderer_GUI_occupant = this;
//        }
//    }
//    else
//    {
//        if(m_main_widget->m_renderer_GUI_occupant==this)
//        {
//            m_main_widget->m_renderer_GUI_occupant = this;
//        }
//    }
//}



//------------------------------------------------------------------------------------------------------



vmxGUIWidgetCollection::vmxGUIWidgetCollection()
{
    m_class_name.Assign("vmxGUIWidgetCollection");
    m_event_caller_object = NULL;
    m_event_caller_object_position_of_event[0] = -1;
    m_event_caller_object_position_of_event[1] = -1;
    m_is_visible_collection = 0;
}


vmxGUIWidgetCollection::~vmxGUIWidgetCollection()
{
}


int vmxGUIWidgetCollection::AddWidget(vmxGUIWidget *widget)
{
    if(!widget) return 0;
    
    // First check if the widget is already in the list, if not, add it.
    mxListNode<vmxGUIWidget*> *node = mxListFunctions::GetNodeWithElement(m_objects, widget);
    if(!node)
    {
        m_objects.AddToEnd(widget);
        widget->SetCollection(this);
        if(!widget->GetMainWidget()) widget->SetMainWidget(this->m_main_widget);
    }
    
    return 1;
}


mxArray<vmxGUIWidget*> vmxGUIWidgetCollection::GetWidgets()
{
    mxArray<vmxGUIWidget*> a;
    a.SetNumberOfElements(m_objects.GetNumberOfElements());
    mxListIterator<vmxGUIWidget*> it;
    int i=0;
    for(it.SetToBegin(m_objects), i=0; it.IsValid() && i<(int)a.GetNumberOfElements(); it.MoveToNext(), i++)
    {
        a[i] = it.GetElement();
    }
    return a;
}


int vmxGUIWidgetCollection::RemoveWidget(vmxGUIWidget *widget)
{
    if(!widget) return 0;
    
    // First check if the widget is already in the list, if not, add it.
    mxListNode<vmxGUIWidget*> *node = mxListFunctions::GetNodeWithElement(m_objects, widget);
    if(node)
    {
        widget->SetVisibility(0);
        m_objects.DeleteNode(node);
        widget->SetCollection(NULL);
        //if(!widget->GetMainWidget()) widget->SetMainWidget(this->m_main_widget);
        return 1;
    }
    
    return 0;
}


void vmxGUIWidgetCollection::Reset()
{
    // should call Reset() on all containing objects.
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->Reset();
    }
    
    m_objects.Reset();
}


void vmxGUIWidgetCollection::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    m_main_widget = main_widget;
    
    if(this->IsOccupayingRenderer())
    {
        vmxGUIWidgetBase* obj = static_cast<vmxGUIWidgetBase*>(this);
        m_main_widget->m_exclusive_visibility_objects.AddToEnd(obj);
    }
}


void vmxGUIWidgetCollection::SetPlacementOrderToAbove()
{
    m_placement_order = ABOVE;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementOrderToAbove();
    }
}


void vmxGUIWidgetCollection::SetPlacementOrderToBelow()
{
    m_placement_order = BELOW;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementOrderToBelow();
    }
}

void vmxGUIWidgetCollection::SetPlacementToCenterCenter()
{
    m_placement = CENTER_CENTER;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToCenterCenter();
    }
}


void vmxGUIWidgetCollection::SetPlacementToCenterLeft()
{
    m_placement = CENTER_LEFT;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToCenterLeft();
    }
}


void vmxGUIWidgetCollection::SetPlacementToCenterRight()
{
    m_placement = CENTER_RIGHT;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToCenterRight();
    }
}


void vmxGUIWidgetCollection::SetPlacementToLowerCenter()
{
    m_placement = LOWER_CENTER;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToLowerCenter();
    }
}


void vmxGUIWidgetCollection::SetPlacementToLowerLeft()
{
    m_placement = LOWER_LEFT;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToLowerLeft();
    }
}


void vmxGUIWidgetCollection::SetPlacementToLowerRight()
{
    m_placement = LOWER_RIGHT;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToLowerRight();
    }
}


void vmxGUIWidgetCollection::SetPlacementToUpperCenter()
{
    m_placement = UPPER_CENTER;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToUpperCenter();
    }
}


void vmxGUIWidgetCollection::SetPlacementToUpperLeft()
{
    m_placement = UPPER_LEFT;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToUpperLeft();
    }
}


void vmxGUIWidgetCollection::SetPlacementToUpperRight()
{
    m_placement = UPPER_RIGHT;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->SetPlacementToUpperRight();
    }
}


void vmxGUIWidgetCollection::SetVisibility(int is_visible)
{
    m_is_visible_collection = is_visible;
    
    //std::cout<<std::endl<<"vmxGUIWidgetCollection::SetVisibility():"<<std::endl;
    
    mxListIterator<vmxGUIWidget*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        //std::cout<<it.GetElement()->GetClassName().Get_C_String()<<" ";
        it.GetElement()->SetVisibility(is_visible);
    }
}






//------------------------------------------------------------------------------------------------------




vmxGUIRenderer::vmxGUIRenderer()
{
    m_class_name.Assign("vmxGUIRenderer");
    m_object_name.Assign("Unnamed");
    m_is_visible = 1;
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_is_capturing_event = 0;
    m_number_of_picked_positions = 0;
    m_font_size = 14;//16;
    
    m_command_text_actor_separator = 3;
    m_command_text_actor_coordinate_x = 0 + m_command_text_actor_separator;
    m_command_text_actor_color_RGB[0] = 0.3;
    m_command_text_actor_color_RGB[1] = 0.3;
    m_command_text_actor_color_RGB[2] = 0.3;
    
//    int separator = 3;
//    int x_position = 0 + separator;
//    
//    int i = 0;
//    m_text_actor = vtkSmartPointer<vtkTextActor>::New();
//    m_text_actor->SetInput(" ");
//    m_text_actor->SetPosition(x_position, i*(m_font_size+separator));
//    m_text_actor->GetTextProperty()->SetFontSize ( m_font_size );
//    m_text_actor->GetTextProperty()->SetColor ( 1.0, 0.0, 0.0 );
//    m_renderer->AddActor2D( m_text_actor );
//    i++;
    
}


vmxGUIRenderer::~vmxGUIRenderer()
{
}


vtkRenderer* vmxGUIRenderer::GetVTKRenderer()
{
    return m_renderer;
}


vtkTextActor* vmxGUIRenderer::AddCommand(const char *command_text)
{
    *(this->m_command_text_actors.AddNewToEnd()) = vtkSmartPointer<vtkTextActor>::New();
    vtkTextActor *ta = m_command_text_actors.GetEndElement().GetPointer();
    ta->SetInput(command_text);
    ta->SetPosition(m_command_text_actor_coordinate_x, (m_command_text_actors.GetNumberOfElements()-1)*(m_font_size+m_command_text_actor_separator));
    ta->GetTextProperty()->SetFontSize(m_font_size);
    ta->GetTextProperty()->SetColor(m_command_text_actor_color_RGB[0], m_command_text_actor_color_RGB[1], m_command_text_actor_color_RGB[2]);
    m_renderer->AddActor2D( ta );
    return ta;
}


void vmxGUIRenderer::AddPickPosition(double x, double y, double z, double v)
{
    if(m_number_of_picked_positions>=VMX_GUI_RENDERER_MAX_NUMBER_OF_PICKED_POSITIONS) return;
    
    m_picked_poisitions[m_number_of_picked_positions].m_x = x;
    m_picked_poisitions[m_number_of_picked_positions].m_y = y;
    m_picked_poisitions[m_number_of_picked_positions].m_z = z;
    m_picked_poisitions[m_number_of_picked_positions].m_v = v;
    
    m_number_of_picked_positions++;
}


void vmxGUIRenderer::ClearLastPickedPosition()
{
    if(m_number_of_picked_positions==0) return;
    m_number_of_picked_positions--;
}


vtkTextActor* vmxGUIRenderer::GetCommand(const char *command_text)
{
    mxString s;
    s.Assign(command_text);
    mxListIterator< vtkSmartPointer< vtkTextActor > > it;
    for(it.SetToBegin(m_command_text_actors); it.IsValid(); it.MoveToNext())
    {
        vtkTextActor *ta = it.GetElement().GetPointer();
        if(s == ta->GetInput())
        {
            return ta;
        }
    }
    return NULL;
}


vtkTextActor* vmxGUIRenderer::GetCommand(int screen_coordinate_x, int screen_coordinate_y)
{
    int renderer_pos[2];
    renderer_pos[0] = m_renderer->GetViewport()[0] * ((double)this->GetMainWidget()->GetRenderWindow()->GetSize()[0]);
    renderer_pos[1] = m_renderer->GetViewport()[1] * ((double)this->GetMainWidget()->GetRenderWindow()->GetSize()[1]);
    
    int event_local[2];
    event_local[0] = screen_coordinate_x - renderer_pos[0];
    event_local[1] = screen_coordinate_y - renderer_pos[1];

    mxListIterator< vtkSmartPointer< vtkTextActor > > it;
    for(it.SetToBegin(m_command_text_actors); it.IsValid(); it.MoveToNext())
    {
        vtkTextActor *ta = it.GetElement().GetPointer();
        
        double size[2];
        ta->GetSize(m_renderer, size);
        
        if(event_local[0] >= ta->GetPosition()[0] && event_local[0] <= ta->GetPosition()[0] + size[0])
        {
            //std::cout<<std::endl<<" 2 ";
            if(event_local[1] >= ta->GetPosition()[1] && event_local[1] <= ta->GetPosition()[1] + size[1])
            {
                //std::cout<<std::endl<<"Reset view";
                return ta;
            }
        }
    }
    return NULL;
}


int vmxGUIRenderer::GetLastPickedPosition(double &x, double &y, double &z, double &v)
{
    if(m_number_of_picked_positions==0) return 0;
    
    x = m_picked_poisitions[m_number_of_picked_positions-1].m_x;
    y = m_picked_poisitions[m_number_of_picked_positions-1].m_y;
    z = m_picked_poisitions[m_number_of_picked_positions-1].m_z;
    v = m_picked_poisitions[m_number_of_picked_positions-1].m_v;
    
    return 1;
}


int vmxGUIRenderer::GetPickedPosition(int index_of_the_picked_position, double &x, double &y, double &z, double &v)
{
    if(index_of_the_picked_position<0 || index_of_the_picked_position>=m_number_of_picked_positions) return 0;
    
    x = m_picked_poisitions[index_of_the_picked_position].m_x;
    y = m_picked_poisitions[index_of_the_picked_position].m_y;
    z = m_picked_poisitions[index_of_the_picked_position].m_z;
    v = m_picked_poisitions[index_of_the_picked_position].m_v;
    
    return 1;
}


void vmxGUIRenderer::GetScreenCoordinates(int &x0, int &y0, int &x1, int &y1)
{
    double *viewport = m_renderer->GetViewport();
    int *size = m_renderer->GetRenderWindow()->GetSize();
    x0 = viewport[0] * size[0];
    y0 = viewport[1] * size[1];
    x1 = viewport[2] * size[0];
    y1 = viewport[3] * size[1];
}


vmxGUIRenderer* vmxGUIRenderer::New()
{
    return new vmxGUIRenderer;
}


//------------------------------------------------------------------------------------------------------




vmxGUIMainWidgetRenderWindowModifiedCallback::vmxGUIMainWidgetRenderWindowModifiedCallback()
{
    m_widget = NULL;
    m_previous_window_size[0] = m_previous_window_size[1] = 0;
}


vmxGUIMainWidgetRenderWindowModifiedCallback::~vmxGUIMainWidgetRenderWindowModifiedCallback()
{
    m_widget = NULL;
}


vmxGUIMainWidgetRenderWindowModifiedCallback* vmxGUIMainWidgetRenderWindowModifiedCallback::New()
{
    return new vmxGUIMainWidgetRenderWindowModifiedCallback;
}


void vmxGUIMainWidgetRenderWindowModifiedCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_widget)
    {
        int rw_size[2];
        m_widget->GetRenderWindowSize(rw_size[0], rw_size[1]);
        if(rw_size[0]>0 && rw_size[1]>0)
        {
            if(rw_size[0]!=m_previous_window_size[0] || rw_size[1]!=m_previous_window_size[1])
            {
                m_previous_window_size[0] = rw_size[0];
                m_previous_window_size[1] = rw_size[1];
                m_widget->RedoPlacement();
            }
        }
    }
}



//------------------------------------------------------------------------------------------------------



vmxGUIMainWidget::vmxGUIMainWidget()
{
    m_class_name.Assign("vmxGUIMainWidget");
    m_render_window = NULL;
    
    
    m_renderer_GUI = vtkSmartPointer<vtkRenderer>::New();
    
    m_render_window_internal = vtkSmartPointer<vtkSDL2OpenGLRenderWindow>::New();
    m_interactor = vtkSmartPointer<vtkSDL2RenderWindowInteractor>::New();
    

    m_interactor_style = vtkSmartPointer<vmxGUIInteractorStyle>::New();
    m_interactor_style->SetMainWidget(this);

    
    m_render_window_internal->AddRenderer(m_renderer_GUI);

    m_interactor->SetRenderWindow(m_render_window_internal);
    m_interactor->SetInteractorStyle(m_interactor_style);
    m_size_xy[0] = 1536; //1920
    m_size_xy[1] = 864; //1080
    m_render_window_internal->SetSize(m_size_xy[0],m_size_xy[1]);
    
    m_renderer_GUI->SetViewport(0,0,1,1);
    
    m_render_window_internal->SetNumberOfLayers(2);
    
    m_renderer_GUI->SetLayer(1);
    
    m_renderer_GUI_occupant = NULL;
    

    
    m_spacing = 1;
    
    int window_size[2];
    this->GetRenderWindowSize(window_size[0], window_size[1]);
    m_left_x_extent[0] = 0;
    m_left_x_extent[1] = window_size[0]-1;
    m_left_y_extent[0] = 0;
    m_left_y_extent[1] = window_size[1]-1;
    m_center_x_extent[0] = 0;
    m_center_x_extent[1] = window_size[0]-1;
    m_center_y_extent[0] = 0;
    m_center_y_extent[1] = window_size[1]-1;
    m_right_x_extent[0] = 0;
    m_right_x_extent[1] = window_size[0]-1;
    m_right_y_extent[0] = 0;
    m_right_y_extent[1] = window_size[1]-1;

    
    m_window_modified_callback = vtkSmartPointer<vmxGUIMainWidgetRenderWindowModifiedCallback>::New();
    m_window_modified_callback->m_widget = this;
    m_window_modified_callback->m_previous_window_size[0] = window_size[0];
    m_window_modified_callback->m_previous_window_size[1] = window_size[1];
    
    // use the internal render window.
    this->SetRenderWindow(m_render_window_internal);
    
}


vmxGUIMainWidget::~vmxGUIMainWidget()
{
   // this->Reset();
}


int vmxGUIMainWidget::AddRenderer(vmxGUIRenderer *renderer)
{
    if(!renderer) return 0;
    
    m_renderers_list.AddToEnd(renderer);
    m_interactor_style->AddRenderer(renderer);
    m_render_window_internal->AddRenderer(renderer->GetVTKRenderer());
    
    for(int execute_once = 1; execute_once; execute_once = 0)
    {
        if(m_renderers_list.GetNumberOfElements()==1)
        {
            renderer->GetVTKRenderer()->SetViewport(0, 0, 1, 1);
            renderer->GetVTKRenderer()->SetLayer(0);
            
            break;
        }
        
        if(m_renderers_list.GetNumberOfElements()==2)
        {
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0, 0, 0.5, 1);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
            
            renderer->GetVTKRenderer()->SetViewport(0.5, 0, 1, 1);
            renderer->GetVTKRenderer()->SetLayer(0);
            
            break;
        }
        
        if(m_renderers_list.GetNumberOfElements()==3)
        {            
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0, 0, 0.33, 1);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[1]->GetVTKRenderer()->SetViewport(0.33, 0, 0.66, 1);
            m_renderers_list[1]->GetVTKRenderer()->SetLayer(0);
            
            renderer->GetVTKRenderer()->SetViewport(0.66, 0, 1, 1);
            renderer->GetVTKRenderer()->SetLayer(0);

            break;
        }
        
        if(m_renderers_list.GetNumberOfElements()==4)
        {
            renderer->GetVTKRenderer()->SetViewport(0, 0, 0.5, 0.5);
            renderer->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0.5, 0, 1, 0.5);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[1]->GetVTKRenderer()->SetViewport(0, 0.5, 0.5, 1);
            m_renderers_list[1]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[2]->GetVTKRenderer()->SetViewport(0.5, 0.5, 1, 1);
            m_renderers_list[2]->GetVTKRenderer()->SetLayer(0);

            break;
        }
        
        
        if(m_renderers_list.GetNumberOfElements()==5)
        {
            renderer->GetVTKRenderer()->SetViewport(0, 0, 0.33, 0.5);
            renderer->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0.33, 0, 0.66, 0.5);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[1]->GetVTKRenderer()->SetViewport(0.66, 0, 1, 1);
            m_renderers_list[1]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[2]->GetVTKRenderer()->SetViewport(0, 0.5, 0.33, 1);
            m_renderers_list[2]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[3]->GetVTKRenderer()->SetViewport(0.33, 0.5, 0.66, 1);
            m_renderers_list[3]->GetVTKRenderer()->SetLayer(0);

            break;
        }

        
        if(m_renderers_list.GetNumberOfElements()==6)
        {
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0, 0, 0.33, 0.5);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[1]->GetVTKRenderer()->SetViewport(0.33, 0, 0.66, 0.5);
            m_renderers_list[1]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[2]->GetVTKRenderer()->SetViewport(0.66, 0, 1, 0.5);
            m_renderers_list[2]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[3]->GetVTKRenderer()->SetViewport(0, 0.5, 0.33, 1);
            m_renderers_list[3]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[4]->GetVTKRenderer()->SetViewport(0.33, 0.5, 0.66, 1);
            m_renderers_list[4]->GetVTKRenderer()->SetLayer(0);
            
            renderer->GetVTKRenderer()->SetViewport(0.66, 0.5, 1, 1);
            renderer->GetVTKRenderer()->SetLayer(0);

            break;
        }
        
        
        if(m_renderers_list.GetNumberOfElements()==7)
        {
            renderer->GetVTKRenderer()->SetViewport(0, 0, 0.33, 0.33);
            renderer->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0.33, 0, 0.66, 0.33);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
        
            m_renderers_list[1]->GetVTKRenderer()->SetViewport(0, 0.33, 0.33, 0.66);
            m_renderers_list[1]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[2]->GetVTKRenderer()->SetViewport(00.33, 0.33, 0.66, 0.66);
            m_renderers_list[2]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[3]->GetVTKRenderer()->SetViewport(0, 0.66, 0.33, 1);
            m_renderers_list[3]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[4]->GetVTKRenderer()->SetViewport(00.33, 0.66, 0.66, 1);
            m_renderers_list[4]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[5]->GetVTKRenderer()->SetViewport(0.66, 0, 1, 1);
            m_renderers_list[5]->GetVTKRenderer()->SetLayer(0);
            
            break;
        }
        
        
        if(m_renderers_list.GetNumberOfElements()==8)
        {
            renderer->GetVTKRenderer()->SetViewport(0, 0, 0.33, 0.33);
            renderer->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0.33, 0, 0.66, 0.33);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[1]->GetVTKRenderer()->SetViewport(0, 0.33, 0.33, 0.66);
            m_renderers_list[1]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[2]->GetVTKRenderer()->SetViewport(00.33, 0.33, 0.66, 0.66);
            m_renderers_list[2]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[3]->GetVTKRenderer()->SetViewport(0, 0.66, 0.33, 1);
            m_renderers_list[3]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[4]->GetVTKRenderer()->SetViewport(00.33, 0.66, 0.66, 1);
            m_renderers_list[4]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[5]->GetVTKRenderer()->SetViewport(0.66, 0, 1, 0.5);
            m_renderers_list[5]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[6]->GetVTKRenderer()->SetViewport(0.66, 0.5, 1, 1);
            m_renderers_list[6]->GetVTKRenderer()->SetLayer(0);

            break;
        }
        
        if(m_renderers_list.GetNumberOfElements()==9)
        {
            renderer->GetVTKRenderer()->SetViewport(0, 0, 0.33, 0.33);
            renderer->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[0]->GetVTKRenderer()->SetViewport(0.33, 0, 0.66, 0.33);
            m_renderers_list[0]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[1]->GetVTKRenderer()->SetViewport(0.66, 0, 1, 0.33);
            m_renderers_list[1]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[2]->GetVTKRenderer()->SetViewport(0, 0.33, 0.33, 0.66);
            m_renderers_list[2]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[3]->GetVTKRenderer()->SetViewport(00.33, 0.33, 0.66, 0.66);
            m_renderers_list[3]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[4]->GetVTKRenderer()->SetViewport(0.66, 0.33, 1, 0.66);
            m_renderers_list[4]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[5]->GetVTKRenderer()->SetViewport(0, 0.66, 0.33, 1);
            m_renderers_list[5]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[6]->GetVTKRenderer()->SetViewport(00.33, 0.66, 0.66, 1);
            m_renderers_list[6]->GetVTKRenderer()->SetLayer(0);
            
            m_renderers_list[7]->GetVTKRenderer()->SetViewport(0.66, 0.66, 1, 1);
            m_renderers_list[7]->GetVTKRenderer()->SetLayer(0);
            
            break;
        }
    }
    
    return 1;
}


void vmxGUIMainWidget::GetAvailableExtentForLeftSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis)
{
    x_min = m_left_x_extent[0];
    x_max = m_left_x_extent[1];
    
    y_min = m_left_y_extent[0];
    y_max = m_left_y_extent[1];
    
    if(is_stretching_over_x_axis)
    {
        if(x_min < m_center_x_extent[0]) x_min = m_center_x_extent[0];
        if(x_min < m_right_x_extent[0]) x_min = m_right_x_extent[0];
        
        if(x_max > m_center_x_extent[1]) x_max = m_center_x_extent[1];
        if(x_max > m_right_x_extent[1]) x_max = m_right_x_extent[1];
        
        if(y_min < m_center_y_extent[0]) y_min = m_center_y_extent[0];
        if(y_min < m_right_y_extent[0]) y_min = m_right_y_extent[0];
        
        if(y_max > m_center_y_extent[1]) y_max = m_center_y_extent[1];
        if(y_max > m_right_y_extent[1]) y_max = m_right_y_extent[1];
    }
}


void vmxGUIMainWidget::GetAvailableExtentForCenter(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis)
{
    x_min = m_center_x_extent[0];
    x_max = m_center_x_extent[1];
    
    y_min = m_center_y_extent[0];
    y_max = m_center_y_extent[1];
    
    if(is_stretching_over_x_axis)
    {
        if(x_min < m_left_x_extent[0]) x_min = m_left_x_extent[0];
        if(x_min < m_right_x_extent[0]) x_min = m_right_x_extent[0];
        
        if(x_max > m_left_x_extent[1]) x_max = m_left_x_extent[1];
        if(x_max > m_right_x_extent[1]) x_max = m_right_x_extent[1];
        
        if(y_min < m_left_y_extent[0]) y_min = m_left_y_extent[0];
        if(y_min < m_right_y_extent[0]) y_min = m_right_y_extent[0];
        
        if(y_max > m_left_y_extent[1]) y_max = m_left_y_extent[1];
        if(y_max > m_right_y_extent[1]) y_max = m_right_y_extent[1];
    }
}


void vmxGUIMainWidget::GetAvailableExtentForRightSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis)
{
    x_min = m_right_x_extent[0];
    x_max = m_right_x_extent[1];
    
    y_min = m_right_y_extent[0];
    y_max = m_right_y_extent[1];
    
    if(is_stretching_over_x_axis)
    {
        if(x_min < m_left_x_extent[0]) x_min = m_left_x_extent[0];
        if(x_min < m_center_x_extent[0]) x_min = m_center_x_extent[0];
        
        if(x_max > m_left_x_extent[1]) x_max = m_left_x_extent[1];
        if(x_max > m_center_x_extent[1]) x_max = m_center_x_extent[1];
        
        if(y_min < m_left_y_extent[0]) y_min = m_left_y_extent[0];
        if(y_min < m_center_y_extent[0]) y_min = m_center_y_extent[0];
        
        if(y_max > m_left_y_extent[1]) y_max = m_left_y_extent[1];
        if(y_max > m_center_y_extent[1]) y_max = m_center_y_extent[1];
    }
}


void vmxGUIMainWidget::GetAvailableSizeForLeftSide(int &x_size, int &y_size, int is_stretching_over_x_axis)
{
    int x_min, x_max, y_min, y_max;
    this->GetAvailableExtentForLeftSide(x_min, x_max, y_min, y_max, is_stretching_over_x_axis);
    
    x_size = x_max-x_min+1;
    y_size = y_max-y_min+1;
}


void vmxGUIMainWidget::GetAvailableSizeForCenter(int &x_size, int &y_size, int is_stretching_over_x_axis)
{
    int x_min, x_max, y_min, y_max;
    this->GetAvailableExtentForCenter(x_min, x_max, y_min, y_max, is_stretching_over_x_axis);
    
    x_size = x_max-x_min+1;
    y_size = y_max-y_min+1;
}


void vmxGUIMainWidget::GetAvailableSizeForRightSide(int &x_size, int &y_size, int is_stretching_over_x_axis)
{
    int x_min, x_max, y_min, y_max;
    this->GetAvailableExtentForRightSide(x_min, x_max, y_min, y_max, is_stretching_over_x_axis);
    
    x_size = x_max-x_min+1;
    y_size = y_max-y_min+1;
}


vtkRenderer* vmxGUIMainWidget::GetRenderer_GUI()
{
    return m_renderer_GUI;
}


vtkSDL2OpenGLRenderWindow* vmxGUIMainWidget::GetRenderWindow()
{
    return m_render_window;
}


int vmxGUIMainWidget::GetRenderWindowSize(int &x_size, int &y_size)
{
    if(!m_render_window)
    {
        x_size = y_size = 0;
        cout<<"vmxGUIMainWidget::GetRenderWindowSize(int&,int&): render window pointer is NULL.";
        return 0;
    }
    
    int *window_size = m_render_window->GetSize();
    x_size = window_size[0];
    y_size = window_size[1];
    return 1;
}


vmxGUIWidgetBase* vmxGUIMainWidget::GetGUIObjectForVTKActor2D(vtkActor2D *actor)
{
    mxListIterator<vmxGUIWidgetBase*> it; //mxListIterator<vmxGUIBaseObject*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->HasVTKActor2D(actor)) return it.GetElement();
    }
    return NULL;
}


vmxGUIWidgetBase* vmxGUIMainWidget::GetGUIObjectForScreenCoordinates(int pos1, int pos2)
{
    mxListIterator<vmxGUIWidgetBase*> it; //mxListIterator<vmxGUIBaseObject*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()->IsVisible())
        {
            if(it.GetElement()->IsPicked(pos1,pos2)) return it.GetElement();
        }
    }
    return NULL;
}



void vmxGUIMainWidget::GetSize(int &size_x, int &size_y)
{
    size_x = m_size_xy[0];
    size_y = m_size_xy[1];
}


void vmxGUIMainWidget::RedoPlacement()
{
    int window_size[2];
    if(!this->GetRenderWindowSize(window_size[0], window_size[1])) return;
    
    //cout<<endl<<" vmxGUIMainWidget::RedoPlacement() "<<endl;
    
    m_left_x_extent[0] = 0;
    m_left_x_extent[1] = window_size[0]-1;
    m_left_y_extent[0] = 0;
    m_left_y_extent[1] = window_size[1]-1;
    m_center_x_extent[0] = 0;
    m_center_x_extent[1] = window_size[0]-1;
    m_center_y_extent[0] = 0;
    m_center_y_extent[1] = window_size[1]-1;
    m_right_x_extent[0] = 0;
    m_right_x_extent[1] = window_size[0]-1;
    m_right_y_extent[0] = 0;
    m_right_y_extent[1] = window_size[1]-1;
    
    // Empty all the object lists that indicate placements
    m_lower_center_objects.Reset();
    m_lower_left_objects.Reset();
    m_lower_right_objects.Reset();
    m_upper_center_objects.Reset();
    m_upper_left_objects.Reset();
    m_upper_right_objects.Reset();
    
    

    // See if there is a single occupant of the renderer
    if(m_renderer_GUI_occupant)
    {
        if(!m_renderer_GUI_occupant->IsVisible())
        {
            m_renderer_GUI_occupant = NULL;
        }
    }
    
    if(!m_renderer_GUI_occupant)
    {
        mxListIterator<vmxGUIWidgetBase*> it; //mxListIterator<vmxGUIBaseObject*> it;
        //for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
        for(it.SetToBegin(m_exclusive_visibility_objects); it.IsValid(); it.MoveToNext())
        {
            //cout<<endl<<it.GetElement()->GetClassName().Get_C_String()<<"  ";
            //cout<<"  is visible: "<<it.GetElement()->IsVisible()<<"  ";
            //cout<<"  occupies: "<<it.GetElement()->IsOccupayingRenderer();
            
            if(it.GetElement()->IsVisible() && it.GetElement()->IsOccupayingRenderer())
            {
                m_renderer_GUI_occupant = it.GetElement();
                break;
            }
        }
    }
    
    // Populate the lists that indicate placements
    if(m_renderer_GUI_occupant)
    {
        // first temporarily remove all actors from GUI renderer
        m_renderer_GUI->RemoveAllViewProps();
        
//        for(it.SetToBegin(m_exclusive_visibility_objects); it.IsValid(); it.MoveToNext())
//        {
//            cout<<endl<<it.GetElement()->GetClassName().Get_C_String()<<"  ";
//            cout<<"  is visible: "<<it.GetElement()->IsVisible()<<"  ";
//            cout<<"  occupies: "<<it.GetElement()->IsOccupayingRenderer();
//
//            if(it.GetElement()->IsVisible() && it.GetElement()->IsOccupayingRenderer())
//            {
//                m_renderer_GUI_occupant = it.GetElement();
//                break;
//            }
//        }
        
        //cout<<"   if(m_renderer_GUI_occupant)   ";
        //cout<<endl<<m_renderer_GUI_occupant->GetClassName().Get_C_String()<<"  ";
        //cout<<"  is visible: "<<m_renderer_GUI_occupant->IsVisible()<<"  ";
        //cout<<"  occupies: "<<m_renderer_GUI_occupant->IsOccupayingRenderer();
        
        mxArray<vmxGUIWidget*> array_of_widgets = m_renderer_GUI_occupant->GetWidgets();
        for(unsigned int i=0; i<array_of_widgets.GetNumberOfElements(); i++)
        {
            //cout<<" # ";
            vmxGUIWidget *obj = array_of_widgets[i]; //vmxGUIWidget *obj = it.GetElement();
            
            for(int execute_once = 1; execute_once; execute_once =0)
            {
                if(obj->GetPlacement()==vmxGUIWidget::UPPER_LEFT)
                {
                    if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                    {
                        m_upper_left_objects.AddToEnd(obj);
                    }
                    else
                    {
                        m_upper_left_objects.AddToBegin(obj);
                    }
                    break;
                }
                
                if(obj->GetPlacement()==vmxGUIWidget::LOWER_LEFT)
                {
                    if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                    {
                        m_lower_left_objects.AddToEnd(obj);
                    }
                    else
                    {
                        m_lower_left_objects.AddToBegin(obj);
                    }
                    break;
                }
                
                if(obj->GetPlacement()==vmxGUIWidget::UPPER_CENTER)
                {
                    if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                    {
                        m_upper_center_objects.AddToEnd(obj);
                    }
                    else
                    {
                        m_upper_center_objects.AddToBegin(obj);
                    }
                    break;
                }
                
                if(obj->GetPlacement()==vmxGUIWidget::LOWER_CENTER)
                {
                    if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                    {
                        m_lower_center_objects.AddToEnd(obj);
                    }
                    else
                    {
                        m_lower_center_objects.AddToBegin(obj);
                    }
                    break;
                }
                
                if(obj->GetPlacement()==vmxGUIWidget::UPPER_RIGHT)
                {
                    if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                    {
                        m_upper_right_objects.AddToEnd(obj);
                    }
                    else
                    {
                        m_upper_right_objects.AddToBegin(obj);
                    }
                    break;
                }
                
                if(obj->GetPlacement()==vmxGUIWidget::LOWER_RIGHT)
                {
                    if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                    {
                        m_lower_right_objects.AddToEnd(obj);
                    }
                    else
                    {
                        m_lower_right_objects.AddToBegin(obj);
                    }
                    break;
                }
            }
        }

    }
    else
    {
        mxListIterator<vmxGUIWidgetBase*> it; //mxListIterator<vmxGUIBaseObject*> it; //mxListIterator<vmxGUIWidget*> it;
        for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
        {
            mxArray<vmxGUIWidget*> array_of_widgets = it.GetElement()->GetWidgets();
            for(unsigned int i=0; i<array_of_widgets.GetNumberOfElements(); i++)
            {
                vmxGUIWidget *obj = array_of_widgets[i]; //vmxGUIWidget *obj = it.GetElement();
                
                for(int execute_once = 1; execute_once; execute_once =0)
                {
                    if(obj->GetPlacement()==vmxGUIWidget::UPPER_LEFT)
                    {
                        if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                        {
                            m_upper_left_objects.AddToEnd(obj);
                        }
                        else
                        {
                            m_upper_left_objects.AddToBegin(obj);
                        }
                        break;
                    }
                    
                    if(obj->GetPlacement()==vmxGUIWidget::LOWER_LEFT)
                    {
                        if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                        {
                            m_lower_left_objects.AddToEnd(obj);
                        }
                        else
                        {
                            m_lower_left_objects.AddToBegin(obj);
                        }
                        break;
                    }
                    
                    if(obj->GetPlacement()==vmxGUIWidget::UPPER_CENTER)
                    {
                        if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                        {
                            m_upper_center_objects.AddToEnd(obj);
                        }
                        else
                        {
                            m_upper_center_objects.AddToBegin(obj);
                        }
                        break;
                    }
                    
                    if(obj->GetPlacement()==vmxGUIWidget::LOWER_CENTER)
                    {
                        if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                        {
                            m_lower_center_objects.AddToEnd(obj);
                        }
                        else
                        {
                            m_lower_center_objects.AddToBegin(obj);
                        }
                        break;
                    }
                    
                    if(obj->GetPlacement()==vmxGUIWidget::UPPER_RIGHT)
                    {
                        if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                        {
                            m_upper_right_objects.AddToEnd(obj);
                        }
                        else
                        {
                            m_upper_right_objects.AddToBegin(obj);
                        }
                        break;
                    }
                    
                    if(obj->GetPlacement()==vmxGUIWidget::LOWER_RIGHT)
                    {
                        if(obj->GetPlacementOrder()==vmxGUIWidget::ABOVE)
                        {
                            m_lower_right_objects.AddToEnd(obj);
                        }
                        else
                        {
                            m_lower_right_objects.AddToBegin(obj);
                        }
                        break;
                    }
                }
            }
        }
    }
    
    
    // F
    // E
    // F
    
    // E
    // F
    // E
    
    
    // Call RedoPlacement() on all containing objects for each list
    
//    // TOP
//    {
//        mxListIterator<vmxGUIWidget*> it;
//        for(it.SetToBegin(m_top_objects); it.IsValid(); it.MoveToNext())
//        {
//            vmxGUIWidget *obj = it.GetElement();
//            
//            if(obj->IsVisible())
//            {
//                obj->RedoPlacement();
//                
//                // update available extent
//                int obj_origin[2];
//                obj->GetOrigin(obj_origin[0],obj_origin[1]);
//                int obj_size[2];
//                obj->GetSize(obj_size[0],obj_size[1]);
//                
//                if(obj->GetPlacement()==vmxGUIWidget::UPPER_LEFT)
//                {
//                    m_left_y_extent[1] = obj_origin[1]-1-m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_center_y_extent[1] = m_left_y_extent[1];
//                        m_right_y_extent[1] = m_left_y_extent[1];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::LOWER_LEFT)
//                {
//                    m_left_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_center_y_extent[0] = m_left_y_extent[0];
//                        m_right_y_extent[0] = m_left_y_extent[0];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::UPPER_CENTER)
//                {
//                    m_center_y_extent[1] = obj_origin[1]-1-m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[1] = m_center_y_extent[1];
//                        m_right_y_extent[1] = m_center_y_extent[1];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::LOWER_CENTER)
//                {
//                    m_center_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[0] = m_center_y_extent[0];
//                        m_right_y_extent[0] = m_center_y_extent[0];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::UPPER_RIGHT)
//                {
//                    m_right_y_extent[1] = obj_origin[1]-1-m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[1] = m_right_y_extent[1];
//                        m_center_y_extent[1] = m_right_y_extent[1];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::LOWER_RIGHT)
//                {
//                    m_right_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[0] = m_right_y_extent[0];
//                        m_center_y_extent[0] = m_right_y_extent[0];
//                    }
//                }
//            }
//        }
//
//    }

    
//    // BOTTOM
//    {
//        mxListIterator<vmxGUIWidget*> it;
//        for(it.SetToBegin(m_bottom_objects); it.IsValid(); it.MoveToNext())
//        {
//            vmxGUIWidget *obj = it.GetElement();
//            
//            if(obj->IsVisible())
//            {
//                obj->RedoPlacement();
//                
//                // update available extent
//                int obj_origin[2];
//                obj->GetOrigin(obj_origin[0],obj_origin[1]);
//                int obj_size[2];
//                obj->GetSize(obj_size[0],obj_size[1]);
//                
//                if(obj->GetPlacement()==vmxGUIWidget::UPPER_LEFT)
//                {
//                    m_left_y_extent[1] = obj_origin[1]-1-m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_center_y_extent[1] = m_left_y_extent[1];
//                        m_right_y_extent[1] = m_left_y_extent[1];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::LOWER_LEFT)
//                {
//                    m_left_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_center_y_extent[0] = m_left_y_extent[0];
//                        m_right_y_extent[0] = m_left_y_extent[0];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::UPPER_CENTER)
//                {
//                    m_center_y_extent[1] = obj_origin[1]-1-m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[1] = m_center_y_extent[1];
//                        m_right_y_extent[1] = m_center_y_extent[1];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::LOWER_CENTER)
//                {
//                    m_center_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[0] = m_center_y_extent[0];
//                        m_right_y_extent[0] = m_center_y_extent[0];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::UPPER_RIGHT)
//                {
//                    m_right_y_extent[1] = obj_origin[1]-1-m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[1] = m_right_y_extent[1];
//                        m_center_y_extent[1] = m_right_y_extent[1];
//                    }
//                }
//                
//                if(obj->GetPlacement()==vmxGUIWidget::LOWER_RIGHT)
//                {
//                    m_right_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
//                    
//                    if(obj->IsStretchingOver_X_Axis())
//                    {
//                        m_left_y_extent[0] = m_right_y_extent[0];
//                        m_center_y_extent[0] = m_right_y_extent[0];
//                    }
//                }
//            }
//        }
//        
//    }

    
    // UPPER_LEFT
    {
        mxListIterator<vmxGUIWidget*> it;
        for(it.SetToBegin(m_upper_left_objects); it.IsValid(); it.MoveToNext())
        {
            vmxGUIWidget *obj = it.GetElement();
            
            //cout<<endl<<" obj is visible= "<<obj->IsVisible()<<" ";
            
            if(obj->IsVisible())
            {
                obj->RedoPlacement();
                
                // update available extent
                int obj_origin[2];
                obj->GetOrigin(obj_origin[0],obj_origin[1]);
                int obj_size[2];
                obj->GetSize(obj_size[0],obj_size[1]);
                
                //if(obj->GetPlacement()==vmxGUIWidget::UPPER_LEFT)
                //{
                    m_left_y_extent[1] = obj_origin[1]-1-m_spacing;
                    
                    if(obj->IsStretchingOver_X_Axis())
                    {
                        m_center_y_extent[1] = m_left_y_extent[1];
                        m_right_y_extent[1] = m_left_y_extent[1];
                    }
                //}
            }
        }
    }

    
    // LOWER_LEFT
    {
        mxListIterator<vmxGUIWidget*> it;
        for(it.SetToBegin(m_lower_left_objects); it.IsValid(); it.MoveToNext())
        {
            vmxGUIWidget *obj = it.GetElement();
            
            if(obj->IsVisible())
            {
                obj->RedoPlacement();
                
                // update available extent
                int obj_origin[2];
                obj->GetOrigin(obj_origin[0],obj_origin[1]);
                int obj_size[2];
                obj->GetSize(obj_size[0],obj_size[1]);
                
                //if(obj->GetPlacement()==vmxGUIWidget::LOWER_LEFT)
                {
                    m_left_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
                    
                    if(obj->IsStretchingOver_X_Axis())
                    {
                        m_center_y_extent[0] = m_left_y_extent[0];
                        m_right_y_extent[0] = m_left_y_extent[0];
                    }
                }
            }
        }
    }

    
    // UPPER_CENTER
    {
        mxListIterator<vmxGUIWidget*> it;
        for(it.SetToBegin(m_upper_center_objects); it.IsValid(); it.MoveToNext())
        {
            vmxGUIWidget *obj = it.GetElement();
            
            if(obj->IsVisible())
            {
                obj->RedoPlacement();
                
                // update available extent
                int obj_origin[2];
                obj->GetOrigin(obj_origin[0],obj_origin[1]);
                int obj_size[2];
                obj->GetSize(obj_size[0],obj_size[1]);
                
                //if(obj->GetPlacement()==vmxGUIWidget::UPPER_CENTER)
                {
                    m_center_y_extent[1] = obj_origin[1]-1-m_spacing;
                    
                    if(obj->IsStretchingOver_X_Axis())
                    {
                        m_left_y_extent[1] = m_center_y_extent[1];
                        m_right_y_extent[1] = m_center_y_extent[1];
                    }
                }
            }
        }
    }

    
    // LOWER_CENTER
    {
        mxListIterator<vmxGUIWidget*> it;
        for(it.SetToBegin(m_lower_center_objects); it.IsValid(); it.MoveToNext())
        {
            vmxGUIWidget *obj = it.GetElement();
            
            if(obj->IsVisible())
            {
                obj->RedoPlacement();
                
                // update available extent
                int obj_origin[2];
                obj->GetOrigin(obj_origin[0],obj_origin[1]);
                int obj_size[2];
                obj->GetSize(obj_size[0],obj_size[1]);
                
                //if(obj->GetPlacement()==vmxGUIWidget::LOWER_CENTER)
                {
                    m_center_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
                    
                    if(obj->IsStretchingOver_X_Axis())
                    {
                        m_left_y_extent[0] = m_center_y_extent[0];
                        m_right_y_extent[0] = m_center_y_extent[0];
                    }
                }
            }
        }
    }

    
    // UPPER_RIGHT
    {
        mxListIterator<vmxGUIWidget*> it;
        for(it.SetToBegin(m_upper_right_objects); it.IsValid(); it.MoveToNext())
        {
            vmxGUIWidget *obj = it.GetElement();
            
            if(obj->IsVisible())
            {
                obj->RedoPlacement();
                
                // update available extent
                int obj_origin[2];
                obj->GetOrigin(obj_origin[0],obj_origin[1]);
                int obj_size[2];
                obj->GetSize(obj_size[0],obj_size[1]);
                
                //if(obj->GetPlacement()==vmxGUIWidget::UPPER_RIGHT)
                {
                    m_right_y_extent[1] = obj_origin[1]-1-m_spacing;
                    
                    if(obj->IsStretchingOver_X_Axis())
                    {
                        m_left_y_extent[1] = m_right_y_extent[1];
                        m_center_y_extent[1] = m_right_y_extent[1];
                    }
                }
            }
        }
    }

    
    // LOWER_RIGHT
    {
        mxListIterator<vmxGUIWidget*> it;
        for(it.SetToBegin(m_lower_right_objects); it.IsValid(); it.MoveToNext())
        {
            vmxGUIWidget *obj = it.GetElement();
            
            if(obj->IsVisible())
            {
                obj->RedoPlacement();
                
                // update available extent
                int obj_origin[2];
                obj->GetOrigin(obj_origin[0],obj_origin[1]);
                int obj_size[2];
                obj->GetSize(obj_size[0],obj_size[1]);
                
                //if(obj->GetPlacement()==vmxGUIWidget::LOWER_RIGHT)
                {
                    m_right_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
                    
                    if(obj->IsStretchingOver_X_Axis())
                    {
                        m_left_y_extent[0] = m_right_y_extent[0];
                        m_center_y_extent[0] = m_right_y_extent[0];
                    }
                }
            }
        }
    }
}



void vmxGUIMainWidget::Reset()
{
    // should call reset on all containing objects.
//    mxListIterator<vmxGUIWidget*> it;
//    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
//    {
//        it.GetElement()->Reset();
//    }
}


void vmxGUIMainWidget::SetRenderWindow(vtkSDL2OpenGLRenderWindow *render_window)
{
    //if(m_render_window == render_window) return;
    
    if(m_render_window)
    {
        m_render_window->RemoveObservers(vtkCommand::ModifiedEvent, m_window_modified_callback);
    }
    
    m_render_window = render_window;
    
    if(m_render_window)
    {
        m_render_window->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
    }
}


void vmxGUIMainWidget::SetSize(int size_x, int size_y)
{
    if(size_x<=0 || size_y<=0) return;
    
    m_render_window_internal->SetSize(size_x, size_y);
    m_size_xy[0] = size_x;
    m_size_xy[1] = size_y;
}


