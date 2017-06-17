/*=========================================================================
 
 Program:   mipx
 Module:    mxIndex.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxIndex.h"



mxIndex::mxIndex()
{
	this->m_indexes[0] = this->m_indexes[1] = this->m_indexes[2] = this->m_indexes[2] = 0;
}


mxIndex::~mxIndex()
{
    this->Reset();
}


void mxIndex::CopyFrom(mxIndex &index)
{
    if(&index==this) return;
    this->m_indexes[0] = index.m_indexes[0];
    this->m_indexes[1] = index.m_indexes[1];
    this->m_indexes[2] = index.m_indexes[2];
    this->m_indexes[3] = index.m_indexes[3];
}


unsigned int mxIndex::DistanceCityBlock(mxIndex &index)
{
    int ds = this->m_indexes[2] - index.m_indexes[2];
    int dr = this->m_indexes[1] - index.m_indexes[1];
    int dc = this->m_indexes[0] - index.m_indexes[0];
    if(ds<0) ds = -ds;
    if(dr<0) dr = -dr;
    if(dc<0) dc = -dc;
    return(ds+dr+dc);
}


double mxIndex::DistanceEuclidean(mxIndex &index)
{
    return (sqrt((double)( (this->m_indexes[2]-index.m_indexes[2])*(this->m_indexes[2]-index.m_indexes[2]) +
                          (this->m_indexes[1]-index.m_indexes[1])*(this->m_indexes[1]-index.m_indexes[1]) +
                          (this->m_indexes[0]-index.m_indexes[0])*(this->m_indexes[0]-index.m_indexes[0]) )));
}


unsigned int mxIndex::DistanceMaximumNorm(mxIndex &index)
{
    int ds = this->m_indexes[2] - index.m_indexes[2];
    int dr = this->m_indexes[1] - index.m_indexes[1];
    int dc = this->m_indexes[0] - index.m_indexes[0];
    if(ds<0) ds = -ds;
    if(dr<0) dr = -dr;
    if(dc<0) dc = -dc;
    if(ds>=dr && ds>=dc) { return ds; }
    else
    {
        if(dr>=ds && dr>=dc) { return dr; }
        else { return dc; }
    }
}


unsigned int mxIndex::DistanceSquaredEuclidean(mxIndex &index)
{
    return ( (this->m_indexes[2]-index.m_indexes[2])*(this->m_indexes[2]-index.m_indexes[2]) +
            (this->m_indexes[1]-index.m_indexes[1])*(this->m_indexes[1]-index.m_indexes[1]) +
            (this->m_indexes[0]-index.m_indexes[0])*(this->m_indexes[0]-index.m_indexes[0]) );
}


int* mxIndex::GetIndexes()
{
    return this->m_indexes;
}


int mxIndex::IsEqualIndexIn_2D(mxIndex &index)
{
    if(this->m_indexes[0] != index.m_indexes[0]) return 0;
    if(this->m_indexes[1] != index.m_indexes[1]) return 0;
    return 1;
}


int mxIndex::IsEqualIndexIn_3D(mxIndex &index)
{
    if(this->m_indexes[0] != index.m_indexes[0]) return 0;
    if(this->m_indexes[1] != index.m_indexes[1]) return 0;
    if(this->m_indexes[2] != index.m_indexes[2]) return 0;
    return 1;
}


int mxIndex::IsEqualIndexIn_4D(mxIndex &index)
{
    if(this->m_indexes[0] != index.m_indexes[0]) return 0;
    if(this->m_indexes[1] != index.m_indexes[1]) return 0;
    if(this->m_indexes[2] != index.m_indexes[2]) return 0;
    if(this->m_indexes[3] != index.m_indexes[3]) return 0;
    return 1;
}


int& mxIndex::C()
{
    return this->m_indexes[0];
}


int& mxIndex::R()
{
    return this->m_indexes[1];
}


int& mxIndex::S()
{
    return this->m_indexes[2];
}


int& mxIndex::T()
{
    return this->m_indexes[3];
}


void mxIndex::Reset()
{
    this->m_indexes[0] = this->m_indexes[1] = this->m_indexes[2] = this->m_indexes[2] = 0;
}


void mxIndex::SetIndex(int t_index, int s_index, int r_index, int c_index)
{
    this->m_indexes[0] = c_index;
    this->m_indexes[1] = r_index;
    this->m_indexes[2] = s_index;
    this->m_indexes[3] = t_index;
}


void mxIndex::SetIndex(int s_index, int r_index, int c_index)
{
    this->m_indexes[0] = c_index;
    this->m_indexes[1] = r_index;
    this->m_indexes[2] = s_index;
}


void mxIndex::SetIndex(int r_index, int c_index)
{
    this->m_indexes[0] = c_index;
    this->m_indexes[1] = r_index;
}


int mxIndex::SetIndexes2D(int *cr)
{
    if(!cr) return 0;
    this->m_indexes[0] = cr[0];
    this->m_indexes[1] = cr[1];
    return 1;
}


int mxIndex::SetIndexes3D(int *crs)
{
    if(!crs) return 0;
    this->m_indexes[0] = crs[0];
    this->m_indexes[1] = crs[1];
    this->m_indexes[2] = crs[2];
    return 1;
}


int mxIndex::SetIndexes4D(int *crst)
{
    if(!crst) return 0;
    this->m_indexes[0] = crst[0];
    this->m_indexes[1] = crst[1];
    this->m_indexes[2] = crst[2];
    this->m_indexes[3] = crst[3];
    return 1;
}


mxIndex& mxIndex::operator =(mxIndex &index)
{
    if(&index==this) return *this;
    this->CopyFrom(index);
    return *this;
}


std::ostream& operator<< (std::ostream &o, mxIndex &index)
{ 
	o<<"["<<index.m_indexes[3]<<","<<index.m_indexes[2]<<","<<index.m_indexes[1]<<","<<index.m_indexes[0]<<"]";
    return o;
}