/*=========================================================================
 
 Program:   mipx
 Module:    mxFunctionObject.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxFunctionObject.h"



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

