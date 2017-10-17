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



#include "vmxGUIObject.h"



//------------------------------------------------------------------------------------------------------



// Definition of the static members
//mxString vmxGUIObject::m_class_name;
//mxString vmxGUIMainWidget::m_class_name;



vmxGUIObject::vmxGUIObject()
{
    m_class_name.Assign("vmxGUIObject");
    m_is_stretching_over_x_axis = 0;
    m_object_bounds[0] = m_object_bounds[1] = m_object_bounds[2] = m_object_bounds[3] = 0;
}


vmxGUIObject::~vmxGUIObject()
{
	this->Reset();
}


int vmxGUIObject::CopyFrom(vmxGUIObject *object)
{
    if(!object) return 0;
    
    // Do not copy object name. Depending on higher level app, it could be a unique name.
    // Also, no need to copy class names, as they are the same for each object of the same class.
    
    return 1;
}


mxString vmxGUIObject::GetClassName()
{
    return m_class_name;
}


vmxGUIObject::vmxGUIObjectPlacement vmxGUIObject::GetPlacement()
{
    return m_placement;
}


int vmxGUIObject::IsPicked(int pos1, int pos2)
{
    int size[2];
    this->GetSize(size[0],size[1]);
    
    int origin[2];
    this->GetOrigin(origin[0],origin[1]);
    
    int end_pos1 = origin[0] + size[0];
    if(pos1<origin[0] || pos1>end_pos1) return 0;
    
    int end_pos2 = origin[1] + size[1];
    if(pos2<origin[1] || pos2>end_pos2) return 0;
    
    return 1;
}


int vmxGUIObject::IsStretchingOver_X_Axis()
{
    return m_is_stretching_over_x_axis;
}


void vmxGUIObject::RedoPlacement()
{
    if(m_placement==RELATIVE)
    {
        this->SetPlacementToRelative(m_placement_relative_percentages[0],m_placement_relative_percentages[1]);
        return;
    }
    if(m_placement==LOWER_LEFT)
    {
        this->SetPlacementToLowerLeft();
        return;
    }
    if(m_placement==LOWER_CENTER)
    {
        this->SetPlacementToLowerCenter();
        return;
    }
    if(m_placement==LOWER_RIGHT)
    {
        this->SetPlacementToLowerRight();
        return;
    }
    if(m_placement==CENTER_LEFT)
    {
        this->SetPlacementToCenterLeft();
        return;
    }
    if(m_placement==CENTER_CENTER)
    {
        this->SetPlacementToCenterCenter();
        return;
    }
    if(m_placement==CENTER_RIGHT)
    {
        this->SetPlacementToCenterRight();
        return;
    }
    if(m_placement==UPPER_LEFT)
    {
        this->SetPlacementToUpperLeft();
        return;
    }
    if(m_placement==UPPER_CENTER)
    {
        this->SetPlacementToUpperCenter();
        return;
    }
    if(m_placement==UPPER_RIGHT)
    {
        this->SetPlacementToUpperRight();
        return;
    }
}


void vmxGUIObject::RepositionAfterResizing()
{
    // (0,3) (1,3)
    // (0,2) (1,2)
    
    if(m_placement==LOWER_LEFT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);
        
        int new_origin_x = m_object_bounds[0];
        int new_origin_y = m_object_bounds[2];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = LOWER_LEFT;
        
        return;
    }
//    if(m_placement==LOWER_CENTER)
//    {
//        this->SetPlacementToLowerCenter();
//    }
    if(m_placement==LOWER_RIGHT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);
        
        int new_origin_x = m_object_bounds[1] - new_object_size[0];
        int new_origin_y = m_object_bounds[2];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = LOWER_RIGHT;
        
        return;
    }
//    if(m_placement==CENTER_LEFT)
//    {
//        this->SetPlacementToCenterLeft();
//    }
//    if(m_placement==CENTER_CENTER)
//    {
//        this->SetPlacementToCenterCenter();
//    }
//    if(m_placement==CENTER_RIGHT)
//    {
//        this->SetPlacementToCenterRight();
//    }
    if(m_placement==UPPER_LEFT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);
        
        int new_origin_x = m_object_bounds[0];
        int new_origin_y = m_object_bounds[3] - new_object_size[1];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = UPPER_LEFT;
        
        return;
    }
