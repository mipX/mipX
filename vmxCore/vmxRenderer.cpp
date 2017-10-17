/*=========================================================================
 
 Program:   mipx
 Module:    vmxRenderer.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxRenderer.h"


vmxRenderer::vmxRenderer()
{
    m_interaction_style = vmxRenderer::TRACKBALL_3D;
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
}


vmxRenderer::~vmxRenderer()
{
}


vmxRenderer* vmxRenderer::New()
{
    return new vmxRenderer;
}
