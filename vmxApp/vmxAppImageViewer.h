 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppImageViewer.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppImageViewer_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppImageViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppImageViewer_EXPORTS
            #define vmxAppImageViewer_API __declspec(dllexport)
        #else
            #define vmxAppImageViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppImageViewer_EXPORTS
            #define vmxAppImageViewer_API __attribute__((visibility("default")))
        #else
            #define vmxAppImageViewer_API
        #endif
    #endif
#endif


#ifndef vmxAppImageViewer_H
	#define vmxAppImageViewer_H


#include "mxScopedPointer.h"
#include "vmxImage.h"
#include "vmxAppWidget.h"
#include "vmxGUIRendererImageViewer.h"
#include "vmxPointList.h"



/// App Main widget.

class vmxAppImageViewer_API vmxAppImageViewer : public vmxAppViewer
{
    
protected:
    
    mxScopedPointer< vmxGUIRendererImageViewer > m_image_renderer;
    
    /// Image being visualized.
    vmxImage *m_image;
    
public:
    
    /// command for converting seeds to point list.
    vtkTextActor *m_command_seeds_to_point_list;
    
    /// Constructor.
    vmxAppImageViewer()
    {
        m_class_name.Assign("vmxAppImageViewer");
        m_image = NULL;
    };
    
    /// Destructor.
    virtual ~vmxAppImageViewer() {};
    
    /// Disconnect the viewer from the default list widget.
    void DisconnectFromDefaultDataListWidget();

    /// Get image.
    vmxImage** GetImage() { return &m_image; };
    
    /// Get the pointer to the GUI renderer.
    vmxGUIRendererImageViewer* GetRenderer(int index=0) { return m_image_renderer; };
    
    /// Get the seed point list.
    void GetSeedPointList(mxPointList *pl);
    
    /// Set input image for the viewer.
    void SetInputImage(vmxImage *image);

    /// Set overlayed image for the viewer.
    void SetOverlayedImage(vmxImage *image);
    
    /// Set up the viewer.
    void Setup(vmxAppMainWidget *main_widget);
    
    /// Update display of the viewer.
    void Update();
        
    /// Slot that will be executed when the viewer need to be updated.
    static int Slot_UpdateViewer(vmxGUIConnection *connection);
    
    /// Slot that will be executed on left button drop - use to update viewer based on the drag&drop of image from data list.
    static int Slot_OnDrop(vmxGUIConnection *connection);
    
    /// Slot that will be executed on key press (if the evet was not caught by gui renderer image viewer).
    static int Slot_OnKeyPress(vmxGUIConnection *connection);
    
    /// Slot that will be executed on left button up event (if the evet was not caught by gui renderer image viewer).
    static int Slot_OnLeftButtonUp(vmxGUIConnection *connection);

};



#endif
