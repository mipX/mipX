/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRendererImageViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxGUIRendererImageViewer.h"




vmxGUIInteractorStyleImageViewer::vmxGUIInteractorStyleImageViewer()
{
    m_gui_renderer = NULL;
}


vmxGUIInteractorStyleImageViewer::~vmxGUIInteractorStyleImageViewer()
{
}


vmxGUIInteractorStyleImageViewer* vmxGUIInteractorStyleImageViewer::New()
{
    return new vmxGUIInteractorStyleImageViewer;
}


void vmxGUIInteractorStyleImageViewer::OnMouseMove()
{
    vtkInteractorStyleImage::OnMouseMove();
}


void vmxGUIInteractorStyleImageViewer::OnLeftButtonUp()
{
    vtkInteractorStyleImage::OnLeftButtonUp();
}


void vmxGUIInteractorStyleImageViewer::OnLeftButtonDown()
{
    vtkInteractorStyleImage::OnLeftButtonDown();
}


void vmxGUIInteractorStyleImageViewer::OnKeyPress()
{
    //get the keypress
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    
    
    if(key.compare("a") == 0)
    {
        m_gui_renderer->SetIndexSlice(m_gui_renderer->m_index_slice+1);
        m_gui_renderer->UpdatePickedPositionsVisualization();
        
        // Don't forget to invoke event, otherwise connections won't work.
        this->m_gui_renderer->InvokeEvent(ImageSliceChangeEvent, this->m_gui_renderer, NULL); //this->m_gui_renderer->InvokeEvent(KeyPressEvent, this->m_gui_renderer, NULL);
        
        rwi->Render();
    }
    if(key.compare("z") == 0)
    {
        m_gui_renderer->SetIndexSlice(m_gui_renderer->m_index_slice-1);
        m_gui_renderer->UpdatePickedPositionsVisualization();
        
        // Don't forget to invoke event, otherwise connections won't work.
        this->m_gui_renderer->InvokeEvent(ImageSliceChangeEvent, this->m_gui_renderer, NULL); //this->m_gui_renderer->InvokeEvent(KeyPressEvent, this->m_gui_renderer, NULL);
        
        rwi->Render();
        
        return;
    }
    if(key.compare("s") == 0)
    {
        //cout<<" s ";
        //m_v_img->SetIndexSlices(m_v_img->GetIndexSlices()+1);
        //rwi->Render();
        return;
    }
    if(key.compare("x") == 0)
    {
        //cout<<" x ";
        //m_v_img->SetIndexSlices(m_v_img->GetIndexSlices()-1);
        //rwi->Render();
        return;
    }
    if(key.compare("l") == 0)
    {
        int is_visible = m_gui_renderer->IsPickedPositionsActorVisible();
        m_gui_renderer->SetVisibilityOfPickedPositions(!is_visible);
        
        rwi->Render();
        
        return;
    }
    if(key.compare("e") == 0)
    {
        m_gui_renderer->ClearAllPickedPositions();
        m_gui_renderer->UpdatePickedPositionsVisualization();
        rwi->Render();
        
        return;
    }
    if(key.compare("k") == 0)
    {
        vtkSmartPointer< vtkCellPicker > picker = vtkSmartPointer< vtkCellPicker >::New();
        
        picker->SetTolerance(0.0005);
        
        // Pick from this location.
        picker->Pick(rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], 0, m_gui_renderer->GetVTKRenderer());
        
        double* local_position = picker->GetPickPosition();
        int index_s, index_r, index_c;
        
        //cout<<" local_w_pos=("<<world_position[0]<<","<<world_position[1]<<","<<world_position[2]<<") ";
        
        this->m_gui_renderer->GetIndexesForLocalCoordinates(local_position[2],local_position[1],local_position[0],index_s,index_r,index_c);
        
        //cout<<" index=["<<index_c<<","<<index_r<<","<<index_s<<"]   ";
        
        double wp[3];
        this->m_gui_renderer->GetWorldCoordinatesForLocalCoordinates(local_position[2],local_position[1],local_position[0], wp[2], wp[1], wp[0]);
        
        
        if(picker->GetCellId() != -1)
        {
            m_gui_renderer->AddPickPosition(wp[0],wp[1],wp[2],0);
            
            this->m_gui_renderer->Pick(index_c,index_r,index_s);
            
            this->m_gui_renderer->InvokeEvent(SeedPointsModifiedEvent, this->m_gui_renderer, NULL);
            
            m_gui_renderer->UpdatePickedPositionsVisualization();
            
            rwi->Render();
        }
        
        return;
    }
    if(key.compare("p") == 0)
    {
        //cout<<"event_pos="<<rwi->GetEventPosition()[0]<<","<< rwi->GetEventPosition()[1]<<"   ";
        
        
        vtkSmartPointer< vtkCellPicker > picker = vtkSmartPointer< vtkCellPicker >::New();
        
        picker->SetTolerance(0.0005);
        
        // Pick from this location.
        picker->Pick(rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], 0, m_gui_renderer->GetVTKRenderer());
        
        double* world_position = picker->GetPickPosition();
        int index_s, index_r, index_c;
        
        //cout<<" world_pos=("<<world_position[0]<<","<<world_position[1]<<","<<world_position[2]<<") ";
        
        this->m_gui_renderer->GetIndexesForLocalCoordinates(world_position[2],world_position[1],world_position[0],index_s,index_r,index_c);
        
        //cout<<" index=["<<index_c<<","<<index_r<<","<<index_s<<"]   ";
        
        if(picker->GetCellId() != -1)
        {
            this->m_gui_renderer->Pick(index_c,index_r,index_s);
            
            m_gui_renderer->UpdatePickedPositionsVisualization();
            
            this->m_gui_renderer->InvokeEvent(ImagePickedEvent, this->m_gui_renderer, NULL);
            
             rwi->Render();
        }
        
        return;
    }
    
    // If the key press event has not been caught, invoke the event.
    this->m_gui_renderer->InvokeEvent(KeyPressEvent, this->m_gui_renderer, NULL);
}





//---------------------------------------------------------------------------



