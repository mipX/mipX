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



#if defined(vmxGUIWidget_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIWidget_EXPORTS
            #define vmxGUIWidget_API __declspec(dllexport)
        #else
            #define vmxGUIWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIWidget_EXPORTS
            #define vmxGUIWidget_API __attribute__((visibility("default")))
        #else
            #define vmxGUIWidget_API
        #endif
    #endif
#endif



#ifndef vmxGUIWidget_H
	#define vmxGUIWidget_H



#include "mxList.h"
#include "mxString.h"
#include "vmxGUIClipBoard.h"
//#include "vmxGUIInteractorStyleTrackballCamera.h"
#include "vmxGUIInteractorStyle.h"

#include <iostream>

#include <vtkActor2D.h>
#include <vtkSDL2OpenGLRenderWindow.h>
#include <vtkSDL2RenderWindowInteractor.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyle.h>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



// Pre-declarations.
class vmxGUIBaseObject;
class vmxGUIConnection;
class vmxGUIWidget;
class vmxGUIWidgetCollection;
class vmxGUIMainWidget;
class vmxGUIInteractorStyle;
class vmxGUIRenderer;




//----------------------------------------------------------------------------------------------------------------------




static const int VMX_GUI_USER_EVENT_START_INDEX = 500;
static int vmxGUI_user_event_index = VMX_GUI_USER_EVENT_START_INDEX + 1;
    

/// VTK GUI Event IDs

enum vmxGUIEventID
{
    NoEvent = 0,
    /*AnyEvent, DeleteEvent, StartEvent,
    EndEvent, RenderEvent, ProgressEvent, PickEvent,
    StartPickEvent, EndPickEvent, AbortCheckEvent, ExitEvent,
    LeftButtonPressEvent, LeftButtonReleaseEvent, MiddleButtonPressEvent, MiddleButtonReleaseEvent,
    RightButtonPressEvent, RightButtonReleaseEvent, EnterEvent, LeaveEvent,
    KeyPressEvent, KeyReleaseEvent, CharEvent, ExposeEvent,
    ConfigureEvent, TimerEvent, MouseMoveEvent, MouseWheelForwardEvent,
    MouseWheelBackwardEvent, ResetCameraEvent, ResetCameraClippingRangeEvent, ModifiedEvent,
    WindowLevelEvent, StartWindowLevelEvent, EndWindowLevelEvent, ResetWindowLevelEvent,
    SetOutputEvent, ErrorEvent, WarningEvent, StartInteractionEvent,
    InteractionEvent, EndInteractionEvent, EnableEvent, DisableEvent,
    CreateTimerEvent, DestroyTimerEvent, PlacePointEvent, PlaceWidgetEvent,
    CursorChangedEvent, ExecuteInformationEvent, RenderWindowMessageEvent, WrongTagEvent,
    StartAnimationCueEvent, AnimationCueTickEvent, EndAnimationCueEvent, VolumeMapperRenderEndEvent,
    VolumeMapperRenderProgressEvent, VolumeMapperRenderStartEvent, VolumeMapperComputeGradientsEndEvent, VolumeMapperComputeGradientsProgressEvent,
    VolumeMapperComputeGradientsStartEvent, WidgetModifiedEvent, WidgetValueChangedEvent, WidgetActivateEvent,
    ConnectionCreatedEvent, ConnectionClosedEvent, DomainModifiedEvent, PropertyModifiedEvent,
    UpdateEvent, RegisterEvent, UnRegisterEvent, UpdateInformationEvent,
    SelectionChangedEvent, UpdatePropertyEvent,*/
    ButtonClickedEvent, ButtonOKClickedEvent, ButtonCancelClickedEvent,
    ImageSliceChangeEvent, ImageSelectionChangeEvent, ImagePickedEvent,
    ItemSelectedEvent, ItemModifiedEvent,
    KeyPressEvent,
    LeftButtonUpEvent, LeftButtonDownEvent, LeftButtonSingleClickEvent, LeftButtonDoubleClickEvent, LeftButtonDragEvent, LeftButtonDropEvent,
    MouseMoveEvent,
    MouseWheelForwardEvent, MouseWheelBackwardEvent,
    SeedPointsModifiedEvent,
    UserEvent = VMX_GUI_USER_EVENT_START_INDEX
};

    

/// Macro used to declare a new event.
#define vmxGUIMacro_DeclareNewEvent(event) \
unsigned long int event = vmxGUI_user_event_index; \
vmxGUI_user_event_index = vmxGUI_user_event_index + 1; \


/// Define a string that is used to separate items in a string listing.
static const char *vmxGUIItemSeparationString = ", ";
static const char vmxGUIItemSeparationChar1 = ',';
static const char vmxGUIItemSeparationChar2 = ' ';


/// Type definition of Slot function with connection object as input argument. With this type declaration,the slot is eather a global function or a static method within a class.
typedef int (*vmxGUISlotFunction)(vmxGUIConnection *connection);



/// Connection used to establish a signal - slot binding.

class vmxGUIWidget_API vmxGUIConnection
{
protected:

    /// Pointer to the main widget, the same for the whole class.
    static vmxGUIMainWidget *m_main_widget;

    /// GUI objet that is triggering this connection.
    vmxGUIBaseObject *m_sender_object;
    
    /// Object which is not GUI object, that is triggering this connection.
    void *m_sender_non_gui_object;

    /// Trigger event ID number.
    vmxGUIEventID m_trigger_event_id;

    /// GUI object that is being used when the connection is triggered.
    vmxGUIBaseObject *m_receiver_object;

    /// Object on which the slot is executed (object of the class that has the slot as a method).
    void *m_slot_caller_object;

    /// Slot that will be executed when the connection is triggered
    vmxGUISlotFunction m_slot;


/// Define the max number of passed data elements for a connection.
#define vmxGUIConnection_max_number_of_passed_data_elements 10

    /// Data that is being passed to the receiver.
    void **m_passed_address[vmxGUIConnection_max_number_of_passed_data_elements];

    /// Data that is being passed to the receiver.
    void *m_passed_data[vmxGUIConnection_max_number_of_passed_data_elements];

    /// Data that is being passed to the receiver.
    double *m_passed_data_double[vmxGUIConnection_max_number_of_passed_data_elements];

    /// Data that is being passed to the receiver.
    int *m_passed_data_int[vmxGUIConnection_max_number_of_passed_data_elements];

    /// Data that is being passed to the receiver.
    unsigned int *m_passed_data_unsigned_int[vmxGUIConnection_max_number_of_passed_data_elements];




public:

