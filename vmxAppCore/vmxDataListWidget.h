 /*=========================================================================
 
 Program:   mipx
 Module:    vmxDataListWidget.h
 
  Authors: Danilo Babin.
  Copyright (c) Danilo Babin.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxDataListWidget_USE_SOURCE_CODE) || defined(vmxAppCore_USE_SOURCE_CODE)
    #define vmxDataListWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxDataListWidget_EXPORTS
            #define vmxDataListWidget_API __declspec(dllexport)
        #else
            #define vmxDataListWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxDataListWidget_EXPORTS
            #define vmxDataListWidget_API __attribute__((visibility("default")))
        #else
            #define vmxDataListWidget_API
        #endif
    #endif
#endif


#ifndef vmxDataListWidget_H
	#define vmxDataListWidget_H


#include "mxDataObjectFactory.h"
#include "mxDataObjectTree.h"
#include "mxString.h"
#include "vmxGUIListWidget.h"
#include <iostream>




class vmxDataListWidgetMouseLftClickSlot : public vmxGUIListWidgetMouseLeftClickSlot
{
public:
    vmxDataListWidgetMouseLftClickSlot() {};
    virtual ~vmxDataListWidgetMouseLftClickSlot() {};
    
    void Execute(vmxGUIListWidgetItem *list_widget_item, int picked_checkbox_index);
};



/// Data list class for visual manipulation of data.

class vmxDataListWidget_API vmxDataListWidget : public mxDataObjectTree
{
    
protected:
        
    /// List widget that will show the names of factories and objects from the list of object factories.
    vmxGUIListWidget m_list_widget;
    
    /// Updates list widget from the list of data object factories and data objects.
    void UpdateListWidget();
    
    
public:
    
    /// Constructor.
    vmxDataListWidget();

    /// Destructor.
    virtual ~vmxDataListWidget();

    
    /// Add existing data object to the data tree.
    int AddExistingObject(mxDataObject* data_object);
    
//    /// Fails to add if the factory with the same class object name already exists.
//    int AddFactory(mxDataObjectFactory *factory);
    
//    /// Check is the input desired name is available (unique) in the list.
//    /// The return value is the closest (equal or most similar) unique name.
//    mxString CheckObjectName(mxString desired_name);
    
    /// Create an instance of a data object for the given input class/type name. Fail returns NULL.
    mxDataObject* Create(const char *class_or_type_name, const char *data_object_name = NULL);
    
//    /// Get data object by its unique data object name. Fail returns NULL.
//    mxDataObject* GetDataObject(const char *data_object_name);
    
    /// Get the list widget.
    vmxGUIListWidget* GetListWidget() { return &m_list_widget; };
    
    /// Load/create multiple instances of data objects (it can load multiple classes). Returns the list of
    /// pointers to loaded objects.
    mxList<mxDataObject*> Load(const char *file_names, const char *loading_class_name = NULL);
    
    /// Release an instance of a data object.
    void Release(mxDataObject* data_object);
    
    /// Release all data objects from the tree. Factories remain intact.
    void ReleaseAllDataObjects();
    
//    /// Rename the data object that is in the data tree.
//    void RenameDataObject(mxDataObject* data_object, const char *desired_name);
    
};



#endif