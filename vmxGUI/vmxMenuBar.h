/*=========================================================================
 
 Program:   mipx
 Module:    vmxMenuBar.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxMenuBar_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxMenuBar_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxMenuBar_EXPORTS
            #define vmxMenuBar_API __declspec(dllexport)
        #else
            #define vmxMenuBar_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxMenuBar_EXPORTS
            #define vmxMenuBar_API __attribute__((visibility("default")))
        #else
            #define vmxMenuBar_API
        #endif
    #endif
#endif



#ifndef vmxMenuBar_H
#define vmxMenuBar_H




#include <stdint.h>
#include "mxArray.h"
#include "mxList.h"
#include "mxString.h"
#include "vmxClipBoard.h"
#include "vmxGUIObject.h"
#include "vmxMenu.h"


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
class vmxMenuBar;



//class vmxMenuBar_API vmxMenuBarInteractorLeftButtonDownCallback : public vtkCommand
//{
//    
//public:
//    
//    /// Pointer to text input that uses this callback.
//    vmxMenuBar *m_text_input;
//    
//    /// Constructor.
//    vmxMenuBarInteractorLeftButtonDownCallback();
//    
//    /// Destructor.
//    ~vmxMenuBarInteractorLeftButtonDownCallback();
//    
//    /// Initialize the text input linked to this callback
//    void SetTextInput(vmxMenuBar *text_input);
//    
//    /// Create new object instance.
//    static vmxMenuBarInteractorLeftButtonDownCallback* New();
//    
//    /// Method that executes when the callback is called.
//    virtual void Execute(vtkObject *caller, unsigned long, void *);
//};



class vmxMenuBar_API vmxMenuBarInteractorLeftButtonUpCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxMenuBar *m_menu_bar;
    
    /// Constructor.
    vmxMenuBarInteractorLeftButtonUpCallback();
    
    /// Destructor.
    ~vmxMenuBarInteractorLeftButtonUpCallback();
    
    /// Initialize the text input linked to this callback
    void SetTextInput(vmxMenuBar *menu_bar);
    
    /// Create new object instance.
    static vmxMenuBarInteractorLeftButtonUpCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};





class vmxMenuBar_API vmxMenuBarItem
{
    
public:
    
    /// Pointer to the menu bar this item belongs to.
    vmxMenuBar *m_menu_bar;
    
    /// Menu called from this item. If none pointer is NULL.
    vmxMenu m_menu;
    
    /// Text of the item.
    mxString m_text;
    
    /// Text actor of the item.
    vtkSmartPointer<vtkTextActor> m_text_actor;

    
    /// Constructor.
    vmxMenuBarItem();
    
    /// Destructor.
    ~vmxMenuBarItem();
    
    /// Get the menu attached to this item.
    vmxMenu* GetMenu()
    {
        return &m_menu;
    };
    
    void GetOriginOfTextActor(int &output_origin1, int &output_origin2);
    
    /// Get the size of the menu text actor.
    void GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Given the input positions, check if the item is picked (if the position falls within the text actor).
    int IsPicked(int pos1, int pos2);
    
    /// Show the input text.
    int ShowInputText();
};





class vmxMenuBar_API vmxMenuBar : public vmxGUIObject
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxClipBoard *m_clip_board;
    
//    /// Description text.
//    mxString m_description;
    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Actors containing the text of menu bar items.
    mxList< vmxMenuBarItem > m_items;
    
//    /// Callback executed when the left button is pressed.
//    vtkSmartPointer<vmxMenuBarInteractorLeftButtonDownCallback> m_left_button_down_callback;
    
    /// Callback executed when the left button is released.
    vtkSmartPointer<vmxMenuBarInteractorLeftButtonUpCallback> m_left_button_up_callback;
    
   
    
    
    /// Constructor.
    vmxMenuBar();
    
    /// Destructor.
    ~vmxMenuBar();
    
    /// Create new object instance.
    static vmxMenuBar* New();
    
    /// Add a new item to the menu bar.
    vmxMenuBarItem* AddItem(const char *item_name);
    
    /// Get origin (position) of the whole object.
    void GetOrigin(int &origin1, int &origin2);
    
    /// For given input positions, get the picked item. If none return NULL.
    vmxMenuBarItem* GetPickedItem(int pos1, int pos2);
    
    /// Get the size of the whole object (all actors together).
    void GetSize(int &output_size1, int &output_size2);
    
    /// This will set the visibility of all menus to 0 (not visible).
    void HideMenus();
    
    /// Given the input positions, check if the object is picked (if the position falls within the menu bar).
    int IsPicked(int pos1, int pos2);
    
    /// Get visibility of the object.
    int IsVisible();
    
//    /// Set the description text that precedes the input
//    void SetDescriptionText(const char *description) { m_description.Assign(description); };
    
    /// Set size of font in the object.
    void SetFontSize(double font_size);
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxClipBoard *clip_board)
    {
        m_clip_board = clip_board;
    };
    
    /// Set color of text in the object.
    void SetColor(double r, double g, double b);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set maximum size of the object. Re-implemented from vmxGUIObject.
    void SetMaximumSize(int max_size_x, int max_size_y);
    
    /// Set the origin (position) of the menu.
    void SetOrigin(int origin1, int origin2);
    
    /// Set visibility of the menu.
    void SetVisibility(int is_visible);
    
    /// Show the input text.
    int ShowInputText();
    
};



#endif