    // Example use:
    // int Slot_PrintConnectionProperties(vmxGUIConnection *connection);
    // vmxGUIConnection::New(caller_object,LeftButtonDoubleClick,receiver_object,Slot_PrintConnectionProperties);
    // vmxGUIConnection::Delete(caller_object,LeftButtonDoubleClick,receiver_object,Slot_PrintConnectionProperties);


    /// Constructor.
    vmxGUIConnection()
    {
        m_sender_object = NULL;
        m_sender_non_gui_object = NULL;
        
        for(int i=0; i<vmxGUIConnection_max_number_of_passed_data_elements; i++)
        {
            m_passed_address[i] = NULL;
            m_passed_data[i] = NULL;
            m_passed_data_double[i] = NULL;
            m_passed_data_int[i] = NULL;
            m_passed_data_unsigned_int[i] = NULL;
        }
        m_receiver_object = NULL;
        m_slot = NULL;
        m_slot_caller_object = NULL;
    };

    /// Destructor.
    ~vmxGUIConnection() {};

    /// To be used internally!
    static void internal_SetMainWidget(vmxGUIMainWidget *main_widget)
    {
        m_main_widget = main_widget;
    };

    /// Create a new object instance.
//    static vmxGUIConnection* New(vmxGUIBaseObject *sender, void *sender_subobject, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, void *receiver_subobject, vmxGUISlotFunction slot);
//    static vmxGUIConnection* New(vmxGUIBaseObject *sender, void *sender_subobject, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, vmxGUISlotFunction slot);

    /// Create a new connection.
    /// 'sender' is an object that catches some event and initiates the connection exectution (e.g. a push button will intiate some function when it is pressed).
    /// 'sender_non_gui_object' is an object usually contained in the 'sender' or that contains the 'sender' which is not GUI object and that initiates connection exectution.
    ///  Default value is NULL, which means that there is no sender non-gui object info.
    /// 'trigger_event_id' is the event (signal) that triggers execution (e.g. pressing a button, selecting an item...).
    /// 'receiver' is an object that receives this connection. It is not necessarily the object that calles the slot method (that is slot_caller_object).
    ///  In case 'receiver' is not defined, sender is used as a receiver.
    /// 'slot_caller_object' is an object that executes its static method (defined by 'slot').
    /// 'slot' is the actual static method being executed.
    static vmxGUIConnection* New(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, void *sender_non_gui_object, vmxGUIBaseObject *receiver, void *slot_caller_object, vmxGUISlotFunction slot);
    static vmxGUIConnection* New(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, void *slot_caller_object, vmxGUISlotFunction slot);
    
    //static vmxGUIConnection* New(vmxGUIBaseObject *sender, void *sender_subobject,  vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, void *slot_caller_object, vmxGUISlotFunction slot);

//    /// This is a special type of New method, where the receiver is the vmxGUIMainWidget. In fact, it is equal to setting sender as receiver object and
//   /// vmxGUIMainWidget as receiver_subobject.
//    static vmxGUIConnection* New(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIMainWidget *receiver, vmxGUISlotFunction slot);
//    static vmxGUIConnection* New(vmxGUIBaseObject *sender, void *sender_subobject, vmxGUIEventID trigger_event_id, vmxGUIMainWidget *receiver, vmxGUISlotFunction slot);


    /// Delete an object instance.
    static int Delete(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, vmxGUISlotFunction slot);
    static int Delete(vmxGUIConnection *connection);
    
    /// Get main widget.
    vmxGUIMainWidget* GetMainWidget() { return m_main_widget; };

    /// Get GUI objet that is triggering this connection.
    vmxGUIBaseObject* GetSenderObject() { return m_sender_object; };

    /// Get Trigger event ID number.
    vmxGUIEventID GetEventID() { return m_trigger_event_id; };

    /// Get the sender non-GUI object from which the connection is triggered.
    void* GetSenderNonGUIObject() { return m_sender_non_gui_object; };

    /// Get data that is being passed to the receiver.
    void** GetPassedAddress(unsigned int i=0) { return m_passed_address[i]; };

    /// Get data that is being passed to the receiver.
    void* GetPassedData(unsigned int i=0) { return m_passed_data[i]; };

    /// Get data that is being passed to the receiver.
    double* GetPassedDataDouble(unsigned int i=0) { return m_passed_data_double[i]; };

    /// Get data that is being passed to the receiver.
    int* GetPassedDataInt(unsigned int i=0) { return m_passed_data_int[i]; };

    /// Get data that is being passed to the receiver.
    unsigned int* GetPassedDataUnsignedInt(unsigned int i=0) { return m_passed_data_unsigned_int[i]; };

//    /// Get Slot Function that is being passed to the receiver.
//    vmxGUISlotFunction GetPassedSlotFunction(unsigned int i=0) { return m_passed_slot_function[i]; };

    /// Get GUI object that is being used when the connection is triggered.
    vmxGUIBaseObject* GetReceiverObject() { return m_receiver_object; };

//    /// Get a part of the GUI object that is being used when the connection is triggered.
//    void* GetReceiverSubObject() { return m_receiver_subobject; };

    /// Get the Slot that will be executed when the connection is triggered.
    vmxGUISlotFunction GetSlotFunction() { return m_slot; };

    /// Get the Object on which the slot is executed (object of the class that has the slot as a method).
    void* GetSlotCallerObject() { return m_slot_caller_object; };

    /// Check if this connection is equal to the input one (all member variables have to be equal).
    int IsEqualTo(vmxGUIConnection &connection);

    /// Check if this connection is equal to the input one (all member variables have to be equal).
    int IsEqualTo(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, vmxGUISlotFunction slot);
    
    /// Execute the slot function.
    int Execute()
    {
        return m_slot(this);
    };

    /// Print out the properties of this connection.
    void PrintSelf();

//    /// Set ptoperties of an existing connection object.
//    void Set(vmxGUIBaseObject *sender, void *sender_subobject, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, void *receiver_subobject, vmxGUISlotFunction slot);

    /// Set data that is being passed to the receiver.
    void SetPassedAddress(void **passed_address, unsigned int i=0) { m_passed_address[i] = passed_address; };

    /// Set data that is being passed to the receiver.
    void SetPassedData(void *passed_data, unsigned int i=0) { m_passed_data[i] = passed_data; };

    /// Set data that is being passed to the receiver.
    void SetPassedDataDouble(double *passed_data, unsigned int i=0) { m_passed_data_double[i] = passed_data; };

    /// Set data that is being passed to the receiver.
    void SetPassedDataInt(int *passed_data, unsigned int i=0) { m_passed_data_int[i] = passed_data; };

