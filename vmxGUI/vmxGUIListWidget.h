/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIListWidget.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxGUIListWidget_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIListWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIListWidget_EXPORTS
            #define vmxGUIListWidget_API __declspec(dllexport)
        #else
            #define vmxGUIListWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIListWidget_EXPORTS
            #define vmxGUIListWidget_API __attribute__((visibility("default")))
        #else
            #define vmxGUIListWidget_API
        #endif
    #endif
#endif



#ifndef vmxGUIListWidget_H
#define vmxGUIListWidget_H



#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"
#include "vmxGUIClipBoard.h"
#include "vmxGUIEventDetector.h"
#include "vmxGUIWidget.h"


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
class vmxGUIListWidget;
class vmxGUIListWidgetItem;




class vmxGUIListWidgetMouseLeftClickSlot
{
public:
    virtual void Execute(vmxGUIListWidgetItem *list_widget, int picked_checkbox_index) { /* Reimplement this. */ };
    virtual ~vmxGUIListWidgetMouseLeftClickSlot() {};
};


/// Item class of a list widget.

class vmxGUIListWidget_API vmxGUIListWidgetItem
{
    
public:
    
    /// Define the max number of check boxes allowed for the list widget.
#define vmxGUIListWidgetItem_max_number_of_checkboxes 10
    
    /// The item can be made to controll/contain other items, this indicator shows if that is the case.
    /// By default all items are controllers, i.e. they are all on level 0.
    int m_is_controller;
    
    /// Pointer to the list_widget slot method that is executed when this item is left clicked.
    vmxGUIListWidgetMouseLeftClickSlot *m_mouse_left_click_slot;
    
    /// Pointer to the list_widget this item belongs to.
    vmxGUIListWidget *m_list_widget;
    
    /// Pointer to sublist_widget called from this item. If none pointer is NULL.
    vmxGUIListWidget *m_sub_list_widget;
    
    /// Indicators for checked check boxes. Preset size, member m_number_of_check_boxes controlls how many check boxes there really are.
    int m_checkboxes[vmxGUIListWidgetItem_max_number_of_checkboxes];
    
    /// Number of check boxes for this item.
    unsigned int m_number_of_check_boxes;
    
    /// Indicates if this item has been selected.
    int m_is_selected;
    
    /// Indicate if only a single item (under the same controller) can be checked. Default is 0 (no).
    int m_is_exclusively_checked;
    
    /// Text of the item.
    mxString m_text;
    
    /// Pointer to external data attached to this item. The type of the data is regulated from external side.
    void *m_data;
    
    /// Constructor.
    vmxGUIListWidgetItem();
    
    /// Destructor.
    ~vmxGUIListWidgetItem();
    
    /// Attach a slot to this item. Takes possession of the slot object and will release it on destruction.
    /// Should be called as: item->AttachSlot(new MySlot);
    void AttachSlot(vmxGUIListWidgetMouseLeftClickSlot *slot);
    
    /// Return pointer to attached external data.
    void* GetData()
    {
        return m_data;
    };
    
    /// Get the slot attached to this item.
    vmxGUIListWidgetMouseLeftClickSlot* GetMouseLeftClickSlot();
    
    /// Indicates if the checkbox with given index is checked.
    int IsChecked(int checkbox_index);
    
    /// Check if the item is a controller.
    int IsController() { return m_is_controller; };
    
    /// Indicates if the item is selected.
    int IsSelected()
    {
        return m_is_selected;
    };
    
    /// Set the pointer to external data.
    void SetData(void *data)
    {
        m_data = data;
    };
    
    /// Get the sublist_widget attached to this item.
    vmxGUIListWidget* GetSubListWidget();

//    /// Set the actual number of checkboxes for this item.
//    void SetNumberOfCheckBoxes(unsigned int number_of_check_boxes);
};



/// VTK GUI list widget.

class vmxGUIListWidget_API vmxGUIListWidget : public vmxGUIWidget
{
    
public:
    
    /// List of items in the widget.
    mxList<vmxGUIListWidgetItem> m_items;
    
    /// List of pointers to items in the list of items. This array is built when calling BuildListWidget() and is used for (fast)
    /// indexing access  to the items.
    mxArray<vmxGUIListWidgetItem*> m_item_pointers;
    
    /// List of pointers to controllers in the list of items. This array is built when calling BuildListWidget() and is used for (fast)
    /// indexing access  to the items.
    mxArray<vmxGUIListWidgetItem*> m_controller_pointers;
    
    /// Maintains the maximum number of checkboxes contained in an item - this is needed for checking/picking check boxes.
    /// This value gets updated when adding and removing items.
    unsigned int m_max_number_of_check_boxes;
    
    /// Maximum nuber of items that will be stored in the text actor. This will be determined from m_maximum_size (remark: lower numbers will improve efficiency).
    int m_text_actor_max_number_of_items;
    
