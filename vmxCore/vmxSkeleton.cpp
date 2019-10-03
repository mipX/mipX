/*=========================================================================
 
 Program:   mipx
 Module:    vmxSkeleton.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxSkeleton.h"



vmxSkeleton3D::vmxSkeleton3D()
{
	m_points = vtkSmartPointer<vtkPoints>::New();
	m_lines = vtkSmartPointer<vtkCellArray>::New();
	m_actor = vtkSmartPointer<vtkActor>::New();
	mxPoint p;
	unsigned int maximum_number_of_scalars = p.GetNumberOfValues();
	m_scalars_vtk.SetNumberOfElements(maximum_number_of_scalars);
	for(unsigned int i=0; i<m_scalars_vtk.GetNumberOfElements(); i++)
    {
        m_scalars_vtk[i] = vtkSmartPointer<vtkFloatArray>::New();
    }
	m_poly_data = vtkSmartPointer<vtkPolyData>::New();
	m_clean_poly_data = vtkSmartPointer<vtkCleanPolyData>::New();
	m_tube_filter = vtkSmartPointer<vtkTubeFilter>::New();
	m_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_vtk_skeleton = NULL;
}

void vmxSkeleton3D::SetUpObject(vmxSkeleton *skeleton_vtk)
{
	m_vtk_skeleton = skeleton_vtk;
}


vmxSkeleton3D::~vmxSkeleton3D()
{
	this->Reset();
}


void vmxSkeleton3D::Reset()
{
	m_poly_data->Initialize();
}


int vmxSkeleton3D::IsVisible()
{
	return m_actor->GetVisibility();
}


void vmxSkeleton3D::SetVisibility(int is_visible)
{
	m_actor->SetVisibility(is_visible);
}



//---------------------------------------------------------------------------------------------------------------------------------------------





vmxSkeleton::vmxSkeleton()
{
    this->GetClassName().Assign("vmxSkeleton");
    
	this->m_vtk_data.SetNumberOfElements(1);
	for(unsigned int i=0; i<m_vtk_data.GetNumberOfElements(); i++) { m_vtk_data[i].SetUpObject(this); }

	m_spline_widget = vtkSmartPointer<vtkSplineWidget>::New();
	m_spline_widget_points = vtkSmartPointer<vtkPoints>::New();
	
	m_picked_link_outline_points = vtkSmartPointer<vtkPoints>::New();
	m_picked_link_outline_lines = vtkSmartPointer<vtkCellArray>::New();
	m_picked_link_outline_poly_data = vtkSmartPointer<vtkPolyData>::New();
	m_picked_link_outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_picked_link_outline_actor = vtkSmartPointer<vtkActor>::New();

    m_is_coloring_scalars = 1;
    m_is_visible = 0;
	m_index_time = 0;
	m_interactor = NULL;
    m_renderer = NULL;
	m_link_represented_with_spline_widget = NULL; 
	m_picked_link = NULL; 
	m_tube_radius_min = 0.75; 
}


vmxSkeleton::~vmxSkeleton()
{
	this->Reset();
}


void vmxSkeleton::Reset()
{
	this->mxSkeleton::Reset();
	
	for(unsigned int i=0; i<m_vtk_data.GetNumberOfElements(); i++) { m_vtk_data[i].Reset(); }
	this->m_vtk_data.SetNumberOfElements(1);
	for(unsigned int i=0; i<m_vtk_data.GetNumberOfElements(); i++) { m_vtk_data[i].SetUpObject(this); }


	if(m_renderer)
	{
		m_renderer->RemoveActor(m_picked_link_outline_actor);
	}

	m_index_time = 0;
	//m_interactor = NULL;
	m_link_represented_with_spline_widget = NULL; 
	m_picked_link = NULL; 
	m_tube_radius_min = 0.75; 
}


int vmxSkeleton::Copy(vmxSkeleton *p_skel)
{
    return this->CopyFrom(p_skel);
}


int vmxSkeleton::CopyFromDataObject(mxDataObject *data_object)
{
    if(!data_object) return 0;
    if(data_object->GetClassName()!=this->GetClassName()) return 0;
    
    vmxSkeleton *s = dynamic_cast<vmxSkeleton*> (data_object);
    
    if(!s) return 0;
    
    return (this->vmxSkeleton::Copy(s));
}



int vmxSkeleton::CopyFrom(vmxSkeleton *p_skel)
{
    std::cout<<std::endl<<" vmxSkeleton::CopyFrom(vmxSkeleton *)"<<std::endl;
    if(!p_skel) return 0;
    
    //this->Reset();
    mxSkeleton::CopyFrom(p_skel);
    
    //this->Update();
    
    return 1;
}


int vmxSkeleton::CopyFrom(vmxSkeleton &skel)
{
    return (this->CopyFrom(&skel));
}



vmxSkeleton3D& vmxSkeleton::operator()(unsigned int t)
{
	return m_vtk_data[t];
}



vtkActor* vmxSkeleton::GetActor(unsigned int t)
{
	return m_vtk_data[t].m_actor;
}


vtkActor* vmxSkeleton::GetPickedLinkOutlineActor()
{
	return this->m_picked_link_outline_actor;
}


int vmxSkeleton::HasActor(vtkActor *actor)
{
	if(!actor) return 0;
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++) 
		{ if(this->GetActor(t)==actor) return 1; }
	return 0;
}


int vmxSkeleton::IsVisible()
{
	return m_is_visible;
}


void vmxSkeleton::SetRenderer(vtkRenderer *renderer)
{
    m_renderer = renderer;
    this->SetTimeIndex_NoRender(0);
}


void vmxSkeleton::SetVisibility(int is_visible)
{
	m_is_visible = is_visible;
	m_vtk_data[m_index_time].SetVisibility(is_visible);
	if(!is_visible) m_picked_link_outline_actor->SetVisibility(is_visible);
}


void vmxSkeleton::SetInteractor(vtkRenderWindowInteractor *interactor)
{
//	if(!interactor) return;

	m_interactor = interactor;
//	this->SetTimeIndex(0);
	this->AddSplineWidgetToInteractor();
}


vtkRenderWindowInteractor* vmxSkeleton::GetInteractor()
{
	return m_interactor;
}


int vmxSkeleton::IsInteractorSet()
{
	if(m_interactor==NULL) return 0;
	else return 1;
}


void vmxSkeleton::SetTimeIndex(unsigned int t)
{
	this->SetTimeIndex_NoRender(t);
	//m_interactor->Render();
    m_renderer->Render();
}


void vmxSkeleton::SetTimeIndex_NoRender(unsigned int t)
{
	if(t>=m_vtk_data.GetNumberOfElements()) return;
	
	m_renderer->RemoveActor(m_vtk_data[m_index_time].m_actor);
	m_index_time = t;
	m_vtk_data[m_index_time].m_actor->SetVisibility(m_is_visible);
	m_renderer->AddActor(m_vtk_data[m_index_time].m_actor);
}


void vmxSkeleton::Update()
{
	if(this->m_data.GetNumberOfElements() != this->m_vtk_data.GetNumberOfElements())
	{
		for(unsigned int i=0; i<m_vtk_data.GetNumberOfElements(); i++) { m_vtk_data[i].Reset(); }
		this->m_vtk_data.SetNumberOfElements(this->GetNumberOfTimeSeries());
		for(unsigned int i=0; i<m_vtk_data.GetNumberOfElements(); i++) { m_vtk_data[i].SetUpObject(this); }
	}
	for(unsigned int t=0; t<m_vtk_data.GetNumberOfElements(); t++) { this->Update(t); }
}


void vmxSkeleton::Update(unsigned int t)
{
	vmxSkeleton3D *skl3D = &(m_vtk_data[t]);

	//Reset all the entered values
	skl3D->m_points->Initialize(); 
	skl3D->m_lines->Initialize(); 
	skl3D->m_poly_data->Initialize();
	for(unsigned int i=0; i<skl3D->m_scalars_vtk.GetNumberOfElements(); i++) skl3D->m_scalars_vtk[i]->Initialize();

	//This is used to save the number of entered points in 'this->m_points' after each FOR loop iteration
	int previous_number_of_points = 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElementAddress();
		//Enter all the points and scalars
		mxPoint link_start_node_position, link_end_node_position, link_start_node_position_world, link_end_node_position_world, pos;
		link_start_node_position = *(psl->GetNode_1_Position());
        
        this->WorldPositionFromLocalPosition(link_start_node_position.C(), link_start_node_position.R(), link_start_node_position.S(), link_start_node_position_world.C(), link_start_node_position_world.R(), link_start_node_position_world.S());
        
		link_end_node_position = *(psl->GetNode_2_Position());
        this->WorldPositionFromLocalPosition(link_end_node_position.C(), link_end_node_position.R(), link_end_node_position.S(), link_end_node_position_world.C(), link_end_node_position_world.R(), link_end_node_position_world.S());
		skl3D->m_points->InsertPoint(previous_number_of_points, link_start_node_position_world.C(), link_start_node_position_world.R(), link_start_node_position_world.S());
		
		for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
		{
			skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points,&(link_start_node_position.V(j)));
		}

		int n = psl->GetPositionsList()->GetNumberOfElements();
		for(int i=1; i<n+1; i++)
		{

			pos = psl->GetPositionsList()->operator[](i-1);
            this->WorldPositionFromLocalPosition(pos.C(), pos.R(), pos.S(), pos.C(), pos.R(), pos.S());

			skl3D->m_points->InsertPoint(previous_number_of_points+i, pos.C(), pos.R(), pos.S());
			
			for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
			{
				skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points+i,&(psl->GetPositionsList()->operator[](i-1).V(j)));
			}
		}

		skl3D->m_points->InsertPoint(previous_number_of_points+n+1, link_end_node_position_world.C(), link_end_node_position_world.R(), link_end_node_position_world.S());
		
		for(unsigned int j=0; j<skl3D->m_scalars_vtk.GetNumberOfElements(); j++)
		{
			skl3D->m_scalars_vtk[j]->InsertTuple(previous_number_of_points+n+1,&(link_end_node_position.V(j)));
		}

		//The number of points that will be used for the cell is 'new_number_of_points - previous_number_of_points'.
		skl3D->m_lines->InsertNextCell(skl3D->m_points->GetNumberOfPoints() - previous_number_of_points);
		for(int i=previous_number_of_points; i<skl3D->m_points->GetNumberOfPoints(); i++){ skl3D->m_lines->InsertCellPoint(i); }

		//Save the new 'previous_number_of_points' value.
		previous_number_of_points = skl3D->m_points->GetNumberOfPoints();
	}

	skl3D->m_poly_data->SetPoints(skl3D->m_points);
	skl3D->m_poly_data->SetLines(skl3D->m_lines);

	//Add scalars with index 0 as current scalars
	skl3D->m_poly_data->GetPointData()->SetScalars(skl3D->m_scalars_vtk[0]);


	//Remove duplicates to avoid the vtkTubeFilter "can't create normals" error
	skl3D->m_clean_poly_data->SetInputData(skl3D->m_poly_data);


	//Add thickness to the resulting line.
	skl3D->m_tube_filter->SetNumberOfSides(6);
	skl3D->m_tube_filter->SetInputConnection(skl3D->m_clean_poly_data->GetOutputPort());//this->m_tube_filter->SetInputData(this->m_poly_data);
	skl3D->m_tube_filter->SetRadius(this->m_tube_radius_min);


	skl3D->m_mapper->SetInputConnection(skl3D->m_tube_filter->GetOutputPort());
    
    if(this->GetScalarTagElement(0)->IsVisualizationRangeSet())
    {
        skl3D->m_mapper->SetScalarRange(this->GetScalarTagElement(0)->GetVisualizationRange()[0], this->GetScalarTagElement(0)->GetVisualizationRange()[1]);
    }
    else
    {
        double scalar_min_value, scalar_max_value;
        this->GetScalarMinAndMaxValue(0,scalar_min_value,scalar_max_value);
        if(scalar_min_value>=scalar_max_value){ scalar_max_value = scalar_min_value + 1; }
        skl3D->m_mapper->SetScalarRange(scalar_min_value, scalar_max_value);//for scalars
    }

	if(m_is_coloring_scalars) skl3D->m_mapper->ScalarVisibilityOn();
    else skl3D->m_mapper->ScalarVisibilityOff();
	
	skl3D->m_actor->SetMapper(skl3D->m_mapper);
}


int vmxSkeleton::UpdateScalars(unsigned int scalar_index)
{
	for(unsigned int t=0; t<m_vtk_data.GetNumberOfElements(); t++){ if(!this->UpdateScalars(scalar_index,t)) return 0; }
	return 1;
}


int vmxSkeleton::UpdateScalars(unsigned int scalar_index, unsigned int t)
{
	if(scalar_index>=this->GetMaximumNumberOfScalars()) return 0;

	vmxSkeleton3D *skl3D = &(m_vtk_data[t]);

	//Reset the scalar values
	skl3D->m_scalars_vtk[scalar_index]->Initialize();

	//This is used to save the number of entered points in 'this->m_points' after each FOR loop iteration
	int previous_number_of_points = 0;

	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		mxSkeletonLink *psl = it.GetElementAddress();
		//Enter all the points and scalars
		mxPoint link_start_node_position, link_end_node_position;
		link_start_node_position = *(psl->GetNode_1_Position());
		link_end_node_position = *(psl->GetNode_2_Position());
		
		skl3D->m_scalars_vtk[scalar_index]->InsertTuple(previous_number_of_points,&(link_start_node_position.V(scalar_index)));

		int n = psl->GetPositionsList() ->GetNumberOfElements();
		for(int i=1; i<n+1; i++)
		{
			skl3D->m_scalars_vtk[scalar_index]->InsertTuple(previous_number_of_points+i,&(psl->GetPositionsList()->operator[](i-1).V(scalar_index)));
		}

		skl3D->m_scalars_vtk[scalar_index]->InsertTuple(previous_number_of_points+n+1,&(link_end_node_position.V(scalar_index)));
		
		//Save the new 'previous_number_of_points' value.
		previous_number_of_points = skl3D->m_scalars_vtk[scalar_index]->GetNumberOfTuples();
	}

	//Add modified scalars as current scalars
	skl3D->m_poly_data->GetPointData()->SetScalars(skl3D->m_scalars_vtk[scalar_index]);
    
    if(this->GetScalarTagElement(scalar_index)->IsVisualizationRangeSet())
    {
        skl3D->m_mapper->SetScalarRange(this->GetScalarTagElement(scalar_index)->GetVisualizationRange()[0], this->GetScalarTagElement(scalar_index)->GetVisualizationRange()[1]);
    }
    else
    {
        double scalar_min_value, scalar_max_value;
        this->GetScalarMinAndMaxValue(scalar_index,scalar_min_value,scalar_max_value);
        if(scalar_min_value>=scalar_max_value){ scalar_max_value = scalar_min_value + 1;}
        skl3D->m_mapper->SetScalarRange(scalar_min_value, scalar_max_value);
    }
    
    if(m_is_coloring_scalars) skl3D->m_mapper->ScalarVisibilityOn();
    else skl3D->m_mapper->ScalarVisibilityOff();
	
	return 1;
}


void vmxSkeleton::ExportMeshToSTLFile(const char *file_name, unsigned int t)
{
	vmxSkeleton3D *skl3D = &(m_vtk_data[t]);
	// Save STL 
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputConnection(skl3D->m_tube_filter->GetOutputPort());
	normals->ConsistencyOn();
	normals->SplittingOff();
	normals->Update();
	vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New(); 
	stlWriter->SetFileName(file_name); 
	stlWriter->SetInputConnection(normals->GetOutputPort()); 
	stlWriter->Write(); 
}


void vmxSkeleton::ExportMeshToVTKFile(const char *file_name, unsigned int t)
{
	vmxSkeleton3D *skl3D = &(m_vtk_data[t]);
	vtkSmartPointer<vtkGenericDataObjectWriter> writer = vtkSmartPointer<vtkGenericDataObjectWriter>::New();
	writer->SetFileName(file_name);
	writer->SetInputData(skl3D->m_tube_filter->GetOutputDataObject(0));
	writer->Write();
}


void vmxSkeleton::ConvertWorldPositionToSkeletonPosition(mxPoint &input_world_position, mxPoint &output_skeleton_position)
{
    this->LocalPositionFromWorldPosition(input_world_position.C(), input_world_position.R(), input_world_position.S(), output_skeleton_position.C(), output_skeleton_position.R(), output_skeleton_position.S());
    for(int i=0; i<input_world_position.GetNumberOfValues(); i++)
    {
        output_skeleton_position.V(i) = input_world_position.V(i);
    }
    output_skeleton_position.T() = input_world_position.T();
}


int vmxSkeleton::AddSplineWidgetToInteractor()
{
	if(m_interactor==NULL) return 0;
	m_spline_widget->SetInteractor(m_interactor);
	m_spline_widget->On();
	m_spline_widget->Off();

	return 1;
}


int vmxSkeleton::MarkSkeletonLinkForGivenActorAndPickWorldPosition(vtkActor *input_actor, mxPoint &pick_position)
{
	//The picked actor is not the actor of this object, return fail.
	vmxSkeleton3D *skl3D = NULL;
	for(unsigned int i=0; i<m_vtk_data.GetNumberOfElements(); i++)
	{
		if(m_vtk_data[i].m_actor==input_actor) { skl3D = &(m_vtk_data[i]); break; }
	}

	if(!skl3D) return 0;

	m_renderer->RemoveActor(m_picked_link_outline_actor);

	mxSkeletonLink *p_found_link;
	mxPoint input_position;
	this->ConvertWorldPositionToSkeletonPosition(pick_position,input_position);
	if(!this->GetLinkClosestToPosition(input_position, &p_found_link)) return 0;
	
	m_picked_link = p_found_link;

	//Find the positions of the points of the outline box
	double minC, minR, minS, maxC, maxR, maxS;
	mxPoint *ppos;
	mxSkeletonNode *psn;
	psn = m_picked_link->GetNode_1();
	minC = maxC = psn->GetPosition() .C(); minR = maxR = psn->GetPosition() .R(); minS = maxS = psn->GetPosition() .S();
	
	mxListIterator<mxPoint> it;
	for(it.SetToBegin(m_picked_link->GetPositionsList()); it.IsValid(); it.MoveToNext())
	{
		ppos = it.GetElementAddress();
		if(ppos->C() > maxC) maxC = ppos->C(); if(ppos->C() < minC) minC = ppos->C();
		if(ppos->R() > maxR) maxR = ppos->R(); if(ppos->R() < minR) minR = ppos->R();
		if(ppos->S() > maxS) maxS = ppos->S(); if(ppos->S() < minS) minS = ppos->S();
	}
	psn = m_picked_link->GetNode_2();
	if(psn->GetPosition() .C() > maxC) maxC = psn->GetPosition() .C(); if(psn->GetPosition() .C() < minC) minC = psn->GetPosition() .C();
	if(psn->GetPosition() .R() > maxR) maxR = psn->GetPosition() .R(); if(psn->GetPosition() .R() < minR) minR = psn->GetPosition() .R();
	if(psn->GetPosition() .S() > maxS) maxS = psn->GetPosition() .S(); if(psn->GetPosition() .S() < minS) minS = psn->GetPosition() .S();
	//Convert the found coordinates to world coordinates
	double minX, minY, minZ, maxX, maxY, maxZ;
    
    this->WorldPositionFromLocalPosition(minC, minR, minS, minX, minY, minZ);
    this->WorldPositionFromLocalPosition(maxC, maxR, maxS, maxX, maxY, maxZ);
    
	double p0[3], p1[3], p2[3], p3[3], p4[3], p5[3], p6[3], p7[3];
	p0[0] = minX; p0[1] = minY; p0[2] = minZ; //000
	p1[0] = maxX; p1[1] = minY; p1[2] = minZ; //001
	p2[0] = minX; p2[1] = maxY; p2[2] = minZ; //010
	p3[0] = maxX; p3[1] = maxY; p3[2] = minZ; //011
	p4[0] = minX; p4[1] = minY; p4[2] = maxZ; //100
	p5[0] = maxX; p5[1] = minY; p5[2] = maxZ; //101
	p6[0] = minX; p6[1] = maxY; p6[2] = maxZ; //110
	p7[0] = maxX; p7[1] = maxY; p7[2] = maxZ; //111
	//Create the link outline actor

	//Enter points
	m_picked_link_outline_points->Initialize();
	m_picked_link_outline_points->InsertNextPoint(p0); m_picked_link_outline_points->InsertNextPoint(p1);
	m_picked_link_outline_points->InsertNextPoint(p2); m_picked_link_outline_points->InsertNextPoint(p3);
	m_picked_link_outline_points->InsertNextPoint(p4); m_picked_link_outline_points->InsertNextPoint(p5);
	m_picked_link_outline_points->InsertNextPoint(p6); m_picked_link_outline_points->InsertNextPoint(p7);

	//Enter lines
	m_picked_link_outline_lines->Initialize();
	vtkSmartPointer<vtkLine> line_0_1 = vtkSmartPointer<vtkLine>::New();
	line_0_1->GetPointIds()->SetId(0,0); line_0_1->GetPointIds()->SetId(1,1);
	m_picked_link_outline_lines->InsertNextCell(line_0_1);
	vtkSmartPointer<vtkLine> line_0_2 = vtkSmartPointer<vtkLine>::New();
	line_0_2->GetPointIds()->SetId(0,0); line_0_2->GetPointIds()->SetId(1,2);
	m_picked_link_outline_lines->InsertNextCell(line_0_2);
	vtkSmartPointer<vtkLine> line_0_4 = vtkSmartPointer<vtkLine>::New();
	line_0_4->GetPointIds()->SetId(0,0); line_0_4->GetPointIds()->SetId(1,4);
	m_picked_link_outline_lines->InsertNextCell(line_0_4);
	vtkSmartPointer<vtkLine> line_1_5 = vtkSmartPointer<vtkLine>::New();
	line_1_5->GetPointIds()->SetId(0,1); line_1_5->GetPointIds()->SetId(1,5);
	m_picked_link_outline_lines->InsertNextCell(line_1_5);
	vtkSmartPointer<vtkLine> line_1_3 = vtkSmartPointer<vtkLine>::New();
	line_1_3->GetPointIds()->SetId(0,1); line_1_3->GetPointIds()->SetId(1,3);
	m_picked_link_outline_lines->InsertNextCell(line_1_3);
	vtkSmartPointer<vtkLine> line_2_3 = vtkSmartPointer<vtkLine>::New();
	line_2_3->GetPointIds()->SetId(0,2); line_2_3->GetPointIds()->SetId(1,3);
	m_picked_link_outline_lines->InsertNextCell(line_2_3);
	vtkSmartPointer<vtkLine> line_2_6 = vtkSmartPointer<vtkLine>::New();
	line_2_6->GetPointIds()->SetId(0,2); line_2_6->GetPointIds()->SetId(1,6);
	m_picked_link_outline_lines->InsertNextCell(line_2_6);
	vtkSmartPointer<vtkLine> line_3_7 = vtkSmartPointer<vtkLine>::New();
	line_3_7->GetPointIds()->SetId(0,3); line_3_7->GetPointIds()->SetId(1,7);
	m_picked_link_outline_lines->InsertNextCell(line_3_7);
	vtkSmartPointer<vtkLine> line_4_5 = vtkSmartPointer<vtkLine>::New();
	line_4_5->GetPointIds()->SetId(0,4); line_4_5->GetPointIds()->SetId(1,5);
	m_picked_link_outline_lines->InsertNextCell(line_4_5);
	vtkSmartPointer<vtkLine> line_4_6 = vtkSmartPointer<vtkLine>::New();
	line_4_6->GetPointIds()->SetId(0,4); line_4_6->GetPointIds()->SetId(1,6);
	m_picked_link_outline_lines->InsertNextCell(line_4_6);
	vtkSmartPointer<vtkLine> line_5_7 = vtkSmartPointer<vtkLine>::New();
	line_5_7->GetPointIds()->SetId(0,5); line_5_7->GetPointIds()->SetId(1,7);
	m_picked_link_outline_lines->InsertNextCell(line_5_7);
	vtkSmartPointer<vtkLine> line_6_7 = vtkSmartPointer<vtkLine>::New();
	line_6_7->GetPointIds()->SetId(0,6); line_6_7->GetPointIds()->SetId(1,7);
	m_picked_link_outline_lines->InsertNextCell(line_6_7);

	//Create the poly data, mapper and actor
	m_picked_link_outline_poly_data->Initialize();
	m_picked_link_outline_poly_data->SetPoints(m_picked_link_outline_points);
	m_picked_link_outline_poly_data->SetLines(m_picked_link_outline_lines);
	m_picked_link_outline_mapper->SetInputData(m_picked_link_outline_poly_data);
	m_picked_link_outline_actor->SetMapper(m_picked_link_outline_mapper);
	m_picked_link_outline_actor->GetProperty()->SetColor(1,0.8,0);
	m_picked_link_outline_actor->GetProperty()->SetAmbient(1);
	m_picked_link_outline_actor->GetProperty()->SetDiffuse(0);
	m_picked_link_outline_actor->GetProperty()->SetSpecular(0);
	m_picked_link_outline_actor->SetVisibility(0);

	m_renderer->AddActor(m_picked_link_outline_actor);

	return 1;
}


void vmxSkeleton::PruningOrder()
{
	this->SetLinkMetricsAsStubLinkRemovalStepNumber();
	mxString tag; tag.Append("Pruning Order");
	this->SetLinkValuesFromLinkMetrics(tag);
	unsigned int scalar_index;
	if(this->GetScalarTagIndex(tag,scalar_index)) this->UpdateScalars(scalar_index);
	else this->Update();
}


void vmxSkeleton::HierarchyFromRoot(mxSkeletonLink* root)
{
	if(!this->SetLinkMetricsToHierarchyFromRoot(root)) return;
	mxString tag; tag.Append("Hierarchy From Root");
	this->SetLinkValuesFromLinkMetrics(tag);
	unsigned int scalar_index;
	if(this->GetScalarTagIndex(tag,scalar_index)) this->UpdateScalars(scalar_index);
}


void vmxSkeleton::HierarchyFromRoot()
{
	this->HierarchyFromRoot(m_picked_link);
}


int vmxSkeleton::RepresentPickedLinkAsSplineWidget()
{
	//If there is already a link represented with the spline widget, do not allow one more representation!
	if(m_link_represented_with_spline_widget != NULL) return 0;

	if(m_picked_link == NULL) return 0;
	unsigned int t;
	if(!this->TimeIndexForLink(m_picked_link, t)) return 0;

	//--- Create a spline widget from the given link ---
	//this->m_spline_widget_points->Reset();
	this->m_spline_widget_points->Initialize();
	
	//Enter all the points and scalars
	mxPoint link_start_node_position, link_end_node_position, pos;
	link_start_node_position = *(m_picked_link->GetNode_1_Position());
    this->WorldPositionFromLocalPosition(link_start_node_position.C(), link_start_node_position.R(), link_start_node_position.S(), link_start_node_position.C(), link_start_node_position.R(), link_start_node_position.S());
	link_end_node_position = *(m_picked_link->GetNode_2_Position());
    this->WorldPositionFromLocalPosition(link_end_node_position.C(), link_end_node_position.R(), link_end_node_position.S(), link_end_node_position.C(), link_end_node_position.R(), link_end_node_position.S());
	this->m_spline_widget_points->InsertPoint(0, link_start_node_position.C(), link_start_node_position.R(), link_start_node_position.S());
	int n = m_picked_link->GetPositionsList() ->GetNumberOfElements();
	for(int i=1; i<n+1; i++)
	{
		pos = m_picked_link->GetPositionsList()->operator[](i-1);
        this->WorldPositionFromLocalPosition(pos.C(), pos.R(), pos.S(), pos.C(), pos.R(), pos.S());
		this->m_spline_widget_points->InsertPoint(i, pos.C(), pos.R(), pos.S());
	}
	this->m_spline_widget_points->InsertPoint(n+1, link_end_node_position.C(), link_end_node_position.R(), link_end_node_position.S());
	
	if(this->m_spline_widget_points->GetNumberOfPoints()==0) return 0;//return 0 because there are no points entered.

	//Make the skeleton actor transparent (opacity to 0.5)
	this->m_vtk_data[t].m_actor->GetProperty()->SetOpacity(0.4);

	this->m_spline_widget->InitializeHandles(this->m_spline_widget_points);
	this->m_spline_widget->SetHandleSize(0.002);
	this->m_spline_widget->GetHandleProperty()->SetColor(0.9,0.9,0.9);
	this->m_spline_widget->GetLineProperty()->SetColor(0.7,0,0.7);
	//---

	//Adds the spline widget to the input interactor
	this->m_spline_widget->On();

	//Set the pointer to the link represented with spline widget
	this->m_link_represented_with_spline_widget = m_picked_link;

	return 1;
}


int vmxSkeleton::TurnOffRepresentationOfMemorizedLinkAsSplineWidget()
{
	//If no link is represented with the spline widget, return 0
	if(m_link_represented_with_spline_widget==NULL) return 0;

	unsigned int t;
	if(!this->TimeIndexForLink(m_link_represented_with_spline_widget, t)) return 0;

	//Remove the Spline Widget from the interactor
	m_spline_widget->Off();

	//Read all the Handle positions and re-adjust the link and connecting nodes!
	int n_of_handles = m_spline_widget->GetNumberOfHandles();

	if(this->IsLinkInSkeleton(m_link_represented_with_spline_widget))
	{
		if(n_of_handles>=2)
		{
			//Delete all the positions from the link - they will be re-entered from the Spline Widget
			m_link_represented_with_spline_widget->RemoveAllPositionsAndSkeletonizationVoxels();

			double xyz[3];
			for(int i=0; i<n_of_handles; i++)
			{
				m_spline_widget->GetHandlePosition(i,xyz);
				mxPoint pos1, pos2;//'pos2' represents the position to add/modify
				pos1.SetPosition(xyz[2],xyz[1],xyz[0]);
				this->ConvertWorldPositionToSkeletonPosition(pos1,pos2);

				//If the handle represents the start node modify the position of the node in the Skeleton
				if(i==0) m_link_represented_with_spline_widget->GetNode_1()->GetPosition()  = pos2;
				
				//If the handle represents the end node modify the position of the node in the Skeleton
				if(i==n_of_handles-1) m_link_represented_with_spline_widget->GetNode_2()->GetPosition()  = pos2;

				//If the handle does not represent any nodes we modify the link itself...
				if(i>0 && i<n_of_handles-1) m_link_represented_with_spline_widget->GetPositionsList()->AddToEnd(pos2);
			}
		}
	}
	
	//Update the skeleton
	this->Update();

	m_vtk_data[t].m_actor->GetProperty()->SetOpacity(1);

	//Reset the pointer to the link that is represented with the spline widget
	m_link_represented_with_spline_widget = NULL;

	return 1;
}


void vmxSkeleton::ToggleLinkAsSplineWidgetRepresentation()
{
	if(m_link_represented_with_spline_widget==NULL) 
		this->RepresentPickedLinkAsSplineWidget();
	else this->TurnOffRepresentationOfMemorizedLinkAsSplineWidget();
}


void vmxSkeleton::SetColoringScalars(mxString &tag)
{
    m_is_coloring_scalars = 1;
    
	for(unsigned int t=0; t<m_vtk_data.GetNumberOfElements(); t++)
	{
		unsigned int index;
		this->GetScalarTagIndex(tag, index);
		m_vtk_data[t].m_poly_data->GetPointData()->SetScalars(m_vtk_data[t].m_scalars_vtk[index]);
		double min, max;
		this->GetScalarMinAndMaxValue(index,min,max);
		m_vtk_data[t].m_mapper->SetScalarVisibility(1);
		m_vtk_data[t].m_mapper->SetScalarRange(min,max);
	}
}


void vmxSkeleton::SetColoringToSingleColor(double red, double green, double blue)
{
    m_is_coloring_scalars = 0;
    
	for(unsigned int t=0; t<m_vtk_data.GetNumberOfElements(); t++)
	{
		m_vtk_data[t].m_mapper->SetScalarVisibility(0);
		m_vtk_data[t].m_actor->GetProperty()->SetColor(red, green, blue);
	}
}


int vmxSkeleton::SetScalarVisualizationRange(unsigned int scalar_index, double range_lower_value, double range_upper_value)
{
    if(scalar_index >= this->GetMaximumNumberOfScalars()) return 0;
    
    m_scalars[scalar_index].m_scalar_visualization_range[0] = range_lower_value;
    m_scalars[scalar_index].m_scalar_visualization_range[1] = range_upper_value;
    
    return 1;
}



void vmxSkeleton::SetVisibilityOfComponent(int component_index, int is_visible)
{
    this->m_is_visible = is_visible;
    this->m_vtk_data[m_index_time].SetVisibility(is_visible);
}


int vmxSkeleton::InterpolateWithCardinalSpline(int interpolation_coefficient)
{
	if(interpolation_coefficient<3 || interpolation_coefficient>10) interpolation_coefficient = 3;// we take 3 as minimum value, because links with only 1 position
	// will produce 2 positions. Empty links will remain empty (no need to interpolate straight lines).
	
    this->MergeNodeWithLinksForAllSkeletonNodes();
    
	for(unsigned int t=0; t<m_vtk_data.GetNumberOfElements(); t++)
	{
		mxListIterator<mxSkeletonLink> it;
		for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
		{
			//If there are no positions in the link, insert one position in between the nodes
			if(it.GetElementAddress()->GetNumberOfPositions()==0)
			{
				mxPoint *pos = it.GetElementAddress()->GetPositionsList()->AddNewToEnd();
				pos->S() = ( it.GetElementAddress()->GetNode_1_Position()->S() + it.GetElementAddress()->GetNode_2_Position()->S() ) / 2.0;
				pos->R() = ( it.GetElementAddress()->GetNode_1_Position()->R() + it.GetElementAddress()->GetNode_2_Position()->R() ) / 2.0;
				pos->C() = ( it.GetElementAddress()->GetNode_1_Position()->C() + it.GetElementAddress()->GetNode_2_Position()->C() ) / 2.0;
			}

			mxList< mxPoint > new_position_list;

			int number_of_output_points = it.GetElementAddress()->GetNumberOfPositions() + (it.GetElementAddress()->GetNumberOfPositions())*interpolation_coefficient;

			//----- 1. Adjust positions -----
			vtkSmartPointer<vtkCardinalSpline> spline_x = vtkSmartPointer<vtkCardinalSpline>::New();
			vtkSmartPointer<vtkCardinalSpline> spline_y = vtkSmartPointer<vtkCardinalSpline>::New();
			vtkSmartPointer<vtkCardinalSpline> spline_z = vtkSmartPointer<vtkCardinalSpline>::New();

			//Enter the points into the splines
			mxPoint *pp = it.GetElementAddress()->GetNode_1_Position();
			spline_x->AddPoint(0, pp->C()); spline_y->AddPoint(0, pp->R()); spline_z->AddPoint(0, pp->S());
			int i=1;
			mxListIterator<mxPoint> itp;
			for(itp.SetToBegin(it.GetElementAddress()->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
			{
				spline_x->AddPoint(i, itp.GetElementAddress()->C()); spline_y->AddPoint(i, itp.GetElementAddress()->R()); spline_z->AddPoint(i, itp.GetElementAddress()->S());
				i++;
			}
			pp = it.GetElementAddress()->GetNode_2_Position();
			spline_x->AddPoint(i, pp->C()); spline_y->AddPoint(i, pp->R()); spline_z->AddPoint(i, pp->S());

			//Sampling parameter 't'
			double t;
			double dt = (spline_z->GetNumberOfPoints()-1.0)/((double)(number_of_output_points-1));

			//Interpolating
			for(int j=1; j<number_of_output_points-1; j++)//we don't take positions with indexes 0 and number_of_output_points-1 into account (they are nodes).
			{
				t = dt*j;
				pp = new_position_list.AddNewToEnd();
				pp->C() = spline_x->Evaluate(t);
				pp->R() = spline_y->Evaluate(t);
				pp->S() = spline_z->Evaluate(t);
			}
			//-----------------------------


			//----- 2. Adjust scalars -----
			//Do for each of the scalar values...
			int noel_new = new_position_list.GetNumberOfElements();
			int noel_old = it.GetElementAddress()->GetPositionsList()->GetNumberOfElements();
			int ratio = noel_new / noel_old + 1;
			//for(unsigned int scalar_index=0; scalar_index<this->GetNumberOfScalars(); scalar_index++)
            for(unsigned int scalar_index=0; scalar_index<this->GetMaximumNumberOfScalars(); scalar_index++)
			{
				for(int q=0; q<noel_new; q++)
				{
					new_position_list[q].V(scalar_index) = (*(it.GetElementAddress()->GetPositionsList()))[q/ratio].V(scalar_index);
				}
			}



			*(it.GetElementAddress()->GetPositionsList()) = new_position_list;

		}
	}
	this->Update();

	return 1;
}





