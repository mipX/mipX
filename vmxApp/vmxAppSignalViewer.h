 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppSignalViewer.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppSignalViewer_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppSignalViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppSignalViewer_EXPORTS
            #define vmxAppSignalViewer_API __declspec(dllexport)
        #else
            #define vmxAppSignalViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppSignalViewer_EXPORTS
            #define vmxAppSignalViewer_API __attribute__((visibility("default")))
        #else
            #define vmxAppSignalViewer_API
        #endif
    #endif
#endif


#ifndef vmxAppSignalViewer_H
	#define vmxAppSignalViewer_H


#include "mxScopedPointer.h"
#include "vmxCurve.h"
#include "vmxAppWidget.h"
#include "vmxGUIRendererSignalViewer.h"



//-------------------------------------------------------------------------------------------



/// Signal viewer.

class vmxAppSignalViewer_API vmxAppSignalViewer : public vmxAppViewer
{
    
protected:
    
    mxScopedPointer< vmxGUIRendererSignalViewer > m_signal_renderer;
    
    
public:
    
    /// Constructor.
    vmxAppSignalViewer()
    {
        m_class_name.Assign("vmxAppSignalViewer");
    };
    
    /// Destructor.
    virtual ~vmxAppSignalViewer() {};
    
    /// Metod to add curve (signal) to the viewer.
    void AddInputSignal(vmxCurve *curve);
    
    /// Set up the viewer.
    void Setup(vmxAppMainWidget *main_widget);
    
    /// Update display of the viewer.
    void Update();
    
    /// Remove all signals from the viewer.
    void RemoveAllSignals();
    
    /// Slot that will be executed when the viewer needs to be updated.
    static int Slot_UpdateViewer(vmxGUIConnection *connection);

};



#endif
