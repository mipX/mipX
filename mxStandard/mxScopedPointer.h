/*=========================================================================
 
 Program:   mipx
 Module:    mxScopedPointer.h

 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#ifndef mxScopedPointer_H
	#define mxScopedPointer_H



#include <iostream>
#include <cassert>



/// Scoped pointer template.

template <typename T> 
class mxScopedPointer
{
    
protected:
    
    /// Pointer to element.
	T *m_pointer;

public:
    
	/// Constructor.
    mxScopedPointer(T* pointer) : m_pointer(pointer)
    {
    };
    
    /// Constructor.
    mxScopedPointer()
    {
        m_pointer = NULL;
    };
    
	/// Destructor.
    ~mxScopedPointer()
    {
        delete m_pointer;
    };
    
    /// Conversion operator.
    operator T* ()
    {
        return m_pointer;
    };
    
    /// Indirection ("object pointed to by ") operator.
    T& operator *()
    {
        return *m_pointer;
    };
    
    /// Arrow operator.
    T* operator ->()
    {
        return m_pointer;
    };
    
    /// Assignment operator.
    mxScopedPointer<T>& operator =(T *pointer)
    {
        m_pointer = pointer;
        return *this;
    };
    
    /// Check if the addresses are equal.
    void Attach(T *pointer)
    {
        m_pointer=pointer;
    };
    
    /// Check if the addresses are equal.
    int IsEqualTo(T *pointer)
    {
        if(m_pointer==pointer) return 1;
        return 0;
    };

    /// Check if the pointer is NULL.
	int IsNULL()
    {
        if(m_pointer==NULL) return 1;
        return 0;
    };
    
    /// Get address/pointer.
    T* GetPointer()
    {
        return m_pointer;
    };
};



#endif
