/*=========================================================================
 
 Program:   mipx
 Module:    mxGeometry.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxGeometry.h"



// Define static members.
mxArray<mxArray<mxIndex> > mxGeometry::m_indexes_circle;
mxArray<mxArray<mxIndex> > mxGeometry::m_indexes_cube;
mxArray<mxArray<mxIndex> > mxGeometry::m_indexes_sphere;
mxArray<mxArray<mxIndex> > mxGeometry::m_indexes_square;



//-----------------------------------------------------------------------------------------------------------



void mxGeometry::ForIndexes(int s, int r, int c, unsigned int start_array_index_included)
{
    this->m_current_central_indexes.SetIndex(s,r,c);
    this->m_current_array_index = start_array_index_included;
    this->m_current_local_index = 0;
}


mxGeometry::mxGeometry()
{
	this->m_dimensions[0] = this->m_dimensions[1] = this->m_dimensions[2] = 0;

    this->m_current_array_index = this->m_current_local_index = 0;
	this->m_current_central_indexes.SetIndex(0,0,0);
    
    this->m_line_begin_indexes.SetIndex(0,0,0);
    this->m_line_current_local_index = 0;
    this->m_line_delta_c = this->m_line_delta_r = this->m_line_delta_s = 0;
    this->m_line_direction_indexes.SetIndex(0,0,0);
}


mxGeometry::~mxGeometry()
{
}


int mxGeometry::IsIndexValid(int s, int r, int c)
{
	if(s<0 || s>=this->m_dimensions[2]) return 0;
	if(r<0 || r>=this->m_dimensions[1]) return 0;
	if(c<0 || c>=this->m_dimensions[0]) return 0;
	return 1;
}


void mxGeometry::For_4_Neighborhood(int r, int c)
{
	this->ForCircle(r,c,1);
}


void mxGeometry::For_6_Neighborhood(int s, int r, int c)
{
    this->ForSphere(s,r,c,1);
}


void mxGeometry::For_8_Neighborhood(int r, int c)
{
	this->ForSquare(r,c,1);
}


void mxGeometry::For_9_Neighborhood(int r, int c)
{
	this->ForSquare(r,c,0);
}


void mxGeometry::For_26_Neighborhood(int s, int r, int c)
{
	this->ForCube(s,r,c,1);
}


void mxGeometry::For_27_Neighborhood(int s, int r, int c)
{
	this->ForCube(s,r,c,0);
}


void mxGeometry::ForCircle(int r, int c, unsigned int start_squared_radius_included)
{
    if(this->m_indexes_circle.IsEmpty())
    {
        this->PopulateCircleIndexArray();
    }
	this->ForIndexes(0,r,c,start_squared_radius_included);
}


void mxGeometry::ForCube(int s, int r, int c, unsigned int start_size_included)
{
    if(this->m_indexes_cube.IsEmpty())
    {
        this->PopulateCubeIndexArray();
    }
    this->ForIndexes(s,r,c,start_size_included);
}


void mxGeometry::ForLine(int s_start_included, int r_start_included, int c_start_included, int direction_s, int direction_r, int direction_c)
{
    this->m_line_begin_indexes.SetIndex(s_start_included,r_start_included,c_start_included);
    this->m_line_direction_indexes.SetIndex(direction_s,direction_r,direction_c);
    double d = this->m_line_direction_indexes.DistanceMaximumNorm(this->m_line_begin_indexes);
    this->m_line_current_local_index = 0;
    this->m_line_delta_s = ((double)(direction_s - s_start_included)) / d;
    this->m_line_delta_r = ((double)(direction_r - r_start_included)) / d;
    this->m_line_delta_c = ((double)(direction_c - c_start_included)) / d;
}


void mxGeometry::ForSphere(int s, int r, int c, unsigned int start_squared_radius_included)
{
    if(this->m_indexes_sphere.IsEmpty())
    {
        this->PopulateSphereIndexArray();
    }
	this->ForIndexes(s,r,c,start_squared_radius_included);
}


void mxGeometry::ForSquare(int r, int c, unsigned int start_size_included)
{
    if(this->m_indexes_square.IsEmpty())
    {
        this->PopulateSquareIndexArray();
    }
    this->ForIndexes(0,r,c,start_size_included);
}


int mxGeometry::Get_4_Neighborhood(int &r, int &c)
{
    return (this->GetCircle(1,r,c));
}


int mxGeometry::Get_4_Neighborhood(mxIndex &output_indexes)
{
	return (this->Get_4_Neighborhood(output_indexes.R(),output_indexes.C()));
}


int mxGeometry::Get_6_Neighborhood(int &s, int &r, int &c)
{
    return (this->GetSphere(1,s,r,c));
}


int mxGeometry::Get_6_Neighborhood(mxIndex &output_indexes)
{
    return (this->Get_6_Neighborhood(output_indexes.S(),output_indexes.R(),output_indexes.C()));
}


int mxGeometry::Get_8_Neighborhood(int &r, int &c)
{
    return (this->GetSquare(1,r,c));
}


int mxGeometry::Get_8_Neighborhood(mxIndex &output_indexes)
{
    return (this->Get_8_Neighborhood(output_indexes.R(),output_indexes.C()));
}


int mxGeometry::Get_9_Neighborhood(int &r, int &c)
{
	return (this->GetSquare(1,r,c));
}


int mxGeometry::Get_9_Neighborhood(mxIndex &output_indexes)
{
	return (this->Get_9_Neighborhood(output_indexes.R(),output_indexes.C()));
}


int mxGeometry::Get_26_Neighborhood(int &s, int &r, int &c)
{
    return (this->GetCube(1,s,r,c));
}


int mxGeometry::Get_26_Neighborhood(mxIndex &output_indexes)
{
	return (this->Get_26_Neighborhood(output_indexes.S(), output_indexes.R(),output_indexes.C()));
}


int mxGeometry::Get_27_Neighborhood(int &s, int &r, int &c)
{
    return (this->GetCube(1,s,r,c));
}


int mxGeometry::Get_27_Neighborhood(mxIndex &output_indexes)
{
    return (this->Get_27_Neighborhood(output_indexes.S(), output_indexes.R(),output_indexes.C()));
}


int mxGeometry::GetCircle(unsigned int end_square_radius_included, int &r, int &c)
{
    while(m_current_array_index<m_indexes_circle.GetNumberOfElements() && this->m_current_array_index<=end_square_radius_included)
    {
        if(!m_indexes_circle[this->m_current_array_index].IsEmpty())
        {
            mxIndex *index = &(this->m_indexes_circle[this->m_current_array_index][this->m_current_local_index]);
            
            this->m_current_local_index++;
            if(m_current_local_index==m_indexes_circle[this->m_current_array_index].GetNumberOfElements())
            {
                this->m_current_local_index = 0;
                this->m_current_array_index++;
            }
            
            r = this->m_current_central_indexes.R() + index->R();
            c = this->m_current_central_indexes.C() + index->C();
            if(r>=0 && r<this->m_dimensions[1] && c>=0 && c<this->m_dimensions[0])
            {
                return 1;
            }
        }
        else
        {
            this->m_current_local_index = 0;
            this->m_current_array_index++;
        }
    }
    return 0;
}


int mxGeometry::GetCircle(unsigned int end_square_radius_included, mxIndex &output_indexes)
{
	return (this->GetCircle(end_square_radius_included, output_indexes.R(),output_indexes.C()));
}


int mxGeometry::GetCube(unsigned int end_size_included, int &s, int &r, int &c)
{
    while(m_current_array_index<m_indexes_cube.GetNumberOfElements() && this->m_current_array_index<=end_size_included)
    {
        mxIndex *index = &(this->m_indexes_cube[this->m_current_array_index][this->m_current_local_index]);
        
        this->m_current_local_index++;
        if(m_current_local_index==m_indexes_cube[this->m_current_array_index].GetNumberOfElements())
        {
            this->m_current_local_index = 0;
            this->m_current_array_index++;
        }
        
        s = this->m_current_central_indexes.S() + index->S();
        r = this->m_current_central_indexes.R() + index->R();
        c = this->m_current_central_indexes.C() + index->C();
        if(s>=0 && s<this->m_dimensions[2] && r>=0 && r<this->m_dimensions[1] && c>=0 && c<this->m_dimensions[0])
        {
            return 1;
        }
    
    }
    return 0;
}


int mxGeometry::GetCube(unsigned int end_size_included, mxIndex &output_indexes)
{
    return (this->GetCube(end_size_included, output_indexes.S(),output_indexes.R(),output_indexes.C()));
}


int mxGeometry::GetLine(int &s, int &r, int &c)
{
    s = this->m_line_begin_indexes.S() + ((double)this->m_line_current_local_index) * this->m_line_delta_s;
    r = this->m_line_begin_indexes.R() + ((double)this->m_line_current_local_index) * this->m_line_delta_r;
    c = this->m_line_begin_indexes.C() + ((double)this->m_line_current_local_index) * this->m_line_delta_c;
    
    this->m_line_current_local_index++;
    
    if(s>=0 && s<this->m_dimensions[2] && r>=0 && r<this->m_dimensions[1] && c>=0 && c<this->m_dimensions[0])
    {
        return 1;
    }
    return 0;
}


int mxGeometry::GetLine(mxIndex &output_indexes)
{
    return (this->GetLine(output_indexes.S(),output_indexes.R(),output_indexes.C()));
}


int mxGeometry::GetMaxCircleSquaredRadius()
{
    if(this->m_indexes_circle.IsEmpty())
    {
        this->PopulateCircleIndexArray();
    }
    return this->m_indexes_circle.GetNumberOfElements();
}


int mxGeometry::GetMaxCubeSize()
{
    if(this->m_indexes_cube.IsEmpty())
    {
        this->PopulateCubeIndexArray();
    }
    return this->m_indexes_cube.GetNumberOfElements();
}


int mxGeometry::GetMaxSphereSquaredRadius()
{
    if(this->m_indexes_sphere.IsEmpty())
    {
        this->PopulateSphereIndexArray();
    }
    return this->m_indexes_sphere.GetNumberOfElements();
}


int mxGeometry::GetMaxSquareSize()
{
    if(this->m_indexes_square.IsEmpty())
    {
        this->PopulateSquareIndexArray();
    }
    return this->m_indexes_square.GetNumberOfElements();
}


int mxGeometry::GetSphere(unsigned int end_square_radius_included, int &s, int &r, int &c)
{
    while(m_current_array_index<m_indexes_sphere.GetNumberOfElements() && this->m_current_array_index<=end_square_radius_included)
    {
        if(!m_indexes_sphere[this->m_current_array_index].IsEmpty())
        {
            mxIndex *index = &(this->m_indexes_sphere[this->m_current_array_index][this->m_current_local_index]);
            
            this->m_current_local_index++;
            if(m_current_local_index==m_indexes_sphere[this->m_current_array_index].GetNumberOfElements())
            {
                this->m_current_local_index = 0;
                this->m_current_array_index++;
            }
            
            s = this->m_current_central_indexes.S() + index->S();
            r = this->m_current_central_indexes.R() + index->R();
            c = this->m_current_central_indexes.C() + index->C();
            if(s>=0 && s<this->m_dimensions[2] && r>=0 && r<this->m_dimensions[1] && c>=0 && c<this->m_dimensions[0])
            {
                return 1;
            }
        }
        else
        {
            this->m_current_local_index = 0;
            this->m_current_array_index++;
        }
    }
    return 0;
}


int mxGeometry::GetSphere(unsigned int end_square_radius_included, mxIndex &output_indexes)
{
	return (this->GetSphere(end_square_radius_included, output_indexes.S(),output_indexes.R(),output_indexes.C()));
}


int mxGeometry::GetSquare(unsigned int end_size_included, int &r, int &c)
{
    while(m_current_array_index<m_indexes_square.GetNumberOfElements() && this->m_current_array_index<=end_size_included)
    {
        mxIndex *index = &(this->m_indexes_square[this->m_current_array_index][this->m_current_local_index]);
        
        this->m_current_local_index++;
        if(m_current_local_index==m_indexes_square[this->m_current_array_index].GetNumberOfElements())
        {
            this->m_current_local_index = 0;
            this->m_current_array_index++;
        }
        
        r = this->m_current_central_indexes.R() + index->R();
        c = this->m_current_central_indexes.C() + index->C();
        if(r>=0 && r<this->m_dimensions[1] && c>=0 && c<this->m_dimensions[0])
        {
            return 1;
        }
        
    }
    return 0;
}


int mxGeometry::GetSquare(unsigned int end_size_included, mxIndex &output_indexes)
{
    return (this->GetSquare(end_size_included, output_indexes.R(),output_indexes.C()));
}


void mxGeometry::PopulateCircleIndexArray(unsigned int maximum_radius)
{
	int radius_max = maximum_radius*maximum_radius;// Square of the input radius 

	m_indexes_circle.SetNumberOfElements(radius_max+1);
	
	mxArray< mxList<mxIndex> > coordinates_array;
	coordinates_array.SetNumberOfElements(radius_max+1);

	unsigned int limit = 2*maximum_radius+3;
	mxIndex p0, p_relative, *pp;
	p0.SetIndex(0, maximum_radius+1, maximum_radius+1);

	// Generate all distances and enter them to the array of lists 'coordinates_array'.
	for(unsigned int r=0; r<limit; r++)
	{
		for(unsigned int c=0; c<limit; c++)
		{
            p_relative.SetIndex(0,r,c);
            unsigned int d = p0.DistanceSquaredEuclidean(p_relative);
			if(d<coordinates_array.GetNumberOfElements())
			{
				pp = coordinates_array[d].AddNewToEnd();
				pp->S() = 0; 
				pp->R() = p0.R()-r;
				pp->C() = p0.C()-c;
			}
		}
	}

	//Copy the points into the container array and set its 'size' index array.
	for(unsigned int k=0; k<coordinates_array.GetNumberOfElements(); k++)
	{
		(m_indexes_circle[k]).SetNumberOfElements((coordinates_array[k]).GetNumberOfElements());
        for(unsigned j=0; j<m_indexes_circle[k].GetNumberOfElements() && (!coordinates_array[k].IsEmpty()); j++)
        {
            (m_indexes_circle[k])[j] = coordinates_array[k].GetBeginElement();
            coordinates_array[k].DeleteBegin();
        }
	}
}


void mxGeometry::PopulateCubeIndexArray(unsigned int maximum_size)
{
    //std::cout<<" mxGeometry::PopulateCubeIndexArray() ";
    
    this->m_indexes_cube.SetNumberOfElements(maximum_size+1);
    
    mxArray< mxList<mxIndex> > coordinates_array;
    coordinates_array.SetNumberOfElements(maximum_size+1);
    
    unsigned int limit = 2*maximum_size+3;
    mxIndex p0, p_relative, *pp;
    p0.SetIndex(maximum_size+1, maximum_size+1, maximum_size+1);
    
    // Generate all distances and enter them to the array of lists 'coordinates_array'.
    for(unsigned int s=0; s<limit; s++)
    {
        for(unsigned int r=0; r<limit; r++)
        {
            for(unsigned int c=0; c<limit; c++)
            {
                p_relative.SetIndex(s,r,c);
                unsigned int d = p0.DistanceMaximumNorm(p_relative);
                if(d<coordinates_array.GetNumberOfElements())
                {
                    pp = coordinates_array[d].AddNewToEnd();
                    pp->S() = p0.S()-s;
                    pp->R() = p0.R()-r;
                    pp->C() = p0.C()-c;
                }
            }
        }
    }
    
    //Copy the points into the container array and set its 'size' index array.
    for(unsigned int k=0; k<coordinates_array.GetNumberOfElements(); k++)
    {
        (this->m_indexes_cube[k]).SetNumberOfElements((coordinates_array[k]).GetNumberOfElements());
        for(unsigned j=0; j<m_indexes_cube[k].GetNumberOfElements() && (!coordinates_array[k].IsEmpty()); j++)
        {
            (this->m_indexes_cube[k])[j] = coordinates_array[k].GetBeginElement();
            coordinates_array[k].DeleteBegin();
        }
    }
}


void mxGeometry::PopulateSphereIndexArray(unsigned int maximum_radius)
{
    int radius_max = maximum_radius*maximum_radius;// Square of the input radius
    
    m_indexes_sphere.SetNumberOfElements(radius_max+1);
    
    mxArray< mxList<mxIndex> > coordinates_array;
    coordinates_array.SetNumberOfElements(radius_max+1);
    
    unsigned int limit = 2*maximum_radius+3;
    mxIndex p0, p_relative, *pp;
    p0.SetIndex(maximum_radius+1, maximum_radius+1, maximum_radius+1);
    
    // Generate all distances and enter them to the array of lists 'coordinates_array'.
    for(unsigned int s=0; s<limit; s++)
    {
        for(unsigned int r=0; r<limit; r++)
        {
            for(unsigned int c=0; c<limit; c++)
            {
                p_relative.SetIndex(s,r,c);
                unsigned int d = p0.DistanceSquaredEuclidean(p_relative);
                if(d<coordinates_array.GetNumberOfElements())
                {
                    pp = coordinates_array[d].AddNewToEnd();
                    pp->S() = p0.S()-s;
                    pp->R() = p0.R()-r;
                    pp->C() = p0.C()-c;
                }
            }
        }
    }
    
    //Copy the points into the container array and set its 'size' index array.
    for(unsigned int k=0; k<coordinates_array.GetNumberOfElements(); k++)
    {
        (m_indexes_sphere[k]).SetNumberOfElements((coordinates_array[k]).GetNumberOfElements());
        for(unsigned j=0; j<m_indexes_sphere[k].GetNumberOfElements() && (!coordinates_array[k].IsEmpty()); j++)
        {
            (m_indexes_sphere[k])[j] = coordinates_array[k].GetBeginElement();
            coordinates_array[k].DeleteBegin();
        }
    }
}


void mxGeometry::PopulateSquareIndexArray(unsigned int maximum_size)
{
    this->m_indexes_square.SetNumberOfElements(maximum_size+1);
    
    mxArray< mxList<mxIndex> > coordinates_array;
    coordinates_array.SetNumberOfElements(maximum_size+1);
    
    unsigned int limit = 2*maximum_size+3;
    mxIndex p0, p_relative, *pp;
    p0.SetIndex(0, maximum_size+1, maximum_size+1);
    
    // Generate all distances and enter them to the array of lists 'coordinates_array'.
    for(unsigned int s=0; s<limit; s++)
    {
        for(unsigned int r=0; r<limit; r++)
        {
            for(unsigned int c=0; c<limit; c++)
            {
                p_relative.SetIndex(0,r,c);
                unsigned int d = p0.DistanceMaximumNorm(p_relative);
                if(d<coordinates_array.GetNumberOfElements())
                {
                    pp = coordinates_array[d].AddNewToEnd();
                    pp->S() = 0;
                    pp->R() = p0.R()-r;
                    pp->C() = p0.C()-c;
                }
            }
        }
    }
    
    //Copy the points into the container array and set its 'size' index array.
    for(unsigned int k=0; k<coordinates_array.GetNumberOfElements(); k++)
    {
        (this->m_indexes_square[k]).SetNumberOfElements((coordinates_array[k]).GetNumberOfElements());
        for(unsigned j=0; j<m_indexes_square[k].GetNumberOfElements() && (!coordinates_array[k].IsEmpty()); j++)
        {
            (this->m_indexes_square[k])[j] = coordinates_array[k].GetBeginElement();
            coordinates_array[k].DeleteBegin();
        }
    }
}


void mxGeometry::SetDimensions(unsigned int slices, unsigned int rows, unsigned int columns)
{
    this->m_dimensions[0] = columns;
    this->m_dimensions[1] = rows;
    this->m_dimensions[2] = slices;
}


void mxGeometry::SetDimensions(unsigned int rows, unsigned int columns)
{
    this->SetDimensions(1,rows,columns);
}


