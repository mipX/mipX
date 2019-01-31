/*=========================================================================
 
 Program:   mipx
 Module:    mxObject.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxObject.h"



mxObject::mxObject()
{
    this->m_class_name.Assign("mxObject");
    this->m_class_descriptive_name.Assign(" ");
    this->m_object_name.Assign(" ");
    this->m_object_descriptive_name.Assign(" ");
}


mxObject::~mxObject()
{
	this->Reset();
}


int mxObject::Copy(mxObject *object)
{
    if(!object) return 0;
    this->m_object_descriptive_name.Assign(object->m_object_descriptive_name);
    
    // Do not copy object name. Depending on higher level app, it could be a unique name.
    // Also, do not copy class names, as they define the type of the object.
    
    return 1;
}


mxString& mxObject::GetClassName()
{
    return m_class_name;
}


mxString& mxObject::GetClassDescriptiveName()
{
    return m_class_descriptive_name;
}


mxString& mxObject::GetObjectName()
{
    return this->m_object_name;
}


mxString& mxObject::GetObjectDescriptiveName()
{
    return this->m_object_descriptive_name;
}


void mxObject::Reset()
{
}