//    if(m_placement==UPPER_CENTER)
//    {
//        this->SetPlacementToUpperCenter();
//    }
    if(m_placement==UPPER_RIGHT)
    {
        int new_object_size[2];
        this->GetSize(new_object_size[0],new_object_size[1]);

        int new_origin_x = m_object_bounds[1] - new_object_size[0];
        int new_origin_y = m_object_bounds[3] - new_object_size[1];
        
        this->SetOrigin(new_origin_x,new_origin_y);
        
        m_object_bounds[0] = new_origin_x;
        m_object_bounds[1] = new_origin_x + new_object_size[0];
        m_object_bounds[2] = new_origin_y;
        m_object_bounds[3] = new_origin_y + new_object_size[1];
        
        m_placement = UPPER_RIGHT;
        
        return;
    }
}


void vmxGUIObject::SetMainWidget(vmxGUIMainWidget *main_widget)
{
    m_main_widget = main_widget;
    if(m_main_widget)
    {
        vmxGUIObject *obj = this;
        m_main_widget->m_objects.AddToEnd(obj);
        m_clip_board = main_widget->GetClipBoard();
        obj->SetInteractor(main_widget->GetInteractor());
    }
}


void vmxGUIObject::SetPlacementToRelative(unsigned int x_percent, unsigned int y_percent)
{
    m_placement_relative_percentages[0] = x_percent;
    m_placement_relative_percentages[1] = y_percent;
    
    int x_min, x_max, y_min, y_max;
    //this->GetContainerAvailableExtent(x_min, x_max, y_min, y_max);
    
    if(x_percent<34)
    {
        m_main_widget->GetAvailableExtentForLeftSide(x_min, x_max, y_min, y_max, this->IsStretchingOver_X_Axis());
    }
    else
    {
        if(x_percent<67)
        {
            m_main_widget->GetAvailableExtentForCenter(x_min, x_max, y_min, y_max, this->IsStretchingOver_X_Axis());
        }
        else
        {
            m_main_widget->GetAvailableExtentForRightSide(x_min, x_max, y_min, y_max, this->IsStretchingOver_X_Axis());
        }
    }
    
    
    cout<<this->m_class_name.Get_C_String()<<": x_min="<<x_min<<", x_max="<<x_max<<", y_min="<<y_min<<", y_max="<<y_max<<endl;

    
    int window_size[2];
    window_size[0] = x_max - x_min + 1;
    window_size[1] = y_max - y_min + 1;
    
    this->SetMaximumSize(window_size[0],window_size[1]);
    
    int origin[2];
    origin[0] = x_min + (window_size[0] * x_percent) / 100;
    origin[1] = y_min + (window_size[1] * y_percent) / 100;
    
    //cout<<"temp: origin[0]="<<origin[0]<<", origin[1]="<<origin[1]<<endl;

    
    int widget_size[2];
    this->GetSize(widget_size[0], widget_size[1]);
    
    if(origin[0]+widget_size[0]>window_size[0])
    {
        origin[0] = window_size[0] - widget_size[0] + x_min;
    }

    if(y_percent==100)
    {
        if(origin[1]+widget_size[1]>window_size[1])
        {
            origin[1] = window_size[1] - widget_size[1] + y_min;
        }
    }

    cout<<this->m_class_name.Get_C_String()<<": origin[0]="<<origin[0]<<", origin[1]="<<origin[1]<<endl;
    
    this->SetOrigin(origin[0],origin[1]);
    
    m_object_bounds[0] = origin[0];
    m_object_bounds[1] = origin[0] + widget_size[0];
    m_object_bounds[2] = origin[1];
    m_object_bounds[3] = origin[1] + widget_size[1];
    
    m_placement = RELATIVE;
}


void vmxGUIObject::SetPlacementToCenterCenter()
{
    this->SetPlacementToRelative(50,50);
    m_placement = CENTER_CENTER;
}


void vmxGUIObject::SetPlacementToCenterLeft()
{
    this->SetPlacementToRelative(0,50);
    m_placement = CENTER_LEFT;
}


void vmxGUIObject::SetPlacementToCenterRight()
{
    this->SetPlacementToRelative(100,50);
    m_placement = CENTER_RIGHT;
}


void vmxGUIObject::SetPlacementToLowerCenter()
{
    this->SetPlacementToRelative(50,0);
    m_placement = LOWER_CENTER;
}


void vmxGUIObject::SetPlacementToLowerLeft()
{
    this->SetPlacementToRelative(0,0);
    m_placement = LOWER_LEFT;
}


void vmxGUIObject::SetPlacementToLowerRight()
{
    this->SetPlacementToRelative(100,0);
    m_placement = LOWER_RIGHT;
}


void vmxGUIObject::SetPlacementToUpperCenter()
{
    this->SetPlacementToRelative(50,100);
    m_placement = UPPER_CENTER;
}


