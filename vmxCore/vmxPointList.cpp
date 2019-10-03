/*=========================================================================
 
 Program:   mipx
 Module:    vmxPointList.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxPointList.h"



vmxPointList::vmxPointList()
{
    this->SetNumberOfVisualizationComponents(2);
    
    this->GetClassName().Assign("vmxPointList");
    
	m_vtk_points = vtkSmartPointer<vtkPoints>::New();
	m_vtk_actor = vtkSmartPointer<vtkActor>::New();
	m_vtk_poly_data = vtkSmartPointer<vtkPolyData>::New();
	m_vtk_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_vtk_sphere_source = vtkSmartPointer<vtkSphereSource>::New();
	m_vtk_glyph = vtkSmartPointer<vtkGlyph3D>::New();


	m_vtk_label_point_source = vtkSmartPointer<vtkPointSource>::New();
	m_vtk_label_labels = vtkSmartPointer<vtkStringArray>::New();
	m_vtk_label_hierarchy_filter = vtkSmartPointer<vtkPointSetToLabelHierarchy>::New();
	m_vtk_label_mapper = vtkSmartPointer<vtkLabelPlacementMapper>::New();
	m_vtk_label_actor = vtkSmartPointer<vtkActor2D>::New();

	m_vtk_scalars.SetNumberOfElements( mxPoint_NUMBER_OF_VALUES );
	for(unsigned int i=0; i<m_vtk_scalars.GetNumberOfElements(); i++)
    {
        m_vtk_scalars[i] = vtkSmartPointer<vtkFloatArray>::New();
    }

	m_vtk_pick_outline_points = vtkSmartPointer<vtkPoints>::New();
	m_vtk_pick_outline_lines = vtkSmartPointer<vtkCellArray>::New();
	m_vtk_pick_outline_poly_data = vtkSmartPointer<vtkPolyData>::New();
	m_vtk_pick_outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_vtk_pick_outline_actor = vtkSmartPointer<vtkActor>::New();
    
    this->m_is_visible_labels = 0;
    this->m_is_visible_picked_point = 0;
    this->m_is_visible = 0;
    
    this->SetVisibilityOfLabels(0);
    this->SetVisibilityOfPickedPoint(0);
    
    m_renderer = NULL;

	m_picked_list_element = NULL;
    
}


vmxPointList::~vmxPointList()
{
	this->Reset();
}


int vmxPointList::Copy(vmxPointList *l)
{
    if(!l) return 0;
    if(this==l) return 1;

    this->Reset();
    if(!mxPointList::Copy(l)) return 0;
    this->Update();
    return 1;
}


int vmxPointList::GetVisibilityOfComponent(int component_index)
{
    if(component_index==0)
    {
        return m_is_visible;
    }
    if(component_index==1)
    {
        return m_is_visible_labels;
    }
    return 0;
}


int vmxPointList::HasActor(vtkActor *actor)
{
    if(m_vtk_actor==actor) return 1;
    return 0;
}


int vmxPointList::InterpolateUsingCardinalSpline(int new_number_of_points)
{
    if(new_number_of_points<=this->GetNumberOfElements()) return 0;
    if(this->GetNumberOfElements()<3) return 0;
    
    
    //--- Adjust positions ---
    vtkSmartPointer<vtkCardinalSpline> spline_x = vtkSmartPointer<vtkCardinalSpline>::New();
    vtkSmartPointer<vtkCardinalSpline> spline_y = vtkSmartPointer<vtkCardinalSpline>::New();
    vtkSmartPointer<vtkCardinalSpline> spline_z = vtkSmartPointer<vtkCardinalSpline>::New();
    
    vmxPointList::Iterator itpl;
    int i=0;
    for(itpl.SetToBegin(this->GetListOfPoints()), i=0; itpl.IsValid(); itpl.MoveToNext(), i++)
    {
        spline_x->AddPoint(i, itpl.GetElement().m_position.C());
        spline_y->AddPoint(i, itpl.GetElement().m_position.R());
        spline_z->AddPoint(i, itpl.GetElement().m_position.S());
    }
    
    //Sampling parameter 't'
    double t;
    double dt = (spline_z->GetNumberOfPoints()-1.0)/((double)(new_number_of_points-1));
    
    //Interpolating
    mxPointList new_pl;
    for(int j=0; j<new_number_of_points; j++)
    {
        t = dt*j;
        new_pl.AddPosition(spline_z->Evaluate(t), spline_y->Evaluate(t), spline_x->Evaluate(t));
    }
    //-----------------------------
    
    this->Reset();
    
    this->AddPositions(new_pl);
    
    
    //!!!!!!!!!!!! MISSING HERE: ADDING EDGES BETWEEN VERTICES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    //        //----- 2. Adjust scalars -----
    //        //Do for each of the scalar values...
    //        int noel_new = new_position_list.GetNumberOfElements();
    //        int noel_old = it.GetElementPointer()->GetPositionsList()->GetNumberOfElements();
    //        int ratio = noel_new / noel_old + 1;
    //        for(unsigned int scalar_index=0; scalar_index<this->GetNumberOfScalars(); scalar_index++)
    //        {
    //            for(int q=0; q<noel_new; q++)
    //            {
    //                new_position_list[q].V(scalar_index) = (*(it.GetElementPointer()->GetPositionsList()))[q/ratio].V(scalar_index);
    //            }
    //        }
        

    this->Update();
    
    return 1;

}


int vmxPointList::MarkPickedPoint(vtkActor *input_actor, mxPosition *pick_position)
{
    // If the point list actor was not picked return 0.
    if(m_vtk_actor.GetPointer() != input_actor) return 0;
    
    m_picked_list_element = this->GetClosestPointListElement(pick_position->S(), pick_position->R(), pick_position->C());
    mxPoint *list_position = &(m_picked_list_element->m_position);
    
    double radius = m_vtk_sphere_source->GetRadius();
    
    // Calculate coordinates for points of the outline box
    double minC, minR, minS, maxC, maxR, maxS;
    minC = list_position->C() - radius; minR = list_position->R() - radius; minS = list_position->S() - radius;
    maxC = list_position->C() + radius; maxR = list_position->R() + radius; maxS = list_position->S() + radius;
    
    double p0[3], p1[3], p2[3], p3[3], p4[3], p5[3], p6[3], p7[3];
    p0[0] = minC; p0[1] = minR; p0[2] = minS; //000
    p1[0] = maxC; p1[1] = minR; p1[2] = minS; //001
    p2[0] = minC; p2[1] = maxR; p2[2] = minS; //010
    p3[0] = maxC; p3[1] = maxR; p3[2] = minS; //011
    p4[0] = minC; p4[1] = minR; p4[2] = maxS; //100
    p5[0] = maxC; p5[1] = minR; p5[2] = maxS; //101
    p6[0] = minC; p6[1] = maxR; p6[2] = maxS; //110
    p7[0] = maxC; p7[1] = maxR; p7[2] = maxS; //111
    
    // Points
    m_vtk_pick_outline_points->Initialize();
    m_vtk_pick_outline_points->InsertNextPoint(p0); m_vtk_pick_outline_points->InsertNextPoint(p1);
    m_vtk_pick_outline_points->InsertNextPoint(p2); m_vtk_pick_outline_points->InsertNextPoint(p3);
    m_vtk_pick_outline_points->InsertNextPoint(p4); m_vtk_pick_outline_points->InsertNextPoint(p5);
    m_vtk_pick_outline_points->InsertNextPoint(p6); m_vtk_pick_outline_points->InsertNextPoint(p7);
    
    // Lines
    m_vtk_pick_outline_lines->Initialize();
    vtkSmartPointer<vtkLine> line_0_1 = vtkSmartPointer<vtkLine>::New();
    line_0_1->GetPointIds()->SetId(0,0); line_0_1->GetPointIds()->SetId(1,1);
    m_vtk_pick_outline_lines->InsertNextCell(line_0_1);
    vtkSmartPointer<vtkLine> line_0_2 = vtkSmartPointer<vtkLine>::New();
    line_0_2->GetPointIds()->SetId(0,0); line_0_2->GetPointIds()->SetId(1,2);
    m_vtk_pick_outline_lines->InsertNextCell(line_0_2);
    vtkSmartPointer<vtkLine> line_0_4 = vtkSmartPointer<vtkLine>::New();
    line_0_4->GetPointIds()->SetId(0,0); line_0_4->GetPointIds()->SetId(1,4);
    m_vtk_pick_outline_lines->InsertNextCell(line_0_4);
    vtkSmartPointer<vtkLine> line_1_5 = vtkSmartPointer<vtkLine>::New();
    line_1_5->GetPointIds()->SetId(0,1); line_1_5->GetPointIds()->SetId(1,5);
    m_vtk_pick_outline_lines->InsertNextCell(line_1_5);
    vtkSmartPointer<vtkLine> line_1_3 = vtkSmartPointer<vtkLine>::New();
    line_1_3->GetPointIds()->SetId(0,1); line_1_3->GetPointIds()->SetId(1,3);
    m_vtk_pick_outline_lines->InsertNextCell(line_1_3);
    vtkSmartPointer<vtkLine> line_2_3 = vtkSmartPointer<vtkLine>::New();
    line_2_3->GetPointIds()->SetId(0,2); line_2_3->GetPointIds()->SetId(1,3);
    m_vtk_pick_outline_lines->InsertNextCell(line_2_3);
    vtkSmartPointer<vtkLine> line_2_6 = vtkSmartPointer<vtkLine>::New();
    line_2_6->GetPointIds()->SetId(0,2); line_2_6->GetPointIds()->SetId(1,6);
    m_vtk_pick_outline_lines->InsertNextCell(line_2_6);
    vtkSmartPointer<vtkLine> line_3_7 = vtkSmartPointer<vtkLine>::New();
    line_3_7->GetPointIds()->SetId(0,3); line_3_7->GetPointIds()->SetId(1,7);
    m_vtk_pick_outline_lines->InsertNextCell(line_3_7);
    vtkSmartPointer<vtkLine> line_4_5 = vtkSmartPointer<vtkLine>::New();
    line_4_5->GetPointIds()->SetId(0,4); line_4_5->GetPointIds()->SetId(1,5);
    m_vtk_pick_outline_lines->InsertNextCell(line_4_5);
    vtkSmartPointer<vtkLine> line_4_6 = vtkSmartPointer<vtkLine>::New();
    line_4_6->GetPointIds()->SetId(0,4); line_4_6->GetPointIds()->SetId(1,6);
    m_vtk_pick_outline_lines->InsertNextCell(line_4_6);
    vtkSmartPointer<vtkLine> line_5_7 = vtkSmartPointer<vtkLine>::New();
    line_5_7->GetPointIds()->SetId(0,5); line_5_7->GetPointIds()->SetId(1,7);
    m_vtk_pick_outline_lines->InsertNextCell(line_5_7);
    vtkSmartPointer<vtkLine> line_6_7 = vtkSmartPointer<vtkLine>::New();
    line_6_7->GetPointIds()->SetId(0,6); line_6_7->GetPointIds()->SetId(1,7);
    m_vtk_pick_outline_lines->InsertNextCell(line_6_7);
    
    // Poly data and actor
    m_vtk_pick_outline_poly_data->Initialize();
    m_vtk_pick_outline_poly_data->SetPoints(m_vtk_pick_outline_points);
    m_vtk_pick_outline_poly_data->SetLines(m_vtk_pick_outline_lines);
    m_vtk_pick_outline_mapper->SetInputData(m_vtk_pick_outline_poly_data);
    m_vtk_pick_outline_actor->SetMapper(m_vtk_pick_outline_mapper);
    m_vtk_pick_outline_actor->GetProperty()->SetColor(1,0.8,0);
    m_vtk_pick_outline_actor->GetProperty()->SetAmbient(1);
    m_vtk_pick_outline_actor->GetProperty()->SetDiffuse(0);
    m_vtk_pick_outline_actor->GetProperty()->SetSpecular(0);
    m_vtk_pick_outline_actor->SetVisibility(0);
    
    return 1;
}


void vmxPointList::RemovePickedPoint()
{
    this->RemovePosition(&(m_picked_list_element->m_position));
    this->SetVisibilityOfPickedPoint(0);
    this->Update();
}


void vmxPointList::Reset()
{
//    if(m_renderer)
//    {
//        m_renderer->RemoveActor(m_vtk_actor);
//        m_renderer->RemoveActor(m_vtk_label_actor);
//        m_renderer->RemoveActor(m_vtk_pick_outline_actor);
//    }
    
    mxPointList::Reset();
//    this->Update();
}


void vmxPointList::SetColorMappingToScalars(const char *tag)
{
    unsigned int index;
    if(!this->GetScalarIndex(tag, index))
    {
        std::cout<<std::endl<<"vmxPointList::SetColorMappingToScalars(): error: cannot find specified scalar tag '"<<tag<<"'";
    }
    m_vtk_poly_data->GetPointData()->SetScalars(m_vtk_scalars[index]);
    
    double min, max;
    this->GetScalarValueRange(index, min, max);
    m_vtk_mapper->SetScalarVisibility(1);
    m_vtk_mapper->SetScalarRange(min,max);
}


void vmxPointList::SetColorMappingToSingleColor(double red, double green, double blue)
{
    m_vtk_mapper->SetScalarVisibility(0);
    m_vtk_actor->GetProperty()->SetColor(red, green, blue);
}


void vmxPointList::SetRenderer(vtkRenderer *renderer)
{
    if(!renderer) return;
    
    this->SetVisibility(0);
    this->m_renderer = renderer;
    
    m_renderer->AddActor(m_vtk_actor);
    m_renderer->AddActor(m_vtk_label_actor);
    m_renderer->AddActor(m_vtk_pick_outline_actor);
}


int vmxPointList::SetScalarVisualizationRange(unsigned int scalar_index, double range_lower_value, double range_upper_value)
{
    if(scalar_index >= this->GetMaximumNumberOfScalars()) return 0;
    
    m_scalars[scalar_index].m_scalar_visualization_range[0] = range_lower_value;
    m_scalars[scalar_index].m_scalar_visualization_range[1] = range_upper_value;
    
    return 1;
}


void vmxPointList::SetVisibility(int is_visible)
{
	m_vtk_actor->SetVisibility(is_visible);
    m_is_visible = is_visible;
    
	if(!is_visible) 
	{
        //this->SetVisibilityOfLabels(0);
        this->SetVisibilityOfPickedPoint(0);
	}
}


void vmxPointList::SetVisibilityOfComponent(int component_index, int is_visible)
{
    if(component_index==0)
    {
        this->vmxPointList::SetVisibility(is_visible);
    }
    if(component_index==1)
    {
        this->vmxPointList::SetVisibilityOfLabels(is_visible);
    }
}


void vmxPointList::SetVisibilityOfLabels(int is_visible)
{
    m_is_visible_labels = is_visible;
    m_vtk_label_actor->SetVisibility(is_visible);
}


void vmxPointList::SetVisibilityOfPickedPoint(int is_visible)
{
    m_is_visible_picked_point = is_visible;
	m_vtk_pick_outline_actor->SetVisibility(is_visible);
}


void vmxPointList::Update()
{
	//--- Glyph ---
	this->m_vtk_points->Initialize();
	this->m_vtk_poly_data->Initialize();
	for(unsigned int i=0; i<m_vtk_scalars.GetNumberOfElements(); i++) m_vtk_scalars[i]->Initialize();

    // Update points and scalars.
	int n_of_points = 0;
	mxPointList::Iterator it;
	for(it.SetToBegin(m_positions), n_of_points = 0; it.IsValid(); it.MoveToNext(), n_of_points++)
	{
        mxPoint *p = &(it.GetElementAddress()->m_position);
		
		m_vtk_points->InsertPoint(n_of_points, p->C(), p->R(), p->S());
		
		for(int j=0; j<m_vtk_scalars.GetNumberOfElements(); j++)
		{
			double v = p->V(j);
			m_vtk_scalars[j]->InsertTuple(n_of_points, &v);
		}
	}

	m_vtk_poly_data->SetPoints(m_vtk_points);
	m_vtk_poly_data->GetPointData()->SetScalars(m_vtk_scalars[0]);

	m_vtk_sphere_source->SetRadius(2.5);
	m_vtk_glyph->SetColorModeToColorByScalar();
	m_vtk_glyph->SetSourceConnection(m_vtk_sphere_source->GetOutputPort());
	m_vtk_glyph->SetInputData(m_vtk_poly_data);
	m_vtk_glyph->ScalingOff();
	m_vtk_glyph->Update();
	m_vtk_mapper->SetInputConnection(m_vtk_glyph->GetOutputPort());
    
    // vtkSmartPointer< vtkLookupTable > lut = vtkSmartPointer< vtkLookupTable >::New();

    
    if(m_scalars[0].m_scalar_visualization_range[0] >= m_scalars[0].m_scalar_visualization_range[1])
    {
        double scalar_min_value, scalar_max_value;
        this->GetScalarValueRange(0, scalar_min_value, scalar_max_value);
        this->m_vtk_mapper->SetScalarRange(scalar_min_value, scalar_max_value);
    }
    else
    {
        this->m_vtk_mapper->SetScalarRange(m_scalars[0].m_scalar_visualization_range[0], m_scalars[0].m_scalar_visualization_range[1]);
    }
	
//    vtkLookupTable *lut = dynamic_cast<vtkLookupTable*>(this->m_vtk_mapper->GetLookupTable());
    //lut->SetHueRange(0.667, 0);
//    lut->SetNumberOfTableValues(4);//how many different colors do we want...
//    lut->SetTableRange(7, 10);//(7, 10);
//    //lut->SetValueRange(7, 10);
//    lut->Build();
    this->m_vtk_mapper->ScalarVisibilityOn();
    
	m_vtk_actor->SetMapper(m_vtk_mapper);
	//---

	//--- Labels ---
	m_vtk_label_labels->Initialize();
	m_vtk_label_labels->SetName("Labels");
	m_vtk_label_labels->SetNumberOfValues(m_vtk_points->GetNumberOfPoints());
	for(it.SetToBegin(m_positions), n_of_points = 0; it.IsValid(); it.MoveToNext(), n_of_points++)
	{
		m_vtk_label_labels->SetValue(n_of_points, it.GetElementAddress()->m_label.Get_C_String());
	}

	m_vtk_label_point_source->SetNumberOfPoints(m_vtk_points->GetNumberOfPoints());
	m_vtk_label_point_source->Update();
	m_vtk_label_point_source->GetOutput()->GetPointData()->AddArray(m_vtk_label_labels);
	m_vtk_label_point_source->GetOutput()->SetPoints(m_vtk_points);

	m_vtk_label_hierarchy_filter->SetInputConnection(m_vtk_label_point_source->GetOutputPort());
	m_vtk_label_hierarchy_filter->SetLabelArrayName("Labels");
	m_vtk_label_hierarchy_filter->Update();
 
	m_vtk_label_mapper->SetMaximumLabelFraction(0.5);
	m_vtk_label_mapper->SetInputConnection(m_vtk_label_hierarchy_filter->GetOutputPort());
	m_vtk_label_actor->SetMapper(m_vtk_label_mapper);
	//---
}


int vmxPointList::UpdateScalars(unsigned int scalar_index)
{
	if(scalar_index>=this->GetMaximumNumberOfScalars()) return 0;

	// Initialize vtk scalars
	m_vtk_scalars[scalar_index]->Initialize();

	int n_of_points = 0;
	mxPointList::Iterator it;
	for(it.SetToBegin(m_positions), n_of_points=0; it.IsValid(); it.MoveToNext(), n_of_points++)
	{
        m_vtk_scalars[scalar_index]->InsertTuple(n_of_points, &(it.GetElementAddress()->m_position.V(scalar_index)));
	}

	// Set as current scalars
	m_vtk_poly_data->GetPointData()->SetScalars(m_vtk_scalars[scalar_index]);
    
    if(m_scalars[scalar_index].m_scalar_visualization_range[0] >= m_scalars[scalar_index].m_scalar_visualization_range[1])
    {
        double scalar_min_value, scalar_max_value;
        this->GetScalarValueRange(scalar_index, scalar_min_value, scalar_max_value);
        this->m_vtk_mapper->SetScalarRange(scalar_min_value, scalar_max_value);
    }
    else
    {
        this->m_vtk_mapper->SetScalarRange(m_scalars[scalar_index].m_scalar_visualization_range[0], m_scalars[scalar_index].m_scalar_visualization_range[1]);
    }
    
	this->m_vtk_mapper->ScalarVisibilityOn();
	
	return 1;
}
