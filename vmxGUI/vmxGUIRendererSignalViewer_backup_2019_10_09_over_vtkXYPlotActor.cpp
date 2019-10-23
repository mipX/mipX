/*=========================================================================
 
 Program:   mipx
 Module:    vmxGUIRendererSignalViewer.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "vmxGUIRendererSignalViewer.h"




//---------------------------------------------------------------------------



vmxGUIRendererSignalViewer::vmxGUIRendererSignalViewer(vmxGUIMainWidget *main_widget)
{
    m_class_name.Assign("vmxGUIRendererSignalViewer");
    
    m_clip_board = NULL;
    
    m_is_capturing_event = 1;
    
    m_main_widget = main_widget;
    
    if(m_main_widget)
    {
        m_clip_board = main_widget->GetClipBoard();
    }
    
    m_data_object = vtkSmartPointer<vtkDataObject>::New();
    
    
    xmin = 0.;
    ymin = 0.;
    xmax = 0.;
    ymax = 0.;
    
    
    scale = 1;
    
    int_scale = 1;
    max_int_scale = 10;
    
    //double visible_range_x[2], visible_range_y[2];
    
    
    m_default_array_x = vtkSmartPointer<vtkDoubleArray>::New();
    m_default_array_y = vtkSmartPointer<vtkDoubleArray>::New();
    double value = 0;
    m_default_array_x->InsertValue(0, value);
    m_default_array_y->InsertValue(0, value);
    value = 1;
    m_default_array_x->InsertValue(1, value);
    m_default_array_y->InsertValue(1, value);
    
    
    m_plot_actor = vtkSmartPointer<vtkXYPlotActor>::New();
    m_plot_actor->ExchangeAxesOff();
    m_plot_actor->SetLabelFormat( "%g" );
    m_plot_actor->SetXTitle( "X" );
    m_plot_actor->SetYTitle( "Y" );
    m_plot_actor->SetXValuesToValue();
    
    m_plot_actor->SetPosition(0,0);
    m_plot_actor->SetWidth(1);
    m_plot_actor->SetHeight(1);
    
//    m_plot_actor->LegendOn();
    
    // Predefine colors to use for coloring signals.
//    m_colors.SetNumberOfElements(8);
//    int i=0;
//    m_colors[i].m_red = 1; m_colors[i].m_green = 0; m_colors[i].m_blue = 0; i++;
//    m_colors[i].m_red = 0; m_colors[i].m_green = 1; m_colors[i].m_blue = 0; i++;
//    m_colors[i].m_red = 0; m_colors[i].m_green = 0; m_colors[i].m_blue = 1; i++;
//    m_colors[i].m_red = 0.5; m_colors[i].m_green = 0.5; m_colors[i].m_blue = 0.5; i++;
//    m_colors[i].m_red = 1; m_colors[i].m_green = 0; m_colors[i].m_blue = 1; i++;
//    m_colors[i].m_red = 0; m_colors[i].m_green = 1; m_colors[i].m_blue = 1; i++;
//    m_colors[i].m_red = 0.5; m_colors[i].m_green = 0; m_colors[i].m_blue = 0; i++;
//    m_colors[i].m_red = 0; m_colors[i].m_green = 0.5; m_colors[i].m_blue = 0; i++;
    
    
    
    m_coordinate_text_actor = vtkSmartPointer<vtkTextActor>::New();
    m_coordinate_text_actor->SetInput ( " " );
    m_coordinate_text_actor->SetPosition2 ( 10, 40 );
    m_coordinate_text_actor->GetTextProperty()->SetFontSize ( 24 );
    m_coordinate_text_actor->GetTextProperty()->SetColor ( 1.0, 0.0, 0.0 );
    m_renderer->AddActor2D ( m_coordinate_text_actor );
    

    this->SetListeningFor_MouseMove_Event(1);

//    // VTK structures for creating pick marker.
//    m_pick_marker_actor = vtkSmartPointer<vtkActor>::New();
//    m_pick_marker_lines = vtkSmartPointer<vtkCellArray>::New();
//    m_pick_marker_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//    m_pick_marker_points = vtkSmartPointer<vtkPoints>::New();
//    m_pick_marker_poly_data = vtkSmartPointer<vtkPolyData>::New();
//
//
//    this->internal_CreateCrossHairs();
//
//    m_renderer->AddActor(m_pick_marker_actor);
    
    
    
}


vmxGUIRendererSignalViewer::~vmxGUIRendererSignalViewer()
{
    
}


void vmxGUIRendererSignalViewer::AddInputData(vtkDoubleArray *x_array, vtkDoubleArray *y_array, const char *name, const char *x_label, const char *y_label, unsigned char red, unsigned char green, unsigned char blue)
{
    vmxGUIRendererSignalViewerSignalContainer *signal = m_signals.AddNewToEnd();
    
    if(name) signal->m_name.Assign(name);
    if(x_label) signal->m_X_label.Assign(x_label);
    if(y_label) signal->m_Y_label.Assign(y_label);
    signal->m_X_array = x_array;
    signal->m_Y_array = y_array;
    signal->m_red = red;
    signal->m_green = green;
    signal->m_blue = blue;
    
    if(m_signals.GetNumberOfElements()==1)
    {
        xmin = x_array->GetValue(0);
        xmax = x_array->GetValue(0);
        ymin = y_array->GetValue(0);
        ymax = y_array->GetValue(0);
    }
    for(vtkIdType j = 0; j < x_array->GetNumberOfTuples(); j++)
    {
        if(x_array->GetValue(j)>xmax) xmax = x_array->GetValue(j);
        if(x_array->GetValue(j)<xmin) xmin = x_array->GetValue(j);
    }
    for(vtkIdType j = 0; j < y_array->GetNumberOfTuples(); j++)
    {
        if(y_array->GetValue(j)>ymax) ymax = y_array->GetValue(j);
        if(y_array->GetValue(j)<ymin) ymin = y_array->GetValue(j);
    }
}


void vmxGUIRendererSignalViewer::OnMouseMove()
{
    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();
    int* click_pos = rwi->GetEventPosition();
    
    if(rwi->FindPokedRenderer(click_pos[0], click_pos[1]) != this->m_renderer) return;
    
    double p_x, p_y;
    this->ViewportPositionToPlotCoordinates(click_pos[0], click_pos[1], p_x, p_y);
    //int *renderer_size = this->m_renderer->GetSize();
    //cout<<"  renderer="<<renderer_size[0]<<","<<renderer_size[1]<<"  ";
    //cout<<"  cl_x="<<click_pos[0]<<"  cl_y="<<click_pos[1]<<"  ";
    //cout<<"  p_x="<<p_x<<"  p_y="<<p_y<<" ";
    mxString s;
    s.AssignNumber(p_x);
    s.Append(", ");
    s.AppendNumber(p_y);
    m_coordinate_text_actor->SetInput(s.Get_C_String());
//    m_coordinate_text_actor->SetPosition(click_pos[0],click_pos[1]);
    rwi->Render();
}


void vmxGUIRendererSignalViewer::OnMouseWheelBackward()
{
    int_scale--;
    if(int_scale<1)
    {
        int_scale = 1;
        return;
    }
    
    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();
    
    scale = 1.0 / ((double) int_scale);
    
    int* click_pos = rwi->GetEventPosition();
    double p_x, p_y;
    this->ViewportPositionToPlotCoordinates(click_pos[0], click_pos[1], p_x, p_y);
    
    //this->Zoom(p_x, p_y, xmin, xmax, ymin, ymax, scale);
    
    int *renderer_size = this->m_renderer->GetSize();
    
    this->Zoom2(p_x,p_y, renderer_size[0],renderer_size[1], click_pos[0],click_pos[1], int_scale);
    
    rwi->Render();
}


void vmxGUIRendererSignalViewer::OnMouseWheelForward()
{
    int_scale++;
    if(int_scale>10)
    {
        int_scale = 10;
        return;
    }
    
    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();
    
    scale = 1.0 / ((double) int_scale);
    
    int* click_pos = rwi->GetEventPosition();
    double p_x, p_y;
    this->ViewportPositionToPlotCoordinates(click_pos[0], click_pos[1], p_x, p_y);
    
    //this->Zoom(p_x, p_y, xmin, xmax, ymin, ymax, scale);
    
    int *renderer_size = this->m_renderer->GetSize();
    
    this->Zoom2(p_x,p_y, renderer_size[0],renderer_size[1], click_pos[0],click_pos[1], int_scale);
    
    rwi->Render();
}



void vmxGUIRendererSignalViewer::OnKeyPress()
{
    
    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();

    std::string key = rwi->GetKeySym();
    
    if(key == "a")
    {
        int_scale++;
        if(int_scale>10)
        {
            int_scale = 10;
            return;
        }
        
        scale = 1.0 / ((double) int_scale);
        
        int* click_pos = rwi->GetEventPosition();
        double p_x, p_y;
        this->ViewportPositionToPlotCoordinates(click_pos[0], click_pos[1], p_x, p_y);
        
        //this->Zoom(p_x, p_y, xmin, xmax, ymin, ymax, scale);
        
        int *renderer_size = this->m_renderer->GetSize();
        
        this->Zoom2(p_x,p_y, renderer_size[0],renderer_size[1], click_pos[0],click_pos[1], int_scale);
        
        rwi->Render();
        
    }
    
    if(key == "z")
    {
        int_scale--;
        if(int_scale<1)
        {
            int_scale = 1;
            return;
        }
        
        scale = 1.0 / ((double) int_scale);
        
        int* click_pos = rwi->GetEventPosition();
        double p_x, p_y;
        this->ViewportPositionToPlotCoordinates(click_pos[0], click_pos[1], p_x, p_y);
        
        //this->Zoom(p_x, p_y, xmin, xmax, ymin, ymax, scale);
        
        int *renderer_size = this->m_renderer->GetSize();
        
        this->Zoom2(p_x,p_y, renderer_size[0],renderer_size[1], click_pos[0],click_pos[1], int_scale);
    
        rwi->Render();
        
    }
}


void vmxGUIRendererSignalViewer::RemoveAllData()
{
    m_signals.Reset();
}



void vmxGUIRendererSignalViewer::Update()
{
    if(m_signals.IsEmpty())
    {
        this->AddInputData(m_default_array_x,m_default_array_y," "," "," ",25,25,25);
    }
    

    // find out what is the max number of samples over all curves (arrays).
    int max_number_of_samples = 0;
    {
        mxListIterator< vmxGUIRendererSignalViewerSignalContainer > it;
        for(it.SetToBegin(m_signals); it.IsValid(); it.MoveToNext())
        {
            if(it.GetElementAddress()->m_X_array->GetNumberOfValues() > max_number_of_samples)
            {
                max_number_of_samples = it.GetElementAddress()->m_X_array->GetNumberOfValues();
            }
        }
    }

    for(int i=0; i<m_signals.GetNumberOfElements(); i++)
    {
        m_plot_actor->RemoveDataObjectInput(m_data_object);
    }

    m_data_object->ReleaseData();
    
    m_data_object->Initialize();

    m_plot_actor->RemoveDataObjectInput(m_data_object);

    m_data_object->ReleaseData();

    vtkSmartPointer<vtkFieldData> field_data = vtkSmartPointer<vtkFieldData>::New();


    // enter all arrays to field data.
    mxListIterator< vmxGUIRendererSignalViewerSignalContainer > it;
    for(it.SetToBegin(m_signals); it.IsValid(); it.MoveToNext())
    {
        //IT SEEMS THAT ALL ARRAYS MUST BE OF THE SAME SIZE - HENCE, CREATE TEMP ARRAYS FROM INPUT ONES SUCH THAT THEY ARE ALL THE SAME SIZE AND THEN ADD TO THE FIELD!
        vtkSmartPointer<vtkDoubleArray> array_x = vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> array_y = vtkSmartPointer<vtkDoubleArray>::New();
        int i=0;
        for(i=0; i<it.GetElementAddress()->m_X_array->GetNumberOfValues(); i++)
        {
            array_x->InsertValue(i,it.GetElementAddress()->m_X_array->GetValue(i));
            array_y->InsertValue(i,it.GetElementAddress()->m_Y_array->GetValue(i));
        }
        for(int j=i; j<max_number_of_samples; j++)
        {
            array_x->InsertValue(j,it.GetElementAddress()->m_X_array->GetValue( it.GetElementAddress()->m_X_array->GetNumberOfValues()-1));
            array_y->InsertValue(j,it.GetElementAddress()->m_Y_array->GetValue( it.GetElementAddress()->m_Y_array->GetNumberOfValues()-1));
        }
        
        field_data->AddArray(array_y);
        field_data->AddArray(array_x);
    }

    // connect index value with corresponding X and Y arrays.
    int point_component_counter = 1;
    int i =0;
    for(point_component_counter = 1, i=0; i<m_signals.GetNumberOfElements(); point_component_counter+=2, i++)
    {
        m_data_object->SetFieldData(field_data);

        // connect index and X value array.
        m_plot_actor->SetDataObjectXComponent(i, point_component_counter);

        // connect index and Y value array.
        m_plot_actor->SetDataObjectYComponent(i, point_component_counter-1);

        
        double red = (double(m_signals[i].m_red)) / 255.0;
        double green = (double(m_signals[i].m_green)) / 255.0;
        double blue = (double(m_signals[i].m_blue)) / 255.0;
        
        
        m_plot_actor->SetPlotColor(i, red,green,blue);

        // it seems that this line needs to be here in order to render all curves.
        m_plot_actor->AddDataObjectInput(m_data_object);
    }

    m_plot_actor->SetXValuesToValue();


    visible_range_x[0] = xmin;
    visible_range_x[1] = xmax;
    visible_range_y[0] = ymin;
    visible_range_y[1] = ymax;

    m_plot_actor->SetXRange( xmin, xmax );
    m_plot_actor->SetYRange( ymin, ymax );
    
    if(!m_signals.IsEmpty())
    {
        m_plot_actor->SetXTitle( this->m_signals[0].m_X_label.Get_C_String() );
        m_plot_actor->SetYTitle( this->m_signals[0].m_Y_label.Get_C_String() );
    }

    m_renderer->AddActor(m_plot_actor);

    if(m_renderer->GetRenderWindow())
    {
        if(m_renderer->GetRenderWindow()->GetInteractor())
        {
            m_renderer->GetRenderWindow()->GetInteractor()->Render();
        }
    }
}






void vmxGUIRendererSignalViewer::ViewportPositionToPlotCoordinates(int viewport_pos_x, int viewport_pos_y, double &output_plot_coordinate_x, double &output_plot_coordinate_y)
{
    double v0[2], p0[2], v1[2], p1[2];
    
    int screen_viewport[4];
    this->GetScreenCoordinates(screen_viewport[0],screen_viewport[1],screen_viewport[2],screen_viewport[3]);
    
    // amend viewport position based on the position of the renderer in the render window.
    viewport_pos_x = viewport_pos_x - screen_viewport[0];
    viewport_pos_y = viewport_pos_y - screen_viewport[1];
    
    v0[0] = 0; v0[1] = 0;
    m_plot_actor->ViewportToPlotCoordinate(m_renderer,p0[0],p0[1]);
    
    p1[0] = 0; p1[1] = 0;
    m_plot_actor->PlotToViewportCoordinate(m_renderer,v1[0],v1[1]);
    
    double dx_P = p1[0] - p0[0];
    double dx_V = v1[0] - v0[0];
    
    double dy_P = p1[1] - p0[1];
    double dy_V = v1[1] - v0[1];
    
    double dx_V2 = viewport_pos_x - v1[0];
    output_plot_coordinate_x = p1[0] + dx_P * dx_V2 / dx_V;
    
    double dy_V2 = viewport_pos_y - v1[1];
    output_plot_coordinate_y = p1[1] + dy_P * dy_V2 / dy_V;
}


void vmxGUIRendererSignalViewer::Zoom1(double plot_value_x, double plot_value_y, double x_min, double x_max, double y_min, double y_max, double zoom_scale)
{
    double d_visible_range_x1 = x_max - x_min + 1;

    double normalized_distance_x = (plot_value_x - x_min) / (d_visible_range_x1);

    double d_visible_range_x2 = zoom_scale * d_visible_range_x1;


    visible_range_x[0] = plot_value_x - normalized_distance_x * d_visible_range_x2;
    visible_range_x[1] = visible_range_x[0] + d_visible_range_x2;


    m_plot_actor->SetXRange(visible_range_x[0], visible_range_x[1]);

    double d_visible_range_y1 = y_max - y_min + 1;

    double normalized_distance_y = (plot_value_y - y_min) / (d_visible_range_y1);

    double d_visible_range_y2 = zoom_scale * d_visible_range_y1;


    visible_range_y[0] = plot_value_y - normalized_distance_y * d_visible_range_y2;
    visible_range_y[1] = visible_range_y[0] + d_visible_range_y2;


    m_plot_actor->SetYRange(visible_range_y[0], visible_range_y[1]);
}



void vmxGUIRendererSignalViewer::Zoom2(double plot_value_x, double plot_value_y, int renderer_size_x, int renderer_size_y, int click_pos_x,int click_pos_y, double int_zoom_scale)
{
    m_plot_actor->SetWidth(int_zoom_scale);
    m_plot_actor->SetHeight(int_zoom_scale);
    
    int screen_viewport[4];
    this->GetScreenCoordinates(screen_viewport[0],screen_viewport[1],screen_viewport[2],screen_viewport[3]);
    
    // amend the click position based on the coordinates of the renderer.
    click_pos_x = click_pos_x - screen_viewport[0];
    click_pos_y = click_pos_y - screen_viewport[1];
    
    // relative click position to the screen coordinates (not the axes crossing!)
    double click_relative_pos_x, click_relative_pos_y;
    click_relative_pos_x = ((double)click_pos_x) / ((double)renderer_size_x);
    click_relative_pos_y = ((double)click_pos_y) / ((double)renderer_size_y);
    
    m_plot_actor->SetPosition((int_zoom_scale-1)*(-click_relative_pos_x), (int_zoom_scale-1)*(-click_relative_pos_y));
    
}


