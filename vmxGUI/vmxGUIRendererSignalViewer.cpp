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
    
    m_is_left_button_down = 0;
    
    
    
    vtkRenderer *renderer = this->GetVTKRenderer();
    renderer->SetBackground(0.02, 0.02, 0.02);

    
    m_chart = vtkSmartPointer<vtkChartXY>::New();
    m_chart_scene = vtkSmartPointer<vtkContextScene>::New();
    m_chart_actor = vtkSmartPointer<vtkContextActor>::New();
    
    
    //   m_chart->SetAutoSize(false);
    //   m_chart->SetSize(vtkRectf(0.0, 0.0, 300, 200));
    
    m_chart->SetShowLegend(true);
//    m_chart->SetSelectionMethod(vtkChart::SELECTION_PLOTS);
    double grey_level = 0.6;
    
    m_chart->GetAxis(0)->GetLabelProperties()->SetColor(grey_level,grey_level,grey_level);
    m_chart->GetAxis(0)->GetTitleProperties()->SetColor(grey_level,grey_level,grey_level);
    m_chart->GetAxis(1)->GetLabelProperties()->SetColor(grey_level,grey_level,grey_level);
    m_chart->GetAxis(1)->GetTitleProperties()->SetColor(grey_level,grey_level,grey_level);
    
    unsigned char grid_grey_level = 25;
    m_chart->GetAxis(0)->GetGridPen()->SetColor(grid_grey_level,grid_grey_level,grid_grey_level);// SetGridVisible(0);
    m_chart->GetAxis(1)->GetGridPen()->SetColor(grid_grey_level,grid_grey_level,grid_grey_level);// SetGridVisible(0);

    m_chart->GetLegend()->SetVerticalAlignment(vtkChartLegend::BOTTOM);//CUSTOM);
    m_chart->GetLegend()->SetHorizontalAlignment(vtkChartLegend::LEFT);//CENTER);//CUSTOM);
    
    m_chart_scene->AddItem(m_chart);
    m_chart_actor->SetScene(m_chart_scene);
    
    
    
    //both needed
    renderer->AddActor(m_chart_actor);
    m_chart_scene->SetRenderer(renderer);
    
    // Create a table.
    m_table = vtkSmartPointer<vtkTable>::New();
    
    m_table->Initialize();
    

//    this->GetMainWidget()->GetRenderWindow()->SetMultiSamples(0);
    


    // create default signal to plot
    m_default_array_x = vtkSmartPointer<vtkDoubleArray>::New();
    m_default_array_y = vtkSmartPointer<vtkDoubleArray>::New();
    double value = 0;
    m_default_array_x->InsertValue(0, value);
    m_default_array_y->InsertValue(0, value);
    value = 1;
    m_default_array_x->InsertValue(1, value);
    m_default_array_y->InsertValue(1, value);

    
//    m_coordinate_text_actor = vtkSmartPointer<vtkTextActor>::New();
//    m_coordinate_text_actor->SetInput ( " " );
//    m_coordinate_text_actor->SetPosition2 ( 10, 40 );
//    m_coordinate_text_actor->GetTextProperty()->SetFontSize ( 24 );
//    m_coordinate_text_actor->GetTextProperty()->SetColor ( 1.0, 0.0, 0.0 );
//    m_renderer->AddActor2D ( m_coordinate_text_actor );
//
//
/////    this->SetListeningFor_MouseMove_Event(1);
//
////    // VTK structures for creating pick marker.
////    m_pick_marker_actor = vtkSmartPointer<vtkActor>::New();
////    m_pick_marker_lines = vtkSmartPointer<vtkCellArray>::New();
////    m_pick_marker_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
////    m_pick_marker_points = vtkSmartPointer<vtkPoints>::New();
////    m_pick_marker_poly_data = vtkSmartPointer<vtkPolyData>::New();
////
////
////    this->internal_CreateCrossHairs();
////
////    m_renderer->AddActor(m_pick_marker_actor);
//
//
    
}


vmxGUIRendererSignalViewer::~vmxGUIRendererSignalViewer()
{
    
}


