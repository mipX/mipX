/*=========================================================================
 
 Program:   mipx
 Module:    mxConnectedComponent3DStatic.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/





#include "mxConnectedComponent3DStatic.h"






template<class T>
bdStaticRegionNode<T>& bdStaticRegionNode<T>::operator ()(int index)
{
	m_index = index;
	return (*this);
}


template <class T>
int bdStaticRegionNode<T>::operator ==(const bdStaticRegionNode<T> &rn)
{
	if(m_index==rn.m_index) return 1;
	else return 0;
}




template <class T>
int bdStaticRegionNode<T>::operator >(const bdStaticRegionNode<T> &rn)
{
	if(m_index>rn.m_index) return 1;
	else return 0;
}



template <class T>
int bdStaticRegionNode<T>::operator >=(const bdStaticRegionNode<T> &rn)
{
	if(m_index>=rn.m_index) return 1;
	else return 0;
}



template <class T>
int bdStaticRegionNode<T>::operator <(const bdStaticRegionNode<T> &rn)
{
	if(m_index<rn.m_index) return 1;
	else return 0;
}




//-------------------------------------------------------------------------------------------------------------------------------




template<class T>
void bdStaticRegionIterator<T>::SetBegin(mxConnectedComponent3DStatic<T> &region)
{
	m_region = &region;
	if(!region.GetNumberOfElements()) m_node = NULL;
	if(!region.IsDimensionsSet()) m_node = NULL;

	for(m_s=0; m_s<region.GetNumberOfSlices(); m_s++)
	{
		for(m_r=0; m_r<region.GetNumberOfRows(); m_r++)
		{
			if(!(region.GetGrid()(m_s,m_r)).IsEmpty())
			{
				m_node = (region.GetGrid()(m_s,m_r)).GetLeftEndNodePointer();
				return;
			}
		}
	}
}


template<class T>
void bdStaticRegionIterator<T>::SetBegin(mxConnectedComponent3DStatic<T> *region)
{
	m_region = region;
	if(!region->GetNumberOfElements()) m_node = NULL;
	if(!region->IsDimensionsSet()) m_node = NULL;

	for(m_s=0; m_s<region->GetNumberOfSlices(); m_s++)
	{
		for(m_r=0; m_r<region->GetNumberOfRows(); m_r++)
		{
			if(!(region->GetGrid()(m_s,m_r)).IsEmpty())
			{
				m_node = (region->GetGrid()(m_s,m_r)).GetLeftEndNodePointer();
				return;
			}
		}
	}
}


template<class T>
int bdStaticRegionIterator<T>::IsValid()
{
	if(m_node) return 1;
	else return 0;
}


template<class T>
void bdStaticRegionIterator<T>::MoveToNext()
{
	// Find the next element in the current list...
	if(!m_node->IsRightEnd()) 
	{ 
		m_node = m_node->m_right; 
		return;
	}

	//... or the next element in the grid.
	while(m_s<m_region->GetNumberOfSlices())
	{
		m_r = (m_r+1) % m_region->GetNumberOfRows();
		if(m_r==0) m_s++;
		if(m_s>=m_region->GetNumberOfSlices()) { m_node = NULL; return; }

		if(!(m_region->GetGrid()(m_s,m_r)).IsEmpty())
		{
			m_node = (m_region->GetGrid()(m_s,m_r)).GetLeftEndNodePointer();
			return;
		}
	}

	m_node = NULL;
}


template<class T>
void bdStaticRegionIterator<T>::GetCoordinates(int &s, int &r, int &c)
{
	c = this->GetRegionNodePointer()->m_index;
	r = m_r;
	s = m_s;
}





//-------------------------------------------------------------------------------------------------------------------------------





template<class T>
mxConnectedComponent3DStatic<T>::mxConnectedComponent3DStatic()
{
	m_number_of_slices = m_number_of_rows = m_number_of_columns = 0;
	m_number_of_elements = 0;
}



template<class T>
mxConnectedComponent3DStatic<T>::~mxConnectedComponent3DStatic()
{
	this->Reset();
}


template<class T>
int mxConnectedComponent3DStatic<T>::SetDimensions(int s_dimension, int r_dimension, int c_dimension)
{
	if(s_dimension<0 || r_dimension<0 || c_dimension<0) return 0;
	m_matrix.SetSize(s_dimension, r_dimension);
	m_number_of_slices = s_dimension;
	m_number_of_rows = r_dimension;
	m_number_of_columns = c_dimension;
	return 1;
}


template<class T>
int mxConnectedComponent3DStatic<T>::IsDimensionsSet()
{
	if(m_number_of_slices>0 && m_number_of_rows>0 && m_number_of_columns>0 && (!m_matrix.IsEmpty())) return 1;
	else return 0;
}


template<class T>
int mxConnectedComponent3DStatic<T>::IsEmpty()
{
	if(m_number_of_elements!=0) return 0;
	else return 1;
}


template<class T>
void mxConnectedComponent3DStatic<T>::Reset()
{
	for(unsigned int r=0; r<m_matrix.GetNumberOfRows(); r++)
	{
		for(unsigned int c=0; c<m_matrix.GetNumberOfColumns(); c++) { m_matrix(r,c).Reset(); }
	}
	m_matrix.Reset();
	m_number_of_elements = 0;
	m_number_of_slices = m_number_of_rows = m_number_of_columns = 0;
}


template<class T>
void mxConnectedComponent3DStatic<T>::RemoveAllPointsFromRegion()
{
	for(unsigned int r=0; r<m_matrix.GetNumberOfRows(); r++)
	{
		for(unsigned int c=0; c<m_matrix.GetNumberOfColumns(); c++) { m_matrix(r,c).Reset(); }
	}
	m_number_of_elements = 0;
}


template<class T>
int mxConnectedComponent3DStatic<T>::DeletePoint(int s, int r, int c)//returns 0 if point not found, 1 if delete succeeded
{
	if(IsEmpty()) return 0;

	bdStaticRegionNode<T> node; 
	bdListNode< bdStaticRegionNode<T> > *ln = bdListing::GetNodeWithElement_Ascending((m_matrix(s,r)),node(c));
	if(!ln) return 0;
	else
	{
		(m_matrix(s,r)).DeleteNode(ln);
		m_number_of_elements--;
		return 1;
	}
}


template<class T>
int mxConnectedComponent3DStatic<T>::DeletePoint(bdVoxel &p)
{
	return(this->DeletePoint(p.S(),p.R(),p.C()));
}


template<class T>
int mxConnectedComponent3DStatic<T>::AddPointToRegion(int s, int r, int c)
{
	T el;
	return this->AddPointToRegion(s,r,c,el);
}


template<class T>
int mxConnectedComponent3DStatic<T>::AddPointToRegion(bdVoxel &p)
{
	return(this->AddPointToRegion(p.S(),p.R(),p.C()));
}


template<class T>
int mxConnectedComponent3DStatic<T>::AddPointToRegion(int s, int r, int c, T &element)
{

	//If the list at [s][r] position is empty (but the whole region is not)
	if(m_matrix(s,r).IsEmpty()) 
	{		
		bdStaticRegionNode<T> *srn = m_matrix(s,r).AddNewToRightEnd();	
		srn->m_index = c;
		srn->m_element = element;	
		m_number_of_elements++;
		return 1;
	}
	else
	{
		bdStaticRegionNode<T> srn; 
		srn.m_element = element;

		bdListNode< bdStaticRegionNode<T> > *ln = bdListing::GetNodeWithElementGreaterOrEqual_Ascending((m_matrix(s,r)),srn(c));
		// If the element is not found, we should add to the right end of the list.
		if(!ln) { m_matrix(s,r).AddToRightEnd(srn); return 1; }
		else 
		{	
			// Check if the node is already in the list, add point if not.
			if(ln->GetElementPointer()->m_index != c)
			{
				m_matrix(s,r).AddToLeftOfNode(ln,srn); 
				return 1;
			}
			else return 0;// If node in the list, return 0!
		}
	}
}


template<class T>
int mxConnectedComponent3DStatic<T>::AddPointToRegion(bdVoxel &p, T &element)
{
	return(this->AddPointToRegion(p.S(),p.R(),p.C(),element));
}


template<class T>
T* mxConnectedComponent3DStatic<T>::GetElementPointer(int s, int r, int c)
{
	if(this->IsEmpty()) return NULL;
	if(s<0 || s>=m_number_of_slices || r<0 || r>=m_number_of_rows || c<0 || c>=m_number_of_columns) return NULL;
	bdStaticRegionNode<T> srn;
	bdListNode< bdStaticRegionNode<T> > *ln = bdListing::GetNodeWithElement_Ascending((m_matrix(s,r)),srn(c));
	if(!ln) return NULL;
	else return (&(ln->GetElementPointer()->m_element));
}


template <class T>
int mxConnectedComponent3DStatic<T>::IsPointInTheRegion(int s, int r, int c)
{
	if(this->IsEmpty()) return 0;
	if(s<0 || s>=m_number_of_slices || r<0 || r>=m_number_of_rows || c<0 || c>=m_number_of_columns) return 0;
	bdStaticRegionNode<T> srn;
	bdListNode< bdStaticRegionNode<T> > *ln = bdListing::GetNodeWithElement_Ascending((m_matrix(s,r)),srn(c));
	if(!ln) return 0;
	else return 1;
}


template <class T>
int mxConnectedComponent3DStatic<T>::IsPointInTheRegion(bdVoxel &p)
{
	return(this->IsPointInTheRegion(p.S(),p.R(),p.C()));
}


template<class T>
mxConnectedComponent3DStatic<T>& mxConnectedComponent3DStatic<T>::operator =(mxConnectedComponent3DStatic &region)
{
	if (&region==this) return *this;
	this->Reset();
	m_matrix = region.m_matrix;
	m_number_of_elements = region.m_number_of_elements;
	m_number_of_slices = region.m_number_of_slices;
	m_number_of_rows = region.m_number_of_rows;
	m_number_of_columns = region.m_number_of_columns;
	return *this;
}




template<class T>
int mxConnectedComponent3DStatic<T>::CreateRegionOf_26_NeighborhoodFromBinaryImage(bdImage16U &binary_image, bdVoxel &starting_point)
{
	if(binary_image.IsEmpty()) return 0;
	if(!this->IsEmpty()) this->Reset();

	bdGeometry g;
	g.SetDimensions(binary_image.GetNumberOfSlices(),binary_image.GetNumberOfRows(),binary_image.GetNumberOfColumns());
	this->SetDimensions(binary_image.GetNumberOfSlices(),binary_image.GetNumberOfRows(),binary_image.GetNumberOfColumns());

	bdList<bdVoxel> l;
	l.AddToRightEnd(starting_point);
	this->AddPointToRegion(starting_point);
	
	int s,r,c;
	for(; !l.IsEmpty(); l.DeleteLeftEnd() )
	{
		bdVoxel p = l.GetLeftEnd();
		for(g.ForCoordinates(p.S(),p.R(),p.C(),g.Neighborhood26StartIndex()); g.Get_26_Neighbors(s,r,c); )
		{
			if(binary_image(s,r,c)!=0)
			{
				//If a point doesn't already exist in the region, add it to the list of points
				if(this->AddPointToRegion(s,r,c)) { l.AddToRightEnd(p(s,r,c)); }
			}
		}
	}
	return 1;
}


template<class T>
int mxConnectedComponent3DStatic<T>::PasteRegionToImage(bdImage16U &image, int value)
{
	if(this->IsEmpty()) return 0;
	if(image.IsEmpty()) return 0;
	if(!(m_number_of_slices==image.GetNumberOfSlices() && m_number_of_rows==image.GetNumberOfRows() 
		&& m_number_of_columns==image.GetNumberOfColumns())) return 0;

	int s,r,c;
	bdStaticRegionIterator<T> it;
	for(it.SetBegin(this); it.IsValid(); it.MoveToNext())
	{
		it.GetCoordinates(s,r,c);
		image(s,r,c) = value;
	}
	return 1;
}






template class BD_STATIC_REGION_3D_API mxConnectedComponent3DStatic<int>;
template class BD_STATIC_REGION_3D_API bdStaticRegionIterator<int>;
template class BD_STATIC_REGION_3D_API bdStaticRegionNode<int>;






