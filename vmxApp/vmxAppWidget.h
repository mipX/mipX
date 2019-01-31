 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppWidget.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppWidget_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppWidget_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppWidget_EXPORTS
            #define vmxAppWidget_API __declspec(dllexport)
        #else
            #define vmxAppWidget_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppWidget_EXPORTS
            #define vmxAppWidget_API __attribute__((visibility("default")))
        #else
            #define vmxAppWidget_API
        #endif
    #endif
#endif


#ifndef vmxAppWidget_H
	#define vmxAppWidget_H


#include "mxDataObjectFactory.h"
#include "mxDataObjectTree.h"
#include "mxScopedPointer.h"

#include "vmxGUIRenderer3DTrackballCamera.h"
#include "vmxGUIWidget.h"
#include "vmxGUIListWidget.h"

class vmxAppDataListWidget;
class vmxAppMainWidget;



//-------------------------------------------------------------------------------------------


class vmxAppDataListWidgetMouseLeftClickSlot : public vmxGUIListWidgetMouseLeftClickSlot
{
public:
    vmxAppDataListWidgetMouseLeftClickSlot() {};
    virtual ~vmxAppDataListWidgetMouseLeftClickSlot() {};
    
    void Execute(vmxGUIListWidgetItem *list_widget_item, int picked_checkbox_index);
};



/// Data list widget for visual manipulation of data.

class vmxAppWidget_API vmxAppDataListWidget : public mxDataObjectTree
{
    
protected:
    
    /// List widget that will show the names of factories and objects from the list of object factories.
    vmxGUIListWidget m_list_widget;
    
    /// Updates list widget from the list of data object factories and data objects.
    void UpdateListWidget();
    
    /// Pointer to the app main widget.
    vmxAppMainWidget *m_app_main_widget;
    
public:
    
    /// Constructor.
    vmxAppDataListWidget(vmxAppMainWidget *app_main_widget);
    
    /// Destructor.
    virtual ~vmxAppDataListWidget();
    
    
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
    
    /// Get an array of data objects that are selected in the list.
    int GetSelectedDataObjects(mxArray< mxDataObject* > &selected_data_objects);
    
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



//-------------------------------------------------------------------------------------------



/// App viewer base class.

class vmxAppWidget_API vmxAppViewer : public vmxGUIBaseObject
{
    
protected:
    
    /// Array of pointers to renderers of this viewer. Each of the pointers will be linked to an actual defined (member) renderer in the child class.
    mxArray< vmxGUIRenderer* > m_renderers;
    
    /// Pointer to the main app widget.
    vmxAppMainWidget *m_main_widget;
    
    /// Class name.
    mxString m_class_name;
    
    /// Set number of renderers. Must be called before registering a new renderer.
    void SetNumberOfRenderers(unsigned int number_of_renderers) { m_renderers.SetNumberOfElements(number_of_renderers); };
    
    /// Add/register a renderer to this viewer.
    void RegisterRenderer(unsigned int renderer_index, vmxGUIRenderer *renderer);
    
    
public:
    
    /// Constructor.
    vmxAppViewer()
    {
        m_class_name.Assign("vmxAppViewer");
    };
    
    /// Destructor.
    virtual ~vmxAppViewer() {};

    /// Get the pointer to the GUI renderer. Use this if there is only one renderer.
    virtual vmxGUIRenderer* GetRenderer(int index=0) { return m_renderers[index]; };
    
    /// Get the pointer to the array of GUI renderers.
    mxArray< vmxGUIRenderer* >& GetRenderers() { return m_renderers; };
    
    /// Set up the viewer. This method needs to be re-implemented in child class.
    virtual void Setup(vmxAppMainWidget *main_widget)
    {
        cout<<m_class_name.Get_C_String()<<" called vmxAppViewer::Setup(vmxAppMainWidget*)"<<endl;
    };
    
    /// Update the display of the viewer.
    virtual void Update()
    {
        cout<<m_class_name.Get_C_String()<<" called vmxAppViewer::Update()"<<endl;
    };
    
};


//-------------------------------------------------------------------------------------------



/// App Main widget.

class vmxAppWidget_API vmxAppMainWidget : public vmxGUIMainWidget
{
    
protected:

    /// Default 3D renderer (in case there is no user-specified 3D renderer). The main app will have the 3D trackball camera renderer by default.
    mxScopedPointer< vmxGUIRenderer3DTrackballCamera > m_default_renderer_3D;
    
    /// Default data list widget (in case there is no user-specified data list widget).
    mxScopedPointer< vmxAppDataListWidget > m_default_data_list_widget;
    
    /// Pointer to the actual used data list widget. It points to either the default or the user specified data list widget.
    vmxAppDataListWidget *m_data_list_widget;

    /// Pointer to the actual used 3D renderer. It points to either the default or the user specified 3D renderer.
    vmxGUIRenderer *m_renderer_3D;

    
public:
    
    /// Constructor.
    vmxAppMainWidget();
    
    /// Destructor.
    virtual ~vmxAppMainWidget();
    
    /// Get renderer for the 3D scene.
    vmxAppDataListWidget* GetDataListWidget()
    {
        return m_data_list_widget;
    };
    
    /// Get renderer for the 3D scene.
    vmxGUIRenderer* GetRenderer_3D()
    {
        return m_renderer_3D;
    };
    
    /// Register a viewer in the main app.
    void RegisterViewer(vmxAppViewer *viewer);
    
    /// Use default data list widget (there will not be a user specified one).
    void SetDataListWidgetToDefault();

    /// Use default 3D renderer with trackball camera interaction list widget (there will not be a user specified one).
    void SetRenderer3DToDefaultTrackballCamera();

    
};



#endif
