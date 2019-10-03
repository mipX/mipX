/*=========================================================================
 
 Program:   mipx
 Module:    mxPoint.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxPoint.h"



mxPoint::mxPoint()
{
	m_value[0] = 0;
}


mxPoint::~mxPoint()
{
    this->Reset();
}


void mxPoint::CopyFrom(mxPoint &point)
{
    if(&point==this) return;
    mxPosition::CopyFrom(point);
    for(unsigned int i=0; i<mxPoint_NUMBER_OF_VALUES; i++)
    {
        this->m_value[i] = point.m_value[i];
    }
}


int mxPoint::IsEqualValueAndPositionIn_2D(mxPoint &point)
{
    if(this->m_value != point.m_value) return 0;
    return this->IsEqualPositionIn_2D(point);
}


int mxPoint::IsEqualValueAndPositionIn_3D(mxPoint &point)
{
    if(this->m_value != point.m_value) return 0;
    return this->IsEqualPositionIn_2D(point);
}


int mxPoint::IsEqualValueAndPositionIn_4D(mxPoint &point)
{
    if(this->m_value != point.m_value) return 0;
    return this->IsEqualPositionIn_4D(point);
}


void mxPoint::Print(std::ostream &s, mxArray<int> &indexes_of_values_for_printing)
{
    s<<"("<<this->T()<<","<<this->S()<<","<<this->R()<<","<<this->C()<<"){";
    for(unsigned int i=0; i<indexes_of_values_for_printing.GetNumberOfElements() && i<this->GetNumberOfValues(); i++)
    {
        s<<this->V(indexes_of_values_for_printing[i]);
        if(i<indexes_of_values_for_printing.GetNumberOfElements()-1 && i<this->GetNumberOfValues()-1) s<<",";
    }
    s<<"}";
}


void mxPoint::Print(std::stringstream &s, mxArray<int> &indexes_of_values_for_printing)
{
    s<<"("<<this->T()<<","<<this->S()<<","<<this->R()<<","<<this->C()<<"){";
    for(unsigned int i=0; i<indexes_of_values_for_printing.GetNumberOfElements() && i<this->GetNumberOfValues(); i++)
    {
        s<<this->V(indexes_of_values_for_printing[i]);
        if(i<indexes_of_values_for_printing.GetNumberOfElements()-1 && i<this->GetNumberOfValues()-1) s<<",";
    }
    s<<"}";
}



int mxPoint::ReadFromString(const char *position_string)
{
    mxString s, s_coord, s_values, s_time;
    s.Assign(position_string);
    
    // Read coordinates
    if(!s.ExtractString('(', ')', s_coord)) return 0;
    
    mxList< double > list_of_doubles;
    s_coord.ExtractNumbers(list_of_doubles);
    
    if(list_of_doubles.GetNumberOfElements()<3 || list_of_doubles.GetNumberOfElements()>4)
    {
        std::cout<<std::endl<<"mxPoint::ReadFromString() Error: cannot read coordinates!"<<std::endl;
        return 0;
    }
    
    if(list_of_doubles.GetNumberOfElements()==3)
    {
        this->S() = list_of_doubles[0];
        this->R() = list_of_doubles[1];
        this->C() = list_of_doubles[2];
    }
    if(list_of_doubles.GetNumberOfElements()==4)
    {
        this->T() = list_of_doubles[0];
        this->S() = list_of_doubles[1];
        this->R() = list_of_doubles[2];
        this->C() = list_of_doubles[3];
    }

    // Read values
    if(!s.ExtractString('{', '}', s_values)) return 0;
    
    s_values.ExtractNumbers(list_of_doubles);
    
    mxListIterator< double > it;
    int i=0;
    for(it.SetToBegin(list_of_doubles), i=0; it.IsValid() && i<mxPoint_NUMBER_OF_VALUES; it.MoveToNext(), i++)
    {
        this->V(i) = it.GetElement();
    }
        
    return 1;

}


void mxPoint::SetValue(double value, unsigned int index)
{
    this->m_value[index] = value;
}


double& mxPoint::V(unsigned int index)
{
	return this->m_value[index];
}


mxPoint& mxPoint::operator =(mxPoint &point)
{
    if(&point==this) return *this;
    this->CopyFrom(point);
    return *this;
}


mxPoint& mxPoint::operator =(mxPosition &position)
{
    if(&position==this) return *this;
    mxPosition::CopyFrom(position);
    return *this;
}


std::ostream& operator<<(std::ostream &o, mxPoint &point)
{ 
    o<<"("<<point.T()<<","<<point.S()<<","<<point.R()<<","<<point.C()<<"){";
    for(int i=0; i<mxPoint_NUMBER_OF_VALUES; i++)
    {
        o<<point.V(i);
        if(i!=mxPoint_NUMBER_OF_VALUES-1)
        {
            o<<",";
        }
    }
    o<<"}";
    return o;
}
