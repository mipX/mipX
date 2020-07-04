/*=========================================================================
 
 Program:   mipx
 Module:    vmxAppSkeletonSliceViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxAppSkeletonSliceViewer.h"



//-------------------------------------------------------------------------------------------



void vmxAppSkeletonSliceViewer::DisconnectFromDefaultDataListWidget()
{
    vmxGUIConnection::Delete(m_main_widget->GetDataListWidget()->GetListWidget(), ItemSelectedEvent, this->GetRenderer(), vmxAppSkeletonSliceViewer::Slot_UpdateViewer);
}


void vmxAppSkeletonSliceViewer::GetSeedPointList(mxPointList *pl)
{
    pl->Reset();
    double c,r,s,v;
    for(int i=0; i<this->m_image_renderer->GetNumberOfPickedPositions(); i++)
    {
        this->m_image_renderer->GetPickedPosition(i, c,r,s,v);
        pl->AddPosition(s, r, c);
    }
}


void vmxAppSkeletonSliceViewer::internalComputeCutPlaneSkeletons()
{
    if(m_image.IsEmpty() || m_skeleton.IsEmpty())
    {
        std::cout<<std::endl<<"vmxAppSkeletonSliceViewer::internalComputeCutPlaneSkeletons(): Error: image or skeleton empty."<<std::endl;
        return;
    }
    
    m_cut_plane_skeletons.SetNumberOfElements(m_image.GetDimension_S());
    for(int s=0; s< (int)m_cut_plane_skeletons.GetNumberOfElements(); s++)
    {
        m_cut_plane_skeletons[s].SetRenderer(this->m_image_renderer->GetVTKRenderer());
        m_cut_plane_skeletons[s].SetSpacing(m_skeleton.GetSpacing_S(), m_skeleton.GetSpacing_R(), m_skeleton.GetSpacing_C());
        m_cut_plane_skeletons[s].SetOrigin(m_skeleton.GetOrigin_S(), m_skeleton.GetOrigin_R(), m_skeleton.GetOrigin_C());
    }
    
//    //---- version with positions of node 1 and node 2 ----
//    for(int s=0; s<m_image.GetDimension_S(); s++)
//    {
//        double wz, temp1, temp2;
//        m_image.WorldCoordinatesFromIndexes(s, 0, 0, wz, temp1, temp2);
//
//        mxListIterator< mxSkeletonLink > itl;
//        for(itl.SetToBegin(m_skeleton.GetLinks()); itl.IsValid(); itl.MoveToNext())
//        {
//            if( (itl.GetElementAddress()->GetNode_1_Position()->S() > wz && itl.GetElementAddress()->GetNode_2_Position()->S() < wz) ||
//               (itl.GetElementAddress()->GetNode_2_Position()->S() > wz && itl.GetElementAddress()->GetNode_1_Position()->S() < wz)
//               )
//            {
//                m_skeleton.CopyLinkWithItsNodesToOutputSkeleton(itl.GetElementAddress(), &(m_cut_plane_skeletons[s]));
//            }
//        }
//    }
//    //---------

//    //---- version with skeletonization voxels ----
//    for(int s=0; s<m_image.GetDimension_S(); s++)
//    {
//        //double wz, temp1, temp2;
//        //m_image.WorldCoordinatesFromIndexes(s, 0, 0, wz, temp1, temp2);
//
//        mxListIterator< mxSkeletonLink > itl;
//        for(itl.SetToBegin(m_skeleton.GetLinks()); itl.IsValid(); itl.MoveToNext())
//        {
//            mxListIterator< mxVoxel > itv;
//            for(itv.SetToBegin( itl.GetElementAddress()->GetSkeletonizationVoxels()); itv.IsValid(); itv.MoveToNext())
//            {
//                if(itv.GetElementAddress()->S()==s)
//                {
//                    m_skeleton.CopyLinkWithItsNodesToOutputSkeleton(itl.GetElementAddress(), &(m_cut_plane_skeletons[s]));
//                    break;
//                }
//            }
//        }
//    }
//    //--------
    
    //---- version with pasted voxels from link points ----
    mxGeometry g;
    g.SetDimensions(m_image.GetDimension_S(), m_image.GetDimension_R(), m_image.GetDimension_C());
    
    for(int s=0; s< (int)m_image.GetDimension_S(); s++)
    {
        mxListIterator< mxSkeletonLink > itl;
        for(itl.SetToBegin(m_skeleton.GetLinks()); itl.IsValid(); itl.MoveToNext())
        {
            int is_link_selected = 0;
            
            mxArray<mxPoint *> array_of_position_pointers;
            itl.GetElementAddress()->GetArrayOfPositionPointersIncludingNodes(array_of_position_pointers);
            
            for(unsigned int i=0; i<array_of_position_pointers.GetNumberOfElements()-1 && !is_link_selected; i++)
            {
                //double d = array_of_position_pointers[i]->DistanceEuclidean(*(array_of_position_pointers[i+1]));
                
                int local_s, local_r, local_c;
                m_image.IndexFromWorldCoordinates(array_of_position_pointers[i]->S(), array_of_position_pointers[i]->R(), array_of_position_pointers[i]->C(), local_s, local_r, local_c);
                
                int local_s_next, local_r_next, local_c_next;
                m_image.IndexFromWorldCoordinates(array_of_position_pointers[i+1]->S(), array_of_position_pointers[i+1]->R(), array_of_position_pointers[i+1]->C(), local_s_next, local_r_next, local_c_next);
                
                int sn,rn,cn;
                for(g.ForLine(local_s,local_r,local_c, local_s_next,local_r_next,local_c_next); g.GetLine(sn,rn,cn); )
                {
                    if(sn==local_s_next && rn==local_r_next && cn==local_c_next)
                    {
                        break;
                    }
                    
                    if(sn==s)
                    {
                        is_link_selected = 1;
                        m_skeleton.CopyLinkWithItsNodesToOutputSkeleton(itl.GetElementAddress(), &(m_cut_plane_skeletons[s]));
                        break;
                    }
                }
            }
        }
    }
    //--------

    
    for(int s=0; s< (int)m_cut_plane_skeletons.GetNumberOfElements(); s++)
    {
        m_cut_plane_skeletons[s].Update();
        m_cut_plane_skeletons[s].SetColoringScalars(m_scalar_name_index_of_subskeleton);
        m_cut_plane_skeletons[s].SetVisibility(1);
    }
    
}


void vmxAppSkeletonSliceViewer::SetInputImage(vmxImage *image)
{
    m_image.Copy(image);
    m_image.SetInteractor(this->m_image_renderer->GetMainWidget()->GetInteractor());
    //m_image.SetInteractor(this->GetMainWidget()->GetInteractor()); // CALLING THIS LINE CRASHES THE APP!!!!!!!!!!!!
    m_image.SetRenderer(this->m_image_renderer->GetVTKRenderer());
    m_image.Update();
    m_image.SetMappingToOpaqueGrayScale();
    m_image.SetVisibility(1);
    
    this->internalComputeCutPlaneSkeletons();
}


void vmxAppSkeletonSliceViewer::SetInputSkeleton(vmxSkeleton *skeleton)
{
    if(skeleton)
    {
        m_skeleton.Copy(skeleton);
        
        skeleton->InsertScalarsAsIndexOfLink(m_scalar_name_index_of_subskeleton.Get_C_String());
        
        //m_skeleton.InsertScalarsAsIndexOfSubSkeleton(m_scalar_name_index_of_subskeleton.Get_C_String());
        m_skeleton.InsertScalarsAsIndexOfLink(m_scalar_name_index_of_subskeleton.Get_C_String());
        
        //m_skeleton.SetInteractor(this->GetMainWidget()->GetInteractor());
        m_skeleton.SetRenderer(this->m_image_renderer->GetVTKRenderer());
        m_skeleton.Update();
        m_skeleton.SetVisibility(0);
        m_skeleton.SetColoringScalars(m_scalar_name_index_of_subskeleton);

        this->internalComputeCutPlaneSkeletons();
        
        return;
    }
}


void vmxAppSkeletonSliceViewer::Setup(vmxAppMainWidget *main_widget)
{
    m_main_widget = main_widget;
    
    m_scalar_name_index_of_subskeleton.Assign("Index of Subskeleton");

    m_image_renderer = new vmxGUIRenderer3DTrackballCamera(m_main_widget);
    this->SetNumberOfRenderers(1);
    this->RegisterRenderer(0,m_image_renderer);
    
    m_command_seeds_to_point_list = m_image_renderer->AddCommand("Seeds to PointList");
    
    vmxGUIConnection::New(m_image_renderer, LeftButtonDropEvent, m_image_renderer, this, vmxAppSkeletonSliceViewer::Slot_OnDrop);
    vmxGUIConnection::New(m_image_renderer, KeyPressEvent, m_image_renderer, this, vmxAppSkeletonSliceViewer::Slot_OnKeyPress);
    vmxGUIConnection::New(m_image_renderer, LeftButtonUpEvent, m_image_renderer, this, vmxAppSkeletonSliceViewer::Slot_OnLeftButtonUp);
}


void vmxAppSkeletonSliceViewer::Update()
{
    //m_image_renderer->Update();
}


int vmxAppSkeletonSliceViewer::Slot_UpdateViewer(vmxGUIConnection *connection)
{
    vmxAppSkeletonSliceViewer *viewer = static_cast<vmxAppSkeletonSliceViewer *>(connection->GetSlotCallerObject());
    
    
    mxDataObject *obj = static_cast<mxDataObject *>(*(connection->GetPassedAddress(0)));
    
    if(!obj) return 0;
    
    vmxImage image;
    if(obj->GetClassName() == image.GetClassName())
    {
        vmxImage *img = static_cast<vmxImage *>(obj);
        viewer->SetInputImage(img);
    }

    viewer->Update();
    
    return 1;
}



int vmxAppSkeletonSliceViewer::Slot_OnDrop(vmxGUIConnection *connection)
{
    //std::cout<<std::endl<<"vmxAppSkeletonSliceViewer::Slot_OnDrop() called.";
    
    vmxAppSkeletonSliceViewer *app_viewer = static_cast<vmxAppSkeletonSliceViewer *>(connection->GetSlotCallerObject());
    vmxGUIRenderer3DTrackballCamera *gui_viewer = static_cast<vmxGUIRenderer3DTrackballCamera *>(connection->GetSenderObject());
    
    mxString data_string;
    
    if(!gui_viewer->GetClipBoard()->FetchData(&data_string)) return 0;
    
    mxString s;
    s.Assign(vmxGUIItemSeparationChar1);
    s.Append(data_string);
    mxString item_name;
    s.ExtractString(vmxGUIItemSeparationChar1, vmxGUIItemSeparationChar1, item_name);
    
    mxDataObject *obj = app_viewer->m_main_widget->GetDataListWidget()->GetDataObject(item_name.Get_C_String());
    if(!obj) return 0;
    
    vmxImage img;
    if(obj->GetClassName()==img.GetClassName())
    {
        vmxImage *image = static_cast< vmxImage* >(obj);
        if(image)
        {
            app_viewer->SetInputImage(image);
            
            
            //app_viewer->m_image_renderer->SetMappingToGrayScale();
        }
    }
    else
    {
        vmxSkeleton skl;
        if(obj->GetClassName()==skl.GetClassName())
        {
            vmxSkeleton *skeleton = static_cast< vmxSkeleton* >(obj);
            if(skeleton)
            {
                app_viewer->SetInputSkeleton(skeleton);
                //app_viewer->m_image_renderer->SetMappingToGrayScale();
            }
        }
    }
    return 1;
}



int vmxAppSkeletonSliceViewer::Slot_OnKeyPress(vmxGUIConnection *connection)
{
    vmxAppSkeletonSliceViewer *app_viewer = static_cast<vmxAppSkeletonSliceViewer *>(connection->GetSlotCallerObject());
    vmxGUIRenderer3DTrackballCamera *gui_viewer = static_cast<vmxGUIRenderer3DTrackballCamera *>(connection->GetSenderObject());

    vtkRenderWindowInteractor *rwi = gui_viewer->m_interactor_style->GetInteractor();
    std::string key = rwi->GetKeySym();
    
    if(key.compare("a") == 0)
    {
        if(app_viewer->GetImage()->IsEmpty()) return 1;
        
        unsigned int index = app_viewer->GetImage()->GetIndex_S();
        
        int new_index = 0;
        
        if(index == app_viewer->GetImage()->GetDimension_S()-1)
        {
            app_viewer->GetImage()->SetIndex_S(0);
            gui_viewer->m_index_slice = 0;
            new_index = 0;
        }
        else
        {
            app_viewer->GetImage()->SetIndex_S(index+1);
            gui_viewer->m_index_slice = index+1;
            new_index = index+1;
        }
        
        if(!app_viewer->m_cut_plane_skeletons.IsEmpty())
        {
            for(int s=0; s< (int)app_viewer->m_cut_plane_skeletons.GetNumberOfElements(); s++)
            {
                app_viewer->m_cut_plane_skeletons[s].SetVisibility(0);
            }
            //unsigned int scalar_index;
            //app_viewer->m_cut_plane_skeletons[new_index].GetScalarTagIndex(app_viewer->m_scalar_name_index_of_subskeleton, scalar_index);
            //app_viewer->m_cut_plane_skeletons[new_index].UpdateScalars(scalar_index);
            app_viewer->m_cut_plane_skeletons[new_index].SetColoringScalars(app_viewer->m_scalar_name_index_of_subskeleton);
            app_viewer->m_cut_plane_skeletons[new_index].SetVisibility(1);
            app_viewer->m_cut_plane_skeletons[new_index].SetColoringScalars(app_viewer->m_scalar_name_index_of_subskeleton);
        }
        
        
        gui_viewer->DisplayProperties();

        // Don't forget to invoke event, otherwise connections won't work.
        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);

        rwi->Render();
        
        return 1;
    }
    
    if(key.compare("z") == 0)
    {
        if(app_viewer->GetImage()->IsEmpty()) return 1;
        
        unsigned int index = app_viewer->GetImage()->GetIndex_S();
        
        int new_index = 0;
        
        if(index == 0)
        {
            app_viewer->GetImage()->SetIndex_S(app_viewer->GetImage()->GetDimension_S()-1);
            gui_viewer->m_index_slice = app_viewer->GetImage()->GetDimension_S()-1;
            new_index = app_viewer->GetImage()->GetDimension_S()-1;
        }
        else
        {
            app_viewer->GetImage()->SetIndex_S(index-1);
            gui_viewer->m_index_slice = index-1;
            new_index = index-1;
        }
        
        if(!app_viewer->m_cut_plane_skeletons.IsEmpty())
        {
            for(int s=0; s< (int)app_viewer->m_cut_plane_skeletons.GetNumberOfElements(); s++)
            {
                app_viewer->m_cut_plane_skeletons[s].SetVisibility(0);
            }
            //app_viewer->m_cut_plane_skeletons[new_index].SetVisibility(1);
            
            app_viewer->m_cut_plane_skeletons[new_index].SetColoringScalars(app_viewer->m_scalar_name_index_of_subskeleton);
            app_viewer->m_cut_plane_skeletons[new_index].SetVisibility(1);
            app_viewer->m_cut_plane_skeletons[new_index].SetColoringScalars(app_viewer->m_scalar_name_index_of_subskeleton);
        }
        
        
        gui_viewer->DisplayProperties();
        
        // Don't forget to invoke event, otherwise connections won't work.
        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
        
        rwi->Render();
        
        return 1;
    }
    

    
    
    if(key.compare("x") == 0)
    {
//        if(!gui_viewer->m_image_data) return 1;
//
//        if(gui_viewer->m_index_time==0) gui_viewer->m_index_time = app_viewer->m_image->GetDimension_T() - 1;
//        else gui_viewer->m_index_time = gui_viewer->m_index_time-1;
//        gui_viewer->SetImageData(app_viewer->m_image->GetVTKImageData(gui_viewer->m_index_time));
//
//        gui_viewer->DisplayProperties();
//
//        // Don't forget to invoke event, otherwise connections won't work.
//        gui_viewer->InvokeEvent(ImageSliceChangeEvent, gui_viewer, NULL);
//
        rwi->Render();
        
        return 1;
    }
    
    return 1;

}



int vmxAppSkeletonSliceViewer::Slot_OnLeftButtonUp(vmxGUIConnection *connection)
{
    vmxAppSkeletonSliceViewer *app_viewer = static_cast<vmxAppSkeletonSliceViewer *>(connection->GetSlotCallerObject());
    vmxGUIRenderer3DTrackballCamera *gui_viewer = static_cast<vmxGUIRenderer3DTrackballCamera *>(connection->GetSenderObject());
    
    vtkRenderWindowInteractor *rwi = gui_viewer->m_interactor_style->GetInteractor();
    
    vtkTextActor *ta = gui_viewer->GetCommand(rwi->GetEventPosition()[0],rwi->GetEventPosition()[1]);
    
    if(ta == app_viewer->m_command_seeds_to_point_list)
    {
        if(gui_viewer->GetNumberOfPickedPositions()>0)
        {
            vmxPointList *pl = static_cast< vmxPointList* >( app_viewer->m_main_widget->GetDataListWidget()->Create("vmxPointList", "Seeds") );
            
            if(pl)
            {
                app_viewer->GetSeedPointList(pl);
                pl->Update();
            }
        }
    }
    
    return 1;
}
