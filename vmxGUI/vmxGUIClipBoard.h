/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIClipBoard.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxGUIClipBoard_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIClipBoard_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIClipBoard_EXPORTS
            #define vmxGUIClipBoard_API __declspec(dllexport)
        #else
            #define vmxGUIClipBoard_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIClipBoard_EXPORTS
            #define vmxGUIClipBoard_API __attribute__((visibility("default")))
        #else
            #define vmxGUIClipBoard_API
        #endif
    #endif
#endif



#ifndef vmxGUIClipBoard_H
	#define vmxGUIClipBoard_H



#include <chrono>
#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"


#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>



/// Clipboard class used to store data for copy-pasting.

class vmxGUIClipBoard_API vmxGUIClipBoard
{
    
public:
    
    /// Indicates if the copying to the clipboard has occured, so data is up-to-date.
    int m_is_valid;
    
    /// Text data copied from an object.
    mxString m_data_text;
    
    /// Address of an object that performed the last copying to clipboard.
    void* m_owner_of_data;
    
    ///Constructor.
    vmxGUIClipBoard(){};
    
    /// Destructor.
    ~vmxGUIClipBoard(){};
};




#endif
