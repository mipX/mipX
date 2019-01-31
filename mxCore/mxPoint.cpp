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
	m_value = 0;
}


mxPoint::~mxPoint()
{
    this->Reset();
}


void mxPoint::CopyFrom(mxPoint &point)
{
    if(&point==this) return;
    mxPosition::CopyFrom(point);
    this->m_value = point.m_value;
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


void mxPoint::SetValue(double value)
{
    this->m_value = value;
}


double& mxPoint::V()
{
	return this->m_value;
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
	o<<"["<<point.S()<<","<<point.R()<<","<<point.C()<<"]{"<<point.V()<<"}";
    return o;
}
