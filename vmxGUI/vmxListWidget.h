/*=========================================================================
 
 Program:   mipx
 Module:    vmxListWidget.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxListWidget_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxListWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxListWidget_EXPORTS
            #define vmxListWidget_API __declspec(dllexport)
        #else
            #define vmxListWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxListWidget_EXPORTS
            #define vmxListWidget_API __attribute__((visibility("default")))
        #else
            #define vmxListWidget_API
        #endif
    #endif
#endif



#ifndef vmxListWidget_H
	#define vmxListWidget_H



//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)



#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"
#include "vmxClipBoard.h"
#include "vmxEventDetector.h"
#include "vmxGUIObject.h"


#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>




// Pre-declaration.
class vmxListWidget;



//class vmxListWidget_API vmxListWidgetRenderWindowModifiedCallback : public vtkCommand
//{
//    /// Indicates if the callback is on or off.
//    int m_is_active;
//    
//public:
//    
//    /// Pointer to list_widget that uses this callback.
//    vmxListWidget *m_list_widget;
//    
//    int m_previous_window_size[2];
//    
//    /// Constructor.
//    vmxListWidgetRenderWindowModifiedCallback();
//    
//    /// Destructor.
//    ~vmxListWidgetRenderWindowModifiedCallback();
//    
//    /// Initialize the list_widget linked to this callback
//    void SetListWidget(vmxListWidget *list_widget);
//    
//    /// Create new object instance.
//    static vmxListWidgetRenderWindowModifiedCallback* New();
//    
//    /// Turn callback on or off.
//    void Activate(int is_active);
//    
//    /// Method that executes when the callback is called.
//    virtual void Execute(vtkObject *caller, unsigned long, void *);
//};



class vmxListWidget_API vmxListWidgetInteractorLeftButtonDownCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to list_widget that uses this callback.
    vmxListWidget *m_list_widget;
    
    /// Constructor.
    vmxListWidgetInteractorLeftButtonDownCallback();
    
    /// Destructor.
    ~vmxListWidgetInteractorLeftButtonDownCallback();
    
    /// Initialize the list_widget linked to this callback
    void SetListWidget(vmxListWidget *list_widget);
    
    /// Create new object instance.
    static vmxListWidgetInteractorLeftButtonDownCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxListWidget_API vmxListWidgetInteractorLeftButtonUpCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to list_widget that uses this callback.
    vmxListWidget *m_list_widget;
    
    /// Constructor.
    vmxListWidgetInteractorLeftButtonUpCallback();
    
    /// Destructor.
    ~vmxListWidgetInteractorLeftButtonUpCallback();
    
    /// Initialize the list_widget linked to this callback
    void SetListWidget(vmxListWidget *list_widget);
    
    /// Create new object instance.
    static vmxListWidgetInteractorLeftButtonUpCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxListWidget_API vmxListWidgetInteractorMouseMoveCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to list_widget that uses this callback.
    vmxListWidget *m_list_widget;
    
    /// Constructor.
    vmxListWidgetInteractorMouseMoveCallback();
    
    /// Destructor.
    ~vmxListWidgetInteractorMouseMoveCallback();
    
    /// Initialize the list_widget linked to this callback
    void SetListWidget(vmxListWidget *list_widget);
        
    /// Create new object instance.
    static vmxListWidgetInteractorMouseMoveCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};


class vmxListWidget_API vmxListWidgetInteractorMouseWheelBackwardCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to list_widget that uses this callback.
    vmxListWidget *m_list_widget;
    
    /// Constructor.
    vmxListWidgetInteractorMouseWheelBackwardCallback();
    
    /// Destructor.
    ~vmxListWidgetInteractorMouseWheelBackwardCallback();
    
    /// Initialize the list_widget linked to this callback
    void SetListWidget(vmxListWidget *list_widget);
    
    /// Create new object instance.
    static vmxListWidgetInteractorMouseWheelBackwardCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};


class vmxListWidget_API vmxListWidgetInteractorMouseWheelForwardCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to list_widget that uses this callback.
    vmxListWidget *m_list_widget;
    
    /// Constructor.
    vmxListWidgetInteractorMouseWheelForwardCallback();
    
    /// Destructor.
    ~vmxListWidgetInteractorMouseWheelForwardCallback();
    
    /// Initialize the list_widget linked to this callback
    void SetListWidget(vmxListWidget *list_widget);
    
    /// Create new object instance.
    static vmxListWidgetInteractorMouseWheelForwardCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};




class vmxListWidgetSlot
{
public:
    virtual int Execute(vmxListWidget *list_widget) {cout<<" reimplement this "; return 0;};
    virtual ~vmxListWidgetSlot() {};
};



class vmxListWidget_API vmxListWidgetItem
{
    
public:
    
    /// The item can be made to controll/contain other items, this indicator shows if that is the case.
    /// By default all items are controllers, i.e. they are all on level 0.
    int m_is_controller;
    
    /// Pointer to the list_widget slot method that is executed when this item is clicked.
    vmxListWidgetSlot *m_slot;
    
    /// Pointer to the list_widget this item belongs to.
    vmxListWidget *m_list_widget;
    
    /// Pointer to sublist_widget called from this item. If none pointer is NULL.
    vmxListWidget *m_sub_list_widget;
    
    /// Indicators for checked check boxes. Preset size, member m_number_of_check_boxes controlls how many check boxes there really are.
    int m_checkboxes[10];
    
    /// Number of check boxes for this item.
    unsigned int m_number_of_check_boxes;
    
    /// Indicates if this item has been selected.
    int m_is_selected;
    
    /// Text of the item.
    mxString m_text;
    
    /// Constructor.
    vmxListWidgetItem();
    
    /// Destructor.
    ~vmxListWidgetItem();
    
    /// Attach a slot to this item. Takes possession of the slot object and will release it on destruction.
    /// Should be called as: item->AttachSlot(new MySlot);
    void AttachSlot(vmxListWidgetSlot *slot);
    
    /// Get the slot attached to this item.
    vmxListWidgetSlot* GetSlot();
    
    /// Get the sublist_widget attached to this item.
    vmxListWidget* GetSubListWidget();

};



class vmxListWidget_API vmxListWidget : public vmxGUIObject
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxClipBoard *m_clip_board;
    
    /// ListWidget that contains this list_widget (in case this list_widget is a sub-list_widget). If this list_widget is a root list_widget
    /// the pointer will contain 'this' address (points to itself).
    vmxListWidget *m_list_widget;
    
    /// List of items in the widget.
    mxList<vmxListWidgetItem> m_items;
    
    /// List of pointers to items in the list of items. This array is built when calling BuildListWidget() and is used for (fast)
    /// indexing access  to the items.
    mxArray<vmxListWidgetItem*> m_item_pointers;
    
    /// List of pointers to controllers in the list of items. This array is built when calling BuildListWidget() and is used for (fast)
    /// indexing access  to the items.
    mxArray<vmxListWidgetItem*> m_controller_pointers;
    
    /// Maintin the maximum number of checkboxes contained in an item - this is needed for checking/picking check boxes.
    /// This value gets updated when adding and removing items.
    unsigned int m_max_number_of_check_boxes;
    
    /// Maximum nuber of items that will be stored in the text actor. This will be determined from m_maximum_size (remark: lower numbers will improve efficiency).
    int m_text_actor_max_number_of_items;
    
    /// Actual number of items contained in the text actor.
    /// Notice: this number can be different than the total number of items in the widget.
    int m_number_of_items_in_text_actor;
   
    /// Detects if double click was performed.
    vmxDoubleClickDetector m_double_click_detector;
    
    /// Detects if mouse draggung was performed.
    vmxMouseDragDetector m_mouse_drag_detector;
    
    /// Detects if items were selected.
    vmxItemSelectionDetector m_item_selection_detector;
    
    /// Index of the picked item. If none, the value will be -1.
    int m_picked_item_index;
    
    /// Index of the picked checkbox. If none, the value will be -1.
    int m_picked_check_box_index;

    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    int m_text_index_offset;
    
    /// Actor containing the list_widget item text.
    vtkSmartPointer<vtkTextActor> m_text_actor;
    
    /// Actor marking selected items of the list_widget.
    vtkSmartPointer<vtkTextActor> m_selected_text_actor;
    
    /// Actor containing the check boxes (as text) of the list_widget.
    vtkSmartPointer<vtkTextActor> m_checkboxes_actor;
    
    /// Actor shown to indicate drag event (drag item from list_widget).
    vtkSmartPointer<vtkTextActor> m_drag_actor;
    
    /// Callback executed when the left button is pressed.
    vtkSmartPointer<vmxListWidgetInteractorLeftButtonDownCallback> m_left_button_down_callback;
    
    /// Callback executed when the left button is released.
    vtkSmartPointer<vmxListWidgetInteractorLeftButtonUpCallback> m_left_button_up_callback;

    /// Callback executed when the mouse is moved.
    vtkSmartPointer<vmxListWidgetInteractorMouseMoveCallback> m_mouse_move_callback;
    
    /// Callback executed when the mouse wheel backward is detected.
    vtkSmartPointer<vmxListWidgetInteractorMouseWheelBackwardCallback> m_mouse_wheel_backward_callback;
    
    /// Callback executed when the mouse wheel forward is detected.
    vtkSmartPointer<vmxListWidgetInteractorMouseWheelForwardCallback> m_mouse_wheel_forward_callback;

    
    /// This is the character used to mark items as selected (by default it is underscore '_').
    char m_selection_text_character;
    
    /// Text line used to mark a single line of text as selected.
    mxString m_selection_text_line;
    
    
    
    /// Constructor.
    vmxListWidget();
    
    /// Destructor.
    virtual ~vmxListWidget();
    
    /// Create new object instance.
    static vmxListWidget* New();
    
    /// Add a new item to the item list.
    vmxListWidgetItem* AddItem(const char *item_text, unsigned int number_of_check_boxes=0);
    
    /// Add a new item to the list under a given controller.
    vmxListWidgetItem* AddItem(const char *item_text, vmxListWidgetItem *controller, unsigned int number_of_check_boxes=0);

    /// Update the VTK actors of the list_widget based on the changes of (added/removed) items.
    void BuildListWidget();
    
    /// Compute the max number of items in text actor based on available space in main widget.
    int ComputeMaxNumberOfItemsInTextActor();
    
    /// Based on the size of text actor, this will compute the selection text line to be used
    /// to mark items as selected. Use after interactor and font have been set.
    int ComputeSelectionTextLine();
    
    /// Copy data of selected items to clipboard.
    int CopySelectedItemsToClipBoard();
    
    /// Get color of the list_widget text.
    void GetColor(double &r, double &g, double &b);
    
    /// Get item by index.
    vmxListWidgetItem* GetItem(int item_index) { if(item_index<0 || item_index>=this->GetNumberOfItems()) return NULL; return m_item_pointers[item_index]; };
    
    /// Get number of items in the list_widget.
    int GetNumberOfItems() { return m_items.GetNumberOfElements(); };
    
    /// Get number of items in the list_widget.
    int GetMaxNumberOfCheckBoxes() { return m_max_number_of_check_boxes; };

    /// Get origin (position) of the whole list_widget.
    void GetOrigin(int &origin1, int &origin2);

    /// Get origin (position) of the text actor.
    int GetOriginOfTextActor(int &origin1, int &origin2);

    /// Get origin (position) of the check box actor.
    int GetOriginOfCheckBoxActor(int &origin1, int &origin2);
    
    /// Get index of picked check box (for existing item index - call GetPickedItemIndex() before this method). If none, return -1 (invalid index).
    int GetPickedCheckBoxIndex(int picked_item_index, int pos1, int pos2);
    
    /// Get index of picked item. If none, return -1 (invalid index).
    int GetPickedItemIndex(int pos1, int pos2);
    
    /// Get the size of the list_widget check box actor.
    int GetSizeOfCheckBoxActor(int &output_size1, int &output_size2);
    
    /// Get the size of the whole list_widget (all actors together).
    void GetSize(int &output_size1, int &output_size2);
    
    /// Get the size of the list_widget text actor.
    int GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Get visibility of the list_widget (check if the list_widget is visible).
    int GetVisibility();
    
    /// Given the input positions, check if the check box actor is picked (if the position falls within the check box actor).
    int IsCheckBoxActorPicked(int pos1, int pos2);
    
    /// Given the input positions, check if the list_widget is picked (if the position falls within the list_widget).
    int IsListWidgetPicked(int pos1, int pos2);
    
    /// Check if there are multiple items selected (more than one item must be selected).
    int IsMultipleItemsSelected();
    
    /// Given the input positions, check if the text actor is picked (if the position falls within the text actor).
    int IsTextActorPicked(int pos1, int pos2);
    
    /// Get visibility of this object.
    int IsVisible();
    
//    /// Turn on/off listening for render window modified event.
//    void ListenForRenderWindowModifiedEvent(int is_listening);
    
    /// Turn on/off listening for left button down event.
    void ListenForLeftButtonDownEvent(int is_listening);

    /// Turn on/off listening for mouse move event.
    void ListenForMouseMoveEvent(int is_listening);
    
    /// Turn on/off listening for left button up event.
    void ListenForLeftButtonUpEvent(int is_listening);
    
    /// Turn on/off listening for mouse wheel event.
    void ListenForMouseWheelBackwardEvent(int is_listening);
    
    /// Turn on/off listening for mouse wheel event.
    void ListenForMouseWheelForwardEvent(int is_listening);
    
    /// Method being executed when an item is picked.
    virtual void OnItemPicked() { cout<<endl<<"picked_item_index="<<m_picked_item_index<<endl<<"picked_check_box_index="<<m_picked_check_box_index<<endl;};
    
    /// Set selection indicator of item with given index to 1, all others will be 0 (not selected).
    void SelectSingleItem(unsigned int item_index);
    
    /// Set size of font in the list_widget.
    void SetFontSize(double font_size);
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxClipBoard *clip_board) { m_clip_board = clip_board; };
    
    /// Set color of text in the list_widget.
    void SetColor(double r, double g, double b);
    
    /// Set offset of item indexes. Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    void SetIndexOffset(int offset);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set maximum size of the object. Re-implemented from vmxGUIObject.
    void SetMaximumSize(int max_size_x, int max_size_y);
 
    /// Set the origin (position) of the list_widget.
    void SetOrigin(int origin1, int origin2);
    
    /// Set visibility of the list_widget.
    void SetVisibility(int is_visible);
    
    /// Set visibility of the list_widget.
    void SetVisibilityOfDragEventActor(int is_visible);
    
    /// Shows selected items. Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    int ShowSelectedItems();
    
    /// Shows text actor items. Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    int ShowTextItems();

};





#endif