    /// Set data that is being passed to the receiver.
    void SetPassedDataUnsignedInt(unsigned int *passed_data, unsigned int i=0) { m_passed_data_unsigned_int[i] = passed_data; };


};


//typedef vmxGUIConnection vmxGUISlotDataContainer;



//----------------------------------------------------------------------------------------------------------------------



/// Connection magaer stores established connections and calls slots on given signals.

class vmxGUIWidget_API vmxGUIConnectionManager
{
private:
    
//!!!!!!!!!!!!!!!!! CONNECTION MANAGER IS BOUND TO SENDER OBJECT, SO IT SHOULD HAVE A PRIVATE MEMBER WHICH POINTS TO ITS CONTAINING OBJECT !!!!!!!!!!!!!!!!!!!!!!!!!!
    
    
//!!!!!!!!!!!!!!!!! MAKE 2 DIFFERENT STRUCTURES FOR STORING CONNECTIONS, ONE TO BE USED WHEN THE CONTAINING OBJECT IS A SENDER AND ONE WHEN IT IS A REVEIVER !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    /// Array of (list of) connections sorted by event id.
    mxArray< mxList< vmxGUIConnection* > > m_connections_by_event_id;
    
    
    // vmxGUIBaseObject *m_sender;
    
    
public:
    
    /// Constructor.
    vmxGUIConnectionManager();
    
    /// Destructor.
    ~vmxGUIConnectionManager();
    
    /// Add a newly created connection to the list of connections.
    void AddConnection(vmxGUIConnection *connection);
    
//!!!!!!!!!!!!!!!!! CONNECTION MANAGER IS BOUND TO SENDER OBJECT, SO IT SHOULD NOT HAVE TO TAKE sender_object AS INPUT OR TO CHECK ITS SENDER OBJECT WHEN EXECUTING SLOT !!!!!!!!!!!!!!!!!!!
    /// Get connection with the given properties and return its pointer. If none found, return NULL.
    vmxGUIConnection* GetConnection(vmxGUIBaseObject *sender, vmxGUIEventID trigger_event_id, vmxGUIBaseObject *receiver, vmxGUISlotFunction slot);
    
    /// Get connection
    void GetConnection(vmxGUIEventID event_id, vmxGUIBaseObject *sender_object);
    
    /// Remove the connection equal to the input connection from the list.
    vmxGUIConnection* RemoveConnection(vmxGUIConnection &connection);

//!!!!!!!!!!!!!!!!! CONNECTION MANAGER IS BOUND TO SENDER OBJECT, SO IT SHOULD NOT HAVE TO TAKE sender_object AS INPUT OR TO CHECK ITS SENDER OBJECT WHEN EXECUTING SLOT !!!!!!!!!!!!!!!!!!!
    /// Execute the slot based on the info of sender (sub)object and event.
    int Execute(vmxGUIEventID event_id, vmxGUIBaseObject *sender_object);//, void *sender_subobject);
};




//----------------------------------------------------------------------------------------------------------------------




/// Base calss of GUI objects. All objects inherit from this class.

class vmxGUIWidget_API vmxGUIBaseObject
{
public:
    
    
protected:
    
    /// Main widget to which this object belongs.
    vmxGUIMainWidget *m_main_widget;
    
    /// Pointer to clipboard this object uses.
    vmxGUIClipBoard *m_clip_board;
    
    /// Connections Manager for signal-slot calling.
    vmxGUIConnectionManager m_connection_manager;
    
    /// Class (type) unique name string.
    mxString m_class_name;
    
    /// Object name string. It can happen that this name is not unique (depends how the higher level app handles object names).
    mxString m_object_name;
    
    /// Indicates if the object accepts a specific event. These events are forwarded (by interactor style) only
    /// to the object which is picked by the events. E.g. double click is forwarded only to the clicked object
    /// (if corresonding indicator is set).
    int m_is_accepting_LeftButtonDoubleClick_event;
    int m_is_accepting_LeftButtonSingleClick_event;
    int m_is_accepting_LeftButtonDrag_event;
    int m_is_accepting_LeftButtonDrop_event;
    
    /// Indicates if the object is listening for a specific event.
    int m_is_listening_for_KeyPress_event;
    int m_is_listening_for_MouseMove_event;
    int m_is_listening_for_LeftButtonUp_event;
    int m_is_listening_for_LeftButtonDown_event;
    int m_is_listening_for_MouseWheelForward_event;
    int m_is_listening_for_MouseWheelBackward_event;
    
    
    /// Use this method in constructor of child classes to set their class name.
    void SetClassName(const char *class_name);
    
    
public:
    
    
    /// Constructor.
    vmxGUIBaseObject();
    
    /// Destructor.
    virtual ~vmxGUIBaseObject();
    
    //    /// Reset the object.
    //    virtual void Reset();
    
    /// Get class (type) unique name string.
    mxString GetClassName();
    
    /// Get the pointer to the clipboard this object uses.
    vmxGUIClipBoard* GetClipBoard()
    {
        return m_clip_board;
    };
    
    /// Get the pointer to the manager of connections.
    vmxGUIConnectionManager* GetConnectionManager()
    {
        return &m_connection_manager;
    };
    
    /// Get the pointer to the main widget.
    vmxGUIMainWidget* GetMainWidget()
    {
        return m_main_widget;
    };
    
    /// Get object name string. It can happen that this name is not unique (depends how the higher level app handles object names).
    mxString GetObjectName();
    