vmxGUIRendererImageViewer::vmxGUIRendererImageViewer()
{
    m_class_name.Assign("vmxGUIRendererImageViewer");
    
    m_clip_board = NULL;
    
    m_is_capturing_event = 1;
    
    m_main_widget = NULL;
    
    m_clip_board = NULL;
    
    m_image_data = NULL;
    
    m_interactor_style = vtkSmartPointer<vmxGUIInteractorStyleImageViewer>::New();
    m_image_actor = vtkSmartPointer<vtkImageActor>::New();
    
    m_reslice = vtkSmartPointer<vtkImageReslice>::New();
    
    m_interactor_style->m_gui_renderer = this;
    
    
    m_plane_orientation = vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL;
    
    m_index_slice = 0;
    m_index_time = 0;
    
    
    // Matrices for transversal, coronal, sagittal, oblique view orientations
    m_transversal_direction_matrix[0] = 1; m_transversal_direction_matrix[1] = 0; m_transversal_direction_matrix[2] = 0; m_transversal_direction_matrix[3] = 0;
    m_transversal_direction_matrix[4] = 0; m_transversal_direction_matrix[5] = 1; m_transversal_direction_matrix[6] = 0; m_transversal_direction_matrix[7] = 0;
    m_transversal_direction_matrix[8] = 0; m_transversal_direction_matrix[9] = 0; m_transversal_direction_matrix[10] = 1; m_transversal_direction_matrix[11] = 0;
    m_transversal_direction_matrix[12] = 0; m_transversal_direction_matrix[13] = 0; m_transversal_direction_matrix[14] = 0; m_transversal_direction_matrix[15] = 1;
    
    //    m_coronal_direction_matrix[0] = 1; m_coronal_direction_matrix[1] = 0; m_coronal_direction_matrix[2] = 0; m_coronal_direction_matrix[3] = 0;
    //    m_coronal_direction_matrix[4] = 0; m_coronal_direction_matrix[5] = 0; m_coronal_direction_matrix[6] = 1; m_coronal_direction_matrix[7] = 0;
    //    m_coronal_direction_matrix[8] = 0; m_coronal_direction_matrix[9] = -1; m_coronal_direction_matrix[10] = 0; m_coronal_direction_matrix[11] = 0;
    //    m_coronal_direction_matrix[12] = 0; m_coronal_direction_matrix[13] = 0; m_coronal_direction_matrix[14] = 0; m_coronal_direction_matrix[15] = 1;
    
    m_coronal_direction_matrix[0] = -1; m_coronal_direction_matrix[1] = 0; m_coronal_direction_matrix[2] = 0; m_coronal_direction_matrix[3] = 0;
    m_coronal_direction_matrix[4] = 0; m_coronal_direction_matrix[5] = 0; m_coronal_direction_matrix[6] = -1; m_coronal_direction_matrix[7] = 0;
    m_coronal_direction_matrix[8] = 0; m_coronal_direction_matrix[9] = 1; m_coronal_direction_matrix[10] = 0; m_coronal_direction_matrix[11] = 0;
    m_coronal_direction_matrix[12] = 0; m_coronal_direction_matrix[13] = 0; m_coronal_direction_matrix[14] = 0; m_coronal_direction_matrix[15] = 1;
    
    //    m_sagittal_direction_matrix[0] = 0; m_sagittal_direction_matrix[1] = 0; m_sagittal_direction_matrix[2] = -1; m_sagittal_direction_matrix[3] = 0;
    //    m_sagittal_direction_matrix[4] = 1; m_sagittal_direction_matrix[5] = 0; m_sagittal_direction_matrix[6] = 0; m_sagittal_direction_matrix[7] = 0;
    //    m_sagittal_direction_matrix[8] = 0; m_sagittal_direction_matrix[9] = -1; m_sagittal_direction_matrix[10] = 0; m_sagittal_direction_matrix[11] = 0;
    //    m_sagittal_direction_matrix[12] = 0; m_sagittal_direction_matrix[13] = 0; m_sagittal_direction_matrix[14] = 0; m_sagittal_direction_matrix[15] = 1;
    
    
    m_sagittal_direction_matrix[0] = 0; m_sagittal_direction_matrix[1] = 0; m_sagittal_direction_matrix[2] = 1; m_sagittal_direction_matrix[3] = 0;
    m_sagittal_direction_matrix[4] = -1; m_sagittal_direction_matrix[5] = 0; m_sagittal_direction_matrix[6] = 0; m_sagittal_direction_matrix[7] = 0;
    m_sagittal_direction_matrix[8] = 0; m_sagittal_direction_matrix[9] = 1; m_sagittal_direction_matrix[10] = 0; m_sagittal_direction_matrix[11] = 0;
    m_sagittal_direction_matrix[12] = 0; m_sagittal_direction_matrix[13] = 0; m_sagittal_direction_matrix[14] = 0; m_sagittal_direction_matrix[15] = 1;
    
    
    
    
    // Create a greyscale lookup table
    m_lookup_table = vtkSmartPointer<vtkLookupTable>::New();
    m_lookup_table->SetRange(0, 65535);
    m_lookup_table->SetValueRange(0,1); // brightness
    m_lookup_table->SetAlphaRange(1,1); // transparency opaque
    m_lookup_table->SetSaturationRange(0,0); // no color saturation
    m_lookup_table->SetHueRange(0,0.66666);
    m_lookup_table->SetRampToLinear();
    m_lookup_table->Build();
    
    
    // Map the image through the lookup table
    
    m_image_map_to_colors = vtkSmartPointer<vtkImageMapToColors>::New();
    m_reslice_axes = vtkSmartPointer<vtkMatrix4x4>::New();
    
    
    
    
    // VTK structures for creating pick marker.
    m_pick_marker_actor = vtkSmartPointer<vtkActor>::New();
    m_pick_marker_lines = vtkSmartPointer<vtkCellArray>::New();
    m_pick_marker_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_pick_marker_points = vtkSmartPointer<vtkPoints>::New();
    m_pick_marker_poly_data = vtkSmartPointer<vtkPolyData>::New();
    
    
    this->internal_CreateCrossHairs();
    
    m_renderer->AddActor(m_pick_marker_actor);
    
    
    // VTK structures for visualization of picked points.
    m_pp_points = vtkSmartPointer<vtkPoints>::New();
    m_pp_poly_data = vtkSmartPointer<vtkPolyData>::New();
    m_pp_sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    m_pp_glyph = vtkSmartPointer<vtkGlyph3D>::New();
    m_pp_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_pp_actor = vtkSmartPointer<vtkActor>::New();
    
    m_renderer->AddActor(m_pp_actor);
    
    
    // Set up overlayed image structures.
    m_overlay_image_data = NULL;
    m_overlay_image_actor = vtkSmartPointer< vtkImageActor >::New();
    m_overlay_reslice = vtkSmartPointer< vtkImageReslice >::New();
    m_overlay_reslice_axes = vtkSmartPointer< vtkMatrix4x4 >::New();
    m_overlay_image_map_to_colors = vtkSmartPointer< vtkImageMapToColors >::New();
    m_overlay_lookup_table = vtkSmartPointer<vtkLookupTable>::New();
    
    
    /// Default mapping is grayscale opaque.
    m_mapping = vmxGUIRendererImageViewerPlaneColorMapping::GRAY_OPAQUE;
    
    /// Default mapping of overlayed image.
    m_mapping_of_overlayed_image = vmxGUIRendererImageViewerPlaneColorMapping::COLOR_TRANSPARENT;
    
    
}





