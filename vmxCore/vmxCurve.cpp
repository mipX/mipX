/*=========================================================================
 
 Program:   mipx
 Module:    vmxCurve.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxCurve.h"



mxArray< vmxCurveColor > vmxCurve::m_colors;
int vmxCurve::m_color_index = 0;



vmxCurve::vmxCurve()
{
    m_class_name.Assign("vmxCurve");
    
    m_number_of_visualization_components = 1;
    
    if(m_colors.IsEmpty())
    {
        m_color_index = 0;
        
        m_colors.SetNumberOfElements(10);
        int i=0;
       
        m_colors[i].m_red = 255; m_colors[i].m_green = 0; m_colors[i].m_blue = 0; i++;
        m_colors[i].m_red = 0; m_colors[i].m_green = 255; m_colors[i].m_blue = 0; i++;
        m_colors[i].m_red = 0; m_colors[i].m_green = 0; m_colors[i].m_blue = 255; i++;
        m_colors[i].m_red = 255; m_colors[i].m_green = 0; m_colors[i].m_blue = 255; i++;
        m_colors[i].m_red = 0; m_colors[i].m_green = 255; m_colors[i].m_blue = 255; i++;
        m_colors[i].m_red = 128; m_colors[i].m_green = 0; m_colors[i].m_blue = 128; i++;
        m_colors[i].m_red = 128; m_colors[i].m_green = 0; m_colors[i].m_blue = 0; i++;
        m_colors[i].m_red = 0; m_colors[i].m_green = 128; m_colors[i].m_blue = 0; i++;
        m_colors[i].m_red = 0; m_colors[i].m_green = 128; m_colors[i].m_blue = 128; i++;
        m_colors[i].m_red = 128; m_colors[i].m_green = 128; m_colors[i].m_blue = 0; i++;
    }
    
    m_is_visible = 0;
    
    m_array_y = vtkSmartPointer<vtkDoubleArray>::New();
    m_array_x = vtkSmartPointer<vtkDoubleArray>::New();

    m_color[0] = m_colors[m_color_index].m_red;
    m_color[1] = m_colors[m_color_index].m_green;
    m_color[2] = m_colors[m_color_index].m_blue;
    m_color_index = (m_color_index+1) % m_colors.GetNumberOfElements();

}


vmxCurve::~vmxCurve()
{
}


void vmxCurve::Update()
{
    m_array_x->Initialize();
    m_array_y->Initialize();
    for(int i=0; i<(int)this->GetNumberOfSamples(); i++)
    {
        m_array_x->InsertValue(i, this->X(i));
        m_array_y->InsertValue(i, this->Y(i));
    }
}

