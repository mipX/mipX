/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeleton.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxSkeleton.h"



mxSkeletonLink::mxSkeletonLink()
{
	m_skeleton = NULL;
	m_skeleton3D = NULL;
	m_node1 = NULL;
    m_node2 = NULL;
	m_cost=0; 
}


void mxSkeletonLink::Reset()
{
	m_skeleton = NULL;
	m_skeleton3D = NULL;
	m_node1 = NULL;
    m_node2 = NULL;
	m_cost=0; 
	m_positions.Reset();
	m_skeletonization_voxels.Reset();
}


mxSkeletonLink::~mxSkeletonLink()
{
	this->Reset();
}


void mxSkeletonLink::CorrectLink()
{
    // Iteratively delete positions while they are the same as node1.
    {
        int is_position_deleted = 1;
        while((!this->m_positions.IsEmpty()) && is_position_deleted)
        {
            is_position_deleted = 0;
            
            mxPoint *pos = this->GetNode_1_Position();
            if(this->m_positions.GetBeginElement().IsEqualPositionIn_3D(*pos))
            {
                this->m_positions.DeleteBegin();
                is_position_deleted = 1;
            }
        }
    }
    
    // Iteratively delete positions while they are the same as node2.
    {
        int is_position_deleted = 1;
        while((!this->m_positions.IsEmpty()) && is_position_deleted)
        {
            is_position_deleted = 0;
            
            mxPoint *pos = this->GetNode_2_Position();
            if(this->m_positions.GetEndElement().IsEqualPositionIn_3D(*pos))
            {
                this->m_positions.DeleteEnd();
                is_position_deleted = 1;
            }
        }
    }
    
    //Delete the neighboring positions in the list that have the same coordinates
    mxList< mxPoint > l;
    mxPoint *pp = &(this->m_node1->GetPosition());
    while(!this->m_positions.IsEmpty())
    {
        mxPoint pos = this->m_positions.GetBeginElement();
        
        // If the recorded position is different than the current one...
        if(!pp->IsEqualPositionIn_3D(pos))
        {
            // ...record it to list l.
            l.AddToEnd(pos);
            pp = l.GetEndElementAddress();
        }
        this->m_positions.DeleteBegin();
    }
    this->m_positions = l;
}


mxPoint* mxSkeletonLink::GetNode_1_Position()
{
    if(m_node1) return &(m_node1->GetPosition());
    return NULL;
}


mxPoint* mxSkeletonLink::GetNode_2_Position()
{
    if(m_node2) return &(m_node2->GetPosition());
    return NULL;
}


mxSkeletonNode* mxSkeletonLink::GetNodeCloserToInputPosition(mxPoint &input_position)
{
    mxPoint *p1 = this->GetNode_1_Position();
    mxPoint *p2 = this->GetNode_2_Position();
    if(!p1 || !p2) return NULL;
    double d1 = p1->DistanceEuclidean(input_position);
    double d2 = p2->DistanceEuclidean(input_position);
    if(d1<d2) return m_node1;
    return m_node2;
}


void mxSkeletonLink::GetAdjacentLinks(mxList< mxSkeletonLink* > &adjacent_links)
{
    adjacent_links.Reset();
    
    mxListIterator< mxSkeletonLink* > it;
    for(it.SetToBegin(m_node1->GetLinksList()); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()!=this) adjacent_links.AddToEnd(it.GetElement());
    }
    for(it.SetToBegin(m_node2->GetLinksList()); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElement()!=this) adjacent_links.AddToEnd(it.GetElement());
    }
}


int mxSkeletonLink::GetScalarMinAndMaxValue(unsigned int scalar_index, double &output_min, double &output_max)
{
    if(m_positions.IsEmpty()) return 0;
    
    output_min = -1;
    output_max = -1;
    mxListIterator< mxPoint > it;
    for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
    {
        mxPoint *p = it.GetElementAddress();
        if(p->V(scalar_index) < output_min || output_min < 0)
        {
            output_min = p->V(scalar_index);
        }
        if(p->V(scalar_index) > output_max)
        {
            output_max = p->V(scalar_index);
        }
    }
    return 1;
}


int mxSkeletonLink::GetScalarAverageValue(unsigned int scalar_index, double &output_average)
{
    if(m_positions.IsEmpty()) return 0;
    
    double sum = 0, counter = 0;
    mxListIterator< mxPoint > it;
    for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
    {
        sum += it.GetElementAddress()->V(scalar_index);
        counter += 1;
    }
    output_average = sum / counter;
    return 1;
}


int mxSkeletonLink::GetArrayOfPositionPointersIncludingNodes(mxArray< mxPoint* > &output_array_of_position_pointers)
{
    mxSkeletonNode *psn1 = this->GetNode_1();
    mxSkeletonNode *psn2 = this->GetNode_2();
    if(!psn1 || !psn2) return 0;
    
    output_array_of_position_pointers.SetNumberOfElements(this->m_positions.GetNumberOfElements()+2);
    
    output_array_of_position_pointers[0] = &(psn1->GetPosition());
    
    mxListIterator< mxPoint > it;
    int i=1;
    for(it.SetToBegin(m_positions), i=1; it.IsValid() && i<output_array_of_position_pointers.GetNumberOfElements()-1; it.MoveToNext(), i++ )
    {
        mxPoint *pp = it.GetElementAddress();
        output_array_of_position_pointers[i] = pp;
    }
    
    output_array_of_position_pointers[output_array_of_position_pointers.GetNumberOfElements()-1] = &(psn2->GetPosition());
    return 1;
}


int mxSkeletonLink::GetArrayOfWorldPositionsIncludingNodes(mxArray< mxPoint > &output_array_of_world_positions)
{
    mxSkeletonNode *psn1 = this->GetNode_1();
    mxSkeletonNode *psn2 = this->GetNode_2();
    if(!psn1 || !psn2) return 0;
    
    double spacing_s = this->GetSkeleton()->GetSpacing_S();
    double spacing_r = this->GetSkeleton()->GetSpacing_R();
    double spacing_c = this->GetSkeleton()->GetSpacing_C();
    
    double origin_s = this->GetSkeleton()->GetOrigin_S();
    double origin_r = this->GetSkeleton()->GetOrigin_R();
    double origin_c = this->GetSkeleton()->GetOrigin_C();
    
    output_array_of_world_positions.SetNumberOfElements(this->m_positions.GetNumberOfElements()+2);
    
    (output_array_of_world_positions[0]).S() = origin_s + psn1->GetPosition().S()*spacing_s;
    (output_array_of_world_positions[0]).R() = origin_r + psn1->GetPosition().R()*spacing_r;
    (output_array_of_world_positions[0]).C() = origin_c + psn1->GetPosition().C()*spacing_c;
    for(int j=0; j<psn1->GetPosition().GetNumberOfValues(); j++)
    {
        output_array_of_world_positions[0].V(j) = psn1->GetPosition().V(j);
    }
    
    mxListIterator<mxPoint> it;
    int i=1;
    for(it.SetToBegin(m_positions), i=1; it.IsValid(); it.MoveToNext(), i++)
    {
        mxPoint *pp = it.GetElementAddress();
        (output_array_of_world_positions[i]).S() = origin_s + pp->S()*spacing_s;
        (output_array_of_world_positions[i]).R() = origin_r + pp->R()*spacing_r;
        (output_array_of_world_positions[i]).C() = origin_c + pp->C()*spacing_c;
//!!!!!!! THIS SEEMS TO BE INCORRECT - INSTEAD OF 'psn1->m_position' it should be 'pp' - CHECK !!!!!!!!!!!!!
        for(int j=0; j<psn1->GetPosition().GetNumberOfValues(); j++)
        {
            output_array_of_world_positions[i].V(j) = pp->V(j);
        }
    }
    
    (output_array_of_world_positions[output_array_of_world_positions.GetNumberOfElements()-1]).S() = origin_s + psn2->GetPosition().S()*spacing_s;
    (output_array_of_world_positions[output_array_of_world_positions.GetNumberOfElements()-1]).R() = origin_r + psn2->GetPosition().R()*spacing_r;
    (output_array_of_world_positions[output_array_of_world_positions.GetNumberOfElements()-1]).C() = origin_c + psn2->GetPosition().C()*spacing_c;
    for(int j=0; j<psn2->GetPosition().GetNumberOfValues(); j++)
    {
        output_array_of_world_positions[output_array_of_world_positions.GetNumberOfElements()-1].V(j) = psn2->GetPosition().V(j);
    }
    
    return 1;
}



int mxSkeletonLink::IsConnectedToNodes()
{
	if(m_node1 && m_node2) return 01;
	return 0;
}


int mxSkeletonLink::IsEqualTo(mxSkeletonLink *psl)
{
	if(!(this->m_node1->GetPosition().IsEqualPositionIn_3D(psl->m_node1->GetPosition()))) return 0;
	if(!(this->m_node2->GetPosition().IsEqualPositionIn_3D(psl->m_node2->GetPosition()))) return 0;
	if(this->GetPositionsList()->GetNumberOfElements() != psl->GetPositionsList()->GetNumberOfElements()) return 0;
	if(this->m_skeletonization_voxels.GetNumberOfElements() != psl->m_skeletonization_voxels.GetNumberOfElements()) return 0;

	if(this->m_positions.GetNumberOfElements() > 0)
	{
		// Check the first element in the list of positions
		if(!(this->m_positions.GetBeginElement().IsEqualPositionIn_3D(psl->m_positions.GetBeginElement()))) return 0;

		// Check all positions one by one
		mxListIterator< mxPoint > it1, it2;
        for(it1.SetToBegin(m_positions), it2.SetToBegin(psl->m_positions); it1.IsValid() && it2.IsValid(); it1.MoveToNext(), it2.MoveToNext())
        {
            if(!(it1.GetElementAddress()->IsEqualPositionIn_3D(it2.GetElement()))) return 0;
        }
	}

	if(this->m_skeletonization_voxels.GetNumberOfElements() > 0)
	{
		// Check the first element in the list of points
		if(!(m_skeletonization_voxels.GetBeginElement().IsEqualIndexIn_3D(psl->m_skeletonization_voxels.GetBeginElement()))) return 0;

		// Check all positions one by one
		mxListIterator< mxVoxel > it1, it2;
        for(it1.SetToBegin(m_skeletonization_voxels), it2.SetToBegin(psl->m_skeletonization_voxels); it1.IsValid() && it2.IsValid(); it1.MoveToNext(), it2.MoveToNext())
        {
            if(!it1.GetElementAddress()->IsEqualIndexIn_3D(it2.GetElement())) return 0;
        }
	}
	return 1;
}


int mxSkeletonLink::IsListOfPositionsEmpty()
{
	if(m_positions.IsEmpty()) return 1;
	return 0;
}


int mxSkeletonLink::IsLoop()
{
	if(m_node1==m_node2) return 1;
    return 0;
}


int mxSkeletonLink::IsStub()
{
	if(m_node1->GetNumberOfLinks()==1 || m_node2->GetNumberOfLinks()==1) return 1;
    return 0;
}


int mxSkeletonLink::IsLocalPositionOnLink(mxPosition &local_position)
{
    mxArray< mxPoint* > array_of_position_pointers;
    this->GetArrayOfPositionPointersIncludingNodes(array_of_position_pointers);
    
    for(unsigned int i=0; i<array_of_position_pointers.GetNumberOfElements()-1; i++)
    {
        double d1 = local_position.DistanceEuclidean(*(array_of_position_pointers[i]));
        double d2 = local_position.DistanceEuclidean(*(array_of_position_pointers[i+1]));
        double d_sum = array_of_position_pointers[i]->DistanceEuclidean(*(array_of_position_pointers[i+1]));
        
        double precision = mxSkeleton_POINT_PRECISION;//local_position. GetPrecision();
        if( (d1+d2 >= d_sum - precision) && (d1+d2 <= d_sum + precision) )
        {
            return 1;
        }
    }
    return 0;
}


int mxSkeletonLink::IsWorldPositionOnLink(mxPosition &world_position)
{
    mxArray<mxPoint> array_of_world_positions;
    this->GetArrayOfWorldPositionsIncludingNodes(array_of_world_positions);
    
    for(unsigned int i=0; i<array_of_world_positions.GetNumberOfElements()-1; i++)
    {
        double d1 = world_position.DistanceEuclidean(array_of_world_positions[i]);
        double d2 = world_position.DistanceEuclidean(array_of_world_positions[i+1]);
        double d_sum = array_of_world_positions[i].DistanceEuclidean(array_of_world_positions[i+1]);
        
        double precision = 1;//world_position.GetPrecision();
        std::cout<<std::endl;
        std::cout<<"world_position="<<world_position<<std::endl;
        std::cout<<"array_of_world_positions["<<i<<"]="<<array_of_world_positions[i]<<std::endl;
        std::cout<<"array_of_world_positions["<<i+1<<"]="<<array_of_world_positions[i+1]<<std::endl;
        std::cout<<"d1="<<d1<<", d2="<<d2<<", d1+d2="<<(d1+d2)<<", d_sum="<<d_sum<<", precision="<<precision<<std::endl;
        if( (d1+d2 >= d_sum - precision) && (d1+d2 <= d_sum + precision) ) { return 1; }
    }
    return 0;
}






std::ostream& mxSkeletonLink::Print(std::ostream &o)
{
    o<<" mxSkeletinLink:"<<std::endl;
    o<<" - number of points: "<<this->GetNumberOfPositions()<<std::endl;
    o<<" - node1 address: "<<((void*)m_node1)<<std::endl;
    mxPoint *p = this->GetNode_1_Position();
    if(p) o<<" - node1 point: "<<(*p)<<std::endl;
    o<<" - node2 address: "<<((void*)m_node2)<<std::endl;
    p = this->GetNode_2_Position();
    if(p) o<<" - node2 point: "<<(*p)<<std::endl;
    return(o);
}



int mxSkeletonLink::RemovePosition(mxPoint &position_to_remove)
{
	mxListNode<mxPoint> *pln_to_delete = NULL;
	int is_delete_position_found = 0;
	mxListIterator<mxPoint> itp;
	for(itp.SetToBegin(m_positions); itp.IsValid(); itp.MoveToNext())
	{
		if(itp.GetElementAddress()->IsEqualPositionIn_3D(position_to_remove))
		{
			pln_to_delete = itp.GetNode();
			is_delete_position_found = 1;
			break;
		}
	}
	if(!is_delete_position_found) return 0;
	m_positions.DeleteNode(pln_to_delete);
	return 1;
}


int mxSkeletonLink::InvertPositionOder()
{
	if(!this->IsConnectedToNodes()) return 0;

	//This invertes the list order
	m_positions.InvertOrder();

	mxSkeletonNode *psn;
	psn = m_node1;
	m_node1 = m_node2;
	m_node2 = psn;

	return 1;
}


double mxSkeletonLink::LengthOfLink()
{
	double distance = 0;
	
    mxPoint world_prev;
    this->m_skeleton->WorldPositionFromLocalPosition(this->GetNode_1_Position()->C(), this->GetNode_1_Position()->R(), this->GetNode_1_Position()->S(), world_prev.C(), world_prev.R(), world_prev.S());
	
	mxListIterator<mxPoint> it;
	for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
	{
        mxPoint world_current;
        this->m_skeleton->WorldPositionFromLocalPosition(it.GetElementAddress()->C(), it.GetElementAddress()->R(), it.GetElementAddress()->S(), world_current.C(), world_current.R(), world_current.S());
        distance += world_prev.DistanceEuclidean(world_current);
        world_prev = world_current;
	}
    
    mxPoint world_current;
    this->m_skeleton->WorldPositionFromLocalPosition(this->GetNode_2_Position()->C(), this->GetNode_2_Position()->R(), this->GetNode_2_Position()->S(), world_current.C(), world_current.R(), world_current.S());
	distance += world_prev.DistanceEuclidean(world_current);

	return distance;
}


int mxSkeletonLink::GetBounds(int &s_start_included, int &s_end_included, int &r_start_included, int &r_end_included, int &c_start_included, int &c_end_included)
{
	if(m_positions.IsEmpty()) return 0;

	//Initialize values with the first node
	mxPoint *pos = this->GetNode_1_Position();
	s_start_included = (int) pos->S();
	s_end_included = (int) pos->S();
	r_start_included = (int) pos->R();
	r_end_included = (int) pos->R();
	c_start_included = (int) pos->C();
	c_end_included = (int) pos->C();

	mxListIterator<mxPoint> it;
	for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
	{
		mxPoint *ppos = it.GetElementAddress();
		if(ppos->S() < s_start_included) s_start_included = (int) ppos->S();
		if(ppos->S() > s_end_included) s_end_included = (int) ppos->S();
		if(ppos->R() < r_start_included) r_start_included = (int) ppos->R();
		if(ppos->R() > r_end_included) r_end_included = (int) ppos->R();
		if(ppos->C() < c_start_included) c_start_included = (int) ppos->C();
		if(ppos->C() > c_end_included) c_end_included = (int) ppos->C();
	}

	//Check also the end node
	pos = this->GetNode_2_Position();
	if(pos->S() < s_start_included) s_start_included = (int) pos->S();
	if(pos->S() > s_end_included) s_end_included = (int) pos->S();
	if(pos->R() < r_start_included) r_start_included = (int) pos->R();
	if(pos->R() > r_end_included) r_end_included = (int) pos->R();
	if(pos->C() < c_start_included) c_start_included = (int) pos->C();
	if(pos->C() > c_end_included) c_end_included = (int) pos->C();

	return 1;
}




int mxSkeletonLink::Smooth(double prev_3, double prev_2, double prev_1, double next_1, double next_2, double next_3, double node_point_strength)
{
	//Check if the link is connected to nodes
	if(!this->IsConnectedToNodes()) return 0;

	//There must be at least two points in the link! If not, do not smooth, but return success (return 1)
	if(m_positions.GetNumberOfElements()<=1) return 1;
	
	// find the link middle position (position half way between start and end node position)
	mxPoint link_middle_pos;
    mxPoint *node_1_pos = this->GetNode_1_Position();
	mxPoint *node_2_pos = this->GetNode_2_Position();
	
	link_middle_pos.S() = (node_1_pos->S() + node_2_pos->S()) / 2;
	link_middle_pos.R() = (node_1_pos->R() + node_2_pos->R()) / 2;
	link_middle_pos.C() = (node_1_pos->C() + node_2_pos->C()) / 2;

	double sum_distance_from_link_middle_pos_old = 0;
	mxListIterator<mxPoint> itp;
	for(itp.SetToBegin(m_positions); itp.IsValid(); itp.MoveToNext())
	{
		mxPoint pos = itp.GetElement();
		sum_distance_from_link_middle_pos_old += link_middle_pos.DistanceEuclidean(pos); 
	}

	mxArray<mxPoint> a;
	a.SetNumberOfElements(m_positions.GetNumberOfElements());

	//Copy the list into the array
	int i=0;
	for(itp.SetToBegin(m_positions), i=0; itp.IsValid() && i<a.GetNumberOfElements(); itp.MoveToNext(), i++)
	{
		mxPoint *p = itp.GetElementAddress();
		a[i] = (*p);
	}

	//Delete the list of link positions
	this->m_positions.Reset();

	//Initialize the positions
	mxPoint p1 = *(this->GetNode_1_Position());
    mxPoint p2; p2.SetPosition(0,0,0);
	mxPoint p3; p3.SetPosition(0,0,0);
    mxPoint n1; n1 = a[1];//No check needed for n1 because we must have at least 2 points in the link
    mxPoint n2;
	if(a.GetNumberOfElements()==2) { n2 = *(this->GetNode_2_Position()); }
	if(a.GetNumberOfElements()>2) { n2 = a[2]; }
    mxPoint n3;
	if(a.GetNumberOfElements()==3) { n3 = *(this->GetNode_2_Position()); }
	if(a.GetNumberOfElements()>3) { n3 = a[3]; }
	if(a.GetNumberOfElements()<3) { n3.SetPosition(0,0,0); }
    
    // conversion to int is done on purpose!
	for(i=0; i<(int)(a.GetNumberOfElements()); i++)
	{
		//Initialize coefficient values
		double cp3 = prev_3;
		double cp2 = prev_2;
		double cp1 = prev_1;
		double cn1 = next_1;
		double cn2 = next_2;
		double cn3 = next_3;

		//Check if any of the positions is the node position and adjust coefficient
		mxPoint node1_p = *(this->GetNode_1_Position());
		mxPoint node2_p = *(this->GetNode_2_Position());
		if(p3.IsEqualPositionIn_3D(node1_p)) cp3 = cp3 * node_point_strength;
		if(p2.IsEqualPositionIn_3D(node1_p)) cp2 = cp2 * node_point_strength;
		if(p1.IsEqualPositionIn_3D(node1_p)) cp1 = cp1 * node_point_strength;
		if(n1.IsEqualPositionIn_3D(node2_p)) cn1 = cn1 * node_point_strength;
		if(n2.IsEqualPositionIn_3D(node2_p)) cn2 = cn2 * node_point_strength;
		if(n3.IsEqualPositionIn_3D(node2_p)) cn3 = cn3 * node_point_strength;

		//Count the number of non-zero coefficients (number of positions taken into account)
		double n = 1.0;//start with 1 because of the current position
		if(i-3>=-1) n += cp3;
		if(i-2>=-1) n += cp2;
		if(i-1>=-1) n += cp1;
		if(i+1<=(int)a.GetNumberOfElements()) n += cn1;
		if(i+2<=(int)a.GetNumberOfElements()) n += cn2;
		if(i+3<=(int)a.GetNumberOfElements()) n += cn3;

		//Calculate new position
        mxPoint p_new;
		p_new.S() = (p3.S()*cp3 + p2.S()*cp2 + p1.S()*cp1 + a[i].S() + n1.S()*cn1 + n2.S()*cn2 + n3.S()*cn3) / n;
		p_new.R() = (p3.R()*cp3 + p2.R()*cp2 + p1.R()*cp1 + a[i].R() + n1.R()*cn1 + n2.R()*cn2 + n3.R()*cn3) / n;
		p_new.C() = (p3.C()*cp3 + p2.C()*cp2 + p1.C()*cp1 + a[i].C() + n1.C()*cn1 + n2.C()*cn2 + n3.C()*cn3) / n;

		// Copy the previous values.
        for(int v=0; v<a[i].GetNumberOfValues(); v++)
        {
            p_new.V(v) = a[i].V(v);
        }
//		p_new.CopyTimeElementsFrom(a[i]);


		//Put the newly calculated position into the list
		this->m_positions.AddToEnd(p_new);

		//Assign new positions to the positions in kernel
		p3 = p2;
		p2 = p1;
		p1 = a[i];
		n1 = n2;
		n2 = n3;
		//if(i+4==a.NumberOfElements()) n3 = link_end_node_position;
		if(i+4==a.GetNumberOfElements()) n3 = *(this->GetNode_2_Position());
		if(i+4>(int)a.GetNumberOfElements()) n3.SetPosition(0,0,0);
		if(i+4<(int)a.GetNumberOfElements()) n3 = a[i+4];
	}

	//Now that the link is smooth, we have to rescale to maintain the old link length
	
	double sum_distance_from_link_middle_pos_new = 0;
	for(itp.SetToBegin(m_positions); itp.IsValid(); itp.MoveToNext())
	{
		mxPoint *p = itp.GetElementAddress();
		sum_distance_from_link_middle_pos_new += link_middle_pos.DistanceEuclidean(*p);
	}

	double ratio = sum_distance_from_link_middle_pos_old / sum_distance_from_link_middle_pos_new;

	//Reposition linearly each position in link by increasing distance from the link_middle_pos
	
	for(itp.SetToBegin(m_positions); itp.IsValid(); itp.MoveToNext())
	{
		mxPoint *p = itp.GetElementAddress();
		double distance = link_middle_pos.DistanceEuclidean(*p);
		p->SetDistanceFromPosition(link_middle_pos,(distance*ratio));
	}

	return 1;
}


