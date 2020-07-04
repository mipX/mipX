/*=========================================================================
 
 Program:   mipx
 Module:    vmxProfile.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxProfile_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxProfile_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxProfile_EXPORTS
            #define vmxProfile_API __declspec(dllexport)
        #else
            #define vmxProfile_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxProfile_EXPORTS
            #define vmxProfile_API __attribute__((visibility("default")))
        #else
            #define vmxProfile_API
        #endif
    #endif
#endif



#ifndef vmxProfile_H
    #define vmxProfile_H


#include "mxProfile.h"
#include "mxString.h"
#include "mxDataObjectFactory.h"



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




/// VTK level profile structure.

class vmxProfile_API vmxProfile : public mxProfile
{
    
public:
    
    /// Visibility indicator.
    int m_is_visible;
    
    /// Constructor.
    vmxProfile();
    
    /// Destructor.
    virtual ~vmxProfile();
    
    /// Set the visibility of the object component for VTK visualization.
    /// In this case just a single plot of a curve is shown so number of components is 1.
    void SetVisibilityOfComponent(int component_index, int is_visible) { m_is_visible = is_visible; };
    
    /// Get the visibility of the object component for VTK visualization.
    int GetVisibilityOfComponent(int component_index) { return m_is_visible; };

    /// Update visual structures for the curve after modification.
    void Update();
};



typedef mxDataObjectFactoryT<vmxProfile> vmxProfileFactory;




#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
