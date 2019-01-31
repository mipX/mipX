 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppHistogramViewer.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppHistogramViewer_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppHistogramViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppHistogramViewer_EXPORTS
            #define vmxAppHistogramViewer_API __declspec(dllexport)
        #else
            #define vmxAppHistogramViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppHistogramViewer_EXPORTS
            #define vmxAppHistogramViewer_API __attribute__((visibility("default")))
        #else
            #define vmxAppHistogramViewer_API
        #endif
    #endif
#endif


#ifndef vmxAppHistogramViewer_H
	#define vmxAppHistogramViewer_H


#include "mxScopedPointer.h"
#include "vmxCurve.h"
#include "vmxImage.h"
#include "vmxAppWidget.h"
#include "vmxGUIRendererSignalViewer.h"



//-------------------------------------------------------------------------------------------



/// Signal viewer.

class vmxAppHistogramViewer_API vmxAppHistogramViewer : public vmxAppViewer
{
    
protected:
    
    /// Signal viewer.
    mxScopedPointer< vmxGUIRendererSignalViewer > m_signal_renderer;
    
    /// Curve representing the histogram for a single slice.
    vmxCurve m_histogram_for_slice;
    
    
public:
    
    /// Constructor.
    vmxAppHistogramViewer()
    {
        m_class_name.Assign("vmxAppHistogramViewer");
    };
    
    /// Destructor.
    virtual ~vmxAppHistogramViewer() {};
    
    /// Get the pointer to the GUI renderer. Use this if there is only one renderer.
    vmxGUIRendererSignalViewer* GetRenderer(int index=0) { return m_signal_renderer; };
    
    /// Set the image for which to show the histogram of the slice with indexes (t,s).
    void SetInputImage(vmxImage *image, unsigned int t, unsigned int s);
    
    /// Set up the viewer.
    void Setup(vmxAppMainWidget *main_widget);
    
    /// Update display of the viewer.
    void Update();
    
    /// Remove all signals from the viewer.
    void RemoveAllSignals();
    
    /// Slot that will be executed when the viewer need to be updated.
    /// Calls: SetInputImage(vmxImage *image, unsigned int t, unsigned int s).
    static int Slot_UpdateViewer(vmxGUIConnection *connection);

};



#endif