int mxSkeletonLink::Sample(int number_of_samples)
{
	if(number_of_samples<1) return 0;

	if(this->GetNumberOfPositions()==number_of_samples) return 1;
    
    
	//If oversampling
	if(number_of_samples>GetNumberOfPositions())
	{
		while(GetNumberOfPositions()!=number_of_samples)
		{
			//std::cout<<"("<<GetNumberOfPositions()<<","<<number_of_samples<<") ";
			
            //If the difference is still too big, upsample to two times 'the number of already existing positions' + 1
			if(number_of_samples>(2*GetNumberOfPositions()+1)) this->SimpleUpSamplingOfPath(2*GetNumberOfPositions()+1);
			//If the difference is not too big, upsamle to wanted number of samples
			else this->SimpleUpSamplingOfPath(number_of_samples);
		}
	}
    
    // If downsampling - performs equal spacing downsampling
    else
    {
        mxList<double> list_of_normalized_distances;
        mxList<mxPoint> list_of_positions;
        
        double d = 1.0 / ((double) number_of_samples+1);
        for(unsigned int i=1; i<=number_of_samples; i++)
        {
            double nd = ((double)i)*d;// / l);
            list_of_normalized_distances.AddToEnd(nd);
        }
        
        
        double normalized_distance;
        mxListIterator<double> itd;
        
        
        //For the existing positions in the link get the list of cumulative distances from the START node
        mxArray<double> cumulative_distances_array;
        cumulative_distances_array.SetNumberOfElements(this->GetPositionsList()->GetNumberOfElements()+2);
        mxArray<mxPoint> cumulative_distances_positions;
        cumulative_distances_positions.SetNumberOfElements(this->GetPositionsList()->GetNumberOfElements()+2);
        double cumulative_distance = 0;
        mxPoint *ppos, *ppos_previous;
        mxSkeletonNode *psn = this->GetNode_1();
        ppos_previous = &(psn->GetPosition());
        cumulative_distances_array[0] = 0;
        cumulative_distances_positions[0] = psn->GetPosition();
        int i=1;
        mxListIterator<mxPoint> itp;
        for(itp.SetToBegin(this->GetPositionsList()), i=1; itp.IsValid(); itp.MoveToNext(), i++)
        {
            ppos = itp.GetElementAddress();
            cumulative_distance += ppos_previous->DistanceEuclidean(*ppos);
            cumulative_distances_positions[i] = *ppos;
            cumulative_distances_array[i] = cumulative_distance;
            ppos_previous = ppos;
        }
        psn = this->GetNode_2();
        cumulative_distance += ppos_previous->DistanceEuclidean(psn->GetPosition());
        cumulative_distances_array[cumulative_distances_array.GetNumberOfElements()-1] = cumulative_distance;
        cumulative_distances_positions[cumulative_distances_positions.GetNumberOfElements()-1] = psn->GetPosition();
        //The 'cumulative_distance' now has the WHOLE distance of the link path, so we devide entered distances with this one to normalize
        for(i=0; i<cumulative_distances_array.GetNumberOfElements(); i++) cumulative_distances_array[i] = cumulative_distances_array[i]/cumulative_distance;
        
        
        //Cumulative array: [0]=0,...[noel-1]=1; noel = n_o_positions +2;
        
        //Insert new positions
        //Go through all the normalized distances where we have to enter new positions...
        for(itd.SetToBegin(list_of_normalized_distances); itd.IsValid(); itd.MoveToNext())
        {
            normalized_distance = itd.GetElement();
            
            //... and find between which normalized cumulative distances it belongs
            int is_distance_found = 0;
            for(i=0; i<cumulative_distances_array.GetNumberOfElements()-1 && !is_distance_found; i++)
            {
                //If the distance is exactly the same (desired position already exists in the list of positions), just enter to output list of added positions
                if(cumulative_distances_array[i]==normalized_distance)
                {
                    list_of_positions.AddToEnd(cumulative_distances_positions[i]);
                    is_distance_found = 1;
                }
                else
                {
                    if(cumulative_distances_array[i]<normalized_distance && cumulative_distances_array[i+1]>normalized_distance )
                    {
                        double distance1, distance2;
                        mxPoint pos1, pos2;
                        pos1 = cumulative_distances_positions[i];
                        pos2 = cumulative_distances_positions[i+1];
                        distance1 = cumulative_distances_array[i];
                        distance2 = cumulative_distances_array[i+1];
                        
                        //Now we have to scale and insert new position between 'pos1' and 'pos2' based on 'cumulative_distances_array[i]' distance and 'distance2'.
                        double percentage_of_distance = (normalized_distance-distance1)/(distance2-cumulative_distances_array[i]);
                        double ds = (pos2.S() - pos1.S())*percentage_of_distance;
                        double dr = (pos2.R() - pos1.R())*percentage_of_distance;
                        double dc = (pos2.C() - pos1.C())*percentage_of_distance;
                        mxPoint new_pos;
                        new_pos.S() = pos1.S() + ds;
                        new_pos.R() = pos1.R() + dr;
                        new_pos.C() = pos1.C() + dc;
                        list_of_positions.AddToEnd(new_pos);
                        
                        is_distance_found = 1;
                    }
                }
            }
        }
        
        this->m_positions = list_of_positions;
    }

	return 1;
}



int mxSkeletonLink::SimpleUpSamplingOfPath(int number_of_samples)
{
	//If there are n number of elements in position list, we can add maximum n+1 new positions
	if(number_of_samples<=this->GetNumberOfPositions()) return 0;
	if(number_of_samples>(2*this->GetNumberOfPositions()+1)) return 0;

	//Specially - if list of positions has only one element
	if(this->GetNumberOfPositions()==1)
	{
		//If the result must have 2 positions...
		if(number_of_samples==2)
		{
			//... add new position to left end
			mxPoint pos1, pos2, pos_new;
			pos1 = m_node1->GetPosition();
			pos2 = m_positions.GetBeginElement();
            pos_new.CopyFrom(pos1);
            pos_new.Addition(pos2.S(), pos2.R(), pos2.C());
            pos_new.Division(2);
			m_positions.AddToBegin(pos_new);
			return 1;
		}
		else
		{
			//If the result must have 3 positions...
			if(number_of_samples==3)
			{
				//... add new position both to left and right end
				mxPoint pos, pos_node, pos_new;
				pos_node = m_node1->GetPosition();
				pos = m_positions.GetBeginElement();
                pos_new.CopyFrom(pos_node);
                pos_new.Addition(pos.S(), pos.R(), pos.C());
                pos_new.Division(2);
				m_positions.AddToBegin(pos_new);

				pos_node = m_node2->GetPosition();
                pos_new.CopyFrom(pos_node);
                pos_new.Addition(pos.S(), pos.R(), pos.C());
                pos_new.Division(2);
				m_positions.AddToBegin(pos_new);

				return 1;
			}
		}
	}

	//Record number of positions, because we modify the list of positions temporarily
	int number_of_positions = GetNumberOfPositions();

	//Temporarily add the start node position and end node position to the list of positions
	m_positions.AddToEnd(m_node2->GetPosition());
	m_positions.AddToBegin(m_node1->GetPosition());

	//This is the value with which the insertion positions in list will be calculated
	double d = ((double)(number_of_positions*2+1)) / ((double)(number_of_samples));
			
	//The index goes from [1...(number_of_samples-number_of_positions)]
	//Do adding in inverse order! (so that the indexes do not get changed)
	mxListNode<mxPoint> *node;
	for(int i=number_of_samples-number_of_positions; i>=1; i--)
	{
		int insertion_index = (int)(((double)i)*d);
		node = this->m_positions.GetNodeWithIndex(insertion_index-1);
		mxPoint pos1, pos2, pos_new;
		pos1 = node->GetElement();
        node = node->GetNextNode();//m_right;
		pos2 = node->GetElement();
        pos_new.CopyFrom(pos1);
        pos_new.Addition(pos2.S(), pos2.R(), pos2.C());
        pos_new.Division(2);
		this->m_positions.AddBeforeNode(node,pos_new);
	}

	//Remove temporarily added start node position and end node position
	m_positions.DeleteBegin();
	m_positions.DeleteEnd();

	return 1;
}


int mxSkeletonLink::PasteToExisting3DImage(mxImage &output_binary_skeleton_image, unsigned int scalar_index)
{
	if(output_binary_skeleton_image.IsEmpty()) return 0;

	if(scalar_index>=this->GetSkeleton()->GetMaximumNumberOfScalars()) scalar_index = 0;

	//Enter start and end node
	mxPoint start_node_position = *(this->GetNode_1_Position());
	mxPoint end_node_position = *(this->GetNode_2_Position());
	output_binary_skeleton_image(((int)(end_node_position.S())),((int)(end_node_position.R())),((int)(end_node_position.C()))) 
		= 255;
	output_binary_skeleton_image(((int)(start_node_position.S())),((int)(start_node_position.R())),((int)(start_node_position.C()))) 
		= 255;

	mxVoxel p1, p2;
	p1.SetIndex(((int)(start_node_position.S())),((int)(start_node_position.R())),((int)(start_node_position.C())));

	mxGeometry g;
	g.SetDimensions(output_binary_skeleton_image.GetDimension_S(),output_binary_skeleton_image.GetDimension_R(),output_binary_skeleton_image.GetDimension_C());

	//Enter other link positions
	mxPoint *ppos;
	mxListIterator<mxPoint> itp;
	for(itp.SetToBegin(m_positions); itp.IsValid(); itp.MoveToNext() )
	{
		ppos = itp.GetElementAddress();
		p2.SetIndex(((int)(ppos->S())),((int)(ppos->R())),((int)(ppos->C())));
		output_binary_skeleton_image(p2.S(),p2.R(),p2.C()) = 255;
		
        int s,r,c;
        for(g.ForLine(p1.S(),p1.R(),p1.C(), p2.S(),p2.R(),p2.C()); g.GetLine(s,r,c); )
        {
            output_binary_skeleton_image(s,r,c) = 255;
        }
        
		p1 = p2;
	}

	//Make the line between the end node position and the last position in the link (which is now p1)
	p2.SetIndex(((int)(end_node_position.S())),((int)(end_node_position.R())),((int)(end_node_position.C())));
	output_binary_skeleton_image(p2.S(),p2.R(),p2.C()) = 255;
    
    int s,r,c;
    for(g.ForLine(p1.S(),p1.R(),p1.C(), p2.S(),p2.R(),p2.C()); g.GetLine(s,r,c); )
    {
        output_binary_skeleton_image(s,r,c) = 255;
    }
	return 1;
}


int mxSkeletonLink::LoadFromStringOfPositions(const char *positions_string, mxPoint &output_start_node_position, mxPoint &output_end_node_position)
{
	int i = 0;
	int count  = 0;

	char *temp;

	int spline_point_counter = 0;

	//Check for the open brackets for coordinates of mxPoint or mxVoxel
	while(positions_string[i]=='(' || positions_string[i]=='[')
	{
		//Find the '}' and the count of float number
		for(count = 0; positions_string[i+count]!='}'; count++)
		{
			if(positions_string[i+count]=='\0') //error in string
			{
				std::cout<<"mxSkeletonLink::LoadFromStringOfPositions(): Error in string: "<<positions_string<<std::endl;
				return 0;
			}
		}
        

		//Copy the string
		temp = new char [count+2];
		temp[count+1] = '\0';
        for(int j=0; j<count+1; j++)
        {
            temp[j] = positions_string[j+i];
        }

		if(positions_string[i]=='(')
		{
			//Read the position
			mxPoint *ppos = m_positions.AddNewToEnd();
			if(!ppos->ReadFromString(temp)) //error in string for position
			{
				m_positions.DeleteEnd();
				std::cout<<"mxSkeletonLink::LoadFromStringOfPositions(): Error reading mxPoint from string: "<<temp<<std::endl;
				return 0;
			}
		}

		if(positions_string[i]=='[')
		{
			//Read the point
			mxVoxel *pp = m_skeletonization_voxels.AddNewToEnd();
			if(!pp->ReadFromString(temp)) //error in string for point
			{
				m_skeletonization_voxels.DeleteEnd();
				std::cout<<"mxSkeletonLink::LoadFromStringOfPositions(): Error reading mxVoxel from string: "<<temp<<std::endl;
				return 0;
			}
		}
		

		//Release string
		delete [] temp;
		//Set the new value for index i
		i = i+count+1;

		//Increase the spline point counter
		spline_point_counter++;
	}
	
	//At this point all the positions are in the list. Put left end position into 'output_start_node_position'
	// and the right end position into 'output_end_node_position'. After that erase those positions from the list.
	output_start_node_position.CopyFrom(m_positions.GetBeginElement());
	output_end_node_position.CopyFrom(m_positions.GetEndElement());
	this->m_positions.DeleteBegin();
	this->m_positions.DeleteEnd();

	return 1;
}



void mxSkeletonLink::RemoveAllPositionsAndSkeletonizationVoxels()
{
	m_positions.Reset();
	m_skeletonization_voxels.Reset();
}




//-------------------------------------------------------------------------------------------------------------------------





mxSkeletonNode::mxSkeletonNode()
{
	m_position.SetPosition(-1,-1,-1);
	m_metric_value = -1; 
	m_metric_value_2 = -1;
	m_link_to_source_node = NULL; 
	m_link_to_source_node_2 = NULL;
}


mxSkeletonNode::~mxSkeletonNode()
{
	this->Reset();
}


void mxSkeletonNode::Reset()
{
	m_links.Reset();
	m_skeletonization_voxels.Reset();
	m_position.SetPosition(-1,-1,-1);
}


int mxSkeletonNode::GetNumberOfLinks()
{
	return(m_links.GetNumberOfElements());
}


mxPoint& mxSkeletonNode::GetPosition()
{
	return m_position;
}


mxList<mxSkeletonLink*>* mxSkeletonNode::GetLinksList()
{
	return &m_links;
}


mxList<mxVoxel>* mxSkeletonNode::GetSkeletonizationVoxels()
{
	return &m_skeletonization_voxels;
}


double& mxSkeletonNode::GetMetricValue()
{
	return m_metric_value;
}


double& mxSkeletonNode::GetMetricValue_2()
{
	return m_metric_value_2;
}


mxSkeletonLink* mxSkeletonNode::GetLinkToSourceNode()
{
	return m_link_to_source_node;
}


mxSkeletonLink* mxSkeletonNode::GetLinkToSourceNode_2()
{
	return m_link_to_source_node_2;
}


std::ostream& mxSkeletonNode::Print(std::ostream &o)
{
	//PrintType(o)<<std::endl;
    o<<" Skeleton Node:"<<std::endl;
	o<<" - position: "<<m_position<<std::endl;
	o<<" - number of links: "<<m_links.GetNumberOfElements()<<std::endl;
	return(o);
}


int mxSkeletonNode::LoadFromStringOfPoints(char *points_string)
{
	int i = 0;
	int count  = 0;

	//Check for the open brackets for coordinates of mxVoxel
	while(points_string[i]=='[')
	{
		//Find the '}' and the count of int number
		for(count = 0; points_string[i+count]!='}'; count++)
		{
			if(points_string[i+count]=='\0') 
			{
				std::cout<<"bdmxSkeletonNode::LoadFromStringOfPoints(): Error!"<<std::endl;
				return 0;//error in string
			}
		}

		//Read the point
		mxVoxel p;
		if(p.ReadFromString(&(points_string[i]))==0)
		{
			std::cout<<"mxSkeletonNode::LoadFromStringOfPoints(): Error!"<<std::endl;
			return 0;
		}

		//Enter point into the list
		this->m_skeletonization_voxels.AddToEnd(p);
		
		//Set the new value for index i
		i = i+count+1;

	}

	return 1;
}







int mxSkeletonNode::GetUpdates_PropagateHigherValue()
{
	double propagated_value;
	double current_value;
	int is_metric_value_changed = 0;
	mxSkeletonLink *psl;
	mxSkeletonNode *psn;
	
	//For each link of this node...
	mxListIterator<mxSkeletonLink*> it;
	for(it.SetToBegin(m_links); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElement();
		//... get the other node of the link...
		psn = psl->GetNode_1();
		if(psn==this) psn = psl->GetNode_2();
		if(psn!=this)//If the pointer to node is still the same, it means that we found a loop, do not process!
		{
			propagated_value = psn->m_metric_value + (1/((psl->GetCost())*(psl->GetCost())*(psl->GetCost())));

			//If the node has not been updated before...
			if(m_metric_value<0)
			{
				if(psn->m_metric_value>=0)//The update is not coming from a node in the initial state...
				{
					is_metric_value_changed = 1;
                    m_metric_value = propagated_value;
					m_link_to_source_node = psl;
				}
			}
			//If the node has been updated before...
			else
			{
				if(psn->m_metric_value>=0)
				{
					current_value = m_metric_value;

                    if(propagated_value<current_value)
					{
						is_metric_value_changed = 1;
                        m_metric_value = propagated_value;
						m_link_to_source_node = psl;
					}
				}
			}
		}
	}

	return is_metric_value_changed;
}



int mxSkeletonNode::GetUpdates_PropagateLowerValue()
{
	double propagated_value;
	double current_value;
	int is_metric_value_changed = 0;
	mxSkeletonLink *psl;
	mxSkeletonNode *psn;
	
	//For each link of this node...
	mxListIterator<mxSkeletonLink*> it;
	for(it.SetToBegin(m_links); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElement();
		//... get the other node of the link...
		psn = psl->GetNode_1();
		if(psn==this) psn = psl->GetNode_2();
		if(psn!=this)//If the pointer to node is still the same, it means that we found a loop, do not process!
		{
			propagated_value = psn->m_metric_value + (psl->GetCost())*(psl->GetCost())*(psl->GetCost());

			//If the node has not been updated before...
			if(m_metric_value<0)
			{
				if(psn->m_metric_value>=0)//The update is not coming from a node in the initial state...
				{
					is_metric_value_changed = 1;
					m_metric_value = propagated_value;
					m_link_to_source_node = psl;
				}
			}
			//If the node has been updated before...
			else
			{
				if(psn->m_metric_value>=0)
				{
					current_value = m_metric_value;

					if(propagated_value<current_value)
					{
						is_metric_value_changed = 1;
						m_metric_value = propagated_value;
						m_link_to_source_node = psl;
					}
				}
			}
		}
	}

	return is_metric_value_changed;
}



int mxSkeletonNode::GetUpdates_PropagateHigherValue_2()
{
	double propagated_value;
	double current_value;
	int is_metric_value_changed = 0;
	mxSkeletonLink *psl;
	mxSkeletonNode *psn;
	
	//For each link of this node...
	mxListIterator<mxSkeletonLink*> it;
	for(it.SetToBegin(m_links); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElement();

		//... get the other node of the link...
		psn = psl->GetNode_1();
		if(psn==this) psn = psl->GetNode_2();
		if(psn!=this)//If the pointer to node is still the same, it means that we found a loop, do not process!
		{
			propagated_value = psn->m_metric_value_2 + (1/((psl->GetCost())*(psl->GetCost())*(psl->GetCost())));

			//If the node has not been updated before...
			if(m_metric_value_2<0)
			{
				if(psn->m_metric_value_2>=0)//The update is not coming from a node in the initial state...
				{
					is_metric_value_changed = 1;
					m_metric_value_2 = propagated_value;
					m_link_to_source_node_2 = psl;
				}
			}
			//If the node has been updated before...
			else
			{

				if(psn->m_metric_value_2>=0)
				{
					current_value = m_metric_value_2;

					if(propagated_value<current_value)
					{
						//std::cout<<"(7)"<<std::endl;
						is_metric_value_changed = 1;
						//this->m_metric_value = psn->m_metric_value+psl->m_metric_value;
						m_metric_value_2 = propagated_value;
						m_link_to_source_node_2 = psl;
					}
				}
			}
		}
	}

	return is_metric_value_changed;
}



int mxSkeletonNode::GetUpdates_PropagateLowerValue_2()
{
	double propagated_value;
	double current_value;
	int is_metric_value_changed = 0;
	mxSkeletonLink *psl;
	mxSkeletonNode *psn;
	
	//For each link of this node...
	mxListIterator<mxSkeletonLink*> it;
	for(it.SetToBegin(m_links); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElement();
		//... get the other node of the link...
		psn = psl->GetNode_1();
		if(psn==this) psn = psl->GetNode_2();
		if(psn!=this)//If the pointer to node is still the same, it means that we found a loop, do not process!
		{
			propagated_value = psn->m_metric_value_2 + (psl->GetCost())*(psl->GetCost())*(psl->GetCost());

			//If the node has not been updated before...
			if(m_metric_value_2<0)
			{
				if(psn->m_metric_value_2>=0)//The update is not coming from a node in the initial state...
				{
					is_metric_value_changed = 1;
					m_metric_value_2 = propagated_value;
					m_link_to_source_node_2 = psl;
				}
			}
			//If the node has been updated before...
			else
			{
				if(psn->m_metric_value_2>=0)
				{
					current_value = m_metric_value_2;
                    
					if(propagated_value<current_value)
					{
						is_metric_value_changed = 1;
						m_metric_value_2 = propagated_value;
						m_link_to_source_node_2 = psl;
					}
				}
			}
		}
	}

	return is_metric_value_changed;
}



//======================================================================================================================



mxSkeleton3D::mxSkeleton3D()
{
	m_convergence_node1 = NULL;
	m_convergence_node2 = NULL;
}


mxSkeleton3D::~mxSkeleton3D()
{
	this->Reset();
}


void mxSkeleton3D::Reset()
{
	m_links.Reset();
	m_nodes.Reset();
	m_convergence_node1 = NULL;
	m_convergence_node2 = NULL;
}


int mxSkeleton3D::IsEmpty()
{
	if(m_links.GetNumberOfElements()==0 && m_nodes.GetNumberOfElements()==0) return 1;
	else return 0;
}


int mxSkeleton3D::HasAtLeastOneLink()
{
	if(m_links.GetNumberOfElements()>=1 && m_nodes.GetNumberOfElements()>=2) return 1;
	else return 0;
}


unsigned int mxSkeleton3D::GetNumberOfLinks()
{
	return(m_links.GetNumberOfElements());
}


unsigned int mxSkeleton3D::GetNumberOfNodes()
{
	return(m_nodes.GetNumberOfElements());
}


std::ostream& mxSkeleton3D::PrintHeader(std::ostream &o)
{
    o<<" mxSkeleton3D: "<<std::endl;
	o<<" - number of nodes: "<<m_nodes.GetNumberOfElements()<<std::endl;
	o<<" - number of links: "<<m_links.GetNumberOfElements()<<std::endl;
	return(o);
}



//======================================================================================================================



mxSkeleton::mxSkeleton()
{
    this->GetClassName().Assign("mxSkeleton");
    
	m_data.SetNumberOfElements(1);
	m_origin[0] = m_origin[1] = m_origin[2] = m_origin[3] = 0;
    m_spacing[0] = m_spacing[1] = m_spacing[2] = m_spacing[3] = 1;
    
	//mxPoint p;
	//m_maximum_number_of_scalars = p.GetNumberOfValues();
}


mxSkeleton::~mxSkeleton()
{
	this->Reset();
}


void mxSkeleton::Reset()
{
	for(unsigned int i=0; i<m_data.GetNumberOfElements(); i++)
    {
        m_data[i].Reset();
    }
	m_data.SetNumberOfElements(1);
	
    m_origin[0] = m_origin[1] = m_origin[2] = m_origin[3] = 0;
    m_spacing[0] = m_spacing[1] = m_spacing[2] = m_spacing[3] = 1;
    
    for(int i=0; i<this->GetMaximumNumberOfScalars(); i++)
    {
        m_scalars[i].Reset();
    }
    
//    m_scalar_tags.Reset();
//    m_scalar_tags_indexes.Reset();
}


void mxSkeleton::ResetTimeIndex(unsigned int t)
{
	m_data[t].Reset();
}


int mxSkeleton::IsEmpty()
{
	for(unsigned int i=0; i<m_data.GetNumberOfElements(); i++)
    {
        if(!m_data[i].IsEmpty()) return 0;
    }
	return 1;
}


int mxSkeleton::SetNumberOfTimeSeries(unsigned int number_of_time_series)
{
	if(!number_of_time_series) return 0;
    
	m_data.SetNumberOfElements(number_of_time_series);
	
    return 1;
}


unsigned int mxSkeleton::GetNumberOfTimeSeries()
{
	return m_data.GetNumberOfElements();
}


int mxSkeleton::TimeIndexForLink(mxSkeletonLink *input_link, unsigned int &output_time_index)
{
	if(input_link->GetSkeleton()!=this) return 0;
	for(unsigned int i=0; i<m_data.GetNumberOfElements(); i++)
	{ 
		if(input_link->GetSkeleton3D()==(&(m_data[i])))
		{
			output_time_index = i;
			return 1; 
		}
	}
	return 0;
}


int mxSkeleton::Bounds_3D_WorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max)
{
    if(this->IsEmpty()) return 0;
    
    mxPoint min, max;
    this->GetBounds(min, max);
    this->WorldPositionFromLocalPosition(min.C(), min.R(), min.S(), x_min, y_min, z_min);
    this->WorldPositionFromLocalPosition(max.C(), max.R(), max.S(), x_max, y_max, z_max);

    return 1;
}



int mxSkeleton::HasAtLeastOneLink()
{
	for(unsigned int i=0; i<m_data.GetNumberOfElements(); i++)
	{ if(m_data[i].HasAtLeastOneLink()) return 1; }
	return 0;
}


mxList<mxSkeletonLink>* mxSkeleton::GetLinks(unsigned int t)
{
	return &(m_data[t].m_links);
}	


mxList<mxSkeletonNode>* mxSkeleton::GetNodes(unsigned int t)
{
	return &(m_data[t].m_nodes);
}


mxSkeletonNode* mxSkeleton::GetConvergenceNode(unsigned int t)
{
	return m_data[t].m_convergence_node1;
}


mxSkeletonNode* mxSkeleton::GetConvergenceNode_2(unsigned int t)
{
	return m_data[t].m_convergence_node2;
}


unsigned int mxSkeleton::GetNumberOfLinksInTotal()
{
	unsigned int n=0;
	for(unsigned int i=0; i<m_data.GetNumberOfElements(); i++)
	{ n += m_data[i].GetNumberOfLinks(); }
	return n;
}


unsigned int mxSkeleton::GetNumberOfLinks(unsigned int t)
{
	return(m_data[t].m_links.GetNumberOfElements());
}


unsigned int mxSkeleton::GetNumberOfNodesInTotal()
{
	unsigned int n=0;
	for(unsigned int i=0; i<m_data.GetNumberOfElements(); i++)
	{ n += m_data[i].GetNumberOfNodes(); }
	return n;
}


unsigned int mxSkeleton::GetNumberOfNodes(unsigned int t)
{
	return(m_data[t].m_nodes.GetNumberOfElements());
}


////mxList<mxString>* mxSkeleton::GetScalarTags()
////{
////    return &m_scalar_tags;
////}
////
////
////mxList<unsigned int>* mxSkeleton::GetScalarTagIndexes()
////{
////    return &m_scalar_tags_indexes;
////}
//
//
//unsigned int mxSkeleton::GetNumberOfScalars()
//{
//    return m_scalar_tags.GetNumberOfElements();
//}
//
//
//unsigned int mxSkeleton::GetMaximumNumberOfScalars()
//{
//    return m_maximum_number_of_scalars;
//}


double* mxSkeleton::GetOrigin()
{
	return m_origin;
}


double mxSkeleton::GetOrigin_T()
{
	return m_origin[3];
}


double mxSkeleton::GetOrigin_S()
{
	return m_origin[2];
}


double mxSkeleton::GetOrigin_R()
{
	return m_origin[1];
}


double mxSkeleton::GetOrigin_C()
{
	return m_origin[0];
}


void mxSkeleton::SetOrigin(double t, double s, double r, double c)
{
	m_origin[0] = c; m_origin[1] = r; m_origin[2] = s; m_origin[3] = t;
}


void mxSkeleton::SetOrigin(double s, double r, double c)
{
	m_origin[0] = c; m_origin[1] = r; m_origin[2] = s;
}


double* mxSkeleton::GetSpacing()
{
	return m_spacing;
}


double mxSkeleton::GetSpacing_T()
{
	return m_spacing[3];
}


double mxSkeleton::GetSpacing_S()
{
	return m_spacing[2];
}


