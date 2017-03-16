/*=========================================================================
 
 Program:   mipx
 Module:    vmxTextInput.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxTextInput_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxTextInput_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxTextInput_EXPORTS
            #define vmxTextInput_API __declspec(dllexport)
        #else
            #define vmxTextInput_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxTextInput_EXPORTS
            #define vmxTextInput_API __attribute__((visibility("default")))
        #else
            #define vmxTextInput_API
        #endif
    #endif
#endif



#ifndef vmxTextInput_H
#define vmxTextInput_H



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
class vmxTextInput;


class vmxTextInput_API vmxTextInputRenderWindowModifiedCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxTextInput *m_text_input;
    
    int m_previous_window_size[2];
    
    /// Constructor.
    vmxTextInputRenderWindowModifiedCallback();
    
    /// Destructor.
    ~vmxTextInputRenderWindowModifiedCallback();
    
    /// Initialize the text input linked to this callback
    void SetTextInput(vmxTextInput *text_input);
    
    /// Create new object instance.
    static vmxTextInputRenderWindowModifiedCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



//class vmxTextInput_API vmxTextInputInteractorLeftButtonDownCallback : public vtkCommand
//{
//    
//public:
//    
//    /// Pointer to text input that uses this callback.
//    vmxTextInput *m_text_input;
//    
//    /// Constructor.
//    vmxTextInputInteractorLeftButtonDownCallback();
//    
//    /// Destructor.
//    ~vmxTextInputInteractorLeftButtonDownCallback();
//    
//    /// Initialize the text input linked to this callback
//    void SetTextInput(vmxTextInput *text_input);
//    
//    /// Create new object instance.
//    static vmxTextInputInteractorLeftButtonDownCallback* New();
//    
//    /// Method that executes when the callback is called.
//    virtual void Execute(vtkObject *caller, unsigned long, void *);
//};



class vmxTextInput_API vmxTextInputInteractorKeyPressCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxTextInput *m_text_input;
    
    /// Constructor.
    vmxTextInputInteractorKeyPressCallback();
    
    /// Destructor.
    ~vmxTextInputInteractorKeyPressCallback();
    
    /// Initialize the text input linked to this callback
    void SetTextInput(vmxTextInput *text_input);
    
    /// Create new object instance.
    static vmxTextInputInteractorKeyPressCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};



class vmxTextInput_API vmxTextInputInteractorLeftButtonUpCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxTextInput *m_text_input;
    
    /// Constructor.
    vmxTextInputInteractorLeftButtonUpCallback();
    
    /// Destructor.
    ~vmxTextInputInteractorLeftButtonUpCallback();
    
    /// Initialize the text input linked to this callback
    void SetTextInput(vmxTextInput *text_input);
    
    /// Create new object instance.
    static vmxTextInputInteractorLeftButtonUpCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};








class vmxTextInput_API vmxTextInput
{
    
public:
    
    /// Pointer to clipboard this object uses.
    vmxClipBoard *m_clip_board;
    
    /// Description text that precedes the input.
    mxString m_description;
    
    /// Input string.
    char m_input[100];
    
    /// Number of entered characters (indicates how many characters are valid in m_input array).
    unsigned int m_number_of_entered_characters;
    
    /// Predefined placement positions for menu.
    enum vmxTextInputPlacement
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
    
    /// Position (placement) of the menu. FREE is default.
    vmxTextInputPlacement m_placement;
    
    /// For relative placement, the percentages are stored here.
    unsigned int m_placement_relative_percentages[2];
    
    /// Assigned VTK Render Window Interactor.
    //vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    vtkRenderWindowInteractor *m_interactor;
    
//    /// Indicates if the text input was picked with the last left click and is therefore currently active.
//    int m_is_active;
    
    /// Font size of the used chacaters.
    int m_font_size;
    
    /// Actor containing the text.
    vtkSmartPointer<vtkTextActor> m_text_actor;
    
    /// Callback regulating the positioning of the object when the render window is resized.
    vtkSmartPointer<vmxTextInputRenderWindowModifiedCallback> m_window_modified_callback;
    
//    /// Callback executed when the left button is pressed.
//    vtkSmartPointer<vmxTextInputInteractorLeftButtonDownCallback> m_left_button_down_callback;
    
    /// Callback executed when the left button is released.
    vtkSmartPointer<vmxTextInputInteractorLeftButtonUpCallback> m_left_button_up_callback;
    
    /// Callback executed when the key is pressed.
    vtkSmartPointer<vmxTextInputInteractorKeyPressCallback> m_key_press_callback;
    
    
    
    /// Constructor.
    vmxTextInput();
    
    /// Destructor.
    ~vmxTextInput();
    
    /// Create new object instance.
    static vmxTextInput* New();
    
    /// Get origin (position) of the whole object.
    int GetOrigin(int &origin1, int &origin2);
    
    /// Get the size of the whole object (all actors together).
    int GetSize(int &output_size1, int &output_size2);
    
    /// Get the size of the menu text actor.
    int GetSizeOfTextActor(int &output_size1, int &output_size2);
    
    /// Given the input positions, check if the object is picked (if the position falls within the menu).
    int IsPicked(int pos1, int pos2);
    
    /// Based on existing placement preference, repositions the object.
    /// To be used in case window changes size.
    void RedoPlacement();
    
//    /// Set activity indicator.
//    void SetActive(int is_active) { m_is_active = is_active; };
    
    /// Set the description text that precedes the input
    void SetDescriptionText(const char *description) { m_description.Assign(description); };
    
    /// Set size of font in the object.
    void SetFontSize(double font_size);
    
    /// Set pointer to clipboard.
    void SetClipBoard(vmxClipBoard *clip_board) { m_clip_board = clip_board; };
    
    /// Set color of text in the object.
    void SetColor(double r, double g, double b);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);
    
    /// Set the origin (position) of the menu.
    void SetOrigin(int origin1, int origin2);
    
    /// Set visibility of the menu.
    void SetVisibility(int is_visible);
    
    /// Show the input text.
    int ShowInputText();
    
    
};



#endif
