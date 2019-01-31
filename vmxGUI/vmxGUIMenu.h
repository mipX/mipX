/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIMenu.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxGUIMenu_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIMenu_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIMenu_EXPORTS
            #define vmxGUIMenu_API __declspec(dllexport)
        #else
            #define vmxGUIMenu_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIMenu_EXPORTS
            #define vmxGUIMenu_API __attribute__((visibility("default")))
        #else
            #define vmxGUIMenu_API
        #endif
    #endif
#endif



#ifndef vmxGUIMenu_H
	#define vmxGUIMenu_H





#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"
#include "vmxGUIClipBoard.h"
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
class vmxGUIMenu;



/// Item of a menu widget.

class vmxGUIMenu_API vmxGUIMenuItem
{
    
public:
    
    /// Pointer to the menu slot method that is executed when this item is clicked.
    vmxGUISlotFunction m_slot; //vmxGUIMenuSlot *m_slot;
    
    /// Pointer to the menu this item belongs to.
    vmxGUIMenu *m_menu;
    
    /// Pointer to submenu called from this item. If none pointer is NULL.
    vmxGUIMenu *m_sub_menu;
    
    /// Indicators for checked check boxes. Preset size, member m_number_of_check_boxes controlls how many check boxes there really are.
    int m_checkboxes[10];
    
    /// Number of check boxes for this item.
    unsigned int m_number_of_check_boxes;
    
    /// Indicates if this item has been selected.
    int m_is_selected;
    
    /// Text of the item.
    mxString m_text;
    
    /// Constructor.
    vmxGUIMenuItem();
    
    /// Destructor.
    ~vmxGUIMenuItem();
    
    /// Attach a slot to this item. Takes possession of the slot object and will release it on destruction.
    /// Should be called as: item->AttachSlot(new MySlot);
    void AttachSlot(vmxGUISlotFunction slot);
    
    /// Get the slot attached to this item.
    vmxGUISlotFunction GetSlot();
    
    /// Get the submenu attached to this item.
    vmxGUIMenu* GetSubMenu();

};


/// VTK GUI menu widget.

class vmxGUIMenu_API vmxGUIMenu : public vmxGUIWidget
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxGUIClipBoard *m_clip_board;
    
    /// Menu that contains this menu (in case this menu is a sub-menu). If this menu is a root menu
    /// the pointer will contain 'this' address (points to itself).
    vmxGUIMenu *m_menu;
    
    /// List of items in the menu.
    mxList<vmxGUIMenuItem> m_items;
    
    /// List of pointers to items in the list of items. This array is built when calling BuildMenu() and is used for (fast)
    /// indexing access  to the items.
    mxArray<vmxGUIMenuItem*> m_item_pointers;
    
    /// Maintin the maximum number of checkboxes contained in an item - this is needed for checking/picking check boxes.
    /// This value gets updated when adding and removing items.
    unsigned int m_max_number_of_check_boxes;
    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Actor containing the menu item text.
    vtkSmartPointer<vtkTextActor> m_text_actor;
    
    /// Actor marking selected items of the menu.
    vtkSmartPointer<vtkTextActor> m_selected_text_actor;
    
    /// Actor containing the check boxes (as text) of the menu.
    vtkSmartPointer<vtkTextActor> m_checkboxes_actor;
    
//    /// Actor shown to indicate drag event (drag item from menu).
//    vtkSmartPointer<vtkTextActor> m_drag_actor;
    