    /// Check if the given 2D VTK actor is contained in this object.
    virtual int HasVTKActor2D(vtkActor2D *actor_2D)
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::HasVTKActor2D(vtkActor2D*)!"<<endl;
        return 0;
    };
    
    /// Invoke an event from the contained connection manager.
    int InvokeEvent(vmxGUIEventID event_id, vmxGUIBaseObject *sender_object, void *sender_subobject)
    {
        return m_connection_manager.Execute(event_id, sender_object);//, sender_subobject);
    };
    
    /// Indicate if the object is accepting the given event.
    int IsAccepting_LeftButtonDoubleClick_Event()
    {
        return m_is_accepting_LeftButtonDoubleClick_event;
    };
    
    /// Indicate if the object is accepting the given event.
    int IsAccepting_LeftButtonSingleClick_Event()
    {
        return m_is_accepting_LeftButtonSingleClick_event;
    };
    
    /// Indicate if the object is accepting the given event.
    int IsAccepting_LeftButtonDrag_Event()
    {
        return m_is_accepting_LeftButtonDrag_event;
    };
    
    /// Indicate if the object is accepting the given event.
    int IsAccepting_LeftButtonDrop_Event()
    {
        return m_is_accepting_LeftButtonDrop_event;
    };
    
    /// Indicate if the object is listening for the given event.
    int IsListeningFor_KeyPress_Event()
    {
        return m_is_listening_for_KeyPress_event;
    };
    
    /// Indicate if the object is listening for the given event.
    int IsListeningFor_MouseMove_Event()
    {
        return m_is_listening_for_MouseMove_event;
    };
    
    /// Indicate if the object is listening for the given event.
    int IsListeningFor_LeftButtonUp_Event()
    {
        return m_is_listening_for_LeftButtonUp_event;
    };
    
    /// Indicate if the object is listening for the given event.
    int IsListeningFor_LeftButtonDown_Event()
    {
        return m_is_listening_for_LeftButtonDown_event;
    };
    
    /// Indicate if the object is listening for the given event.
    int IsListeningFor_MouseWheelForward_Event()
    {
        return m_is_listening_for_MouseWheelForward_event;
    };
    
    /// Indicates if the object is listening for the given event.
    int IsListeningFor_MouseWheelBackward_Event()
    {
        return m_is_listening_for_MouseWheelBackward_event;
    };
    
    /// Indicate if the object was picked by checking if the screen pick coordinates fall inside the object.
    virtual int IsPicked(int pos1, int pos2)
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::IsPicked(int,int)!"<<endl;
        return 0;
    };
    
    /// Get visibility of this object.
    virtual int IsVisible()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::IsVisible()!"<<endl; return 0;
    };
    
    /// Callback method executed for the given event.
    virtual void OnKeyPress() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnKeyPress()!"<<endl; };
    virtual void OnMouseMove() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnMouseMove()!"<<endl; };
    virtual void OnLeftButtonUp() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnLeftButtonUp()!"<<endl; };
    virtual void OnLeftButtonDown() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnLeftButtonDown()!"<<endl; };
    virtual void OnLeftButtonDoubleClick() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnLeftButtonDoubleClick()!"<<endl; };
    virtual void OnLeftButtonSingleClick() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnLeftButtonSingleClick()!"<<endl; };
    virtual void OnLeftButtonDrag() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnLeftButtonDrag()!"<<endl; };
    virtual void OnLeftButtonDrop() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnLeftButtonDrop()!"<<endl; };
    virtual void OnMouseWheelForward() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnMouseWheelForward()!"<<endl; };
    virtual void OnMouseWheelBackward() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::OnMouseWheelBackward()!"<<endl; };
    
    /// Toggle on/off accepting of the given event.
    void SetAccepting_LeftButtonDoubleClick_Event(int is_accepting) { m_is_accepting_LeftButtonDoubleClick_event = is_accepting; };
    void SetAccepting_LeftButtonSingleClick_Event(int is_accepting) { m_is_accepting_LeftButtonSingleClick_event = is_accepting; };
    void SetAccepting_LeftButtonDrag_Event(int is_accepting) { m_is_accepting_LeftButtonDrag_event = is_accepting; };
    void SetAccepting_LeftButtonDrop_Event(int is_accepting) { m_is_accepting_LeftButtonDrop_event = is_accepting; };
    
    /// Toggle on/off listening for the given event.
    void SetListeningFor_KeyPress_Event(int is_listening);
    
    /// Toggle on/off listening for the given event.
    void SetListeningFor_MouseMove_Event(int is_listening);
    
    /// Toggle on/off listening for the given event.
    void SetListeningFor_LeftButtonUp_Event(int is_listening);
    
    /// Toggle on/off listening for the given event.
    void SetListeningFor_LeftButtonDown_Event(int is_listening);
    
    /// Toggle on/off listening for the given event.
    void SetListeningFor_MouseWheelForward_Event(int is_listening);
    
    /// Toggle on/off listening for the given event.
    void SetListeningFor_MouseWheelBackward_Event(int is_listening);
    
    /// Set the pointer to the main widget.
    virtual void SetMainWidget(vmxGUIMainWidget *main_widget)
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::SetMainWidget(vmxGUIMainWidget*)!"<<endl;
    };
    
    /// Set the object name. It can happen that this name is not unique (depends how the higher level app handles object names).
    void SetObjectName(const char *object_name);
    
    /// Set visibility of the object.
    virtual void SetVisibility(int is_visible);
    //    {
    //        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::SetVisibility()!"<<endl;
    //    };
    
};




//----------------------------------------------------------------------------------------------------------------------







/// Base calss of GUI widdgets. All widgets inherit from this one.

class vmxGUIWidget_API vmxGUIWidgetBase : public vmxGUIBaseObject
{
public:
    
    /// Predefined placement positions for object.
    enum vmxGUIWidgetPlacement
    {
        RELATIVE, //'relative' means relative to window size in percentages.
        FIXED, //'fixed' means positioned to given absolute coordinates.
        LOWER_LEFT,
        LOWER_CENTER,
        LOWER_RIGHT,
        CENTER_LEFT,
        CENTER_CENTER,
        CENTER_RIGHT,
        UPPER_LEFT,
        UPPER_CENTER,
        UPPER_RIGHT,
    };
    
    /// Predefined placement orders for object.
    enum vmxGUIWidgetPlacementOrder
    {
        ABOVE, // 'above' means that the object will be placed above (on top) of other objects with the same placement.
        BELOW, // 'below' means that the object will be placed below other objects with the same placement.
    };
    
protected:
    
    /// Pointer to the collection containing this object. If none the pointer is NULL (default).
    vmxGUIWidgetCollection *m_collection;
    
    /// Position (placement) of the list_widget. FREE is default.
    vmxGUIWidgetPlacement m_placement;
    
    /// Position within the placement of the object. ABOVE is default.
    /// 'above' means that the object will be placed above (on top) of other objects with the same placement.
    /// 'below' means that the object will be placed bellow other objects with the same placement.
    vmxGUIWidgetPlacementOrder m_placement_order;
    
    /// Indicates if the object stretches over the whole X axis.
    int m_is_stretching_over_x_axis;
    
    /// Indicates if this object occupies GUI renderer.
    int m_is_occupying_renderer;
    
    
public:
    
    
    /// Constructor.
    vmxGUIWidgetBase();
    
    /// Destructor.
    virtual ~vmxGUIWidgetBase();
    
//    /// Reset the object.
//    virtual void Reset();
    
    /// Get the pointer to the collection containing this object. If none the pointer is NULL (default).
    vmxGUIWidgetCollection* GetCollection()
    {
        return m_collection;
    };
    
