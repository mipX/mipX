 /*=========================================================================
 
 Program:   mipx
 Module:    mxObject.h
 
  Authors: Danilo Babin.
  Copyright (c) Danilo Babin.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(mxObject_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxObject_API
#else
    #if defined(_MSC_VER)
        #ifdef mxObject_EXPORTS
            #define mxObject_API __declspec(dllexport)
        #else
            #define mxObject_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxObject_EXPORTS
            #define mxObject_API __attribute__((visibility("default")))
        #else
            #define mxObject_API
        #endif
    #endif
#endif


#ifndef mxObject_H
	#define mxObject_H



#include "mxString.h"
#include <iostream>



//// Pre-declarations.
class mxObject;


/// Object class as parent of all other data/funcion classes.

class mxObject_API mxObject
{
    
protected:
    
    /// Class (type) unique name string (no other class should have this name - to be regulated on a higher level).
    static mxString m_class_name;

    /// Class (type) descriptive name string used to in short describe the class (e.g. "mxObject" can be described as "Object").
    /// This name in not unique, multiple classes can have the same descriptive name.
    static mxString m_class_descriptive_name;
    
    /// Object unique name string (no other object should have this name - to be regulated on a higher level).
    mxString m_object_name;
    
    /// Object descriptive name string used to in short describe the object (e.g. "mxDataObject" can be described as "Data Object").
    /// This name in not unique, multiple objects can have the same descriptive name.
    mxString m_object_descriptive_name;
    
public:

	/// Constructor.
	mxObject();

	/// Destructor.
	~mxObject();
    
    /// Copy members of input object.
    /// Notice: Do not copy object name. Depending on higher level app, it could be a unique name.
    /// Also, no need to copy class names, as they are the same for each object of the same class.
    virtual int CopyFrom(mxObject *object);
    
    /// Get class (type) unique name string (no other class should have this name - to be regulated on a higher level).
    static mxString& GetClassName();
    
    /// Get class (type) descriptive name string used to in short describe the class (e.g. "mxObject" can be described as "Object").
    /// This name in not unique, multiple classes can have the same descriptive name.
    static mxString& GetClassDescriptiveName();
    
    /// Get object unique name string (no other object should have this name - to be regulated on a higher level).
    mxString& GetObjectName();
    
    /// Get object descriptive name string used to in short describe the object (e.g. "mxDataObject" can be described as "Data Object").
    /// This name in not unique, multiple objects can have the same descriptive name.
    mxString& GetObjectDescriptiveName();
    
    /// Reset the object.
    virtual void Reset();
};



#endif