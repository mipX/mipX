 /*=========================================================================
 
 Program:   mipx
 Module:    mxFunctionObjectProgressIndicator.h
 
  Authors: Danilo Babin.
  Copyright (c) Danilo Babin.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(mxFunctionObjectProgressIndicator_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxFunctionObjectProgressIndicator_API
#else
    #if defined(_MSC_VER)
        #ifdef mxFunctionObjectProgressIndicator_EXPORTS
            #define mxFunctionObjectProgressIndicator_API __declspec(dllexport)
        #else
            #define mxFunctionObjectProgressIndicator_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxFunctionObjectProgressIndicator_EXPORTS
            #define mxFunctionObjectProgressIndicator_API __attribute__((visibility("default")))
        #else
            #define mxFunctionObjectProgressIndicator_API
        #endif
    #endif
#endif


#ifndef mxFunctionObjectProgressIndicator_H
	#define mxFunctionObjectProgressIndicator_H


//#include "mxObject.h"
#include "mxString.h"
#include <iostream>



//Pre-declaration
class mxFunctionObject;


/// Progress indicator class used in mxFunctionObject class to track the progress of method execution.
/// A child class will have the visual implementation (e.g. progress bar) of values from this class.

class mxFunctionObjectProgressIndicator_API mxFunctionObjectProgressIndicator
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



#endif