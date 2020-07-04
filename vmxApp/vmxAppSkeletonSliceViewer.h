 /*=========================================================================
 
 Program:   mipx
 Module:    vmxAppSkeletonSliceViewer.h
 
  Authors: Danilo Babin, Hrvoje Leventic.
  Copyright (c) Danilo Babin, Hrvoje Leventic.
  All rights reserved.
  See Copyright.txt
  
  Licensed under the BSD License 2.0.
  
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE. See the above copyright notice for more information.
  
  =========================================================================*/



#if defined(vmxAppSkeletonSliceViewer_USE_SOURCE_CODE) || defined(vmxApp_USE_SOURCE_CODE)
    #define vmxAppSkeletonSliceViewer_API
#else
    #if defined(_MSC_VER)
        #ifdef vmxAppSkeletonSliceViewer_EXPORTS
            #define vmxAppSkeletonSliceViewer_API __declspec(dllexport)
        #else
            #define vmxAppSkeletonSliceViewer_API __declspec(dllimport)
        #endif
    #else
        #ifdef vmxAppSkeletonSliceViewer_EXPORTS
            #define vmxAppSkeletonSliceViewer_API __attribute__((visibility("default")))
        #else
            #define vmxAppSkeletonSliceViewer_API
        #endif
    #endif
#endif


#ifndef vmxAppSkeletonSliceViewer_H
	#define vmxAppSkeletonSliceViewer_H


#include "mxScopedPointer.h"
#include "vmxImage.h"
#include "vmxSkeleton.h"
#include "vmxAppWidget.h"
//#include "vmxGUIRendererImageViewer.h"
#include "vmxGUIRenderer3DTrackballCamera.h"
#include "vmxPointList.h"



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



/// App Main widget.

class vmxAppSkeletonSliceViewer_API vmxAppSkeletonSliceViewer : public vmxAppViewer
{
    
protected:
    
    //mxScopedPointer< vmxGUIRendererImageViewer > m_image_renderer;
    mxScopedPointer< vmxGUIRenderer3DTrackballCamera > m_image_renderer;
    
    /// Image being visualized.
    vmxImage m_image;
    
    /// Skeleton being visualized.
    vmxSkeleton m_skeleton;
    
    /// Separate skeletons made from the input skeleton that represent cutting plane for each slice index.
    mxArray< vmxSkeleton > m_cut_plane_skeletons;
    
    /// String used as a name tag in skeleton for 'index of subskeleton' scalar values.
    mxString m_scalar_name_index_of_subskeleton;
    
public:
    
    /// command for converting seeds to point list.
    vtkTextActor *m_command_seeds_to_point_list;
    
    /// Constructor.
    vmxAppSkeletonSliceViewer()
    {
        m_class_name.Assign("vmxAppSkeletonSliceViewer");
        //m_image = NULL;
    };
    
    /// Destructor.
    virtual ~vmxAppSkeletonSliceViewer() {};
    
    /// Disconnect the viewer from the default list widget.
    void DisconnectFromDefaultDataListWidget();

    /// Get image.
    vmxImage* GetImage() { return &m_image; };
    
    /// Get the pointer to the GUI renderer.
    //vmxGUIRendererImageViewer* GetRenderer(int index=0) { return m_image_renderer; };
    vmxGUIRenderer3DTrackballCamera* GetRenderer(int index=0) { return m_image_renderer; };
    
    /// Get the seed point list.
    void GetSeedPointList(mxPointList *pl);
    
    /// Compute the separate skeletons made from the input skeleton that represent cutting plane for each slice index.
    /// This method works only if the skeleton and image were set.
    void internalComputeCutPlaneSkeletons();
    
    /// Set input image for the viewer.
    void SetInputImage(vmxImage *image);

    /// Set input skeleton for the viewer.
    void SetInputSkeleton(vmxSkeleton *skeleton);
    
    /// Set up the viewer.
    void Setup(vmxAppMainWidget *main_widget);
    
    /// Update display of the viewer.
    void Update();
        
    /// Slot that will be executed when the viewer need to be updated.
    static int Slot_UpdateViewer(vmxGUIConnection *connection);
    
    /// Slot that will be executed on left button drop - use to update viewer based on the drag&drop of image from data list.
    static int Slot_OnDrop(vmxGUIConnection *connection);
    
    /// Slot that will be executed on key press (if the evet was not caught by gui renderer image viewer).
    static int Slot_OnKeyPress(vmxGUIConnection *connection);
    
    /// Slot that will be executed on left button up event (if the evet was not caught by gui renderer image viewer).
    static int Slot_OnLeftButtonUp(vmxGUIConnection *connection);

};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif


#endif