double mxSkeleton::GetSpacing_R()
{
	return m_spacing[1];
}


double mxSkeleton::GetSpacing_C()
{
	return m_spacing[0];
}


int mxSkeleton::SetSpacing(double t, double s, double r, double c)
{
	if(t<=0 || s<=0 || r<=0 || c<=0) return 0;
	m_spacing[0] = c; m_spacing[1] = r; m_spacing[2] = s; m_spacing[3] = t;
	return 1;
}


int mxSkeleton::SetSpacing(double s, double r, double c)
{
	if(s<=0 || r<=0 || c<=0) return 0;
	m_spacing[0] = c; m_spacing[1] = r; m_spacing[2] = s;
	return 1;
}


void mxSkeleton::LocalPositionFromWorldPosition(double world_x, double world_y, double world_z, double &output_local_c, double &output_local_r, double &output_local_s)
{
    // world_coordinates = origin + orientation_matrix * (local_position * spacing).
    // we assume orientation is 1, so: world = origin + local*spacing.
    
    double lx = world_x - this->GetOrigin_C();
    double ly = world_y - this->GetOrigin_R();
    double lz = world_z - this->GetOrigin_S();
    
    output_local_c = lx / this->GetSpacing_C();
    output_local_r = ly / this->GetSpacing_R();
    output_local_s = lz / this->GetSpacing_S();
}


void mxSkeleton::WorldPositionFromLocalPosition(double local_c, double local_r, double local_s, double &output_world_x, double &output_world_y, double &output_world_z)
{
    // world_coordinates = origin + orientation_matrix * (local_position * spacing).
    // we assume orientation is 1, so: world = origin + local*spacing.
    
    double lx = local_c * this->GetSpacing_C();
    double ly = local_r * this->GetSpacing_R();
    double lz = local_s * this->GetSpacing_S();
    
    output_world_x = this->GetOrigin_C() + lx;
    output_world_y = this->GetOrigin_R() + ly;
    output_world_z = this->GetOrigin_S() + lz;
    
}


void mxSkeleton::SetVisualizationPropertiesAs(mxSkeleton &input)
{
	this->SetSpacing(input.m_spacing[3], input.m_spacing[2] ,input.m_spacing[1], input.m_spacing[0]);
	this->SetOrigin(input.m_origin[3], input.m_origin[2], input.m_origin[1], input.m_origin[0]);
}


int mxSkeleton::GetScalarTagIndex(mxString &tag, unsigned int &output_index)
{
    output_index = 0;
    for(unsigned int i=0; i<this->GetMaximumNumberOfScalars(); i++)
    {
        if(*(m_scalars[i].GetTag()) == tag)
        {
            output_index = i;
            return 1;
        }
    }
    return 0;
    
//    unsigned int local_index;
//    if(!mxListFunctions::GetIndexOfElement(m_scalar_tags,tag,local_index)) return 0;
//    output_index = m_scalar_tags_indexes[local_index];
//    return 1;
}


int mxSkeleton::AddScalarTag(mxString &tag, unsigned int &output_index)
{
    // Find the lowest available index to add a tag.
    for(int i=0; i<this->GetMaximumNumberOfScalars(); i++)
    {
        if(!m_scalars[i].IsUsed())
        {
            m_scalars[i].GetTag()->Assign(tag);
            m_scalars[i].m_is_used = 1;
            output_index = i;
            return 1;
        }
    }
    return 0;
    
//    //Find the lowest available index to add a tag.
//    output_index = -1;// Initialize with error value.
//    for(unsigned int i=0; i<this->GetMaximumNumberOfScalars(); i++)
//    {
//        if(!mxListFunctions::HasElement(m_scalar_tags_indexes,i))
//        {
//            output_index = (int) i;
//            break;
//        }
//    }
//    if(output_index==-1) return 0;
//    m_scalar_tags_indexes.AddToEnd(output_index);
//    m_scalar_tags.AddToEnd(tag);
//
//    return 1;
}


mxSkeletonLink* mxSkeleton::AddLink(unsigned int t)
{
	mxSkeletonLink *psl;
	psl = m_data[t].m_links.AddNewToEnd();
	psl->SetSkeleton(this);
	psl->SetSkeleton3D(&(m_data[t]));
	return psl;
}


mxSkeletonLink* mxSkeleton::AddLink(mxSkeleton3D *skeleton_3D)
{
	mxSkeletonLink *psl;
	psl = skeleton_3D->m_links.AddNewToEnd();
	psl->SetSkeleton(this);
	psl->SetSkeleton3D(skeleton_3D);
	return psl;
}



int mxSkeleton::NumberOfStubLinks(unsigned int t)
{
	if(this->GetNumberOfLinks(t)==1) return 1; 

	int n = 0;
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		if(psn->GetNumberOfLinks()==1) n++;
	}
	return n;
}


int mxSkeleton::IsNumberOfStubLinksGreaterThan(int n, unsigned int t)
{
	if(n<1) return 1;
	if(this->GetNumberOfLinks(t)==1) return 0;

	int j = 0;
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		if(psn->GetNumberOfLinks()==1) j++;
		if(j>n) return 1;
	}
	return 0;
}


int mxSkeleton::IsNumberOfStubOrFreeNodesGreaterThan(int n, unsigned int t)
{
	if(n<1) return 1;
	if(this->GetNumberOfNodes(t)==1) return 0;

	int j = 0;
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		if(psn->GetNumberOfLinks()<=1) j++;
		if(j>n) return 1;
	}
	return 0;
}


mxSkeleton& mxSkeleton::operator =(mxSkeleton &s)
{
	if(&s==this) return *this;
	this->CopyFrom(s);
	return *this;
}


void mxSkeleton::SetPropertiesAs(mxSkeleton &input)
{
	m_origin[0] = input.m_origin[0];
    m_origin[1] = input.m_origin[1];
	m_origin[2] = input.m_origin[2];
    m_origin[3] = input.m_origin[3];
	m_spacing[0] = input.m_spacing[0];
    m_spacing[1] = input.m_spacing[1];
	m_spacing[2] = input.m_spacing[2];
    m_spacing[3] = input.m_spacing[3];
}


void mxSkeleton::SetScalarTagsAs(mxSkeleton &input)
{
    for(unsigned int i=0; i<this->GetMaximumNumberOfScalars(); i++)
    {
        m_scalars[i].Copy(input.GetScalarTagElement(i));
    }
    
//    m_scalar_tags = input.m_scalar_tags;
//    m_scalar_tags_indexes = input.m_scalar_tags_indexes;
}


int mxSkeleton::IsEquivalentLinkInSkeleton(mxSkeletonLink *psl, unsigned int t)
{
	if(this->IsEmpty()) return 0;

	mxSkeletonLink *p_link;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t].m_links); it.IsValid(); it.MoveToNext())
	{
		p_link = it.GetElementAddress();
		if(psl->IsEqualTo(p_link)) return 1;
	}
	return 0;
}


int mxSkeleton::IsLinkInSkeleton(mxSkeletonLink *psl, unsigned int t)
{
	if(m_data[t].m_links.IsEmpty()) return 0;

	mxSkeletonLink *p_link;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t].m_links); it.IsValid(); it.MoveToNext())
	{
		p_link = it.GetElementAddress();
		if(psl==p_link) return 1;
	}
	return 0;
}


int mxSkeleton::IsNodeInSkeleton(mxSkeletonNode *psn, unsigned int t)
{
	if(psn==NULL) return 0;
	if(m_data[t].m_nodes.IsEmpty()) return 0;

	mxSkeletonNode *p_node;
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		p_node = it.GetElementAddress();
		if(psn==p_node) return 1;
	}
	return 0;
}


int mxSkeleton::IsLinkConnectingGivenNodes(mxSkeletonNode *psn1, mxSkeletonNode *psn2)
{
	if(psn1==NULL || psn2==NULL) return 0;

	mxListIterator<mxSkeletonLink*> itl;
	for(itl.SetToBegin(psn1->GetLinksList()); itl.IsValid(); itl.MoveToNext())
	{
		mxSkeletonLink *psl = itl.GetElement();
		mxSkeletonNode *p_sn1 = psl->GetNode_1();
		mxSkeletonNode *p_sn2 = psl->GetNode_2();
		if((p_sn1==psn1 && p_sn2==psn2) || (p_sn1==psn2 && p_sn2==psn1)) { return 1; }
	}
	return 0;
}


int mxSkeleton::IsPathConnectingGivenNodes(mxSkeletonNode *psn1, mxSkeletonNode *psn2)
{
	if(!psn1 || !psn2) return 0;
	if(psn1->GetSkeleton() != this || psn2->GetSkeleton() != this) return 0;
	if(psn1->GetSkeleton3D() != psn2->GetSkeleton3D()) return 0;
	mxSkeleton3D *skl3D = psn1->GetSkeleton3D();

	//We will use metric values to grow skeleton region from the starting node. First, we will back up the
	// existing metrics (in case some other function wants to use them later).
	mxList<double> node_metrics;
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(skl3D->m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		node_metrics.AddToEnd(psn->GetMetricValue());
	}
	if(node_metrics.GetNumberOfElements()!=skl3D->m_nodes.GetNumberOfElements()) 
	{
		std::cout<<"mxSkeleton::IsPathConnectingGivenNodes(): Error when saving node metrics!"<<std::endl;
		return 0;
	}

	//Now reset all the node metrics to 0 (this will indicate that the region was not yet grown to this node),
	// except the starting node which we set to 1.
	for(it.SetToBegin(skl3D->m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		psn->GetMetricValue() = 0;
	}
	psn1->GetMetricValue() = 1;

	//Do the region growing
	mxList<mxSkeletonNode*> l;
	l.AddToBegin(psn1);
	mxSkeletonLink *psl;
	int is_path_found = 0;
	while((!is_path_found) && (!l.IsEmpty()))
	{
		psn = l.GetBeginElement();
		
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(psn->GetLinksList()); itl.IsValid(); itl.MoveToNext())
		{
			psl = itl.GetElement();
			mxSkeletonNode *psnn = psl->GetNode_2();
			if(psnn==psn) psnn = psl->GetNode_1();

			if(psnn == psn2)//If the destination node is found
			{
				is_path_found = 1;
				break;
			}
			else
			{
				if(psnn->GetMetricValue()==0)
				{
					psnn->GetMetricValue() = 1;
					l.AddToEnd(psnn);
				}
			}
		}
		l.DeleteBegin();
	}

	//Restore all the metrics before exiting the function
	for(it.SetToBegin(skl3D->m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		psn->GetMetricValue() = node_metrics.GetBeginElement();
		node_metrics.DeleteBegin();
	}

	return is_path_found;
}


int mxSkeleton::IsEveryLinkConnectedToOneNode(unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(m_data[t].m_links.GetNumberOfElements()==1)  return 1;

	//Find the node that has more than one link connected to it
	mxSkeletonNode *psn = NULL;
	int is_connecting_node_found = 0;
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid() && !is_connecting_node_found; it.MoveToNext())
	{
		psn = it.GetElementAddress();
		if(psn->GetNumberOfLinks()>1) is_connecting_node_found = 1;
	}
	//Now check if all the links are connected to found node psn
	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> it2;
	for(it2.SetToBegin(m_data[t].m_links); it2.IsValid(); it2.MoveToNext())
	{
		psl = it2.GetElementAddress();
		mxSkeletonNode *psn2 = psl->GetNode_2();
		if(psn!=psn2)
		{
			psn2 = psl->GetNode_1();
			//If a link is not connected to psn, return 0.
			if(psn!=psn2) return 0;
		}
	}

	//If the previous check was passed, it means that all the links are connected to one node - return true
	return 1;
}


int mxSkeleton::GetBounds(mxPoint &min, mxPoint &max, unsigned int t)
{
	if(this->IsEmpty()) return 0;

	min = m_data[t].m_nodes.GetBeginElement().GetPosition();
	max = min;

	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> itn;
	for(itn.SetToBegin(m_data[t].m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		if(psn->GetPosition().S() > max.S()) max.S() = psn->GetPosition().S();
		if(psn->GetPosition().R() > max.R()) max.R() = psn->GetPosition().R();
		if(psn->GetPosition().C() > max.C()) max.C() = psn->GetPosition().C();
		if(psn->GetPosition().S() < min.S()) min.S() = psn->GetPosition().S();
		if(psn->GetPosition().R() < min.R()) min.R() = psn->GetPosition().R();
		if(psn->GetPosition().C() < min.C()) min.C() = psn->GetPosition().C();
	}

	mxPoint *ppos;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(m_data[t].m_links); itl.IsValid(); itl.MoveToNext())
	{
		mxListIterator<mxPoint> itp;
		for(itp.SetToBegin(itl.GetElementAddress()->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
		{
			ppos = itp.GetElementAddress();
			if(ppos->S() > max.S()) max.S() = ppos->S();
			if(ppos->R() > max.R()) max.R() = ppos->R();
			if(ppos->C() > max.C()) max.C() = ppos->C();
			if(ppos->S() < min.S()) min.S() = ppos->S();
			if(ppos->R() < min.R()) min.R() = ppos->R();
			if(ppos->C() < min.C()) min.C() = ppos->C();
		}
	}

	return 1;
}


int mxSkeleton::GetLinkPointerForGivenPort(mxPoint &port, mxSkeletonLink **output_link_pointer, unsigned int t)
{
	if(m_data[t].m_links.IsEmpty())
	{
		*output_link_pointer = NULL;
		return 0;
	}

	mxPoint pos;
	int port_found = 0;
	//Go through all the links of the skeleton
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t].m_links); it.IsValid(); it.MoveToNext())
	{
		*output_link_pointer = it.GetElementAddress();
		//Check the first port of the link - if port is found return 1 (the link is already in the output_link)
		if((*output_link_pointer)->GetPositionsList()->IsEmpty()) return 0;

		pos = (*output_link_pointer)->GetPositionsList()->GetBeginElement();
		if(pos.IsEqualPositionIn_3D(port))
		{
			port_found = 1;
			break;
		}
		//Check the second port of the link 
		pos = (*output_link_pointer)->GetPositionsList()->GetEndElement();
		if(pos.IsEqualPositionIn_3D(port))
		{
			port_found = 1;
			break;
		}
	}

	//If port was found return 1 (the link is already in the output_link)
	if(port_found) return 1;
	
	//If the port was not found, set link to NULL and return 0
	*output_link_pointer = NULL;
	return 0;
}


int mxSkeleton::GetLinkWithHighestMetricConnetingTwoNodes(mxSkeletonNode *psn1, mxSkeletonNode *psn2, mxSkeletonLink **output_psl)
{
	if(psn1->GetLinksList()->GetNumberOfElements()==0 || psn2->GetLinksList()->GetNumberOfElements()==0) return 0;

	int is_link_found = 0;
	if(psn1->GetLinksList()->GetNumberOfElements()<psn2->GetLinksList()->GetNumberOfElements())
	{
		mxSkeletonLink *psl;
		double max = -1;
		mxListIterator<mxSkeletonLink*> it;
		for(it.SetToBegin(psn1->GetLinksList()); it.IsValid(); it.MoveToNext())
		{
			psl = it.GetElement();
			mxSkeletonNode *psnn1 = psl->GetNode_1();
			mxSkeletonNode *psnn2 = psl->GetNode_2();
			if( (psnn1==psn1 && psnn2==psn2) || (psnn1==psn2 && psnn2==psn1) )
			{
				if(psl->GetCost()>max)
				{
					max= psl->GetCost();
					*output_psl = psl;
					is_link_found = 1;
				}
			}
		}
	}
	else
	{
		mxSkeletonLink *psl;
		double max = -1;
		mxListIterator<mxSkeletonLink*> it;
		for(it.SetToBegin(psn1->GetLinksList()); it.IsValid(); it.MoveToNext())
		{
			psl = it.GetElement();
			mxSkeletonNode *psnn1 = psl->GetNode_1();
			mxSkeletonNode *psnn2 = psl->GetNode_2();
			if( (psnn1==psn1 && psnn2==psn2) || (psnn1==psn2 && psnn2==psn1) )
			{
				if(psl->GetCost()>max)
				{
					max= psl->GetCost();
					*output_psl = psl;
					is_link_found = 1;
				}
			}
		}
	}

	return is_link_found;
}



int mxSkeleton::GetNodeForPosition(mxPoint &position, mxSkeletonNode **output_node_pointer, unsigned int t)
{
	if(m_data[t].m_nodes.IsEmpty())
	{
		*output_node_pointer = NULL;
		return 0;
	}

	mxPoint pos;
	//Go through all the nodes of the skeleton
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		*output_node_pointer = it.GetElementAddress();
		//Check the position of the node - if node is found return 1 (the node is in the node list of the skeleton)
		pos = (*output_node_pointer)->GetPosition();
		if(pos.IsEqualPositionIn_3D(position)) return 1;
	}
	
	//If the node was not found, set pointer to NULL and return 0
	*output_node_pointer = NULL;
	return 0;
}


int mxSkeleton::GetNodeWithTheMostLinks(mxSkeletonNode **output_node_pointer, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	int max_n_of_links = -1;	
	mxListIterator<mxSkeletonNode> itn;
	for(itn.SetToBegin(this->GetNodes(t)); itn.IsValid(); itn.MoveToNext())
	{
		mxSkeletonNode *psn = itn.GetElementAddress();
		if(psn->GetNumberOfLinks()>max_n_of_links)
		{
			max_n_of_links = psn->GetNumberOfLinks();
			*output_node_pointer = psn;
		}
	}
	return 1;
}


int mxSkeleton::GetNodeClosestToPosition(mxPoint &input_position, mxSkeletonNode **output_closest_node, unsigned int t)
{
	if(m_data[t].m_nodes.IsEmpty()) return 0;

	mxSkeletonNode *psn;
	double distance, min_distance;
	
	//Initialize
	min_distance = m_data[t].m_nodes.GetBeginElement().GetPosition().DistanceEuclidean(input_position);
	*output_closest_node = &(m_data[t].m_nodes.GetBeginElement());

	//Search
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		distance = input_position.DistanceEuclidean(psn->GetPosition());
		if(distance<min_distance) 
		{
			min_distance = distance;
			*output_closest_node = psn;
		}
	}

	return 1;
}


int mxSkeleton::GetLinkClosestToPosition(mxPoint &input_position, mxSkeletonLink **output_closest_link, unsigned int t)
{
	if(m_data[t].m_links.IsEmpty()) return 0;

	mxSkeletonLink *psl;
	mxPoint *ppos;
	double distance, min_distance;
	
	//Initialize
	if(m_data[t].m_links.GetBeginElement().GetPositionsList()->IsEmpty())
	{
		mxPoint p;
		p.S() = ( m_data[t].m_links.GetBeginElement().GetNode_2_Position()->S() + m_data[t].m_links.GetBeginElement().GetNode_1_Position()->S() ) / 2.0;
		p.R() = ( m_data[t].m_links.GetBeginElement().GetNode_2_Position()->R() + m_data[t].m_links.GetBeginElement().GetNode_1_Position()->R() ) / 2.0;
		p.C() = ( m_data[t].m_links.GetBeginElement().GetNode_2_Position()->C() + m_data[t].m_links.GetBeginElement().GetNode_1_Position()->C() ) / 2.0;
		min_distance = p.DistanceEuclidean(input_position);
		*output_closest_link = &(m_data[t].m_links.GetBeginElement());
	}
	else
	{
		min_distance = m_data[t].m_links.GetBeginElement().GetPositionsList()->GetBeginElement().DistanceEuclidean(input_position);
		*output_closest_link = &(m_data[t].m_links.GetBeginElement());
	}

	//Search each link....
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t].m_links); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElementAddress();

		if(psl->GetPositionsList()->IsEmpty())
		{
			mxPoint p;
			p.S() = (psl->GetNode_2_Position()->S() + psl->GetNode_1_Position()->S() ) / 2.0;
			p.R() = (psl->GetNode_2_Position()->R() + psl->GetNode_1_Position()->R() ) / 2.0;
			p.C() = (psl->GetNode_2_Position()->C() + psl->GetNode_1_Position()->C() ) / 2.0;
			distance = p.DistanceEuclidean(input_position);
			if(distance<min_distance) 
			{
				min_distance = distance;
				*output_closest_link = psl;
			}
		}
		else
		{
			//... and each position in the link...
			mxListIterator<mxPoint> itp;
			for(itp.SetToBegin(psl->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
			{
				ppos = itp.GetElementAddress();
				//... for the minimum distance of positions to the input position
				distance = input_position.DistanceEuclidean(*ppos);
				if(distance<min_distance) 
				{
					min_distance = distance;
					*output_closest_link = psl;
				}
			}
		}
	}

	return 1;
}


mxSkeletonLink* mxSkeleton::GetLinkOfMaxLenghtAdjacentToNode(mxSkeletonNode *node)
{
	double max_lenght = 0;
	mxSkeletonLink *max_length_link = NULL;
	//Search each link....
	mxListIterator<mxSkeletonLink*> it;
	for(it.SetToBegin(node->GetLinksList()); it.IsValid(); it.MoveToNext())
	{
		double l = it.GetElement()->LengthOfLink();
		if(l > max_lenght)
		{
			max_lenght = l;
			max_length_link = it.GetElement();
		}
	}
	return max_length_link;
}


int mxSkeleton::GetScalarMinAndMaxValue(unsigned int index, double &output_min, double &output_max, unsigned int t)
{
	if(m_data[t].m_links.IsEmpty()) return 0;
	if(index>=this->GetMaximumNumberOfScalars()) return 0;
	mxSkeletonLink *psl;
	output_max = 0; 
	output_min = -1;
	double mmin, mmax;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t].m_links); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElementAddress();
		psl->GetScalarMinAndMaxValue(index, mmin, mmax);
		if(mmax>output_max) output_max = mmax;
		if(mmin<output_min || output_min<0) output_min = mmin;
	}
	return 1;
}



int mxSkeleton::GetLinkLength(mxSkeletonLink *p_input_skeleton_link, double *distance)
{
	if(p_input_skeleton_link==NULL) return 0;
	if(p_input_skeleton_link->GetSkeleton()!=this) return 0;
	
	*distance = 0;

	mxPoint *ppos, *ppos_previous;
	ppos_previous = p_input_skeleton_link->GetNode_1_Position();
	mxListIterator<mxPoint> itp;
	for(itp.SetToBegin(p_input_skeleton_link->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
	{
		ppos = itp.GetElementAddress();
		*distance += ppos->DistanceEuclidean(*ppos_previous);
		ppos_previous = ppos; 
	}

	*distance += ppos_previous->DistanceEuclidean(*(p_input_skeleton_link->GetNode_2_Position()));

	return 1;
}


double mxSkeleton::GetLinkLength(mxSkeletonLink *p_input_skeleton_link)
{	
	double distance = 0;
	mxPoint *ppos, *ppos_previous;
	ppos_previous = p_input_skeleton_link->GetNode_1_Position();
	mxListIterator<mxPoint> itp;
	for(itp.SetToBegin(p_input_skeleton_link->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
	{
		ppos = itp.GetElementAddress();
		distance += ppos->DistanceEuclidean(*ppos_previous);
		ppos_previous = ppos; 
	}
	distance += ppos_previous->DistanceEuclidean(*(p_input_skeleton_link->GetNode_2_Position()));
	return distance;
}


double mxSkeleton::LengthOfSkeleton(unsigned int t)
{	
	double distance = 0;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t].m_links); it.IsValid(); it.MoveToNext())
	{ distance += it.GetElementAddress()->LengthOfLink(); }
	return distance;
}


int mxSkeleton::GetListOfStubNodes(mxList<mxSkeletonNode*> &list_of_stub_node_pointers, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	list_of_stub_node_pointers.Reset();
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		mxSkeletonNode *psn = it.GetElementAddress();
		if(psn->GetNumberOfLinks()==1) list_of_stub_node_pointers.AddToEnd(psn);
	}
	return 1;
}


int mxSkeleton::GetListOfStubAndFreeNodes(mxList<mxSkeletonNode*> &list_of_stub_and_free_node_pointers, unsigned int t)
{
	if(this->GetNumberOfNodes()==0) return 0;
	list_of_stub_and_free_node_pointers.Reset();
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		mxSkeletonNode *psn = it.GetElementAddress();
		if(psn->GetNumberOfLinks()<=1) list_of_stub_and_free_node_pointers.AddToEnd(psn);
	}
	return 1;
}


int mxSkeleton::CreateSkeletonizationVoxelsFromPositions(unsigned int scalar_index, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(scalar_index>=this->GetMaximumNumberOfScalars()) scalar_index = 0;

	mxPoint bounds_min, bounds_max;
	this->GetBounds(bounds_min, bounds_max, t);

	//Since the GetBounds method analysis all the positions in links and nodes, if any of the returned values
	// are less than 0, return 0! (because we will no be able to store these positions in the image- skeletonization points).
	if(bounds_min.S()<0 || bounds_min.R()<0 || bounds_min.C()<0) return 0;

	mxGeometry g;
	g.SetDimensions(((int)(bounds_max.S()+1)),((int)(bounds_max.R()+1)),((int)(bounds_max.C()+1)));

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t].m_links); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElementAddress();

		//Erase any existing skeletonization points
		psl->GetSkeletonizationVoxels()->Reset();

		mxArray<mxPoint*> a;//This is the array of positions including node positions
		psl->GetArrayOfPositionPointersIncludingNodes(a);

		for(unsigned int i=0; i<a.GetNumberOfElements()-1; i++)
		{
			if(i>0)//Do for all points except the first (it's a node- the last position is also not entered, because it's also a node)
			{
				mxVoxel *pp;
				pp = psl->GetSkeletonizationVoxels()->AddNewToEnd();

				pp->SetIndex((int)(a[i]->S()), (int)(a[i]->R()), (int)(a[i]->C()));
				pp->V() = (unsigned int) a[i]->V(scalar_index);
			}
			mxVoxel p1,p2;
			p1.SetIndex((int)(a[i]->S()), (int)(a[i]->R()), (int)(a[i]->C()));
			p2.SetIndex((int)(a[i+1]->S()), (int)(a[i+1]->R()), (int)(a[i+1]->C()));

            
            int s,r,c;
            for(g.ForLine(p1.S(),p1.R(),p1.C(), p2.S(),p2.R(),p2.C()); g.GetLine(s,r,c); )
            {
                mxVoxel *pp = psl->GetSkeletonizationVoxels()->AddNewToEnd();
                
                pp->SetIndex(s,r,c);
                pp->V() = (unsigned int) a[i]->V(scalar_index);
            }
		}
	}

	return 1;
}


