/*=========================================================================
 
 Program:   mipx
 Module:    mxFunctionObjectProgressIndicator.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxFunctionObjectProgressIndicator.h"



void mxFunctionObjectProgressIndicator::DetachFromObject()
{
    if(this->m_function_object)
    {
        this->m_function_object->m_progress_indicator = NULL;
        this->m_function_object = NULL;
    }
}


mxFunctionObjectProgressIndicator::mxFunctionObjectProgressIndicator()
{
    this->m_current_value = 0;
    this->m_minimum_value = 0;
    this->m_maximum_value = 100;
    this->m_function_object = NULL;
}


mxFunctionObjectProgressIndicator::~mxFunctionObjectProgressIndicator()
{
    this->m_current_value = 0;
    this->m_minimum_value = 0;
    this->m_maximum_value = 100;
    if(this->m_function_object) this->DetachFromObject();
    this->m_function_object = NULL;
}


mxFunctionObject* mxFunctionObjectProgressIndicator::GetFunctionObject()
{
    return this->m_function_object;
}


int mxFunctionObjectProgressIndicator::GetMaximumValue()
{
    return m_maximum_value;
}



int mxFunctionObjectProgressIndicator::GetMinimumValue()
{
    return m_minimum_value;
}



int mxFunctionObjectProgressIndicator::GetValue()
{
    return m_current_value;
}



void mxFunctionObjectProgressIndicator::SetRange(int minimum, int maximum)
{
    this->m_minimum_value = minimum;
    this->m_maximum_value = maximum;
}


void mxFunctionObjectProgressIndicator::SetValue(int relative_value)
{
    if(relative_value<0 || relative_value>100) m_current_value = m_minimum_value;
    m_current_value = m_minimum_value + (relative_value*(m_maximum_value-m_minimum_value))/100;
}
