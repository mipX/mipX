/*=========================================================================
 
 Program:   mipx
 Module:    vmxProfile.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxProfile.h"





vmxProfile::vmxProfile()
{
    m_class_name.Assign("vmxProfile");
    
    m_number_of_visualization_components = 0;
    
    m_is_exclusive_visibility_on = 1;
    
    m_is_visible = 0;
}


vmxProfile::~vmxProfile()
{
}


void vmxProfile::Update()
{
    
}

