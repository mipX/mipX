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




/// App Main widget.

class vmxAppImageViewer_API vmxAppImageViewer : public vmxAppViewer//, public vmxGUIRendererImageViewer
{
    
protected:
    
    mxScopedPointer< vmxGUIRendererImageViewer > m_image_renderer;
    
    /// Image being visualized.
    vmxImage *m_image;
    
public:
    
    /// Constructor.
    vmxAppImageViewer()
    {
        m_class_name.Assign("vmxAppImageViewer");
        m_image = NULL;
    };
    
    /// Destructor.
    virtual ~vmxAppImageViewer() {};
    
    /// Get image.
    vmxImage** GetImage() { return &m_image; };
    
    /// Get the pointer to the GUI renderer.
    vmxGUIRendererImageViewer* GetRenderer(int index=0) { return m_image_renderer; };
    
    /// Set input image for the viewer.
    void SetInputImage(vmxImage *image);

    /// Set overlayed image for the viewer.
    void SetOverlayedImage(vmxImage *image);
    
    /// Set up the viewer.
    void Setup(vmxAppMainWidget *main_widget);
    
    /// Update display of the viewer.
    void Update();
    
//    /// Remove all signals from the viewer.
//    void RemoveAllSignals();
    
    /// Slot that will be executed when the viewer need to be updated.
    static int Slot_UpdateViewer(vmxGUIConnection *connection);

};



#endif
