/*=========================================================================
 
 Program:   mipx
 Module:    mxVoxel.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxVoxel.h"



mxVoxel::mxVoxel()
{
	m_value = 0;
}


mxVoxel::~mxVoxel()
{
    this->Reset();
}


void mxVoxel::CopyFrom(mxVoxel &voxel)
{
    if(&voxel==this) return;
    mxIndex::CopyFrom(voxel);
    this->m_value = voxel.m_value;
}


int mxVoxel::IsEqualValueAndIndexIn_2D(mxVoxel &voxel)
{
    if(this->m_value != voxel.m_value) return 0;
    return this->IsEqualIndexIn_2D(voxel);
}


int mxVoxel::IsEqualValueAndIndexIn_3D(mxVoxel &voxel)
{
    if(this->m_value != voxel.m_value) return 0;
    return this->IsEqualIndexIn_2D(voxel);
}


int mxVoxel::IsEqualValueAndIndexIn_4D(mxVoxel &voxel)
{
    if(this->m_value != voxel.m_value) return 0;
    return this->IsEqualIndexIn_4D(voxel);
}


void mxVoxel::SetValue(unsigned int value)
{
    this->m_value = value;
}


unsigned int& mxVoxel::V()
{
	return this->m_value;
}


mxVoxel& mxVoxel::operator =(mxVoxel &voxel)
{
    if(&voxel==this) return *this;
    this->CopyFrom(voxel);
    return *this;
}


mxVoxel& mxVoxel::operator =(mxIndex &index)
{
    if(&index==this) return *this;
    mxIndex::CopyFrom(index);
    return *this;
}


std::ostream& operator<<(std::ostream &o, mxVoxel &voxel)
{ 
	o<<"["<<voxel.S()<<","<<voxel.R()<<","<<voxel.C()<<"]{"<<voxel.V()<<"}";
    return o;
}