    /// Get the array of all widgets contained within this one.
    virtual mxArray<vmxGUIWidget*> GetWidgets()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::GetWidgets()!"<<endl;
        mxArray<vmxGUIWidget*> empty_array; return empty_array;
    };
    
    /// Get the placement type for this object.
    vmxGUIWidgetBase::vmxGUIWidgetPlacement GetPlacement();
    
    /// Get the placement order for this object.
    vmxGUIWidgetBase::vmxGUIWidgetPlacementOrder GetPlacementOrder();
    
    /// Indictes is this object occupies the whole GUI renderer.
    int IsOccupayingRenderer()
    {
        return m_is_occupying_renderer;
    };
    
    /// Check if the object stretches over the whole X axis.
    int IsStretchingOver_X_Axis();

    /// Callback method executed for the given event.
    virtual void OnKeyPress() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnKeyPress()!"<<endl; };
    virtual void OnMouseMove() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnMouseMove()!"<<endl; };
    virtual void OnLeftButtonUp() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnLeftButtonUp()!"<<endl; };
    virtual void OnLeftButtonDown() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnLeftButtonDown()!"<<endl; };
    virtual void OnLeftButtonDoubleClick() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnLeftButtonDoubleClick()!"<<endl; };
    virtual void OnLeftButtonSingleClick() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnLeftButtonSingleClick()!"<<endl; };
    virtual void OnLeftButtonDrag() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnLeftButtonDrag()!"<<endl; };
    virtual void OnLeftButtonDrop() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnLeftButtonDrop()!"<<endl; };
    virtual void OnMouseWheelForward() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnMouseWheelForward()!"<<endl; };
    virtual void OnMouseWheelBackward() { cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::OnMouseWheelBackward()!"<<endl; };
    
    /// Set the pointer to the collection containing this object.
    void SetCollection(vmxGUIWidgetCollection* collection)
    {
        m_collection = collection;
    };
    
    /// Set the object name. It can happen that this name is not unique (depends how the higher level app handles object names).
    void SetObjectName(const char *object_name);
    
    /// Set the order of object placement so that the object appears above existing object at the same placement.
    virtual void SetPlacementOrderToAbove()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementOrderToAbove()!"<<endl;
    };
    
    /// Set the order of object placement so that the object appears below existing object at the same placement.
    virtual void SetPlacementOrderToBelow()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementOrderToBelow()!"<<endl;
    };
    
    /// Place the object relative to the size of the render window.
    virtual void SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent)
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToRelative(unsigned int, unsigned int)!"<<endl;
    };
    
    /// Place the object in the cernter left side of the render window.
    virtual void SetPlacementToCenterCenter()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToCenterCenter()!"<<endl;
    };
    
    /// Place the object in the cernter left side of the render window.
    virtual void SetPlacementToCenterLeft()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToCenterLeft()!"<<endl;
    };
    
    /// Place the object in the cernter right side of the render window.
    virtual void SetPlacementToCenterRight()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToCenterRight()!"<<endl;
    };
    
    /// Place the object in the lower center of the render window.
    virtual void SetPlacementToLowerCenter()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToLowerCenter()!"<<endl;
    };
    
    /// Place the object in the lower left corner of the render window.
    virtual void SetPlacementToLowerLeft()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToLowerLeft()!"<<endl;
    };
    
    /// Place the object in the lower right corner of the render window.
    virtual void SetPlacementToLowerRight()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToLowerRight()!"<<endl;
    };
    
    /// Place the object in the upper center of the render window.
    virtual void SetPlacementToUpperCenter()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToUpperCenter()!"<<endl;
    };
    
    /// Place the object in the upper left corner of the render window.
    virtual void SetPlacementToUpperLeft()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToUpperLeft()!"<<endl;
    };
    
    /// Place the object in the upper right corner of the render window.
    virtual void SetPlacementToUpperRight()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetBase::SetPlacementToUpperRight()!"<<endl;
    };
    
    /// Set visibility of the object.
    virtual void SetVisibility(int is_visible);
//    {
//        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIBaseObject::SetVisibility()!"<<endl;
//    };

};




//----------------------------------------------------------------------------------------------------------------------



/// Object class as parent of all widgets.

class vmxGUIWidget_API vmxGUIWidget : public vmxGUIWidgetBase
{

protected:
    
    /// For relative placement, the percentages are stored here.
    unsigned int m_placement_relative_percentages[2];
    
    /// Used to define the exact position of the object based on its placement.
    /// It will be used when the object needs to be re-positioned after rescaling (e.g. after scroll event),
    /// but in case when the RedoPlacemet does not need to be called (placement is not changed). These values
    /// are in fact: (origin_x, origin_x+size_x, origin_y, origin_y+size_y).
    int m_object_bounds[4];
    
    /// Maximum available size for this object on the screen and consequently the maximum size that
    /// this object can have. Important for scalable-size objects to determine their limits of scaling.
    int m_maximum_size[2];

    
public:
    

	/// Constructor.
	vmxGUIWidget();

	/// Destructor.
	virtual ~vmxGUIWidget();
    
    /// Copy members of input object.
    /// Notice: No need to copy class names, as they are the same for each object of the same class.
    virtual int CopyFrom(vmxGUIWidget *object);
    
    /// Get origin of the object in absolute coordinates on the render window.
    /// These are absolute (actual) positions on the screen.
    virtual void GetOrigin(int &x_origin, int &y_origin) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::GetOrigin()!"<<endl; };
    
    /// Get size of the object.
    virtual void GetSize(int &x_size, int &y_size) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::GetSize()!"<<endl; };
    
    /// Get the array of all widgets contained within this one.
    mxArray<vmxGUIWidget*> GetWidgets();
    
    /// Check if the given 2D VTK actor is contained in this object.
    virtual int HasVTKActor2D(vtkActor2D *actor_2D)
    {
        cout<<"Widget '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidget::HasVTKActor2D()!"<<endl;
        return 0;
    };
    
    /// Given the input positions, check if the object is picked (if the position falls within the object bounds).
    int IsPicked(int pos1, int pos2);
    