void vmxGUIRendererSignalViewer::CreateMouseEvent(vtkContextMouseEvent &event, int button)
{
    int renderer_coords_x0y0x1y1[4];
    this->GetScreenCoordinates(renderer_coords_x0y0x1y1[0],renderer_coords_x0y0x1y1[1], renderer_coords_x0y0x1y1[2],renderer_coords_x0y0x1y1[3]);
    
    
    event.SetInteractor(this->GetMainWidget()->GetInteractor());
    event.SetPos(vtkVector2f(this->GetMainWidget()->GetInteractor()->GetEventPosition()[0] - renderer_coords_x0y0x1y1[0],
                             this->GetMainWidget()->GetInteractor()->GetEventPosition()[1] - renderer_coords_x0y0x1y1[1]));
    event.SetButton(button);
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
}


void vmxGUIRendererSignalViewer::OnMouseMove()
{
    int mouse_screen_pos_xy[2];
    mouse_screen_pos_xy[0] = this->GetMainWidget()->GetInteractor()->GetEventPosition()[0];
    mouse_screen_pos_xy[1] = this->GetMainWidget()->GetInteractor()->GetEventPosition()[1];
    
    
    int renderer_coords_x0y0x1y1[4];
    this->GetScreenCoordinates(renderer_coords_x0y0x1y1[0],renderer_coords_x0y0x1y1[1], renderer_coords_x0y0x1y1[2],renderer_coords_x0y0x1y1[3]);
    
    int local_mouse_position_xy[2];
    local_mouse_position_xy[0] = mouse_screen_pos_xy[0] - renderer_coords_x0y0x1y1[0];
    local_mouse_position_xy[1] = mouse_screen_pos_xy[1] - renderer_coords_x0y0x1y1[1];
    
    
    //    cout<<" axis(0)pos="<< this->m_gui_renderer->m_chart->GetAxis(0)->GetPoint1()[0]<<","<< this->m_gui_renderer->m_chart->GetAxis(0)->GetPoint1()[1];
    //    //cout<<" axis(1)pos="<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint1()[0]<<","<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint1()[1];
    //    cout<<" axis()pos2="<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint2()[0]<<","<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint2()[1];
    
    //chartP1=[44,37], chartP2=[762,758] range_x=[0,7.5], range_y=[-35,40]
    
    //std::cout<<" {"<<local_mouse_position_xy[0]<<","<<local_mouse_position_xy[1]<<"}";
    
    int chart_p1[2];
    this->m_chart->GetPoint1(chart_p1[0],chart_p1[1]);
    int chart_p2[2];
    this->m_chart->GetPoint2(chart_p2[0],chart_p2[1]);
    double chart_p1_x = chart_p1[0];
    double chart_p1_y = chart_p1[1];
    double chart_p2_x = chart_p2[0];
    double chart_p2_y = chart_p2[1];
    //cout<<" chartP1=["<<chart_p1[0]<<","<<chart_p1[1]<<"], chartP2=["<<chart_p2[0]<<","<<chart_p2[1]<<"]";
    
    
    double x_value_range[2];
    this->m_chart->GetAxis(1)->GetRange(x_value_range);
    double y_value_range[2];
    this->m_chart->GetAxis(0)->GetRange(y_value_range);
    //cout<<" range_x=["<<x_value_range[0]<<","<<x_value_range[1]<<"], range_y=["<<y_value_range[0]<<","<<y_value_range[1]<<"]";
    
    double dx = (x_value_range[1]-x_value_range[0]) / (chart_p2_x-chart_p1_x);
    double value_x = ( local_mouse_position_xy[0] - chart_p1_x ) * dx + x_value_range[0];
    
    double dy = (y_value_range[1]-y_value_range[0]) / (chart_p2_y-chart_p1_y);
    double value_y = ( local_mouse_position_xy[1] - chart_p1_y ) * dy + y_value_range[0];

    
    
    if(m_is_left_button_down)
    {
        
        int local_last_left_button_position_xy[2];
        local_last_left_button_position_xy[0] = m_last_mouse_screen_position_xy[0] - renderer_coords_x0y0x1y1[0];
        local_last_left_button_position_xy[1] = m_last_mouse_screen_position_xy[1] - renderer_coords_x0y0x1y1[1];
        

        double last_left_button_hold_value_x = (local_last_left_button_position_xy[0] - chart_p1_x) * dx + x_value_range[0];
        double last_left_button_hold_value_y = (local_last_left_button_position_xy[1] - chart_p1_y) * dy + y_value_range[0];
        
        //std::cout<<" {"<<last_left_button_hold_value_x<<","<<last_left_button_hold_value_y<<"}";
        
        double move_x = value_x - last_left_button_hold_value_x;
        double move_y = value_y - last_left_button_hold_value_y;
        
        //std::cout<<"_["<<move_x<<","<<move_y<<"]";
        
        vtkAxis* x_axis = this->m_chart->GetAxis(1);
        vtkAxis* y_axis = this->m_chart->GetAxis(0);
        
        
        x_axis->SetRange(x_value_range[0]-move_x, x_value_range[1]-move_x);
        y_axis->SetRange(y_value_range[0]-move_y, y_value_range[1]-move_y);
        
        m_last_mouse_screen_position_xy[0] = mouse_screen_pos_xy[0];
        m_last_mouse_screen_position_xy[1] = mouse_screen_pos_xy[1];
    }

    
    m_last_mouse_screen_position_xy[0] = mouse_screen_pos_xy[0];
    m_last_mouse_screen_position_xy[1] = mouse_screen_pos_xy[1];
    


    mxString s;
    s.AppendNumber(value_x);
    s.Append(", ");
    s.AppendNumber(value_y);
    vtkStdString text;
    text.assign(s.Get_C_String());
    this->m_chart->GetTooltip()->SetText(text);
    vtkVector2f local_pos;
    local_pos.Set(local_mouse_position_xy[0], local_mouse_position_xy[1]);
    this->m_chart->GetTooltip()->SetPosition(local_pos);
    this->m_chart->GetTooltip()->SetVisible(1);


    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();

    rwi->Render();

    
}


