/*=========================================================================
 
 Program:   mipx
 Module:    mxPosition.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxPosition.h"



mxPosition::mxPosition()
{
	this->m_positions[0] = this->m_positions[1] = this->m_positions[2] = this->m_positions[3] = 0;
}


mxPosition::~mxPosition()
{
    this->Reset();
}


void mxPosition::Addition(double s, double r, double c)
{
    this->S() += s;
    this->R() += r;
    this->C() += c;
}


void mxPosition::Subtraction(double s, double r, double c)
{
    this->S() -= s;
    this->R() -= r;
    this->C() -= c;
}


void mxPosition::Division(double n)
{
    this->S() = this->S() / n;
    this->R() = this->R() / n;
    this->C() = this->C() / n;
}


void mxPosition::Multiplication(double n)
{
    this->S() = this->S() * n;
    this->R() = this->R() * n;
    this->C() = this->C() * n;
}



void mxPosition::CopyFrom(mxPosition &position)
{
    if(&position==this) return;
    this->m_positions[0] = position.m_positions[0];
    this->m_positions[1] = position.m_positions[1];
    this->m_positions[2] = position.m_positions[2];
    this->m_positions[3] = position.m_positions[3];
}


unsigned int mxPosition::DistanceCityBlock(mxPosition &position)
{
    int ds = (int)(this->m_positions[2] - position.m_positions[2]);
    int dr = (int)(this->m_positions[1] - position.m_positions[1]);
    int dc = (int)(this->m_positions[0] - position.m_positions[0]);
    if(ds<0) ds = -ds;
    if(dr<0) dr = -dr;
    if(dc<0) dc = -dc;
    return(ds+dr+dc);
}


double mxPosition::DistanceEuclidean(mxPosition &position)
{
    return (sqrt((double)( (this->m_positions[2]-position.m_positions[2])*(this->m_positions[2]-position.m_positions[2]) +
                          (this->m_positions[1]-position.m_positions[1])*(this->m_positions[1]-position.m_positions[1]) +
                          (this->m_positions[0]-position.m_positions[0])*(this->m_positions[0]-position.m_positions[0]) )));
}


unsigned int mxPosition::DistanceMaximumNorm(mxPosition &position)
{
    int ds = (int)(this->m_positions[2] - position.m_positions[2]);
    int dr = (int)(this->m_positions[1] - position.m_positions[1]);
    int dc = (int)(this->m_positions[0] - position.m_positions[0]);
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


unsigned int mxPosition::DistanceSquaredEuclidean(mxPosition &position)
{
    return (unsigned int)( (this->m_positions[2]-position.m_positions[2])*(this->m_positions[2]-position.m_positions[2]) +
            (this->m_positions[1]-position.m_positions[1])*(this->m_positions[1]-position.m_positions[1]) +
            (this->m_positions[0]-position.m_positions[0])*(this->m_positions[0]-position.m_positions[0]) );
}


double* mxPosition::GetPositions()
{
    return this->m_positions;
}


int mxPosition::IsEqualPositionIn_2D(mxPosition &position)
{
    if(this->m_positions[0] != position.m_positions[0]) return 0;
    if(this->m_positions[1] != position.m_positions[1]) return 0;
    return 1;
}


int mxPosition::IsEqualPositionIn_3D(mxPosition &position)
{
    if(this->m_positions[0] != position.m_positions[0]) return 0;
    if(this->m_positions[1] != position.m_positions[1]) return 0;
    if(this->m_positions[2] != position.m_positions[2]) return 0;
    return 1;
}


int mxPosition::IsEqualPositionIn_4D(mxPosition &position)
{
    if(this->m_positions[0] != position.m_positions[0]) return 0;
    if(this->m_positions[1] != position.m_positions[1]) return 0;
    if(this->m_positions[2] != position.m_positions[2]) return 0;
    if(this->m_positions[3] != position.m_positions[3]) return 0;
    return 1;
}


double& mxPosition::C()
{
    return this->m_positions[0];
}


double& mxPosition::R()
{
    return this->m_positions[1];
}


double& mxPosition::S()
{
    return this->m_positions[2];
}


double& mxPosition::T()
{
    return this->m_positions[3];
}


void mxPosition::Reset()
{
    this->m_positions[0] = this->m_positions[1] = this->m_positions[2] = this->m_positions[3] = 0;
}


int mxPosition::SetDistanceFromPosition(mxPosition &input_pos, double new_distance, int direction)
{
    if(new_distance<0) return 0;
    
    double ds = this->S() - input_pos.S();
    double dr = this->R() - input_pos.R();
    double dc = this->C() - input_pos.C();
    double old_distance = this->DistanceEuclidean(input_pos);
    double ratio = new_distance / old_distance;
    
    if(direction==0)
    {
        this->S() = input_pos.S() + ratio*ds;
        this->R() = input_pos.R() + ratio*dr;
        this->C() = input_pos.C() + ratio*dc;
    }
    else
    {
        this->S() = input_pos.S() - ratio*ds;
        this->R() = input_pos.R() - ratio*dr;
        this->C() = input_pos.C() - ratio*dc;
    }
    return 1;
}



void mxPosition::SetPosition(double t_position, double s_position, double r_position, double c_position)
{
    this->m_positions[0] = c_position;
    this->m_positions[1] = r_position;
    this->m_positions[2] = s_position;
    this->m_positions[3] = t_position;
}


void mxPosition::SetPosition(double s_position, double r_position, double c_position)
{
    this->m_positions[0] = c_position;
    this->m_positions[1] = r_position;
    this->m_positions[2] = s_position;
}


void mxPosition::SetPosition(double r_position, double c_position)
{
    this->m_positions[0] = c_position;
    this->m_positions[1] = r_position;
}


int mxPosition::SetPositions2D(double *cr)
{
    if(!cr) return 0;
    this->m_positions[0] = cr[0];
    this->m_positions[1] = cr[1];
    return 1;
}


int mxPosition::SetPositions3D(double *crs)
{
    if(!crs) return 0;
    this->m_positions[0] = crs[0];
    this->m_positions[1] = crs[1];
    this->m_positions[2] = crs[2];
    return 1;
}


int mxPosition::SetPositions4D(double *crst)
{
    if(!crst) return 0;
    this->m_positions[0] = crst[0];
    this->m_positions[1] = crst[1];
    this->m_positions[2] = crst[2];
    this->m_positions[3] = crst[3];
    return 1;
}


mxPosition& mxPosition::operator =(mxPosition &position)
{
    if(&position==this) return *this;
    this->CopyFrom(position);
    return *this;
}


std::ostream& operator<< (std::ostream &o, mxPosition &position)
{ 
	o<<"["<<position.m_positions[3]<<","<<position.m_positions[2]<<","<<position.m_positions[1]<<","<<position.m_positions[0]<<"]";
    return o;
}
