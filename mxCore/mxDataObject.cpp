/*=========================================================================
 
 Program:   mipx
 Module:    mxDataObject.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxDataObject.h"



mxDataObject::mxDataObject()
{
    this->m_factory = NULL;
	this->m_is_locked = 0;
    this->m_is_exclusive_visibility_on = 0;
    this->m_color[0] = this->m_color[1] = this->m_color[2] = this->m_color[3] = 0;
    this->m_number_of_visualization_components = 1;
}


mxDataObject::~mxDataObject()
{
    this->mxDataObject::Reset();
}


int mxDataObject::CopyFromDataObject(mxDataObject *data_object)
{
    if(!data_object) return 0;
   
    // Normally we would hve type checking in CopyFromObject method, but here it's not needed
    // as the input is already of the same type.
    
    this->mxObject::Copy(data_object);
    this->m_log = data_object->m_log;
    this->SetColor(data_object->m_color[0], data_object->m_color[1], data_object->m_color[2], data_object->m_color[3]);
    
    // Do not copy factory pointer
    // Do not copy locked indicator
    // Do not copy exclusive visibility indicator (it's the same for all objects).
    
    return 1;

}


int mxDataObject::GetBoundsInWorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::GetBoundsInWorldCoordinates()"<<std::endl;
    z_min = y_min = x_min = 0;
    z_max = y_max = x_max = 100;
    return 0;
}


int mxDataObject::GetColor(unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *a)
{
    //if(!red || !green || !blue || !a) return 0;
    *red = m_color[0];
    *green = m_color[1];
    *blue = m_color[2];
    *a = m_color[3];
    return 1;
}


qmxDataObjectContextMenu* mxDataObject::GetContextMenu()
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::GetContextMenu()"<<std::endl;
    return NULL;
}


mxDataObjectFactory* mxDataObject::GetFactory()
{
    return (this->m_factory);
}


mxString& mxDataObject::GetLog()
{
	return m_log;
}


qmxDataObjectPropertiesWidget* mxDataObject::GetPropertiesWidget()
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::GetPropertiesWidget()"<<std::endl;
    return NULL;
}


int mxDataObject::GetVisibilityOfComponent(int component_index)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::GetVisibilityOfComponent(int)"<<std::endl;
    return 0;
}


int mxDataObject::HasActor(vtkActor *actor)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::HasActor()"<<std::endl;
    return 0;
}


int mxDataObject::IsExclusiveVisibilityOn()
{
    return this->m_is_exclusive_visibility_on;
}


int mxDataObject::IsLocked()
{
    return m_is_locked;
}


int mxDataObject::IsVisibleIn3DView()
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::IsVisibleIn3DView()"<<std::endl;
    return 0;
}


int mxDataObject::LoadFromFile(const char *file_path, const char *extension)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::LoadFromFile()"<<std::endl;
    return 0;
}


int mxDataObject::LoadFromFolder(const char *folder_path, const char *extension)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::LoadFromFolder()"<<std::endl;
    return 0;
}


void mxDataObject::Reset()
{
    this->mxObject::Reset();
    this->m_is_locked = 0;
    this->m_log.Clear();
}


int mxDataObject::SaveToFile(const char *file_path, const char *extension)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::SaveToFile()"<<std::endl;
    return 0;
}


int mxDataObject::SaveToFolder(const char *folder_path, const char *extension)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::SaveToFolder()"<<std::endl;
    return 0;
}


void mxDataObject::SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char a)
{
    m_color[0] = red;
    m_color[1] = green;
    m_color[2] = blue;
    m_color[3] = a;
}


void mxDataObject::SetExclusiveVisibility(int is_visibility_exclusive)
{
    this->m_is_exclusive_visibility_on = is_visibility_exclusive;
}


void mxDataObject::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::SetInteractor()"<<std::endl;
}


void mxDataObject::SetLocked(int is_locked)
{
	this->m_is_locked = is_locked;
}


void mxDataObject::SetRenderer(vtkRenderer *renderer)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::SetRenderer()"<<std::endl;
}


void mxDataObject::SetVisibilityIn3DView(int is_visible)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::SetVisibilityIn3DView()"<<std::endl;
}


void mxDataObject::SetVisibilityOfComponent(int component_index, int is_visible)
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::SetVisibilityOfComponent()"<<std::endl;
}


void mxDataObject::ShowBasicContextMenu()
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::ShowBasicContextMenu()"<<std::endl;
}


void mxDataObject::ShowContextMenu()
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::ShowContextMenu()"<<std::endl;
}


void mxDataObject::ShowProperties()
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::ShowProperties()"<<std::endl;
}


void mxDataObject::Update()
{
    std::cout<<this->GetClassName().Get_C_String()<<" called mxDataObject::Update()"<<std::endl;
}