void vmxGUIRendererSignalViewer::OnLeftButtonDown()
{
    m_is_left_button_down = 1;
}


void vmxGUIRendererSignalViewer::OnLeftButtonUp()
{
    m_is_left_button_down = 0;
}


void vmxGUIRendererSignalViewer::OnMouseWheelBackward()
{
    
    vtkContextMouseEvent event;
    this->CreateMouseEvent(event, vtkContextMouseEvent::MIDDLE_BUTTON);
    
    this->m_chart->MouseWheelEvent(event, -1);
    
    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();
    rwi->Render();
    
}


void vmxGUIRendererSignalViewer::OnMouseWheelForward()
{
    vtkContextMouseEvent event;
    this->CreateMouseEvent(event, vtkContextMouseEvent::MIDDLE_BUTTON);
    
    this->m_chart->MouseWheelEvent(event, 1);

    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();
    rwi->Render();
}



void vmxGUIRendererSignalViewer::OnKeyPress()
{
    
//    vtkRenderWindowInteractor *rwi = this->GetMainWidget()->GetInteractor();
//
//    std::string key = rwi->GetKeySym();
//
//    if(key == "a")
//    {
//        rwi->Render();
//
//    }
//
//    if(key == "z")
//    {
//        rwi->Render();
//
//    }
}


void vmxGUIRendererSignalViewer::RemoveAllData()
{
    m_signals.Reset();
}



