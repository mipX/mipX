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



#include "mxPointList.h"





mxPointList::mxPointList()
{
    this->GetClassName().Assign("mxPointList");
}


mxPointList::~mxPointList()
{
	this->Reset();
}


mxPointListElement* mxPointList::AddPosition(double s, double r, double c)
{
    mxPoint p;
    p.SetPosition(s, r, c);
    return this->AddPosition(&p);
}


mxPointListElement* mxPointList::AddPosition(mxPosition *p)
{
    mxPointListElement* e = m_positions.AddNewToEnd();
    e->m_position.mxPosition::CopyFrom(*p);
    e->m_label.AppendNumber((int)(m_positions.GetNumberOfElements()-1));
    return e;
}


mxPointListElement* mxPointList::AddPosition(mxPoint *p)
{
    mxPointListElement* e = m_positions.AddNewToEnd();
    e->m_position.CopyFrom(*p);
    e->m_label.AppendNumber((int)(m_positions.GetNumberOfElements()-1));
    return e;
}


void mxPointList::AddPositions(mxPointList &pl)
{
    mxPointList::Iterator it;
    for(it.SetToBegin(pl.m_positions); it.IsValid(); it.MoveToNext())
    {
        mxPointListElement *pel = it.GetElementAddress();
        mxPointListElement *pel2 = this->AddPosition(&(pel->m_position));
        pel2->m_label.Assign(pel->m_label);
    }
}


int mxPointList::AddScalarTag(const char *tag, unsigned int &output_index)
{
    // Find the lowest available index to add a tag.
    for(int i=0; i<(int)this->GetMaximumNumberOfScalars(); i++)
    {
        if(!m_scalars[i].m_is_used)
        {
            m_scalars[i].m_tag.Assign(tag);
            m_scalars[i].m_is_used = 1;
            output_index = i;
            return 1;
        }
    }
    return 0;
}


int mxPointList::Copy(mxPointList *l)
{
    if (l==this) return 1;
    if(!l) return 0;
    
    this->Reset();
    mxDataObject::Copy(l);
    m_positions = l->m_positions;
    
    // copy scalars.
    for(int i=0; i<(int)this->GetMaximumNumberOfScalars(); i++)
    {
        m_scalars[i].Copy(l->Scalar(i));
    }
    
    return 1;
}


mxPointListElement* mxPointList::GetClosestPointListElement(double s, double r, double c)
{
    if(this->IsEmpty()) return NULL;
    
    mxPointListElement *p_min_distance = NULL;
    mxPoint p;
    p.SetPosition(s,r,c);
    double d_min = -1;
    mxPointList::Iterator it;
    for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
    {
        double d = it.GetElementAddress()->m_position.DistanceEuclidean(p);
        if(d < d_min || d_min < 0)
        {
            p_min_distance = it.GetElementAddress();
            d_min = d;
        }
    }
    return p_min_distance;
}


unsigned int mxPointList::GetNumberOfElements()
{
	return m_positions.GetNumberOfElements();
}


int mxPointList::GetScalarIndex(const char *tag, unsigned int &output_index)
{
    for(int i=0; i<(int)this->GetMaximumNumberOfScalars(); i++)
    {
        if(m_scalars[i].m_is_used)
        {
            if(m_scalars[i].m_tag == tag)
            {
                output_index = i;
                return 1;
            }
        }
    }
    output_index = 0;
    return 0;
}


int mxPointList::GetScalarValueRange(unsigned int scalar_index, double &output_min, double &output_max)
{
    if(this->IsEmpty()) return 0;
    output_min = output_max = -1;
    mxPointList::Iterator it;
    for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElementAddress()->m_position.V(scalar_index) < output_min || output_min < 0)
        {
            output_min = it.GetElementAddress()->m_position.V(scalar_index);
        }
        if(it.GetElementAddress()->m_position.V(scalar_index) > output_max)
        {
            output_max = it.GetElementAddress()->m_position.V(scalar_index);
        }
    }
    return 1;
}


int mxPointList::HasPositionIn3D(mxPosition &position)
{
    mxPointList::Iterator it;
    for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
    {
        if(it.GetElementAddress()->m_position.IsEqualPositionIn_3D(position))
        {
            return 1;
        }
    }
    return 0;
}


int mxPointList::IsEmpty()
{
    return m_positions.IsEmpty();
}


