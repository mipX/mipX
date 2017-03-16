/*=========================================================================
 
 Program:   mipx
 Module:    vmxMenu.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxMenu_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxMenu_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxMenu_EXPORTS
            #define vmxMenu_API __declspec(dllexport)
        #else
            #define vmxMenu_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxMenu_EXPORTS
            #define vmxMenu_API __attribute__((visibility("default")))
        #else
            #define vmxMenu_API
        #endif
    #endif
#endif



#ifndef vmxMenu_H
	#define vmxMenu_H



#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)



#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"
#include "vmxClipBoard.h"


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
class vmxMenu;


class vmxMenu_API vmxMenuRenderWindowModifiedCallback : public vtkCommand
{
    
public:
    
    /// Pointer to menu that uses this callback.
    vmxMenu *m_menu;
    
    int m_previous_window_size[2];
    
    /// Constructor.
    vmxMenuRenderWindowModifiedCallback();
    
    /// Destructor.
    ~vmxMenuRenderWindowModifiedCallback();
    
    /// Initialize the menu linked to this callback
    void SetMenu(vmxMenu *menu);
    
    /// Create new object instance.
    static vmxMenuRenderWindowModifiedCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxMenu_API vmxMenuInteractorLeftButtonDownCallback : public vtkCommand
{
    
public:
    
    /// Position of the last left button down event.
    int m_previous_left_button_down_position[2];
    
    /// Pointer to menu that uses this callback.
    vmxMenu *m_menu;
    
    /// Constructor.
    vmxMenuInteractorLeftButtonDownCallback();
    
    /// Destructor.
    ~vmxMenuInteractorLeftButtonDownCallback();
    
    /// Initialize the menu linked to this callback
    void SetMenu(vmxMenu *menu);
    
    /// Create new object instance.
    static vmxMenuInteractorLeftButtonDownCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};


class vmxMenu_API vmxMenuInteractorLeftButtonUpCallback : public vtkCommand
{
    
public:
    
    /// Position of the last left button down event.
    int m_previous_left_button_down_position[2];
    
    /// Indicates if an item should be selected on button up.
    int m_is_selecting;
    
    /// Pointer to menu that uses this callback.
    vmxMenu *m_menu;
    
    /// Constructor.
    vmxMenuInteractorLeftButtonUpCallback();
    
    /// Destructor.
    ~vmxMenuInteractorLeftButtonUpCallback();
    
    /// Initialize the menu linked to this callback
    void SetMenu(vmxMenu *menu);
    
    /// Create new object instance.
    static vmxMenuInteractorLeftButtonUpCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};


class vmxMenu_API vmxMenuInteractorMouseMoveCallback : public vtkCommand
{
    
public:
    
    /// Position of the last left button down event.
    int m_previous_left_button_down_position[2];
    
    /// Pointer to menu that uses this callback.
    vmxMenu *m_menu;
    
    /// Constructor.
    vmxMenuInteractorMouseMoveCallback();
    
    /// Destructor.
    ~vmxMenuInteractorMouseMoveCallback();
    
    /// Initialize the menu linked to this callback
    void SetMenu(vmxMenu *menu);
    
    /// Set the position of the last left button down event.
    void SetPreviousLeftButtonDownPosition(int p0, int p1) { m_previous_left_button_down_position[0] = p0; m_previous_left_button_down_position[1] = p1; };
    
    /// Create new object instance.
    static vmxMenuInteractorMouseMoveCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxMenu_API vmxMenuItem
{
    
public:
    
    /// Indicators for checked check boxes. Preset size, member m_number_of_check_boxes controlls how many check boxes there really are.
    int m_checkboxes[10];
    
    /// Number of check boxes for this item.
    unsigned int m_number_of_check_boxes;
    
    /// Indicates if this item has been selected.
    int m_is_selected;
    
    /// Text of the item.
    mxString m_text;
    
    /// Constructor.
    vmxMenuItem();
    
    /// Destructor.
    ~vmxMenuItem(){};
};



class vmxMenu_API vmxMenu
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxClipBoard *m_clip_board;
    
    /// Predefined placement positions for menu.
    enum vmxMenuPlacement
    {
        RELATIVE,//'relative' means relative to window size in percentages.
        FREE,//'free' means no fixed placement.
        LOWER_LEFT,
        LOWER_CENTER,
        LOWER_RIGHT,
        CENTER_LEFT,
        //CENTER_CENTER,
        CENTER_RIGHT,
        UPPER_LEFT,
        UPPER_CENTER,
        UPPER_RIGHT,
    };
    
    /// List of items in the menu.
    mxList<vmxMenuItem> m_items;
    
    /// List of pointers to items in the list of items. This array is built when calling BuildMenu() and is used for (fast)
    /// indexing access  to the items.
    mxArray<vmxMenuItem*> m_item_pointers;
    
    /// Maintin the maximum number of checkboxes contained in an item - this is needed for checking/picking check boxes.
    /// This value gets updated when adding and removing items.
    unsigned int m_max_number_of_check_boxes;
    
    /// Position (placement) of the menu. FREE is default.
    vmxMenuPlacement m_placement;
    
    /// For relative placement, the percentages are stored here.
    unsigned int m_placement_relative_percentages[2];
    
    
//    /// Indicates if the marked item needs to be shown (providing the m_index_of_marked_item is valid).
//    int m_is_marked_item_visible;
    
    
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
    
    /// Actor shown to indicate drag event (drag item from menu).
    vtkSmartPointer<vtkTextActor> m_drag_actor;

    /// Callback regulating the positioning of the menu when the render window is resized.
    vtkSmartPointer<vmxMenuRenderWindowModifiedCallback> m_window_modified_callback;
    
    /// Callback executed when the left button is pressed.
    vtkSmartPointer<vmxMenuInteractorLeftButtonDownCallback> m_left_button_down_callback;
    
    /// Callback executed when the left button is released.
    vtkSmartPointer<vmxMenuInteractorLeftButtonUpCallback> m_left_button_up_callback;

    /// Callback executed when the mouse is moved.
    vtkSmartPointer<vmxMenuInteractorMouseMoveCallback> m_mouse_move_callback;
    
    /// This is the character used to mark items as selected (by default it is underscore '_').
    char m_selection_text_character;
    
    /// Text line used to mark a single line of text as selected.
    mxString m_selection_text_line;
    
    
    //mxDataObjectTree *m_tree;
    
    
    /// Constructor.
    vmxMenu();
    
    /// Destructor.
    ~vmxMenu();
    
    /// Create new object instance.
    static vmxMenu* New();
    
    /// Add a new item to the item list.
    vmxMenuItem* AddItem(const char *item_text, unsigned int number_of_check_boxes=0);
    
    /// Update the VTK actors of the menu based on the changes of (added/removed) items.
    void BuildMenu();
    
    /// Based on the size of text actor, this will compute the selection text line to be used
    /// to mark items as selected. Use after interactor and font have been set.
    int ComputeSelectionTextLine();
    
    /// Copy data of selected items to clipboard.
    int CopySelectedItemsToClipBoard();
    
    /// Get item by index.
    vmxMenuItem* GetItem(unsigned int item_index) { return m_item_pointers[item_index]; };
    
    /// Get number of items in the menu.
    int GetNumberOfItems() { return m_items.GetNumberOfElements(); };
    
    /// Get number of items in the menu.
    int GetMaxNumberOfCheckBoxes() { return m_max_number_of_check_boxes; };

    /// Get origin (position) of the whole menu.
    int GetOriginOfMenu(int &origin1, int &origin2);

    /// Get origin (position) of the text actor.
    int GetOriginOfTextActor(int &origin1, int &origin2);

    /// Get origin (position) of the check box actor.
    int GetOriginOfCheckBoxActor(int &origin1, int &origin2);
    
    /// Get index of picked check box (for existing item index - call GetPickedItemIndex() before this method). If none, return -1 (invalid index).
    int GetPickedCheckBoxIndex(int picked_item_index, int pos1, int pos2);
    
    /// Get index of picked item. If none, return -1 (invalid index).
    int GetPickedItemIndex(int pos1, int pos2);
    
    /// Get the size of the menu check box actor.
    int GetSizeOfCheckBoxActor(int &output_size1, int &output_size2);
    
    /// Get the size of the whole menu (all actors together).
    int GetSizeOfMenu(int &output_size1, int &output_size2);
    
    //int GetSizeOfSelectionCharacter()
    
    /// Get the size of the menu text actor.
    int GetSizeOfTextActor(int &output_size1, int &output_size2);

    /// Given the input positions, check if the check box actor is picked (if the position falls within the check box actor).
    int IsCheckBoxActorPicked(int pos1, int pos2);
    
    /// Given the input positions, check if the menu is picked (if the position falls within the menu).
    int IsMenuPicked(int pos1, int pos2);
    
    /// Check if there are multiple items selected (more than one item must be selected).
    int IsMultipleItemsSelected();
    
    /// Given the input positions, check if the text actor is picked (if the position falls within the text actor).
    int IsTextActorPicked(int pos1, int pos2);
    
    /// Turn on/off listening for left button up event.
    void ListenForLeftButtonUpEvent(int is_listening);

    /// Turn on/off listening for mouse move event.
    void ListenForMouseMoveEvent(int is_listening);
    
    /// Based on existing placement preference, repositions the menu.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Set selection indicator of item with given index to 1, all others will be 0 (not selected).
    void SelectSingleItem(unsigned int item_index);
    
    /// Set size of font in the menu.
    void SetFontSize(double font_size);
    
//    /// Set the data object tree.
//    void SetDataObjectTree(mxDataObjectTree *tree);
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxClipBoard *clip_board) { m_clip_board = clip_board; };
    
    /// Set color of text in the menu.
    void SetColor(double r, double g, double b);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
 
//    /// Set visibility of the marked item.
//    void SetMarkedItemVisibility(int is_showing_marked_item);
//    
//    /// Mark the item with given input index.
//    void SetMarkedItemIndex(int index_of_marked_item);

    /// Set the origin (position) of the menu.
    void SetOrigin(int origin1, int origin2);
    
    /// Place the menu in the cernter left side of the render window.
    void SetPlacementToCenterLeft();
    
    /// Place the menu in the cernter right side of the render window.
    void SetPlacementToCenterRight();
    
    /// Place the menu in the lower center of the render window.
    void SetPlacementToLowerCenter();
    
    /// Place the menu in the lower left corner of the render window.
    void SetPlacementToLowerLeft();
    
    /// Place the menu in the lower right corner of the render window.
    void SetPlacementToLowerRight();
    
    /// Place the menu relative to the size of the render window.
    void SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent);
    
    /// Place the menu in the upper center of the render window.
    void SetPlacementToUpperCenter();
    
    /// Place the menu in the upper left corner of the render window.
    void SetPlacementToUpperLeft();
    
    /// Place the menu in the upper right corner of the render window.
    void SetPlacementToUpperRight();
    
//    /// Set text of the menu. Each item is a new line, hence separated with '\n' in input text.
//    void SetText(const char *text);
    
    /// Set visibility of the menu.
    void SetVisibility(int is_visible);
    
    /// Set visibility of the menu.
    void SetVisibilityOfDragEventActor(int is_visible);
    
    /// Shows selected items.
    int ShowSelectedItems();

};





#endif