void mxSkeleton::CorrectLinks()
{
	if(this->IsEmpty()) return;

	for(unsigned int i=0; i<m_data.GetNumberOfElements(); i++)
	{
		mxListIterator<mxSkeletonLink> itl;
		for(itl.SetToBegin(m_data[i].m_links); itl.IsValid(); itl.MoveToNext())
		{
			itl.GetElementAddress()->CorrectLink();
		}
	}
}


int mxSkeleton::AddNodeWithPosition(mxPoint &node_pos, mxSkeletonNode **output_added_node, unsigned int t)
{
	mxSkeletonNode *psn;

	//If the node with this position already exists, return 0 without adding new node, but set the output pointer to the existing node
	if(this->GetNodeForPosition(node_pos,&psn,t)) 
	{
		*output_added_node = psn;
		return 0;
	}

	//Make a new node and set its position
	*output_added_node = m_data[t].m_nodes.AddNewToEnd();
	(*output_added_node)->GetPosition() = node_pos;
	(*output_added_node)->SetSkeleton(this);
	(*output_added_node)->SetSkeleton3D(&(m_data[t]));

	return 1;
}


void mxSkeleton::AddLinkBetweenTwoExistingNodes(mxSkeletonNode *pnode1, mxSkeletonNode *pnode2, mxSkeletonLink **output_link_pointer)
{
	//For Debug
	if(pnode1==NULL || pnode2==NULL) return;
	if(pnode1->GetSkeleton3D() != pnode2->GetSkeleton3D()) return;

	mxSkeleton3D *skl3D = pnode1->GetSkeleton3D();
	
	//Create a new link
	*output_link_pointer = this->AddLink(skl3D);

	//Set the pointers to nodes in the link and their positions
	//(mxSkeletonNode *)((*output_link_pointer)->GetNode_1());
	(*output_link_pointer)->SetNode_1(pnode1);

	//(mxSkeletonNode *)((*output_link_pointer)->m_node2);
	(*output_link_pointer)->SetNode_2(pnode2);

	//Add the pointer to the link into both nodes
	pnode1->GetLinksList()->AddToEnd(*output_link_pointer);
	pnode2->GetLinksList()->AddToEnd(*output_link_pointer);
}



int mxSkeleton::RemoveLink(mxSkeletonLink *psl)
{
	//If wrong input parameter, return 0
	if(psl==NULL) return 0;
	if(psl->GetSkeleton()!=this) return 0;
	if(psl->GetSkeleton3D()==NULL) return 0;

	mxSkeleton3D *skl3D = psl->GetSkeleton3D();

	//Get the double list node in which the link is contained (because the node has to be deleted too)
	mxListNode<mxSkeletonLink> *p_dln_sl;
	p_dln_sl = skl3D->m_links.GetNodeWithElementAddress(psl);

	//Get the start and end node of the link
	
	mxSkeletonNode *p_end_node = psl->GetNode_2();
	mxSkeletonNode *p_start_node = psl->GetNode_1();

	//Delete link positions
	psl->Reset();//delete the links separately (not needed, but in any case...)
	//Delete the list node from the list
	skl3D->m_links.DeleteNode(p_dln_sl);

	//Now we have to check the nodes and delete the pointer to link from the list of links attached to nodes

	//----- Start Node -----

	//If only one link was attached to the node, delete the node
	if(p_start_node->GetLinksList()->GetNumberOfElements()<=1)
	{
		mxListNode<mxSkeletonNode> *p_dln_sn;
		p_dln_sn = skl3D->m_nodes.GetNodeWithElementAddress(p_start_node);
		skl3D->m_nodes.DeleteNode(p_dln_sn);
	}
	//If there were more links attached to the node...
	else
	{
		//...find the link that has to be removed
		mxSkeletonLink **p_sl_temp;
		mxListNode<mxSkeletonLink*> *p_dln_p_sl = NULL;
		int is_link_found = 0;
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(p_start_node->GetLinksList()); itl.IsValid(); itl.MoveToNext())
		{
			p_sl_temp = itl.GetElementAddress();
			p_dln_p_sl = itl.GetNode();
			//...if link found
			if((*p_sl_temp)==psl)
			{
				is_link_found = 1;
				break;
			}
		}

		if(!is_link_found) return 0;

		p_start_node->GetLinksList()->DeleteNode(p_dln_p_sl);
	}


	//----- End Node -----

	//If only one link was attached to the node, delete the node
	if(p_end_node->GetLinksList()->GetNumberOfElements()<=1)
	{
		mxListNode<mxSkeletonNode> *p_dln_sn;
		p_dln_sn = skl3D->m_nodes.GetNodeWithElementAddress(p_end_node);
		skl3D->m_nodes.DeleteNode(p_dln_sn);
	}
	//If there were more links attached to the node...
	else
	{
		//...find the link that has to be removed
		mxSkeletonLink **p_sl_temp;
		mxListNode<mxSkeletonLink*> *p_dln_p_sl = NULL;
		int is_link_found = 0;
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(p_end_node->GetLinksList()); itl.IsValid(); itl.MoveToNext())
		{
			p_sl_temp = itl.GetElementAddress();
			p_dln_p_sl = itl.GetNode();
			//...if link found
			if((*p_sl_temp)==psl)
			{
				is_link_found = 1;
				break;
			}
		}

		if(!is_link_found) return 0;

		p_end_node->GetLinksList()->DeleteNode(p_dln_p_sl);
	}

	return 1;
}


int mxSkeleton::RemoveLinkWithMerging(mxSkeletonLink *psl)
{
	if(psl==NULL) return 0;
	if(psl->GetSkeleton()!=this) return 0;
	mxSkeleton3D *skl3D = psl->GetSkeleton3D();
	if(!skl3D) return 0;

	mxSkeletonNode *psn1 = psl->GetNode_1();
    mxSkeletonNode *psn2 = psl->GetNode_2();//m_node1;

	//In case the link is connected with both ends to the same node
	if(psn1==psn2)
	{
		//If the node will have 2 links after deleting this link...
		if(psn1->GetNumberOfLinks()==4)
		{
			//Add points of the link to the node
			psn1->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));

			//... delete the link and perform merging
			this->RemoveLink(psl);
			this->MergeNodeWithLinksForGivenNode(psn1);
		}
		//If the node will not have 2 links after deleting this link...
		else
		{
			//Add points of the link to the node
			psn1->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));

			//... just delete the link
			this->RemoveLink(psl);
		}
	}
	//In case the link is connected to different nodes
	else
	{
		//If both nodes will have 2 links after deleting this link...
		if(psn1->GetNumberOfLinks()==3 && psn2->GetNumberOfLinks()==3)
		{
			//Add points of the link to the node
			psn1->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));

			//... delete the link and perform merging
			this->RemoveLink(psl);
			this->MergeNodeWithLinksForGivenNode(psn1);
			this->MergeNodeWithLinksForGivenNode(psn2);
		}	
		//If the first node will have 2 links after deleting this link...
		if(psn1->GetNumberOfLinks()==3 && psn2->GetNumberOfLinks()!=3)
		{
			//Add points of the link to the node
			psn1->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));

			//... delete the link and perform merging
			this->RemoveLink(psl);
			this->MergeNodeWithLinksForGivenNode(psn1);
		}
		//If the second node will have 2 links after deleting this link...
		if(psn1->GetNumberOfLinks()!=3 && psn2->GetNumberOfLinks()==3)
		{
			//Add points of the link to the node
			psn2->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));

			//... delete the link and perform merging
			this->RemoveLink(psl);
			this->MergeNodeWithLinksForGivenNode(psn2);
		}
		//If neither of nodes will have 2 links after deleting this link
		if(psn1->GetNumberOfLinks()!=3 && psn2->GetNumberOfLinks()!=3)
		{
			//Add points of the link to the node
			psn1->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));

			//... just delete the link
			this->RemoveLink(psl);
		}	
	}

	return 1;
}


int mxSkeleton::RemoveNode(mxSkeletonNode *psn)
{
	//Make a separate list of links connected to the node
	mxList<mxSkeletonLink*> l;
	l = *(psn->GetLinksList());

	//While the list is not empty, do removing
	while(!l.IsEmpty())
	{
		this->RemoveLink(l.GetBeginElement());
		l.DeleteBegin();
	}
	return 1;
}


int mxSkeleton::RemoveSkeletonFromThis(mxSkeleton &skeleton_to_be_removed, mxSkeleton &output_skeleton, unsigned int t1, unsigned int t2)
{
	if(this->IsEmpty()) return 0;
	output_skeleton.ResetTimeIndex(t2);
	if(skeleton_to_be_removed.IsEmpty()) return 1;

	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(m_data[t1].m_links); it.IsValid(); it.MoveToNext())
	{
		int is_link_found = 0;
		mxListIterator<mxSkeletonLink> it2;
		for(it2.SetToBegin(skeleton_to_be_removed.GetLinks(t2)); it2.IsValid(); it2.MoveToNext())
		{
			if(it2.GetElementAddress()->IsEqualTo(it.GetElementAddress()))
			{
				is_link_found = 1;
				break;
			}
		}
		// If the link was not found, add it to the output skeleton
		if(!is_link_found)
		{
			this->CopyLinkWithItsNodesToOutputSkeleton(it.GetElementAddress(), &output_skeleton,t2);
		}
	}

	return 1;
}


int mxSkeleton::MergeNodeWithLinksForGivenNode(mxSkeletonNode *pnode)
{
	if(pnode==NULL) return 0;
	mxSkeleton3D *skl3D = pnode->GetSkeleton3D();
	if(!skl3D) return 0;
	
	//If number of links is not 2, merging can not be performed
	if(pnode->GetNumberOfLinks()!=2) return 0;

	mxSkeletonLink *plink1, *plink2, *pnewlink;
	plink1 = pnode->GetLinksList()->GetBeginElement();
	plink2 = pnode->GetLinksList()->GetEndElement();

	//Create a new link to which the positions from the two links and a node will be copied
	pnewlink = this->AddLink(skl3D);

	//----- First link -----

	//If left end of plink1 is closer to the node...
	if( plink1->GetNode_1()==pnode)
	{
		//Version without copying
		plink1->GetPositionsList()->InvertOrder();
		pnewlink->GetPositionsList()->MergeInputListWithThisFromEnd(*(plink1->GetPositionsList()));
		plink1->GetSkeletonizationVoxels()->InvertOrder();
		pnewlink->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(plink1->GetSkeletonizationVoxels()));

		//... end node will be start node of new link
		pnewlink->SetNode_1(plink1->GetNode_2());
		pnewlink->GetNode_1()->GetLinksList()->AddToEnd(pnewlink);
	}
	//If right end of plink1 is closer to the node...
	else
	{
		//Version without copying			
		pnewlink->GetPositionsList()->MergeInputListWithThisFromEnd(*(plink1->GetPositionsList()));
		pnewlink->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(plink1->GetSkeletonizationVoxels()));

		//... start node will be start node of new link
		pnewlink->SetNode_1(plink1->GetNode_1());
		pnewlink->GetNode_1()->GetLinksList()->AddToEnd(pnewlink);
	}


	//----- Node position -----
	pnewlink->GetPositionsList()->AddToEnd(pnode->GetPosition());

	//Version without copying
	pnewlink->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(pnode->GetSkeletonizationVoxels()));


	//----- Second link -----

	//If left end of plink2 is closer to the node...
    if( plink2->GetNode_1()==pnode)
	{
		//Version without copying
		pnewlink->GetPositionsList()->MergeInputListWithThisFromEnd(*(plink2->GetPositionsList()));
		pnewlink->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(plink2->GetSkeletonizationVoxels()));

		//... end node will be end node of new link
		pnewlink->SetNode_2(plink2->GetNode_2());
		pnewlink->GetNode_2()->GetLinksList()->AddToEnd(pnewlink);
	}
	//If right end of plink2 is closer to the node...
	else
	{

		//Version without copying
		plink2->GetPositionsList()->InvertOrder();
		pnewlink->GetPositionsList()->MergeInputListWithThisFromEnd(*(plink2->GetPositionsList()));
		plink2->GetSkeletonizationVoxels()->InvertOrder();
		pnewlink->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(plink2->GetSkeletonizationVoxels()));

		//... start node will be end node of new link
		pnewlink->SetNode_2(plink2->GetNode_1());
		pnewlink->GetNode_2()->GetLinksList()->AddToEnd(pnewlink);
	}

	//Delete the links
	this->RemoveLink(plink1);
	this->RemoveLink(plink2);
	
	return 1;
}


void mxSkeleton::MergeNodeWithLinksForAllSkeletonNodes()
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		this->MergeNodeWithLinksForAllSkeletonNodes(t);
	}
}


void mxSkeleton::MergeNodeWithLinksForAllSkeletonNodes(unsigned int t)
{
	mxList<mxSkeletonNode*> l;
	mxSkeletonNode *psn;
	
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(m_data[t].m_nodes); it.IsValid(); it.MoveToNext())
	{
		psn = it.GetElementAddress();
		if(psn->GetNumberOfLinks()==2) l.AddToBegin(psn);
	}

	while(!l.IsEmpty())
	{
		psn = l.GetBeginElement();
		this->MergeNodeWithLinksForGivenNode(psn);
		l.DeleteBegin();
	}
}



int mxSkeleton::MergeTwoAdjacentNodesConnectedByGivenLink(mxSkeletonNode *psn1_priority, mxSkeletonNode *psn2, mxSkeletonLink *psl)
{
	mxSkeletonNode *psnn1 = psl->GetNode_1();
	mxSkeletonNode *psnn2 = psl->GetNode_2();
	if(!( (psnn1==psn1_priority && psnn2==psn2) || (psnn2==psn1_priority && psnn1==psn2) )) return 0;
	mxSkeleton3D *skl3D = psl->GetSkeleton3D();
	if(!skl3D) return 0;

	//Copy the points from link psl to node psn1
	while(!psl->GetSkeletonizationVoxels()->IsEmpty())
	{
		psn1_priority->GetSkeletonizationVoxels()->AddToEnd(psl->GetSkeletonizationVoxels()->GetBeginElement());
		psl->GetSkeletonizationVoxels()->DeleteBegin();
	}

	//Copy the points from node psn2 to node psn1
	while(!psn2->GetSkeletonizationVoxels()->IsEmpty())
	{
		psn1_priority->GetSkeletonizationVoxels()->AddToEnd(psn2->GetSkeletonizationVoxels()->GetBeginElement());
		psn2->GetSkeletonizationVoxels()->DeleteBegin();
	}

	//Copy the link pointers from psn2 to psn1 and re-arrange pointers to psn1 in these links
	mxSkeletonLink *psll;
	while(!psn2->GetLinksList()->IsEmpty())
	{
		psll = psn2->GetLinksList()->GetBeginElement();
		if(psll!=psl)
		{
			psn1_priority->GetLinksList()->AddToEnd(psll);
			if(psll->GetNode_1()==psn2) psll->SetNode_1(psn1_priority);
			else psll->SetNode_2(psn1_priority);
		}
		psn2->GetLinksList()->DeleteBegin();
	}

	//Go through the link pointers of psn1_priority and remove pointer to psl
	mxListNode<mxSkeletonLink*> *dln, *dln_to_delete = NULL;
	mxSkeletonLink **ppsll;
	mxListIterator<mxSkeletonLink*> itlp;
	for(itlp.SetToBegin(psn1_priority->GetLinksList()); itlp.IsValid(); itlp.MoveToNext())
	{
		ppsll = itlp.GetElementAddress();
		dln = itlp.GetNode();
		if((*ppsll)==psl) 
		{
			dln_to_delete = dln;
			break;
		}
	}
	psn1_priority->GetLinksList()->DeleteNode(dln_to_delete);

	//Set the position of the psn1 to be in between old psn1 position and psn2 position
	psn1_priority->GetMetricValue() = (psn1_priority->GetMetricValue() + psn2->GetMetricValue() ) / 2;


	//Remove the psn2 from this skeleton
	mxListNode<mxSkeletonNode> *dlsn, *dlsn_to_delete = NULL;
	mxListIterator<mxSkeletonNode> it;
	for(it.SetToBegin(skl3D->m_nodes); it.IsValid(); it.MoveToNext())
	{
		psnn2 = it.GetElementAddress();
		dlsn = it.GetNode();
		if(psnn2==psn2) 
		{
			dlsn_to_delete = dlsn;
			break;
		}
	}
	skl3D->m_nodes.DeleteNode(dlsn_to_delete);

	//Remove the psl from this skeleton
	mxListNode<mxSkeletonLink> *dlsl, *dlsl_to_delete = NULL;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(skl3D->m_links); itl.IsValid(); itl.MoveToNext())
	{
		psll = itl.GetElementAddress();
		dlsl = itl.GetNode();
		if(psll==psl) 
		{
			dlsl_to_delete = dlsl;
			break;
		}
	}
	skl3D->m_links.DeleteNode(dlsl_to_delete);

	return 1;
}


int mxSkeleton::FilterStubBranches(unsigned int threshold_number_of_positions_in_link, unsigned int t)
{
	//If skeleton is empty return success
	if(this->IsEmpty()) return 1;

	mxList<mxSkeletonLink *> list_of_links_to_be_deleted;
	
	mxSkeletonLink *psl;
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		//Check if the link is a part of a stub branch
        psn = psl->GetNode_1();
		if(!psn) return 0;
		int is_stub_branch = 0;
		if(psn->GetLinksList()->GetNumberOfElements()==1) is_stub_branch = 1;
		else
		{
            psn = psl->GetNode_2();
			if(!psn) return 0;
			if(psn->GetLinksList()->GetNumberOfElements()==1) is_stub_branch = 1;
		}
		
		//If stub branch is found...
		if(is_stub_branch)
		{
			//... check if the number of positions in link is equal or smaller than 'threshold_number_of_positions_in_link'
			if(psl->GetPositionsList()->GetNumberOfElements()<=threshold_number_of_positions_in_link)
			{
				//... if it should be deleated add psl to list of links to be deleted and pln to list of list_nodes to be deleted
				list_of_links_to_be_deleted.AddToEnd(psl);
			}
		}
	}

	//Delete the links that are found as parts of stub branches
	while(!list_of_links_to_be_deleted.IsEmpty())
	{
		psl = list_of_links_to_be_deleted.GetBeginElement();

		//Delete the skeleton link
		this->RemoveLinkWithMerging(psl);

		list_of_links_to_be_deleted.DeleteBegin();
	}

	return 1;
}


int mxSkeleton::FilterStubBranches(unsigned int threshold_number_of_positions_in_link)
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		if(!this->FilterStubBranches(threshold_number_of_positions_in_link,t)) return 0;
	}
	return 1;
}


int mxSkeleton::FilterLoops(unsigned int t)
{
	//If skeleton is empty return success
	if(this->IsEmpty()) return 1;

	mxList<mxSkeletonLink *> list_of_links_to_be_deleted;
	

	mxSkeletonLink *psl;
	mxSkeletonNode *psn1, *psn2;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		//If the address of start node and end node are the same...
		psn1 = psl->GetNode_1();
		psn2 = psl->GetNode_2();
		if(psn1==psn2)
		{
			//... add psl to list of links to be deleted
			list_of_links_to_be_deleted.AddToEnd(psl);
		}
	}

	//Delete the links that are found as parts of loop branches
	while(!list_of_links_to_be_deleted.IsEmpty())
	{
		psl = list_of_links_to_be_deleted.GetBeginElement();

		//CHANGE: before deleting the link, copy its points to the node!
		psl->GetNode_1()->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));

		//Delete the skeleton link
        this->RemoveLinkWithMerging(psl);

        list_of_links_to_be_deleted.DeleteBegin();
	}

	return 1;
}


int mxSkeleton::FilterLoops()
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		if(!this->FilterLoops(t)) return 0;
	}
	return 1;
}


int mxSkeleton::FilterMultiplePathsForGivenNodes(mxSkeletonNode *pnode1, mxSkeletonNode *pnode2, int threshold_number_of_positions_in_link)
{
	if(pnode1==NULL || pnode2==NULL) return 0;
	if(threshold_number_of_positions_in_link<1) return 0;
	mxSkeleton3D *skl3D = pnode1->GetSkeleton3D();
	if(!skl3D) return 0;
	if(pnode1->GetSkeleton3D() != pnode2->GetSkeleton3D()) return 0;

	//If the nodes are the same - they belong to the same link, which is a loop, so this should not be processed
	if(pnode1==pnode2) return 0;

	//If one of the nodes has only one link attached to it - this is a stub branch, so it should not be processed
	if(pnode1->GetNumberOfLinks()<=1 || pnode2->GetNumberOfLinks()<=1) return 0;

	//List of pointers to links which we will have to process
	mxList<mxSkeletonLink*> l;

	//Find all links that have to be filtered
	mxSkeletonLink *psl;
	int max_number_of_positions = 0;
	if(pnode1->GetNumberOfLinks()<pnode2->GetNumberOfLinks())//This is just so that we search a node with less links attached
	{
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(pnode1->GetLinksList()); itl.IsValid(); itl.MoveToNext())
		{
			psl = itl.GetElement();
			if((psl->GetNumberOfPositions()<=threshold_number_of_positions_in_link) && 
				((psl->GetNode_1()==pnode1 && psl->GetNode_2()==pnode2) ||
					(psl->GetNode_1()==pnode2 && psl->GetNode_2()==pnode1)))
			{
					l.AddToBegin(psl);
					if(max_number_of_positions<psl->GetNumberOfPositions()) max_number_of_positions = psl->GetNumberOfPositions();
			}
		}
	}
	else
	{
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(pnode2->GetLinksList()); itl.IsValid(); itl.MoveToNext())
		{
			psl = itl.GetElement();
			if((psl->GetNumberOfPositions()<=threshold_number_of_positions_in_link) && 
				((psl->GetNode_1()==pnode1 && psl->GetNode_2()==pnode2) ||
					(psl->GetNode_1()==pnode2 && psl->GetNode_2()==pnode1)))
			{
					l.AddToBegin(psl);
					if(max_number_of_positions<psl->GetNumberOfPositions()) max_number_of_positions = psl->GetNumberOfPositions();
			}
		}
	}

	//If the list 'l' is empty or has one link - no links have to be processed, return 0 (means nothing has been done)
	if(l.GetNumberOfElements()<=1) return 0;

	//For all the found links, put them in the same orientation (pnode1 is start node, pnode2 end node),
	// and sample them to have the maximum number of positions found in all of the links

	mxListIterator<mxSkeletonLink*> itl;
	for(itl.SetToBegin(l); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElement();
		if(psl->GetNode_1()!=pnode1) { psl->InvertPositionOder(); }

		psl->Sample(max_number_of_positions);
	}


	mxArray<mxPoint> temp;
	temp.SetNumberOfElements(max_number_of_positions);
	//This is not needed because all values are initialized with 0, but just in case...
	for(unsigned int i=0; i<temp.GetNumberOfElements(); i++) temp[i].S() = temp[i].R() = temp[i].C() = 0;

	//For all the found links add their corresponding positions
	mxList<mxVoxel> list_of_points_for_the_new_link;
	for(itl.SetToBegin(l); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElement();
		int i=0;
		while(!(psl->GetPositionsList()->IsEmpty()))
		{
			temp[i].Addition(psl->GetPositionsList()->GetBeginElement().S(), psl->GetPositionsList()->GetBeginElement().R(), psl->GetPositionsList()->GetBeginElement().C());
			i++;
			psl->GetPositionsList()->DeleteBegin();
		}

		//Add the list of points into the new list (we generate the list of all the points for all the links)
		list_of_points_for_the_new_link.MergeInputListWithThisFromEnd(*(psl->GetSkeletonizationVoxels()));
	}

	//Make an average of calculated positions
	for(unsigned int i=0; i<temp.GetNumberOfElements(); i++) temp[i].Division(l.GetNumberOfElements());

	//Make a new link, enter the calculated positions, set start and end node
	mxSkeletonLink *psl_new;
	psl_new = AddLink(skl3D);
	psl_new->GetSkeletonizationVoxels()->MergeInputListWithThisFromEnd(list_of_points_for_the_new_link);//Put the list of all points into the link
	for(unsigned int i=0; i<temp.GetNumberOfElements(); i++) 
	{
		psl_new->GetPositionsList()->AddToEnd(temp[i]);
	}
	psl_new->SetNode_1(pnode1);
	psl_new->SetNode_2(pnode2);
	pnode1->GetLinksList()->AddToEnd(psl_new);
	pnode2->GetLinksList()->AddToEnd(psl_new);

	//Delete the links from temporary 'l' list
	while(!l.IsEmpty())
	{
		psl = l.GetBeginElement();
		this->RemoveLinkWithMerging(psl);
		l.DeleteBegin();
	}

	return 1;
}


int mxSkeleton::FilterMultiplePaths(int threshold_number_of_positions_in_link, unsigned int t)
{
	if(threshold_number_of_positions_in_link<1) return 0;

	mxSkeletonLink *psl;
	int i = -1;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElementAddress();
		i++;
		//If there were multiple links between the nodes, the filtering is performed...
		if(FilterMultiplePathsForGivenNodes(psl->GetNode_1(),psl->GetNode_2(),threshold_number_of_positions_in_link))
		{
			//... and the iterator is reset to left end
			it.SetToBegin(this->GetLinks(t));
			i = -1;
		}
	}

	return 1;
}


int mxSkeleton::FilterMultiplePaths(int threshold_number_of_positions_in_link)
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		if(!this->FilterMultiplePaths(threshold_number_of_positions_in_link,t)) return 0;
	}
	return 1;
}


