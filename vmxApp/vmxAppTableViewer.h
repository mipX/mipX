 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppTableViewer.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppTableViewer_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppTableViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppTableViewer_EXPORTS
            #define vmxAppTableViewer_API __declspec(dllexport)
        #else
            #define vmxAppTableViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppTableViewer_EXPORTS
            #define vmxAppTableViewer_API __attribute__((visibility("default")))
        #else
            #define vmxAppTableViewer_API
        #endif
    #endif
#endif


#ifndef vmxAppTableViewer_H
	#define vmxAppTableViewer_H


#include "mxScopedPointer.h"
#include "vmxTable.h"
#include "vmxAppWidget.h"
#include "vmxGUIRendererTableViewer.h"



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif


/// Table viewer widget.

class vmxAppTableViewer_API vmxAppTableViewer : public vmxAppViewer
{
    
protected:
    
    mxScopedPointer< vmxGUIRendererTableViewer > m_table_renderer;
    
    /// Pointer to currently shown table.
    vmxTable *m_table;
    
public:
    
    /// Constructor.
    vmxAppTableViewer()
    {
        m_class_name.Assign("vmxAppTableViewer");
        m_table = NULL;
    };
    
    /// Destructor.
    virtual ~vmxAppTableViewer() {};
    
    /// Metod to add curve (Table) to the viewer.
    void SetInputTable(vmxTable *table);
    
    /// Set up the viewer.
    void Setup(vmxAppMainWidget *main_widget);
    
    /// Update display of the viewer.
    void Update();
    
//    /// Remove all Tables from the viewer.
//    void RemoveAllTables();
    
    /// Slot that will be executed when the viewer need to be updated.
    static int Slot_UpdateViewer(vmxGUIConnection *connection);

};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif



#endif
