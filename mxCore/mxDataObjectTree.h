 /*=========================================================================
 
 Program:   mipx
 Module:    mxDataObjectTree.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/





#if defined(mxDataObjectTree_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxDataObjectTree_API
#else
    #if defined(_MSC_VER)
        #ifdef mxDataObjectTree_EXPORTS
            #define mxDataObjectTree_API __declspec(dllexport)
        #else
            #define mxDataObjectTree_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxDataObjectTree_EXPORTS
            #define mxDataObjectTree_API __attribute__((visibility("default")))
        #else
            #define mxDataObjectTree_API
        #endif
    #endif
#endif


#ifndef mxDataObjectTree_H
	#define mxDataObjectTree_H


#include "mxDataObjectFactory.h"
#include "mxString.h"
#include <iostream>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



/// Data object tree used to handle data on mx level.

class mxDataObjectTree_API mxDataObjectTree
{
    
protected:
    
    /// List of all data object factories.
    mxList<mxDataObjectFactory*> m_data_object_factories;
    
    
public:
    
    /// Constructor.
    mxDataObjectTree();

    /// Destructor.
    virtual ~mxDataObjectTree();

    /// Add existing data object to the data tree.
    virtual int AddExistingObject(mxDataObject* data_object);
    
    /// Fails to add if the factory with the same class object name already exists.
    int AddFactory(mxDataObjectFactory *factory);
    
    /// Check is the input desired name is available (unique) in the list.
    /// The return value is the closest (equal or most similar) unique name.
    mxString CheckObjectName(mxString desired_name);
    
    /// Create an instance of a data object for the given input class/type name. Fail returns NULL.
    virtual mxDataObject* Create(const char *class_or_type_name, const char *data_object_name = NULL);
    
    /// Get data object by its unique data object name. Fail returns NULL.
    mxDataObject* GetDataObject(const char *data_object_name);

    /// Get data object by its unique data object name if it has the given class name. Fail returns NULL.
    mxDataObject* GetDataObject(const char *data_object_name, const char *class_name);
    
    /// Get data object factories.
    mxList< mxDataObjectFactory*>* GetDataObjectFactories() { return &m_data_object_factories; };

    /// Load/create multiple instances of data objects (it can load multiple classes). Returns the list of
    /// pointers to loaded objects.
    virtual mxList<mxDataObject*> Load(const char *file_names, const char *loading_class_name = NULL);
    
    /// Release an instance of a data object.
    virtual void Release(mxDataObject* data_object);
    
    /// Release all data objects from the tree. Factories remain intact.
    virtual void ReleaseAllDataObjects();
    
//    /// Rename the data object that is in the data tree.
//    virtual void RenameDataObject(mxDataObject* data_object, const char *desired_name);
//    
};


#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif


#endif