/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIMenuBar.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxGUIMenuBar_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIMenuBar_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIMenuBar_EXPORTS
            #define vmxGUIMenuBar_API __declspec(dllexport)
        #else
            #define vmxGUIMenuBar_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIMenuBar_EXPORTS
            #define vmxGUIMenuBar_API __attribute__((visibility("default")))
        #else
            #define vmxGUIMenuBar_API
        #endif
    #endif
#endif



#ifndef vmxGUIMenuBar_H
#define vmxGUIMenuBar_H




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





// Pre-declaration.
class vmxGUIMenuBar;



/// Item of a menu bar.

class vmxGUIMenuBar_API vmxGUIMenuBarItem
{
    
public:
    
    /// Pointer to the menu bar this item belongs to.
    vmxGUIMenuBar *m_menu_bar;
    
    /// Menu called from this item. If none pointer is NULL.
    vmxGUIMenu m_menu;
    
    /// Text of the item.
    mxString m_text;
    
    /// Text actor of the item.
    vtkSmartPointer<vtkTextActor> m_text_actor;

    
    /// Constructor.
    vmxGUIMenuBarItem();
    
    /// Destructor.
    ~vmxGUIMenuBarItem();
    
    /// Get the menu attached to this item.
    vmxGUIMenu* GetMenu()
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




/// VTK GUI menu bar widget.

class vmxGUIMenuBar_API vmxGUIMenuBar : public vmxGUIWidget
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxGUIClipBoard *m_clip_board;
    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Color of the characters.
    double m_color_r, m_color_g, m_color_b;
    
    /// Actors containing the text of menu bar items.
    mxList< vmxGUIMenuBarItem > m_items;
   
    
    
    /// Constructor.
    vmxGUIMenuBar();
    
    /// Destructor.
    ~vmxGUIMenuBar();
    
    /// Create new object instance.
    static vmxGUIMenuBar* New();
    
    /// Add a new item to the menu bar.
    vmxGUIMenuBarItem* AddItem(const char *item_name);
    
    /// Call this to rebuild menu bar.
    void BuildMenuBar()
    {
        this->SetInteractor(this->m_interactor);
        this->SetFontSize(m_font_size);
        this->SetColor(m_color_r, m_color_g, m_color_b);
        //this->HideMenus();
    };

    /// Get item based on its name. If none found, return NULL.
    vmxGUIMenuBarItem* GetItem(const char *item_name);
    
    /// Get origin (position) of the whole object.
    void GetOrigin(int &origin1, int &origin2);
    
    /// For given input positions, get the picked item. If none return NULL.
    vmxGUIMenuBarItem* GetPickedItem(int pos1, int pos2);
    
    /// Get the size of the whole object (all actors together).
    void GetSize(int &output_size1, int &output_size2);
    
    /// This will set the visibility of all menus to 0 (not visible).
    void HideMenus();
    
    /// Given the input positions, check if the object is picked (if the position falls within the menu bar).
    int IsPicked(int pos1, int pos2);
    
    /// Get visibility of the object.
    int IsVisible();
    
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
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxGUIClipBoard *clip_board)
    {
        m_clip_board = clip_board;
    };
    
    /// Set color of text in the object.
    void SetColor(double r, double g, double b);
    
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
    
    /// Show the input text.
    int ShowInputText();
    
};



#endif
