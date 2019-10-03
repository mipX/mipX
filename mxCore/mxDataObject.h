 /*=========================================================================
 
 Program:   mipx
 Module:    mxDataObject.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(mxDataObject_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxDataObject_API
#else
    #if defined(_MSC_VER)
        #ifdef mxDataObject_EXPORTS
            #define mxDataObject_API __declspec(dllexport)
        #else
            #define mxDataObject_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxDataObject_EXPORTS
            #define mxDataObject_API __attribute__((visibility("default")))
        #else
            #define mxDataObject_API
        #endif
    #endif
#endif


#ifndef mxDataObject_H
	#define mxDataObject_H



#include "mxObject.h"
#include "mxString.h"
#include <iostream>



// Pre-declarations.
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkActor;
class qmxDataObjectContextMenu;
class qmxDataObjectPropertiesWidget;
//class qmxDataContainer;
class mxDataObjectFactory;


/// Base class for data objects.
/// Notice: Child class (inheriting from this object) will have a pointer to its context menu (that will
/// inherit from qmxDataObjectContextMenu in "qmxObject.h") and a pointer to its properties widget (that will inherit
/// from qmxDataObjectPropertiesWidget in "qmxObject.h").
/// Notice: Child class (inheriting from this object) will have a map between batch commands and actual methods
/// of the class. In this way we will be able to call the methods of each class through batch command line. Declaration
/// of mapper list will be in the following fashion: mxObjectBatchCallMapper<TypeName_Of_Pointer_To_Method>  m_batch_call_mapper;

class mxDataObject_API mxDataObject : public mxObject
{
    
protected:
    
    /// Color of the data object in RGBA order.
    unsigned char m_color[4];
    
    /// Pointer to the factory of this class object.
    mxDataObjectFactory *m_factory;
    
    /// Indicates if multiple object of this class can be displayed together.
    /// If the indicator is set, only a single object can be viewed at a time.
    /// By default the indicator is not set (multiple objects can be displayed together).
    int m_is_exclusive_visibility_on;
    
    /// Indicates if the object is in use by a function, so it should not be used for another function (hence, locked).
    /// Notice: The indicator is controlled (set/reset) on a higher level of the app.
    /// Example: The indicator will be used on Qt level to allow or refuse the appearance of the context menu.
    int m_is_locked;
    
    /// Logbook of all signatures (records/names) and parameters of functions called on this object.
    mxString m_log;
    
    /// Number of visualization components of the object (e.g. an image visualized with orthogonal planes
    /// will have 3 components: axil, sagittal and coronal plane). Default value is 1.
    unsigned int m_number_of_visualization_components;

public:
    
    /// Constructor.
    mxDataObject();
    
    /// Destructor.
    virtual ~mxDataObject();
    
    /// Copy data members of input object if the input object nad this object are of the same type.
    virtual int CopyFromDataObject(mxDataObject *data_object);
    
    /// Get bounds of the object in 3D space. If not applicable (e.g. object is a 2D signal) return fail 0.
    virtual int GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max);
    
    /// Get data object color in RGBA order.
    int GetColor(unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *a);
    
    /// Get the context menu of the object.
    virtual qmxDataObjectContextMenu* GetContextMenu();

    /// Get number of elements of S dimention of data (usually corresponds to number of "slices" of the data).
    virtual unsigned int GetDimension_S();
    
    /// Get number of elements of T dimention of data (usually corresponds to number of "time series" of the data).
    virtual unsigned int GetDimension_T();
    
    /// Get the factory of this class object.
    mxDataObjectFactory* GetFactory();

    /// Get index s (usually corresponds to slice index).
    virtual unsigned int GetIndex_S();
    
    /// Get index t (usually corresponds to time index).
    virtual unsigned int GetIndex_T();
    
    /// Get log string of all signatures (records/names) and parameters of functions called on this object.
    mxString& GetLog();
    
    /// Get the number of visualization components of this object (e.g. an image visualized with orthogonal planes
    /// will have 3 components: axil, sagittal and coronal plane).
    unsigned int GetNumberOfVisualizationComponents()
    {
        return m_number_of_visualization_components;
    };
    
    /// Get the properties widget of the object.
    virtual qmxDataObjectPropertiesWidget* GetPropertiesWidget();
    
    /// Get the visibility of the object component for VTK visualization.
    /// E.g. a 3D image has 3 visualization components: axial, sagittal and coronal planes.
    virtual int GetVisibilityOfComponent(int component_index);
    
    /// Check if the actor belongs to the object (used for calling the context menu).
    virtual int HasActor(vtkActor *actor);
    
    /// Internal method, to be used with caution!!!
    /// This method is used in Create() method of mxDataObjectFactory when creating a data object and setting
    /// its pointer to its factory. The method is virtual, so it can be overridden to do some additional stuff if needed.
    virtual void internalSetDataObjectFactory(mxDataObjectFactory *factory) { this->m_factory = factory; };
    
    /// Check if multiple objects can be displayed at the same time.
    int IsExclusiveVisibilityOn();
    
    /// Check if the object is being used by a function (so it should not be used by another function/method at the same time).
    int IsLocked();
    
    /// Check if the object is visible.
    virtual int IsVisibleIn3DView();
    
    /// Load data object from a file of given extension.
    /// Note: the "extension" does not necessarily need to be used, but is present if needed.
    virtual int LoadFromFile(const char *file_path, const char *extension);

    /// Load data object from files of given extension in a given folder.
    /// Note: the "extension" does not necessarily need to be used, but is present if needed.
    virtual int LoadFromFolder(const char *folder_path, const char *extension);
    
    /// Reset the object.
    virtual void Reset();
    
    /// Save the data object to a file of given extension.
    /// Note: the "extension" does not necessarily need to be used, but is present if needed.
    virtual int SaveToFile(const char *file_path, const char *extension);
    
    /// Save the data object to file of given extension in a given folder.
    /// Note: the "extension" does not necessarily need to be used, but is present if needed.
    virtual int SaveToFolder(const char *folder_path, const char *extension);
    
    /// Set data object color in RGBA order.
    void SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char a);
    
    /// Set exclusive visibility.
    void SetExclusiveVisibility(int is_visibility_exclusive);

    /// Set S index for the data object (usually corresponds to slice).
    virtual void SetIndex_S(unsigned int index);
    
    /// Set T index for the data object (usually corresponds to time).
    virtual void SetIndex_T(unsigned int index);
    
    /// Set the interactor for VTK visualization.
    virtual void SetInteractor(vtkRenderWindowInteractor *interactor);

    /// Lock/unlock the object. Use this to indicate that an external function is using the object. Do not forget to release the
    /// object after the efunction execution has ended.
    void SetLocked(int is_locked);
    
    /// Set the number of visualization components of this object (e.g. an image visualized with orthogonal planes
    /// will have 3 components: axil, sagittal and coronal plane).
    void SetNumberOfVisualizationComponents(unsigned int number_of_components)
    {
        if(number_of_components==0) return;
        m_number_of_visualization_components = number_of_components;
    };
    
    /// Set the renderer for VTK visualization.
    virtual void SetRenderer(vtkRenderer *renderer);
    
    /// Set the visibility of the object for VTK visualization.
    virtual void SetVisibilityIn3DView(int is_visible);
    
    /// Set the visibility of the object component for VTK visualization.
    /// E.g. a 3D image has 3 visualization components: axial, sagittal and coronal planes.
    virtual void SetVisibilityOfComponent(int component_index, int is_visible);
    
    /// Method that will show basic context menu of the object.
    virtual void ShowBasicContextMenu();
    
    /// This will show context menu of the object.
    virtual void ShowContextMenu();
    
    /// Method that will show the object properties in the properties widget.
    virtual void ShowProperties();
    
    /// Update the visual structures of object after modification.
    virtual void Update();
};



#endif
