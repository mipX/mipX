/*=========================================================================
 
 Program:   mipx
 Module:    vmxClipBoard.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxClipBoard_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxClipBoard_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxClipBoard_EXPORTS
            #define vmxClipBoard_API __declspec(dllexport)
        #else
            #define vmxClipBoard_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxClipBoard_EXPORTS
            #define vmxClipBoard_API __attribute__((visibility("default")))
        #else
            #define vmxClipBoard_API
        #endif
    #endif
#endif



#ifndef vmxClipBoard_H
	#define vmxClipBoard_H



#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)



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





class vmxClipBoard_API vmxClipBoard
{
    
public:
    
    /// Indicates if the copying to the clipboard has occured, so data is up-to-date.
    int m_is_valid;
    
    /// Text data copied from an object.
    mxString m_data_text;
    
    /// Address of an object that performed the last copying to clipboard.
    void* m_owner_of_data;
};






#endif
