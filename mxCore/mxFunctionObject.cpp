/*=========================================================================
 
 Program:   mipx
 Module:    mxFunctionObject.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxFunctionObject.h"



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


//-----------------------------------------------------------------------


mxFunctionObject::mxFunctionObject()
{
    this->m_progress_indicator = NULL;
    this->m_is_abort_initiated = 0;
}


mxFunctionObject::~mxFunctionObject()
{
	this->Reset();
}


int mxFunctionObject::AttachProgressIndicator(mxFunctionObjectProgressIndicator *progress_indicator)
{
    if(!progress_indicator) return 0;
    
    if(progress_indicator->GetFunctionObject() != NULL)
    {
        // if the progress indicator is in use by another object, deny attachment and return 0...
        if(progress_indicator->GetFunctionObject() != this) { return 0; }
        // if the progress indicator is already attached to this object report success.
        else { return 1; }
    }
    else
    {
        // if the progress indicator is not in use, attach it to this function object.
        progress_indicator->m_function_object = this;
        this->m_progress_indicator = progress_indicator;
        return 1;
    }
}


int mxFunctionObject::DetachProgressIndicator()
{
    if(!this->m_progress_indicator) return 1;
    
    if(this->m_progress_indicator->GetFunctionObject() != NULL)
    {
        // if the progress indicator is in use by another object, deny attachment and return 0...
        if(this->m_progress_indicator->GetFunctionObject() != this) { return 0; }
        // if the progress indicator is attached to this object, detach it.
        else
        {
            this->m_progress_indicator->m_function_object = NULL;
            this->m_progress_indicator = NULL;
            return 1;
        }
    }
    // if the progress indicator is not in use by any object, return success.
    else { return 1; }
}


mxFunctionObjectProgressIndicator* mxFunctionObject::GetProgressIndicator()
{
    return this->m_progress_indicator;
}


int mxFunctionObject::IsAbortInitiated()
{
    return this->m_is_abort_initiated;
}


void mxFunctionObject::Reset()
{
    mxObject::Reset();
    this->m_progress_indicator = NULL;
    this->m_is_abort_initiated = 0;
    this->DetachProgressIndicator();
}


void mxFunctionObject::SetAbortInitiated(int is_abort_initiated)
{
	this->m_is_abort_initiated = is_abort_initiated;
}


void mxFunctionObject::SetProgressIndicatorRange(int min, int max)
{
    if(this->m_progress_indicator) { this->m_progress_indicator->SetRange(min,max); }
}


void mxFunctionObject::SetProgressIndicatorValue(int value)
{
    if(this->m_progress_indicator) this->m_progress_indicator->SetValue(value);
}


void mxFunctionObjectProgressIndicator::DetachFromObject()
{
    if(this->m_function_object)
    {
        this->m_function_object->m_progress_indicator = NULL;
        this->m_function_object = NULL;
    }
}

