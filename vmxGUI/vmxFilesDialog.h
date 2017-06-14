/*=========================================================================
 
 Program:   mipx
 Module:    vmxFilesDialog.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



/*
 
 ToDo: add SelectExistingDirectory, SaveFile, OK/Cancel buttons, stop scrolling when text actor fits on screen, SHIFT button selection,
        connect to vmxGUI, replace window->GetSize with GetRenderWindowAvailableSize and make it use vmxGUI size maintenance.
 
 */





#if defined(vmxFilesDialog_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxFilesDialog_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxFilesDialog_EXPORTS
            #define vmxFilesDialog_API __declspec(dllexport)
        #else
            #define vmxFilesDialog_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxFilesDialog_EXPORTS
            #define vmxFilesDialog_API __attribute__((visibility("default")))
        #else
            #define vmxFilesDialog_API
        #endif
    #endif
#endif



#ifndef vmxFilesDialog_H
	#define vmxFilesDialog_H



//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)



#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"
#include "vmxClipBoard.h"
#include "vmxEventDetector.h"

#include "tinydir.h"


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
class vmxFilesDialog;



class vmxFilesDialog_API vmxFilesDialogRenderWindowModifiedCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to dialog that uses this callback.
    vmxFilesDialog *m_dialog;
    
    int m_previous_window_size[2];
    
    /// Constructor.
    vmxFilesDialogRenderWindowModifiedCallback();
    
    /// Destructor.
    ~vmxFilesDialogRenderWindowModifiedCallback();
    
    /// Initialize the dialog linked to this callback
    void SetDialog(vmxFilesDialog *dialog);
    
    /// Create new object instance.
    static vmxFilesDialogRenderWindowModifiedCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);

    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxFilesDialog_API vmxFilesDialogInteractorLeftButtonDownCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to dialog that uses this callback.
    vmxFilesDialog *m_dialog;
    
    /// Constructor.
    vmxFilesDialogInteractorLeftButtonDownCallback();
    
    /// Destructor.
    ~vmxFilesDialogInteractorLeftButtonDownCallback();
    
    /// Initialize the dialog linked to this callback
    void SetDialog(vmxFilesDialog *dialog);
    
    /// Create new object instance.
    static vmxFilesDialogInteractorLeftButtonDownCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};





class vmxFilesDialog_API vmxFilesDialogInteractorMouseWheelBackwardCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to dialog that uses this callback.
    vmxFilesDialog *m_dialog;
    
    /// Constructor.
    vmxFilesDialogInteractorMouseWheelBackwardCallback();
    
    /// Destructor.
    ~vmxFilesDialogInteractorMouseWheelBackwardCallback();
    
    /// Initialize the dialog linked to this callback
    void SetDialog(vmxFilesDialog *dialog);
    
    /// Create new object instance.
    static vmxFilesDialogInteractorMouseWheelBackwardCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};


