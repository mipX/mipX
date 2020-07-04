/*=========================================================================
 
 Program:   mipx
 Module:    mxConnectedComponent3DStatic.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



//
//#if defined(_MSC_VER) //  Microsoft
//    #ifdef BD_STATIC_REGION_3D_EXPORTS
//        #define BD_STATIC_REGION_3D_API __declspec(dllexport)
//    #else
//        #define BD_STATIC_REGION_3D_API __declspec(dllimport)
//    #endif
//#else // GCC
//    #ifdef BD_STATIC_REGION_3D_EXPORTS
//        #define BD_STATIC_REGION_3D_API __attribute__((visibility("default")))
//    #else
//        #define BD_STATIC_REGION_3D_API
//    #endif
//#endif



#ifndef mxConnectedComponent3DStatic_H
	#define mxConnectedComponent3DStatic_H


#include "mxImage.h"
#include "mxVoxel.h"
#include "mxGeometry.h"
#include "mxList.h"
#include "mxMatrix.h"




/// Node element of mxConnectedComponent3DStatic.

template<class T>
class mxConnectedComponent3DStaticNode
{
public:
    
    int m_index;
    T m_element;

    /// Constructor.
    mxConnectedComponent3DStaticNode() {};
    
    /// Destructor.
    ~mxConnectedComponent3DStaticNode() {};

    /// Indexing operator.
	mxConnectedComponent3DStaticNode<T>& operator ()(int index)
    {
        m_index = index;
        return (*this);
    };

	// WARNING! operator==() conmpares ONLY m_index of bdStaticRegionNode. So if m_index==rn.m_index it returns 1.
	int operator ==(const mxConnectedComponent3DStaticNode<T> &rn)
    {
        if(m_index==rn.m_index) return 1;
        return 0;
    };

	// WARNING! operator>() conmpares ONLY m_index of bdStaticRegionNode. So if m_index>rn.m_index it returns 1.
	int operator > (const mxConnectedComponent3DStaticNode<T> &rn)
    {
        if(m_index>rn.m_index) return 1;
        return 0;
    };

	// WARNING! operator>=() conmpares ONLY m_index of bdStaticRegionNode. So if m_index>=rn.m_index it returns 1.
	int operator >= (const mxConnectedComponent3DStaticNode<T> &rn)
    {
        if(m_index>=rn.m_index) return 1;
        else return 0;
    };

	// WARNING! operator<() conmpares ONLY m_index of bdStaticRegionNode. So if m_index<rn.m_index it returns 1.
	int operator < (const mxConnectedComponent3DStaticNode<T> &rn)
    {
        if(m_index<rn.m_index) return 1;
        else return 0;
    };
};



//-------------------------------------------------------------------------------------------------------------------------------


template<class T> class  mxConnectedComponent3DStatic;


/// Iterator for mxConnectedComponent3DStatic. Example use:
/// for(iterator.SetToBegin(connected_component); iterator.IsValid(); iterator.MoveToNext()) {...do something}

template<class T>
class mxConnectedComponent3DStaticIterator
{
    
private:
    
    /// Pointer to the current list node.
	mxListNode< mxConnectedComponent3DStaticNode<T> > *m_node;
    
    /// Pointer to the region it iterates through.
    mxConnectedComponent3DStatic<T> *m_region;
    
    /// Current indexes in the static part of the region.
	int m_s, m_r;
    
public:

	/// Constructor.
	mxConnectedComponent3DStaticIterator()
    {
        m_node = NULL; m_region = NULL; m_s = m_r = 0;
    };
    
    /// Destructor.
	~mxConnectedComponent3DStaticIterator()
    {
        this->Reset();
    };

	/// Reset object to initial state (just after construction).
	void Reset()
    {
        m_node = NULL; m_region = NULL; m_s = m_r = 0;
    };

	/// Set the iterator to begin of the region.
    void SetToBegin(mxConnectedComponent3DStatic<T> *region);

	/// Check if the iterator is valid.
	int IsValid()
    {
        if(m_node) return 1;
        return 0;
    };

	/// Move iterator in the region to next region node.
	void MoveToNext();
	
	/// Get node pointer.
	mxConnectedComponent3DStaticNode<T>* GetNode()
    {
        return(m_node->GetElementAddress());
    };
    
    /// Get element pointer.
    T* GetElementAddress()
    {
        return(&(m_node->GetElementAddress()->m_element));
    };
    
    /// Get element.
    //Does not check validity of node pointer (assumes it is correct- use in FOR loop!)
    T& GetElement()
    {
        return(m_node->GetElementAddress()->m_element);
    };
    
    /// Get coordinates.
	void GetCoordinates(int &s, int &r, int &c)
    {
        c = this->GetNode()->m_index;
        r = m_r;
        s = m_s;
    };
	
	// Get Node address.
	mxListNode< mxConnectedComponent3DStaticNode<T> >* GetListNode()
    {
        return m_node;
    };
};




//-------------------------------------------------------------------------------------------------------------------------------



/// Static region, which is a 2D matrix of lists of voxel indexes. Use for fast access in case of 3D space.

template<class T>
class  mxConnectedComponent3DStatic
{
private:

	/// Matrix of lists containing SR node (index and a Template element)
	mxMatrix< mxList< mxConnectedComponent3DStaticNode<T> > > m_matrix;

	/// Size of the 3D image (or region area - must be defined!)
    int m_dimension_S;
    int m_dimension_R;
    int m_dimension_C;

	/// Total number of elements in the region.
	int m_number_of_elements;


public:

    /// Constructor.
	mxConnectedComponent3DStatic()
    {
        m_dimension_S = m_dimension_R = m_dimension_C = 0;
        m_number_of_elements = 0;
    };
    
    /// Destructor.
	~mxConnectedComponent3DStatic()
    {
        this->Reset();
    };
	
	/// Set dimensions of a region (usually taken from image size). Success 1, fail 0.
	int SetDimensions(int s_dimension, int r_dimension, int c_dimension)
    {
        if(s_dimension<0 || r_dimension<0 || c_dimension<0) return 0;
        m_matrix.SetDimensions(s_dimension, r_dimension);
        m_dimension_S = s_dimension;
        m_dimension_R = r_dimension;
        m_dimension_C = c_dimension;
        return 1;
    };
	
    /// Check if the dimensions are set.
    int IsDimensionsSet()
    {
        if(m_dimension_S>0 && m_dimension_R>0 && m_dimension_C>0 && (!m_matrix.IsEmpty())) return 1;
        return 0;
    };

	/// Check if the region is empty.
	int IsEmpty()
    {
        if(m_number_of_elements) return 0;
        return 1;
    };
	
	/// Reset the object to its initial state (just like after construction).
	void Reset()
    {
        for(unsigned int r=0; r<m_matrix.GetNumberOfRows(); r++)
        {
            for(unsigned int c=0; c<m_matrix.GetNumberOfColumns(); c++)
            {
                m_matrix(r,c).Reset();
            }
        }
        m_matrix.Reset();
        m_number_of_elements = 0;
        m_dimension_S = m_dimension_R = m_dimension_C = 0;
    };

	/// Removes all points from the region, but does NOT reset the dimenstions od the region!
	void RemoveAllPoints()
    {
        for(unsigned int r=0; r<m_matrix.GetNumberOfRows(); r++)
        {
            for(unsigned int c=0; c<m_matrix.GetNumberOfColumns(); c++)
            {
                m_matrix(r,c).Reset();
            }
        }
        m_number_of_elements = 0;
    };

	/// Get number of total elements, slices, rows, columns.
	int GetNumberOfElements()
    {
        return m_number_of_elements;
    };
    
	int GetDimension_S()
    {
        return m_dimension_S;
    };
    
    int GetDimension_R()
    {
        return m_dimension_R;
    };
    
    int GetDimension_C()
    {
        return m_dimension_C;
    };

	/// Get pointer to static grid (matrix) containing lists.
	mxMatrix< mxList< mxConnectedComponent3DStaticNode<T> > >& GetGrid()
    {
        return m_matrix;
    };

	/// Delete a point from the region.
	int DeletePoint(int s, int r, int c)
    {
        if(IsEmpty()) return 0;
        
        mxConnectedComponent3DStaticNode<T> node;
        mxListNode< mxConnectedComponent3DStaticNode<T> > *ln = mxListFunctions::GetNodeWithElement_Ascending((m_matrix(s,r)),node(c));
        if(!ln) return 0;
        else
        {
            (m_matrix(s,r)).DeleteNode(ln);
            m_number_of_elements--;
            return 1;
        }
    };
    
	int DeletePoint(mxVoxel &p)
    {
        return this->DeletePoint(p.S(), p.R(), p.C());
    };
    
    
    /// Adds point to the region. Returns 1 if successful, 0 if the point already is in the region.
	int AddPoint(int s, int r, int c, T &element)
    {
        //If the list at [s][r] position is empty (but the whole region is not)
        if(m_matrix(s,r).IsEmpty())
        {
            mxConnectedComponent3DStaticNode<T> *srn = m_matrix(s,r).AddNewToEnd();
            srn->m_index = c;
            srn->m_element = element;
            m_number_of_elements++;
            return 1;
        }
        else
        {
            mxConnectedComponent3DStaticNode<T> srn;
            srn.m_element = element;
            
            mxListNode< mxConnectedComponent3DStaticNode<T> > *ln = mxListFunctions::GetNodeWithElementGreaterOrEqual_Ascending((m_matrix(s,r)),srn(c));
            // If the element is not found, we should add to the right end of the list.
            if(!ln) { m_matrix(s,r).AddToEnd(srn); return 1; }
            else
            {
                // Check if the node is already in the list, add point if not.
                if(ln->GetElementAddress()->m_index != c)
                {
                    m_matrix(s,r).AddBeforeNode(ln,srn);
                    return 1;
                }
                else return 0;// If node in the list, return 0!
            }
        }
    };
    
    int AddPoint(int s, int r, int c)
    {
        T el;
        return this->AddPoint(s,r,c,el);
    };
    
    int AddPoint(mxVoxel &p)
    {
        return this->AddPoint(p.S(), p.R(), p.C());
    };
    
	int AddPoint(mxVoxel &p, T &element)
    {
        return this->AddPoint(p.S(), p.R(), p.C(), element);
    };
	
	/// Get element for given coordinates.
	T* GetElementAddress(int s, int r, int c)
    {
        if(this->IsEmpty()) return NULL;
        if(s<0 || s>=m_dimension_S || r<0 || r>=m_dimension_R || c<0 || c>=m_dimension_C) return NULL;
        mxConnectedComponent3DStaticNode<T> srn;
        mxListNode< mxConnectedComponent3DStaticNode<T> > *ln = mxListFunctions::GetNodeWithElement_Ascending((m_matrix(s,r)),srn(c));
        if(!ln) return NULL;
        return (&(ln->GetElementAddress()->m_element));
    };

	/// Check if the point is in the region.
	int IsPointInConnectedComponent(int s, int r, int c)
    {
        if(this->IsEmpty()) return 0;
        if(s<0 || s>=m_dimension_S || r<0 || r>=m_dimension_R || c<0 || c>=m_dimension_C) return 0;
        mxConnectedComponent3DStaticNode<T> srn;
        mxListNode< mxConnectedComponent3DStaticNode<T> > *ln = mxListFunctions::GetNodeWithElement_Ascending((m_matrix(s,r)),srn(c));
        if(!ln) return 0;
        return 1;
    };
    
	int IsPointInConnectedComponent(mxVoxel &p)
    {
        return(this->IsPointInConnectedComponent(p.S(),p.R(),p.C()));
    };

	/// Assignment operator.
	mxConnectedComponent3DStatic<T>& operator =(mxConnectedComponent3DStatic<T> &region)
    {
        if (&region==this) return *this;
        this->Reset();
        m_matrix = region.m_matrix;
        m_number_of_elements = region.m_number_of_elements;
        m_dimension_S = region.m_dimension_S;
        m_dimension_R = region.m_dimension_R;
        m_dimension_C = region.m_dimension_C;
        return *this;
    };

	/// Create a region using 26-neighborhood region growing from given start point in a binary image.
	int CreateFromImage_26Neighborhood(mxImage &binary_image, unsigned int seed_S, unsigned int seed_R, unsigned int seed_C)
    {
        if(binary_image.IsEmpty()) return 0;
        if(!this->IsEmpty()) this->Reset();
        
        mxGeometry g;
        g.SetDimensions(binary_image.GetDimension_S(), binary_image.GetDimension_R(), binary_image.GetDimension_C());
        this->SetDimensions(binary_image.GetDimension_S(),binary_image.GetDimension_R(),binary_image.GetDimension_C());
        
        mxVoxel starting_point;
        starting_point.SetIndex(seed_S,seed_R,seed_C);
        
        mxList< mxVoxel > l;
        l.AddToEnd(starting_point);
        this->AddPoint(starting_point);
        
        int s,r,c;
        for(; !l.IsEmpty(); l.DeleteBegin() )
        {
            mxVoxel p = l.GetBeginElement();
            for(g.For_26_Neighborhood(p.S(),p.R(),p.C()); g.Get_26_Neighborhood(s,r,c); )
            {
                if(binary_image(s,r,c)!=0)
                {
                    //If a point doesn't already exist in the region, add it to the list of points
                    if(this->AddPoint(s,r,c))
                    {
                        p.SetIndex(s,r,c);
                        l.AddToEnd(p);
                    }
                }
            }
        }
        return 1;
    };

    /// Create a region using 4-neighborhood region growing from given start point in a binary image.
    int CreateFromImage_4Neighborhood(mxImage &binary_image, unsigned int seed_S, unsigned int seed_R, unsigned int seed_C)
    {
        if(binary_image.IsEmpty()) return 0;
        if(!this->IsEmpty()) this->Reset();
        
        mxGeometry g;
        g.SetDimensions(binary_image.GetDimension_S(), binary_image.GetDimension_R(), binary_image.GetDimension_C());
        this->SetDimensions(binary_image.GetDimension_S(),binary_image.GetDimension_R(),binary_image.GetDimension_C());
        
        mxVoxel starting_point;
        starting_point.SetIndex(seed_S,seed_R,seed_C);
        
        mxList< mxVoxel > l;
        l.AddToEnd(starting_point);
        this->AddPoint(starting_point);
        
        int r,c;
        for(; !l.IsEmpty(); l.DeleteBegin() )
        {
            mxVoxel p = l.GetBeginElement();
            for(g.For_4_Neighborhood(p.R(),p.C()); g.Get_4_Neighborhood(r,c); )
            {
                if(binary_image(seed_S,r,c)!=0)
                {
                    //If a point doesn't already exist in the region, add it to the list of points
                    if(this->AddPoint(seed_S,r,c))
                    {
                        p.SetIndex(seed_S,r,c);
                        l.AddToEnd(p);
                    }
                }
            }
        }
        return 1;
    };
    
    
    /// Create a region using 3-neighborhood region growing in the direction of increasing R values from given start point in a binary image.
    int CreateFromImage_3NeighborhoodDirection_R_Increasing(mxImage &binary_image, unsigned int seed_S, unsigned int seed_R, unsigned int seed_C)
    {
        if(binary_image.IsEmpty()) return 0;
        if(!this->IsEmpty()) this->Reset();
        
        mxGeometry g;
        g.SetDimensions(binary_image.GetDimension_S(), binary_image.GetDimension_R(), binary_image.GetDimension_C());
        this->SetDimensions(binary_image.GetDimension_S(),binary_image.GetDimension_R(),binary_image.GetDimension_C());
        
        mxVoxel starting_point;
        starting_point.SetIndex(seed_S,seed_R,seed_C);
        
        mxList< mxVoxel > l;
        l.AddToEnd(starting_point);
        this->AddPoint(starting_point);
        
        int r,c;
        for(; !l.IsEmpty(); l.DeleteBegin() )
        {
            mxVoxel p = l.GetBeginElement();
            for(g.For_8_Neighborhood(p.R(),p.C()); g.Get_8_Neighborhood(r,c); )
            {
                if(r>p.R())
                {
                    if(binary_image(seed_S,r,c)!=0)
                    {
                        //If a point doesn't already exist in the region, add it to the list of points
                        if(this->AddPoint(seed_S,r,c))
                        {
                            p.SetIndex(seed_S,r,c);
                            l.AddToEnd(p);
                        }
                    }
                }
            }
        }
        return 1;
    };
    
	/// Paste a region to an image or PRE-SET SIZE. Warning: IMAGE SIZE HAS TO BE SET BEFORE CALLING THIS FUNCTION.
	int PasteToImage(mxImage16U &image, int value)
    {
        if(this->IsEmpty()) return 0;
        if(image.IsEmpty()) return 0;
        if(!(m_dimension_S==image.GetDimension_S() && m_dimension_R==image.GetDimension_R() && m_dimension_C==image.GetDimension_C())) return 0;
        
        int s,r,c;
        mxConnectedComponent3DStaticIterator<T> it;
        for(it.SetToBegin(this); it.IsValid(); it.MoveToNext())
        {
            it.GetCoordinates(s,r,c);
            image(s,r,c) = value;
        }
        return 1;
    };

};





template<class T>
void mxConnectedComponent3DStaticIterator<T>::SetToBegin(mxConnectedComponent3DStatic<T> *region)
{
    m_region = region;
    if(!region->GetNumberOfElements()) m_node = NULL;
    if(!region->IsDimensionsSet()) m_node = NULL;
            
    for(m_s=0; m_s<region->GetDimension_S(); m_s++)
    {
        for(m_r=0; m_r<region->GetDimension_R(); m_r++)
        {
            if(!(region->GetGrid()(m_s,m_r)).IsEmpty())
            {
                m_node = (region->GetGrid()(m_s,m_r)).GetBeginNodeAddress();
                return;
            }
        }
    }
};


template<class T>
void mxConnectedComponent3DStaticIterator<T>::MoveToNext()
{
    // Find the next element in the current list...
    if(!m_node->IsEnd())
    {
        m_node = m_node->m_next_node;
        return;
    }
    
    //... or the next element in the grid.
    while(m_s<m_region->GetDimension_S())
    {
        m_r = (m_r+1) % m_region->GetDimension_R();
        if(m_r==0) m_s++;
        if(m_s>=m_region->GetDimension_S()) { m_node = NULL; return; }
        
        if(!(m_region->GetGrid()(m_s,m_r)).IsEmpty())
        {
            m_node = (m_region->GetGrid()(m_s,m_r)).GetBeginNodeAddress();
            return;
        }
    }
    
    m_node = NULL;
};




#endif
