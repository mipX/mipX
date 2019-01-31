/*=========================================================================
 
 Program:   mipx
 Module:    vmxTable.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxTable_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxTable_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxTable_EXPORTS
            #define vmxTable_API __declspec(dllexport)
        #else
            #define vmxTable_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxTable_EXPORTS
            #define vmxTable_API __attribute__((visibility("default")))
        #else
            #define vmxTable_API
        #endif
    #endif
#endif



#ifndef vmxTable_H
    #define vmxTable_H


#include "mxTable.h"
#include "mxString.h"
#include "mxDataObjectFactory.h"



/// VTK level multisheet table.

class vmxTable : public mxTable
{
    
public:
    
    /// Visibility indicator.
    int m_is_visible;
    
    /// Constructor.
    vmxTable();
    
    /// Destructor.
    virtual ~vmxTable();
    
    /// Set the visibility of the object component for VTK visualization.
    /// In this case just a single plot of a curve is shown so number of components is 1.
    void SetVisibilityOfComponent(int component_index, int is_visible) { m_is_visible = is_visible; };
    
    /// Get the visibility of the object component for VTK visualization.
    int GetVisibilityOfComponent(int component_index) { return m_is_visible; };

    /// Update visual structures for the curve after modification.
    void Update();
};



typedef mxDataObjectFactoryT<vmxTable> vmxTableFactory;






#endif
