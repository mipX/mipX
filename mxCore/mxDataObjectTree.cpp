/*=========================================================================
 
 Program:   mipx
 Module:    mxDataObjectTree.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxDataObjectTree.h"



mxDataObjectTree::mxDataObjectTree()
{
}


mxDataObjectTree::~mxDataObjectTree()
{
    this->ReleaseAllDataObjects();
    this->m_data_object_factories.Reset();
}


int mxDataObjectTree::AddExistingObject(mxDataObject* data_object)
{
    if(!data_object) return 0;
    
    mxListIterator<mxDataObjectFactory*> itf;
    for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid(); itf.MoveToNext())
    {
        if(itf.GetElement()->AddExistingObject(data_object))
        {
            mxString name = this->CheckObjectName(data_object->GetObjectName());
            data_object->GetObjectName().Assign(name);
            return 1;
        }
    }

    return 0;
}


int mxDataObjectTree::AddFactory(mxDataObjectFactory *factory)
{
    //std::cout<<"factoru_address="<<((void*)factory)<<std::endl;
    if(!factory) return 0;
    if(!mxListFunctions::HasElement(this->m_data_object_factories, factory))
    {
        //std::cout<<" adding_new_factory" <<std::endl;
        this->m_data_object_factories.AddToEnd(factory);
        factory->SetDataObjectTree(this);
    }
    
    //std::cout<<"no of factories="<<m_data_object_factories.GetNumberOfElements()<<std::endl;

    return 1;
}


mxString mxDataObjectTree::CheckObjectName(mxString desired_name)
{
    mxString desired_name_modified;
    desired_name_modified.Assign(desired_name);
    
    for(int i=1; i<32768; i++)
    {
        int is_equivalent_name_found = 0;
        
        mxListIterator<mxDataObjectFactory*> itf;
        for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid() && !is_equivalent_name_found; itf.MoveToNext())
        {
            mxListIterator<mxDataObject*> ito;
            for(ito.SetToBegin(itf.GetElement()->GetCreatedDataObjects()); ito.IsValid(); ito.MoveToNext())
            {
                if(ito.GetElement()->GetObjectName()==desired_name_modified)
                {
                    is_equivalent_name_found = 1;
                    break;
                }
            }
        }
        if(!is_equivalent_name_found)
        {
            return desired_name_modified;
        }
        else
        {
            desired_name_modified.Assign(desired_name);
            desired_name_modified.Append("_");
            desired_name_modified.AppendNumber(i);
        }
    }
    return desired_name;
}


mxDataObject* mxDataObjectTree::Create(const char *class_or_type_name, const char *data_object_name)
{
    //std::cout<<"mxDataObjectTree::Create: no of factories="<<m_data_object_factories.GetNumberOfElements()<<std::endl;
    mxListIterator<mxDataObjectFactory*> itf;
    for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid(); itf.MoveToNext())
    {
        mxString class_name = itf.GetElement()->GetClassName();
        if(class_name == class_or_type_name)
        {
            mxDataObject *obj = itf.GetElement()->Create();
            mxString approved_name;
            if(!data_object_name)
            {
                approved_name.Assign(this->CheckObjectName(obj->GetClassName()).Get_C_String());
            }
            else
            {
                mxString desired_name;
                desired_name.Assign(data_object_name);
                approved_name.Assign(this->CheckObjectName(desired_name).Get_C_String());
            }
            obj->GetObjectName().Assign(approved_name);
            return obj;
        }
    }
    std::cout<<"mxDataObjectTree::Create(): could not find factory for class/type name class_or_type_name='"<<class_or_type_name<<"'"<<std::endl;
    return NULL;
}


mxDataObject* mxDataObjectTree::GetDataObject(const char *data_object_name)
{
    mxListIterator<mxDataObjectFactory*> itf;
    for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid(); itf.MoveToNext())
    {
        mxListIterator<mxDataObject*> ito;
        for(ito.SetToBegin(itf.GetElement()->GetCreatedDataObjects()); ito.IsValid(); ito.MoveToNext())
        {
            if(ito.GetElement()->GetObjectName()==data_object_name)
            {
                return ito.GetElement();
            }
        }
    }
    return NULL;
}


mxList<mxDataObject*> mxDataObjectTree::Load(const char *file_names, const char *loading_class_name)
{
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NOT IMPLEMENTED YET !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    mxList<mxDataObject*> l;
    return l;
}


void mxDataObjectTree::Release(mxDataObject* data_object)
{
    mxListIterator<mxDataObjectFactory*> itf;
    for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid(); itf.MoveToNext())
    {
        if(data_object->GetClassName()==itf.GetElement()->GetClassName().Get_C_String())
        {
            itf.GetElement()->Release(data_object);
        }
    }
}


void mxDataObjectTree::ReleaseAllDataObjects()
{
    mxListIterator<mxDataObjectFactory*> itf;
    for(itf.SetToBegin(this->m_data_object_factories); itf.IsValid(); itf.MoveToNext())
    {
        itf.GetElement()->ReleaseAllDataObjects();
    }
}