    /// Get visibility of this object.
    virtual int IsVisible() { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::IsVisible()!"<<endl; return 0; };
    
    /// Reset the object.
    virtual void Reset() { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::Reset()!"<<endl; };
    
    /// Based on existing placement preference, repositions the object.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Method to be called when the object changes size, but not it's placement.
    /// E.g. a list becomes smaller (e.g. due to scrolling) but remains placed at its previous placement (e.g. lower right corner).
    /// The repositioning will be done based on the saved values of m_position_of_upper_right_corner.
    void RepositionAfterResizing();
    
    /// Set the pointer to the main widget.
    virtual void SetMainWidget(vmxGUIMainWidget *main_widget);
    
    /// Set the maximum size of the object.
    virtual void SetMaximumSize(int max_size_x, int max_size_y)
    {
        // // Should check if the new size is different than the old one...
        // if(max_size_x!=m_maximum_size[0] || max_size_y!=m_maximum_size[1])
        // {
        //      m_maximum_size[0] = max_size_x; m_maximum_size[1] = max_size_y;
        //      // now perform resizing of the object.
        // }
        cout<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::SetMaximumSize()!"<<endl;
    };
    
    /// Set interactor of the object.
    virtual void SetInteractor(vtkRenderWindowInteractor *interactor) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::SetInteractor()!"<<endl; };

    /// Set origin of the object on the screen.
    /// These are absolute (actual) positions on the screen.
    virtual void SetOrigin(int x_origin, int y_origin) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::SetOrigin()!"<<endl; };
    
    /// Set the order of object placement so that the object appears above existing object at the same placement.
    void SetPlacementOrderToAbove();
    
    /// Set the order of object placement so that the object appears below existing object at the same placement.
    void SetPlacementOrderToBelow();
    
    /// Place the object relative to the size of the render window.
    void SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent);
    
    /// Place the object in the cernter left side of the render window.
    void SetPlacementToCenterCenter();
    
    /// Place the object in the cernter left side of the render window.
    void SetPlacementToCenterLeft();
    
    /// Place the object in the cernter right side of the render window.
    void SetPlacementToCenterRight();
    
    /// Place the object in the lower center of the render window.
    void SetPlacementToLowerCenter();
    
    /// Place the object in the lower left corner of the render window.
    void SetPlacementToLowerLeft();
    
    /// Place the object in the lower right corner of the render window.
    void SetPlacementToLowerRight();
    
    /// Place the object in the upper center of the render window.
    void SetPlacementToUpperCenter();
    
    /// Place the object in the upper left corner of the render window.
    void SetPlacementToUpperLeft();
    
    /// Place the object in the upper right corner of the render window.
    void SetPlacementToUpperRight();
    
    /// Set the indicator showing if the object stretches over the whole X axis.
    void SetStretchingOver_X_Axis(int is_stretching_over_x_axis);
    
    /// Set visibility of the object. This method should be re-implemented
    /// in child class, but should call RedoPlacement() mehod within it!
    virtual void SetVisibility(int is_visible)
    {
        std::cout<<std::endl<<this->m_class_name.Get_C_String()<<" called vmxGUIWidget::SetVisibility()! ";
    };
};




//----------------------------------------------------------------------------------------------------------------------



/// Collection of widgets. Serves as a frame for widgets which are grouped togehter (e.g. for making more complex forms).

class vmxGUIWidget_API vmxGUIWidgetCollection : public vmxGUIWidgetBase //vmxGUIBaseObject
{

protected:
    
    /// List of contained objects.
    mxList<vmxGUIWidget*> m_objects;//mxList<vmxGUIWidgetBase*> m_objects;
    
    /// Object within the collection that called the last event callback method.
    vmxGUIBaseObject *m_event_caller_object;
    
    /// Position at which event occured within the caller object (that belongs to this collection).
    int m_event_caller_object_position_of_event[2];

    /// Indicates if the collection of widgets is visible.
    int m_is_visible_collection;

public:


    /// Constructor.
    vmxGUIWidgetCollection();

    /// Destructor.
    virtual ~vmxGUIWidgetCollection();
    
    /// Get the array of all widgets contained within this one.
    mxArray<vmxGUIWidget*> GetWidgets();

    /// Add a widget to the collection.
    virtual int AddWidget(vmxGUIWidget *widget);
    
    /// Get the event caller object.
    vmxGUIBaseObject* GetEventCallerObject()
    {
        return this->m_event_caller_object;
    };
    
    /// Get the event starting position.
    void GetEventPositionOfCallerObject(int &output_event_position1, int &output_event_position2)
    {
        output_event_position1 = this->m_event_caller_object_position_of_event[0];
        output_event_position2 = this->m_event_caller_object_position_of_event[1];
    };
    
    /// Get visibility of this object.
    virtual int IsVisible()
    {
        return m_is_visible_collection;
    };

    
    /// Callback method executed for the given event.
    virtual void OnKeyPress()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnKeyPress()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnMouseMove()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnMouseMove()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonUp()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnLeftButtonUp()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDown()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnLeftButtonDown()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDoubleClick()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnLeftButtonDoubleClick()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrag()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnLeftButtonDrag()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrop()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnLeftButtonDrop()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnMouseWheelForward()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnMouseWheelForward()!"<<endl;
    };
    
    /// Callback method executed for the given event.
    virtual void OnMouseWheelBackward()
    {
        cout<<"Object '"<<this->m_object_name<<"' of type "<<this->m_class_name<<" called vmxGUIWidgetCollection::OnMouseWheelBackward()!"<<endl;
    };

    /// Remove a widget from the collection.
    virtual int RemoveWidget(vmxGUIWidget *widget);

    /// Reset the object.
    virtual void Reset();
    
    /// Set the event caller object and event starting position.
    void SetEventCallerObjectAndEventPosition(vmxGUIBaseObject *caller_object, int event_position1, int event_position2)
    {
        this->m_event_caller_object = caller_object;
        this->m_event_caller_object_position_of_event[0] = event_position1;
        this->m_event_caller_object_position_of_event[1] = event_position2;
    };
    
    /// Set the pointer to the main widget.
    void SetMainWidget(vmxGUIMainWidget *main_widget);

    /// Set the order of object placement so that the object appears above existing object at the same placement.
    void SetPlacementOrderToAbove();
    
    /// Set the order of object placement so that the object appears below existing object at the same placement.
    void SetPlacementOrderToBelow();
    
//    /// Place the object relative to the size of the render window.
//    void SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent);
    
    /// Place the object in the cernter left side of the render window.
    void SetPlacementToCenterCenter();
    
    /// Place the object in the cernter left side of the render window.
    void SetPlacementToCenterLeft();
    
    /// Place the object in the cernter right side of the render window.
    void SetPlacementToCenterRight();
    
    /// Place the object in the lower center of the render window.
    void SetPlacementToLowerCenter();
    
    /// Place the object in the lower left corner of the render window.
    void SetPlacementToLowerLeft();
    
    /// Place the object in the lower right corner of the render window.
    void SetPlacementToLowerRight();
    
    /// Place the object in the upper center of the render window.
    void SetPlacementToUpperCenter();
    
    /// Place the object in the upper left corner of the render window.
    void SetPlacementToUpperLeft();
    