void vmxGUIObject::SetPlacementToUpperLeft()
{
    this->SetPlacementToRelative(0,100);
    m_placement = UPPER_LEFT;
}


void vmxGUIObject::SetPlacementToUpperRight()
{
    this->SetPlacementToRelative(100,100);
    m_placement = UPPER_RIGHT;
}


void vmxGUIObject::SetStretchingOver_X_Axis(int is_stretching_over_x_axis)
{
    m_is_stretching_over_x_axis = is_stretching_over_x_axis;
}


//void vmxGUIObject::SetVisibility(int is_visible)
//{
//    m_main_widget->RedoPlacement();
//}



//------------------------------------------------------------------------------------------------------



vmxGUIMainWidgetRenderWindowModifiedCallback::vmxGUIMainWidgetRenderWindowModifiedCallback()
{
    m_widget = NULL;
    m_previous_window_size[0] = m_previous_window_size[1] = 0;
}


vmxGUIMainWidgetRenderWindowModifiedCallback::~vmxGUIMainWidgetRenderWindowModifiedCallback()
{
    m_widget = NULL;
}


vmxGUIMainWidgetRenderWindowModifiedCallback* vmxGUIMainWidgetRenderWindowModifiedCallback::New()
{
    return new vmxGUIMainWidgetRenderWindowModifiedCallback;
}


void vmxGUIMainWidgetRenderWindowModifiedCallback::Execute(vtkObject *caller, unsigned long, void *)
{
    if(m_widget)
    {
        int rw_size[2];
        m_widget->GetRenderWindowSize(rw_size[0], rw_size[1]);
        if(rw_size[0]>0 && rw_size[1]>0)
        {
            if(rw_size[0]!=m_previous_window_size[0] || rw_size[1]!=m_previous_window_size[1])
            {
                m_previous_window_size[0] = rw_size[0];
                m_previous_window_size[1] = rw_size[1];
                m_widget->RedoPlacement();
            }
        }
    }
}



//------------------------------------------------------------------------------------------------------



vmxGUIMainWidget::vmxGUIMainWidget()
{
    m_class_name.Assign("vmxGUIMainWidget");
    m_render_window = NULL;
    
    
    m_renderer_GUI = vtkSmartPointer<vtkRenderer>::New();
    m_renderer_3D = vtkSmartPointer<vtkRenderer>::New();
    m_render_window_internal = vtkSmartPointer<vtkRenderWindow>::New();
    m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_interactor_style = vtkSmartPointer<vmxGUIInteractorStyleTrackballCamera>::New();
    
    m_render_window_internal->AddRenderer(m_renderer_GUI);
    m_render_window_internal->AddRenderer(m_renderer_3D);
    m_interactor->SetRenderWindow(m_render_window_internal);
    m_interactor->SetInteractorStyle(m_interactor_style);
    m_render_window_internal->SetSize(1920, 1080);
    
    
    m_renderer_GUI->SetViewport(0,0,1,1);
    m_renderer_3D->SetViewport(0,0,1,1);
    
    
    m_render_window_internal->SetNumberOfLayers(2);
    
    m_renderer_GUI->SetLayer(1);
    m_renderer_3D->SetLayer(0);
    
    
    m_spacing = 1;
    
    int window_size[2];
    this->GetRenderWindowSize(window_size[0], window_size[1]);
    m_left_x_extent[0] = 0;
    m_left_x_extent[1] = window_size[0]-1;
    m_left_y_extent[0] = 0;
    m_left_y_extent[1] = window_size[1]-1;
    m_center_x_extent[0] = 0;
    m_center_x_extent[1] = window_size[0]-1;
    m_center_y_extent[0] = 0;
    m_center_y_extent[1] = window_size[1]-1;
    m_right_x_extent[0] = 0;
    m_right_x_extent[1] = window_size[0]-1;
    m_right_y_extent[0] = 0;
    m_right_y_extent[1] = window_size[1]-1;

    
    m_window_modified_callback = vtkSmartPointer<vmxGUIMainWidgetRenderWindowModifiedCallback>::New();
    m_window_modified_callback->m_widget = this;
    m_window_modified_callback->m_previous_window_size[0] = window_size[0];
    m_window_modified_callback->m_previous_window_size[1] = window_size[1];
    
    // use the internal render window.
    this->SetRenderWindow(m_render_window_internal);
    
}


vmxGUIMainWidget::~vmxGUIMainWidget()
{
    this->Reset();
}