int mxSkeleton::PasteTo3DImage(mxImage &output_binary_skeleton_image, unsigned int scalar_index, int set_output_size_automatically, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(scalar_index >= this->GetMaximumNumberOfScalars()) scalar_index = 0;

	if(set_output_size_automatically)
	{
		mxPoint min, max;
		this->GetBounds(min,max,t);
		output_binary_skeleton_image.SetDimensions(1,(int)(max.S()+2),(int)(max.R()+2),(int)(max.C()+2));
		output_binary_skeleton_image.FillInWith(0);
	}
	else
	{
		if(output_binary_skeleton_image.IsEmpty()) return 0;
	}
	
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		it.GetElementAddress()->PasteToExisting3DImage(output_binary_skeleton_image,scalar_index);
	}

	if(set_output_size_automatically)
	{
		output_binary_skeleton_image.SetOrigin(0,this->GetOrigin_S(),this->GetOrigin_R(),this->GetOrigin_C());
		output_binary_skeleton_image.SetSpacing(0,this->GetSpacing_S(),this->GetSpacing_R(),this->GetSpacing_C());
	}

	return 1;
}


void mxSkeleton::Smooth(double prev_3, double prev_2, double prev_1, double next_1, double next_2, double next_3, double node_point_strength, unsigned int t)
{
	if(this->IsEmpty()) return;

	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		itl.GetElementAddress()->Smooth(prev_3, prev_2, prev_1, next_1, next_2, next_3, node_point_strength);
	}
}


void mxSkeleton::Smooth(double prev_3, double prev_2, double prev_1, double next_1, double next_2, double next_3, double node_point_strength)
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		this->Smooth(prev_3, prev_2, prev_1, next_1, next_2, next_3, node_point_strength,t);
	}
}


int mxSkeleton::CopyLinkWithItsNodesToOutputSkeleton(mxSkeletonLink *psl, mxSkeleton *p_output_skeleton, unsigned int t2)
{
	if(psl==NULL) return 0;
	if(p_output_skeleton==NULL) return 0;
	
	if(this->IsEmpty()) return 1;

	//If the output skeleton does not exist (is empty), we set the spacing and origin to it from this skeleton
	if(p_output_skeleton->IsEmpty())
	{
		p_output_skeleton->m_origin[0] = m_origin[0]; p_output_skeleton->m_origin[1] = m_origin[1];
		p_output_skeleton->m_origin[2] = m_origin[2]; p_output_skeleton->m_origin[3] = m_origin[3];
		p_output_skeleton->m_spacing[0] = m_spacing[0]; p_output_skeleton->m_spacing[1] = m_spacing[1]; 
		p_output_skeleton->m_spacing[2] = m_spacing[2]; p_output_skeleton->m_spacing[3] = m_spacing[3]; 
	}

	mxSkeletonLink *p_link;
	mxSkeletonNode *p_start_node, *p_end_node;

	//Go through the p_output_skeleton and find out if the psl link already exists in it...
	int is_link_already_in_output_skeleton = 0;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(p_output_skeleton->GetLinks(t2)); itl.IsValid(); itl.MoveToNext())
	{
		p_link = itl.GetElementAddress();
		if(psl->IsEqualTo(p_link))
		{
			is_link_already_in_output_skeleton = 1;
			break;
		}
	}
	//If the link is already in output skeleton, do not copy, just exit
	if(is_link_already_in_output_skeleton) return 1;

	//Add nodes, the link between them and interconnect them all
	mxPoint start = *(psl->GetNode_1_Position());
    mxPoint end = *(psl->GetNode_2_Position());
	p_output_skeleton->AddNodeWithPosition(start, &p_start_node,t2);
	p_output_skeleton->AddNodeWithPosition(end, &p_end_node,t2);
	p_output_skeleton->AddLinkBetweenTwoExistingNodes(p_start_node, p_end_node, &p_link);
	
	//Copy the list of points of the nodes
	mxSkeletonNode *p_node_temp = psl->GetNode_1();
	*(p_start_node->GetSkeletonizationVoxels()) = *(p_node_temp->GetSkeletonizationVoxels());
	p_node_temp = psl->GetNode_2();
	*(p_end_node->GetSkeletonizationVoxels()) = *(p_node_temp->GetSkeletonizationVoxels());

	//Set the metric values
	mxSkeletonNode *p_start = psl->GetNode_1();
	mxSkeletonNode *p_end = psl->GetNode_2();
	p_start_node->GetMetricValue() = p_start->GetMetricValue();
	p_start_node->GetMetricValue_2() = p_start->GetMetricValue_2();
	p_end_node->GetMetricValue() = p_end->GetMetricValue();
	p_end_node->GetMetricValue_2() = p_end->GetMetricValue_2();
	p_link->GetCost() = psl->GetCost();

	//Copy the positions and points of the link
	*(p_link->GetPositionsList()) = *(psl->GetPositionsList());
	*(p_link->GetSkeletonizationVoxels()) = *(psl->GetSkeletonizationVoxels());

	return 1;
}


int mxSkeleton::CopyLinkWithNodesAsFinalPositionsInLink(mxSkeletonLink *psl, mxSkeleton *p_output_skeleton, unsigned int t2)
{
	if(psl==NULL) return 0;
	if(p_output_skeleton==NULL) return 0;
	if(this->IsEmpty()) return 1;

	//If the output skeleton does not exist (is empty), we set the spacing and origin to it from this skeleton
	if(p_output_skeleton->IsEmpty())
	{
        p_output_skeleton->SetOrigin(m_origin[3], m_origin[2], m_origin[1], m_origin[0]);
        p_output_skeleton->SetSpacing(m_spacing[3], m_spacing[2], m_spacing[1], m_spacing[0]);
	}

	//Since the psl contains the node positions, we extract the node positions and the real link positions to sl_real
	mxPoint start = psl->GetPositionsList()->GetBeginElement();
	mxPoint end = psl->GetPositionsList()->GetEndElement();
	mxSkeletonLink *p_link, sl_real;
	sl_real = *psl;
	sl_real.GetPositionsList()->DeleteBegin();
	sl_real.GetPositionsList()->DeleteEnd();

	//Go through the p_output_skeleton and find out if the psl link already exists in it...
	int is_link_already_in_output_skeleton = 0;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(p_output_skeleton->GetLinks(t2)); itl.IsValid(); itl.MoveToNext())
	{
		p_link = itl.GetElementAddress();
		if(sl_real.IsEqualTo(p_link))
		{
			is_link_already_in_output_skeleton = 1;
			break;
		}
	}
	//If the link is already in output skeleton, do not copy, just exit
	if(is_link_already_in_output_skeleton) return 1;

	//Add nodes, the link between them and interconnect them all
	mxSkeletonNode *p_start_node, *p_end_node;
	p_output_skeleton->AddNodeWithPosition(start, &p_start_node,t2);
	p_output_skeleton->AddNodeWithPosition(end, &p_end_node,t2);
	p_output_skeleton->AddLinkBetweenTwoExistingNodes(p_start_node, p_end_node, &p_link);


	//Copy the positions and points of the link
	*(p_link->GetPositionsList()) = *(sl_real.GetPositionsList());
	*(p_link->GetSkeletonizationVoxels()) = *(sl_real.GetSkeletonizationVoxels());
	return 1;
}



int mxSkeleton::CopyFrom(mxSkeleton *p_input_skeleton)
{
    std::cout<<std::endl<<" mxSkeleton::CopyFrom(mxSkeleton *)"<<std::endl;
	if(p_input_skeleton==NULL)
	{
		std::cout<<"mxSkeleton::CopyInputSkeletonToThisSkeleton(): Pointer to input skeleton is NULL!"<<std::endl;
		return 0;
	}
	if(!p_input_skeleton->IsEmpty())
	{
        this->mxSkeleton::Reset();
        
        this->mxDataObject::CopyFromDataObject(p_input_skeleton);

		m_data.SetNumberOfElements(p_input_skeleton->m_data.GetNumberOfElements());

        this->SetScalarTagsAs(*p_input_skeleton);
//		m_scalar_tags = p_input_skeleton->m_scalar_tags;
//		m_scalar_tags_indexes = p_input_skeleton->m_scalar_tags_indexes;
		
        m_origin[0] = p_input_skeleton->m_origin[0]; m_origin[1] = p_input_skeleton->m_origin[1];
		m_origin[2] = p_input_skeleton->m_origin[2]; m_origin[3] = p_input_skeleton->m_origin[3];
		
        m_spacing[0] = p_input_skeleton->m_spacing[0]; m_spacing[1] = p_input_skeleton->m_spacing[1];
		m_spacing[2] = p_input_skeleton->m_spacing[2]; m_spacing[3] = p_input_skeleton->m_spacing[3];

//        m_maximum_number_of_scalars = p_input_skeleton->m_maximum_number_of_scalars;
        
		for(unsigned int t2=0; t2<p_input_skeleton->m_data.GetNumberOfElements(); t2++)
		{	
			mxSkeletonLink *psl;
			mxListIterator<mxSkeletonLink> itl;
			for(itl.SetToBegin(p_input_skeleton->GetLinks(t2)); itl.IsValid(); itl.MoveToNext())
			{
				psl = itl.GetElementAddress();
				p_input_skeleton->CopyLinkWithItsNodesToOutputSkeleton(psl,this,t2); 
			}
		}
	}
    
    std::cout<<std::endl<<" n_of_nodes="<<this->GetNumberOfNodes()<<", n_of_links="<<this->GetNumberOfLinks()<<std::endl;
	return 1;
}


int mxSkeleton::CopyFrom(mxSkeleton &input_skeleton)
{
	return this->CopyFrom(&input_skeleton);
}


int mxSkeleton::CopyFromDataObject(mxDataObject *data_object)
{
    if(!data_object) return 0;
    if(data_object->GetClassName()!=this->GetClassName()) return 0;
    
    mxSkeleton *s = dynamic_cast<mxSkeleton*> (data_object);
    
    if(!s) return 0;
    
    return (this->mxSkeleton::Copy(s));
}


int mxSkeleton::CreateFromPointList(mxPointList *pl)
{
    if(!pl) return 0;
    if(pl->IsEmpty()) return 0;
    this->Reset();
    
    
    mxSkeletonNode *psn1, *psn2;
    mxPoint pos;
    pos = *(pl->GetListOfPoints()->GetBeginElementAddress()->GetPoint());
    this->AddNodeWithPosition(pos,&psn1);
    pos =  *(pl->GetListOfPoints()->GetEndElementAddress()->GetPoint());
    this->AddNodeWithPosition(pos,&psn2);
    mxSkeletonLink *psl;
    this->AddLinkBetweenTwoExistingNodes(psn1,psn2,&psl);
    
    // Copy points to the link.
    {
        unsigned int i=0;
        mxPointList::Iterator it;
        for(it.SetToBegin(pl->GetListOfPoints()), i=0; it.IsValid(); it.MoveToNext(), i++)
        {
            if(i>0 && i<pl->GetNumberOfElements()-1)
            {
                mxPoint *p = it.GetElementAddress()->GetPoint();
                psl->GetPositionsList()->AddToEnd(*p);
            }
        }
    }
    
    // Copy the tags list.
    for(int i=0; i<pl->GetMaximumNumberOfScalars(); i++)
    {
        if(pl->Scalar(i)->IsUsed())
        {
            unsigned int output_index;
            this->AddScalarTag(*(pl->Scalar(i)->GetTag()), output_index);
            //this->GetScalarTags()->AddToEnd(*(pl->Scalar(i)->GetTag()));
        }
    }
    
    return 1;
}



int mxSkeleton::Add(mxSkeleton *p_input_skeleton)
{
    if(p_input_skeleton==NULL)
    {
        std::cout<<"mxSkeleton::Add(): Pointer to input skeleton is NULL!"<<std::endl;
        return 0;
    }
    if(!p_input_skeleton->IsEmpty())
    {
        this->SetScalarTagsAs(*p_input_skeleton);
//        m_scalar_tags = p_input_skeleton->m_scalar_tags;
//        m_scalar_tags_indexes = p_input_skeleton->m_scalar_tags_indexes;
        
        for(unsigned int t2=0; t2<p_input_skeleton->m_data.GetNumberOfElements(); t2++)
        {
            mxSkeletonLink *psl;
            mxListIterator<mxSkeletonLink> itl;
            for(itl.SetToBegin(p_input_skeleton->GetLinks(t2)); itl.IsValid(); itl.MoveToNext())
            {
                psl = itl.GetElementAddress();
                p_input_skeleton->CopyLinkWithItsNodesToOutputSkeleton(psl,this,t2);
            }
        }
    }
    return 1;
}


int mxSkeleton::Add(mxSkeleton &input_skeleton)
{
    return this->Add(&input_skeleton);
}


int mxSkeleton::StoreNodePositionsToLinks(unsigned int t)
{
	if(this->IsEmpty()) return 0;

	mxSkeletonLink *psl;
	mxSkeletonNode *psn1,*psn2;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psn1 = psl->GetNode_1();
		psn2 = psl->GetNode_2();
		psl->GetPositionsList()->AddToBegin(psn1->GetPosition());
		psl->GetPositionsList()->AddToEnd(psn2->GetPosition());
	}
	return 1;
}


int mxSkeleton::StoreNodePositionsToLinks()
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		if(!this->StoreNodePositionsToLinks(t)) return 0;
	}
	return 1;
}


int mxSkeleton::GrowSkeletonFromNode(mxSkeletonNode *p_node, int number_of_steps, mxSkeleton *p_output_skeleton, int *is_skeleton_closed, mxSkeleton *p_limiting_skeleton, unsigned int t2)
{
	if(number_of_steps<1)
	{
		std::cout<<"mxSkeleton::GrowSkeletonFromNode(): Number of growing steps is incorrect!"<<std::endl;
		return 0;
	}

	if(this->IsEmpty()) 
	{
		std::cout<<"mxSkeleton::GrowSkeletonFromNode(): This skeleton is empty!"<<std::endl;
		return 0;
	}

	if(p_node->GetSkeleton()!=this)
	{
		std::cout<<"mxSkeleton::GrowSkeletonFromNode(): Node is not a part of this skeleton!"<<std::endl;
		return 0;
	}

	p_output_skeleton->ResetTimeIndex(t2);//Reset();

	p_output_skeleton->SetVisualizationPropertiesAs(*this);
	
	//Temp list of pointers to nodes for growing the skeleton
	mxList<mxSkeletonNode*> l1, l2, *pl1, *pl2, *pl_temp;
	l1.AddToBegin(p_node);
	pl1 = &l1;
	pl2 = &l2;
	for(int i=0; i<number_of_steps; i++)
	{
		while(!pl1->IsEmpty())
		{
			mxSkeletonNode *psn = pl1->GetBeginElement();
			mxSkeletonLink *psl;
			mxListIterator<mxSkeletonLink*> itl;
			for(itl.SetToBegin(psn->GetLinksList()); itl.IsValid(); itl.MoveToNext())
			{
				psl = itl.GetElement();
				//----- 1. If there is no limiting skeleton -----
				if(p_limiting_skeleton==NULL)
				{
					//If the link was not already added to the output skeleton...
					if(!(p_output_skeleton->IsEquivalentLinkInSkeleton(psl,t2))) 
					{
						//... add it to the output skeleton...
						this->CopyLinkWithItsNodesToOutputSkeleton(psl, p_output_skeleton, t2);
						
						//... and add its nodes to the pl2 list
                        mxSkeletonNode *p_node_temp = psl->GetNode_1();
						pl2->AddToBegin(p_node_temp);
                        p_node_temp = psl->GetNode_2();
						pl2->AddToBegin(p_node_temp);
					}
				}
				//----- 2. If the limiting skeleton exists -----
				else
				{
					//If the link is not in the limiting skeleton...
					if(!(p_limiting_skeleton->IsEquivalentLinkInSkeleton(psl, t2))) 
					{
						//... and if the link was not already added to the output skeleton...
						if(!(p_output_skeleton->IsEquivalentLinkInSkeleton(psl, t2))) 
						{
							//... add it to the output skeleton...
							this->CopyLinkWithItsNodesToOutputSkeleton(psl, p_output_skeleton, t2);
							
							//... and add its nodes to the pl2 list
                            mxSkeletonNode *p_node_temp = psl->GetNode_1();
							pl2->AddToBegin(p_node_temp);
                            p_node_temp = psl->GetNode_2();
							pl2->AddToBegin(p_node_temp);
						}
					}
				}
			}
			pl1->DeleteBegin();
		}
		
		//Exchange pointers of lists
		pl_temp = pl1;
		pl1 = pl2;
		pl2 = pl_temp;

		//If at this moment the pl1 list is empty, this means that the skeleton is grown to the end and that it is closed
		if(pl1->IsEmpty())
		{
			//Set the indicator and exit the loop
			*is_skeleton_closed = 1;
			p_output_skeleton->m_origin[0] = m_origin[0]; p_output_skeleton->m_origin[1] = m_origin[1];
			p_output_skeleton->m_origin[2] = m_origin[2]; p_output_skeleton->m_origin[3] = m_origin[3];
			p_output_skeleton->m_spacing[0] = m_spacing[0]; p_output_skeleton->m_spacing[1] = m_spacing[1];
			p_output_skeleton->m_spacing[2] = m_spacing[2]; p_output_skeleton->m_spacing[3] = m_spacing[3];
			return 1;
		}
	}

	p_output_skeleton->m_origin[0] = m_origin[0]; p_output_skeleton->m_origin[1] = m_origin[1];
	p_output_skeleton->m_origin[2] = m_origin[2]; p_output_skeleton->m_origin[3] = m_origin[3];
	p_output_skeleton->m_spacing[0] = m_spacing[0]; p_output_skeleton->m_spacing[1] = m_spacing[1];
	p_output_skeleton->m_spacing[2] = m_spacing[2]; p_output_skeleton->m_spacing[3] = m_spacing[3];	
	*is_skeleton_closed = 0;
	return 1;
}


int mxSkeleton::GrowSkeletonFromNode(mxSkeletonNode *p_node, mxSkeleton &output_sk, unsigned int t2)
{
	if(this->IsEmpty()) return 0;
	if(p_node==NULL) return 0;
	if(p_node->GetSkeleton()!=this) return 0;
	mxSkeleton3D *skl3D = p_node->GetSkeleton3D();
	if(!skl3D) return 0;

	// We will use metric values to grow skeleton region from the starting node. First, we will back up the
	// existing metrics (in case some other function wants to use them later).
	mxList<double> node_metrics;
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> itn;
	for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		node_metrics.AddToEnd(psn->GetMetricValue());
	}
	if(node_metrics.GetNumberOfElements()!=skl3D->m_nodes.GetNumberOfElements()) 
	{
		std::cout<<"mxSkeleton::IsPathConnectingGivenNodes(): Error when saving node metrics!"<<std::endl;
		return 0;
	}

	// Now reset all the node metrics to 0 (this will indicate that the region was not yet grown to this node),
	// except the starting node which we set to 1.
	for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		psn->GetMetricValue() = 0;
	}
	
	p_node->GetMetricValue() = 1;
	output_sk.ResetTimeIndex(t2);
	output_sk.SetVisualizationPropertiesAs(*this);

	//Do the region growing
	mxList<mxSkeletonNode*> l;
	l.AddToBegin(p_node);
	mxSkeletonLink *psl;
	while(!l.IsEmpty())
	{
		psn = l.GetBeginElement();
		
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(psn->GetLinksList()); itl.IsValid(); itl.MoveToNext())
		{
			psl = itl.GetElement();
			mxSkeletonNode *psnn = psl->GetNode_2();
			if(psnn==psn) psnn = psl->GetNode_1();

			if(psnn->GetMetricValue()==0)
			{
				if(psnn!=psn)
				{
					l.AddToEnd(psnn);
					this->CopyLinkWithItsNodesToOutputSkeleton(psl,&output_sk,t2);
				}
			}
		}
		psn->GetMetricValue() = 1;
		l.DeleteBegin();
	}

	//Restore all the metrics before exiting the function
	for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		psn->GetMetricValue() = node_metrics.GetBeginElement();
		node_metrics.DeleteBegin();
	}

	return 1;
}


