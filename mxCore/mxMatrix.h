/*=========================================================================
 
 Program:   mipx
 Module:    mxMatrix.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#ifndef mxMatrix_DEF
	#define mxMatrix_DEF



#include "mxArray.h"



/// Matrix template

template<class T>
class mxMatrix
{
    
protected:

	/// Dimensions of 4D matrix as: [0] N of Columns, [1] N of Rows, [3] N of Slices, [3] N of Time series.
	unsigned int m_dimensions[4];

	/// Array of pointers to different 3D matrices in the 4D matrix.
    mxArray<unsigned int> m_aT;
    
    /// Array of RELATIVE pointers to different 2D matrices in the 3D matrix.
    mxArray<unsigned int> m_aS;
    
    /// Array of RELATIVE pointers to different matrix rows in the 2D matrix.
    mxArray<unsigned int> m_aR;

	/// Pointer to data elements. The assumption is that all data is created (read) in a single continuous memory space.
    /// Address of the data element with index (t,s,r,c) is: address = m_aT[t] + m_aS[s] + m_aR[r] + c.
	T* m_data_address;

 
public:
	
	/// Constructor.
	mxMatrix();
    
    /// Destructor.
	virtual ~mxMatrix();

    /// Reset the object to its intial state (just after construction).
    virtual void Reset();
    
	/// Copy from input matrix.
	virtual int Copy(mxMatrix<T> *m);

	/// Check if the matrix is empty.
	int IsEmpty();
    
    /// Attach existing (already created) data, i.e. sets the m_data_address to data_address and dimensions to given numbers.
    /// Notice: be sure that the data stored at data_address was allocated with the right data size.
    int AttachData(T *data_address, int n_of_columns, int n_of_rows, int n_of_slices, int n_of_time_series);
    
    /// Detach the array structures from the data and returns data address.
    T* DetachData();

	/// Check if the matrix is of equal dimensions with the input matrix.
	int IsEqualDimensions_4D(mxMatrix<T> &matrix);
    
    /// Check if the matrix is of equal dimensions with the input matrix for 3D matrices (differecnce in T dimension is not taken into account).
    int IsEqualDimensions_3D(mxMatrix<T> &matrix);

    /// Check if the matrix is of equal dimensions with the input matrix for 2D matrices
    /// (differecnce in T and S dimensions is not taken into account).
    int IsEqualDimensions_2D(mxMatrix<T> &matrix);

	/// Get address of (pointer to) data elements.
	T* GetDataAddress();

	/// Get matrix dimensions.
	unsigned int* GetDimensions();
	
    /// Number of time series (dimensions[3]).
    unsigned int GetNumberOfTimeSeries();
    
    /// Number of slices (dimensions[2]).
    unsigned int GetNumberOfSlices();
    
    /// Number of rows (dimensions[1]).
	unsigned int GetNumberOfRows();
    
    /// Number of columns (dimensions[0]).
	unsigned int GetNumberOfColumns();
	
    /// Total number of data elements (equal to m_dimensions[0]*m_dimensions[1]*m_dimensions[2]*m_dimensions[3]).
    unsigned int GetNumberOfDataElements();
	
    /// Set dimensions of the matrix.
    virtual int SetDimensions(int t, int s, int r, int c);
    
    /// Set dimensions of the 3D matrix.
    virtual int SetDimensions(int s, int r, int c);
    
    /// Set dimensions of the 2D matrix.
    virtual int SetDimensions(int r, int c);

    /// Set dimensions of the matrix to match input matrix.
    void SetDimensionsAs(mxMatrix<T> &matrix);

    /// Assignment operator.
    mxMatrix<T>& operator =(mxMatrix<T> &m);
    
    /// Indexing operator for accessing matrix as an array.
    T& operator()(int i);

    /// Indexing operator for 2D matrices.
	T& operator()(int r, int c);
    
    /// Indexing operator for 3D matrices.
	T& operator()(int s, int r, int c);
    
    /// Indexing operator for 3D matrices.
	T& operator()(int t, int s, int r, int c);

	/// For an existing matrix re-arrange the pointers to get a desired number of time_series 't', slices 's', rows 'r' and columns 'c'.
    /// Notice: The number of total data elements MUST REMAIN THE SAME: t*s*r*c must be equal to existing total number of elements.
	virtual int ConvertToDimensions(int t, int s, int r, int c);

	/// Fill in the whole matrix with input value.
	void FillInWith(T value);
    void FillInWith(T *value);
};




template<class T>
mxMatrix<T>::mxMatrix()
{
	m_data_address = NULL;
	m_dimensions[0] = m_dimensions[1] = m_dimensions[2] = m_dimensions[3] = 0;
}


template<class T>
mxMatrix<T>::~mxMatrix()
{
    this->mxMatrix<T>::Reset();
}


template<class T>
void mxMatrix<T>::Reset()
{
    if(this->IsEmpty()) return;
    
    delete [] m_data_address;

    m_dimensions[0] = m_dimensions[1] = m_dimensions[2] = m_dimensions[3] = 0;

    m_aR.Reset();
    m_aS.Reset();
    m_aT.Reset();
}


template<class T>
int mxMatrix<T>::Copy(mxMatrix<T> *m)
{
    if(!m) return 0;
    if(m->IsEmpty()) return 0;
    
    this->mxMatrix<T>::SetDimensions(m->GetNumberOfTimeSeries(),m->GetNumberOfSlices(),m->GetNumberOfRows(),m->GetNumberOfColumns());
    
    for(unsigned int i=0; i<this->GetNumberOfDataElements(); i++){ m_data_address[i] = m->m_data_address[i]; }
    
    return 1;
}


template<class T>
int mxMatrix<T>::IsEmpty()
{
    if(m_dimensions[0]==0 || m_dimensions[1]==0 || m_dimensions[2]==0 || m_dimensions[3]==0) return 1;
    return 0;
}


template<class T>
int mxMatrix<T>::AttachData(T *data_address, int n_of_columns, int n_of_rows, int n_of_slices, int n_of_time_series)
{
	if(n_of_columns<=0 || n_of_rows<=0 || n_of_slices<=0 || n_of_time_series<=0) return 0;
    if(data_address==NULL) return 0;
    
    this->mxMatrix<T>::Reset();

	m_dimensions[0] = n_of_columns;
	m_dimensions[1] = n_of_rows;
	m_dimensions[2] = n_of_slices;
	m_dimensions[3] = n_of_time_series;

	m_data_address = data_address;
    
	this->m_aR.SetNumberOfElements(n_of_rows);
	for(unsigned int i=0; i<m_aR.GetNumberOfElements(); i++){ m_aR[i] = i*m_dimensions[0]; }

	this->m_aS.SetNumberOfElements(n_of_slices);
	for(unsigned int i=0; i<m_aS.GetNumberOfElements(); i++){ m_aS[i] = i*m_dimensions[0]*m_dimensions[1]; }

	this->m_aT.SetNumberOfElements(n_of_time_series);
	for(unsigned int i=0; i<m_aT.GetNumberOfElements(); i++){ m_aT[i] = i*m_dimensions[0]*m_dimensions[1]*m_dimensions[2]; }
    
	return 1;
}


template <class T>
T* mxMatrix<T>::DetachData()
{
    if(this->IsEmpty()) return NULL;
    
    m_dimensions[0] = m_dimensions[1] = m_dimensions[2] = m_dimensions[3] = 0;
    
    m_aR.Reset();
    m_aS.Reset();
    m_aT.Reset();
    
    T* address = this->m_data_address;
    this->m_data_address = NULL;
    
    return address;
}


template<class T>
int mxMatrix<T>::IsEqualDimensions_4D(mxMatrix<T> &matrix)
{
	if(this->IsEqualDimensions_3D(matrix) && this->GetNumberOfTimeSeries() == matrix.GetNumberOfTimeSeries()) return 1;
    return 0;
}


template<class T>
int mxMatrix<T>::IsEqualDimensions_3D(mxMatrix<T> &matrix)
{
	if(this->GetNumberOfSlices() == matrix.GetNumberOfSlices() && this->IsEqualDimensions_2D(matrix)) return 1;
    return 0;
}


template<class T>
int mxMatrix<T>::IsEqualDimensions_2D(mxMatrix<T> &matrix)
{
	if(this->GetNumberOfRows() == matrix.GetNumberOfRows() && this->GetNumberOfColumns() == matrix.GetNumberOfColumns()) return 1;
    return 0;
}


template<class T>
T* mxMatrix<T>::GetDataAddress()
{
	return m_data_address;
}


template<class T>
unsigned int* mxMatrix<T>::GetDimensions()
{
	return m_dimensions;
}

template<class T>
unsigned int mxMatrix<T>::GetNumberOfTimeSeries()
{
	return m_dimensions[3];
}


template<class T>
unsigned int mxMatrix<T>::GetNumberOfSlices()
{
	return m_dimensions[2];
}


template<class T>
unsigned int mxMatrix<T>::GetNumberOfRows()
{
	return m_dimensions[1];
}


template<class T>
unsigned int mxMatrix<T>::GetNumberOfColumns()
{
	return m_dimensions[0];
}


template<class T>
unsigned int mxMatrix<T>::GetNumberOfDataElements()
{
	return (m_dimensions[0]*m_dimensions[1]*m_dimensions[2]*m_dimensions[3]);
}


template<class T>
int mxMatrix<T>::SetDimensions(int t, int s, int r, int c)
{
    if(c<=0 || r<=0 || s<=0 || t<=0) return 0;
    if((this->m_dimensions[0]==c) && (this->m_dimensions[1]==r) && (this->m_dimensions[2]==s)
       && (this->m_dimensions[3]==t)) return 1;
    
    this->mxMatrix<T>::Reset();
    
    m_dimensions[0] = c; m_dimensions[1] = r; m_dimensions[2] = s; m_dimensions[3] = t;
    
    m_data_address = new T[(c*r*s*t)] ;
    
    this->m_aR.SetNumberOfElements(r);
    for(unsigned int i=0; i<m_aR.GetNumberOfElements(); i++){ m_aR[i] = i*m_dimensions[0]; }
    
    this->m_aS.SetNumberOfElements(s);
    for(unsigned int i=0; i<m_aS.GetNumberOfElements(); i++){ m_aS[i] = i*m_dimensions[0]*m_dimensions[1]; }
    
    this->m_aT.SetNumberOfElements(t);
    for(unsigned int i=0; i<m_aT.GetNumberOfElements(); i++){ m_aT[i] = i*m_dimensions[0]*m_dimensions[1]*m_dimensions[2]; }
    
    return 1;
}


template<class T>
int mxMatrix<T>::SetDimensions(int s, int r, int c)
{
    return this->mxMatrix<T>::SetDimensions(1,s,r,c);
}


template<class T>
int mxMatrix<T>::SetDimensions(int r, int c)
{
    return this->mxMatrix<T>::SetDimensions(1,1,r,c);
}


template<class T>
void mxMatrix<T>::SetDimensionsAs(mxMatrix<T> &matrix)
{
	this->mxMatrix<T>::SetDimensions(matrix.GetNumberOfTimeSeries(), matrix.GetNumberOfSlices(), matrix.GetNumberOfRows(), matrix.GetNumberOfColumns());
}


template<class T>
mxMatrix<T>& mxMatrix<T>::operator =(mxMatrix<T> &m)
{
	if (&m==this) return *this;
    this->mxMatrix<T>::Copy(m);
    return *this;
}


template<class T>
T& mxMatrix<T>::operator()(int i)
{
    return (m_data_address[i]);
}


template<class T>
T& mxMatrix<T>::operator()(int r, int c)
{
	return ((*this)(0,0,r,c));
}


template<class T>
T& mxMatrix<T>::operator()(int s, int r, int c)
{
	return ((*this)(0,s,r,c));
}


template<class T>
T& mxMatrix<T>::operator()(int t, int s, int r, int c)
{
	return m_data_address[(m_aT[t]+m_aS[s]+m_aR[r]+c)];
}


template<class T>
int mxMatrix<T>::ConvertToDimensions(int t, int s, int r, int c)
{
	if(t*s*r*c != this->GetNumberOfDataElements()) return 0;
	if(t<=0 || s<=0 || r<=0 || c<=0) return 0;

	if((this->m_dimensions[0]==c) && (this->m_dimensions[1]==r) && (this->m_dimensions[2]==s)
		&& (this->m_dimensions[3]==t)) return 1;

	//Reset the arrays of indexes to data.
	this->m_aR.Reset(); this->m_aS.Reset(); this->m_aT.Reset();

	//Set new dimensions
	this->m_dimensions[0] = c; this->m_dimensions[1] = r; this->m_dimensions[2] = s; this->m_dimensions[3] = t;

	this->m_aR.SetNumberOfElements(r);
	for(unsigned int i=0; i<this->m_aR.GetNumberOfElements(); i++)
    {
        this->m_aR[i] = i*this->m_dimensions[0];
    }

	this->m_aS.SetNumberOfElements(s);
	for(unsigned int i=0; i<this->m_aS.GetNumberOfElements(); i++)
    {
        this->m_aS[i] = i*this->m_dimensions[0]*this->m_dimensions[1];
    }

	this->m_aT.SetNumberOfElements(t);
	for(unsigned int i=0; i<this->m_aT.GetNumberOfElements(); i++)
    {
        this->m_aT[i] = i*this->m_dimensions[0]*this->m_dimensions[1]*this->m_dimensions[2];
    }

	return 1;
}


template<class T>
void mxMatrix<T>::FillInWith(T value)
{
	for(int i=0; i<this->GetNumberOfDataElements(); i++){ m_data_address[i] = value; }
}


template<class T>
void mxMatrix<T>::FillInWith(T *value)
{
    for(int i=0; i<this->GetNumberOfDataElements(); i++){ m_data_address[i] = *value; }
}




#endif