void vmxGUIMainWidget::GetAvailableExtentForLeftSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis)
{
    x_min = m_left_x_extent[0];
    x_max = m_left_x_extent[1];
    
    y_min = m_left_y_extent[0];
    y_max = m_left_y_extent[1];
    
    if(is_stretching_over_x_axis)
    {
        if(x_min < m_center_x_extent[0]) x_min = m_center_x_extent[0];
        if(x_min < m_right_x_extent[0]) x_min = m_right_x_extent[0];
        
        if(x_max > m_center_x_extent[1]) x_max = m_center_x_extent[1];
        if(x_max > m_right_x_extent[1]) x_max = m_right_x_extent[1];
        
        if(y_min < m_center_y_extent[0]) y_min = m_center_y_extent[0];
        if(y_min < m_right_y_extent[0]) y_min = m_right_y_extent[0];
        
        if(y_max > m_center_y_extent[1]) y_max = m_center_y_extent[1];
        if(y_max > m_right_y_extent[1]) y_max = m_right_y_extent[1];
    }
}


void vmxGUIMainWidget::GetAvailableExtentForCenter(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis)
{
    x_min = m_center_x_extent[0];
    x_max = m_center_x_extent[1];
    
    y_min = m_center_y_extent[0];
    y_max = m_center_y_extent[1];
    
    if(is_stretching_over_x_axis)
    {
        if(x_min < m_left_x_extent[0]) x_min = m_left_x_extent[0];
        if(x_min < m_right_x_extent[0]) x_min = m_right_x_extent[0];
        
        if(x_max > m_left_x_extent[1]) x_max = m_left_x_extent[1];
        if(x_max > m_right_x_extent[1]) x_max = m_right_x_extent[1];
        
        if(y_min < m_left_y_extent[0]) y_min = m_left_y_extent[0];
        if(y_min < m_right_y_extent[0]) y_min = m_right_y_extent[0];
        
        if(y_max > m_left_y_extent[1]) y_max = m_left_y_extent[1];
        if(y_max > m_right_y_extent[1]) y_max = m_right_y_extent[1];
    }
}


void vmxGUIMainWidget::GetAvailableExtentForRightSide(int &x_min, int &x_max, int &y_min, int &y_max, int is_stretching_over_x_axis)
{
    x_min = m_right_x_extent[0];
    x_max = m_right_x_extent[1];
    
    y_min = m_right_y_extent[0];
    y_max = m_right_y_extent[1];
    
    if(is_stretching_over_x_axis)
    {
        if(x_min < m_left_x_extent[0]) x_min = m_left_x_extent[0];
        if(x_min < m_center_x_extent[0]) x_min = m_center_x_extent[0];
        
        if(x_max > m_left_x_extent[1]) x_max = m_left_x_extent[1];
        if(x_max > m_center_x_extent[1]) x_max = m_center_x_extent[1];
        
        if(y_min < m_left_y_extent[0]) y_min = m_left_y_extent[0];
        if(y_min < m_center_y_extent[0]) y_min = m_center_y_extent[0];
        
        if(y_max > m_left_y_extent[1]) y_max = m_left_y_extent[1];
        if(y_max > m_center_y_extent[1]) y_max = m_center_y_extent[1];
    }
}


void vmxGUIMainWidget::GetAvailableSizeForLeftSide(int &x_size, int &y_size, int is_stretching_over_x_axis)
{
    int x_min, x_max, y_min, y_max;
    this->GetAvailableExtentForLeftSide(x_min, x_max, y_min, y_max, is_stretching_over_x_axis);
    
    x_size = x_max-x_min+1;
    y_size = y_max-y_min+1;
}


void vmxGUIMainWidget::GetAvailableSizeForCenter(int &x_size, int &y_size, int is_stretching_over_x_axis)
{
    int x_min, x_max, y_min, y_max;
    this->GetAvailableExtentForCenter(x_min, x_max, y_min, y_max, is_stretching_over_x_axis);
    
    x_size = x_max-x_min+1;
    y_size = y_max-y_min+1;
}


void vmxGUIMainWidget::GetAvailableSizeForRightSide(int &x_size, int &y_size, int is_stretching_over_x_axis)
{
    int x_min, x_max, y_min, y_max;
    this->GetAvailableExtentForRightSide(x_min, x_max, y_min, y_max, is_stretching_over_x_axis);
    
    x_size = x_max-x_min+1;
    y_size = y_max-y_min+1;
}


vtkRenderWindow* vmxGUIMainWidget::GetRenderWindow()
{
    return m_render_window;
}