vmxGUIRendererImageViewer::vmxGUIRendererImageViewer(vmxGUIMainWidget *main_widget)
{
    m_class_name.Assign("vmxGUIRendererImageViewer");
    
    m_clip_board = NULL;
    
    m_is_capturing_event = 1;
    
    m_main_widget = main_widget;
    
    if(m_main_widget)
    {
        m_clip_board = main_widget->GetClipBoard();
    }
    
    m_image_data = NULL;
    
    m_interactor_style = vtkSmartPointer<vmxGUIInteractorStyleImageViewer>::New();
    m_image_actor = vtkSmartPointer<vtkImageActor>::New();
    
    m_reslice = vtkSmartPointer<vtkImageReslice>::New();
    
    m_interactor_style->m_gui_renderer = this;
    
    
    m_plane_orientation = vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL;
    
    m_index_slice = 0;
    m_index_time = 0;
    
    
    // Matrices for transversal, coronal, sagittal, oblique view orientations
    m_transversal_direction_matrix[0] = 1; m_transversal_direction_matrix[1] = 0; m_transversal_direction_matrix[2] = 0; m_transversal_direction_matrix[3] = 0;
    m_transversal_direction_matrix[4] = 0; m_transversal_direction_matrix[5] = 1; m_transversal_direction_matrix[6] = 0; m_transversal_direction_matrix[7] = 0;
    m_transversal_direction_matrix[8] = 0; m_transversal_direction_matrix[9] = 0; m_transversal_direction_matrix[10] = 1; m_transversal_direction_matrix[11] = 0;
    m_transversal_direction_matrix[12] = 0; m_transversal_direction_matrix[13] = 0; m_transversal_direction_matrix[14] = 0; m_transversal_direction_matrix[15] = 1;
    
//    m_coronal_direction_matrix[0] = 1; m_coronal_direction_matrix[1] = 0; m_coronal_direction_matrix[2] = 0; m_coronal_direction_matrix[3] = 0;
//    m_coronal_direction_matrix[4] = 0; m_coronal_direction_matrix[5] = 0; m_coronal_direction_matrix[6] = 1; m_coronal_direction_matrix[7] = 0;
//    m_coronal_direction_matrix[8] = 0; m_coronal_direction_matrix[9] = -1; m_coronal_direction_matrix[10] = 0; m_coronal_direction_matrix[11] = 0;
//    m_coronal_direction_matrix[12] = 0; m_coronal_direction_matrix[13] = 0; m_coronal_direction_matrix[14] = 0; m_coronal_direction_matrix[15] = 1;

    m_coronal_direction_matrix[0] = -1; m_coronal_direction_matrix[1] = 0; m_coronal_direction_matrix[2] = 0; m_coronal_direction_matrix[3] = 0;
    m_coronal_direction_matrix[4] = 0; m_coronal_direction_matrix[5] = 0; m_coronal_direction_matrix[6] = -1; m_coronal_direction_matrix[7] = 0;
    m_coronal_direction_matrix[8] = 0; m_coronal_direction_matrix[9] = 1; m_coronal_direction_matrix[10] = 0; m_coronal_direction_matrix[11] = 0;
    m_coronal_direction_matrix[12] = 0; m_coronal_direction_matrix[13] = 0; m_coronal_direction_matrix[14] = 0; m_coronal_direction_matrix[15] = 1;
    
//    m_sagittal_direction_matrix[0] = 0; m_sagittal_direction_matrix[1] = 0; m_sagittal_direction_matrix[2] = -1; m_sagittal_direction_matrix[3] = 0;
//    m_sagittal_direction_matrix[4] = 1; m_sagittal_direction_matrix[5] = 0; m_sagittal_direction_matrix[6] = 0; m_sagittal_direction_matrix[7] = 0;
//    m_sagittal_direction_matrix[8] = 0; m_sagittal_direction_matrix[9] = -1; m_sagittal_direction_matrix[10] = 0; m_sagittal_direction_matrix[11] = 0;
//    m_sagittal_direction_matrix[12] = 0; m_sagittal_direction_matrix[13] = 0; m_sagittal_direction_matrix[14] = 0; m_sagittal_direction_matrix[15] = 1;

    
    m_sagittal_direction_matrix[0] = 0; m_sagittal_direction_matrix[1] = 0; m_sagittal_direction_matrix[2] = 1; m_sagittal_direction_matrix[3] = 0;
    m_sagittal_direction_matrix[4] = -1; m_sagittal_direction_matrix[5] = 0; m_sagittal_direction_matrix[6] = 0; m_sagittal_direction_matrix[7] = 0;
    m_sagittal_direction_matrix[8] = 0; m_sagittal_direction_matrix[9] = 1; m_sagittal_direction_matrix[10] = 0; m_sagittal_direction_matrix[11] = 0;
    m_sagittal_direction_matrix[12] = 0; m_sagittal_direction_matrix[13] = 0; m_sagittal_direction_matrix[14] = 0; m_sagittal_direction_matrix[15] = 1;

    
    
    
    // Create a greyscale lookup table
    m_lookup_table = vtkSmartPointer<vtkLookupTable>::New();
    m_lookup_table->SetRange(0, 65535);
    m_lookup_table->SetValueRange(0,1); // brightness
    m_lookup_table->SetAlphaRange(1,1); // transparency opaque
    m_lookup_table->SetSaturationRange(0,0); // no color saturation
    m_lookup_table->SetHueRange(0,0.66666);
    m_lookup_table->SetRampToLinear();
    m_lookup_table->Build();
    
    
    // Map the image through the lookup table
    
    m_image_map_to_colors = vtkSmartPointer<vtkImageMapToColors>::New();
    m_reslice_axes = vtkSmartPointer<vtkMatrix4x4>::New();
    
    
    
    
    // VTK structures for creating pick marker.
    m_pick_marker_actor = vtkSmartPointer<vtkActor>::New();
    m_pick_marker_lines = vtkSmartPointer<vtkCellArray>::New();
    m_pick_marker_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_pick_marker_points = vtkSmartPointer<vtkPoints>::New();
    m_pick_marker_poly_data = vtkSmartPointer<vtkPolyData>::New();
    
    
    this->internal_CreateCrossHairs();
    
    m_renderer->AddActor(m_pick_marker_actor);
    
    
    // VTK structures for visualization of picked points.
    m_pp_points = vtkSmartPointer<vtkPoints>::New();
    m_pp_poly_data = vtkSmartPointer<vtkPolyData>::New();
    m_pp_sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    m_pp_glyph = vtkSmartPointer<vtkGlyph3D>::New();
    m_pp_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_pp_actor = vtkSmartPointer<vtkActor>::New();

     m_renderer->AddActor(m_pp_actor);
    
    
    // Set up overlayed image structures.
    m_overlay_image_data = NULL;
    m_overlay_image_actor = vtkSmartPointer< vtkImageActor >::New();
    m_overlay_reslice = vtkSmartPointer< vtkImageReslice >::New();
    m_overlay_reslice_axes = vtkSmartPointer< vtkMatrix4x4 >::New();
    m_overlay_image_map_to_colors = vtkSmartPointer< vtkImageMapToColors >::New();
    m_overlay_lookup_table = vtkSmartPointer<vtkLookupTable>::New();
    
    
    /// Default mapping is grayscale opaque.
    m_mapping = vmxGUIRendererImageViewerPlaneColorMapping::GRAY_OPAQUE;

    /// Default mapping of overlayed image.
    m_mapping_of_overlayed_image = vmxGUIRendererImageViewerPlaneColorMapping::COLOR_TRANSPARENT;
    
    
}


vmxGUIRendererImageViewer::~vmxGUIRendererImageViewer()
{
    
}