int mxSkeleton::RadiusAtEachPosition(mxImage &segmented_image, mxString &scalar_tag, unsigned int t)
{
	if(this->IsEmpty()) return 0; 
	if(segmented_image.IsEmpty()) return 0;

	unsigned int scalar_index;
	if(!this->AddScalarTag(scalar_tag, scalar_index)) return 0;

	mxGeometry g;
	g.SetDimensions(segmented_image.GetDimension_S(), segmented_image.GetDimension_R(), segmented_image.GetDimension_C());

	//For each link of the p_output_skeleton, caluclate the radius
	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		mxListIterator<mxPoint> itp;

		for(itp.SetToBegin(psl->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
		{
			mxPoint *p = itp.GetElementAddress();
			unsigned int squared_radius = 0;
			int is_radius_found = 0;
			while(!is_radius_found)
			{
				int sn,rn,cn;
				for(g.ForSphere(((int)p->S()),((int)p->R()),((int)p->C()),squared_radius); g.GetSphere(squared_radius,sn,rn,cn); )
				{
					if(segmented_image(sn,rn,cn)==0)
					{
						is_radius_found = 1;
						break;
					}
				}
				squared_radius++;
				if(squared_radius > g.GetMaxSphereSquaredRadius()-1) { is_radius_found = 1; }
			}

			//Now put the calculated radius to the adequate scalar tag.
			p->V(scalar_index) = (double)squared_radius;
		}
	}
	return 1;
}


int mxSkeleton::GetArrayOfSkeletonNodes(mxArray<mxSkeletonNode*> &array_of_node_pointers, unsigned int t)
{
	if(this->GetNumberOfNodes(t)==0) return 0;

	array_of_node_pointers.SetNumberOfElements(this->GetNumberOfNodes(t));

	mxListIterator<mxSkeletonNode> it;
	int i=0;
	for(it.SetToBegin(this->GetNodes(t)); it.IsValid(); it.MoveToNext())
	{
		array_of_node_pointers[i] = it.GetElementAddress();
		i++;
	}
	
	return 1;
}


int mxSkeleton::GetArraysOfCorrespondingSkeletonNodesInThisAndInputSkeleton(mxSkeleton &input_sk, mxArray<mxSkeletonNode*> &array_of_node_addresses1, mxArray<mxSkeletonNode*> &array_of_node_addresses2, unsigned int t1, unsigned int t2)
{
	if(this->GetNumberOfNodes(t1)==0) return 0;
	if(this->GetNumberOfNodes(t1)!=input_sk.GetNumberOfNodes(t2)) return 0;

	array_of_node_addresses1.SetNumberOfElements(this->GetNumberOfNodes(t1));
	array_of_node_addresses2.SetNumberOfElements(this->GetNumberOfNodes(t2));

	mxListIterator<mxSkeletonNode> it;
	int i=0;
	for(it.SetToBegin(this->GetNodes(t1)); it.IsValid(); it.MoveToNext())
	{
		array_of_node_addresses1[i] = it.GetElementAddress();
		i++;
	}
	i=0;
	for(it.SetToBegin(input_sk.GetNodes(t2)); it.IsValid(); it.MoveToNext())
	{
		array_of_node_addresses2[i] = it.GetElementAddress();
		i++;
	}
	
	return 1;
}


int mxSkeleton::GetArraysOfCorrespondingSkeletonLinksInThisAndInputSkeleton(mxSkeleton &input_sk, mxArray<mxSkeletonLink*> &array_of_node_addresses1, mxArray<mxSkeletonLink*> &array_of_node_addresses2, unsigned int t1, unsigned int t2)
{
	if(this->GetNumberOfLinks(t1)==0) return 0;
	if(this->GetNumberOfLinks(t1)!=input_sk.GetNumberOfLinks(t2)) return 0;

	array_of_node_addresses1.SetNumberOfElements(this->GetNumberOfLinks(t1));
	array_of_node_addresses2.SetNumberOfElements(this->GetNumberOfLinks(t2));

	mxListIterator<mxSkeletonLink> it;
	int i=0;
	for(it.SetToNode(this->GetLinks(t1)->GetBeginNodeAddress()); it.IsValid(); it.MoveToNext())
	{
		array_of_node_addresses1[i] = it.GetElementAddress();
		i++;
	}
	i=0;
	for(it.SetToNode(input_sk.GetLinks(t2)->GetBeginNodeAddress()); it.IsValid(); it.MoveToNext())
	{
		array_of_node_addresses2[i] = it.GetElementAddress();
		i++;
	}
	
	return 1;
}


int mxSkeleton::SetLinkMetricsAsOrderOfStubLinkFilteringAndNodeMerging()
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		if(!this->SetLinkMetricsAsOrderOfStubLinkFilteringAndNodeMerging(t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsAsOrderOfStubLinkFilteringAndNodeMerging(unsigned int t)
{
	if(this->IsEmpty()) return 0;

	mxSkeleton temp;
	temp.CopyFrom(this);

	//Create arrays of corresponding node and link pointers for fast access
	mxArray<mxSkeletonLink*> al1,al2;
	this->GetArraysOfCorrespondingSkeletonLinksInThisAndInputSkeleton(temp,al1,al2,t,t);

	mxListIterator<mxSkeletonLink> itsl;
	mxSkeletonLink *psl;
	//We assign each link a metric value equal to the number of positions in the link
	for(itsl.SetToBegin(temp.GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
	{
		psl = itsl.GetElementAddress();
		psl->GetCost() = psl->LengthOfLink();
	}
	//We assign each link a metric value equal to the number of positions in the link
	for(itsl.SetToBegin(this->GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
	{
		psl = itsl.GetElementAddress();
		psl->GetCost() = psl->LengthOfLink();
	}
	
	while(temp.IsNumberOfStubLinksGreaterThan(1,t))
	{
		//Get the minimum stub link metric value
		double min = -1;
		for(itsl.SetToBegin(temp.GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
		{ 
			//If the link is a stub link
			if(itsl.GetElementAddress()->IsStub())
			{
				if(itsl.GetElementAddress()->GetCost()<min || min<0) min = itsl.GetElementAddress()->GetCost();
			}
		}
		
		//Remove each stub link in 'temp' that has the lowest metric value and generate a new value by addition in case some nodes now have only 2 links
		for(itsl.SetToBegin(temp.GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
		{
			if(itsl.GetElementAddress()->IsStub())
			{
				if(itsl.GetElementAddress()->GetCost()<=min)
				{
					//Get list of links connected to the found stub link
					mxList<mxSkeletonLink*> list_of_connecting_links;
					itsl.GetElementAddress()->GetAdjacentLinks(list_of_connecting_links);

					//Remove the found stub link
					mxList<mxSkeletonLink*> list_of_deleted_links;
					temp.RemoveLink(itsl.GetElementAddress());

					//In case when removing the link will leave a node with 2 links 
					// (means links should be connected), we calculate the new metric
					if(list_of_connecting_links.GetNumberOfElements()==2)
					{
						//Calculate the metric as a sum of metrics of connecting links
						double new_addition_to_metric = list_of_connecting_links.GetBeginElement()->GetCost() + list_of_connecting_links.GetBeginElement()->GetCost();

						//Find out which links are connecting links of the newly connected ones
						mxList<mxSkeletonLink*> l;
						l = list_of_connecting_links;
						double max_existing_metric = -1;
						while(!l.IsEmpty())
						{
							mxSkeletonLink *psl = l.GetBeginElement();

							
							//--- 1. Start Node side ---
							mxSkeletonNode *psn = psl->GetNode_1();
							if(psn->GetNumberOfLinks()==2)//If a connection to another link is found...
							{
								mxSkeletonLink *psl2;
								psl2 = psn->GetLinksList()->GetBeginElement();
								if(psl2==psl) psl2 = psn->GetLinksList()->GetEndElement();//Make sure the connecting link is not the currently considered link...
								
								//... and that it's not already in the list of connecting links
								if(!mxListFunctions::HasElement(list_of_connecting_links,psl2))
								{
									l.AddToEnd(psl2);
									list_of_connecting_links.AddToEnd(psl2);
									if(max_existing_metric<psl2->GetCost()) max_existing_metric = psl2->GetCost();
								}
							}
							//--- 2. End Node side ---
							psn = psl->GetNode_2();
							if(psn->GetNumberOfLinks()==2)//If a connection to another link is found...
							{
								mxSkeletonLink *psl2;
								psl2 = psn->GetLinksList()->GetBeginElement();
								if(psl2==psl) psl2 = psn->GetLinksList()->GetEndElement();//Make sure the connecting link is not the currently considered link...
								
								//... and that it's not already in the list of connecting links
								if(!mxListFunctions::HasElement(list_of_connecting_links,psl2))
								{
									l.AddToEnd(psl2);
									list_of_connecting_links.AddToEnd(psl2);
									if(max_existing_metric<psl2->GetCost()) max_existing_metric = psl2->GetCost();
								}
							}

							l.DeleteBegin();
						}

						//For each link in list of connecting nodes assign a new calculated metric value in both temp and this skeleton
						double new_metric = max_existing_metric + new_addition_to_metric;
						mxListIterator<mxSkeletonLink*> itsl2;
						for(itsl2.SetToBegin(list_of_connecting_links); itsl2.IsValid(); itsl2.MoveToNext())
						{
							itsl2.GetElement()->GetCost() = new_metric;
							mxSkeletonLink *psl2 = itsl2.GetElement();
                            int index = mxArrayFunctions::IndexOfElement(al2,psl2);
                            //mxArraySortAndSearch<mxSkeletonLink*> ss;
							//int index = ss.IndexOfElement(al2,psl2);
							(al1[index])->GetCost() = new_metric;
						}
					}
				}
			}
		}
	}

	return 1;
}


int mxSkeleton::SetLinkMetricsAsStubLinkRemovalStepNumber()
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		if(!this->SetLinkMetricsAsStubLinkRemovalStepNumber(t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsAsStubLinkRemovalStepNumber(unsigned int t)
{
	if(this->IsEmpty()) return 0;

	mxSkeleton temp;
	temp.CopyFrom(this);

	//Create arrays of corresponding node and link pointers for fast access
	mxArray<mxSkeletonLink*> al1,al2;
	this->GetArraysOfCorrespondingSkeletonLinksInThisAndInputSkeleton(temp,al1,al2,t,t);

	mxListIterator<mxSkeletonLink> itsl;
	mxSkeletonLink *psl;
	//We assign each link a metric value equal to the number of positions in the link
	for(itsl.SetToBegin(temp.GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
	{
		psl = itsl.GetElementAddress();
		//psl->GetCost() = psl->GetLinkLength();
		psl->GetCost() = 1;
	}
	//We assign each link a metric value equal to the number of positions in the link
	for(itsl.SetToBegin(this->GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
	{
		psl = itsl.GetElementAddress();
		//psl->GetCost() = psl->GetLinkLength();
		psl->GetCost() = 1;
	}
	
	while(temp.IsNumberOfStubLinksGreaterThan(1,t))
	{
		//Get the minimum stub link metric value
		double min = -1;
		for(itsl.SetToBegin(temp.GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
		{ 
			//If the link is a stub link
			if(itsl.GetElementAddress()->IsStub())
			{
				if(itsl.GetElementAddress()->GetCost()<min || min<0) min = itsl.GetElementAddress()->GetCost();
			}
		}
		
		//Remove each stub link in 'temp' and generate a new link in case some nodes now have only 2 links
		for(itsl.SetToBegin(temp.GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
		{
			if(itsl.GetElementAddress()->IsStub())
			{
				if(itsl.GetElementAddress()->GetCost()<=min)
				{
					//Get list of links connected to the found stub link
					mxList<mxSkeletonLink*> list_of_connecting_links;
					itsl.GetElementAddress()->GetAdjacentLinks(list_of_connecting_links);

					//Remove the found stub link
					mxList<mxSkeletonLink*> list_of_deleted_links;
					temp.RemoveLink(itsl.GetElementAddress());

					//In case when removing the link will leave a node with 2 links 
					// (means links should be connected), we calculate the new metric
					if(list_of_connecting_links.GetNumberOfElements()==2)
					{
						//Calculate the metric as a sum of metrics of connecting links
						double new_addition_to_metric = list_of_connecting_links.GetBeginElement()->GetCost() + list_of_connecting_links.GetBeginElement()->GetCost();

						//Find out which links are connecting links of the newly connected ones
						mxList<mxSkeletonLink*> l;
						l = list_of_connecting_links;
						double max_existing_metric = -1;
						while(!l.IsEmpty())
						{
							mxSkeletonLink *psl = l.GetBeginElement();
							
							//--- 1. Start Node side ---
							mxSkeletonNode *psn = psl->GetNode_1();
							if(psn->GetNumberOfLinks()==2)//If a connection to another link is found...
							{
								mxSkeletonLink *psl2;
								psl2 = psn->GetLinksList()->GetBeginElement();
								if(psl2==psl) psl2 = psn->GetLinksList()->GetEndElement();//Make sure the connecting link is not the currently considered link...
								
								//... and that it's not already in the list of connecting links						
								if(!mxListFunctions::HasElement(list_of_connecting_links,psl2))
								{
									l.AddToEnd(psl2);
									list_of_connecting_links.AddToEnd(psl2);
									if(max_existing_metric<psl2->GetCost()) max_existing_metric = psl2->GetCost();
								}
							}
							//--- 2. End Node side ---
							psn = psl->GetNode_2();
							if(psn->GetNumberOfLinks()==2)//If a connection to another link is found...
							{
								mxSkeletonLink *psl2;
								psl2 = psn->GetLinksList()->GetBeginElement();
								if(psl2==psl) psl2 = psn->GetLinksList()->GetEndElement();//Make sure the connecting link is not the currently considered link...
								
								//... and that it's not already in the list of connecting links
								if(!mxListFunctions::HasElement(list_of_connecting_links,psl2))
								{
									l.AddToEnd(psl2);
									list_of_connecting_links.AddToEnd(psl2);
									if(max_existing_metric<psl2->GetCost()) max_existing_metric = psl2->GetCost();
								}
							}

							l.DeleteBegin();
						}

						//For each link in list of connecting nodes assign a new calculated metric value in both temp and this skeleton
						double new_metric = max_existing_metric + new_addition_to_metric;
						mxListIterator<mxSkeletonLink*> itsl2;
						for(itsl2.SetToBegin(list_of_connecting_links); itsl2.IsValid(); itsl2.MoveToNext())
						{
							itsl2.GetElement()->GetCost() = new_metric;
							mxSkeletonLink *psl2 = itsl2.GetElement();
                            int index = mxArrayFunctions::IndexOfElement(al2,psl2);
							(al1[index])->GetCost() = new_metric;
						}
					}
				}
			}
		}
	}

	//Make a 'histogram' of skeleton link metrics
	mxList<int> list_of_existing_link_metrics;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		int metric = (int)(it.GetElementAddress()->GetCost());
		if(!mxListFunctions::HasElement(list_of_existing_link_metrics,metric))
			list_of_existing_link_metrics.AddToEnd(metric);
	}


	mxListFunctions::SortAscending(list_of_existing_link_metrics);
    
	//Copy the list into an array
	mxArray<int> array_of_existing_link_metrics;
	array_of_existing_link_metrics.SetNumberOfElements(list_of_existing_link_metrics.GetNumberOfElements());
	mxListIterator<int> it2;
	int i=0;
	for(it2.SetToBegin(list_of_existing_link_metrics); it2.IsValid(); it2.MoveToNext())
	{
		array_of_existing_link_metrics[i] = it2.GetElement();
		i++;
	}

	//Replace the metric values by the indexes (of the metric values) in the sorted array
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		int metric = (int)(it.GetElementAddress()->GetCost());
        int index = mxArrayFunctions::IndexOfElement(array_of_existing_link_metrics,metric);
		if(index>=0) 
		{
			it.GetElementAddress()->GetCost() = index;
		}
	}

	return 1;
}


int mxSkeleton::SetLinkValuesFromLinkMetrics(mxString &tag)
{
	for(unsigned int t=0; t<m_data.GetNumberOfElements(); t++)
	{
		if(!this->SetLinkValuesFromLinkMetrics(tag,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkValuesFromLinkMetrics(mxString &tag, unsigned int t)
{
	unsigned int index;
	// Search for the input tag
	if(!this->GetScalarTagIndex(tag, index))
	{
		//... if it doesn't exist create new one...
		if(!this->AddScalarTag(tag, index)) return 0;
	}

	mxListIterator<mxSkeletonLink> it_l;
	for(it_l.SetToBegin(this->GetLinks(t)); it_l.IsValid(); it_l.MoveToNext())
	{
		mxListIterator<mxPoint> it;
		for(it.SetToBegin(it_l.GetElement().GetPositionsList()); it.IsValid(); it.MoveToNext())
		{
			it.GetElement().V(index) = it_l.GetElement().GetCost();
		}
	}

	return 1;
}


int mxSkeleton::PruningByLinkMetricsToLeaveGivenNumberOfStubLinks(mxSkeleton &output_sk, unsigned int number_of_stub_links)
{
	output_sk.SetNumberOfTimeSeries(this->GetNumberOfTimeSeries());
	output_sk.SetVisualizationPropertiesAs(*this);
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->PruningByLinkMetricsToLeaveGivenNumberOfStubLinks(output_sk,number_of_stub_links,t,t)) return 0;
	}
	return 1;
}


int mxSkeleton::PruningByLinkMetricsToLeaveGivenNumberOfStubLinks(mxSkeleton &output_sk, unsigned int number_of_stub_links, unsigned int t, unsigned int t2)
{

	//Make a 'histogram' of skeleton link metrics
	mxList<double> list_of_existing_link_metrics;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		if(!mxListFunctions::HasElement(list_of_existing_link_metrics,(it.GetElementAddress()->GetCost())))
			list_of_existing_link_metrics.AddToEnd(it.GetElementAddress()->GetCost());
	}

	//Sort the histogram in descending way
	mxListFunctions::SortDescending(list_of_existing_link_metrics);// list_of_existing_link_metrics.SortDescending();
	

	//Now the input 'number_of_stub_links' represents the index for the threshold value in the list_of_existing_link_metrics.
	//We copy the links with the given metrics into the output skeleton

	output_sk.ResetTimeIndex(t2);//Delete();
	if(list_of_existing_link_metrics.GetNumberOfElements()-1<=number_of_stub_links) //In case all links fall inside the specified metric range, just copy the skeleton
	{
		output_sk.CopyFrom(this);
		return 1;
	}

	//If thresholding needs to be performed
	double threshold = list_of_existing_link_metrics[number_of_stub_links] + 0.001;//We add 0.001 as a tollerance for comparing double values
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		if(it.GetElementAddress()->GetCost() > threshold)
			this->CopyLinkWithItsNodesToOutputSkeleton(it.GetElementAddress(), &output_sk, t2);
	}

	return 1;
}


int mxSkeleton::SetLinkMetricsToMinOfScalars(unsigned int scalar_index)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToMinOfScalars(scalar_index,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToMinOfScalars(unsigned int scalar_index, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(scalar_index >= this->GetNodes(t)->GetBeginElement().GetPosition().GetNumberOfValues()) return 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psl->GetCost() = -1;
		mxPoint *ppos;
		mxListIterator<mxPoint> itp;
		for(itp.SetToBegin(psl->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
		{ 
			ppos = itp.GetElementAddress();
			if(ppos->V(scalar_index) < psl->GetCost() || psl->GetCost()<0) psl->GetCost() = ppos->V(scalar_index);
		}
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToMaxOfScalars(unsigned int scalar_index)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToMaxOfScalars(scalar_index,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToMaxOfScalars(unsigned int scalar_index, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(scalar_index >= this->GetNodes(t)->GetBeginElement().GetPosition().GetNumberOfValues()) return 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psl->GetCost() = -1;
		mxPoint *ppos;
		mxListIterator<mxPoint> itp;
		for(itp.SetToBegin(psl->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
		{ 
			ppos = itp.GetElementAddress();
			if(ppos->V(scalar_index) > psl->GetCost()) psl->GetCost() = ppos->V(scalar_index);
		}
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToAverageOfScalars(unsigned int scalar_index)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToAverageOfScalars(scalar_index,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToAverageOfScalars(unsigned int scalar_index, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(scalar_index >= this->GetNodes(t)->GetBeginElement().GetPosition().GetNumberOfValues()) return 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psl->GetCost() = 0;
		mxPoint *ppos;
		mxListIterator<mxPoint> itp;
		for(itp.SetToBegin(psl->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
		{ 
			ppos = itp.GetElementAddress();
			psl->GetCost() += ppos->V(scalar_index);
		}
		psl->GetCost() = psl->GetCost()/(psl->GetNumberOfPositions());
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToInverseOfMinOfScalars(unsigned int scalar_index)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToInverseOfMinOfScalars(scalar_index,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToInverseOfMinOfScalars(unsigned int scalar_index, unsigned int t)
{
	if(!this->SetLinkMetricsToMinOfScalars(scalar_index,t)) return 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> itl;

	//Find min and max value of metric
	double min=-1, max=-1;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		if(psl->GetCost()<min || min<0) min = psl->GetCost();
		if(psl->GetCost()>max || max<0) max = psl->GetCost();
	}
	if(!(max>min && max>0)) return 0;
	//Inverse the values 
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psl->GetCost() = max+min-psl->GetCost();
	}

	return 1;
}


int mxSkeleton::SetLinkMetricsToInverseOfMaxOfScalars(unsigned int scalar_index)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToInverseOfMaxOfScalars(scalar_index,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToInverseOfMaxOfScalars(unsigned int scalar_index, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(scalar_index >= this->GetNodes(t)->GetBeginElement().GetPosition().GetNumberOfValues()) return 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psl->GetCost() = -1;
		mxPoint *ppos;
		mxListIterator<mxPoint> itp;
		for(itp.SetToBegin(psl->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
		{ 
			ppos = itp.GetElementAddress();
			if(ppos->V(scalar_index) > psl->GetCost()) psl->GetCost() = ppos->V(scalar_index);
		}
		psl->GetCost() = 1.0 / psl->GetCost();
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToInverseOfAverageOfScalars(unsigned int scalar_index)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToInverseOfAverageOfScalars(scalar_index,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToInverseOfAverageOfScalars(unsigned int scalar_index, unsigned int t)
{
	if(this->IsEmpty()) return 0;
	if(scalar_index >= this->GetNodes(t)->GetBeginElement().GetPosition().GetNumberOfValues()) return 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psl->GetCost() = 0;
		mxPoint *ppos;
		mxListIterator<mxPoint> itp;
		for(itp.SetToBegin(psl->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
		{ 
			ppos = itp.GetElementAddress();
			psl->GetCost() += ppos->V(scalar_index);
		}
		psl->GetCost() = psl->GetCost()/(psl->GetNumberOfPositions());
		psl->GetCost() = 1.0 / psl->GetCost();
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToAverageNumberOfLinksOfConnectingNodes()
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToAverageNumberOfLinksOfConnectingNodes(t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToAverageNumberOfLinksOfConnectingNodes(unsigned int t)
{
	if(this->IsEmpty()) return 0;

	mxSkeletonLink *psl;
	mxSkeletonNode *psn1,*psn2;
	mxListIterator<mxSkeletonLink> itl;
	for(itl.SetToBegin(this->GetLinks(t)); itl.IsValid(); itl.MoveToNext())
	{
		psl = itl.GetElementAddress();
		psn1 = psl->GetNode_1();
		psn2 = psl->GetNode_2();
		psl->GetCost() = ((double)(psn1->GetNumberOfLinks() + psn2->GetNumberOfLinks())) / 2.0;
	}

	return 1;
}


int mxSkeleton::SetLinkMetricsToLinkLength()
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToLinkLength(t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToLinkLength(unsigned int t)
{
	if(this->IsEmpty()) return 0;

	mxSkeletonLink *psl;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		psl = it.GetElementAddress();
		psl->GetCost() = psl->LengthOfLink();
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsTo(double metric_value)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsTo(metric_value,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsTo(double metric_value, unsigned int t)
{
	if(this->IsEmpty()) return 0;

	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		it.GetElementAddress()->GetCost() = metric_value;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToHierarchyFromRoot(mxSkeletonLink *root_link)
{
	if(this->IsEmpty()) return 0;
	if(!root_link) return 0;
	if(root_link->GetSkeleton()!=this) return 0;

	this->SetLinkMetricsTo(-1);	//Set link metrics to impossible value

	mxList<mxSkeletonLink*> l;
	l.AddToBegin(root_link);
	double step = 0;
	while(!l.IsEmpty())
	{
		mxList<mxSkeletonLink*> l_of_neighbors;
		l.GetBeginElement()->GetAdjacentLinks(l_of_neighbors);

		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(l_of_neighbors); itl.IsValid(); itl.MoveToNext())
		{
			mxSkeletonLink *psl = itl.GetElement();
			if(psl->GetCost()>step || psl->GetCost()<0)
			{
				psl->GetCost() = step;
				l.AddToEnd(psl);
			}
		}
		step += 1;
		l.DeleteBegin();
	}

	return 1;
}


int mxSkeleton::SetLinkMetricsToSequentialNumbering()
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkMetricsToSequentialNumbering(t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkMetricsToSequentialNumbering(unsigned int t)
{
	if(this->IsEmpty()) return 0;

	int label = 0;
	mxListIterator<mxSkeletonLink> it;
	for(it.SetToBegin(this->GetLinks(t)); it.IsValid(); it.MoveToNext())
	{
		it.GetElementAddress()->GetCost() = label;
		label++;
	}
	return 1;
}


int mxSkeleton::SetLinkAndNodeScalarValuesToPositionSequentialNumbering(mxString &tag)
{
	for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
	{
		if(!this->SetLinkAndNodeScalarValuesToPositionSequentialNumbering(tag,t)) return 0;
	}
	return 1;
}


int mxSkeleton::SetLinkAndNodeScalarValuesToPositionSequentialNumbering(mxString &tag, unsigned int t)
{
	if(this->IsEmpty()) return 0;

	unsigned int index;
	// Search for the input tag and if it doesn't exist create new one...
	if(!this->GetScalarTagIndex(tag, index)) { if(!this->AddScalarTag(tag, index)) return 0; }

	double label = 0;

	//----- 1. positions in the links -----
	mxListIterator<mxSkeletonLink> it_l;
	for(it_l.SetToBegin(this->GetLinks(t)); it_l.IsValid(); it_l.MoveToNext())
	{
		mxListIterator<mxPoint> it;
		for(it.SetToBegin(it_l.GetElement().GetPositionsList()); it.IsValid(); it.MoveToNext())
		{
			it.GetElement().V(index) = label;
			label += 1;
		}
	}

	//----- 2. positions in the nodes -----
	mxListIterator<mxSkeletonNode> it_n;
	for(it_n.SetToBegin(this->GetNodes(t)); it_n.IsValid(); it_n.MoveToNext())
	{
		it_n.GetElementAddress()->GetPosition().V(index) = label;
		label += 1;
	}

	return 1;
}


void mxSkeleton::SetConvergencePoint_PropagateHigherValue(mxSkeletonNode *input_node)
{
	mxSkeleton3D *skl3D = input_node->GetSkeleton3D();
	if(!skl3D) return;

	skl3D->m_convergence_node1 = input_node;

	//Set metrics of all the nodes to be -1, except the _psn, which will be 0
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> itn;
	for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		psn->GetMetricValue() = -1;
		psn->SetLinkToSourceNode(NULL);
	}
	input_node->GetMetricValue() = 0;
	input_node->SetLinkToSourceNode(NULL);
	

	mxList<mxSkeletonNode*> l;
	l.AddToBegin(input_node);
	mxSkeletonNode *psn2;
	mxSkeletonLink *psl;
	while(!l.IsEmpty())
	{
		psn = l.GetBeginElement();
        
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(psn->GetLinksList()); itl.IsValid(); itl.MoveToNext())
		{
			psl = itl.GetElement();
            psn2 = psl->GetNode_1();
			if(psn==psn2) psn2 = psl->GetNode_2();
			if(psn!=psn2)
			{
				//If the node psn2 gets updated, we have to enter its neighboring nodes to list as candidates for updating
				if(psn2->GetUpdates_PropagateHigherValue()) { l.AddToEnd(psn2); }
			}
		}

		l.DeleteBegin();
	}
}



void mxSkeleton::SetConvergencePoint_PropagateLowerValue(mxSkeletonNode *input_node)
{
	mxSkeleton3D *skl3D = input_node->GetSkeleton3D();
	if(!skl3D) return;
	skl3D->m_convergence_node1 = input_node;

	//Set metrics of all the nodes to be -1, except the _psn, which will be 0
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> itn;
	for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		psn->GetMetricValue() = -1;
		psn->SetLinkToSourceNode(NULL);
	}
	input_node->GetMetricValue() = 0;
	input_node->SetLinkToSourceNode(NULL);
	

	mxList<mxSkeletonNode*> l;
	l.AddToBegin(input_node);
	mxSkeletonNode *psn2;
	mxSkeletonLink *psl;
	while(!l.IsEmpty())
	{
		psn = l.GetBeginElement();
	
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(psn->GetLinksList()); itl.IsValid(); itl.MoveToNext())
        {
			psl = itl.GetElement();
            psn2 = psl->GetNode_1();
			if(psn==psn2) psn2 = psl->GetNode_2();
			if(psn!=psn2)
			{
				//If the node psn2 gets updated, we have to enter its neighboring nodes to list as candidates for updating
				if(psn2->GetUpdates_PropagateLowerValue()) { l.AddToEnd(psn2); }
			}
		}

		l.DeleteBegin();
	}
}


void mxSkeleton::SetConvergencePoint_PropagateHigherValue_2(mxSkeletonNode *input_node)
{
	mxSkeleton3D *skl3D = input_node->GetSkeleton3D();
	if(!skl3D) return;
	skl3D->m_convergence_node2 = input_node;

	//Set metrics_2 of all the nodes to be -1, except the _psn, which will be 0
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> itn;
	for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		psn->GetMetricValue_2() = -1;
        psn->SetLinkToSourceNode_2(NULL);
	}
	input_node->GetMetricValue_2() = 0;
    input_node->SetLinkToSourceNode_2(NULL);
	

	mxList<mxSkeletonNode*> l;
	l.AddToBegin(input_node);
	mxSkeletonNode *psn2;
	mxSkeletonLink *psl;
	while(!l.IsEmpty())
	{
        psn = l.GetBeginElement();
		
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(psn->GetLinksList()); itl.IsValid(); itl.MoveToNext())
        {
			psl = itl.GetElement();
            psn2 = psl->GetNode_1();
			if(psn==psn2) psn2 = psl->GetNode_2();
			if(psn!=psn2)
			{
				//If the node psn2 gets updated, we have to enter its neighboring nodes to list as candidates for updating
				if(psn2->GetUpdates_PropagateHigherValue_2()) { l.AddToEnd(psn2); }
			}
		}

		l.DeleteBegin();
	}
}



void mxSkeleton::SetConvergencePoint_PropagateLowerValue_2(mxSkeletonNode *input_node)
{
	mxSkeleton3D *skl3D = input_node->GetSkeleton3D();
	if(!skl3D) return;
	skl3D->m_convergence_node2 = input_node;

	//Set metrics_2 of all the nodes to be -1, except the _psn, which will be 0
	mxSkeletonNode *psn;
	mxListIterator<mxSkeletonNode> itn;
	for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
	{
		psn = itn.GetElementAddress();
		psn->GetMetricValue_2() = -1;
        psn->SetLinkToSourceNode_2(NULL);
	}
	input_node->GetMetricValue_2() = 0;
	input_node->SetLinkToSourceNode_2(NULL);
	

	mxList<mxSkeletonNode*> l;
	l.AddToBegin(input_node);
	mxSkeletonNode *psn2;
	mxSkeletonLink *psl;
	while(!l.IsEmpty())
	{
		psn = l.GetBeginElement();
	
		mxListIterator<mxSkeletonLink*> itl;
		for(itl.SetToBegin(psn->GetLinksList()); itl.IsValid(); itl.MoveToNext())
        {
			psl = itl.GetElement();
            psn2 = psl->GetNode_1();
			if(psn==psn2) psn2 = psl->GetNode_2();
			if(psn!=psn2)
			{
				//If the node psn2 gets updated, we have to enter its neighboring nodes to list as candidates for updating
				if(psn2->GetUpdates_PropagateLowerValue_2())
				{
					l.AddToEnd(psn2);
				}
			}
		}

		l.DeleteBegin();
	}
    
//    // FOR DEBUG
//    for(itn.SetToBegin(skl3D->m_nodes); itn.IsValid(); itn.MoveToNext())
//    {
//        psn = itn.GetElementAddress();
//        std::cout<<"  NODE:"<<((void*)psn)<<", metric_2="<<psn->GetMetricValue_2()<<", n_of_steps_2="<<psn->_number_of_steps_2<<", link_to_source_2="<<((void*)psn->m_link_to_source_node_2)<<";  ";
//    }

    
}


int mxSkeleton::BestPathToNode(mxSkeletonNode *destination_node, mxSkeleton *p_output_skeleton, unsigned int t2)
{
	if(destination_node->GetSkeleton()!=this) return 0;

	p_output_skeleton->ResetTimeIndex(t2);

    p_output_skeleton->SetScalarTagsAs(*this);
//    p_output_skeleton->m_scalar_tags = m_scalar_tags;
//    p_output_skeleton->m_scalar_tags_indexes = m_scalar_tags_indexes;


	mxSkeletonNode *psn, *psn2;

	psn = destination_node;
	while(psn->GetLinkToSourceNode()!=NULL)
	{
		psn2 = psn;
		this->CopyLinkWithItsNodesToOutputSkeleton(psn->GetLinkToSourceNode(),p_output_skeleton,t2);
		if(psn==psn->GetLinkToSourceNode()->GetNode_1()) psn = psn->GetLinkToSourceNode()->GetNode_2();
		else psn = psn->GetLinkToSourceNode()->GetNode_1();

		if(psn2==psn)
		{
			//std::cout<<"psn2==psn! Exit!"<<std::endl;
			return 0;//No output skeleton is produced
		}
	}

	p_output_skeleton->m_origin[0] = m_origin[0]; p_output_skeleton->m_origin[1] = m_origin[1];
	p_output_skeleton->m_origin[2] = m_origin[2]; p_output_skeleton->m_origin[3] = m_origin[3];
	p_output_skeleton->m_spacing[0] = m_spacing[0]; p_output_skeleton->m_spacing[1] = m_spacing[1]; 
	p_output_skeleton->m_spacing[2] = m_spacing[2]; p_output_skeleton->m_spacing[3] = m_spacing[3]; 

	return 1;
}


int mxSkeleton::BestPathToNode_2(mxSkeletonNode *destination_node, mxSkeleton *p_output_skeleton, unsigned int t2)
{
	if(destination_node->GetSkeleton()!=this) return 0;

	p_output_skeleton->ResetTimeIndex(t2);

    p_output_skeleton->SetScalarTagsAs(*this);
    
//    p_output_skeleton->m_scalar_tags = m_scalar_tags;
//    p_output_skeleton->m_scalar_tags_indexes = m_scalar_tags_indexes;


	mxSkeletonNode *psn, *psn2;
	
	psn = destination_node;
	while(psn->GetLinkToSourceNode_2()!=NULL)
	{
		psn2 = psn;
		this->CopyLinkWithItsNodesToOutputSkeleton(psn->GetLinkToSourceNode_2(),p_output_skeleton,t2);
		if(psn==psn->GetLinkToSourceNode_2()->GetNode_1()) psn = psn->GetLinkToSourceNode_2()->GetNode_2();
		else psn = psn->GetLinkToSourceNode_2()->GetNode_1();

		if(psn2==psn)
		{
			std::cout<<"psn2==psn! Exit!"<<std::endl;
			return 0;
		}
	}

	p_output_skeleton->m_origin[0] = m_origin[0]; p_output_skeleton->m_origin[1] = m_origin[1];
	p_output_skeleton->m_origin[2] = m_origin[2]; p_output_skeleton->m_origin[3] = m_origin[3];
	p_output_skeleton->m_spacing[0] = m_spacing[0]; p_output_skeleton->m_spacing[1] = m_spacing[1]; 
	p_output_skeleton->m_spacing[2] = m_spacing[2]; p_output_skeleton->m_spacing[3] = m_spacing[3]; 

	return 1;
}



int mxSkeleton::BestPathToTwoConvergedNodes(mxSkeletonNode *current_node, mxSkeleton *p_output_skeleton, double *distance, unsigned int t2)
{
	if(current_node->GetSkeleton()!=this) return 0;
	p_output_skeleton->ResetTimeIndex(t2);

	this->BestPathToNode(current_node, p_output_skeleton,t2);
	mxSkeleton sk;
	this->BestPathToNode_2(current_node, &sk,t2);
	p_output_skeleton->CopyFrom(&sk);

	*distance = current_node->GetMetricValue() + current_node->GetMetricValue_2();

	p_output_skeleton->m_origin[0] = m_origin[0]; p_output_skeleton->m_origin[1] = m_origin[1];
	p_output_skeleton->m_origin[2] = m_origin[2]; p_output_skeleton->m_origin[3] = m_origin[3];
	p_output_skeleton->m_spacing[0] = m_spacing[0]; p_output_skeleton->m_spacing[1] = m_spacing[1];
	p_output_skeleton->m_spacing[2] = m_spacing[2]; p_output_skeleton->m_spacing[3] = m_spacing[3];
	
	return 1;
}


//void mxSkeleton::SaveToTextFileOfPositions_v1(const char *file_name)
//{
//    unsigned int t = 0;
//
//    mxString bds, bds_dir, bds_file_name, bds_ext;
//    bds.Assign(file_name);
//    bds.ExtractFilePathParts(bds_dir, bds_file_name, bds_ext);
//    if(!(bds_ext=="skl" || bds_ext=="SKL"))
//    {
//        bds.Append(".skl");
//    }
//
//    std::ofstream file;
//    file.open(bds.Get_C_String(),std::ios::binary);
//
//    //----- 1. Write the version, origin, spacing and tags -----
//    file<<"%bdSkeleton File:" <<std::endl;
//    file<<"v"<<std::endl;
//    file<<"1.0"<<std::endl;
//    file<<"o"<<std::endl;//Letter 'o' indicates that from this moment on, the origin is recorded
//    file<<(this->GetOrigin_S())<<std::endl;
//    file<<(this->GetOrigin_R())<<std::endl;
//    file<<(this->GetOrigin_C())<<std::endl;
//    file<<"s"<<std::endl;//Letter 's' indicates that from this moment on, the spacing is recorded
//    file<<(this->GetSpacing_S())<<std::endl;
//    file<<(this->GetSpacing_R())<<std::endl;
//    file<<(this->GetSpacing_C())<<std::endl;
//    file<<"t"<<std::endl;//Letter 't' indicates that from this moment on, tags are recorded
//
//    mxArray<int> indexes_of_recorded_tags;
//    indexes_of_recorded_tags.SetNumberOfElements(m_scalar_tags.GetNumberOfElements());
//    int j=0;
//    mxListIterator<unsigned int> it_i; it_i.SetToBegin(m_scalar_tags_indexes);
//    mxListIterator<mxString> it_s; it_s.SetToBegin(m_scalar_tags);
//    while(it_s.IsValid() && it_i.IsValid())
//    {
//        file<<"%"<<it_s.GetElement()<<std::endl;
//        indexes_of_recorded_tags[j] = it_i.GetElement();
//        it_i.MoveToNext();
//        it_s.MoveToNext();
//        j++;
//    }
//    //-----
//
//    //----- 2. Write all the links -----
//    file<<"l"<<std::endl;//Letter 'l' indicates that from this moment on, the links are recorded
//    mxSkeletonLink *p_link;
//    mxListIterator<mxSkeletonLink> itsl;
//    for(itsl.SetToBegin(this->GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
//    {
//        p_link = itsl.GetElementAddress();
//        mxPoint pos = *(p_link->GetNode_1_Position());
//        pos.Print(file,indexes_of_recorded_tags);
//        mxListIterator<mxPoint> itp;
//        for(itp.SetToBegin(p_link->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
//        {
//            pos = itp.GetElement();
//            pos.Print(file,indexes_of_recorded_tags);
//        }
//        pos = *(p_link->GetNode_2_Position());
//        pos.Print(file,indexes_of_recorded_tags);
//
//        //Write all the skeletonization points of the link
//        mxVoxel p;
//        mxListIterator<mxVoxel> itpt;
//        for(itpt.SetToBegin(p_link->GetSkeletonizationVoxels()); itpt.IsValid(); itpt.MoveToNext())
//        {
//            p = itpt.GetElement();
//            file<<p;
//        }
//        file<<std::endl;
//    }
//    //-----
//
//    //----- 3. Write all the nodes -----
//    file<<"n"<<std::endl;//Letter 'n' indicates that from this moment on, the nodes are recorded
//    mxSkeletonNode *p_node;
//    mxListIterator<mxSkeletonNode> itn;
//    for(itn.SetToBegin(this->GetNodes(t)); itn.IsValid(); itn.MoveToNext())
//    {
//        p_node = itn.GetElementAddress();
//        //file<<(p_node->GetPosition());
//        p_node->GetPosition().Print(file,indexes_of_recorded_tags);
//        mxVoxel p;
//        mxListIterator<mxVoxel> itpt;
//        //for(itpt.SetToBegin(p_link->GetSkeletonizationVoxels()); itpt.IsValid(); itpt.MoveToNext())
//        for(itpt.SetToBegin(p_node->GetSkeletonizationVoxels()); itpt.IsValid(); itpt.MoveToNext())
//        {
//            p = itpt.GetElement();
//            file<<p;
//        }
//        file<<std::endl;
//    }
//    //-----
//
//    file.close();
//}


//void mxSkeleton::SaveToTextFileOfPositions_v2(const char *file_name, const char *comment)
//{
//    mxString bds, bds_dir, bds_file_name, bds_ext;
//    bds.Assign(file_name);
//    bds.ExtractFilePathParts(bds_dir, bds_file_name, bds_ext);
//    if(!(bds_ext=="skl" || bds_ext=="SKL"))
//    {
//        bds.Append(".skl");
//    }
//
//    std::ofstream file;
//    file.open(bds.Get_C_String(),std::ios::binary);
//
//    //----- 1. Write the version, origin, spacing and tags -----
//    file<<"%bdSkeleton File:" <<std::endl;
//    file<<"v"<<std::endl;
//    file<<"2.0"<<std::endl;
//    file<<"c"<<std::endl;//Letter 'c' indicates that from this moment on, the comment is recorded
//    if(comment) file<<"%"<<comment<<std::endl;
//    else file<<"% "<<std::endl;
//    file<<"h"<<std::endl;//Letter 'h' indicates that from this moment on, the history is recorded
//    file<<"%"<<" "<<std::endl;
//    file<<"T"<<std::endl;//Letter 'T' indicates that from this moment on, the number of time instances is recorded
//    file<<this->GetNumberOfTimeSeries()<<std::endl;
//    file<<"o"<<std::endl;//Letter 'o' indicates that from this moment on, the origin is recorded
//    file<<(this->GetOrigin_T())<<std::endl;
//    file<<(this->GetOrigin_S())<<std::endl;
//    file<<(this->GetOrigin_R())<<std::endl;
//    file<<(this->GetOrigin_C())<<std::endl;
//    file<<"s"<<std::endl;//Letter 's' indicates that from this moment on, the spacing is recorded
//    file<<(this->GetSpacing_T())<<std::endl;
//    file<<(this->GetSpacing_S())<<std::endl;
//    file<<(this->GetSpacing_R())<<std::endl;
//    file<<(this->GetSpacing_C())<<std::endl;
//    file<<"t"<<std::endl;//Letter 't' indicates that from this moment on, tags are recorded
//    mxArray<int> indexes_of_recorded_tags;
//    indexes_of_recorded_tags.SetNumberOfElements(m_scalar_tags.GetNumberOfElements());
//    int j=0;
//    mxListIterator<unsigned int> it_i; it_i.SetToBegin(m_scalar_tags_indexes);
//    mxListIterator<mxString> it_s; it_s.SetToBegin(m_scalar_tags);
//    while(it_s.IsValid() && it_i.IsValid())
//    {
//        file<<"%"<<it_s.GetElement()<<std::endl;
//        indexes_of_recorded_tags[j] = it_i.GetElement();
//        it_i.MoveToNext();
//        it_s.MoveToNext();
//        j++;
//    }
//    //-----
//
//    //----- 2. Write all the links -----
//    for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
//    {
//        file<<"l"<<t<<std::endl;//Letter 'l' indicates that from this moment on, the links are recorded
//        mxSkeletonLink *p_link;
//        mxListIterator<mxSkeletonLink> itsl;
//        for(itsl.SetToBegin(this->GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
//        {
//            p_link = itsl.GetElementAddress();
//            mxPoint pos = *(p_link->GetNode_1_Position());
//            pos.Print(file,indexes_of_recorded_tags);
//            mxListIterator<mxPoint> itp;
//            for(itp.SetToBegin(p_link->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
//            {
//                pos = itp.GetElement();
//                pos.Print(file,indexes_of_recorded_tags);
//            }
//            pos = *(p_link->GetNode_2_Position());
//            pos.Print(file,indexes_of_recorded_tags);
//
//            //Write all the skeletonization points of the link
//            mxVoxel p;
//            mxListIterator<mxVoxel> itpt;
//            for(itpt.SetToBegin(p_link->GetSkeletonizationVoxels()); itpt.IsValid(); itpt.MoveToNext())
//            {
//                p = itpt.GetElement();
//                file<<p;
//            }
//            file<<std::endl;
//        }
//    }
//    //-----
//
//    //----- 3. Write all the nodes -----
//    for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
//    {
//        file<<"n"<<t<<std::endl;//Letter 'n' indicates that from this moment on, the nodes are recorded
//        mxSkeletonNode *p_node;
//        mxListIterator<mxSkeletonNode> itn;
//        for(itn.SetToBegin(this->GetNodes(t)); itn.IsValid(); itn.MoveToNext())
//        {
//            p_node = itn.GetElementAddress();
//            p_node->GetPosition().Print(file,indexes_of_recorded_tags);
//            mxVoxel p;
//            mxListIterator<mxVoxel> itpt;
//            for(itpt.SetToBegin(p_node->GetSkeletonizationVoxels()); itpt.IsValid(); itpt.MoveToNext())
//            {
//                p = itpt.GetElement();
//                file<<p;
//            }
//            file<<std::endl;
//        }
//    }
//    //-----
//
//    file.close();
//}


void mxSkeleton::SaveToTextFileOfPositions_v3(const char *file_name, const char *comment)
{
    
    mxString bds, bds_dir, bds_file_name, bds_ext;
    bds.Assign(file_name);
    bds.ExtractFilePathParts(bds_dir, bds_file_name, bds_ext);
    if(!(bds_ext=="skl" || bds_ext=="SKL"))
    {
        bds.Append(".skl");
    }
    
    std::ofstream file;
    file.open(bds.Get_C_String(), std::ios::binary);
    
    //----- 1. Write the version, origin, spacing and tags -----
    file<<"%bdSkeleton File:" <<std::endl;
    file<<"v"<<std::endl;
    file<<"3.0"<<std::endl;
    file<<"c"<<std::endl;//Letter 'c' indicates that from this moment on, the comment is recorded
    if(comment) file<<"%"<<comment<<std::endl;
    else file<<"% "<<std::endl;
    file<<"h"<<std::endl;//Letter 'h' indicates that from this moment on, the history is recorded
    file<<"%"<<" "<<std::endl;;
    file<<"T"<<std::endl;//Letter 'T' indicates that from this moment on, the number of time instances is recorded
    file<<this->GetNumberOfTimeSeries()<<std::endl;
    file<<"o"<<std::endl;//Letter 'o' indicates that from this moment on, the origin is recorded
    file<<(this->GetOrigin_T())<<std::endl;
    file<<(this->GetOrigin_S())<<std::endl;
    file<<(this->GetOrigin_R())<<std::endl;
    file<<(this->GetOrigin_C())<<std::endl;
    file<<"s"<<std::endl;//Letter 's' indicates that from this moment on, the spacing is recorded
    file<<(this->GetSpacing_T())<<std::endl;
    file<<(this->GetSpacing_S())<<std::endl;
    file<<(this->GetSpacing_R())<<std::endl;
    file<<(this->GetSpacing_C())<<std::endl;
    file<<"d"<<std::endl;//Letter 'd' indicates that from this moment on, the direction cosines (orientation) is recorded
    file<<"1"<<std::endl;
    file<<"0"<<std::endl;
    file<<"0"<<std::endl;
    file<<"0"<<std::endl;
    file<<"1"<<std::endl;
    file<<"0"<<std::endl;
    
    // Tags for scalars
    file<<"t"<<std::endl;
    
    int number_of_recorded_tags = 0;
    for(int i=0; i<this->GetMaximumNumberOfScalars(); i++)
    {
        if(this->m_scalars[i].m_is_used)
        {
            number_of_recorded_tags++;
        }
    }
    mxArray<int> indexes_of_recorded_tags;
    indexes_of_recorded_tags.SetNumberOfElements(number_of_recorded_tags);
    
    //std::cout<<std::endl<<"this->GetMaximumNumberOfScalars()="<<this->GetMaximumNumberOfScalars();
    for(int i=0; i<this->GetMaximumNumberOfScalars(); i++)
    {
        //std::cout<<std::endl<<"this->m_scalars["<<i<<"].m_is_used="<<this->m_scalars[i].m_is_used;
        if(this->m_scalars[i].m_is_used)
        {
            file<<"%"<<m_scalars[i].m_tag.Get_C_String();
            
            // if visualization range is correct, we add info on the visualization range
            if(this->m_scalars[i].m_scalar_visualization_range[0] <  this->m_scalars[i].m_scalar_visualization_range[1])
            {
                mxString s;
                s.Assign("{");
                s.AppendNumber(this->m_scalars[i].m_scalar_visualization_range[0]);
                s.Append(", ");
                s.AppendNumber(this->m_scalars[i].m_scalar_visualization_range[1]);
                s.Append("}");
                file<<s.Get_C_String();
            }
            file<<std::endl;
        }
    }
    
//    file<<"t"<<std::endl;//Letter 't' indicates that from this moment on, tags are recorded
//    mxArray<int> indexes_of_recorded_tags;
//    indexes_of_recorded_tags.SetNumberOfElements(m_scalar_tags.GetNumberOfElements());
//    int j=0;
//    mxListIterator<unsigned int> it_i; it_i.SetToBegin(m_scalar_tags_indexes);
//    mxListIterator<mxString> it_s; it_s.SetToBegin(m_scalar_tags);
//    while(it_s.IsValid() && it_i.IsValid())
//    {
//        file<<"%"<<it_s.GetElement()<<std::endl;
//        indexes_of_recorded_tags[j] = it_i.GetElement();
//        it_i.MoveToNext();
//        it_s.MoveToNext();
//        j++;
//    }
    //-----
    
    //----- 2. Write all the links -----
    for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
    {
        file<<"l"<<t<<std::endl;//Letter 'l' indicates that from this moment on, the links are recorded
        mxSkeletonLink *p_link;
        mxListIterator<mxSkeletonLink> itsl;
        for(itsl.SetToBegin(this->GetLinks(t)); itsl.IsValid(); itsl.MoveToNext())
        {
            p_link = itsl.GetElementAddress();
            mxPoint pos = *(p_link->GetNode_1_Position());
            pos.Print(file,indexes_of_recorded_tags);
            mxListIterator<mxPoint> itp;
            for(itp.SetToBegin(p_link->GetPositionsList()); itp.IsValid(); itp.MoveToNext())
            {
                pos = itp.GetElement();
                pos.Print(file,indexes_of_recorded_tags);
            }
            pos = *(p_link->GetNode_2_Position());
            pos.Print(file,indexes_of_recorded_tags);
            
            //Write all the skeletonization points of the link
            mxVoxel p;
            mxListIterator<mxVoxel> itpt;
            for(itpt.SetToBegin(p_link->GetSkeletonizationVoxels()); itpt.IsValid(); itpt.MoveToNext())
            {
                p = itpt.GetElement();
                file<<p;
            }
            file<<std::endl;
        }
    }
    //-----
    
    //----- 3. Write all the nodes -----
    for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
    {
        file<<"n"<<t<<std::endl;//Letter 'n' indicates that from this moment on, the nodes are recorded
        mxSkeletonNode *p_node;
        mxListIterator<mxSkeletonNode> itn;
        for(itn.SetToBegin(this->GetNodes(t)); itn.IsValid(); itn.MoveToNext())
        {
            p_node = itn.GetElementAddress();
            p_node->GetPosition().Print(file,indexes_of_recorded_tags);
            mxVoxel p;
            mxListIterator<mxVoxel> itpt;
            for(itpt.SetToBegin(p_node->GetSkeletonizationVoxels()); itpt.IsValid(); itpt.MoveToNext())
            {
                p = itpt.GetElement();
                file<<p;
            }
            file<<std::endl;
        }
    }
    //-----
    
    file.close();
}


int mxSkeleton::LoadFromTextFileOfPositions_v2(const char *file_name, mxString *output_comment)
{
	if(!IsEmpty()) this->Reset();

    std::ifstream input_file;
    input_file.open(file_name, std::ios::binary);
    if(!input_file)
	{
		std::cout<<"mxSkeleton::LoadFromTextFileOfPositions() : Unable to open skeleton text file: "<<file_name<<std::endl;
		return 0;
    }

	char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
	int line_size = 99999;

	mxSkeletonLink *psl;
	mxSkeletonNode *psn;
	mxPoint _start_node_position, _end_node_position;


	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='S' || text_line[4]!='k' || text_line[5]!='e' || text_line[6]!='l' ||
		text_line[7]!='e' || text_line[8]!='t' || text_line[9]!='o' || text_line[10]!='n' || text_line[11]!=' ' || text_line[12]!='F' ||
		text_line[13]!='i' || text_line[14]!='l' || text_line[15]!='e' || text_line[16]!=':') { input_file.close(); return 0; }

	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='v') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	mxString bds;
	bds.Append(text_line);
	mxList<int> list;
	bds.ExtractNumbers(list);
	if(list.IsEmpty())
	{
		std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): No version value found!"<<std::endl;
		input_file.close();
		return 0;
	}
	if(list.GetBeginElement()>2)
	{
		std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
		input_file.close();
		return 0;
	}

	//--- Read the Comment ---
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='c') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='%') { input_file.close(); return 0; }
	if(output_comment) 
	{
		output_comment->Clear();
		output_comment->Append(text_line[1]);
	}


	//--- Read the History ---
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='h') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='%') { input_file.close(); return 0; }
	// AT THIS POINT WE DON"T DO ANYTHING WITH THIS (history)


	//--- Read the Number of Time Sequences ---
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='T') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	int n_of_T = atoi(text_line);
	if(n_of_T<=0) { input_file.close(); return 0; }
	this->SetNumberOfTimeSeries((unsigned int)n_of_T);


	//--- Read Origin ---
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='o') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_origin[3] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_origin[2] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_origin[1] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_origin[0] = atof(text_line);


	//--- Read Spacing ---
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='s') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_spacing[3] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_spacing[2] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_spacing[1] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_spacing[0] = atof(text_line);


	//--- Read Tags ---
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='t') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	for(unsigned int tag_number=0; text_line[0]=='%'; tag_number++)
	{
        mxString tag;
        tag.Assign(&(text_line[1]));
        unsigned int tag_index;
        this->AddScalarTag(tag, tag_index);
        //this->GetScalarTagElement(tag_index)->m_scalar_visualization_range;
        
		//mxString *tag = m_scalar_tags.AddNewToEnd();
		//tag->Append(&(text_line[1]));
		//m_scalar_tags_indexes.AddToEnd(tag_number);
		if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	}


	//Time index for loading the 4D skeleton.
	unsigned int t = 0;

	if(text_line[0]!='l') { input_file.close(); return 0;  }

	int is_reading_nodes = 0;//Indicator if the nodes are being read.
	{//Limit the variables
		//Read the time index.
		mxString time_index_string;
		time_index_string.Append(text_line);
		mxList<int> l;
		time_index_string.ExtractNumbers(l);
		if(l.GetBeginElement() < 0) { input_file.close(); return 0; }
		t = (unsigned int) (l.GetBeginElement());
	}

	while(input_file.getline(text_line, line_size))
	{
		if(text_line[0]=='n' || text_line[0]=='l') 
		{
			if(text_line[0]=='n') { is_reading_nodes = 1; }//If it found the letter 'n', from this moment on, we read the nodes
			
			//Read the time index.
			mxString time_index_string;
			time_index_string.Append(text_line);
			mxList<int> l;
			time_index_string.ExtractNumbers(l);
			if(l.GetBeginElement() < 0) { input_file.close(); return 0; }
			t = (unsigned int) (l.GetBeginElement());
			//std::cout<<"t="<<t<<" ";
			
		}
		else
		{
			//---------- 1. If the links are being read ----------
			if(!is_reading_nodes)
			{				
				//If a new line is read, create a new link and load it from string
				psl = this->AddLink(t);

				if(!(psl->LoadFromStringOfPositions(text_line,_start_node_position, _end_node_position)))
				{
					std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error loading link from string: "<<text_line<<std::endl;
					input_file.close();
					return 0;
				}

				//Now we have to check if the nodes with the positions '_start_node_position' and '_end_node_position' exist in the list of nodes,
				// if so - we have to connect them with the links, otherwise create them and then connect with the links

				//----- 1a. Check the start node -----

				//If the node with position '_start_node_position' already exist...
				if(GetNodeForPosition(_start_node_position, &psn, t))
				{
					//... connect link with the node...
					psl->SetNode_1(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}
				//If the node with the '_start_node_position' position does not exist in the list of nodes...
				else
				{
					//... create a new node...	
					this->AddNodeWithPosition(_start_node_position,&psn,t);
					//... connect the pointer of the link to the node...
					psl->SetNode_1(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}

				//----- 1b. Check the end node -----

				//If the node with position '_end_node_position' already exist...
				if(GetNodeForPosition(_end_node_position, &psn, t))
				{
					//... connect link with the node...
					psl->SetNode_2(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}
				//If the node with the '_end_node_position' position does not exist in the list of nodes...
				else
				{
					//... create a new node...			
					this->AddNodeWithPosition(_end_node_position,&psn,t);
					//... connect the pointer of the link to the node...
					psl->SetNode_2(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}
			}
			//----------
			
			//---------- 2. If the nodes are being read ----------
			else
			{
				//std::cout<<"READING NODES!"<<std::endl;
				//...first read the position of the node...				
				int count  = 0;

				char *temp;

				//Check for the open brackets for coordinates of mxPoint
				if(text_line[0]!='(') 
				{
					std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Unable to find '(' for reading Positions!"<<std::endl;
					input_file.close();
					return 0;
				}
				else
				{
					//Find the '}' and the count of float number
					for(count = 0; text_line[count]!='}'; count++)
					{
						if(text_line[count]=='\0') 
						{
							std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error in string: (found '\0') unexpected end of string!"<<std::endl;
							input_file.close();
							return 0;//error in string
						}
					}

					//Copy the string
					temp = new char [count+2];
					temp[count+1] = '\0';
                    for(int j=0; j<count+1; j++)
                    {
                        temp[j] = text_line[j];
                    }

					//Read the position of the node
					mxPoint pos;
					if(!pos.ReadFromString(temp))
					{
						std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error reading mxPoint of node!"<<std::endl;
						input_file.close();
						return 0;
					}

					//Find the node in the skeleton by its position
					if(!this->GetNodeForPosition(pos, &psn, t)) 
					{
						this->AddNodeWithPosition(pos,&psn, t);
					}

					if(!psn->LoadFromStringOfPoints(temp))
					{
						std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error reading node string of Points!"<<std::endl;
						input_file.close();
						return 0;
					}

					//Release string
					delete [] temp;
				}				
			}
			//----------
		}
	}
	
	input_file.close();

	return 1;
}



int mxSkeleton::LoadFromTextFileOfPositions_v3(const char *file_name, mxString *output_comment)
{
    if(!IsEmpty()) this->Reset();
    
    std::ifstream input_file;
    input_file.open(file_name, std::ios::binary);
    if(!input_file)
    {
        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions() : Unable to open skeleton text file: "<<file_name<<std::endl;
        return 0;
    }
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    int line_size = 99999;
    
    mxSkeletonLink *psl;
    mxSkeletonNode *psn;
    mxPoint _start_node_position, _end_node_position;
    
    
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='S' || text_line[4]!='k' || text_line[5]!='e' || text_line[6]!='l' ||
       text_line[7]!='e' || text_line[8]!='t' || text_line[9]!='o' || text_line[10]!='n' || text_line[11]!=' ' || text_line[12]!='F' ||
       text_line[13]!='i' || text_line[14]!='l' || text_line[15]!='e' || text_line[16]!=':') { input_file.close(); return 0; }
    
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='v') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    mxString bds;
    bds.Append(text_line);
    mxList<int> list;
    bds.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): No version value found!"<<std::endl;
        input_file.close();
        return 0;
    }
    if(list.GetBeginElement()>3)
    {
        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
        input_file.close();
        return 0;
    }

    //--- Read the Comment ---
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='c') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='%') { input_file.close(); return 0; }
    if(output_comment)
    {
        output_comment->Clear();
        output_comment->Append(text_line[1]);
    }
    
    //--- Read the History ---
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='h') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='%') { input_file.close(); return 0; }
    // AT THIS POINT WE DON"T DO ANYTHING WITH THIS (history)
    
    
    //--- Read the Number of Time Sequences ---
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='T') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    int n_of_T = atoi(text_line);
    if(n_of_T<=0) { input_file.close(); return 0; }
    this->SetNumberOfTimeSeries((unsigned int)n_of_T);
    
    //--- Read Origin ---
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='o') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_origin[3] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_origin[2] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_origin[1] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_origin[0] = atof(text_line);
    
    //--- Read Spacing ---
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='s') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_spacing[3] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_spacing[2] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_spacing[1] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_spacing[0] = atof(text_line);
    
    //--- Read Orientation ---
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='d') { input_file.close(); return 0; }
    double rx,ry,rz,cx,cy,cz;
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    cx = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    cy = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    cz = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    rx = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    ry = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    rz = atof(text_line);
    // AT THIS POINT WE DON"T HANDLE ORIENTATION
    
    
    //--- Read Tags ---
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='t') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    for(unsigned int scalar_index=0; text_line[0]=='%'; scalar_index++)
    {
        this->m_scalars[scalar_index].m_is_used = 1;
        
        mxString s, extracted_s;
        s.Assign(&(text_line[1]));
        if(s.ExtractString('{', '}', extracted_s))
        {
            mxList<double> list_of_doubles;
            extracted_s.ExtractNumbers(list_of_doubles);
            
            if(list_of_doubles.GetNumberOfElements()==2)
            {
                this->m_scalars[scalar_index].m_scalar_visualization_range[0] = list_of_doubles[0];
                this->m_scalars[scalar_index].m_scalar_visualization_range[1] = list_of_doubles[1];
            }
            
            mxString s2, name_s;
            s2.Assign(text_line);
            s2.ExtractString('%','{', name_s);
            
            this->m_scalars[scalar_index].m_tag.Assign(name_s);
        }
        else
        {
            this->m_scalars[scalar_index].m_tag.Assign(&(text_line[1]));
        }
        
        if(!input_file.getline(text_line, line_size)) return 0;
    }
    
    //Time index for loading the 4D skeleton.
    unsigned int t = 0;
    
    if(text_line[0]!='l') { input_file.close(); return 0;  }
    
    int is_reading_nodes = 0;//Indicator if the nodes are being read.
    {//Limit the variables
        //Read the time index.
        mxString time_index_string;
        time_index_string.Append(text_line);
        mxList<int> l;
        time_index_string.ExtractNumbers(l);
        if(l.GetBeginElement() < 0) { input_file.close(); return 0; }
        t = (unsigned int) (l.GetBeginElement());
    }
    
    while(input_file.getline(text_line, line_size))
    {
        if(text_line[0]=='n' || text_line[0]=='l')
        {
            if(text_line[0]=='n') {std::cout<<" N: "; is_reading_nodes = 1;}//If it found the letter 'n', from this moment on, we read the nodes
            
            //Read the time index.
            mxString time_index_string;
            time_index_string.Append(text_line);
            mxList<int> l;
            time_index_string.ExtractNumbers(l);
            if(l.GetBeginElement() < 0) { input_file.close(); return 0; }
            t = (unsigned int) (l.GetBeginElement());
            std::cout<<"t="<<t<<" ";
            
        }
        else
        {
            //---------- 1. If the links are being read ----------
            if(!is_reading_nodes)
            {
                //If a new line is read, create a new link and load it from string
                psl = this->AddLink(t);
                
                if(!(psl->LoadFromStringOfPositions(text_line,_start_node_position, _end_node_position)))
                {
                    std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error loading link from string: "<<text_line<<std::endl;
                    input_file.close();
                    return 0;
                }
                
                //Now we have to check if the nodes with the positions '_start_node_position' and '_end_node_position' exist in the list of nodes,
                // if so - we have to connect them with the links, otherwise create them and then connect with the links
                
                //----- 1a. Check the start node -----
                
                //If the node with position '_start_node_position' already exist...
                if(GetNodeForPosition(_start_node_position, &psn, t))
                {
                    //... connect link with the node...
                    psl->SetNode_1(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
                //If the node with the '_start_node_position' position does not exist in the list of nodes...
                else
                {
                    //... create a new node...
                    this->AddNodeWithPosition(_start_node_position,&psn,t);
                    //... connect the pointer of the link to the node...
                    psl->SetNode_1(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
                
                //----- 1b. Check the end node -----
                
                //If the node with position '_end_node_position' already exist...
                if(GetNodeForPosition(_end_node_position, &psn, t))
                {
                    //... connect link with the node...
                    psl->SetNode_2(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
                //If the node with the '_end_node_position' position does not exist in the list of nodes...
                else
                {
                    //... create a new node...
                    this->AddNodeWithPosition(_end_node_position,&psn,t);
                    //... connect the pointer of the link to the node...
                    psl->SetNode_2(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
            }
            //----------
            
            //---------- 2. If the nodes are being read ----------
            else
            {
                //std::cout<<"READING NODES!"<<std::endl;
                //...first read the position of the node...
                int count  = 0;
                
                char *temp;
                
                //Check for the open brackets for coordinates of mxPoint
                if(text_line[0]!='(')
                {
                    std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Unable to find '(' for reading Positions!"<<std::endl;
                    input_file.close();
                    return 0;
                }
                else
                {
                    //Find the '}' and the count of float number
                    for(count = 0; text_line[count]!='}'; count++)
                    {
                        if(text_line[count]=='\0') 
                        {
                            std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error in string: (found '\0') unexpected end of string!"<<std::endl;
                            input_file.close();
                            return 0;//error in string
                        }
                    }
                    
                    //Copy the string
                    temp = new char [count+2];
                    temp[count+1] = '\0';
                    for(int j=0; j<count+1; j++)
                    {
                        temp[j] = text_line[j];
                    }
                    
                    //Read the position of the node
                    mxPoint pos;
                    if(!pos.ReadFromString(temp))
                    {
                        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error reading mxPoint of node!"<<std::endl;
                        input_file.close();
                        return 0;
                    }
                    
                    //Find the node in the skeleton by its position
                    if(!this->GetNodeForPosition(pos, &psn, t)) 
                    {
                        this->AddNodeWithPosition(pos,&psn, t);

                    }
                    
                    if(!psn->LoadFromStringOfPoints(temp))
                    {
                        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error reading node string of Points!"<<std::endl;
                        input_file.close();
                        return 0;
                    }
                    
                    //Release string
                    delete [] temp;
                }				
            }
            //----------
        }
    }
    
    input_file.close();
    
    return 1;
}



int mxSkeleton::FileVersion(const char *file_name, int &output_version)
{
    std::ifstream input_file;
    input_file.open(file_name, std::ios::binary);
    if(!input_file)
	{
		std::cout<<"mxSkeleton::FileVersion() : Unable to open skeleton text file: "<<file_name<<std::endl;
		return 0;
    }

	char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
	int line_size = 99999;

	mxPoint _start_node_position, _end_node_position;


	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='S' || text_line[4]!='k' || text_line[5]!='e' || text_line[6]!='l' ||
		text_line[7]!='e' || text_line[8]!='t' || text_line[9]!='o' || text_line[10]!='n' || text_line[11]!=' ' || text_line[12]!='F' ||
		text_line[13]!='i' || text_line[14]!='l' || text_line[15]!='e' || text_line[16]!=':') { input_file.close(); return 0; }

	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='v') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	mxString bds;
	bds.Append(text_line);
	mxList<int> list;
	bds.ExtractNumbers(list);
	if(list.IsEmpty())
	{
		std::cout<<"mxSkeleton::FileVersion(): No version value found!"<<std::endl;
		input_file.close();
		return 0;
	}

	output_version = list.GetBeginElement();
	input_file.close();
	return 1;
}


void mxSkeleton::SaveToTextFileOfPositions(const char *file_name)
{
	this->SaveToTextFileOfPositions_v3(file_name);
}


int mxSkeleton::LoadFromTextFileOfPositions(const char *file_name)
{
	int version;
	if(!this->FileVersion(file_name,version))
    {
        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): cannot read file version, trying legacy format..."<<std::endl;
        
        if(this->LoadFromTextFileOfPositions_Legacy(file_name)) { std::cout<<" success!"<<std::endl; return 1; }
        
        std::cout<<" failed."<<std::endl;
        return 0;
    }

	if(version==1) return (this->LoadFromTextFileOfPositions_v1(file_name));
	if(version==2) return (this->LoadFromTextFileOfPositions_v2(file_name));
    if(version==3) return (this->LoadFromTextFileOfPositions_v3(file_name));
    
	std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Unsuppoerted version!"<<std::endl;
	return 0;
}


int mxSkeleton::LoadFromTextFileOfPositions_Legacy(const char *file_name)
{
    // WITH EVERY "RETURN 0" IT SHOULD ALSO CLOSE THE FILE !
    if(!IsEmpty()) this->Reset();
    
    unsigned int t = 0;
    
    std::ifstream input_file;
    input_file.open(file_name, std::ios::binary);
    if(!input_file)
    {
        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy() : Unable to open skeleton text file: "<<file_name<<std::endl;
        return 0;
    }
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    int line_size = 99999;
    
    mxSkeletonLink *psl;
    mxSkeletonNode *psn;
    mxPoint _start_node_position, _end_node_position;
    
    
//    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
//    if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='S' || text_line[4]!='k' || text_line[5]!='e' || text_line[6]!='l' ||
//       text_line[7]!='e' || text_line[8]!='t' || text_line[9]!='o' || text_line[10]!='n' || text_line[11]!=' ' || text_line[12]!='F' ||
//       text_line[13]!='i' || text_line[14]!='l' || text_line[15]!='e' || text_line[16]!=':') { input_file.close(); return 0; }
    
//    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
//    if(text_line[0]!='v') { input_file.close(); return 0; }
//    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
//    mxString bds;
//    bds.Append(text_line);
//    mxList<int> list;
//    bds.ExtractNumbers(list);
//    if(list.IsEmpty())
//    {
//        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy(): No version value found!"<<std::endl;
//        input_file.close();
//        return 0;
//    }
//    if(list.GetBeginElement()>1)
//    {
//        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
//        input_file.close();
//        return 0;
//    }
    
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='o') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_origin[2] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_origin[1] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_origin[0] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    if(text_line[0]!='s') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_spacing[2] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_spacing[1] = atof(text_line);
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
    m_spacing[0] = atof(text_line);
    
    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
//    if(text_line[0]!='t') { input_file.close(); return 0; }
//    if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
//    for(unsigned int tag_number=0; text_line[0]=='%'; tag_number++)
//    {
//        mxString *tag = m_scalar_tags.AddNewToEnd();
//        tag->Append(&(text_line[1]));
//        m_scalar_tags_indexes.AddToEnd(tag_number);
//        if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
//    }
    
    if(text_line[0]!='l') { input_file.close(); return 0; }
    
    int is_reading_nodes = 0;//Indicator if the nodes are being read.
    
    while(input_file.getline(text_line, line_size))
    {
        if(text_line[0]=='n') is_reading_nodes = 1;//If it found the letter 'n', from this moment on, we read the nodes
        else
        {
            //---------- 1. If the links are being read ----------
            if(!is_reading_nodes)
            {
                //If a new line is read, create a new link and load it from string
                psl = this->AddLink(t);
                
                if(!(psl->LoadFromStringOfPositions(text_line,_start_node_position, _end_node_position)))
                {
                    std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy(): Error loading link from string: "<<text_line<<std::endl;
                    input_file.close();
                    return 0;
                }
                
                //Now we have to check if the nodes with the positions '_start_node_position' and '_end_node_position' exist in the list of nodes,
                // if so - we have to connect them with the links, otherwise create them and then connect with the links
                
                //----- 1a. Check the start node -----
                
                //If the node with position '_start_node_position' already exist...
                if(GetNodeForPosition(_start_node_position, &psn, t))
                {
                    //... connect link with the node...
                    psl->SetNode_1(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
                //If the node with the '_start_node_position' position does not exist in the list of nodes...
                else
                {
                    //... create a new node...
                    this->AddNodeWithPosition(_start_node_position,&psn,t);
                    //... connect the pointer of the link to the node...
                    psl->SetNode_1(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
                
                //----- 1b. Check the end node -----
                
                //If the node with position '_end_node_position' already exist...
                if(GetNodeForPosition(_end_node_position, &psn, t))
                {
                    //... connect link with the node...
                    psl->SetNode_2(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
                //If the node with the '_end_node_position' position does not exist in the list of nodes...
                else
                {
                    //... create a new node...
                    this->AddNodeWithPosition(_end_node_position,&psn,t);
                    //... connect the pointer of the link to the node...
                    psl->SetNode_2(psn);
                    //... and add pointer to link in the node
                    psn->GetLinksList()->AddToEnd(psl);
                }
            }
            //----------
            
            //---------- 2. If the nodes are being read ----------
            else
            {
                //...first read the position of the node...
                int count  = 0;
                
                char *temp;
                
                //Check for the open brackets for coordinates of mxPoint
                if(text_line[0]!='(')
                {
                    std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy(): Unable to find '(' for reading Positions!"<<std::endl;
                    input_file.close();
                    return 0;
                }
                else
                {
                    //Find the '}' and the count of float number
                    for(count = 0; text_line[count]!='}'; count++)
                    {
                        if(text_line[count]=='\0')
                        {
                            std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy(): Error in string: (found '\0') unexpected end of string!"<<std::endl;
                            input_file.close();
                            return 0;//error in string
                        }
                    }
                    
                    //Copy the string
                    temp = new char [count+2];
                    temp[count+1] = '\0';
                    for(int j=0; j<count+1; j++)
                    {
                        temp[j] = text_line[j];
                    }
                    
                    //Read the position of the node
                    mxPoint pos;
                    if(!pos.ReadFromString(temp))
                    {
                        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy(): Error reading mxPoint of node!"<<std::endl;
                        input_file.close();
                        return 0;
                    }
                    
                    //Find the node in the skeleton by its position
                    if(!this->GetNodeForPosition(pos, &psn, t))
                    {
                        this->AddNodeWithPosition(pos,&psn, t);
                    }
                    
                    if(!psn->LoadFromStringOfPoints(temp))
                    {
                        std::cout<<"mxSkeleton::LoadFromTextFileOfPositions_Legacy(): Error reading node string of Points!"<<std::endl;
                        input_file.close();
                        return 0;
                    }
                    
                    //Release string
                    delete [] temp;
                }
            }
            //----------
        }
    }
    
    input_file.close();
    
    return 1;
}



int mxSkeleton::LoadFromTextFileOfPositions_v1(const char *file_name)
{
    // WITH EVERY "RETURN 0" IT SHOULD ALSO CLOSE THE FILE !
    if(!IsEmpty()) this->Reset();

	unsigned int t = 0;

    std::ifstream input_file;
    input_file.open(file_name, std::ios::binary);
    if(!input_file)
	{
		std::cout<<"mxSkeleton::LoadFromTextFileOfPositions() : Unable to open skeleton text file: "<<file_name<<std::endl;
		return 0;
    }

	char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
	int line_size = 99999;

	mxSkeletonLink *psl;
	mxSkeletonNode *psn;
	mxPoint _start_node_position, _end_node_position;


	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='S' || text_line[4]!='k' || text_line[5]!='e' || text_line[6]!='l' ||
		text_line[7]!='e' || text_line[8]!='t' || text_line[9]!='o' || text_line[10]!='n' || text_line[11]!=' ' || text_line[12]!='F' ||
		text_line[13]!='i' || text_line[14]!='l' || text_line[15]!='e' || text_line[16]!=':') { input_file.close(); return 0; }

	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='v') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	mxString bds;
	bds.Append(text_line);
	mxList<int> list;
	bds.ExtractNumbers(list);
	if(list.IsEmpty())
	{
		std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): No version value found!"<<std::endl;
		input_file.close();
		return 0;
	}
	if(list.GetBeginElement()>1)
	{
		std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
		input_file.close();
		return 0;
	}

	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='o') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_origin[2] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_origin[1] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_origin[0] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='s') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_spacing[2] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_spacing[1] = atof(text_line);
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	m_spacing[0] = atof(text_line);

	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	if(text_line[0]!='t') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	for(unsigned int tag_number=0; text_line[0]=='%'; tag_number++)
	{
        mxString tag;
        tag.Assign(&(text_line[1]));
        unsigned int tag_index;
        this->AddScalarTag(tag, tag_index);
        
//        mxString *tag = m_scalar_tags.AddNewToEnd();
//        tag->Append(&(text_line[1]));
//        m_scalar_tags_indexes.AddToEnd(tag_number);
		if(!input_file.getline(text_line, line_size)) { input_file.close(); return 0; }
	}

	if(text_line[0]!='l') { input_file.close(); return 0; }

	int is_reading_nodes = 0;//Indicator if the nodes are being read.

	while(input_file.getline(text_line, line_size))
	{
		if(text_line[0]=='n') is_reading_nodes = 1;//If it found the letter 'n', from this moment on, we read the nodes
		else
		{
			//---------- 1. If the links are being read ----------
			if(!is_reading_nodes)
			{				
				//If a new line is read, create a new link and load it from string
				psl = this->AddLink(t);

				if(!(psl->LoadFromStringOfPositions(text_line,_start_node_position, _end_node_position)))
				{
					std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error loading link from string: "<<text_line<<std::endl;
                    input_file.close();
					return 0;
				}

				//Now we have to check if the nodes with the positions '_start_node_position' and '_end_node_position' exist in the list of nodes,
				// if so - we have to connect them with the links, otherwise create them and then connect with the links

				//----- 1a. Check the start node -----

				//If the node with position '_start_node_position' already exist...
				if(GetNodeForPosition(_start_node_position, &psn, t))
				{
					//... connect link with the node...
					psl->SetNode_1(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}
				//If the node with the '_start_node_position' position does not exist in the list of nodes...
				else
				{
					//... create a new node...	
					this->AddNodeWithPosition(_start_node_position,&psn,t);
					//... connect the pointer of the link to the node...
					psl->SetNode_1(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}

				//----- 1b. Check the end node -----

				//If the node with position '_end_node_position' already exist...
				if(GetNodeForPosition(_end_node_position, &psn, t))
				{
					//... connect link with the node...
					psl->SetNode_2(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}
				//If the node with the '_end_node_position' position does not exist in the list of nodes...
				else
				{
					//... create a new node...			
					this->AddNodeWithPosition(_end_node_position,&psn,t);
					//... connect the pointer of the link to the node...
					psl->SetNode_2(psn);
					//... and add pointer to link in the node
					psn->GetLinksList()->AddToEnd(psl);
				}
			}
			//----------
			
			//---------- 2. If the nodes are being read ----------
			else
			{
				//...first read the position of the node...
				int count  = 0;

				char *temp;

				//Check for the open brackets for coordinates of mxPoint
				if(text_line[0]!='(') 
				{
					std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Unable to find '(' for reading Positions!"<<std::endl;
                    input_file.close();
					return 0;
				}
				else
				{
					//Find the '}' and the count of float number
					for(count = 0; text_line[count]!='}'; count++)
					{
						if(text_line[count]=='\0') 
						{
							std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error in string: (found '\0') unexpected end of string!"<<std::endl;
                            input_file.close();
							return 0;//error in string
						}
					}

					//Copy the string
					temp = new char [count+2];
					temp[count+1] = '\0';
                    for(int j=0; j<count+1; j++)
                    {
                        temp[j] = text_line[j];
                    }

					//Read the position of the node
					mxPoint pos;
					if(!pos.ReadFromString(temp))
					{
						std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error reading mxPoint of node!"<<std::endl;
                        input_file.close();
						return 0;
					}

					//Find the node in the skeleton by its position
					if(!this->GetNodeForPosition(pos, &psn, t)) 
					{
						this->AddNodeWithPosition(pos,&psn, t);
					}

					if(!psn->LoadFromStringOfPoints(temp))
					{
						std::cout<<"mxSkeleton::LoadFromTextFileOfPositions(): Error reading node string of Points!"<<std::endl;
                        input_file.close();
						return 0;
					}

					//Release string
					delete [] temp;
				}				
			}
			//----------
		}
	}
	
	input_file.close();

	return 1;
}


void mxSkeleton::SaveToDefaultFileType(const char *file_path)
{
    this->SaveToTextFileOfPositions(file_path);
}


int mxSkeleton::ReadPathFromStringOfPositions(const char *string_of_world_positions)
{
    // Extract positions string list
    mxString bds;
    bds.Assign(string_of_world_positions);
    
    mxList<mxString> list_of_position_strings;
    bds.ExtractStrings('|', '|', list_of_position_strings);
    
    
    mxListIterator<mxString> its;
    for(its.SetToBegin(list_of_position_strings); its.IsValid(); its.MoveToNext())
    {
        mxList<double> list_of_doubles;
        its.GetElementAddress()->ExtractNumbers(list_of_doubles);
        
        if(list_of_doubles.GetNumberOfElements() != 4)
        {
            std::cout<<"mxSkeleton::ReadPathFromStringOfPositions(): Error reading coordinates!"<<std::endl;
            return 0;
        }
        unsigned int t = (unsigned int) (list_of_doubles[0]);
        
        // Get the link if it already exists for the given time index, if it doesn't exist create one.
        mxSkeletonLink *psl;
        if(this->GetLinks(t)->GetNumberOfElements()<=0) psl = this->AddLink(t);
        else psl = this->GetLinks(t)->GetBeginElementAddress();
        
        double s,r,c;
        this->LocalPositionFromWorldPosition(list_of_doubles[3], list_of_doubles[2], list_of_doubles[1], c,r,s);
        
        mxPoint pos; pos.SetPosition(s,r,c);
        psl->GetPositionsList()->AddToEnd(pos);
    }
    
    // At this moment all positions are in the links, so we need to create nodes and move first and last positions from each link to their nodes.
    
    // For each of the created links (should be only one link per time instance), create nodes and connect them.
    for(unsigned int t=0; t<this->GetNumberOfTimeSeries(); t++)
    {
        mxSkeletonLink *psl = this->GetLinks(t)->GetBeginElementAddress();
        if(psl)
        {
            //... create a start node...
            mxSkeletonNode *psn;
            this->AddNodeWithPosition(psl->GetPositionsList()->GetBeginElement(),&psn,t);
            //... connect the pointer of the link to the node...
            psl->SetNode_1(psn);
            //... and add pointer to link in the node
            psn->GetLinksList()->AddToEnd(psl);
            
            
            //... create an end node...
            this->AddNodeWithPosition(psl->GetPositionsList()->GetEndElement(),&psn,t);
            //... connect the pointer of the link to the node...
            psl->SetNode_2(psn);
            //... and add pointer to link in the node
            psn->GetLinksList()->AddToEnd(psl);
            
            
            // Remove first and last positions from each link.
            psl->GetPositionsList()->DeleteBegin();
            psl->GetPositionsList()->DeleteEnd();
            
        }
    }
    
    return 1;
}