int mxPointList::LoadFromPSLFile(const char *file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::binary);
    if(!file)
    {
        std::cout<<"mxPointList::LoadFromFile() Error: unable to open file: "<<file_name<<std::endl;
        return 0;
    }
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    int line_size = 99999;
    
    if(!file.getline(text_line, line_size)) { file.close(); return 0; }
    if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='P' || text_line[4]!='o' || text_line[5]!='s' || text_line[6]!='i' ||
       text_line[7]!='t' || text_line[8]!='i' || text_line[9]!='o' || text_line[10]!='n' || text_line[11]!='L' || text_line[12]!='i' || text_line[13]!='s' ||
       text_line[14]!='t' || text_line[15]!=' ' || text_line[16]!='F' || text_line[17]!='i' ||  text_line[18]!='l' ||  text_line[19]!='e' ||  text_line[20]!=':')
    { file.close(); return 0; }
    
    if(!file.getline(text_line, line_size)) return 0;
    if(text_line[0]!='v') return 0;
    if(!file.getline(text_line, line_size)) return 0;
    mxString bds;
    bds.Append(text_line);
    mxList<int> list;
    bds.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<"mxPointList::LoadFromFile(): No version value found!"<<std::endl;
        return 0;
    }
    if(list[0]>1)
    {
        std::cout<<"mxPointList::LoadFromFile(): Version higher than supported, version: "<<list[0]<<std::endl;
        return 0;
    }
    
    if(!file.getline(text_line, line_size)) { file.close(); return 0; }
    if(text_line[0]!='t') return 0;
    if(!file.getline(text_line, line_size)) return 0;
    
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
        //mxString *tag = m_scalar_tags.AddNewToEnd();
        //tag->Append(&(text_line[1]));
        //m_scalar_tags_indexes.AddToRightEnd(tag_index);
        if(!file.getline(text_line, line_size)) return 0;
    }
    
    if(text_line[0]!='p') { file.close(); return 0; }
    while(file.getline(text_line, line_size))
    {
        
        mxPointListElement *ple = m_positions.AddNewToEnd();
        if(!ple->m_position.ReadFromString(text_line)) { file.close(); return 0; }
        if(!file.getline(text_line, line_size)) { file.close(); return 0; }
        ple->m_label.Assign(text_line);
    }
    
    file.close();
    return 1;
}


mxPoint& mxPointList::operator[] (unsigned int index)
{
    return this->m_positions[index].m_position;
}


void mxPointList::RemovePosition(mxPosition *p)
{
	mxListNode< mxPointListElement > *node_to_remove = NULL;
    
	mxPointList::Iterator it;
	for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
	{
		if(it.GetElementAddress()->m_position.IsEqualPositionIn_3D(*p))
		{
			node_to_remove = it.GetNode();
			break;
		}
	}
	m_positions.DeleteNode(node_to_remove);
}


void mxPointList::RemoveRepeatingPositionsIn3D()
{
    if(this->IsEmpty()) return;
    
    int changes_made = 1;
    while(changes_made)
    {
        changes_made = 0;
        
        for(unsigned int i=0; i<this->GetNumberOfElements()-1 && (!changes_made); i++)
        {
            for(unsigned int j=i+1; j<this->GetNumberOfElements() && (!changes_made); j++)
            {
                if(m_positions[i].m_position.IsEqualPositionIn_3D(m_positions[j].m_position))
                {
                    mxPoint *to_remove;
                    to_remove = &(this->m_positions[j].m_position);
                    this->RemovePosition(to_remove);
                    changes_made = 1;
                }
            }
        }
    }
}


void mxPointList::Reset()
{
    m_positions.Reset();
    
    for(int i=0; i<(int)this->GetMaximumNumberOfScalars(); i++)
    {
        m_scalars[i].Reset();
    }
}


int mxPointList::SaveToPSLFile(const char *file_name)
{
    mxString bds, bds_dir, bds_file_name, bds_ext;
    bds.Assign(file_name);
    bds.ExtractFilePathParts(bds_dir, bds_file_name, bds_ext);
    if(!(bds_ext=="psl" || bds_ext=="PSL"))
    {
        bds.Append(".psl");
    }

    std::ofstream output_file;
    output_file.open(bds.Get_C_String(), std::ios::binary);

    output_file<<"%bdPositionList File: "<<std::endl;

	// Version
    output_file<<"v"<<std::endl;
    output_file<<"1.0"<<std::endl;

	// Tags for scalars
    output_file<<"t"<<std::endl;
    //std::cout<<std::endl<<"this->GetMaximumNumberOfScalars()="<<this->GetMaximumNumberOfScalars();
    for(int i=0; i<(int)this->GetMaximumNumberOfScalars(); i++)
    {
        //std::cout<<std::endl<<"this->m_scalars["<<i<<"].m_is_used="<<this->m_scalars[i].m_is_used;
        if(this->m_scalars[i].m_is_used)
        {
            output_file<<"%"<<m_scalars[i].m_tag.Get_C_String();
            
            // if visualization range is correct, we add info on the visualization range
            if(this->m_scalars[i].m_scalar_visualization_range[0] <  this->m_scalars[i].m_scalar_visualization_range[1])
            {
                mxString s;
                s.Assign("{");
                s.AppendNumber(this->m_scalars[i].m_scalar_visualization_range[0]);
                s.Append(", ");
                s.AppendNumber(this->m_scalars[i].m_scalar_visualization_range[1]);
                s.Append("}");
                output_file<<s.Get_C_String();
            }
            output_file<<std::endl;
        }
    }
    
	// Points
    output_file<<"p"<<std::endl;
	mxPointList::Iterator it;
	for(it.SetToBegin(m_positions); it.IsValid(); it.MoveToNext())
	{
        // position and values
        output_file<<it.GetElement().m_position<<std::endl;
        
        // label
        output_file<<it.GetElement().m_label<<std::endl;
	}

	output_file.close();
    
    return 1;
}
