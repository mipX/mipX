/*=========================================================================

Program:   mipx
Module:    mxPointList.h

Authors: Danilo Babin, Hrvoje Leventic.
Copyright (c) Danilo Babin, Hrvoje Leventic.
All rights reserved.
See Copyright.txt

Licensed under the BSD License 2.0.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the above copyright notice for more information.

=========================================================================*/





#if defined(mxPointList_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxPointList_API
#else
    #if defined(_MSC_VER)
        #ifdef mxPointList_EXPORTS
            #define mxPointList_API __declspec(dllexport)
        #else
            #define mxPointList_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxPointList_EXPORTS
            #define mxPointList_API __attribute__((visibility("default")))
        #else
            #define mxPointList_API
        #endif
    #endif
#endif



#ifndef mxPointList_H
    #define mxPointList_H


#include "mxDataObject.h"
#include "mxList.h"
#include "mxPoint.h"
#include "mxString.h"



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




/// Container element of mxPointList.

class mxPointList_API mxPointListElement
{
public:
	
	/// Position.
	mxPoint m_position;
	
	/// Description of the position - used to label/name the position.
    mxString m_label;
    
	/// Constructor.
    mxPointListElement()
    {
        m_position.SetPosition(0,0,0);
        m_label.Assign("");
    };
    
    /// Destructor.
    ~mxPointListElement() {};
    
    /// Get pointer to point.
    mxPoint* GetPoint() { return &m_position; };

	/// Reset the object to the initial state (just after construction).
	void Reset()
    {
        m_position.SetPosition(0,0,0);
        m_label.Assign("");
    };
};


/// Helper class to map scalar labels to indexes

class mxPointList_API mxPointListScalarTagElement
{
public:
    
    /// Tag name.
    mxString m_tag;
    
    /// Value 0 means unused, otherwise used.
    int m_is_used;
    
    /// For visualization purposes. Tells the range of scalars that needs to be visualized.
    /// If lower bound value is greater than higher bound value, the whole scalars range should be used.
    double m_scalar_visualization_range[2];
    
    /// Constructor.
    mxPointListScalarTagElement()
    {
        m_is_used = 0;
        m_tag.Assign("");
        m_scalar_visualization_range[0] = 1;
        m_scalar_visualization_range[1] = 0;
    };
    
    /// Destructor.
    ~mxPointListScalarTagElement() {};
    
    void Copy(mxPointListScalarTagElement *ste)
    {
        if(!ste) return;
        m_is_used = ste->m_is_used;
        m_tag.Assign(ste->m_tag);
        m_scalar_visualization_range[0] = ste->m_scalar_visualization_range[0];
        m_scalar_visualization_range[1] = ste->m_scalar_visualization_range[1];
    };
    
    /// Get scalar tag.
    mxString* GetTag() { return &m_tag; };

    /// Get visualization range.
    double* GetVisualizationRange() { return m_scalar_visualization_range; };
    
    /// Check if the scalar field is being used.
    int IsUsed() { return m_is_used; };
    
    /// Reset the object to the initial state (just after construction).
    void Reset()
    {
        m_is_used = 0;
        m_tag.Assign("");
        m_scalar_visualization_range[0] = 1;
        m_scalar_visualization_range[1] = 0;
    };
};



//-----------------------------------------------------------------------------------------------------------------------------------------------



/// List of positions ( class mxPoint ). Each position has a textual description and a textual tag. Tag strings can be mapped to integers for faster access.


class mxPointList_API mxPointList : public mxDataObject
{
protected:

	/// Array of scalar names and indexes.
	mxPointListScalarTagElement m_scalars[mxPoint_NUMBER_OF_VALUES];
    
	/// Positions in the list.
	mxList< mxPointListElement > m_positions;

public:

	/// Type iterator for position list.
	typedef mxListIterator< mxPointListElement > Iterator;

	/// Constructor.
    mxPointList();

	/// Destructor.
	virtual ~mxPointList();
    
    /// Add a position to the list (creates default label as index of the position). Returns the pointer to the added position list element.
    mxPointListElement* AddPosition(double s, double r, double c);
    mxPointListElement* AddPosition(mxPosition *p);
    mxPointListElement* AddPosition(mxPoint *p);
    
    /// Add positions from a position list.
    void AddPositions(mxPointList &pl);
    
    /// Add a new scalar tag. Return success 1, fail 0.
    int AddScalarTag(const char *tag, unsigned int &output_index);
    
    /// Make an exact copy from input object.
    int Copy(mxPointList *l);
	
	/// Get point list element that is the closest to input position.
	mxPointListElement* GetClosestPointListElement(double s, double r, double c);
    
    /// Get list of points in the point list.
    mxList< mxPointListElement >* GetListOfPoints() { return &m_positions; };

    /// Get the number of positions.
    unsigned int GetNumberOfElements();
    
	/// Get maximum number of scalars that can be stored (defined by number of scalars in mxPoint).
    unsigned int GetMaximumNumberOfScalars() { return mxPoint_NUMBER_OF_VALUES; };

	/// For an input tag get its index in the mxPoint objects. Use this to get index for sequential access
	/// to the specific scalars (defined by input tag). Success 1, fail 0.
	int GetScalarIndex(const char *tag, unsigned int &output_index);
    
    /// Get minimum and maximum value of scalars with given index. Success 1, fail 0.
    int GetScalarValueRange(unsigned int scalar_index, double &output_min, double &output_max);
    
    /// Check if the list contains a position with given input coordinates.
    int HasPositionIn3D(mxPosition &position);
    
    /// Check if the position list is empty.
    int IsEmpty();
    
    /// Load position list from a file.
    int LoadFromPSLFile(const char *file_name);
    
    /// Indexing operator.
    mxPoint& operator[] (unsigned int index);
    
    /// Remove a position.
    void RemovePosition(mxPosition *p);
    
    /// Remove multiple positions that have the same coordinates (leaves only one).
    /// Warning: the method does not take descriptions into account (if descriptions are different, but the positions are equal, the position is still removed).
    void RemoveRepeatingPositionsIn3D();
    
    /// Reset the list to the empty state.
    void Reset();
    
	/// Save position list to a file.
	int SaveToPSLFile(const char *file_name);
    
    /// Get scalar tag element for given index;
    mxPointListScalarTagElement* Scalar(int i) { return &(m_scalars[i]); };
    
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif



#endif