class vmxFilesDialog_API vmxFilesDialogInteractorMouseWheelForwardCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to dialog that uses this callback.
    vmxFilesDialog *m_dialog;
    
    /// Constructor.
    vmxFilesDialogInteractorMouseWheelForwardCallback();
    
    /// Destructor.
    ~vmxFilesDialogInteractorMouseWheelForwardCallback();
    
    /// Initialize the dialog linked to this callback
    void SetDialog(vmxFilesDialog *dialog);
    
    /// Create new object instance.
    static vmxFilesDialogInteractorMouseWheelForwardCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxFilesDialog_API vmxFilesDialogInteractorKeyPressCallback : public vtkCommand
{
    /// Indicates if the callback is on or off.
    int m_is_active;
    
public:
    
    /// Pointer to dialog that uses this callback.
    vmxFilesDialog *m_dialog;
    
    /// Constructor.
    vmxFilesDialogInteractorKeyPressCallback();
    
    /// Destructor.
    ~vmxFilesDialogInteractorKeyPressCallback();
    
    /// Initialize the dialog linked to this callback
    void SetDialog(vmxFilesDialog *dialog);
    
    /// Create new object instance.
    static vmxFilesDialogInteractorKeyPressCallback* New();
    
    /// Turn callback on or off.
    void Activate(int is_active);
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxFilesDialogSlot
{
public:
    virtual int Execute(vmxFilesDialog *dialog) {cout<<" reimplement this "; return 0;};
    virtual ~vmxFilesDialogSlot() {};
};



class vmxFilesDialog_API vmxFilesDialogItem
{
    
public:
    
    /// Pointer to the dialog this item belongs to.
    vmxFilesDialog *m_dialog;
    
    /// Indicates if this item has been selected.
    int m_is_selected;
    
    /// Text of the item.
    mxString m_text;
    
    /// Constructor.
    vmxFilesDialogItem();
    
    /// Destructor.
    ~vmxFilesDialogItem();
};



class vmxFilesDialog_API vmxFilesDialog
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxClipBoard *m_clip_board;
    
    /// Current directory path
    mxString m_path_directory;
    
    
    /// Predefined placement positions for a dialog.
    enum vmxFilesDialogPlacement
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
        UPPER_RIGHT
    };
    
    /// File/directory selection types of a dialog.
    enum vmxFilesDialogSelectionType
    {
        OPEN_FILES,
        SELECT_EXISTING_DIRECTORY,
        SAVE_FILE
    };
    
    /// Detects if double click was performed.
    vmxDoubleClickDetector m_double_click_detector;
        
    /// Detects if items were selected.
    vmxItemSelectionDetector m_item_selection_detector;

    /// List of items in the dialog.
    mxList<vmxFilesDialogItem> m_items;
    
    /// List of pointers to items in the list of items. This array is built when calling BuildDialog() and is used for (fast)
    /// indexing access  to the items.
    mxArray<vmxFilesDialogItem*> m_item_pointers;
    
    
    /// Position (placement) of the dialog. FREE is default.
    vmxFilesDialogPlacement m_placement;
    
    /// Selection type of files/directories for the dialog. OPEN_FILES is default.
    vmxFilesDialogSelectionType m_selection_type;
    
    /// For relative placement, the percentages are stored here.
    unsigned int m_placement_relative_percentages[2];
    
    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    int m_text_index_offset;
    
    /// Maximumnuber of items that will be stored in the text actor (use lower numbers for efficiency).
    int m_text_actor_max_number_of_items;
    
    /// Actual number of items contained in the text actor.
    /// Notice: this number can be different than the total number of items in the dialog!
    int m_number_of_items_in_text_actor;
    
    /// Actor containing the dialog item text.
    vtkSmartPointer<vtkTextActor> m_text_actor;
    
    /// Actor containing the user text of the dialog.
    vtkSmartPointer<vtkTextActor> m_user_text_actor;
    
    /// Actor containing the check boxes (as text) of the dialog.
    vtkSmartPointer<vtkTextActor> m_path_actor;

    /// Callback regulating the positioning of the dialog when the render window is resized.
    vtkSmartPointer<vmxFilesDialogRenderWindowModifiedCallback> m_window_modified_callback;
    
    /// Callback executed when the left button is pressed.
    vtkSmartPointer<vmxFilesDialogInteractorLeftButtonDownCallback> m_left_button_down_callback;
        
    /// Callback executed when the mouse wheel backward is detected.
    vtkSmartPointer<vmxFilesDialogInteractorMouseWheelBackwardCallback> m_mouse_wheel_backward_callback;
    
    /// Callback executed when the mouse wheel forward is detected.
    vtkSmartPointer<vmxFilesDialogInteractorMouseWheelForwardCallback> m_mouse_wheel_forward_callback;
    
    /// Callback executed when the mouse wheel forward is detected.
    vtkSmartPointer<vmxFilesDialogInteractorKeyPressCallback> m_key_press_callback;

   
    
    /// Constructor.
    vmxFilesDialog();
    
    /// Destructor.
    virtual ~vmxFilesDialog();
    
    /// Create new object instance.
    static vmxFilesDialog* New();
    
    /// Add a new item to the item list.
    vmxFilesDialogItem* AddItem(const char *item_text);
    
    /// Update the VTK actors of the dialog based on the changes of (added/removed) items.
    void BuildDialog();
    
    /// Get color of the dialog text.
    void GetColor(double &r, double &g, double &b);
    
    /// Get item by index.
    vmxFilesDialogItem* GetItem(int item_index) { if(item_index<0 || item_index>=this->GetNumberOfItems()) return NULL; return m_item_pointers[item_index]; };
    
    /// Get number of items in the dialog.
    int GetNumberOfItems() { return m_items.GetNumberOfElements(); };
    
    /// Get origin (position) of the whole dialog.
    int GetOriginOfDialog(int &origin1, int &origin2);

    /// Get origin (position) of the text actor.
    int GetOriginOfTextActor(int &origin1, int &origin2);
    
    /// Get index of picked item. If none, return -1 (invalid index).
    int GetPickedItemIndex(int pos1, int pos2);
    
    /// Get the size of the whole dialog (all actors together).
    int GetSizeOfDialog(int &output_size1, int &output_size2);
    
    /// Get the size of the dialog text actor.
    int GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Get the size of the dialog user text actor.
    int GetSizeOfUserTextActor(int &output_size1, int &output_size2);

    /// Get the size of the dialog path actor.
    int GetSizeOfPathActor(int &output_size1, int &output_size2);
    
    /// Get available size of the render window.
    int GetRenderWindowAvailableSize(int &output_size1, int &output_size2);


    
    /// Get visibility of the dialog (check if the dialog is visible).
    int GetVisibility();
    
    /// Given the input positions, check if the dialog is picked (if the position falls within the dialog).
    int IsDialogPicked(int pos1, int pos2);
    
    /// Check if there are multiple items selected (more than one item must be selected).
    int IsMultipleItemsSelected();
    
    /// Given the input positions, check if the text actor is picked (if the position falls within the text actor).
    int IsTextActorPicked(int pos1, int pos2);
    
    /// Turn on/off listening for left button down event.
    void ListenForLeftButtonDownEvent(int is_listening);

    /// Turn on/off listening for render window modified event.
    void ListenForRenderWindowModifiedEvent(int is_listening);
    
    /// Turn on/off listening for mouse wheel event.
    void ListenForMouseWheelBackwardEvent(int is_listening);
    
    /// Turn on/off listening for mouse wheel event.
    void ListenForMouseWheelForwardEvent(int is_listening);
    
    /// Turn on/off listening for key press event.
    void ListenForKeyPressEvent(int is_listening);

    
    
    /// Based on existing placement preference, repositions the dialog.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Set selection indicator of item with given index to 1, all others will be 0 (not selected).
    void SelectSingleItem(unsigned int item_index);
    
    /// Set size of font in the dialog.
    void SetFontSize(double font_size);
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxClipBoard *clip_board) { m_clip_board = clip_board; };
    
    /// Set color of text in the dialog.
    void SetColor(double r, double g, double b);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
 
    /// Set the origin (position) of the dialog.
    void SetOrigin(int origin1, int origin2);
    
