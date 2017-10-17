/*=========================================================================
 
 Program:   mipx
 Module:    mxDataContainer.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxDataContainer.h"


mxDataContainerStep::mxDataContainerStep()
{
}


mxDataContainerStep::~mxDataContainerStep()
{
	this->Reset();
}


void mxDataContainerStep::Reset()
{
    m_data_objects.Reset();
    m_doubles.Reset();
	m_integers.Reset();
	m_strings.Reset();
    m_check_box_chosen_options.Reset();
    m_check_box_titles_of_options.Reset();
    m_radio_button_chosen_options.Reset();
    m_radio_button_titles_of_options.Reset();
}



//------------------------------------------------------------------------------



mxDataContainer::mxDataContainer()
{
}


mxDataContainer::~mxDataContainer()
{
    this->Reset();
}


void mxDataContainer::Reset()
{
	m_input_steps.Reset();
//    m_tool_objects.Reset();
}


mxDataContainerStep* mxDataContainer::AddStep(unsigned int index_of_step)
{
    if(index_of_step >= m_input_steps.GetNumberOfElements())
    {
        for(unsigned int i=0; i<(index_of_step - m_input_steps.GetNumberOfElements() + 1) ; i++)
        {
            m_input_steps.AddNewToEnd();
        }
    }
    
    return &(m_input_steps[index_of_step]);
}


void mxDataContainer::AddOutputDataObject(mxDataObject *data_object)
{
    m_output_data.m_data_objects.AddToEnd(data_object);
}


void mxDataContainer::AppendOutputText(const char *text)
{
    m_output_text.Append(text);
}


void mxDataContainer::AssignOutputText(const char *text)
{
	m_output_text.Assign(text);
}


mxArray< mxDataObject* > mxDataContainer::GetDataObjects()
{
    mxArray< mxDataObject* > array_of_data_objects;
    
    // Find out how many data objects there ae in total.
    unsigned int number_of_data_objects = 0;
    mxListIterator< mxDataContainerStep > itcs;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        number_of_data_objects += itcs.GetElementAddress()->m_data_objects.GetNumberOfElements();
    }
    
    // Set the appropriate data object array size and fill it in.
    array_of_data_objects.SetNumberOfElements(number_of_data_objects);
    unsigned int i=0;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        mxListIterator< mxDataObject* > itdo;
        for(itdo.SetToBegin(itcs.GetElementAddress()->m_data_objects); itdo.IsValid(); itdo.MoveToNext())
        {
            array_of_data_objects[i] = itdo.GetElement();
            i++;
        }
    }
    
    return array_of_data_objects;
}


mxArray< mxDataObject* > mxDataContainer::GetDataObjectsOfType(const char *data_object_type_name)
{
    mxString type_name;
    type_name.Assign(data_object_type_name);
    
    // Put all objects of given type into a temp list.
    mxList< mxDataObject* > list_of_data_objects;
    mxListIterator< mxDataContainerStep > itcs;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        mxListIterator< mxDataObject* > itdo;
        for(itdo.SetToBegin(itcs.GetElementAddress()->m_data_objects); itdo.IsValid(); itdo.MoveToNext())
        {
            if(itdo.GetElement()->GetClassName()==type_name)
            {
                list_of_data_objects.AddToEnd(itdo.GetElement());
            }
        }
    }

    // Set the appropriate data object array size and fill it in.
    mxArray< mxDataObject* > array_of_data_objects;
    array_of_data_objects.SetNumberOfElements(list_of_data_objects.GetNumberOfElements());
    unsigned int i=0;
    mxListIterator< mxDataObject* > it;
    for(i=0, it.SetToBegin(list_of_data_objects); i<array_of_data_objects.GetNumberOfElements() && it.IsValid(); i++, it.MoveToNext())
    {
        array_of_data_objects[i] = it.GetElement();
    }
    
    return array_of_data_objects;
}


mxArray< mxDataObject* > mxDataContainer::GetDataObjectsOfTypeForStep(const char *data_object_type_name, unsigned int index_of_step)
{
    mxString type_name;
    type_name.Assign(data_object_type_name);
    
    // Put all objects of given type into a temp list.
    mxList< mxDataObject* > list_of_data_objects;
    mxListIterator< mxDataObject* > itdo;
    for(itdo.SetToBegin(m_input_steps[index_of_step].m_data_objects); itdo.IsValid(); itdo.MoveToNext())
    {
        if(itdo.GetElement()->GetClassName()==type_name)
        {
            list_of_data_objects.AddToEnd(itdo.GetElement());
        }
    }

    // Set the appropriate data object array size and fill it in.
    mxArray< mxDataObject* > array_of_data_objects;
    array_of_data_objects.SetNumberOfElements(list_of_data_objects.GetNumberOfElements());
    unsigned int i=0;
    mxListIterator< mxDataObject* > it;
    for(i=0, it.SetToBegin(list_of_data_objects); i<array_of_data_objects.GetNumberOfElements() && it.IsValid(); i++, it.MoveToNext())
    {
        array_of_data_objects[i] = it.GetElement();
    }
    
    return array_of_data_objects;
}


mxArray< double > mxDataContainer::GetDoubleNumbers()
{
    // Put all numbers into a temp list.
    mxList< double > list_of_numbers;
    mxListIterator< mxDataContainerStep > itcs;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        mxListIterator< double > itd;
        for(itd.SetToBegin(itcs.GetElementAddress()->m_doubles); itd.IsValid(); itd.MoveToNext())
        {
            list_of_numbers.AddToEnd(itd.GetElement());
        }
    }
    
    // Set the appropriate array size and fill it in.
    mxArray< double > array_of_numbers;
    array_of_numbers.SetNumberOfElements(list_of_numbers.GetNumberOfElements());
    unsigned int i=0;
    mxListIterator< double > it;
    for(i=0, it.SetToBegin(list_of_numbers); i<array_of_numbers.GetNumberOfElements() && it.IsValid(); i++, it.MoveToNext())
    {
        array_of_numbers[i] = it.GetElement();
    }
    
    return array_of_numbers;
}


mxArray< int > mxDataContainer::GetIntegerNumbers()
{
    // Put all numbers into a temp list.
    mxList< int > list_of_numbers;
    mxListIterator< mxDataContainerStep > itcs;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        mxListIterator< int > iti;
        for(iti.SetToBegin(itcs.GetElementAddress()->m_integers); iti.IsValid(); iti.MoveToNext())
        {
            list_of_numbers.AddToEnd(iti.GetElement());
        }
    }
    
    // Set the appropriate array size and fill it in.
    mxArray< int > array_of_numbers;
    array_of_numbers.SetNumberOfElements(list_of_numbers.GetNumberOfElements());
    unsigned int i=0;
    mxListIterator< int > it;
    for(i=0, it.SetToBegin(list_of_numbers); i<array_of_numbers.GetNumberOfElements() && it.IsValid(); i++, it.MoveToNext())
    {
        array_of_numbers[i] = it.GetElement();
    }
    
    return array_of_numbers;
}


mxString mxDataContainer::GetOutputText()
{
	return m_output_text;
}


mxString mxDataContainer::GetSelectedRadioButtonOption(const char *radio_button_options_title)
{
    mxString options_title;
    options_title.Assign(radio_button_options_title);
    
    mxListIterator< mxDataContainerStep > itcs;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        mxListIterator< mxString > it, it2;
        for(it.SetToBegin(itcs.GetElementAddress()->m_radio_button_titles_of_options), it2.SetToBegin(itcs.GetElementAddress()->m_radio_button_chosen_options); it.IsValid() && it2.IsValid(); it.MoveToNext(), it2.MoveToNext())
        {
            if(it.GetElement()==options_title)
            {
                return it2.GetElement();
            }
        }
    }
    
    // If none found, return empty string
    mxString empty_string;
    return empty_string;
}


mxArray< mxString > mxDataContainer::GetSelectedCheckBoxOptions(const char *check_box_options_title)
{
    mxString options_title;
    options_title.Assign(check_box_options_title);
    
    mxListIterator< mxDataContainerStep > itcs;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        mxListIterator< mxString > it;
        mxListIterator< mxList< mxString > > it2;
        for(it.SetToBegin(itcs.GetElementAddress()->m_check_box_titles_of_options), it2.SetToBegin(itcs.GetElementAddress()->m_check_box_chosen_options); it.IsValid() && it2.IsValid(); it.MoveToNext(), it2.MoveToNext())
        {
            if(it.GetElement()==options_title)
            {
                mxArray< mxString > array_of_chosen_options;
                array_of_chosen_options.SetNumberOfElements(it2.GetElement().GetNumberOfElements());
                
                mxListIterator< mxString > its;
                unsigned int i=0;
                
                for(i=0, its.SetToBegin(it2.GetElementAddress()); its.IsValid() && i<array_of_chosen_options.GetNumberOfElements(); i++, its.MoveToNext())
                {
                    array_of_chosen_options[i].Assign(its.GetElement());
                }
                
                return array_of_chosen_options;
            }
        }
    }

    // If none found, return empty array
    mxArray< mxString > empty_array;
    return empty_array;
}


mxDataContainerStep* mxDataContainer::GetStep(unsigned int index_of_step)
{
	if(index_of_step >= m_input_steps.GetNumberOfElements())
    {
        return NULL;
    }
    
    return &(m_input_steps[index_of_step]);
}


mxArray< mxString > mxDataContainer::GetStrings()
{
    // Put all strings into a temp list.
    mxList< mxString > list_of_strings;
    mxListIterator< mxDataContainerStep > itcs;
    for(itcs.SetToBegin(m_input_steps); itcs.IsValid(); itcs.MoveToNext())
    {
        mxListIterator< mxString > it;
        for(it.SetToBegin(itcs.GetElementAddress()->m_strings); it.IsValid(); it.MoveToNext())
        {
            list_of_strings.AddToEnd(it.GetElement());
        }
    }
    
    // Set the appropriate array size and fill it in.
    mxArray< mxString > array_of_strings;
    array_of_strings.SetNumberOfElements(list_of_strings.GetNumberOfElements());
    unsigned int i=0;
    mxListIterator< mxString > it;
    for(i=0, it.SetToBegin(list_of_strings); i<array_of_strings.GetNumberOfElements() && it.IsValid(); i++, it.MoveToNext())
    {
        array_of_strings[i] = it.GetElement();
    }
    
    return array_of_strings;
}


//vtkRenderWindowInteractor* mxDataContainer::GetVTKRenderWindowInteractor()
//{
//	return m_interactor;
//}








