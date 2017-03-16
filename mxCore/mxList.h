/*=========================================================================
 
 Program:   mipx
 Module:    mxList.h
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#ifndef mxList_DEF
	#define mxList_DEF



#include <cstdlib>
#include <sstream>




template <class T> class mxListNode;
template <class T> class mxList;



//----------------------------------------------------------------------------------------------------------------------


/// List Node class that contains data element of the containing list.

template <class T>
class mxListNode
{
    
public:
    
	/// Element contained in the node.
	T m_element;
    
    /// Address  of the previous node in the list.
    mxListNode<T> *m_previous_node;
    
    /// Address  of the next node in the list.
    mxListNode<T> *m_next_node;
	
    /// Address of the list that contains this node.
	mxList<T> *m_list;
    

	/// Constructor.
	mxListNode(mxList<T> *list);
    
    /// Destructor.
	~mxListNode();
	
	/// Reset to the initial state (just after construction).
	void Reset();

	/// Get element of the node.
	T& GetElement();

	/// Get address of node to the right.
    mxListNode<T>* GetNextNode();

	/// Get address of node to the left.
    mxListNode<T>* GetPreviousNode();
    
	/// Get address of the element of the node.
    T* GetElementAddress();
    
	/// Get address of (pointer to) the containing list.
    mxList<T>* GetList();

	/// Checks if it is the right end node of the list it belongs to.
	int IsEnd();
	
    /// Checks if it is the left end node of the list it belongs to.
    int IsBegin();

};



template<class T>
mxListNode<T>::mxListNode(mxList<T> *list)
{
    m_previous_node = m_next_node = NULL;
    m_list = list;
}


template<class T>
mxListNode<T>::~mxListNode()
{
    this->Reset();
}


template<class T>
void mxListNode<T>::Reset()
{
	m_previous_node = m_next_node = NULL;
	//Note: the address of the containing list remains the same.
}


template<class T>
T& mxListNode<T>::GetElement()
{
    return m_element;
}


template<class T>
mxListNode<T>* mxListNode<T>::GetNextNode()
{
    return m_next_node;
}


template<class T>
mxListNode<T>* mxListNode<T>::GetPreviousNode()
{
    return m_previous_node;
}


template<class T>
T* mxListNode<T>::GetElementAddress()
{
    return (&m_element);
}


template<class T>
mxList<T>* mxListNode<T>::GetList()
{
    return m_list;
}


template<class T> 
int mxListNode<T>::IsEnd()
{
	if(this == m_list->GetEndNodeAddress()) return 1;
	else return 0;
}
	
template<class T> 
int mxListNode<T>::IsBegin()
{
	if(this == m_list->GetBeginNodeAddress()) return 1;
	else return 0;
}



//----------------------------------------------------------------------------------------------------------------------


/// List Iterator class for iterating through the list. Example use:
/// mxListIterator it; for(it.SetToBegin(list); it.IsValid(); it.MoveToNext()) {...code...}

template<class T>
class mxListIterator
{
private:
    
    mxListNode<T> *m_node_address;

public:

	/// Constructor.
    mxListIterator();
    
    /// Destructor.
    ~mxListIterator();
    
    /// Reset to the initial state (just after construction).
    void Reset();

	/// Set the iterator to given node.
	void SetToNode(mxListNode<T> *node);
	
    /// Set the iterator to left end of the given list.
    void SetToBegin(mxList<T> &list);
    void SetToBegin(mxList<T> *list);

    /// Set the iterator to right end of the given list.
    void SetToEnd(mxList<T> &list);
    void SetToEnd(mxList<T> *list);

	/// Check if the iterator is valid.
	int IsValid();

	/// Move iterator in the list to left node.
    void MoveToPrevious();
	
    /// Move iterator in the list to right node.
    void MoveToNext();
	
	/// Get Node address.
    mxListNode<T>* GetNode();
    
    /// Get element address.
    T* GetElementAddress();
    
    /// Get element.
    T& GetElement();
};


template<class T>
mxListIterator<T>::mxListIterator()
{
    this->m_node_address = NULL;
}


template<class T>
mxListIterator<T>::~mxListIterator()
{
    this->Reset();
}


template<class T>
void mxListIterator<T>::Reset()
{
    this->m_node_address = NULL;
}


template<class T>
void mxListIterator<T>::SetToNode(mxListNode<T> *node)
{
    this->m_node_address = node;
}


template<class T>
void mxListIterator<T>::SetToBegin(mxList<T> &list)
{
    this->SetToNode(list.GetBeginNodeAddress());
}


template<class T>
void mxListIterator<T>::SetToBegin(mxList<T> *list)
{
    this->SetToNode(list->GetBeginNodeAddress());
}


template<class T>
void mxListIterator<T>::SetToEnd(mxList<T> &list)
{
    this->SetToNode(list.GetEndNodeAddress());
}


template<class T>
void mxListIterator<T>::SetToEnd(mxList<T> *list)
{
    this->SetToNode(list->GetEndNodeAddress());
}


template<class T> 
int mxListIterator<T>::IsValid()
{
	if(this->m_node_address==NULL) return 0;
	else return 1;
}


template<class T> 
void mxListIterator<T>::MoveToPrevious()
{
    this->m_node_address = this->m_node_address->m_previous_node;
}


template<class T> 
void mxListIterator<T>::MoveToNext()
{
    this->m_node_address = this->m_node_address->m_next_node;
}


template<class T>
mxListNode<T>* mxListIterator<T>::GetNode()
{
    return (this->m_node_address);
}


template<class T> 
T* mxListIterator<T>::GetElementAddress()
{
	if(this->m_node_address==NULL) { return NULL; }
    return (m_node_address->GetElementAddress());
}


template<class T>
T& mxListIterator<T>::GetElement()
{
    return(*(m_node_address->GetElementAddress()));
}



//-------------------------------------------------------------------------------------------------------------------------------



/// Double ended list template.

template <class T>
class mxList
{
    
private:
    
	/// Number of elements
	unsigned int m_number_of_elements;
    
	/// Pointer to left and right end nodes.
	mxListNode<T> *m_begin_node, *m_end_node;

public:

	/// Constructor.
	mxList();
    
    /// Destructor.
    ~mxList();

	/// Reset the object to intial state (just after constructor). The container node is NOT being reset!
	void Reset();

	/// Check if the list is empty. 
	int IsEmpty();

	/// Get number of elements in the list.
    unsigned int GetNumberOfElements();

	/// Get begin element
    T& GetBeginElement();
	
    /// Get begin element address.
    T* GetBeginElementAddress();
    
    /// Get begin node address.
    mxListNode<T>* GetBeginNodeAddress();
    
	/// Get end element, its pointer and node.
	T& GetEndElement();
    
    /// Get end element address.
	T* GetEndElementAddress();
    
    /// Get end node address.
    mxListNode<T>* GetEndNodeAddress();
    
	/// Delete node.
	int DeleteNode(mxListNode<T> *node);

    /// Delete begin node.
    void DeleteBegin();
	
    /// Delete end node.
    void DeleteEnd();

	/// Add given element to beginning of the list.
	void AddToBegin(T &element);
    
    /// Add given element to the end of the list.
	void AddToEnd(T &element);

    /// Add newly created element to beginning of the list.
    T* AddNewToBegin();
    
    /// Add newly created element to the end of the list.
	T* AddNewToEnd();
    
    /// Add given element before the given node.
	int AddBeforeNode(mxListNode<T> *p_node, T &element);
    
    /// Add given element after the given node.
	int AddAfterNode(mxListNode<T> *p_node, T &element);	

	/// Get Node address for a given index. Return NULL if fail.
	mxListNode<T>* GetNodeWithIndex(unsigned int index);
    
    /// Get Node address for a given element address. Return NULL if fail.
    mxListNode<T>* GetNodeWithElementAddress(T *element_address);
    
    /// For this list creates a corresponding list with pointers to elements. If empty return fail 0, non-empty success 1.
    int CreateListOfPointersToElements(mxList<T*> &output_list_of_pointers);

	/// Add an input list to the end of this list.
	void AddListToEnd(mxList<T> &list);

	/// Get the index of input node. Return: success 1, fail 0.
	int GetIndexOfNode(mxListNode<T> *node, unsigned int &output_index);

	/// Puts the 'input list' to begin of this list, after which the input_list will be empty.
	/// Performs no copying, just pointer re-arrangement.
	void MergeInputListWithThisFromBegin(mxList<T> &input_list);

    /// Puts the 'input list' to the end of this list, after which the input_list will be empty.
    /// Performs no copying, just pointer re-arrangement.
	void MergeInputListWithThisFromEnd(mxList<T> &input_list);

	/// Pointers are inverted: begin becomes end, every 'next' pointer becomes 'previous' pointer (and vice versa).
	void InvertOrder();

	/// Sort list according to the given array of indexes.
	int SortByIndexChangeOrderArray(int *array_of_index_change, unsigned int number_of_elements_in_array);

	/// Indexing operator.
	T& operator [](int r);
    
    /// Assignment operator.
	mxList<T>& operator =(mxList<T> &l);
};




namespace mxListFunctions
{
	/// Efficient way to see if element is in ASCENDING SORTED list. Goes through the list and as soon as it finds that 
	/// current node element is higher than input element it returns fail (NULL). If element is found, returns node address.
	template <typename T>
	mxListNode<T>* GetNodeWithElement_Ascending(mxList<T> &list, T &element);

	/// Efficient way to get the first element greater than input (>) in ASCENDING SORTED list. Goes through the list and 
	/// as soon as it finds greater element, returns its pointer. If there is no greater element (end of list), return NULL.
	template <typename T>
	mxListNode<T>* GetNodeWithElementGreaterThan_Ascending(mxList<T> &list, T &element);

	/// Efficient way to get the first element greater than or equal to input (>=) in ASCENDING SORTED list. Goes through the list 
	/// and as soon as it finds greater element, returns its pointer. If there is no greater/equal element (end of list), return NULL.
	template <typename T>
	mxListNode<T>* GetNodeWithElementGreaterOrEqual_Ascending(mxList<T> &list, T &element);

	/// Return pointer to node input with element. The element type must support '==' operator.
	template <typename T>
	mxListNode<T>* GetNodeWithElement(mxList<T> &list, T &element);
    
	/// Get index of element in the list. The element type must support '==' operator. If none found return 0, else 1.
	template <typename T>
	int GetIndexOfElement(mxList<T> &list, T &element, unsigned int &output_index);

	/// Check if the list has the input element. The element type must support '==' operator.
	template <typename T>
	int HasElement(mxList<T> &list, T &element);

	/// Sort elements in the list in the ascending order.
	template <typename T>
	void SortAscending(mxList<T> &list);

	/// Sort elements in the list in the descending order.
	template <typename T>
	void SortDescending(mxList<T> &list);

};



template <class T>
mxList<T>::mxList()
{
    m_begin_node = m_end_node = NULL;
    m_number_of_elements = 0;
}


template <class T>
mxList<T>::~mxList()
{
	this->Reset();
}


template<class T>
void mxList<T>::Reset()
{
	while(!this->IsEmpty()) this->DeleteEnd();
	m_begin_node = m_end_node = NULL;
	m_number_of_elements = 0;
}


template <class T>
int mxList<T>::IsEmpty()
{
    if(m_number_of_elements) return 0;
	return 1;
}


template <class T>
unsigned int mxList<T>::GetNumberOfElements()
{
    return m_number_of_elements;
}


template <class T>
T& mxList<T>::GetBeginElement()
{
	return (m_begin_node->GetElement());
}


template <class T>
T* mxList<T>::GetBeginElementAddress()
{
    return (m_begin_node->GetElementAddress());
}


template <class T>
mxListNode<T>* mxList<T>::GetBeginNodeAddress()
{
    return m_begin_node;
}


template <class T>
T& mxList<T>::GetEndElement()
{
	return (m_end_node->GetElement());
}


template <class T>
T* mxList<T>::GetEndElementAddress()
{
	return (m_end_node->GetElementAddress());
}


template <class T>
mxListNode<T>* mxList<T>::GetEndNodeAddress()
{
    return m_end_node;
}


template<class T>
int mxList<T>::DeleteNode(mxListNode<T> *node)
{
	if(node==NULL) return 0;
    if(node->GetList()!=this) return 0;

	//If the current_node is the end node
	if(node==m_end_node)
	{
		this->DeleteEnd();
		return 1;
	}

	//If the current_node is the left_end
	if(node==m_begin_node)
	{
		this->DeleteBegin();
		return 1;
	}

	node->m_next_node->m_previous_node = node->m_previous_node;
	node->m_previous_node->m_next_node = node->m_next_node;
	delete node;
	m_number_of_elements--;

	return 1;
}


template <class T>
void mxList<T>::DeleteBegin()
{
	if(this->IsEmpty()) return;

	if(m_number_of_elements == 1)
	{		
	    delete m_begin_node;
	    m_begin_node = m_end_node = NULL;
	    m_number_of_elements = 0;
	}
	else
	{
		mxListNode<T> *del;
	    del = m_begin_node;
	    m_begin_node = m_begin_node->m_next_node;
	    m_begin_node->m_previous_node = NULL;		
	    delete del;
	    m_number_of_elements--;
	}
}


template <class T>
void mxList<T>::DeleteEnd()
{
	if(this->IsEmpty()) return;

	if(m_number_of_elements == 1)
	{
		delete m_end_node;
	    m_begin_node = m_end_node = NULL;
	    m_number_of_elements = 0;
	}
	else
	{		
		mxListNode<T> *del;
	    del = m_end_node;
	    m_end_node = m_end_node->m_previous_node;
	    m_end_node->m_next_node = NULL;
	    delete del;
	    m_number_of_elements--;
	}
}


template<class T>
void mxList<T>::AddToEnd(T &element)
{
	T *nel = this->AddNewToEnd();
	(*nel) = element;
}


template<class T>
T* mxList<T>::AddNewToEnd()
{
    mxListNode<T> *nel = new mxListNode<T>(this);
	if(this->IsEmpty())
    {
		nel->m_previous_node = NULL;
		nel->m_next_node = NULL;
		m_begin_node = m_end_node = nel;
		m_number_of_elements++;
    }
    else
    {
		nel->m_next_node = NULL;
		nel->m_previous_node = m_end_node;
		m_end_node->m_next_node = nel;
		m_end_node = nel;
		m_number_of_elements++;
    }
	return (nel->GetElementAddress());
}


template<class T>
void mxList<T>::AddToBegin(T &element)
{
	T *nel = this->AddNewToBegin();
	(*nel) = element;
}


template<class T>
T* mxList<T>::AddNewToBegin()
{
    mxListNode<T> *nel = new mxListNode<T>(this);
	if(this->IsEmpty())
    {
		nel->m_previous_node = NULL;
		nel->m_next_node = NULL;
		m_begin_node = m_end_node = nel;
		m_number_of_elements++;
    }
    else
    {
		nel->m_previous_node = NULL;
		nel->m_next_node = m_begin_node;
		m_begin_node->m_previous_node = nel;
		m_begin_node = nel;
		m_number_of_elements++;
    }
	return (nel->GetElementAddress());
}


template<class T>
int mxList<T>::AddBeforeNode(mxListNode<T> *node, T &element)
{
	if(!node) return 0;
	if(this->IsEmpty()) return 0;

	if(node==m_begin_node){ this->AddToBegin(element); }
	else 
	{
		mxListNode<T> *nel = new mxListNode<T>(this);
		nel->GetElement() = element;

		nel->m_previous_node = node->m_previous_node;
		nel->m_next_node = node;
		node->m_previous_node->m_next_node = nel;
		node->m_previous_node = nel;
		m_number_of_elements++;
	}
	return 1;
}


template<class T>
int mxList<T>::AddAfterNode(mxListNode<T> *node, T &element)
{
	if(!node) return 0;
	if(this->IsEmpty()) return 0;

	if(node==m_end_node){ this->AddToEnd(element); }
	else 
	{
		mxListNode<T> *nel = new mxListNode<T>(this);
		nel->GetElement() = element;
		nel->m_next_node = node->m_next_node;
		nel->m_previous_node = node;
		node->m_next_node->m_previous_node = nel;
		node->m_next_node = nel;
		m_number_of_elements++;
	}
	return 1;
}


template<class T>
mxListNode<T>* mxList<T>::GetNodeWithIndex(unsigned int index)
{
	if(index>=m_number_of_elements) return NULL;
	mxListNode<T> *node = m_begin_node;
	for(unsigned int i=0; i<index; i++) node = node->m_next_node;
	return node;
}


template<class T>
mxListNode<T>* mxList<T>::GetNodeWithElementAddress(T *element_address)
{
	if(this->IsEmpty()) return NULL;
	mxListIterator<T> it;
	for(it.SetToBegin(this); it.IsValid(); it.MoveToNext())
	{
		if( it.GetElementAddress() == element_address ) 
		{
			return it.GetNode();
		}
	}
	return NULL;
}


template<class T>
int mxList<T>::CreateListOfPointersToElements(mxList<T*> &output_list_of_pointers)
{
    if(this->IsEmpty()) return 0;
    output_list_of_pointers.Reset();
    
    mxListIterator<T> it;
    for(it.SetToBegin(this); it.IsValid(); it.MoveToNext())
    {
        T *p = it.GetElementAddress();
        output_list_of_pointers.AddToEnd(p);
    }
    
    return 1;
}


template<class T>
int mxList<T>::GetIndexOfNode(mxListNode<T> *node, unsigned int &output_index)
{
	if(this->IsEmpty()) return 0;
	mxListIterator<T> it;
	output_index = 0;
	for(it.SetToBegin(this); it.IsValid(); it.MoveToNext())
	{
		if( it.GetNode()==node ) return 1;
		output_index++;
	}
	return 0;
}


template<class T>
void mxList<T>::AddListToEnd(mxList<T> &list)
{
	if(list.IsEmpty()) return;
	mxListIterator<T> it;
	for(it.SetToBegin(list); it.IsValid(); it.MoveToNext())
	{
		this->AddToEnd(it.GetElement());
	}
}


template<class T>
void mxList<T>::MergeInputListWithThisFromBegin(mxList<T> &input_list)
{
	//If the input list is empty do nothing
	if(input_list.IsEmpty()) return;

	// Set the pointer of nodes in the input list to point to this list 
	mxListIterator<T> it;
	for(it.SetToBegin(input_list); it.IsValid(); it.MoveToNext())
	{ it.GetNode()->m_list = this; }

	//If this list is empty
	if(this->IsEmpty())
	{
		//Set the pointers of this list to point to the data of input_list
		m_end_node = input_list.m_end_node;
		m_begin_node = input_list.m_begin_node;
		m_number_of_elements = input_list.m_number_of_elements;
	}
	//If this list is not empty
	else
	{
		//Set the pointers of this list to point to the data of input_list
		m_begin_node->m_previous_node = input_list.m_end_node;
		input_list.m_end_node->m_next_node = m_begin_node;
		m_begin_node = input_list.m_begin_node;
		m_number_of_elements += input_list.m_number_of_elements;//Set number of elements (it is sum of number of elements of both lists)
	}

	//Detach the input list from its data
	input_list.m_begin_node = NULL;
	input_list.m_end_node = NULL;
	input_list.m_number_of_elements = 0;
}


template<class T>
void mxList<T>::MergeInputListWithThisFromEnd(mxList<T> &input_list)
{
	// If the input list is empty do nothing
	if(input_list.IsEmpty()) return;

	// Set the pointer of nodes in the input list to point to this list 
	mxListIterator<T> it;
	for(it.SetToBegin(input_list); it.IsValid(); it.MoveToNext())
	{ it.GetNode()->m_list = this; }

	// If this list is empty
	if(this->IsEmpty())
	{
		//Set the pointers of this list to point to the data of input_list
		m_end_node = input_list.m_end_node;
		m_begin_node = input_list.m_begin_node;
		m_number_of_elements = input_list.m_number_of_elements;
	}
	// If this list is not empty
	else
	{
		//Set the pointers of this list to point to the data of input_list
		m_end_node->m_next_node = input_list.m_begin_node;
		input_list.m_begin_node->m_previous_node = m_end_node;
		m_end_node = input_list.m_end_node;
		m_number_of_elements += input_list.m_number_of_elements;//Set number of elements (it is sum of number of elements of both lists)
	}

	// Detach the input list from its data
	input_list.m_begin_node = NULL;
	input_list.m_end_node = NULL;
	input_list.m_number_of_elements = 0;
}


template<class T>
void mxList<T>::InvertOrder()
{
	if(this->IsEmpty()) return;

	mxListNode<T> *temp, *temp2, *current;

	// Invert all left and right pointers in nodes
	current = this->GetBeginNodeAddress();
	while(current!=m_end_node)
	{
		temp = current;
		current = current->m_next_node;
		temp2 = temp->m_previous_node;
		temp->m_previous_node = temp->m_next_node;
		temp->m_next_node = temp2;
	}
	// At this point we have to do it also for the last node
	temp2 = current->m_previous_node;
	current->m_previous_node = current->m_next_node;
	current->m_next_node = temp2;

	// Invert left end and right end
	temp = m_begin_node;
	m_begin_node = m_end_node;
	m_end_node = temp;
}


template <class T>
int mxList<T>::SortByIndexChangeOrderArray(int *array_of_index_change, unsigned int number_of_elements_in_array)
{
	if(number_of_elements_in_array != this->GetNumberOfElements()) return 0;
    if(this->IsEmpty()) return 0;
    if(!array_of_index_change) return 0;

    mxList<T> temp;
    
    for(unsigned int i=0; i<number_of_elements_in_array; i++)
    {
        temp.AddToEnd((*this)[(array_of_index_change[i])]);
    }
    
    (*this) = temp;

    return 1;
}


template <class T>
T& mxList<T>::operator [](int r)
{ 
	mxListNode<T> *node;
	node = m_begin_node;
    for(int i=0; i<r; i++) node = node->m_next_node;
	return (node->GetElement());
}


template <class T>
mxList<T>& mxList<T>::operator =(mxList<T> &l)
{
    if(&l == this) return *this;
    
	this->Reset();
    
	mxListIterator<T> it;
	for(it.SetToBegin(l); it.IsValid(); it.MoveToNext()) { this->AddToEnd(it.GetElement()); }
    return *this;
}



//---------------------------------------------------------------------------------------------------------------------



template<class T>
mxListNode<T>* mxListFunctions::GetNodeWithElement_Ascending(mxList<T> &list, T &element)
{
	if(list.IsEmpty()) return NULL;
	mxListIterator<T> it;
	for(it.SetToBegin(list); it.IsValid(); it.MoveToNext())
	{
		if( it.GetElement() == element ) 
		{
			return it.GetNode();
		}
		else
		{
			if(it.GetElement() > element) return NULL;
		}
	}
	return NULL;
}


template<class T>
mxListNode<T>* mxListFunctions::GetNodeWithElementGreaterThan_Ascending(mxList<T> &list, T &element)
{
	if(list.IsEmpty()) return NULL;
	mxListIterator<T> it;
	for(it.SetToBegin(list); it.IsValid(); it.MoveToNext())
	{
		if( it.GetElement() > element ) { return it.GetNode(); }
	}
	return NULL;
}


template<class T>
mxListNode<T>* mxListFunctions::GetNodeWithElementGreaterOrEqual_Ascending(mxList<T> &list, T &element)
{
	if(list.IsEmpty()) return NULL;
	mxListIterator<T> it;
	for(it.SetToBegin(list); it.IsValid(); it.MoveToNext())
	{
		if( it.GetElement() >= element ) { return it.GetNode(); }
	}
	return NULL;
}


template<class T>
mxListNode<T>* mxListFunctions::GetNodeWithElement(mxList<T> &list, T &element)
{
	if(list.IsEmpty()) return NULL;
	mxListIterator<T> it;
	for(it.SetToBegin(list); it.IsValid(); it.MoveToNext())
	{
		if( it.GetElement() == element ) 
		{
			return it.GetNode();
		}
	}
	return NULL;
}


template<class T>
int mxListFunctions::GetIndexOfElement(mxList<T> &list, T &element, unsigned int &output_index)
{
	if(list.IsEmpty()) return 0;
	mxListIterator<T> it;
	output_index = 0;
	for(it.SetToBegin(list); it.IsValid(); it.MoveToNext())
	{
		if( it.GetElement() == element ) return 1;
		output_index++;
	}
	return 0;
}



template<class T>
int mxListFunctions::HasElement(mxList<T> &list, T &element)
{
	if(list.IsEmpty()) return 0;
	mxListIterator<T> it;
	for(it.SetToBegin(list); it.IsValid(); it.MoveToNext())
	{
		if( (*(it.GetElementAddress()))==element ) return 1;
	}
	return 0;
}


template<class T>
void mxListFunctions::SortAscending(mxList<T> &list)
{
	if(list.GetNumberOfElements()<=1) return;

    int is_change_made = 1;

	mxListNode<T> *node1, *node2;
	T temp;

	// Loop while there are changes in order
	while(is_change_made)
	{
		// Reset indicator, initialize pointers
		is_change_made = 0;
		node1 = list.GetBeginNodeAddress();
		node2 = list.GetBeginNodeAddress()->m_next_node;

		for(unsigned int i=0; i<list.GetNumberOfElements()-1; i++)
        {
			if( (node1->GetElement()) > (node2->GetElement()) )
			{
				is_change_made = 1;
				temp = node2->GetElement();
				node2->GetElement() = node1->GetElement();
				node1->GetElement() = temp;
			}
			node1 = node1->m_next_node;
			node2 = node2->m_next_node;
		}
	}
}


template<class T>
void mxListFunctions::SortDescending(mxList<T> &list)
{
	if(list.GetNumberOfElements()<=1) return;

    int is_change_made = 1;

	mxListNode<T> *node1, *node2;
	T temp;

	// Loop while there are changes in order
	while(is_change_made)
	{
		// Reset indicator, initialize pointers
		is_change_made = 0;
		node1 = list.GetBeginNodeAddress();
		node2 = list.GetBeginNodeAddress()->m_next_node;

		for(unsigned int i=0; i<list.GetNumberOfElements()-1; i++)
        {
			if( (node1->GetElement()) < (node2->GetElement()) )
			{
				is_change_made = 1;
				temp = node2->GetElement();
				node2->GetElement() = node1->GetElement();
				node1->GetElement() = temp;
			}
			node1 = node1->m_next_node;
			node2 = node2->m_next_node;
		}
	}
}


#endif