void vmxGUIRendererImageViewer::FitImageToScreen()
{
    if(m_image_data)
    {
        if( m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
        {
            vtkCamera* camera = this->m_renderer->GetActiveCamera();
            camera->ParallelProjectionOn();
            
            // Set up the camera to fill the renderer with the image
            double origin[3];
            double spacing[3];
            int extent[6];
            m_image_data->GetOrigin( origin );
            m_image_data->GetSpacing( spacing );
            m_image_data->GetExtent( extent );
            
            int dimensions[3];
            m_image_data->GetDimensions(dimensions);
            
            double xc = 0.5*((double)(dimensions[0]-1))*spacing[0];
            double yc = 0.5*((double)(dimensions[1]-1))*spacing[1];
            
            double xd = (extent[1] - extent[0] + 1)*spacing[0];
            double yd = (extent[3] - extent[2] + 1)*spacing[1];
            
            int main_widget_size_xy[2];
            this->GetMainWidget()->GetSize(main_widget_size_xy[0], main_widget_size_xy[1]);
            
            double *viewport_size = this->m_renderer->GetViewport();
            //cout<<"viewport_size="<<viewport_size[0]<<","<<viewport_size[1]<<","<<viewport_size[2]<<","<<viewport_size[3]<<endl;
            
            double viewport_range_x = viewport_size[2] - viewport_size[0];
            double viewport_range_y = viewport_size[3] - viewport_size[1];
            
            double renderer_size_y = viewport_range_y * ((double)main_widget_size_xy[1]);
            double renderer_size_x = viewport_range_x * ((double)main_widget_size_xy[0]);
            
            // take into account if the image will not fit in the renderer along x axis.
            if(xd/yd > viewport_range_x/viewport_range_y)
            {
                yd = renderer_size_y * xd/renderer_size_x;
            }
            
            double d = camera->GetDistance();
            camera->SetParallelScale(0.5*yd); // This scaling is done according to the hight of the parallel projection.
            camera->SetFocalPoint(xc,yc,0.0);
            camera->SetPosition(xc,yc,d);
        }
        
        if( m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
        {
            vtkCamera* camera = this->m_renderer->GetActiveCamera();
            camera->ParallelProjectionOn();
            
            // Set up the camera to fill the renderer with the image
            double origin[3];
            double spacing[3];
            int extent[6];
            m_image_data->GetOrigin( origin );
            m_image_data->GetSpacing( spacing );
            m_image_data->GetExtent( extent );
            
            int dimensions[3];
            m_image_data->GetDimensions(dimensions);
            
            double xc = - 0.5*((double)(dimensions[0]-1))*spacing[0];
            double yc = 0.5*((double)(dimensions[2]-1))*spacing[2];
            
            double xd = dimensions[0]*spacing[0];
            double yd = dimensions[2]*spacing[2];
            
            int main_widget_size_xy[2];
            this->GetMainWidget()->GetSize(main_widget_size_xy[0], main_widget_size_xy[1]);
            
            double *viewport_size = this->m_renderer->GetViewport();
            //cout<<"viewport_size="<<viewport_size[0]<<","<<viewport_size[1]<<","<<viewport_size[2]<<","<<viewport_size[3]<<endl;
            
            double viewport_range_x = viewport_size[2] - viewport_size[0];
            double viewport_range_y = viewport_size[3] - viewport_size[1];
            
            double renderer_size_y = viewport_range_y * ((double)main_widget_size_xy[1]);
            double renderer_size_x = viewport_range_x * ((double)main_widget_size_xy[0]);
            
            // take into account if the image will not fit in the renderer along x axis.
            if(xd/yd > viewport_range_x/viewport_range_y)
            {
                yd = renderer_size_y * xd/renderer_size_x;
            }
            
            double d = camera->GetDistance();
            camera->SetParallelScale(0.5*yd);
            camera->SetFocalPoint(xc,yc,0.0);
            camera->SetPosition(xc,yc,d);
        }
        
        if( m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
        {
            vtkCamera* camera = this->m_renderer->GetActiveCamera();
            camera->ParallelProjectionOn();
            
            // Set up the camera to fill the renderer with the image
            double origin[3];
            double spacing[3];
            int extent[6];
            m_image_data->GetOrigin( origin );
            m_image_data->GetSpacing( spacing );
            m_image_data->GetExtent( extent );
            
            int dimensions[3];
            m_image_data->GetDimensions(dimensions);
            
            double xc = - 0.5*((double)(dimensions[1]-1))*spacing[1];
            double yc = 0.5*((double)(dimensions[2]-1))*spacing[2];
            
            double xd = dimensions[1]*spacing[1];
            double yd = dimensions[2]*spacing[2];
            
            int main_widget_size_xy[2];
            this->GetMainWidget()->GetSize(main_widget_size_xy[0], main_widget_size_xy[1]);
            
            double *viewport_size = this->m_renderer->GetViewport();
            //cout<<"viewport_size="<<viewport_size[0]<<","<<viewport_size[1]<<","<<viewport_size[2]<<","<<viewport_size[3]<<endl;
            
            double viewport_range_x = viewport_size[2] - viewport_size[0];
            double viewport_range_y = viewport_size[3] - viewport_size[1];
            
            double renderer_size_y = viewport_range_y * ((double)main_widget_size_xy[1]);
            double renderer_size_x = viewport_range_x * ((double)main_widget_size_xy[0]);
            
            // take into account if the image will not fit in the renderer along x axis.
            if(xd/yd > viewport_range_x/viewport_range_y)
            {
                yd = renderer_size_y * xd/renderer_size_x;
            }
            
            double d = camera->GetDistance();
            camera->SetParallelScale(0.5*yd);
            camera->SetFocalPoint(xc,yc,0.0);
            camera->SetPosition(xc,yc,d);
        }
    }
}


void vmxGUIRendererImageViewer::GetIndexesForLocalCoordinates(double ws, double wr, double wc, int &output_s, int &output_r, int &output_c)
{
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    {
        output_c = (int)( wc / m_image_data->GetSpacing()[0] );
        output_r = (int)( wr / m_image_data->GetSpacing()[1] );
        output_s = m_index_slice;
        return;
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    {
        output_c = - (int)( wc / m_image_data->GetSpacing()[0] );
        output_r = m_index_slice;
        output_s = (int)( wr / m_image_data->GetSpacing()[2] );
        return;
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    {
        output_c = m_index_slice;
        output_r = - (int)( wc / m_image_data->GetSpacing()[1] );
        output_s = (int)( wr / m_image_data->GetSpacing()[2] );
        return;
    }
}


void vmxGUIRendererImageViewer::GetIndexesForWorldCoordinates(double ws, double wr, double wc, int &output_s, int &output_r, int &output_c)
{
    output_c = (int)( (wc - m_image_data->GetOrigin()[0]) / m_image_data->GetSpacing()[0] );
    output_r = (int)( (wr - m_image_data->GetOrigin()[1]) / m_image_data->GetSpacing()[1] );
    output_s = (int)( (ws - m_image_data->GetOrigin()[2]) / m_image_data->GetSpacing()[2] );
}


void vmxGUIRendererImageViewer::GetLocalCoordinatesForWorldCoordinates(double ws, double wr, double wc, double &output_ls, double &output_lr, double &output_lc)
{
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    {
        output_lc = wc - m_image_data->GetOrigin()[0];
        output_lr = wr - m_image_data->GetOrigin()[1];
        output_ls = 0;//ws - m_image_data->GetOrigin()[2];
        return;
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    {
        output_lc = - (wc - m_image_data->GetOrigin()[0]);
        output_ls = 0;//wr - m_image_data->GetOrigin()[1];
        output_lr = ws - m_image_data->GetOrigin()[2];
        return;
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    {
        output_ls = 0;//wc - m_image_data->GetOrigin()[0];
        output_lc = - (wr - m_image_data->GetOrigin()[1]);
        output_lr = ws - m_image_data->GetOrigin()[2];
        return;
    }
}


void vmxGUIRendererImageViewer::GetWorldCoordinatesForLocalCoordinates(double ls, double lr, double lc, double &output_ws, double &output_wr, double &output_wc)
{
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    {
        output_wc =  lc + m_image_data->GetOrigin()[0];
        output_wr =  lr + m_image_data->GetOrigin()[1];
        output_ws = m_index_slice * m_image_data->GetSpacing()[2] + m_image_data->GetOrigin()[2];
        return;
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    {
        output_wc = - lc + m_image_data->GetOrigin()[0];
        output_wr = m_index_slice * m_image_data->GetSpacing()[1] + m_image_data->GetOrigin()[1];
        output_ws = lr + m_image_data->GetOrigin()[2];
        return;
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    {
        output_wc = m_index_slice * m_image_data->GetSpacing()[0] + m_image_data->GetOrigin()[0];
        output_wr = - lc + m_image_data->GetOrigin()[1];
        output_ws = lr + m_image_data->GetOrigin()[2];
        return;
    }
}


void vmxGUIRendererImageViewer::internal_CreateCrossHairs(int size)
{
    m_pick_marker_lines->Initialize();
    m_pick_marker_points->Initialize();
    m_pick_marker_poly_data->Initialize();
    
    double factor = (double)size;

    double pos[3], direction[3];
    
    //----- point (-1,-1,-1) -----
    pos[0] = -1.0; pos[1] = -1.0; pos[2] = -1.0;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = -0.75; direction[1] = -1.0; direction[2] = -1.0;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = -0.75; direction[2] = -1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = -1; direction[2] = -0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------

    //----- point (-1,-1, 1) -----
    pos[0] = -1; pos[1] = -1; pos[2] = 1;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = -0.75; direction[1] = -1; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = -0.75; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = -1; direction[2] = 0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------
    
    //----- point (-1, 1,-1) -----
    pos[0] = -1; pos[1] = 1; pos[2] = -1;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = -0.75; direction[1] = 1; direction[2] = -1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = 0.75; direction[2] = -1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = 1; direction[2] = -0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------

    //----- point (-1, 1, 1) -----
    pos[0] = -1; pos[1] = 1; pos[2] = 1;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = -0.75; direction[1] = 1; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = 0.75; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = -1; direction[1] = 1; direction[2] = 0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------
    
    //----- point ( 1,-1,-1) -----
    pos[0] = 1.0; pos[1] = -1.0; pos[2] = -1.0;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = 0.75; direction[1] = -1.0; direction[2] = -1.0;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = -0.75; direction[2] = -1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = -1; direction[2] = -0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------

    //----- point ( 1,-1, 1) -----
    pos[0] = 1; pos[1] = -1; pos[2] = 1;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = 0.75; direction[1] = -1; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = -0.75; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = -1; direction[2] = 0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------
    
    //----- point ( 1, 1,-1) -----
    pos[0] = 1; pos[1] = 1; pos[2] = -1;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = 0.75; direction[1] = 1; direction[2] = -1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = 0.75; direction[2] = -1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = 1; direction[2] = -0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------

    //----- point ( 1, 1, 1) -----
    pos[0] = 1; pos[1] = 1; pos[2] = 1;
    pos[0] = pos[0] *factor; pos[1] = pos[1] *factor; pos[2] = pos[2] *factor;
    //---
    direction[0] = 0.75; direction[1] = 1; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = 0.75; direction[2] = 1;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //---
    direction[0] = 1; direction[1] = 1; direction[2] = 0.75;
    direction[0] = direction[0] *factor; direction[1] = direction[1] *factor; direction[2] = direction[2] *factor;
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), pos[0], pos[1], pos[2]);
    m_pick_marker_points->InsertPoint(m_pick_marker_points->GetNumberOfPoints(), direction[0], direction[1], direction[2]);
    m_pick_marker_lines->InsertNextCell(2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-2);
    m_pick_marker_lines->InsertCellPoint(m_pick_marker_points->GetNumberOfPoints()-1);
    //---
    //----------

    m_pick_marker_poly_data->SetPoints(m_pick_marker_points);
    m_pick_marker_poly_data->SetLines(m_pick_marker_lines);

    m_pick_marker_mapper->SetInputData(m_pick_marker_poly_data);
    m_pick_marker_mapper->ScalarVisibilityOff();

    m_pick_marker_actor->SetMapper(m_pick_marker_mapper);
}


int vmxGUIRendererImageViewer::IsPickedPositionsActorVisible()
{
    return m_pp_actor->GetVisibility();
}


void vmxGUIRendererImageViewer::Pick(int index_c, int index_r, int index_s, int t_index)
{
    double world_position_C;
    double world_position_R;
    
    m_picked_indexes[0] = index_c;
    m_picked_indexes[1] = index_r;
    m_picked_indexes[2] = index_s;
    
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    {
        world_position_C = ((double)index_c) * m_image_data->GetSpacing()[0];
        world_position_R = ((double)index_r) * m_image_data->GetSpacing()[1];
        
        this->SetIndexSlice(index_s);
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    {
        world_position_C = - ((double)index_c) * m_image_data->GetSpacing()[0];
        world_position_R = ((double)index_s) * m_image_data->GetSpacing()[2];
        
        this->SetIndexSlice(index_r);
    }
    if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    {
        world_position_C = - ((double)index_r) * m_image_data->GetSpacing()[1];
        world_position_R = ((double)index_s) * m_image_data->GetSpacing()[2];
        
        this->SetIndexSlice(index_c);
    }
    
    m_pick_marker_actor->SetPosition(world_position_C, world_position_R, 0);
}



void vmxGUIRendererImageViewer::SetDirectionToTransversal()
{
    m_plane_orientation = vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL;
    
    if(!m_image_data) return;
    
    // Set slicing location.
    m_index_slice = m_index_slice%(m_image_data->GetDimensions()[2]);
    
    m_reslice_axes->DeepCopy(m_transversal_direction_matrix);
    
    m_reslice_axes->SetElement(0, 3, m_image_data->GetOrigin()[0]);
    m_reslice_axes->SetElement(1, 3, m_image_data->GetOrigin()[1]);
    m_reslice_axes->SetElement(2, 3, m_image_data->GetOrigin()[2]+m_image_data->GetSpacing()[2]*m_index_slice);
    
    m_reslice->SetOutputDimensionality(2);
    m_reslice->SetResliceAxes(m_reslice_axes);
    m_reslice->SetInterpolationModeToLinear();
    m_reslice->Update();
    
    
    
    if(!m_overlay_image_data) return;
    
    m_overlay_reslice->SetInputData(m_overlay_image_data);
    
    // Set slicing location.
//    m_index_slice = m_index_slice%(m_image_data->GetDimensions()[2]);
    
    m_overlay_reslice_axes->DeepCopy(m_transversal_direction_matrix);
    
    m_overlay_reslice_axes->SetElement(0, 3, m_overlay_image_data->GetOrigin()[0]);
    m_overlay_reslice_axes->SetElement(1, 3, m_overlay_image_data->GetOrigin()[1]);
    m_overlay_reslice_axes->SetElement(2, 3, m_overlay_image_data->GetOrigin()[2]+m_overlay_image_data->GetSpacing()[2]*m_index_slice);
    
    m_overlay_reslice->SetOutputDimensionality(2);
    m_overlay_reslice->SetResliceAxes(m_overlay_reslice_axes);
    m_overlay_reslice->SetInterpolationModeToLinear();
    m_overlay_reslice->Update();
    
}


void vmxGUIRendererImageViewer::SetDirectionToCoronal()
{
    m_plane_orientation = vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL;
    
    if(!m_image_data) return;
    
    // Set slicing location.
    m_index_slice = m_index_slice%(m_image_data->GetDimensions()[1]);
    
    m_reslice_axes->DeepCopy(m_coronal_direction_matrix);
    
    m_reslice_axes->SetElement(0, 3, m_image_data->GetOrigin()[0]);
    m_reslice_axes->SetElement(1, 3, m_image_data->GetOrigin()[1]+m_image_data->GetSpacing()[1]*m_index_slice);
    m_reslice_axes->SetElement(2, 3, m_image_data->GetOrigin()[2]);
    
    m_reslice->SetOutputDimensionality(2);
    m_reslice->SetResliceAxes(m_reslice_axes);
    m_reslice->SetInterpolationModeToLinear();
    m_reslice->Update();
}


void vmxGUIRendererImageViewer::SetDirectionToSagittal()
{
    m_plane_orientation = vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL;
    
    if(!m_image_data) return;
    
    // Set slicing location.
    m_index_slice = m_index_slice%(m_image_data->GetDimensions()[0]);
    
    m_reslice_axes->DeepCopy(m_sagittal_direction_matrix);
    
    m_reslice_axes->SetElement(0, 3, m_image_data->GetOrigin()[0]+m_image_data->GetSpacing()[0]*m_index_slice);
    m_reslice_axes->SetElement(1, 3, m_image_data->GetOrigin()[1]);
    m_reslice_axes->SetElement(2, 3, m_image_data->GetOrigin()[2]);
    
    m_reslice->SetOutputDimensionality(2);
    m_reslice->SetResliceAxes(m_reslice_axes);
    m_reslice->SetInterpolationModeToLinear();
    m_reslice->Update();
}


void vmxGUIRendererImageViewer::SetImageData(vtkImageData *image_data)
{
    if(!image_data) return;
    
    m_image_data = image_data;
    
    
    // Make a default grayscale table.
    double min = this->m_image_data->GetScalarTypeMin();
    double max = this->m_image_data->GetScalarTypeMax();
    m_lookup_table->SetRange(min, max);
    m_lookup_table->Build();
    
    
    
    m_reslice->SetInputData(m_image_data);
    
    // Set slicing location.
    m_index_slice = m_index_slice%(m_image_data->GetDimensions()[2]);
    m_reslice_axes->SetElement(0, 3, m_image_data->GetOrigin()[0]);
    m_reslice_axes->SetElement(1, 3, m_image_data->GetOrigin()[1]);
    m_reslice_axes->SetElement(2, 3, m_image_data->GetOrigin()[2]+m_image_data->GetSpacing()[2]*m_index_slice);

    
    m_reslice->SetOutputDimensionality(2);
    m_reslice->SetResliceAxes(m_reslice_axes);
    m_reslice->SetInterpolationModeToLinear();
    m_reslice->Update();
    
    
    m_image_map_to_colors->SetLookupTable(m_lookup_table);
    m_image_map_to_colors->SetInputConnection(m_reslice->GetOutputPort());

    
    
    m_image_actor->GetMapper()->SetInputConnection(m_image_map_to_colors->GetOutputPort()); //m_image_actor->GetMapper()->SetInputConnection(m_reslice->GetOutputPort());
    this->m_renderer->AddActor(m_image_actor);
    
    
    if(m_mapping == vmxGUIRendererImageViewerPlaneColorMapping::GRAY_OPAQUE) this->SetMappingToGrayScale();
    if(m_mapping == vmxGUIRendererImageViewerPlaneColorMapping::COLOR_OPAQUE) this->SetMappingToColor();

    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    {
        this->SetDirectionToTransversal();
    }
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    {
        this->SetDirectionToCoronal();
    }
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    {
        this->SetDirectionToSagittal();
    }
    
}


void vmxGUIRendererImageViewer::SetIndexSlice(unsigned int slice_index)
{
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    {
        m_index_slice = (slice_index+m_image_data->GetDimensions()[2]) % m_image_data->GetDimensions()[2];
        m_reslice_axes->SetElement(2, 3, m_image_data->GetOrigin()[2]+m_image_data->GetSpacing()[2]*m_index_slice);
        m_reslice->Update();
        
        if(m_overlay_image_data)
        {
            m_overlay_reslice_axes->SetElement(2, 3, m_overlay_image_data->GetOrigin()[2]+m_overlay_image_data->GetSpacing()[2]*m_index_slice);
            m_overlay_reslice->Update();
        }
        
        //this->GetConnectionManager()->Execute(ImageSliceChangeEvent, this);//, NULL);
        
        return;
    }
    
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    {
        m_index_slice = (slice_index+m_image_data->GetDimensions()[1]) % m_image_data->GetDimensions()[1];
        m_reslice_axes->SetElement(1, 3, m_image_data->GetOrigin()[1]+m_image_data->GetSpacing()[1]*m_index_slice);
        m_reslice->Update();
        
        if(m_overlay_image_data)
        {
            m_overlay_reslice_axes->SetElement(1, 3, m_overlay_image_data->GetOrigin()[1]+m_overlay_image_data->GetSpacing()[1]*m_index_slice);
            m_overlay_reslice->Update();
        }
        
        //this->GetConnectionManager()->Execute(ImageSliceChangeEvent, this);//, NULL);
        
        return;
    }
    
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    {
        m_index_slice = (slice_index+m_image_data->GetDimensions()[0]) % m_image_data->GetDimensions()[0];
        m_reslice_axes->SetElement(0, 3, m_image_data->GetOrigin()[0]+m_image_data->GetSpacing()[0]*m_index_slice);
        m_reslice->Update();
        
        if(m_overlay_image_data)
        {
            m_overlay_reslice_axes->SetElement(0, 3, m_overlay_image_data->GetOrigin()[0]+m_overlay_image_data->GetSpacing()[0]*m_index_slice);
            m_overlay_reslice->Update();
        }
        
        //this->GetConnectionManager()->Execute(ImageSliceChangeEvent, this);//, NULL);
        
        return;
    }
}


void vmxGUIRendererImageViewer::SetMappingToColor(int is_full_value_range_used)
{
    m_mapping = vmxGUIRendererImageViewerPlaneColorMapping::COLOR_OPAQUE;
    
    if(!m_image_data) return;
    
    double min_max[2];
    if(!is_full_value_range_used)
    {
        m_image_data->GetScalarRange(min_max);
    }
    else
    {
        min_max[0] = this->m_image_data->GetScalarTypeMin();
        min_max[1] = this->m_image_data->GetScalarTypeMax();
    }
    
    m_lookup_table->SetRange(min_max[0], min_max[1]);
    m_lookup_table->SetAlphaRange(1,1); // transparency to opaque
    m_lookup_table->SetValueRange(1,1); // full brightness
    m_lookup_table->SetSaturationRange(1,1); // full color saturation
    m_lookup_table->SetHueRange(0,0.66666);
    m_lookup_table->SetRampToLinear();
    m_lookup_table->Build();
    
    m_image_actor->Update();
}


void vmxGUIRendererImageViewer::SetMappingToGrayScale(int is_full_value_range_used)
{
    m_mapping = vmxGUIRendererImageViewerPlaneColorMapping::GRAY_OPAQUE;
    
    if(!m_image_data) return;
    
    double min_max[2];
    if(!is_full_value_range_used)
    {
        m_image_data->GetScalarRange(min_max);
    }
    else
    {
        min_max[0] = this->m_image_data->GetScalarTypeMin();
        min_max[1] = this->m_image_data->GetScalarTypeMax();
    }
    
    m_lookup_table->SetRange(min_max[0], min_max[1]);
    m_lookup_table->SetAlphaRange(0,1); // transparency to opaque
    m_lookup_table->SetValueRange(1,1); // full brightness
    m_lookup_table->SetSaturationRange(0,0); // no color saturation
    m_lookup_table->SetHueRange(0,0.66666);
    m_lookup_table->SetRampToLinear();
    m_lookup_table->Build();
    
    m_image_actor->Update();
}


void vmxGUIRendererImageViewer::SetOverlayedImageData(vtkImageData *overlayed_image_data)
{
    if(!overlayed_image_data) return;
    
    m_overlay_image_data = overlayed_image_data;
    
    
    
    m_overlay_reslice->SetInputData(m_overlay_image_data);
    
//    // Set slicing location.
//    m_index_slice = m_index_slice%(m_overlay_image_data->GetDimensions()[2]);
    
    m_overlay_reslice_axes->DeepCopy(m_transversal_direction_matrix);
    
    m_overlay_reslice_axes->SetElement(0, 3, m_overlay_image_data->GetOrigin()[0]);
    m_overlay_reslice_axes->SetElement(1, 3, m_overlay_image_data->GetOrigin()[1]);
    m_overlay_reslice_axes->SetElement(2, 3, m_overlay_image_data->GetOrigin()[2]+m_overlay_image_data->GetSpacing()[2]*m_index_slice);

    
    
    m_overlay_reslice->SetOutputDimensionality(2);
    m_overlay_reslice->SetResliceAxes(m_overlay_reslice_axes);
    m_overlay_reslice->SetInterpolationModeToLinear();
    m_overlay_reslice->Update();
    
    
    m_overlay_image_map_to_colors->SetLookupTable(m_overlay_lookup_table);
    m_overlay_image_map_to_colors->SetInputConnection(m_overlay_reslice->GetOutputPort());
    
    
    
    m_overlay_image_actor->GetMapper()->SetInputConnection(m_overlay_image_map_to_colors->GetOutputPort()); //m_image_actor->GetMapper()->SetInputConnection(m_reslice->GetOutputPort());
    this->m_renderer->AddActor(m_overlay_image_actor);

    if(m_mapping_of_overlayed_image == vmxGUIRendererImageViewerPlaneColorMapping::COLOR_TRANSPARENT) this->SetOverlayedImageMappingToColor(0.3);
    if(m_mapping_of_overlayed_image == vmxGUIRendererImageViewerPlaneColorMapping::GRAY_TRANSPARENT) this->SetOverlayedImageMappingToGrayScale(0.3);
    if(m_mapping_of_overlayed_image == vmxGUIRendererImageViewerPlaneColorMapping::BINARY_TRANSPARENT) this->SetOverlayedImageMappingToBinary();
    
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
    {
        this->SetDirectionToTransversal();
    }
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
    {
        this->SetDirectionToCoronal();
    }
    if(m_plane_orientation == vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
    {
        this->SetDirectionToSagittal();
    }
}


void vmxGUIRendererImageViewer::SetOverlayedImageMappingToBinary(unsigned int threshold)
{
    m_mapping_of_overlayed_image = vmxGUIRendererImageViewerPlaneColorMapping::BINARY_TRANSPARENT;
    
    if(!m_overlay_image_data) return;

    double min_max[2];
    min_max[0] = this->m_overlay_image_data->GetScalarTypeMin();
    min_max[1] = this->m_overlay_image_data->GetScalarTypeMax();
    
    
    
    // create a segmentation type lookup table.
    
    m_overlay_lookup_table->SetNumberOfTableValues(min_max[1] - min_max[0] +1);
    m_overlay_lookup_table->SetRange(min_max[0], min_max[1]);
   // m_overlay_lookup_table->SetTableValue( min_max[0], 0, 0, 0, 0 );
    for(unsigned int i=min_max[0]; i<threshold; i++)
    {
        m_overlay_lookup_table->SetTableValue( i, 0.0, 0.0, 0.0, 0.0 );
    }
    for(unsigned int i=threshold; i<=min_max[1]; i++)
    {
        m_overlay_lookup_table->SetTableValue( i, 0.0, 1.0, 0.0, 1.0 );
    }
    //m_overlay_lookup_table->SetAlphaRange(0,1);//set the opacity to increase gradually
    //m_overlay_lookup_table->SetValueRange(1,1); // full brightness to all
    //m_overlay_lookup_table->SetSaturationRange(1,1); // full color saturation
    //m_overlay_lookup_table->SetHueRange(0,0.66666);
    //m_overlay_lookup_table->SetRampToLinear();
    m_overlay_lookup_table->Build();
    
    m_overlay_image_actor->Update();
}


void vmxGUIRendererImageViewer::SetOverlayedImageMappingToColor(double opacity)
{
    m_mapping_of_overlayed_image = vmxGUIRendererImageViewerPlaneColorMapping::COLOR_TRANSPARENT;
    
    if(!m_overlay_image_data) return;
    
    int is_full_value_range_used = 0;
    
    double min_max[2];
    if(!is_full_value_range_used)
    {
        m_overlay_image_data->GetScalarRange(min_max);
    }
    else
    {
        min_max[0] = this->m_overlay_image_data->GetScalarTypeMin();
        min_max[1] = this->m_overlay_image_data->GetScalarTypeMax();
    }
    
    m_overlay_lookup_table->SetRange(min_max[0], min_max[1]);
    m_overlay_lookup_table->SetAlphaRange(opacity,opacity); // transparency to opaque
    m_overlay_lookup_table->SetValueRange(1,1); // full brightness
    m_overlay_lookup_table->SetSaturationRange(1,1); // full color saturation
    m_overlay_lookup_table->SetHueRange(0,0.66666);
    m_overlay_lookup_table->SetRampToLinear();
    m_overlay_lookup_table->Build();
    
    m_overlay_image_actor->Update();
    
}


void vmxGUIRendererImageViewer::SetOverlayedImageMappingToGrayScale(double opacity)
{
    m_mapping_of_overlayed_image = vmxGUIRendererImageViewerPlaneColorMapping::GRAY_TRANSPARENT;
    
    
    if(!m_overlay_image_data) return;
    
    int is_full_value_range_used = 0;
    
    double min_max[2];
    if(!is_full_value_range_used)
    {
        m_overlay_image_data->GetScalarRange(min_max);
    }
    else
    {
        min_max[0] = this->m_overlay_image_data->GetScalarTypeMin();
        min_max[1] = this->m_overlay_image_data->GetScalarTypeMax();
    }
    
    //cout<<"  min="<<min_max[0]<<", max="<<min_max[1]<<"  ";
    
    m_overlay_lookup_table->SetRange(min_max[0], min_max[1]);
    m_overlay_lookup_table->SetAlphaRange(opacity,opacity); // transparency to opaque
    m_overlay_lookup_table->SetValueRange(1,1); // full brightness
    m_overlay_lookup_table->SetSaturationRange(0,0); // full color saturation
    m_overlay_lookup_table->SetHueRange(0,0.66666);
    m_overlay_lookup_table->SetRampToLinear();
    m_overlay_lookup_table->Build();
    
    m_overlay_image_actor->Update();
    
}



void vmxGUIRendererImageViewer::SetVisibilityOfPickedPositions(int is_visible)
{
    m_pp_actor->SetVisibility(is_visible);
}


void vmxGUIRendererImageViewer::UpdatePickedPositionsVisualization()
{
    m_pp_points->Initialize();
    m_pp_poly_data->Initialize();
    
    for(int n=0; n<this->m_number_of_picked_positions; n++)
    {

//        if(slice_index==m_index_slice)
        {
            //cout<<" P ";
            double lp[3];
            this->GetLocalCoordinatesForWorldCoordinates(this->m_picked_poisitions[n].m_z, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_x, lp[2], lp[1], lp[0]);
            m_pp_points->InsertPoint(n, lp[0], lp[1], lp[2]);
        }
    }
    
    
    
//    if(!m_picked_positions_per_index[m_index_slice].IsEmpty())
//    {
//        cout<<" P ";
//        mxListIterator< vmxGUIRendererPosition* > it;
//        int n = 0;
//        for(it.SetToBegin(m_picked_positions_per_index[m_index_slice]); it.IsValid(); it.MoveToNext())
//        {
//            double lp[3];
//            this->GetLocalCoordinatesForWorldCoordinates(it.GetElement()->m_z, it.GetElement()->m_y, it.GetElement()->m_x, lp[2], lp[1], lp[0]);
//            //m_pp_points->InsertPoint(n, this->m_picked_poisitions[n].m_x, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_z);
//            m_pp_points->InsertPoint(n, lp[0], lp[1], lp[2]);
//            n++;
//        }
//    }
    
//    //for(unsigned int n=0; n<this->m_number_of_picked_positions; n++)
//    int n=0;
//    for(unsigned int i=0; i<this->m_number_of_picked_positions; i++)
//    {
//
//        int index[3];
//        this->GetIndexesForWorldCoordinates(this->m_picked_poisitions[n].m_z, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_x, index[2], index[1], index[0]);
//        int slice_index;
//
//        if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_TRANSVERSAL)
//        {
//            slice_index = index[2];
//        }
//        else
//        {
//            if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_CORONAL)
//            {
//                slice_index = index[1];
//            }
//            else //if(this->m_plane_orientation == vmxGUIRendererImageViewer::vmxGUIRendererImageViewerPlaneOrientation::ORIENTATION_SAGITTAL)
//            {
//                slice_index = index[0];
//            }
//        }
//
//        cout<<"  slice_index="<<slice_index<<", m_index_slice="<<m_index_slice<<"  ";
//        //cout<<" index[2]="<<index[2]<<" index[1]="<<index[1]<<" index[0]="<<index[0]<<", m_index_slice="<<m_index_slice<<"  ";
//
//        if(slice_index==m_index_slice)
//        {
//            cout<<" P ";
//            double lp[3];
//            this->GetLocalCoordinatesForWorldCoordinates(this->m_picked_poisitions[n].m_z, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_x, lp[2], lp[1], lp[0]);
//            //m_pp_points->InsertPoint(n, this->m_picked_poisitions[n].m_x, this->m_picked_poisitions[n].m_y, this->m_picked_poisitions[n].m_z);
//            m_pp_points->InsertPoint(n, lp[0], lp[1], lp[2]);
//            n++;
//        }
//    }
    
    
    
    m_pp_poly_data->SetPoints(m_pp_points);
    
//    //Add scalars with index 0 as current scalars
//    m_poly_data->GetPointData()->SetScalars(m_scalars_vtk[0]);
    
    m_pp_sphere_source->SetRadius(2);
    //m_pp_glyph->SetColorModeToColorByScalar();
    m_pp_glyph->SetSourceConnection(m_pp_sphere_source->GetOutputPort());
    m_pp_glyph->SetInputData(m_pp_poly_data);
    m_pp_glyph->ScalingOff();
    m_pp_glyph->Update();
    
    m_pp_mapper->SetInputConnection(m_pp_glyph->GetOutputPort());
//    double scalar_min_value, scalar_max_value;
//    this->GetScalarMinAndMaxValue(0,scalar_min_value,scalar_max_value);
//    this->m_mapper->SetScalarRange(scalar_min_value, scalar_max_value);//for scalars
    
    this->m_pp_mapper->ScalarVisibilityOff();
    
    m_pp_actor->SetMapper(m_pp_mapper);
    //-----
    
//    //----- LABELS -----
//    m_labels->Initialize();
//    m_labels->SetName("Labels");
//    m_labels->SetNumberOfValues(m_points->GetNumberOfPoints());
//    number_of_points = 0;
//    for(it.SetLeftEnd(m_positions); it.IsValid(); it.MoveRight())
//    {
//        m_labels->SetValue(number_of_points,it.GetElementPointer()->GetDescription().C_String());
//        number_of_points++;
//    }
//
//    m_labels_point_source->SetNumberOfPoints(m_points->GetNumberOfPoints());
//    m_labels_point_source->Update();
//    m_labels_point_source->GetOutput()->GetPointData()->AddArray(m_labels);
//    m_labels_point_source->GetOutput()->SetPoints(m_points);
//
//    m_labels_hierarchy_filter->SetInputConnection(m_labels_point_source->GetOutputPort());
//    m_labels_hierarchy_filter->SetLabelArrayName("Labels");
//    m_labels_hierarchy_filter->Update();
//
//    //m_labels_mapper->UseDepthBufferOn();
//    m_labels_mapper->SetMaximumLabelFraction(0.5);
//    m_labels_mapper->SetInputConnection(m_labels_hierarchy_filter->GetOutputPort());
//    m_labels_actor->SetMapper(m_labels_mapper);
//    //-----
}



int vmxGUIRendererImageViewer::Slot_SetIndexSlice(vmxGUIConnection *connection)
{
    vmxGUIRendererImageViewer *viewer_receiver = static_cast<vmxGUIRendererImageViewer *>(connection->GetReceiverObject());
    
    viewer_receiver->SetIndexSlice(*(connection->GetPassedDataInt()));
    
    return 1;
}



int vmxGUIRendererImageViewer::Slot_Pick(vmxGUIConnection *connection)
{
    vmxGUIRendererImageViewer *viewer_receiver = static_cast<vmxGUIRendererImageViewer *>(connection->GetReceiverObject());
    
    viewer_receiver->Pick(connection->GetPassedDataInt()[0], connection->GetPassedDataInt()[1], connection->GetPassedDataInt()[2], connection->GetPassedDataInt()[3]);
    
    return 1;
}

