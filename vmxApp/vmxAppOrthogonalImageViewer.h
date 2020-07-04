 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppOrthogonalImageViewer.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppOrthogonalImageViewer_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppOrthogonalImageViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppOrthogonalImageViewer_EXPORTS
            #define vmxAppOrthogonalImageViewer_API __declspec(dllexport)
        #else
            #define vmxAppOrthogonalImageViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppOrthogonalImageViewer_EXPORTS
            #define vmxAppOrthogonalImageViewer_API __attribute__((visibility("default")))
        #else
            #define vmxAppOrthogonalImageViewer_API
        #endif
    #endif
#endif


#ifndef vmxAppOrthogonalImageViewer_H
	#define vmxAppOrthogonalImageViewer_H


#include "mxScopedPointer.h"
#include "vmxImage.h"
#include "vmxAppWidget.h"
#include "vmxGUIRendererImageViewer.h"



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




/// Viewer for orthogonal visualization of image planes (transversal, sagittal and coronal).

class vmxAppOrthogonalImageViewer_API vmxAppOrthogonalImageViewer : public vmxAppViewer
{
    
protected:
    
    mxScopedPointer< vmxGUIRendererImageViewer > m_renderer_TSC[3];
    
public:
    
    /// Constructor.
    vmxAppOrthogonalImageViewer()
    {
        m_class_name.Assign("vmxAppOrthogonalImageViewer");
    };
    
    /// Destructor.
    virtual ~vmxAppOrthogonalImageViewer() {};
    
    /// Get the pointer to the GUI renderer.
    vmxGUIRendererImageViewer* GetRenderer(int index=0) { return m_renderer_TSC[index]; };
    
    /// Metod to add curve (signal) to the viewer.
    void SetInputImage(vmxImage *image);
    
    /// Set up the viewer.
    void Setup(vmxAppMainWidget *main_widget);
    
    /// Update display of the viewer.
    void Update();
        
    /// Slot that will be executed when the viewer need to be updated.
    static int Slot_UpdateViewer(vmxGUIConnection *connection);

};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif



#endif