    /// Place the object in the upper right corner of the render window.
    void SetPlacementToUpperRight();
    
    /// Set visibility of the object.
    void SetVisibility(int is_visible);

};




//----------------------------------------------------------------------------------------------------------------------



class vmxGUIWidget_API vmxGUIRendererPosition
{
public:
    double m_x;
    double m_y;
    double m_z;
    double m_t;
    double m_v;
    
    vmxGUIRendererPosition(){};
    ~vmxGUIRendererPosition(){};
};



/// Base class for GUI renderers.

class vmxGUIWidget_API vmxGUIRenderer : public vmxGUIBaseObject
{
    
protected:
    
    /// Maximum number of picked positions in this renderer.
    static const int VMX_GUI_RENDERER_MAX_NUMBER_OF_PICKED_POSITIONS = 500;
    
    /// Array of picked positions.
    vmxGUIRendererPosition m_picked_poisitions[VMX_GUI_RENDERER_MAX_NUMBER_OF_PICKED_POSITIONS];
    
    /// Number of picked positions.
    int m_number_of_picked_positions;
        
    /// Font size of the text written on the renderer.
    int m_font_size;

    /// Indicates if the renderer is visible.
    int m_is_visible;
    
    /// VTK renderer.
    vtkSmartPointer<vtkRenderer> m_renderer;
    
    /// Indicates if this renderer captures the event that was detected. E.g. if it captures events, then the left_button_up event
    /// will not be forwarded to an object seen in the renderer scene, but will be used only by the renderer itself.
    int m_is_capturing_event;
    
//    /// Text actor for slice and time index info.
//    vtkSmartPointer<vtkTextActor> m_text_actor;
    
//    /// Text actor for seeds manipulation methods.
//    vtkSmartPointer<vtkTextActor> m_text_actor_seeds_clear;
//    vtkSmartPointer<vtkTextActor> m_text_actor_seeds_add;
//    vtkSmartPointer<vtkTextActor> m_text_actor_seeds_to_point_list;
//
//    /// Text actor for view reset.
//    vtkSmartPointer<vtkTextActor> m_text_actor_view_reset;
    
    /// List of text actors used for command in the renderer.
    mxList< vtkSmartPointer< vtkTextActor > > m_command_text_actors;
    
    /// X coordinate of the text actors for commands.
    int m_command_text_actor_coordinate_x;
    
    /// Separator value for the text actors of commands.
    int m_command_text_actor_separator;
    
    /// Color of the text actors of commands.
    double m_command_text_actor_color_RGB[3];
    
    
public:
    
    /// Constructor.
    vmxGUIRenderer();
    
    /// Destructor.
    virtual ~vmxGUIRenderer();
    
    /// Create new object instance.
    static vmxGUIRenderer* New();
    
//    /// Get relative coordinates of the renderer in the render window. Range of values [0,1].
//    void GetRelativeCoordinates(double &x, double &y) { x = m_relative_coordinates[0]; y = m_relative_coordinates[1]; };
//
//    /// Get relative size of the renderer in the render window. Range of values [0,1].
//    void GetRelativeSize(double &x, double &y) { x = m_relative_size[0]; y = m_relative_size[1]; };
    
    /// Add a new command text actor to the list of commands.
    vtkTextActor* AddCommand(const char *command_text);
    
    /// Add the picked position to the array of picked positions.
    virtual void AddPickPosition(double x, double y, double z, double v);
    
    /// Clear the array of picked positions.
    void ClearAllPickedPositions() { m_number_of_picked_positions = 0; };

    /// Clear the array of picked positions.
    void ClearLastPickedPosition();
    
    /// Get command text actor for the given command text. If none return NULL.
    vtkTextActor* GetCommand(const char *command_text);
    
    /// Get command text actor for the given screen coordinates. If none return NULL.
    vtkTextActor* GetCommand(int screen_coordinate_x, int screen_coordinate_y);
    
    /// Get the last picked position.
    int GetLastPickedPosition(double &x, double &y, double &z, double &v);
    
    /// Get number of picked positions
    int GetNumberOfPickedPositions() { return m_number_of_picked_positions; };
    
    /// Get picked position by index
    int GetPickedPosition(int index_of_the_picked_position, double &x, double &y, double &z, double &v);
    
    /// Get screen coordinates (start and end) of the renderer.
    void GetScreenCoordinates(int &x0, int &y0, int &x1, int &y1);
    
    /// Get VTK renderer.
    vtkRenderer* GetVTKRenderer();
    
    /// Get VTK interactor style for this renderer (if none specified, leave to return value NULL).
    virtual vtkInteractorStyle* GetVTKInteractorStyle() { return NULL;} ;
    
    /// Indicates if this renderer captures the event that was detected. E.g. if it captures events, then the left_button_up event
    /// will not be forwarded to an object seen in the renderer scene, but will be used only by the renderer itself.
    int IsCapturingEvents() { return m_is_capturing_event; };
    
    //    /// Re-implemented to override the existing keypress char functionality.
    //    virtual void OnChar() { };//do nothing here.
    
    /// Executed on mouse move event.
    /// Notice: If there is a interactor style specified (i.e. GetVTKInteractorStyle returns non-zero pointer) this method is NOT executed.
    virtual void OnMouseMove() {};
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelBackward() {};
    
    /// Executed on mouse move event.
    virtual void OnMouseWheelForward() {};
    
    /// Executed when left button is released.
    virtual void OnLeftButtonUp() {};
    
    /// Executed when left button is released. Re-implemented to detect double-click.
    virtual void OnLeftButtonDown() {};
    
    /// Executed when key is pressed.
    virtual void OnKeyPress() {};
    
  
//    /// Set relative coordinates of the renderer in the render window. Range of values [0,1].
//    void SetRelativeCoordinates(double x, double y) { m_relative_coordinates[0] = x; m_relative_coordinates[1] = y; };
//
//    /// Set relative size of the renderer in the render window. Range of values [0,1].
//    void SetRelativeSize(double x, double y) { m_relative_size[0] = x; m_relative_size[1] = y; };

    
};



//----------------------------------------------------------------------------------------------------------------------




/// Callback method class executed when render window is resized.

class vmxGUIWidget_API vmxGUIMainWidgetRenderWindowModifiedCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxGUIMainWidget *m_widget;
    
    int m_previous_window_size[2];
    
    /// Constructor.
    vmxGUIMainWidgetRenderWindowModifiedCallback();
    
    /// Destructor.
    ~vmxGUIMainWidgetRenderWindowModifiedCallback();
    
    /// Create new object instance.
    static vmxGUIMainWidgetRenderWindowModifiedCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};




