 /*=========================================================================
 
 Program:   mipx
 Module:    mxFunctionObject.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
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



#include "mxObject.h"
#include "mxString.h"
#include <iostream>


//Pre-declaration
class mxFunctionObject;


/// Progress indicator class used in mxFunctionObject class to track the progress of method execution.
/// A child class will have the visual implementation (e.g. progress bar) of values from this class.

class mxFunctionObject_API mxFunctionObjectProgressIndicator
{
    
protected:
    
    /// Current progress value in range [m_minimum_value, m_maximum_value].
    int m_current_value;
    
    /// Function object currently connected (attached) to the progress indicator.
    mxFunctionObject *m_function_object;
    
    /// Maximum value of progress indicator.
    int m_maximum_value;
    
    /// Minimum value of progress indicator.
    int m_minimum_value;
    
    /// mxFunctionObject class is a friend class (has access to protected members).
    friend class mxFunctionObject;
    
    /// Method that if a FRIEND to mxFunctionObject and sets its pointer to mxFunctionObjectProgressIndicator to NULL.
    /// Called in destructors to break the connection between objects.
    void DetachFromObject();
    
public:
    
    /// Constructor.
    mxFunctionObjectProgressIndicator();
    
    /// Destructor.
    ~mxFunctionObjectProgressIndicator();
    
    /// Get address of the function object currently connected (attached) to this progress indicator.
    mxFunctionObject* GetFunctionObject();
    
    /// Get maximum value of the progress indicator.
    int GetMaximumValue();
    
    /// Get minimum value of the progress indicator.
    int GetMinimumValue();
    
    /// Get current value.
    int GetValue();
    
    /// Set the range (minimum and maximum) of the progress indicator.
    virtual void SetRange(int minimum, int maximum);
    
    /// Set the current value of the progress indicator rescaled to range m_maximum_value-m_minimum_value
    /// The true current value is calculated as: current_value = minimum_value + (relative_value/100)*(mamaximum_valuex-minimum_value).
    virtual void SetValue(int relative_value);
};



//-----------------------------------------------------------------------


/// Base class for function objects.

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