//    /// Place the dialog in the cernter left side of the render window.
//    void SetPlacementToCenterLeft();
//    
//    /// Place the dialog in the cernter right side of the render window.
//    void SetPlacementToCenterRight();
    
    /// Place the dialog in the lower center of the render window.
    void SetPlacementToLowerCenter();
    
    /// Place the dialog in the lower left corner of the render window.
    void SetPlacementToLowerLeft();
    
    /// Place the dialog in the lower right corner of the render window.
    void SetPlacementToLowerRight();
    
    /// Place the dialog relative to the size of the render window.
    void SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent);
    
    /// Place the dialog in the upper center of the render window.
    void SetPlacementToUpperCenter();
    
    /// Place the dialog in the upper left corner of the render window.
    void SetPlacementToUpperLeft();
    
    /// Place the dialog in the upper right corner of the render window.
    void SetPlacementToUpperRight();
    
    /// Set visibility of the dialog.
    void SetVisibility(int is_visible);
    
    
    /// Shows text actor items. Offset is a number that indicates if we need to start the text with an offset
    /// (e.g. offset=3 means that we start display of text from the third elemet).
    int ShowTextItems(int offset=0);

    
    /// Start the dialog.
    int OpenFiles(const char *directory_path=NULL);
    
    

};





#endif
