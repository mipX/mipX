 /*=========================================================================
 
 Program:   mipx
 Module:    mxDataObjectFactory.h
 
  Authors: Danilo Babin.
  Copyright (c) Danilo Babin.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



//#if defined(mxDataObjectFactory_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
//    #define mxDataObjectFactory_API
//#else
//    #if defined(_MSC_VER)
//        #ifdef mxDataObjectFactory_EXPORTS
//            #define mxDataObjectFactory_API __declspec(dllexport)
//        #else
//            #define mxDataObjectFactory_API __declspec(dllimport)
//        #endif
//    #else
//        #ifdef mxDataObjectFactory_EXPORTS
//            #define mxDataObjectFactory_API __attribute__((visibility("default")))
//        #else
//            #define mxDataObjectFactory_API
//        #endif
//    #endif
//#endif


#ifndef mxDataObjectFactory_H
	#define mxDataObjectFactory_H



#include "mxDataObject.h"
#include "mxString.h"
#include <iostream>





//pre-declare class.
class mxDataObjectTree;

/// Base class of data object factory.

class mxDataObjectFactory
{
    
protected:
    
    /// Pointer to the data tree that contains this factory.
    mxDataObjectTree *m_data_object_tree;
    
    /// Maintins the list of created objects.
    mxList<mxDataObject*> m_created_objects;
    
public:
    
    /// Add existing data object to the data tree.
    /// Note: the object is NOT added with a unique name.
    int AddExistingObject(mxDataObject* data_object)
    {
        if(!data_object) return 0;
        if(data_object->GetClassName()==this->GetClassName().Get_C_String())
        {
            this->m_created_objects.AddToEnd(data_object);
            return 1;
        }
        return 0;
    };
    
    
    /// Create an instance of a data object.
    /// Note: the object is NOT created with a unique name.
    virtual mxDataObject* Create() = 0;
    
    
//    /// Load/create multiple instances of data objects of a single class.
//    virtual mxList<mxDataObject*> Load() = 0;
    
    /// Get class name of object this factory manages.
    virtual mxString GetClassName() = 0;
    
    /// Get pointer to data object list of this factory.
    mxList<mxDataObject*>* GetCreatedDataObjects()
    {
        return (&(this->m_created_objects));
    };
    
    /// Release an instance of a data object.
    virtual void Release(mxDataObject* data_object) = 0;
    
    /// Release all instances of data objects.
    virtual void ReleaseAllDataObjects() = 0;

    /// Set the pointer to the data object tree.
    void SetDataObjectTree(mxDataObjectTree *data_object_tree)
    {
        this->m_data_object_tree = data_object_tree;
    };

};



template<class T>
class mxDataObjectFactoryT : public mxDataObjectFactory
{
    
public:
    
    /// Constructor.
    mxDataObjectFactoryT();
    
    /// Destructor.
    ~mxDataObjectFactoryT();

    
    /// Create an instance of a data object.
    mxDataObject* Create();
    
    
    //    /// Load/create multiple instances of data objects of a single class.
    //    virtual mxList<mxDataObject*> Load() = 0;
    
    mxString GetClassName();
    
    /// Release an instance of a data object.
    void Release(mxDataObject* data_object);
    
    /// Release all instances of data objects.
    void ReleaseAllDataObjects();

};



template<class T>
mxDataObjectFactoryT<T>::mxDataObjectFactoryT()
{
    m_data_object_tree = NULL;
}


template<class T>
mxDataObjectFactoryT<T>::~mxDataObjectFactoryT()
{
    this->m_data_object_tree = NULL;
    this->ReleaseAllDataObjects();
}


template<class T>
mxDataObject* mxDataObjectFactoryT<T>::Create()
{
    T *t = new T;
    mxDataObject *obj = static_cast<mxDataObject*>(t);
    this->m_created_objects.AddToEnd(obj);
    return obj;
}


template<class T>
mxString mxDataObjectFactoryT<T>::GetClassName()
{
    return (T::GetClassName());//class name is accessible through class because it is a static member and method.
}


template<class T>
void mxDataObjectFactoryT<T>::Release(mxDataObject *data_object)
{
    mxListNode<mxDataObject*> *node = mxListFunctions::GetNodeWithElement(this->m_created_objects, data_object);
    if(node)
    {
        delete (static_cast<T*> (data_object));
        this->m_created_objects.DeleteNode(node);
    }
}


template<class T>
void mxDataObjectFactoryT<T>::ReleaseAllDataObjects()
{
    while(!this->m_created_objects.IsEmpty())
    {
        this->Release(this->m_created_objects.GetBeginElement());
    }
}





#endif