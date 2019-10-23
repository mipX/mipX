/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUILabel.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxGUILabel_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUILabel_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUILabel_EXPORTS
            #define vmxGUILabel_API __declspec(dllexport)
        #else
            #define vmxGUILabel_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUILabel_EXPORTS
            #define vmxGUILabel_API __attribute__((visibility("default")))
        #else
            #define vmxGUILabel_API
        #endif
    #endif
#endif



#ifndef vmxGUILabel_H
#define vmxGUILabel_H



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




/// Text label class.

class vmxGUILabel_API vmxGUILabel : public vmxGUIWidget
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxGUIClipBoard *m_clip_board;
    
    /// Text of the label.
    mxString m_description;
    
    /// Assigned VTK Render Window Interactor.
    vtkRenderWindowInteractor *m_interactor;
    
    /// Assigned VTK Renderer.
    vtkRenderer *m_renderer;

    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Actor containing the text.
    vtkSmartPointer<vtkTextActor> m_text_actor;

    
    
    /// Constructor.
    vmxGUILabel();
    
    /// Destructor.
    virtual ~vmxGUILabel();
    
    /// Create new object instance.
    static vmxGUILabel* New();
    
    /// Get origin (position) of the whole object.
    void GetOrigin(int &origin1, int &origin2);
    
    /// Get the size of the whole object (all actors together).
    void GetSize(int &output_size1, int &output_size2);
    
    /// Get the size of the menu text actor.
    int GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Check if the given 2D VTK actor is contained in this object.
    int HasVTKActor2D(vtkActor2D *actor_2D);
    
    /// Get visibility of the object.
    int IsVisible();
        
    /// Callback method executed the given event.
    virtual void OnKeyPress(){};
    
    /// Callback method executed the given event.
    virtual void OnMouseMove(){};
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonUp(){};
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonDown(){};
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDoubleClick(){};
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrag(){};
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrop(){};
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelForward(){};
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelBackward(){};
    
    /// Set the description text that precedes the input
    void SetText(const char *description) { m_description.Assign(description); };
    
    /// Set size of font in the object.
    void SetFontSize(double font_size);
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxGUIClipBoard *clip_board) { m_clip_board = clip_board; };
    
    /// Set color of text in the object.
    void SetColor(double r, double g, double b);
        
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set the main widget and connect visualization objects contained within it.
    void SetMainWidget(vmxGUIMainWidget *main_widget);
    
    void SetMaximumSize(int max_size_x, int max_size_y)
    {
    };

    
    /// Set the origin (position) of the menu.
    void SetOrigin(int origin1, int origin2);
    
    /// Set the renderer for this object.
    void SetRenderer(vtkRenderer *renderer);
    
    /// Set visibility of the menu.
    void SetVisibility(int is_visible);
    
    /// Show the input text.
    int ShowInputText();
    
    
};



#endif
