/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIIconButtonGroup.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



/*
 
 INCOMPLETE AND NOT TESTED!!!!
 
 */


#if defined(vmxGUIIconButtonGroup_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIIconButtonGroup_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIIconButtonGroup_EXPORTS
            #define vmxGUIIconButtonGroup_API __declspec(dllexport)
        #else
            #define vmxGUIIconButtonGroup_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIIconButtonGroup_EXPORTS
            #define vmxGUIIconButtonGroup_API __attribute__((visibility("default")))
        #else
            #define vmxGUIIconButtonGroup_API
        #endif
    #endif
#endif



#ifndef vmxGUIIconButtonGroup_H
#define vmxGUIIconButtonGroup_H



#include <stdint.h>
#include "mxArray.h"
#include "mxList.h"
#include "mxString.h"
#include "vmxGUIClipBoard.h"
#include "vmxGUIWidget.h"
#include "vmxGUIMenu.h"


#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>




#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




// Pre-declaration.
class vmxGUIIconButtonGroup;




class vmxGUIIconButtonGroup_API vmxGUIIconButtonGroupInteractorLeftButtonUpCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxGUIIconButtonGroup *m_button_group;
    
    /// Constructor.
    vmxGUIIconButtonGroupInteractorLeftButtonUpCallback();
    
    /// Destructor.
    ~vmxGUIIconButtonGroupInteractorLeftButtonUpCallback();
    
    /// Initialize the text input linked to this callback
    void SetTextInput(vmxGUIIconButtonGroup *menu_bar);
    
    /// Create new object instance.
    static vmxGUIIconButtonGroupInteractorLeftButtonUpCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};


class vmxPushButtonSlot
{
public:
    virtual int Execute(vmxGUIIconButtonGroup *button_group) {cout<<" reimplement this "; return 0;};
    virtual ~vmxPushButtonSlot() {};
};



class vmxGUIIconButtonGroup_API vmxGUIIconButtonGroupItem
{
    
public:
    
    /// Pointer to the menu bar this item belongs to.
    vmxGUIIconButtonGroup *m_button_group;
    
    /// Indicates if this button is checked (e.g. in case type is radio button or check box).
    int m_is_checked;
    
    /// Slot to be executed when push button is pressed;
    vmxPushButtonSlot *m_slot;
    
    /// Text of the item.
    mxString m_text;
    
    /// Text actor of the item.
    vtkSmartPointer<vtkTextActor> m_text_actor;

    
    /// Constructor.
    vmxGUIIconButtonGroupItem();
    
    /// Destructor.
    ~vmxGUIIconButtonGroupItem();
    
    /// Get the position of the text actor on screen.
    void GetOriginOfTextActor(int &output_origin1, int &output_origin2);
    
    /// Get the size of the menu text actor.
    void GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Given the input positions, check if the item is picked (if the position falls within the text actor).
    int IsPicked(int pos1, int pos2);
    
    /// Check/uncheck the button.
    void SetChecked(int is_checked) { m_is_checked = is_checked; };
    
    /// Show the input text.
    int ShowInputText();
};



class vmxGUIIconButtonGroup_API vmxGUIIconButtonGroup : public vmxGUIWidget
{
    
public:
    
    /// Types of buttons.
    enum vmxInputType
    {
        CHECK_BOX,
        RADIO_BUTTON,
        PUSH_BUTTON, // default
    };
    
    /// Button type of this object.
    vmxInputType m_button_type;
    
//    /// Pointer to clipboard this object uses.
//    vmxClipBoard *m_clip_board;
//    
    /// Description text that preceeds the input
    mxString m_description_text;
    
    /// Text actor of the description text.
    vtkSmartPointer<vtkTextActor> m_description_text_actor;
    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Actors containing the text of menu bar items.
    mxList< vmxGUIIconButtonGroupItem > m_items;
        
    /// Callback executed when the left button is released.
    vtkSmartPointer<vmxGUIIconButtonGroupInteractorLeftButtonUpCallback> m_left_button_up_callback;
    
   
    
    
    /// Constructor.
    vmxGUIIconButtonGroup();
    
    /// Destructor.
    ~vmxGUIIconButtonGroup();
    
    /// Create new object instance.
    static vmxGUIIconButtonGroup* New();
    
    /// Add a new item to the menu bar.
    vmxGUIIconButtonGroupItem* AddItem(const char *item_name, int is_checked = 0);
    
    /// Get origin (position) of the whole object.
    void GetOrigin(int &origin1, int &origin2);
    
    /// Get origin (position on screen) of the description text actor.
    void GetOriginOfDescriptionText(int &origin1, int &origin2);
    
    /// For given input positions, get the picked item. If none return NULL.
    vmxGUIIconButtonGroupItem* GetPickedItem(int pos1, int pos2);
    
    /// Get size of the whole object (all actors together).
    void GetSize(int &output_size1, int &output_size2);
    
    /// Get size (on screen) of the description text actor.
    void GetSizeOfDescriptionText(int &output_size1, int &output_size2);
    
    /// Given the input positions, check if the object is picked (if the position falls within the menu bar).
    int IsPicked(int pos1, int pos2);
    
    /// Get visibility of the object.
    int IsVisible();
    
//    /// Set the description text that precedes the input
//    void SetDescriptionText(const char *description) { m_description.Assign(description); };
    
    /// Set size of font in the object.
    void SetFontSize(double font_size);
    
    /// Set the button type to check box.
    void SetButtonTypeToCheckBox();
    
    /// Set the button type to radio button.
    void SetButtonTypeToRadioButton();
    
    /// Set the button type to push button.
    void SetButtonTypeToPushButton();
    
    /// Set color of text in the object.
    void SetColor(double r, double g, double b);
    
    /// Set the description text (text that preceeds the items).
    void SetDescriptionText(const char *description_text);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set maximum size of the object. Re-implemented from vmxGUIWidget.
    void SetMaximumSize(int max_size_x, int max_size_y);
    
    /// Set the origin (position) of the menu.
    void SetOrigin(int origin1, int origin2);
    
    /// Set visibility of the menu.
    void SetVisibility(int is_visible);
    
    /// Show the input text.
    int ShowInputText();
    
    /// This will uncheck all items. Used for radio button manipulation.
    void UncheckAllItems();
    
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
