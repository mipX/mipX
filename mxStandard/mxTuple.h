/*=========================================================================
 
 Program:   mipx
 Module:    mxTuple.h

 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#ifndef mxTuple_H
	#define mxTuple_H



#include <iostream>
#include <cassert>



/// Scoped pointer template.

template <typename T1, typename T2>
class mxPair
{
    
protected:
    
    /// Element 1.
	T1 m_element1;
    
    /// Element 2.
    T2 m_element2;

public:
    
    /// Constructor.
    mxPair()
    {
    };
    
	/// Destructor.
    ~mxPair()
    {
    };
    
    /// Get element 1.
    T1& GetElement1()
    {
        return m_element1;
    };

    /// Get element 2.
    T2& GetElement2()
    {
        return m_element2;
    };

    void Set(T1 e1, T2 e2)
    {
        m_element1 = e1;
        m_element2 = e2;
    }
    
    /// Check if the pairs are equal.
    int IsEqualTo(mxPair *p)
    {
        if(this==p) return 1;
        
        if(p->m_element1==m_element1 && p->m_element2==m_element2)
        {
            return 1;
        }
        return 0;
    };
};



#endif
