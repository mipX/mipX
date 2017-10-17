 /*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIObject.h
 
  Authors: Danilo Babin.
  Copyright (c) Danilo Babin.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxGUIObject_USE_SOURCE_CODE) || defined(vmxGUI_USE_SOURCE_CODE)
    #define vmxGUIObject_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxGUIObject_EXPORTS
            #define vmxGUIObject_API __declspec(dllexport)
        #else
            #define vmxGUIObject_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxGUIObject_EXPORTS
            #define vmxGUIObject_API __attribute__((visibility("default")))
        #else
            #define vmxGUIObject_API
        #endif
    #endif
#endif


#ifndef vmxGUIObject_H
	#define vmxGUIObject_H



#include "mxString.h"
#include "vmxGUIClipBoard.h"
#include "vmxGUIInteractorStyleTrackballCamera.h"

#include <iostream>

#include <vtkRenderWindow.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>




// Pre-declarations.
//class vmxGUIObjectContainer;
class vmxGUIMainWidget;



/// Object class as parent of all other vtk level GUI classes.

class vmxGUIObject_API vmxGUIObject
{
    
public:
    
    /// Predefined placement positions for object.
    enum vmxGUIObjectPlacement
    {
        RELATIVE, //'relative' means relative to window size in percentages.
        FIXED, //'fixed' means positioned to given absolute coordinates.
        LOWER_LEFT,
        LOWER_CENTER,
        LOWER_RIGHT,
        CENTER_LEFT,
        CENTER_CENTER,
        CENTER_RIGHT,
        UPPER_LEFT,
        UPPER_CENTER,
        UPPER_RIGHT,
    };
    
protected:
    
    ///// Container of the object.
    //vmxGUIObjectContainer *m_container;
    
    /// Main widget to which this object belongs.
    vmxGUIMainWidget *m_main_widget;
    
    /// Pointer to clipboard this object uses.
    vmxGUIClipBoard *m_clip_board;
    
    /// Class (type) unique name string.
    mxString m_class_name;//static mxString m_class_name;
    
    /// Use this method in constructor of child classes to set their class name.
    void SetClassName(const char *class_name);
    
    /// Position (placement) of the list_widget. FREE is default.
    vmxGUIObjectPlacement m_placement;
    
    /// For relative placement, the percentages are stored here.
    unsigned int m_placement_relative_percentages[2];
    
    /// Indicates if the object stretches over the whole X axis.
    int m_is_stretching_over_x_axis;
    
    /// Used to define the exact position of the object based on its placement.
    /// It will be used when the object needs to be re-positioned after rescaling (e.g. after scroll event),
    /// but in case when the RedoPlacemet does not need to be called (placement is not changed). These values
    /// are in fact: (origin_x, origin_x+size_x, origin_y, origin_y+size_y).
    int m_object_bounds[4];
    
    /// Maximum available size for this object on the screen and consequently the maximum size that
    /// this object can have. Important for scalable-size objects to determine their limits of scaling.
    int m_maximum_size[2];

    
public:
    

	/// Constructor.
	vmxGUIObject();

	/// Destructor.
	virtual ~vmxGUIObject();
    
    /// Copy members of input object.
    /// Notice: No need to copy class names, as they are the same for each object of the same class.
    virtual int CopyFrom(vmxGUIObject *object);
    
    /// Get class (type) unique name string.
    mxString GetClassName();//static mxString GetClassName();
    
//    /// Get the available extent in LOCAL coordinates of the container.
//    /// The extent is [x_min, x_max] and [y_min, y_max].
//    void GetContainerAvailableExtent(int &x_min, int &x_max, int &y_min, int &y_max);
    
    /// Get the pointer to the main widget.
    vmxGUIMainWidget* GetMainWidget() { return m_main_widget; };
    
    /// Get the pointer to the clipboard this object uses.
    vmxGUIClipBoard* GetClipBoard() { return m_clip_board; };
    
    /// Get origin of the object in absolute coordinates on the render window.
    /// These are absolute (actual) positions on the screen.
    virtual void GetOrigin(int &x_origin, int &y_origin) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::GetOrigin()!"<<endl; };
    
    /// Get the placement type for this object.
    vmxGUIObject::vmxGUIObjectPlacement GetPlacement();
    
    /// Get size of the object.
    virtual void GetSize(int &x_size, int &y_size) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::GetSize()!"<<endl; };
    
    /// Given the input positions, check if the object is picked (if the position falls within the object bounds).
    int IsPicked(int pos1, int pos2);
    
    /// Get visibility of this object.
    virtual int IsVisible() { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::IsVisible()!"<<endl; return 0; };
    
    /// Check if the object stretches over the whole X axis.
    int IsStretchingOver_X_Axis();
    
    /// Reset the object.
    virtual void Reset() { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::Reset()!"<<endl; };
    
    /// Based on existing placement preference, repositions the object.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Method to be called when the object changes size, but not it's placement.
    /// E.g. a list becomes smaller (e.g. due to scrolling) but remains placed at its previous placement (e.g. lower right corner).
    /// The repositioning will be done based on the saved values of m_position_of_upper_right_corner.
    void RepositionAfterResizing();
    
    /// Set the pointer to the main widget.
    void SetMainWidget(vmxGUIMainWidget *main_widget);
    
    /// Set the maximum size of the object.
    virtual void SetMaximumSize(int max_size_x, int max_size_y)
    {
        // // Should check if the new size is different than the old one...
        // if(max_size_x!=m_maximum_size[0] || max_size_y!=m_maximum_size[1])
        // {
        //      m_maximum_size[0] = max_size_x; m_maximum_size[1] = max_size_y;
        //      // now perform resizing of the object.
        // }
        cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::SetMaximumSize()!"<<endl;
    };
    
    /// Set interactor of the object.
    virtual void SetInteractor(vtkRenderWindowInteractor *interactor) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::SetInteractor()!"<<endl; };

    /// Set origin of the object on the screen.
    /// These are absolute (actual) positions on the screen.
    virtual void SetOrigin(int x_origin, int y_origin) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::SetOrigin()!"<<endl; };
    
    /// Place the list_widget relative to the size of the render window.
    void SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent);
    
    /// Place the list_widget in the cernter left side of the render window.
    void SetPlacementToCenterCenter();
    
    /// Place the list_widget in the cernter left side of the render window.
    void SetPlacementToCenterLeft();
    
    /// Place the list_widget in the cernter right side of the render window.
    void SetPlacementToCenterRight();
    
    /// Place the list_widget in the lower center of the render window.
    void SetPlacementToLowerCenter();
    
    /// Place the list_widget in the lower left corner of the render window.
    void SetPlacementToLowerLeft();
    
    /// Place the list_widget in the lower right corner of the render window.
    void SetPlacementToLowerRight();
    
    /// Place the list_widget in the upper center of the render window.
    void SetPlacementToUpperCenter();
    
    /// Place the list_widget in the upper left corner of the render window.
    void SetPlacementToUpperLeft();
    
    /// Place the list_widget in the upper right corner of the render window.
    void SetPlacementToUpperRight();
    
    /// Set the indicator showing if the object stretches over the whole X axis.
    void SetStretchingOver_X_Axis(int is_stretching_over_x_axis);
    
    /// Set visibility of the object. This method should be re-implemented
    /// in child class, but should call RedoPlacement() mehod within it!
    virtual void SetVisibility(int is_visible) { cout<<this->m_class_name.Get_C_String()<<" called vmxGUIObject::SetVisibility()!"<<endl; };
};


//
////----------------------------------------------------------------------------------------------------------------------
//
//
//
//class vmxGUIObject_API vmxGUIObjectContainer : public vmxGUIObject
//{
//    
//public:
////    
////    /// Predefined placement positions for object.
////    enum vmxGUIObjectContainerSizeType
////    {
////        FIXED, //'fixed' means size of the container is given by the user.
////        FIT_CONTENT, // means the size is adjusted when a new object is added.
////    };
//    
//protected:
//    
//    /// List of contained objects.
//    mxList<vmxGUIObject*> m_objects;
//    
////    /// Size type for this container.
////    vmxGUIObjectContainerSizeType m_size_type;
//    
//    /// Available extent of x values, sequentially [x_min,x_max].
//    int m_x_extent[2];
//    
//    /// Available extent of y values, sequentially [y_min,y_max].
//    int m_y_extent[2];
//    
//    
//public:
//    
//    
//    /// Constructor.
//    vmxGUIObjectContainer()
//    {
//        m_class_name.Assign("vmxGUIObjectContainer");
////        m_size_type = vmxGUIObjectContainer::FIT_CONTENT;
//    };
//    
//    /// Destructor.
//    virtual ~vmxGUIObjectContainer()
//    {
//        this->Reset();
//    };
//    
//    
//    /// Get the available extent in LOCAL coordinates of the container.
//    /// The extent is [x_min, x_max] and [y_min, y_max].
//    void GetContainerAvailableExtent(int &x_min, int &x_max, int &y_min, int &y_max)
//    {
//        
//    };
//    
//    
//    /// Get origin of the container on the screen. These are absolute (actual) positions on the screen.
//    void GetOriginAbsolute(int &x_origin, int &y_origin);
//    
//    /// Get origin of the object within the container.
//    /// Notice: this is NOT the absolute origin on the screen, only the relative origin inside the container.
//    virtual void GetOriginRelative(int &x_origin, int &y_origin) { };
//    
//    /// Get size of the container. The size is adjusted when a new object is added to fit content.
//    virtual void GetSize(int &x_size, int &y_size);
//    
//    /// Reset the object.
//    virtual void Reset()
//    {
//        // should call Reset() on all containing objects.
//        mxListIterator<vmxGUIObject*> it;
//        for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
//        {
//            it.GetElement()->Reset();
//        }
//    };
//    
//    /// Based on existing placement preference, repositions the object.
//    /// To be used in case window changes size.
//    void RedoPlacement()
//    {
//        // should call RedoPlacement() on all containing objects.
//        mxListIterator<vmxGUIObject*> it;
//        for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
//        {
//            it.GetElement()->RedoPlacement();
//        }
//    };
//    
//    /// Set the relative origin (position) of the object. This is the position inside the container.
//    /// Notice:  this is NOT the absolute position of the obejct, i.e. NOT the actual position on the screen.
//    void SetOriginRelative(int origin1, int origin2);
//    
//
//
//    
//    
//};




//----------------------------------------------------------------------------------------------------------------------




class vmxGUIObject_API vmxGUIMainWidgetRenderWindowModifiedCallback : public vtkCommand
{
    
public:
    
    /// Pointer to text input that uses this callback.
    vmxGUIMainWidget *m_widget;
    
    int m_previous_window_size[2];
    
    /// Constructor.
    vmxGUIMainWidgetRenderWindowModifiedCallback();
    
    /// Destructor.
    ~vmxGUIMainWidgetRenderWindowModifiedCallback();
    
    /// Create new object instance.
    static vmxGUIMainWidgetRenderWindowModifiedCallback* New();
    
    /// Method that executes when the callback is called.
    virtual void Execute(vtkObject *caller, unsigned long, void *);
};




//----------------------------------------------------------------------------------------------------------------------




class vmxGUIObject_API vmxGUIMainWidget
{

protected:

    /// Class (type) unique name string.
    mxString m_class_name;//static mxString m_class_name;
    
    /// Pointer to the containing render window. We keep a separate pointer to allow a possibility of use of external render window.
    vtkRenderWindow *m_render_window;
    
    /// Available extent of x values, sequentially [x_min,x_max] for the left side of the main widget.
    int m_left_x_extent[2];

    /// Available extent of y values, sequentially [y_min,y_max] for the left side of the main widget.
    int m_left_y_extent[2];

    /// Available extent of x values, sequentially [x_min,x_max] for the center of the main widget.
    int m_center_x_extent[2];
    
    /// Available extent of y values, sequentially [y_min,y_max] for the center of the main widget.
    int m_center_y_extent[2];

    /// Available extent of x values, sequentially [x_min,x_max] for the right side of the main widget.
    int m_right_x_extent[2];
    
    /// Available extent of y values, sequentially [y_min,y_max] for the right side of the main widget.
    int m_right_y_extent[2];

    /// Spacing between objects.
    int m_spacing;
    
    /// Callback regulating the positioning of objects when the render window is resized.
    vtkSmartPointer<vmxGUIMainWidgetRenderWindowModifiedCallback> m_window_modified_callback;

    /// Renderer that will contain GUI objects.
    vtkSmartPointer<vtkRenderer> m_renderer_GUI;

    /// Renderer that will contain 3D scene.
    vtkSmartPointer<vtkRenderer> m_renderer_3D;
    
// THERE SHOULD BE A LIST OF USER DEFINED RENDERERS HERE.... or maybe not, renderers are maintained in the render window, so they can be accessed.
  

    /// Render window that contains all the renderers and the interactor.
    vtkSmartPointer<vtkRenderWindow> m_render_window_internal; //Maybe the render window should not be included here, but externally. To be determined.

    /// Render window interactor associated with the render window.
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    
    /// Style assigned to the interactor.
    vtkSmartPointer<vmxGUIInteractorStyleTrackballCamera> m_interactor_style;
    
    /// Clipboard owned by the main widget.
    vmxGUIClipBoard m_clipboard;
    
public:
    
    /// List of contained objects.
    mxList<vmxGUIObject*> m_objects;

    
    
    /// Constructor.
    vmxGUIMainWidget();
    
    /// Destructor.
    virtual ~vmxGUIMainWidget();
    
    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForLeftSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);

    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForCenter(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);

    /// Get the available extent in absolute coordinates of the screen. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side). The extent is [x_min, x_max] and [y_min, y_max].
    void GetAvailableExtentForRightSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis);
    
    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForLeftSide(int &x_size, int &y_size, int is_stretching_over_x_axis);
    
    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForCenter(int &x_size, int &y_size, int is_stretching_over_x_axis);

    /// Get available size of the widget. 'is_stretching_over_x_axis' indicates if the object for which we
    /// are checking the extent is stretching over all x axis (left, center and right side).
    void GetAvailableSizeForRightSide(int &x_size, int &y_size, int is_stretching_over_x_axis);

    /// Get pointer to the clipboard.
    vmxGUIClipBoard* GetClipBoard() { return &m_clipboard; };
    
    /// Get pointer to the interactor.
    vtkRenderWindowInteractor* GetInteractor() { return m_interactor; };

    /// Get pointer to the renderer of GUI.
    vtkRenderer* GetRenderer_GUI() { return m_renderer_GUI; };
    
    /// Get pointer to the 3D renderer.
    vtkRenderer* GetRenderer_3D() { return m_renderer_3D; };

    /// Get pointer to the render window.
    vtkRenderWindow* GetRenderWindow();
    
    /// Get the size of the render window. If no render window is attached return fail 0.
    int GetRenderWindowSize(int &x_size, int &y_size);
    
    /// Based on existing placement preference, repositions the object.
    /// To be used in case window changes size.
    void RedoPlacement();
    
    /// Reset the object.
    void Reset();
    
    /// Set the render window.
    void SetRenderWindow(vtkRenderWindow *render_window);
    
    /// Render and start the interaction.
    void StartInteraction()
    {
        this->GetRenderWindow()->Render();
        this->GetInteractor()->Start();
    }
    
    
};





#endif