/*=========================================================================
 
 Program:   mipx
 Module:    mxArray.h

 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#ifndef mxArray_H
	#define mxArray_H



#include <iostream>
#include <cassert>



/// Array template.

template <typename T> 
class mxArray
{
    
protected:
    
    /// Number of elements.
	unsigned int m_number_of_elements;
    
    /// Pointer to elements.
	T *m_data_address;

public:

	/// Constructor.
	mxArray();
    
	/// Destructor.
	~mxArray();
    
    /// Reset the array to initial state (just after creation).
    void Reset();

    /// Check if the array is empty.
	int IsEmpty();
    
    /// Get number of elements in the array.
    unsigned int GetNumberOfElements();
    
    /// Set the number of elements (size) of the array (with possible initialization from existing array). Return: success 1, fail 0.
    int SetNumberOfElements(unsigned int r);
    
    /// Get address/pointer to data.
    T* GetDataAddress();
	
	/// Attach existing (already created) data, i.e. sets the m_data_address to data_address and m_number_of_elements to number_of_data_elements.
    /// Notice: be sure that the data stored at data_address was allocated with the right data size.
	int AttachData(T *data_address, unsigned int number_of_data_elements);

	/// Detach the data in order to pass it to another object, i.e. sets the m_data_address to data to NULL and m_number_of_elements to zero without releasing memory.
    void DetachData();
    
	/// Fills in an array of set size with the input value.
	int FillInWith(T initial_value);
	int FillInWith(T *initial_value);

    /// Copy from input array. Elements can be copied from given start index and up to the given end index.
    /// Notice: this method will erase all elements existing in this array before copying.
    int CopyFrom(mxArray<T> &array_to_copy, unsigned int start_index_of_input_array_included, unsigned int end_index_of_input_array_included);
    int CopyFrom(mxArray<T> &array_to_copy, unsigned int start_index_of_input_array_included);
    int CopyFrom(mxArray<T> &array_to_copy);
    
    /// For this array of a preset size (existing elements in the array), overwrite the values of elements by values from input array
    /// starting from the specified index in this array.
    int OverWriteValues(mxArray<T> &array_to_copy_values_from, unsigned int start_index_included = 0);

	/// Assignment operator. 
	mxArray<T>& operator =(mxArray<T> &m);
    
    /// Indexing operator.
    T& operator [](unsigned int r);
	
    /// Print the array elements.
    void Print(std::ostream &o);
    
};



//----------------------------------------------------------------------------------------------------------------------


/// Array functions name space (contains functions to search and sort elements)
/// Notice: this will work only if the elements are comparable (operators ==, <, >, <=, >= must be defined on the types of elements).

namespace mxArrayFunctions
{
    /// Swap (exchange) values (v1 takes value of v2 and v2 takes value of v1).
    template <typename T>
    void SwapValues(T &v1, T &v2);
    
    /// Load an array of doubles from string of double values.
    template <typename T>
    int LoadFromStringOfDoubleValues(mxArray<T> &a, char *array_string, unsigned int number_of_characters_in_array_string);
    
    /// Sort elements in the ascending order.
    template <typename T>
    int SortAscending(mxArray<T> &a);
    
    /// Sort elements in the ascending order. Index change is recorded to 'order_of_index_change'.
    template <typename T>
    int SortAscending(mxArray<T> &a, mxArray<int> &order_of_index_change);
    
    /// Sort elements in the descending order.
    template <typename T>
    int SortDescending(mxArray<T> &a);
    
    /// Sort elements in the descending order. Index change is recorded to 'order_of_index_change'.
    template <typename T>
    int SortDescending(mxArray<T> &a, mxArray<int> &order_of_index_change);
    
    /// Get minimum and maximum values in the array and their indexes.
    template <typename T>
    int MinAndMax(mxArray<T> &a, T &min, T &max, int &index_of_min, int &index_of_max);
    
    /// Get the index of the given input element. If none found, returns -1.
    template <typename T>
    int IndexOfElement(mxArray<T> &a, T &element);

}



//----------------------------------------------------------------------------------------------------------------------



template <class T>
mxArray<T>::mxArray()
{
    this->m_number_of_elements=0;
    this->m_data_address=NULL;
}


template <class T>
mxArray<T>::~mxArray()
{
    this->Reset();
}


template <class T>
void mxArray<T>::Reset()
{
    delete [] this->m_data_address;
    this->m_number_of_elements = 0;
    this->m_data_address = NULL;
}


template <class T>
int mxArray<T>::IsEmpty()
{
    if(this->m_number_of_elements) { return 0; }
    return 1;
}


template <class T>
unsigned int mxArray<T>::GetNumberOfElements()
{
    return this->m_number_of_elements;
}


template <class T>
int mxArray<T>::SetNumberOfElements(unsigned int r)
{
    if(!this->IsEmpty()) Reset();
    
    this->m_number_of_elements = r;
    if(r>0) { this->m_data_address = new T [this->m_number_of_elements]; }
    else { this->m_data_address = NULL; }
    
    return 1;
}


template <class T>
T* mxArray<T>::GetDataAddress()
{
    return this->m_data_address;
}


template <class T>
int mxArray<T>::AttachData(T *data_address, unsigned int number_of_data_elements)
{
	if(data_address == NULL) return 0;
    
	this->Reset();
	
    this->m_number_of_elements = number_of_data_elements;
	this->m_data_address = data_address;
    
	return 1;
}


template <class T>
void mxArray<T>::DetachData()
{
    this->m_data_address=NULL;
    this->m_number_of_elements=0;
}


template <class T>
int mxArray<T>::FillInWith(T initial_value)
{
    for(unsigned int i=0; i<this->GetNumberOfElements(); i++) { this->m_data_address[i] = initial_value; }
    return 1;
}


template <class T>
int mxArray<T>::FillInWith(T *initial_value)
{
    if(!initial_value) return 0;
    for(unsigned int i=0; i<this->GetNumberOfElements(); i++) { this->m_data_address[i] = *initial_value; }
    return 1;
}


template <class T>
int mxArray<T>::CopyFrom(mxArray<T> &array_to_copy, unsigned int start_index_of_input_array_included, unsigned int end_index_of_input_array_included)
{
	if(start_index_of_input_array_included >= array_to_copy.GetNumberOfElements()) return 0;
    if(end_index_of_input_array_included >= array_to_copy.GetNumberOfElements()) return 0;
    if(end_index_of_input_array_included > start_index_of_input_array_included) return 0;
	if(array_to_copy.IsEmpty()) return 0;
    
    this->Reset();
    this->m_number_of_elements = end_index_of_input_array_included - start_index_of_input_array_included + 1;
    this->m_data_address = new T [this->m_number_of_elements];
    
    unsigned int i1 = 0, i2 = start_index_of_input_array_included;
	for(i1=0, i2=start_index_of_input_array_included; i1<this->GetNumberOfElements() && i2<=end_index_of_input_array_included; i1++, i2++)
	{
 		this->m_data_address[i1] = array_to_copy[i2];
	}
	return 1;
}


template <class T>
int mxArray<T>::CopyFrom(mxArray<T> &array_to_copy, unsigned int start_index_of_input_array_included)
{
    return this->CopyFrom(array_to_copy, start_index_of_input_array_included, array_to_copy.GetNumberOfElements());
}


template <class T>
int mxArray<T>::CopyFrom(mxArray<T> &array_to_copy)
{
    (*this) = array_to_copy;
    return 1;
}


template <class T>
int mxArray<T>::OverWriteValues(mxArray<T> &array_to_copy_values_from, unsigned int start_index_included)
{
    if(this->IsEmpty()) return 0;
    if(start_index_included>=this->GetNumberOfElements()) return 0;
    
    if(array_to_copy_values_from.IsEmpty()) return 1;
    
    unsigned int i1 = start_index_included, i2 = 0;
    for(i1=start_index_included, i2=0; i1<this->GetNumberOfElements() && i2<array_to_copy_values_from.GetNumberOfElements(); i1++, i2++)
    {
        this->m_data_address[i1] = array_to_copy_values_from[i2];
    }
    return 1;
}


template <class T>
mxArray<T>& mxArray<T>::operator =(mxArray<T> &a)
{
    if (&a==this) return *this;
    
    this->SetNumberOfElements(a.GetNumberOfElements());
    
    for(unsigned int i=0; i<this->GetNumberOfElements(); i++) this->m_data_address[i] = a[i];
    return *this;
}


template <class T>
T& mxArray<T>::operator [](unsigned int r)
{
    assert(r < this->GetNumberOfElements());
    return this->m_data_address[r];
}


template <class T>
void mxArray<T>::Print(std::ostream &o)
{
    for(unsigned int i=0; i<this->GetNumberOfElements(); i++) { o<<"["<<i<<"]="<<this->m_data_address[i]<<"  "; }
}



//----------------------------------------------------------------------------------------------------------------------



template <class T>
int mxArrayFunctions::LoadFromStringOfDoubleValues(mxArray<T> &a, char *array_string, unsigned int number_of_characters_in_array_string)
{
    if(!array_string) return 0;
    if(number_of_characters_in_array_string<3) return 0;

    // limit the use of the loading function to data of the same size as double.
    if(sizeof(T)!=sizeof(double)) return 0;
    
    // Find '['
    int i=0;
    for(i=0; array_string[i]!='[' && i<number_of_characters_in_array_string; i++)
    {
        if(i==number_of_characters_in_array_string-1)
        {
            std::cout<<"mxArrayFunctions::LoadFromStringOfDoubleValues(): Error reading string, could not find '[' character!"<<std::endl;
            return 0;
        }
    }
    i++;
    
    // Find ']'
    int i2 = i;
    for(i2=i; array_string[i2]!=']' && i2<number_of_characters_in_array_string; i2++)
    {
        if(i2==number_of_characters_in_array_string-1)
        {
            std::cout<<"mxArrayFunctions::LoadFromStringOfDoubleValues(): Error reading string, could not find ']' character!"<<std::endl;
            return 0;
        }
    }
    i2--;
    
    // Count the number of elements in the array - this is done by calculating the number of commas ","
    // Also checks the correctness of characters in between two brackets: "[" and "]"
    int number_of_elements_of_array = 1;
    for(int c=i; c<=i2; c++)
    {
        if(array_string[c]==',') number_of_elements_of_array++;
        
        // check if characters belong to a real number
        if(array_string[c]!=' ' && array_string[c]!=',' && array_string[c]!='.' && array_string[c]!='-' && array_string[c]!='+' && (!(array_string[c]>='0' && array_string[c]<='9')))
        {
            std::cout<<"mxArrayFunctions::LoadFromStringOfDoubleValues(): Error reading string, incorrect charcters used for a double number!"<<std::endl;
            return 0;
        }
    }
    
    a.Reset();
    a.SetNumberOfElements(number_of_elements_of_array);
    
    
    double dec = 1;
    double value = 0;
    int number_of_added_elements = 0;
    int is_dot_found = 0;
    int is_sign_found = 0;
    for(int r=i2 ; r>=i-1; r--)
    {
        if(array_string[r]==',' || array_string[r]=='[')
        {
            // Write the calculated value in the array
            a[a.GetNumberOfElements()-1-number_of_added_elements] = value;
            number_of_added_elements++;
            
            // Reset 'dec' and 'value' for a new number
            dec = 1;
            value = 0;
            
            // Set the dot indicator to 0
            is_dot_found = 0;
            
            // Set the sign indicator to 0
            is_sign_found = 0;
        }
        // Since we search backwards, the numbers and dot can be taken into account only if the sign has not yet been found!
        if(!is_sign_found)
        {
            // If MINUS sign is found, make negative value, set the indicator of found sign
            if(array_string[r]=='-')
            {
                value = -value;
                is_sign_found = 1;
            }
            // If PLUS sign is found, just set the indicator of found sign
            if(array_string[r]=='+') is_sign_found = 1;
            
            // If a number is found
            if(array_string[r]>='0' && array_string[r]<='9')
            {
                value += ((int)(array_string[r] - '0'))*dec;
                dec = dec * 10;
            }
            if(array_string[r]=='.')
            {
                // If this is the first dot
                if(!is_dot_found)
                {
                    value = value/dec;
                    is_dot_found = 1;
                    dec = 1;
                }
                // If this is the second dot, the error ocurred, exit with 0
                else
                {
                    std::cout<<"mxArrayFunctions::LoadFromStringOfDoubleValues(): Error reading string, incorrect charcter '.' placement!"<<std::endl;
                    return 0;
                }
            }
        }
        else
        {
            // If the sign is found and the current character is not ',' or '[', report an error
            if(array_string[r]!=',' && array_string[r]!='[' && array_string[r]!=' ')
            {
                std::cout<<"mxArrayFunctions::LoadFromStringOfDoubleValues(): Error reading string, incorrect charcter before sign caharcter found!"<<std::endl;
                return 0;
            }
        }
    }
    return 1;
}



template <class T>
void mxArrayFunctions::SwapValues(T &v1, T &v2)
{
    T t;
    t = v1;
    v1 = v2;
    v2 = t;
}


template <class T>
int mxArrayFunctions::SortAscending(mxArray<T> &a)
{
	if(a.IsEmpty()) return 0;

	// indicator if any changes were made in the while loop
	int is_changed = 1;
    
    while(is_changed)
	{
        is_changed = 0;
		for(unsigned int i=0; i<a.GetNumberOfElements()-1; i++)
		{
			if( a[i] > a[i+1])
			{
                mxArrayFunctions::SwapValues(a[i],a[i+1]);
                is_changed = 1;
			}
		}
	}
    return 1;
}


template <class T>
int mxArrayFunctions::SortAscending(mxArray<T> &a, mxArray<int> &order_of_index_change)
{
	if(a.IsEmpty()) return 0;

    // Elements of this array are indexes
	order_of_index_change.SetNumberOfElements(a.GetNumberOfElements());
    for(unsigned int i=0; i<order_of_index_change.GetNumberOfElements(); i++) { order_of_index_change[i] = i; }
    
	// indicator if any changes were made in the while loop
	int is_changed = 1;
    
    while(is_changed)
	{
        is_changed = 0;
		for(unsigned int i=0; i<a.GetNumberOfElements()-1; i++)
		{
			if( a[i] > a[i+1])
			{
				mxArrayFunctions::SwapValues(a[i],a[i+1]);
                mxArrayFunctions::SwapValues(order_of_index_change[i],order_of_index_change[i+1]);
                is_changed = 1;
			}
		}
	}
    return 1;
}


template <class T>
int mxArrayFunctions::SortDescending(mxArray<T> &a)
{
	if(a.IsEmpty()) return 0;

	//indicator if any changes were made in the while loop
	int is_changed = 1;
	
    while(is_changed)
	{
        is_changed = 0;
		for(unsigned int i=0; i<a.GetNumberOfElements()-1; i++)
		{
			if( a[i] < a[i+1])
			{
                mxArrayFunctions::SwapValues(a[i],a[i+1]);
                is_changed = 1;
			}
		}
	}
    return 1;
}


template <class T>
int mxArrayFunctions::SortDescending(mxArray<T> &a, mxArray<int> &order_of_index_change)
{
	if(a.IsEmpty()) return 0;

    // Elements of this array are indexes
    order_of_index_change.SetNumberOfElements(a.GetNumberOfElements());
    for(unsigned int i=0; i<order_of_index_change.GetNumberOfElements(); i++) { order_of_index_change[i] = i; }
    
	//indicator if any changes were made in the while loop
	int is_changed = 1;
	
    while(is_changed)
	{
        is_changed = 0;
		for(unsigned int i=0; i<a.GetNumberOfElements()-1; i++)
		{
			if( a[i] < a[i+1])
			{
                mxArrayFunctions::SwapValues(a[i],a[i+1]);
                mxArrayFunctions::SwapValues(order_of_index_change[i],order_of_index_change[i+1]);
                is_changed = 1;
			}
		}
	}
    return 1;
}


template<class T>
int mxArrayFunctions::MinAndMax(mxArray<T> &a, T &min, T &max, int &index_of_min, int &index_of_max)
{
    if(a.IsEmpty()) return 0;
    
    min = max = a[0];
    index_of_min = index_of_max = 0;
    
    for(unsigned int i=1; i<a.GetNumberOfElements(); i++)
    {
        if(a[i]>(max)) { max = a[i]; index_of_max = i; }
        if(a[i]<(min)) { min = a[i]; index_of_min = i; }
    }
    return 1;
}


template<class T>
int mxArrayFunctions::IndexOfElement(mxArray<T> &a, T &element)
{
    for(unsigned int i=0; i<a.GetNumberOfElements(); i++) { if(a[i]==element) return i; }
    return -1;
}



#endif