/*=========================================================================
 
 Program:   mipx
 Module:    vmxClipBoard.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(vmxClipBoard_USE_SOURCE_CODE) || defined(vmxCore_USE_SOURCE_CODE)
    #define vmxClipBoard_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxClipBoard_EXPORTS
            #define vmxClipBoard_API __declspec(dllexport)
        #else
            #define vmxClipBoard_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxClipBoard_EXPORTS
            #define vmxClipBoard_API __attribute__((visibility("default")))
        #else
            #define vmxClipBoard_API
        #endif
    #endif
#endif



#ifndef vmxClipBoard_H
	#define vmxClipBoard_H



#include <chrono>
#include <stdint.h>
#include "mxArray.h"
#include "mxString.h"


#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>





class vmxClipBoard_API vmxClipBoard
{
    
public:
    
    /// Indicates if the copying to the clipboard has occured, so data is up-to-date.
    int m_is_valid;
    
    /// Text data copied from an object.
    mxString m_data_text;
    
    /// Address of an object that performed the last copying to clipboard.
    void* m_owner_of_data;
    
    ///Constructor.
    vmxClipBoard(){};
    
    /// Destructor.
    ~vmxClipBoard(){};
};



//class vmxClipBoard_API vmxDoubleClickDetector
//{
//
//protected:
//    
//    // for detecting double click.
//    int m_number_of_clicks;
//    
//    // for detecting double click.
//    int m_left_button_down_previous_position[2];
//    
//    /// Indicator that will be set when the double click is detected.
//    int m_is_double_clicked;
//    
//    
//    using clock             = std::chrono::system_clock;
//    using time_point_type   = std::chrono::time_point < clock, std::chrono::milliseconds > ;
//    
//    /// Parameters to measure time between clicks.
//    time_point_type m_time_of_left_click_in_ms, m_time_of_left_click_in_ms2;
//
//public:
//    
//    ///Constructor.
//    vmxDoubleClickDetector()
//    {
//        m_number_of_clicks = 0;
//        m_left_button_down_previous_position[0] = m_left_button_down_previous_position[1] = 0;
//        m_is_double_clicked = 0;
//    };
//    
//    /// Destructor.
//    ~vmxDoubleClickDetector(){};
//    
//    /// For input click position, check if double click was performed.
//    void OnLeftButtonDown(int pos1, int pos2, int is_CTRL_key_down)
//    {
//        m_time_of_left_click_in_ms2 = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
//        
//        long time_diff = (m_time_of_left_click_in_ms2 - m_time_of_left_click_in_ms).count();
//        m_time_of_left_click_in_ms = m_time_of_left_click_in_ms2;
//        
//        //cout<<" time_diff="<<time_diff<<"  ";
//        
//        m_is_double_clicked = 0;
//        
//        if(is_CTRL_key_down)
//        {
//            this->m_number_of_clicks = 1;
//            m_left_button_down_previous_position[0] = pos1;
//            m_left_button_down_previous_position[1] = pos2;
//            m_is_double_clicked = 0;
//            return;
//        }
//        
//        
//        this->m_number_of_clicks++;
//        
//        
//        int xdist = pos1 - m_left_button_down_previous_position[0];
//        int ydist = pos2 - m_left_button_down_previous_position[1];
//        
//        m_left_button_down_previous_position[0] = pos1;
//        m_left_button_down_previous_position[1] = pos2;
//        
//        int move_distance_squared = (xdist*xdist + ydist*ydist);
//        
//        // Reset m_number_of_clicks if the mouse moved further than desired for double click.
//        if(move_distance_squared > 25 || time_diff>700)
//        {
//            this->m_number_of_clicks = 1;
//            m_is_double_clicked = 0;
//        }
//        
//        if(this->m_number_of_clicks == 2)
//        {
//            this->m_number_of_clicks = 0;
//            m_is_double_clicked = 1;
//            
//        }
//    };
//    
//    int IsDoubleClicked(){return m_is_double_clicked;};
//};
//
//
//
//class vmxClipBoard_API vmxMouseDragDetector
//{
//    
//protected:
//    
//    // for detecting double click.
//    int m_left_button_down_previous_position[2];
//    
//    /// Indicator that will be set when the drag is detected.
//    int m_is_dragging;
//    
//    /// Indicates if the left mouse button is down (pressed). This indicator is reset on left button up event.
//    int m_is_left_button_down;
//    
//public:
//    
//    ///Constructor.
//    vmxMouseDragDetector()
//    {
//        m_left_button_down_previous_position[0] = m_left_button_down_previous_position[1] = 0;
//        m_is_dragging = 0;
//        m_is_left_button_down = 0;
//    };
//    
//    /// Destructor.
//    ~vmxMouseDragDetector(){};
//    
//    /// To be called on left button down event.
//    void OnLeftButtonDown(int pos1, int pos2)
//    {
//        m_left_button_down_previous_position[0] = pos1;
//        m_left_button_down_previous_position[1] = pos2;
//        m_is_dragging = 0;
//        m_is_left_button_down = 1;
//    };
//    
//    /// To be called on mouse move event.
//    void OnMouseMove(int pos1, int pos2)
//    {
//        if(!m_is_left_button_down) return;
//        
//        // Get the distance between the last left button down evenr and current position of the mouse.
//        int d0 = pos1 - m_left_button_down_previous_position[0];
//        if(d0<0) d0 = -d0;
//        
//        // if the distance is larger than the bellow predefined value(s), we have detected a drag.
//        if(d0>10)
//        {
//            int d1 = pos2 - m_left_button_down_previous_position[1];
//            if(d1<0) d1 = -d1;
//            if(d1>10)//If drag is detected
//            {
//                m_is_dragging = 1;
//            }
//        }
//    };
//    
//    /// To be called on left button up event.
//    void OnLeftButtonUp(int pos1, int pos2)
//    {
//        m_is_dragging = 0;
//        m_is_left_button_down = 0;
//    };
//    
//    int IsDragging(){return m_is_dragging;};
//};
//
//
//
//
//class vmxClipBoard_API vmxItemSelectionDetector
//{
//    
//protected:
//    
//    /// Indicator that will be set when there is possibility of selecting. This is to account for the case when an already selected item is
//    /// picked in the menu where multiple items are already selected, and we need to see if there will be a drag event or not. If no drag is
//    /// perforemed, we need to perform single selection.
//    int m_is_possibly_selecting;
//    
//    /// Indicator that showing if a single item was selected.
//    int m_is_single_selection_detected;
//    
//    /// Indicates if the CTRL key was pressed, which means there is multiple selection.
//    int m_is_CTRL_key_down;
//    
//public:
//    
//    ///Constructor.
//    vmxItemSelectionDetector()
//    {
//        m_is_CTRL_key_down = 0;
//        m_is_possibly_selecting = 0;
//        m_is_single_selection_detected = 0;
//    };
//    
//    /// Destructor.
//    ~vmxItemSelectionDetector(){};
//    
//    /// To be called on left button down event.
//    void OnLeftButtonDown(int is_CTRL_key_down, int is_picked_item_already_selected, int are_multiple_items_selected_in_menu)
//    {
//        m_is_CTRL_key_down = is_CTRL_key_down;
//        m_is_single_selection_detected = 0;
//        
//        /// If multiple selection is being performed, just return.
//        if(is_CTRL_key_down) return;
//        
//        /// In case when an already selected item is picked in the menu where multiple items are already selected, we need to
//        /// see if there will be a drag event or not. If no drag is perforemed, we need to perform single selection.
//        if(is_picked_item_already_selected && are_multiple_items_selected_in_menu)
//        {
//            m_is_possibly_selecting = 1;
//        }
//        else
//        {
//            m_is_single_selection_detected = 1;
//        }
//    };
//    
//    /// To be called on mouse move event. 'is_dragging' parameter takes the return value of vmxMouseDragDetector::IsDragging().
//    void OnMouseMove(int is_dragging)
//    {
//        /// If items are being dragged, there is no possibility of single item selection on left button up.
//        if(is_dragging) m_is_possibly_selecting = 0;
//    };
//    
//    /// To be called on left button up event.
//    void OnLeftButtonUp()
//    {
//        // If m_is_possibly_selecting is set, this means that mouse move event (OnMouseMove())
//        // did not reset it, so we detect single item selection.
//        if(m_is_possibly_selecting)
//        {
//            m_is_single_selection_detected = 1;
//            m_is_possibly_selecting = 0;
//        }
//    };
//
//    
//    int IsSingleSelectionDetected(){return m_is_single_selection_detected;};
//    
//    int IsMultipleSelectionDetected(){return m_is_CTRL_key_down;};
//};
//





#endif