//----------------------------------------------------------------------------------------------------------------------



/// Main widget of vtk GUI. Each widget is bound to this one.

class vmxGUIWidget_API vmxGUIMainWidget
{

//protected:
public:

    /// Class (type) unique name string.
    mxString m_class_name;
    
    /// Pointer to the containing render window. We keep a separate pointer to allow a possibility of use of external render window.
    vtkSDL2OpenGLRenderWindow *m_render_window;
    
    /// Available extent of x values, sequentially [x_min,x_max] for the left side of the main widget.
    int m_left_x_extent[2];

    /// Available extent of y values, sequentially [y_min,y_max] for the left side of the main widget.
    int m_left_y_extent[2];

    /// Available extent of x values, sequentially [x_min,x_max] for the center of the main widget.
    int m_center_x_extent[2];
    
    /// Available extent of y values, sequentially [y_min,y_max] for the center of the main widget.
    int m_center_y_extent[2];

    /// Available extent of x values, sequentially [x_min,x_max] for the right side of the main widget.
    int m_right_x_extent[2];
    
    /// Available extent of y values, sequentially [y_min,y_max] for the right side of the main widget.
    int m_right_y_extent[2];

    /// Spacing between objects.
    int m_spacing;
    
    /// Callback regulating the positioning of objects when the render window is resized.
    vtkSmartPointer<vmxGUIMainWidgetRenderWindowModifiedCallback> m_window_modified_callback;

    /// Renderer that will contain GUI objects.
    vtkSmartPointer<vtkRenderer> m_renderer_GUI;
    
    /// Pointer to the occupant of the GUI renderer (serves also as indicator - if NULL it means that the GUI renderer is not occupied).
    vmxGUIWidgetBase *m_renderer_GUI_occupant; //vmxGUIBaseObject *m_renderer_GUI_occupant;

//    /// Renderer that will contain 3D scene.
//    vmxGUIRenderer3DTrackBallCamera m_renderer_3D;
    
    /// List of user defined renderers.
    mxList< vmxGUIRenderer* > m_renderers_list;
    
    /// Render window that contains all the renderers and the interactor.
    vtkSmartPointer<vtkSDL2OpenGLRenderWindow> m_render_window_internal; //Maybe the render window should not be included here, but externally. To be determined.

    /// Render window interactor associated with the render window.
    vtkSmartPointer<vtkSDL2RenderWindowInteractor> m_interactor;
    
    /// Style assigned to the interactor.
    vtkSmartPointer<vmxGUIInteractorStyle> m_interactor_style;

    /// Clipboard owned by the main widget.
    vmxGUIClipBoard m_clipboard;
    
    /// Size of the main widget (internal render window). This is maintianed in order to be able to access the size value before the
    /// VTK render window is set up (in order to intialize other objects).
    int m_size_xy[2];
    
    
    /// List of contained objects for particular placement.
    //    mxList<vmxGUIWidget*> m_bottom_objects;
    //    mxList<vmxGUIWidget*> m_top_objects;
    
    mxList<vmxGUIWidget*> m_lower_left_objects;
    mxList<vmxGUIWidget*> m_lower_right_objects;
    mxList<vmxGUIWidget*> m_lower_center_objects;
    mxList<vmxGUIWidget*> m_upper_left_objects;
    mxList<vmxGUIWidget*> m_upper_right_objects;
    mxList<vmxGUIWidget*> m_upper_center_objects;
    
    // This is how the distribution of object placements (according to object lists) looks like:
    // ----t---
    // ul uc ur
    // ll lc lr
    // ----b---

    
public:
    
    /// List of widget objects that take the whole GUI screen.
    mxList<vmxGUIWidgetBase*> m_exclusive_visibility_objects;
    
    /// List of contained objects.
    mxList<vmxGUIWidgetBase*> m_objects; //mxList<vmxGUIBaseObject*> m_objects;

    
    
    /// Constructor.
    vmxGUIMainWidget();
    
    /// Destructor.
    virtual ~vmxGUIMainWidget();
    
    /// Add a new user defined renderer.
    int AddRenderer(vmxGUIRenderer *renderer); //vmxGUIRenderer* AddRenderer(const char *renderer_name = "Renderer");
    
    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForLeftSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);

    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForCenter(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);

    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForRightSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);
    
    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForLeftSide(int &x_size, int &y_size, int is_stretching_over_x_axis);
    
    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForCenter(int &x_size, int &y_size, int is_stretching_over_x_axis);

    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForRightSide(int &x_size, int &y_size, int is_stretching_over_x_axis);

    /// Get pointer to the clipboard.
    vmxGUIClipBoard* GetClipBoard() { return &m_clipboard; };
    
    /// Get pointer to the interactor.
    vtkRenderWindowInteractor* GetInteractor() { return m_interactor; };
    
    /// Get pointer to the interactor style.
    vmxGUIInteractorStyle* GetInteractorStyle() { return m_interactor_style; };

    /// Get pointer to the renderer of GUI.
    vtkRenderer* GetRenderer_GUI();// { return m_renderer_GUI; };
    
//    /// Get pointer to the 3D renderer.
//    vtkRenderer* GetRenderer_3D();

    /// Get pointer to the render window.
    vtkSDL2OpenGLRenderWindow* GetRenderWindow();
    
    /// Get the size of the render window. If no render window is attached return fail 0.
    int GetRenderWindowSize(int &x_size, int &y_size);
    
    /// Get the object that contains the given 2D vtk actor. If none found, return NULL.
    vmxGUIWidgetBase* GetGUIObjectForVTKActor2D(vtkActor2D *actor); //vmxGUIBaseObject* GetGUIObjectForVTKActor2D(vtkActor2D *actor);
    
    /// Get the object falling under the given (mouse click) screen coordinates. If none found, return NULL.
    vmxGUIWidgetBase* GetGUIObjectForScreenCoordinates(int pos1, int pos2); //vmxGUIBaseObject* GetGUIObjectForScreenCoordinates(int pos1, int pos2);
    
    /// Get the size of the main widget (the internal render window size).
    void GetSize(int &size_x, int &size_y);
    
    /// Based on existing placement preference, repositions the object.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Reset the object.
    virtual void Reset();
    
    /// Set the render window.
    void SetRenderWindow(vtkSDL2OpenGLRenderWindow *render_window);
    
    /// Set size of the main window (it is the internal render window size).
    void SetSize(int size_x, int size_y);
    
    /// Render and start the interaction.
    void StartInteraction()
    {
        this->GetRenderWindow()->Render();
        this->GetInteractor()->Start();
    };
};




#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