void vmxGUIRendererSignalViewer::Update()
{
    m_table->Initialize();
    m_chart->ClearPlots();

    
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

    
    
    
    // enter all arrays to table.
    mxListIterator< vmxGUIRendererSignalViewerSignalContainer > it;
    int counter = 0;
    for(it.SetToBegin(m_signals), counter = 0; it.IsValid(); it.MoveToNext(), counter+=2)
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

        {
            mxString name;
            name.Append(it.GetElementAddress()->m_name.Get_C_String());
            name.Append("X_Axis");
            array_x->SetName(name.Get_C_String());
            m_table->AddColumn(array_x);
        }

        array_y->SetName(it.GetElementAddress()->m_name.Get_C_String());
        m_table->AddColumn(array_y);


        vtkPlot *line = m_chart->AddPlot(vtkChart::LINE);
        line->SetInputData(m_table, counter, counter+1);
        line->SetColor(it.GetElementAddress()->m_red, it.GetElementAddress()->m_green, it.GetElementAddress()->m_blue, 255);
        line->SetWidth(1.0);
    }
    
    if(!m_signals.IsEmpty())
    {
        vtkStdString title_x;
        title_x.assign(this->m_signals[0].m_X_label.Get_C_String());
        m_chart->GetAxis(1)->SetTitle(title_x);
        vtkStdString title_y;
        title_y.assign(this->m_signals[0].m_Y_label.Get_C_String());
        m_chart->GetAxis(0)->SetTitle(title_y);
    }

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
    int mouse_screen_pos_xy[2];
    mouse_screen_pos_xy[0] = this->GetMainWidget()->GetInteractor()->GetEventPosition()[0];
    mouse_screen_pos_xy[1] = this->GetMainWidget()->GetInteractor()->GetEventPosition()[1];
    
    int renderer_coords_x0y0x1y1[4];
    this->GetScreenCoordinates(renderer_coords_x0y0x1y1[0],renderer_coords_x0y0x1y1[1], renderer_coords_x0y0x1y1[2],renderer_coords_x0y0x1y1[3]);
    
    int local_mouse_position_xy[2];
    local_mouse_position_xy[0] = mouse_screen_pos_xy[0] - renderer_coords_x0y0x1y1[0];
    local_mouse_position_xy[1] = mouse_screen_pos_xy[1] - renderer_coords_x0y0x1y1[1];
    
    
    //    cout<<" axis(0)pos="<< this->m_gui_renderer->m_chart->GetAxis(0)->GetPoint1()[0]<<","<< this->m_gui_renderer->m_chart->GetAxis(0)->GetPoint1()[1];
    //    //cout<<" axis(1)pos="<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint1()[0]<<","<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint1()[1];
    //    cout<<" axis()pos2="<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint2()[0]<<","<< this->m_gui_renderer->m_chart->GetAxis(1)->GetPoint2()[1];
    
    
    //std::cout<<" {"<<local_mouse_position_xy[0]<<","<<local_mouse_position_xy[1]<<"}";
    
    int chart_p1[2];
    this->m_chart->GetPoint1(chart_p1[0],chart_p1[1]);
    int chart_p2[2];
    this->m_chart->GetPoint2(chart_p2[0],chart_p2[1]);
    double chart_p1_x = chart_p1[0];
    double chart_p1_y = chart_p1[1];
    double chart_p2_x = chart_p2[0];
    double chart_p2_y = chart_p2[1];
    //cout<<" chartP1=["<<chart_p1[0]<<","<<chart_p1[1]<<"], chartP2=["<<chart_p2[0]<<","<<chart_p2[1]<<"]";
    
    
    double x_value_range[2];
    this->m_chart->GetAxis(1)->GetRange(x_value_range);
    double y_value_range[2];
    this->m_chart->GetAxis(0)->GetRange(y_value_range);
    //cout<<" range_x=["<<x_value_range[0]<<","<<x_value_range[1]<<"], range_y=["<<y_value_range[0]<<","<<y_value_range[1]<<"]";
    
    double dx = (x_value_range[1]-x_value_range[0]) / (chart_p2_x-chart_p1_x);
    output_plot_coordinate_x = ( local_mouse_position_xy[0] - chart_p1_x ) * dx + x_value_range[0];
    
    double dy = (y_value_range[1]-y_value_range[0]) / (chart_p2_y-chart_p1_y);
    output_plot_coordinate_y = ( local_mouse_position_xy[1] - chart_p1_y ) * dy + y_value_range[0];
}



