/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIButtonGroup.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxGUIButtonGroup_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIButtonGroup_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIButtonGroup_EXPORTS
            #define vmxGUIButtonGroup_API __declspec(dllexport)
        #else
            #define vmxGUIButtonGroup_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIButtonGroup_EXPORTS
            #define vmxGUIButtonGroup_API __attribute__((visibility("default")))
        #else
            #define vmxGUIButtonGroup_API
        #endif
    #endif
#endif



#ifndef vmxGUIButtonGroup_H
#define vmxGUIButtonGroup_H



#include <stdint.h>
#include "mxArray.h"
#include "mxList.h"
#include "mxString.h"
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
class vmxGUIButtonGroup;





class vmxGUIButtonGroup_API vmxGUIButtonGroupSlot
{
public:
    virtual int Execute(vmxGUIButtonGroup *button_group)
    {
        // reimplement this.
        return 0;
    };
    virtual ~vmxGUIButtonGroupSlot() {};
};



/// GUI class of group of buttons which can be push buttons, radio buttons or check boxes.

class vmxGUIButtonGroup_API vmxGUIButtonGroupItem
{
    
public:
    
    /// Pointer to the object this item belongs to.
    vmxGUIButtonGroup *m_button_group;
    
    /// Indicates if this button is checked (e.g. in case type is radio button or check box).
    int m_is_checked;
    
    /// Slot to be executed when push button is pressed;
    vmxGUIButtonGroupSlot *m_slot;
    
    /// Text of the item.
    mxString m_text;
    
    /// Text actor of the item.
    vtkSmartPointer<vtkTextActor> m_text_actor;

    
    /// Constructor.
    vmxGUIButtonGroupItem();
    
    /// Destructor.
    ~vmxGUIButtonGroupItem();
    
    /// Get the position of the text actor on screen.
    void GetOriginOfTextActor(int &output_origin1, int &output_origin2);
    
    /// Get the size of the text actor.
    void GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Given the input positions, check if the item is picked (if the position falls within the text actor).
    int IsPicked(int pos1, int pos2);
    
    /// Check/uncheck the button.
    void SetChecked(int is_checked) { m_is_checked = is_checked; };
    
    /// Show the input text.
    int ShowInputText();
};



class vmxGUIButtonGroup_API vmxGUIButtonGroup : public vmxGUIWidget
{
    
public:
    
    /// Types of buttons.
    enum vmxInputType
    {
        CHECK_BOX,
        RADIO_BUTTON,
        PUSH_BUTTON, // default is push button
    };
    
    /// Button type of this object.
    vmxInputType m_button_type;
        
    /// Description text that preceeds the input.
    mxString m_description_text;
    
    /// Text actor of the description text.
    vtkSmartPointer<vtkTextActor> m_description_text_actor;
    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// List of items in the group.
    mxList< vmxGUIButtonGroupItem > m_items;
   
    
    
    /// Constructor.
    vmxGUIButtonGroup();
    
    /// Destructor.
    virtual ~vmxGUIButtonGroup();
    
    /// Create new object instance.
    static vmxGUIButtonGroup* New();
    
    /// Add a new item to the menu bar.
    vmxGUIButtonGroupItem* AddItem(const char *item_name, int is_checked = 0);
    
    /// Gt array of checked items.
    void GetCheckedItems(mxArray< vmxGUIButtonGroupItem* > &array_of_checked_items);
    
    /// Get origin (position) of the whole object.
    void GetOrigin(int &origin1, int &origin2);
    
    /// Get origin (position on screen) of the description text actor.
    void GetOriginOfDescriptionText(int &origin1, int &origin2);
    
    /// For given input positions, get the picked item. If none return NULL.
    vmxGUIButtonGroupItem* GetPickedItem(int pos1, int pos2);
    
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
    
    /// Callback method executed the given event.
    virtual void OnKeyPress() {};
    
    /// Callback method executed the given event.
    virtual void OnMouseMove() {};
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonUp();
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonDown() {};
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelForward() {};
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelBackward() {};
    
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
    
    /// Set the main widget and connect visualization objects contained within it.
    void SetMainWidget(vmxGUIMainWidget *main_widget);
    
    /// Set maximum size of the object. Re-implemented from vmxGUIWidget.
    void SetMaximumSize(int max_size_x, int max_size_y);
    
    /// Set the origin (position) of the menu.
    void SetOrigin(int origin1, int origin2);
    
    /// Set visibility of the menu.
    void SetVisibility(int is_visible);
    
    /// Show the input text. // THIS IS LIKE AN UPDATE METHOD!!!!!!!!!!!!!
    int ShowInputText();
    
    /// This will uncheck all items. Used for radio button manipulation.
    void UncheckAllItems();
    
};



#endif