int vmxGUIMainWidget::GetRenderWindowSize(int &x_size, int &y_size)
{
    if(!m_render_window)
    {
        x_size = y_size = 0;
        cout<<"vmxGUIMainWidget::GetRenderWindowSize(int&,int&): render window pointer is NULL.";
        return 0;
    }
    
    int *window_size = m_render_window->GetSize();
    x_size = window_size[0];
    y_size = window_size[1];
    return 1;
}


void vmxGUIMainWidget::RedoPlacement()
{
    int window_size[2];
    if(!this->GetRenderWindowSize(window_size[0], window_size[1])) return;
    m_left_x_extent[0] = 0;
    m_left_x_extent[1] = window_size[0]-1;
    m_left_y_extent[0] = 0;
    m_left_y_extent[1] = window_size[1]-1;
    m_center_x_extent[0] = 0;
    m_center_x_extent[1] = window_size[0]-1;
    m_center_y_extent[0] = 0;
    m_center_y_extent[1] = window_size[1]-1;
    m_right_x_extent[0] = 0;
    m_right_x_extent[1] = window_size[0]-1;
    m_right_y_extent[0] = 0;
    m_right_y_extent[1] = window_size[1]-1;
    
    // should call RedoPlacement() on all containing objects.
    mxListIterator<vmxGUIObject*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        vmxGUIObject *obj = it.GetElement();
        
        if(obj->IsVisible())
        {
            obj->RedoPlacement();
            
            // update available extent
            int obj_origin[2];
            obj->GetOrigin(obj_origin[0],obj_origin[1]);
            int obj_size[2];
            obj->GetSize(obj_size[0],obj_size[1]);
            
            if(obj->GetPlacement()==vmxGUIObject::UPPER_LEFT)
            {
                m_left_y_extent[1] = obj_origin[1]-1-m_spacing;
                
                if(obj->IsStretchingOver_X_Axis())
                {
                    m_center_y_extent[1] = m_left_y_extent[1];
                    m_right_y_extent[1] = m_left_y_extent[1];
                }
            }
            
            if(obj->GetPlacement()==vmxGUIObject::LOWER_LEFT)
            {
                m_left_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
                
                if(obj->IsStretchingOver_X_Axis())
                {
                    m_center_y_extent[0] = m_left_y_extent[0];
                    m_right_y_extent[0] = m_left_y_extent[0];
                }
            }
            
            if(obj->GetPlacement()==vmxGUIObject::UPPER_CENTER)
            {
                m_center_y_extent[1] = obj_origin[1]-1-m_spacing;
                
                if(obj->IsStretchingOver_X_Axis())
                {
                    m_left_y_extent[1] = m_center_y_extent[1];
                    m_right_y_extent[1] = m_center_y_extent[1];
                }
            }
            
            if(obj->GetPlacement()==vmxGUIObject::LOWER_CENTER)
            {
                m_center_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
                
                if(obj->IsStretchingOver_X_Axis())
                {
                    m_left_y_extent[0] = m_center_y_extent[0];
                    m_right_y_extent[0] = m_center_y_extent[0];
                }
            }
            
            if(obj->GetPlacement()==vmxGUIObject::UPPER_RIGHT)
            {
                m_right_y_extent[1] = obj_origin[1]-1-m_spacing;
                
                if(obj->IsStretchingOver_X_Axis())
                {
                    m_left_y_extent[1] = m_right_y_extent[1];
                    m_center_y_extent[1] = m_right_y_extent[1];
                }
            }
            
            if(obj->GetPlacement()==vmxGUIObject::LOWER_RIGHT)
            {
                m_right_y_extent[0] = obj_origin[1] + obj_size[1] + 1 + m_spacing;
                
                if(obj->IsStretchingOver_X_Axis())
                {
                    m_left_y_extent[0] = m_right_y_extent[0];
                    m_center_y_extent[0] = m_right_y_extent[0];
                }
            }


        }
    }
}


void vmxGUIMainWidget::Reset()
{
    // should call reset on all containing objects.
    mxListIterator<vmxGUIObject*> it;
    for(it.SetToBegin(m_objects); it.IsValid(); it.MoveToNext())
    {
        it.GetElement()->Reset();
    }
}


void vmxGUIMainWidget::SetRenderWindow(vtkRenderWindow *render_window)
{
    //if(m_render_window == render_window) return;
    
    if(m_render_window)
    {
        m_render_window->RemoveObservers(vtkCommand::ModifiedEvent, m_window_modified_callback);
    }
    
    m_render_window = render_window;
    
    if(m_render_window)
    {
        m_render_window->AddObserver(vtkCommand::ModifiedEvent, m_window_modified_callback);
    }
}