//    /// Callback executed when the left button is pressed.
//    vtkSmartPointer<vmxGUIMenuInteractorLeftButtonDownCallback> m_left_button_down_callback;
//    
//    /// Callback executed when the left button is released.
//    vtkSmartPointer<vmxGUIMenuInteractorLeftButtonUpCallback> m_left_button_up_callback;
    
    /// This is the character used to mark items as selected (by default it is underscore '_').
    char m_selection_text_character;
    
    /// Text line used to mark a single line of text as selected.
    mxString m_selection_text_line;
    
    
   
    
    /// Constructor.
    vmxGUIMenu();
    
    /// Destructor.
    virtual ~vmxGUIMenu();
    
    /// Create new object instance.
    static vmxGUIMenu* New();
    
    /// Add a new item to the item list.
    vmxGUIMenuItem* AddItem(const char *item_text, unsigned int number_of_check_boxes=0);
    
    /// Add a submenu to this menu. Returns the item to which the submenu belongs.
    /// Access the submenu through item GetSubMenu() method.
    vmxGUIMenuItem* AddSubMenu(const char *item_text);
    
    /// Update the VTK actors of the menu based on the changes of (added/removed) items.
    void BuildMenu();
    
    /// Based on the size of text actor, this will compute the selection text line to be used
    /// to mark items as selected. Use after interactor and font have been set.
    int ComputeSelectionTextLine();
    
    /// Copy data of selected items to clipboard.
    int CopySelectedItemsToClipBoard();
    
    /// Get color of the menu text.
    void GetColor(double &r, double &g, double &b);
    
    /// Get item by index.
    vmxGUIMenuItem* GetItem(int item_index) { if(item_index<0 || item_index>=this->GetNumberOfItems()) return NULL; return m_item_pointers[item_index]; };
    
    /// Get number of items in the menu.
    int GetNumberOfItems() { return m_items.GetNumberOfElements(); };
    
    /// Get number of items in the menu.
    int GetMaxNumberOfCheckBoxes() { return m_max_number_of_check_boxes; };

    /// Get origin (position) of the whole menu.
    void GetOrigin(int &origin1, int &origin2);

    /// Get origin (position) of the text actor.
    int GetOriginOfTextActor(int &origin1, int &origin2);

    /// Get origin (position) of the check box actor.
    int GetOriginOfCheckBoxActor(int &origin1, int &origin2);
    
    /// Get index of picked check box (for existing item index - call GetPickedItemIndex() before this method). If none, return -1 (invalid index).
    int GetPickedCheckBoxIndex(int picked_item_index, int pos1, int pos2);
    
    /// Get index of picked item. If none, return -1 (invalid index).
    int GetPickedItemIndex(int pos1, int pos2);
    
    /// Get pointer to the picked submenu. If none return NULL.
    vmxGUIMenu* GetPickedSubMenu(int pos1, int pos2);
    
    /// Get the size of the menu check box actor.
    int GetSizeOfCheckBoxActor(int &output_size1, int &output_size2);
    
    /// Get the size of the whole menu (all actors together).
    void GetSize(int &output_size1, int &output_size2);
        
    /// Get the size of the menu text actor.
    int GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Get visibility of the menu (check if the menu is visible).
    int GetVisibility();
    
    /// Sets visibility of all submenus to 0 (none of the submenus will be visible) .
    void HideAllSubMenus();

    /// Given the input positions, check if the check box actor is picked (if the position falls within the check box actor).
    int IsCheckBoxActorPicked(int pos1, int pos2);
    
    /// Given the input positions, check if the menu is picked (if the position falls within the menu).
    int IsMenuPicked(int pos1, int pos2);
    
    /// Given the input positions, check if a visible submenu is picked (if the position falls within the menu).
    int IsSubMenuPicked(int pos1, int pos2);
    
    /// Check if there are multiple items selected (more than one item must be selected).
    int IsMultipleItemsSelected();
    
    /// Given the input positions, check if the text actor is picked (if the position falls within the text actor).
    int IsTextActorPicked(int pos1, int pos2);
    
    /// Check if the obejct is visible.
    int IsVisible();
    
//    /// Turn on/off listening for left button up event.
//    void ListenForLeftButtonUpEvent(int is_listening);
//    
//    /// Turn on/off listening for left button down event.
//    void ListenForLeftButtonDownEvent(int is_listening);

//    /// Turn on/off listening for mouse move event.
//    void ListenForMouseMoveEvent(int is_listening);
    
    /// Callback method executed the given event.
    void OnKeyPress() {};
    
    /// Callback method executed the given event.
    void OnMouseMove();
    
    /// Callback method executed the given event.
    void OnLeftButtonUp();
    
    /// Callback method executed the given event.
    void OnLeftButtonDown();
    
    /// Callback method executed the given event.
    void OnMouseWheelForward() {};
    
    /// Callback method executed the given event.
    void OnMouseWheelBackward() {};
    
    /// Set selection indicator of item with given index to 1, all others will be 0 (not selected).
    void SelectSingleItem(unsigned int item_index);
    
    /// Set size of font in the menu.
    void SetFontSize(double font_size);
        
    /// Set pointer to clipboard.
    void SetClipBoard(vmxGUIClipBoard *clip_board) { m_clip_board = clip_board; };
    
    /// Set color of text in the menu.
    void SetColor(double r, double g, double b);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set the main widget and connect visualization objects contained within it.
    void SetMainWidget(vmxGUIMainWidget *main_widget);
 
    /// Set the origin (position) of the menu.
    void SetOrigin(int origin1, int origin2);
        
    /// Set visibility of the menu.
    void SetVisibility(int is_visible);
    
//    /// Set visibility of the menu.
//    void SetVisibilityOfDragEventActor(int is_visible);
    
    /// Shows selected items.
    int ShowSelectedItems();
    
    /// Shows text actor items. Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    int ShowTextItems();

};





#endif
