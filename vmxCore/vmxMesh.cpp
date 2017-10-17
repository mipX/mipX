/*=========================================================================
 
 Program:   mipx
 Module:    vmxMesh.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "vmxMesh.h"



vmxMesh3D::vmxMesh3D()
{
    this->m_actor = vtkSmartPointer<vtkActor>::New();
	this->m_poly_data = vtkSmartPointer<vtkPolyData>::New();
    this->m_poly_data_connectivity_filter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	this->m_poly_data_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
}


vmxMesh3D::~vmxMesh3D()
{
    this->vmxMesh3D::Reset();
}


void vmxMesh3D::CreatePolyData(vtkImageData *image, int threshold, double Cx, double Cy, double Cz, double Rx, double Ry, double Rz, double Sx, double Sy, double Sz)
{
    // Use contour filter to produce contours of poly data.
    vtkSmartPointer<vtkContourFilter> contour_filter = vtkSmartPointer<vtkContourFilter>::New();
    contour_filter->SetInputData(image);
    contour_filter->SetValue(0,threshold);
    contour_filter->Update();
    this->m_poly_data->DeepCopy( contour_filter->GetOutput() );
    
//    // Orientation matrix.
//    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
//    matrix->SetElement(0, 0, Rx); matrix->SetElement(0, 1, Ry); matrix->SetElement(0, 2, Rz); matrix->SetElement(0, 3, 0);
//    matrix->SetElement(1, 0, Cx); matrix->SetElement(1, 1, Cy); matrix->SetElement(1, 2, Cz); matrix->SetElement(1, 3, 0);
//    matrix->SetElement(2, 0, Sx); matrix->SetElement(2, 1, Sy); matrix->SetElement(2, 2, Sz); matrix->SetElement(2, 3, 0);
//    matrix->SetElement(3, 0,  0); matrix->SetElement(3, 1,  0); matrix->SetElement(3, 2,  0); matrix->SetElement(3, 3, 1);
//    
//    // Transform that takes orientation matrix as input.
//    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
//    transform->SetMatrix(matrix);
//    double orientation[3];
//    transform->GetOrientation(orientation);
//    
//    this->m_actor->SetOrientation(orientation);
    //this->m_actor->SetOrigin((this->m_actor->GetOrigin()[0]+image->GetOrigin()[0]), (this->m_actor->GetOrigin()[1]+image->GetOrigin()[1]), (this->m_actor->GetOrigin()[2]+image->GetOrigin()[2]));
}


void vmxMesh3D::CreateActorByDifferentlyColoringDisjointComponents()
{
    this->m_poly_data_connectivity_filter->SetInputData(this->m_poly_data);
    this->m_poly_data_connectivity_filter->SetExtractionModeToAllRegions();
    this->m_poly_data_connectivity_filter->ColorRegionsOn();
    this->m_poly_data_connectivity_filter->Update();
    
    this->m_poly_data_mapper->SetInputConnection( this->m_poly_data_connectivity_filter->GetOutputPort() );
    this->m_poly_data_mapper->SetScalarRange( this->m_poly_data_connectivity_filter->GetOutput()->GetPointData()->GetArray("RegionId")->GetRange() );
    this->m_poly_data_mapper->Update();
    
    this->m_actor->SetMapper(this->m_poly_data_mapper);
}


void vmxMesh3D::CreateActorByLookupTableScalarColoring(vtkLookupTable *lookup_table)
{
    //cout<<"scalar_min_value="<<scalar_min_value<<",scalar_max_value="<<scalar_max_value<<endl;
    
    this->m_poly_data_mapper->SetInputData(this->m_poly_data);
    this->m_poly_data_mapper->SetScalarVisibility(1); //indicates that scalars from the lookup table should be used
    this->m_poly_data_mapper->SetColorModeToMapScalars();
    this->m_poly_data_mapper->SetScalarRange(lookup_table->GetRange()[0], lookup_table->GetRange()[1]);
    this->m_poly_data_mapper->SetLookupTable(lookup_table);
    this->m_poly_data_mapper->Update();
    
    this->m_actor->SetMapper(this->m_poly_data_mapper);
}


void vmxMesh3D::GetColor(double &red, double &green, double &blue)
{
    this->m_actor->GetProperty()->GetColor(red,green,blue);
}


double vmxMesh3D::GetOpacity()
{
    return (this->m_actor->GetProperty()->GetOpacity());
}


void vmxMesh3D::Reset()
{
	this->m_poly_data->Initialize();
}


void vmxMesh3D::SetColor(double red, double green, double blue)
{
	this->m_poly_data_mapper->SetScalarVisibility(0); // turn off scalar visibility.
	this->m_actor->GetProperty()->SetColor(red,green,blue);
	this->m_actor->Modified();
}


void vmxMesh3D::SetOpacity(double opacity)
{
	m_actor->GetProperty()->SetOpacity(opacity);
}


void vmxMesh3D::SetVisibility(int is_visible)
{
	m_actor->SetVisibility(is_visible);
}



//------------------------------------------------------------------------------------------------------------------



vmxMesh::vmxMesh()
{
    this->m_class_name.Assign("vmxMesh");
	this->m_data.SetNumberOfElements(1);
	this->m_index_T = 0;
	this->m_is_visible = 0;
    this->m_lookup_table = vtkSmartPointer<vtkLookupTable>::New();
    this->m_renderer = NULL;
}


vmxMesh::~vmxMesh()
{
    this->vmxMesh::Reset();
    this->m_renderer = NULL;
}


int vmxMesh::CopyFromDataObject(mxDataObject *data_object)
{
    if(!data_object) return 0;
    
    vmxMesh *mesh = dynamic_cast<vmxMesh*>(data_object);
    if(!mesh) return 0;
    
    return (this->vmxMesh::Copy(mesh));
}


int vmxMesh::Copy(vmxMesh *mesh)
{
    this->vmxMesh::Reset();
    
    if(!this->mxDataObject::CopyFromDataObject(mesh)) return 0;
    
    this->m_data.SetNumberOfElements(mesh->GetDimension_T());
    
    for(unsigned int t=0; t<this->m_data.GetNumberOfElements(); t++)
    {
        this->m_data[t].m_poly_data->DeepCopy( mesh->m_data[t].m_poly_data );
    }
    
    return 1;
}


void vmxMesh::CreateActorByDifferentlyColoringDisjointComponents()
{
    for(unsigned int t=0; t<this->GetDimension_T(); t++)
    {
        this->m_data[t].CreateActorByDifferentlyColoringDisjointComponents();
    }
}


void vmxMesh::CreateActorByLookupTableScalarColoring(double scalar_min_value, double scalar_max_value)
{
    this->m_lookup_table->SetTableRange(scalar_min_value, scalar_max_value);
    this->m_lookup_table->SetHueRange(0.0,0.667);
    this->m_lookup_table->Build();
    
    for(unsigned int t=0; t<this->GetDimension_T(); t++)
    {
        this->m_data[t].CreateActorByLookupTableScalarColoring(this->m_lookup_table);
    }
}


void vmxMesh::CreatePolyData(mxBasicImage *image, int threshold)
{
    this->vmxMesh::Reset();
    this->m_data.SetNumberOfElements(image->GetDimension_T());
    
    for(unsigned int t=0; t<image->GetDimension_T(); t++)
    {
        int scalar_size = image->GetScalarSizeInBytes();
        
        vtkSmartPointer<vtkImageData> image_data = vtkSmartPointer<vtkImageData>::New();
        image_data->SetDimensions(image->GetDimension_C(),image->GetDimension_R(),image->GetDimension_S());
        image_data->SetOrigin(image->GetOrigin_C(),image->GetOrigin_R(),image->GetOrigin_S());
        image_data->SetSpacing(image->GetSpacing_C(),image->GetSpacing_R(),image->GetSpacing_S());
        
        // Allocate scalars and copy the 3D part of the image for current t index to vtk image data.
        switch(scalar_size)
        {
            case 1:
            {
                image_data->AllocateScalars(VTK_TYPE_UINT8, 1);
                for(unsigned int s=0; s<image->GetDimension_S(); s++)
                {
                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
                    {
                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
                        {
                            uint8_t *v = static_cast<uint8_t*>(image_data->GetScalarPointer(c,r,s));
                            *v = (uint8_t) image->Get(t,s,r,c);
                        }
                    }
                }
                break;
            }
            case 2:
            {
                image_data->AllocateScalars(VTK_TYPE_UINT16, 1);
                for(unsigned int s=0; s<image->GetDimension_S(); s++)
                {
                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
                    {
                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
                        {
                            uint16_t *v = static_cast<uint16_t*>(image_data->GetScalarPointer(c,r,s));
                            *v = (uint16_t) image->Get(t,s,r,c);
                        }
                    }
                }
                break;
            }
            case 4:
            {
                image_data->AllocateScalars(VTK_TYPE_UINT32, 1);
                for(unsigned int s=0; s<image->GetDimension_S(); s++)
                {
                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
                    {
                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
                        {
                            uint32_t *v = static_cast<uint32_t*>(image_data->GetScalarPointer(c,r,s));
                            *v = (uint32_t) image->Get(t,s,r,c);
                        }
                    }
                }
                break;
            }
            case 8:
            {
                image_data->AllocateScalars(VTK_TYPE_UINT64, 1);
                for(unsigned int s=0; s<image->GetDimension_S(); s++)
                {
                    for(unsigned int r=0; r<image->GetDimension_R(); r++)
                    {
                        for(unsigned int c=0; c<image->GetDimension_C(); c++)
                        {
                            uint64_t *v = static_cast<uint64_t*>(image_data->GetScalarPointer(c,r,s));
                            *v = (uint64_t) image->Get(t,s,r,c);
                        }
                    }
                }
                break;
            }
        }
        
        double Cxyz[3], Rxyz[3], Sxyz[3];
        image->GetOrientation_C_Vector(Cxyz[0],Cxyz[1],Cxyz[2]);
        image->GetOrientation_R_Vector(Rxyz[0],Rxyz[1],Rxyz[2]);
        image->GetOrientation_S_Vector(Sxyz[0],Sxyz[1],Sxyz[2]);
    
        this->m_data[t].CreatePolyData(image_data, threshold, Cxyz[0],Cxyz[1],Cxyz[2], Rxyz[0],Rxyz[1],Rxyz[2], Sxyz[0],Sxyz[1],Sxyz[2]);
    }
}


int vmxMesh::GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max)
{
    if(this->IsEmpty()) return 0;
    
    // We will use bounds of the actor (instead of poly data) to determine the visible bounds (that is what the bounds are for)
    double bounds[6];
    this->m_data[0].m_actor->GetBounds(bounds);
    
    x_min = bounds[0]; x_max = bounds[1];
    y_min = bounds[2]; y_max = bounds[3];
    z_min = bounds[4]; z_max = bounds[5];
    
    for(unsigned int t=1; t<this->GetDimension_T(); t++)
    {
        this->m_data[t].m_actor->GetBounds(bounds);
        if(bounds[0]<x_min) x_min = bounds[0]; if(bounds[1]>x_max) x_max = bounds[1];
        if(bounds[2]<y_min) y_min = bounds[2]; if(bounds[3]>y_max) y_max = bounds[3];
        if(bounds[4]<z_min) z_min = bounds[4]; if(bounds[5]>z_max) z_max = bounds[5];
    }
    
    return 1;
}


void vmxMesh::GetColor(double &red, double &green, double &blue)
{
    if(!this->m_data.IsEmpty())
    {
        this->m_data[0].GetColor(red,green,blue);
    }
}


unsigned int vmxMesh::GetDimension_T()
{
    return (this->m_data.GetNumberOfElements()) ;
}


double vmxMesh::GetOpacity()
{
    if(!this->m_data.IsEmpty())
    {
        return (this->m_data[0].GetOpacity());
    }
    return 0;
}


vtkRenderer* vmxMesh::GetRenderer()
{
    return (this->m_renderer);
}


int vmxMesh::HasActor(vtkActor *actor)
{
    if(!actor) return 0;
    for(unsigned int t=0; t<this->GetDimension_T(); t++)
    {
        if(this->m_data[t].m_actor==actor) { return 1; }
    }
    return 0;
}


int vmxMesh::IsEmpty()
{
    if(this->m_data.GetNumberOfElements()==0) { return 1; }
    else { return 0; }
}


int vmxMesh::IsVisible()
{
    return (this->m_is_visible);
}


int vmxMesh::LoadSTLFile(const char *file_name)
{
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(file_name);
    reader->Update();
    
    this->m_data[0].m_poly_data->DeepCopy(reader->GetOutput());
    this->m_data[0].m_poly_data_mapper->SetInputData(this->m_data[0].m_poly_data);
    this->m_data[0].m_actor->SetMapper(this->m_data[0].m_poly_data_mapper);
    
    return 1;
}


int vmxMesh::LoadVTPFile(const char *file_name)
{
    vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(file_name);
    reader->Update();
    
    this->m_data[0].m_poly_data->DeepCopy(reader->GetOutput());
    this->m_data[0].m_poly_data_mapper->SetInputData(this->m_data[0].m_poly_data);
    this->m_data[0].m_actor->SetMapper(this->m_data[0].m_poly_data_mapper);
    
    return 1;
}


vmxMesh3D& vmxMesh::operator()(unsigned int t)
{
    return (this->m_data[t]);
}


void vmxMesh::Reset()
{
	if(this->m_renderer)
	{
		for(unsigned int t=0; t<this->GetDimension_T(); t++)
		{
			this->m_renderer->RemoveActor(this->m_data[t].m_actor);
		}
	}

    // Do NOT reset the assigned renderer!
    
	this->m_data.Reset();
    this->m_data.SetNumberOfElements(1);
    this->m_index_T = 0;
    this->m_is_visible = 0;
}


void vmxMesh::SaveToSTLFiles(const char *file_name)
{
    if(this->IsEmpty()) return;
    
    mxString file_string, extension, temp;
    temp.Assign(file_name);
    temp.ExtractFileNameAndFileExtension(file_string, extension);
    
    file_string.Append("_t");
    
    for(unsigned int t=0; t<this->GetDimension_T(); t++)
    {
        mxString file_name_string;
        file_name_string.Assign(file_string);
        file_name_string.AppendNumber((int)t);
        file_name_string.Append(".stl");
        
        vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
        normals->SetInputData(this->m_data[t].m_poly_data);
        normals->ConsistencyOn();
        normals->SplittingOff();
        normals->Update();
        
        vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
        writer->SetFileName(file_name);
        writer->SetInputConnection(normals->GetOutputPort());
        writer->Write();
    }
}


void vmxMesh::SaveToVTKFiles(const char *file_name)
{
    if(this->IsEmpty()) return;
    
    mxString file_string, extension, temp;
    temp.Assign(file_name);
    temp.ExtractFileNameAndFileExtension(file_string, extension);
    
    file_string.Append("_t");
    
    for(unsigned int t=0; t<this->GetDimension_T(); t++)
    {
        mxString file_name_string;
        file_name_string.Assign(file_string);
        file_name_string.AppendNumber((int)t);
        file_name_string.Append(".vtk");
        
        vtkSmartPointer<vtkGenericDataObjectWriter> writer = vtkSmartPointer<vtkGenericDataObjectWriter>::New();
        writer->SetFileName(file_name_string.Get_C_String());
        writer->SetInputData(this->m_data[t].m_poly_data);
        writer->Write();
    }
}


void vmxMesh::SetColor(double red, double green, double blue)
{
   for(unsigned int t=0; t<this->GetDimension_T(); t++)
   {
       this->m_data[t].SetColor(red,green,blue);
   }
}


void vmxMesh::SetIndex_T(unsigned int t)
{
    this->SetIndex_T_WithoutRendering(t);
    this->m_renderer->Render();
}


void vmxMesh::SetIndex_T_WithoutRendering(unsigned int t)
{
    if(t>=this->GetDimension_T()) return;
    
    // The current approach has only a single actor added to the renderer (for the current T index) and as the index changes
    // the previous actor is removed and the new index actor is added to the renderer. It would be interesting to check if
    // a better performance could be achieved if alla ctors were added to the renderer and their visibility changed with index change.
    
    if(!this->m_renderer) return;
    
    this->m_renderer->RemoveActor(this->m_data[this->m_index_T].m_actor);
    this->m_index_T = t;
    this->m_data[this->m_index_T].m_actor->SetVisibility(this->m_is_visible);
    this->m_renderer->AddActor(this->m_data[this->m_index_T].m_actor);
}


void vmxMesh::SetOpacity(double opacity)
{
    for(unsigned int t=0; t<this->GetDimension_T(); t++)
    {
        this->m_data[t].SetOpacity(opacity);
    }
}


void vmxMesh::SetRenderer(vtkRenderer *renderer)
{
    if(renderer)
    {
        this->m_renderer = renderer;
        this->SetIndex_T_WithoutRendering(0);
    }
}


void vmxMesh::SetVisibility(int is_visible)
{
    if(this->m_is_visible != is_visible)
    {
        this->m_is_visible = is_visible;
        this->SetIndex_T_WithoutRendering(this->m_index_T);
        this->m_data[m_index_T].SetVisibility(is_visible);
    }
}


void vmxMesh::SetVisibilityOfComponent(int component_index, int is_visible)
{
    // since current mesh version has only 1 component, we just call SetVisibility().
    this->vmxMesh::SetVisibility(is_visible);
}
















