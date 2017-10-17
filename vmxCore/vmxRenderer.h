/*=========================================================================
 
 Program:   mipx
 Module:    vmxRenderer.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(vmxRenderer_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxRenderer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxRenderer_EXPORTS
            #define vmxRenderer_API __declspec(dllexport)
        #else
            #define vmxRenderer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxRenderer_EXPORTS
            #define vmxRenderer_API __attribute__((visibility("default")))
        #else
            #define vmxRenderer_API
        #endif
    #endif
#endif



#ifndef vmxRenderer_H
	#define vmxRenderer_H


#include <vtkSmartPointer.h>
#include <vtkRenderer.h>



class vmxRenderer
{
    
protected:
    
    vtkSmartPointer<vtkRenderer> m_renderer;
    
    
public:
    
    /// Predefined placement positions for menu.
    enum vmxRendererInteractionStyle
    {
        TRACKBALL_3D, // trackball camera interaction in 3D space.
        IMAGE_AXIAL, // camera always looking towards the image of axial direction.
        IMAGE_CORONAL, // camera always looking towards the image of coronal direction.
        IMAGE_SAGITTAL, // camera always looking towards the image of sagittal direction.
 
    };
    
    
    vmxRendererInteractionStyle m_interaction_style;
    
    
    vmxRenderer();
    
    ~vmxRenderer();
    
    
    /// Create new object instance.
    static vmxRenderer* New();
    
    
    
    vtkRenderer* GetVTKRenderer() { return m_renderer; };
    
    
    void SetInteractionStyle(vmxRendererInteractionStyle interaction_style) { m_interaction_style = interaction_style; };
    
    vmxRendererInteractionStyle GetInteractionStyle() { return m_interaction_style; };
    
};


#endif