    /// Actual number of items contained in the text actor.
    /// Notice: this number can be different than the total number of items in the widget.
    int m_number_of_items_in_text_actor;
   
//    /// Detects if double click was performed.
//    vmxDoubleClickDetector m_double_click_detector;
    
//    /// Detects if mouse draggung was performed.
//    vmxMouseDragDetector m_mouse_drag_detector;
    
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
    
    /// This is the character used to mark items as selected (by default it is underscore '_').
    char m_selection_text_character;
    
    /// Text line used to mark a single line of text as selected.
    mxString m_selection_text_line;
    
    /// Pointer to the item that was selected last.
    vmxGUIListWidgetItem *m_last_selected_item;
    
    /// Pointer to the data of the last selected item.
    void *m_last_selected_item_data;
    
    
    
    /// Constructor.
    vmxGUIListWidget();
    
    /// Destructor.
    virtual ~vmxGUIListWidget();
    
    /// Create new object instance.
    static vmxGUIListWidget* New();
    
    /// Add a new item to the item list.
    vmxGUIListWidgetItem* AddItem(const char *item_text, unsigned int number_of_check_boxes=0);
    
    /// Add a new item to the list under a given controller.
    vmxGUIListWidgetItem* AddItem(const char *item_text, vmxGUIListWidgetItem *controller, unsigned int number_of_check_boxes=0);

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
    vmxGUIListWidgetItem* GetItem(int item_index)
    {
        if(item_index<0 || item_index>=this->GetNumberOfItems()) return NULL;
        return m_item_pointers[item_index];
    };
    
    /// Get items as an array of item pointers.
    mxArray<vmxGUIListWidgetItem*>* GetItems()
    {
        return &m_item_pointers;
    };
    
    /// Get the last selected item.
    vmxGUIListWidgetItem** GetLastSelectedItem()
    {
        return &m_last_selected_item;
    };
    
    /// Get the data associated with the last selected item.
    void** GetLastSelectedItemData()
    {
        return &m_last_selected_item_data;
    };
    
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
    
    /// Get array of selected items.
    void GetSelectedItems(mxArray<vmxGUIListWidgetItem*> &array_of_selected_items);
    
    /// Get array of names of selected items.
    void GetSelectedItemsNames(mxArray<mxString> &array_of_selected_items_names);
    
    /// Get the size of the list_widget check box actor.
    int GetSizeOfCheckBoxActor(int &output_size1, int &output_size2);
    
    /// Get the size of the whole list_widget (all actors together).
    void GetSize(int &output_size1, int &output_size2);
    
    /// Get the size of the list_widget text actor.
    int GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Get visibility of the list_widget (check if the list_widget is visible).
    int GetVisibility();
    
    /// Check if the given 2D VTK actor is contained in this object.
    int HasVTKActor2D(vtkActor2D *actor_2D);
    
    /// Given the input positions, check if the check box actor is picked (if the position falls within the check box actor).
    int IsCheckBoxActorPicked(int pos1, int pos2);
    
    /// Given the input positions, check if the list_widget is picked (if the position falls within the list_widget).
    int IsListWidgetPicked(int pos1, int pos2);
    
    /// Check if there are multiple items selected (more than one item must be selected).
    int IsMultipleItemsSelected();
    
    /// Indicate if the object was picked by checking if the screen pick coordinates fall inside the object.
    int IsPicked(int pos1, int pos2) { return this->IsListWidgetPicked(pos1,pos2); };
    
    /// Given the input positions, check if the text actor is picked (if the position falls within the text actor).
    int IsTextActorPicked(int pos1, int pos2);
    
    /// Get visibility of this object.
    int IsVisible();
        
    /// Method being executed when an item is picked.
    virtual void OnItemPicked()
    {
        //cout<<endl<<"picked_item_index="<<m_picked_item_index<<endl<<"picked_check_box_index="<<m_picked_check_box_index<<endl;
    };
    
    /// Callback method executed the given event.
    virtual void OnKeyPress();
    
    /// Callback method executed the given event.
    virtual void OnMouseMove();
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonUp();
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonDown();
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDoubleClick();
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrag();
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrop();
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelForward();
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelBackward();
    
    /// Reset the object.
    void Reset();
    
    /// Set selection indicator of item with given index to 1, all others will be 0 (not selected).
    void SelectSingleItem(unsigned int item_index);
    
    /// Set size of font in the list_widget.
    void SetFontSize(double font_size);
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxGUIClipBoard *clip_board) { m_clip_board = clip_board; };
    
    /// Set color of text in the list_widget.
    void SetColor(double r, double g, double b);
    
    /// Set offset of item indexes. Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    void SetIndexOffset(int offset);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set the main widget and connect visualization objects contained within it.
    void SetMainWidget(vmxGUIMainWidget *main_widget);
    
    /// Set maximum size of the object. Re-implemented from vmxGUIWidget.
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
