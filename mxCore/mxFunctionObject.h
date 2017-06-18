 /*=========================================================================
 
 Program:   mipx
 Module:    mxFunctionObject.h
 
  Authors: Danilo Babin.
  Copyright (c) Danilo Babin.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(mxFunctionObject_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxFunctionObject_API
#else
    #if defined(_MSC_VER)
        #ifdef mxFunctionObject_EXPORTS
            #define mxFunctionObject_API __declspec(dllexport)
        #else
            #define mxFunctionObject_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxFunctionObject_EXPORTS
            #define mxFunctionObject_API __attribute__((visibility("default")))
        #else
            #define mxFunctionObject_API
        #endif
    #endif
#endif


#ifndef mxFunctionObject_H
	#define mxFunctionObject_H



#include "mxFunctionObjectProgressIndicator.h"
#include "mxObject.h"
#include "mxString.h"
#include <iostream>



class mxFunctionObject_API mxFunctionObject : public mxObject
{
    
protected:
    
    /// Indicates that the method was aborted by user and should stop execution as soon as possible.
    int m_is_abort_initiated;
    
    /// Progress indicator object to track progress of method execution.
    mxFunctionObjectProgressIndicator *m_progress_indicator;
    
    /// Friend method for detaching the profress indicator.
    friend void mxFunctionObjectProgressIndicator::DetachFromObject();
    
public:
    
    /// Constructor.
    mxFunctionObject();
    
    /// Destructor.
    ~mxFunctionObject();
    
    /// Attach a progress indicator object to track execution progress.
    int AttachProgressIndicator(mxFunctionObjectProgressIndicator *progress_indicator);
    
    /// Detach the progress indicator from this object (e.g. when method finished execution or was aborted).
    int DetachProgressIndicator();
    
    /// Get address of attached progress indicator.  If none return NULL.
    mxFunctionObjectProgressIndicator* GetProgressIndicator();
    
    /// Check if the method was aborted by user and should stop execution as soon as possible.
    int IsAbortInitiated();
    
    /// Reset the object.
    virtual void Reset();
    
    /// Set the "abort" indicates showing that the method should stop execution as soon as possible.
    void SetAbortInitiated(int is_abort_initiated);
    
    /// Set range of values of attached progress indicator.
    virtual void SetProgressIndicatorRange(int min, int max);
    
    /// Set current value of attached progress indicator. Value must be in range [0,100].
    virtual void SetProgressIndicatorValue(int value);
};



#endif