/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIFilesDialog.h
 
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
 
 ToDo: SHIFT button selection...
 
 */




#if defined(vmxGUIFilesDialog_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIFilesDialog_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIFilesDialog_EXPORTS
            #define vmxGUIFilesDialog_API __declspec(dllexport)
        #else
            #define vmxGUIFilesDialog_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIFilesDialog_EXPORTS
            #define vmxGUIFilesDialog_API __attribute__((visibility("default")))
        #else
            #define vmxGUIFilesDialog_API
        #endif
    #endif
#endif




#ifndef vmxGUIFilesDialog_H
	#define vmxGUIFilesDialog_H



#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"

#include "vmxGUIWidget.h"
#include "vmxGUIButtonGroup.h"
#include "vmxGUILabel.h"
#include "vmxGUIListWidget.h"
#include "vmxGUIInputWidget.h"

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



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



// Pre-declaration.
class vmxGUIFilesDialog;






/// VTK GUI files dialog to be used to open files, save files, select existing directory...

class vmxGUIFilesDialog_API vmxGUIFilesDialog : public vmxGUIWidgetCollection
{
    
public:
    
    vmxGUISlotFunction m_slot_open_files;
    vmxGUISlotFunction m_slot_save_file;
    vmxGUISlotFunction m_slot_select_existing_folder;
    
    vmxGUIButtonGroup m_buttons;
    vmxGUIListWidget m_list_widget;
    vmxGUIInputWidget m_input_text;
    vmxGUILabel m_label;
    
    vmxGUIButtonGroupItem *m_button_OK;
    vmxGUIButtonGroupItem *m_button_Cancel;
    
    
    vmxGUIConnection m_connection_open_files;
    
    
    /// Current directory path
    mxString m_path_directory;
    
    /// File/directory selection types of a dialog.
    enum vmxGUIFilesDialogSelectionType
    {
        OPEN_FILES,
        SELECT_EXISTING_FOLDER,
        SAVE_FILE
    };
    
    /// Selection type of files/directories for the dialog. OPEN_FILES is default.
    vmxGUIFilesDialogSelectionType m_selection_type;
    



    /// Constructor.
    vmxGUIFilesDialog(vmxGUIMainWidget *main_widget);
    
    /// Destructor.
    virtual ~vmxGUIFilesDialog();
    
    /// Update the VTK actors of the dialog based on the changes of (added/removed) items.
    void BuildDialog();
    
    /// Get the path to the selected file.
    int GetSelectedFilePath(mxString &selected_file_path);
    
    /// Get the paths to the selected files.
    int GetSelectedFilePaths(mxArray<mxString> &selected_file_paths);

//    /// Get color of the dialog text.
//    void GetColor(double &r, double &g, double &b);
    
//    /// Get origin (position) of the whole dialog.
//    void GetOrigin(int &x_origin, int &y_origin);
    
//    /// Get the size of the whole dialog (all actors together).
//    void GetSize(int &x_size, int &y_size);
    
    /// Get visibility of the dialog (check if the dialog is visible).
    int IsVisible();
    
//    /// Given the input positions, check if the dialog is picked (if the position falls within the dialog).
//    int IsPicked(int pos1, int pos2);
    
    /// Callback method executed the given event.
    virtual void OnKeyPress(){};
    
    /// Callback method executed the given event.
    virtual void OnMouseMove() {};
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonUp();
    
    /// Callback method executed the given event.
    virtual void OnLeftButtonDown();
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDoubleClick();
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrag(){};
    
    /// Callback method executed for the given event.
    virtual void OnLeftButtonDrop(){};
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelForward(){};
    
    /// Callback method executed the given event.
    virtual void OnMouseWheelBackward(){};

    /// Set size of font in the dialog.
    void SetFontSize(double font_size);

    /// Set color of text in the dialog.
    void SetColor(double r, double g, double b);
    
    /// Set interactor.
    void SetInteractor(vtkRenderWindowInteractor *interactor);

//    /// Set the origin (position) of the dialog.
//    void SetOrigin(int origin1, int origin2);
//    
//    /// Set visibility of the dialog.
//    void SetVisibility(int is_visible);

    /// Start the dialog by displaying folder contents for opening/saving/selecting files and folders.
    int DisplayFolder(const char *directory_path=NULL);
    
    /// Start the dialog for opening files.
    int OpenFiles(vmxGUISlotFunction open_files_slot, vmxGUIConnection *connection, const char *directory_path=NULL);
    
    /// Start the dialog for saving a file.
    int SaveFile(const char *directory_path=NULL);

    /// Start the dialog for selecting an existing folder.
    int SelectExistingFolder(const char *directory_path=NULL);

};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
