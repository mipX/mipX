/*=========================================================================
 
 Program:   mipx
 Module:    mxProfile.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxProfile.h"



//---------------------------------------------------------------------------



mxProfileVector::mxProfileVector()
{
    m_profile = NULL;
    m_values = NULL;
    m_original_voxel_value = 0;
}


mxProfileVector::~mxProfileVector()
{
    this->Reset();
}


float& mxProfileVector::operator ()(int t, int s, int r, int c)
{
    return m_values[t*m_profile->GetVectorDimensions_C()*m_profile->GetVectorDimensions_R()*m_profile->GetVectorDimensions_S() +
                    s*m_profile->GetVectorDimensions_C()*m_profile->GetVectorDimensions_R() +
                    r*m_profile->GetVectorDimensions_C() +
                    c];
}


float& mxProfileVector::operator ()(int s, int r, int c)
{
    return (*this)(0,s,r,c);
}


void mxProfileVector::Reset()
{
    delete m_values;
}


void mxProfileVector::SetDimensions(int t, int s, int r, int c)
{
    m_values = new float[t*s*r*c];
}


void mxProfileVector::SetProfile(mxProfile *profile)
{
    m_profile = profile;
}



//---------------------------------------------------------------------------



mxProfile::mxProfile()
{
	m_number_of_created_vectors = 0;

	mxString bds;
    
    this->SetVectorDimensionLabel_T("T");
    this->SetVectorDimensionLabel_S("S");
    this->SetVectorDimensionLabel_R("R");
    this->SetVectorDimensionLabel_C("C");
}


void mxProfile::CreateVector(int s, int r, int c)
{
    if(this->IsVectorCreated(s,r,c)) return;

    m_grid(s,r,c) = new mxProfileVector;
    m_grid(s,r,c)->SetProfile(this);
    m_grid(s,r,c)->SetDimensions(m_vector_dimensions[3],m_vector_dimensions[2],m_vector_dimensions[1],m_vector_dimensions[0]);
    
    this->m_number_of_created_vectors++;
}


void mxProfile::DeleteVector(int s, int r, int c)
{
    if(this->IsVectorCreated(s,r,c))
    {
        delete m_grid(s,r,c);
        m_number_of_created_vectors--;
    }
}


int mxProfile::GetIndexOfDimensionLabel(const char *label)
{
    for(int i=0; i<mxProfile_NUMBER_OF_DIMENSIONS; i++)
    {
        if(m_vector_dimensions_labels[i] == label)
        {
            return i;
        }
    }
    return -1;
}


int mxProfile::GetIndexOfValuesLabel(int dimensions_index, const char *label)
{
    for(int i=0; i<m_vector_values_labels[dimensions_index].GetNumberOfElements(); i++)
    {
        if(m_vector_values_labels[dimensions_index][i] == label)
        {
            return i;
        }
    }
    return -1;
}


int mxProfile::IsVectorCreated(int s, int r, int c)
{
    if(this->m_grid(s,r,c) == NULL) return 0;
    return 1;
}


void mxProfile::Reset()
{
	for(int s=0; s<this->m_grid.GetNumberOfSlices(); s++)
	{
		for(int r=0; r<this->m_grid.GetNumberOfRows(); r++)
		{
			for(int c=0; c<this->m_grid.GetNumberOfColumns(); c++)
			{
                delete this->m_grid(s,r,c);
			}
		}
	}

	this->m_grid.Reset();

	m_number_of_created_vectors = 0;
    
    this->SetVectorDimensionLabel_T("T");
    this->SetVectorDimensionLabel_S("S");
    this->SetVectorDimensionLabel_R("R");
    this->SetVectorDimensionLabel_C("C");

    for(int i=0; i<mxProfile_NUMBER_OF_DIMENSIONS; i++)
    {
        this->m_vector_values_labels[i].Reset();
    }
}


void mxProfile::SetGridDimensions(int s, int r, int c)
{
    if(!this->IsEmpty())
    {
        for(int s=0; s<this->m_grid.GetNumberOfSlices(); s++)
        {
            for(int r=0; r<this->m_grid.GetNumberOfRows(); r++)
            {
                for(int c=0; c<this->m_grid.GetNumberOfColumns(); c++)
                {
                    delete this->m_grid(s,r,c);
                }
            }
        }
    }
    
    m_grid.SetDimensions(s,r,c);
}


void mxProfile::SetVectorDimensions(int s, int r, int c)
{
    if(s<1 || r<1 || c<1)
    {
        std::cout<<std::endl<<"mxProfile::SetVectorDimension(): incorrect profile vector size: "<<s<<", "<<r<<", "<<c<<".";
        return;
    }

    this->m_vector_dimensions[3] = 1;
    this->m_vector_dimensions[2] = s;
    this->m_vector_dimensions[1] = r;
    this->m_vector_dimensions[0] = c;

    this->m_vector_values_labels[0].SetNumberOfElements( m_vector_dimensions[0] );
    this->m_vector_values_labels[1].SetNumberOfElements( m_vector_dimensions[1] );
    this->m_vector_values_labels[2].SetNumberOfElements( m_vector_dimensions[2] );
    
    //Set default values for vector s,r,c values
    for(int i=0; i<m_vector_dimensions[0]; i++)
    {
        this->m_vector_values_labels[0][i].AssignNumber(i);
    }
    for(int i=0; i<m_vector_dimensions[1]; i++)
    {
        this->m_vector_values_labels[1][i].AssignNumber(i);
    }
    for(int i=0; i<m_vector_dimensions[2]; i++)
    {
        this->m_vector_values_labels[2][i].AssignNumber(i);
    }
}


void mxProfile::SetVectorValuesLabels(int index_of_dimension, mxList< mxString > &labels)
{
    if(labels.IsEmpty())
    {
        std::cout<<std::endl<<"mxProfile::SetVectorValuesLabels() error: labels list is empty.";
        return;
    }
    if(labels.GetNumberOfElements() != this->m_vector_values_labels[index_of_dimension].GetNumberOfElements())
    {
        std::cout<<std::endl<<"mxProfile::SetVectorValuesLabels() error: number of strings in labels list does not match array of vector values labels.";
        return ;
    }
    
    int i=0;
    mxListIterator< mxString > it;
    for(it.SetToBegin(labels), i=0; it.IsValid(); it.MoveToNext(), i++)
    {
        std::cout<<i<<" "<<it.GetElementAddress()->Get_C_String();
        this->SetVectorValuesLabel(index_of_dimension, i, it.GetElementAddress()->Get_C_String());
    }
}


int mxProfile::SaveToFile(const char *file_name)
{
    mxString mxs_file_name;
    mxs_file_name.Assign(file_name);
    mxString mxs_dir, mxs_fnr, mxs_extension;
    mxs_file_name.ExtractFilePathParts(mxs_dir, mxs_fnr, mxs_extension);
    if(mxs_extension != "prf" && mxs_extension != "PRF" )
    {
        mxs_file_name.Append(".prf");
    }
	
	const char *name = mxs_file_name.Get_C_String();

    std::ofstream output_file(name,std::ios::binary);
	if(!output_file)
	{
        std::cout<<"Error: mxProfile::SaveToFile(): Unable to open "<<name<<" !"<<std::endl;
		return 0;
	}

	output_file<<"%Profile File: "<<std::endl;

	//----- Write number of existing vectors -----
	output_file<<"n"<<std::endl;
	output_file<<(this->m_number_of_created_vectors)<<std::endl;

	//----- Write origin -----
	output_file<<"o"<<std::endl;
	output_file<<(m_grid.GetOrigin_S())<<std::endl;
	output_file<<(m_grid.GetOrigin_R())<<std::endl;
	output_file<<(m_grid.GetOrigin_C())<<std::endl;

	//----- Write spacing -----
	output_file<<"s"<<std::endl;
	output_file<<(m_grid.GetSpacing_S())<<std::endl;
	output_file<<(m_grid.GetSpacing_R())<<std::endl;
	output_file<<(m_grid.GetSpacing_C())<<std::endl;

	//----- Write grid dimensions -----
	output_file<<"m"<<std::endl;
	output_file<<(m_grid.GetNumberOfSlices())<<std::endl;
	output_file<<(m_grid.GetNumberOfRows())<<std::endl;
	output_file<<(m_grid.GetNumberOfColumns())<<std::endl;

	//----- Write profile vector dimensions -----
	output_file<<"p"<<std::endl;
	output_file<<(m_vector_dimensions[2])<<std::endl;
	output_file<<(m_vector_dimensions[1])<<std::endl;
	output_file<<(m_vector_dimensions[0])<<std::endl;

	//----- Write vector description strings -----
	output_file<<"r"<<std::endl;
	output_file<<"%"<<(m_vector_dimensions_labels[2])<<std::endl;
	output_file<<"%"<<(m_vector_dimensions_labels[1])<<std::endl;
	output_file<<"%"<<(m_vector_dimensions_labels[0])<<std::endl;
	
	//----- Write vector values description strings -----
	output_file<<"d"<<std::endl;
	for(int i=0; i<m_vector_values_labels[2].GetNumberOfElements(); i++) output_file<<"%"<<(m_vector_values_labels[2][i])<<std::endl;
    for(int i=0; i<m_vector_values_labels[1].GetNumberOfElements(); i++) output_file<<"%"<<(m_vector_values_labels[1][i])<<std::endl;
    for(int i=0; i<m_vector_values_labels[0].GetNumberOfElements(); i++) output_file<<"%"<<(m_vector_values_labels[0][i])<<std::endl;
    
	//----- Write all the values -----
	output_file<<"v"<<std::endl;

	//Save all the points consisting of 3 index values and vector_size_s*vector_size_r*vector_size_c float values
	for(int s=0; s<m_grid.GetNumberOfSlices(); s++)
	{
		for(int r=0; r<m_grid.GetNumberOfRows(); r++)
		{
			for(int c=0; c<m_grid.GetNumberOfColumns(); c++)
			{
				//If the vector at the given position exists
				if(m_grid(s,r,c) != NULL)
				{
					//Write the profile vector values
					output_file.write((char*) &(s), sizeof(int));
					output_file.write((char*) &(r), sizeof(int));
					output_file.write((char*) &(c), sizeof(int));
					output_file.write((char*) (char*) &(m_grid(s,r,c)->m_original_voxel_value), sizeof(unsigned short));
					for(int i=0; i<m_vector_dimensions[2]; i++)
					{
						for(int j=0; j<m_vector_dimensions[1]; j++)
						{
							for(int k=0; k<m_vector_dimensions[0]; k++)
							{
								output_file.write((char*) &( (*(m_grid(s,r,c)))(i,j,k)), sizeof(float));
							}
						}
					}
				}
			}
		}
	}

	output_file.close();
	return 1;
}


int mxProfile::LoadFromFile(const char *file_name)
{
	std::ifstream input_file;
	input_file.open(file_name,std::ios::binary);
    if(!input_file)
	{
		std::cout<<"mxProfile::LoadFromFile() : Unable to open file: "<<file_name<<std::endl;
		return 0;
    }

	char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
	int amount_of_characters_to_load = 99999;

	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    
	if(text_line[0]!='%' || text_line[1]!='P' || text_line[2]!='r' || text_line[3]!='o' || text_line[4]!='f' || text_line[5]!='i' || text_line[6]!='l' ||
		text_line[7]!='e' || text_line[8]!=' ' || text_line[9]!='F' || text_line[10]!='i' || text_line[11]!='l' || text_line[12]!='e' ||
		text_line[13]!=':') { input_file.close(); return 0; }
    

	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	if(text_line[0]!='n') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	int n = atoi(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	if(text_line[0]!='o') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	double o_s = atof(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	double o_r = atof(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	double o_c = atof(text_line);
	m_grid.SetOrigin(0,o_s,o_r,o_c);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	if(text_line[0]!='s') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	double s_s = atof(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	double s_r = atof(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	double s_c = atof(text_line);
	m_grid.SetSpacing(1,s_s,s_r,s_c);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	if(text_line[0]!='m') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	int s = atoi(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	int r = atoi(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	int c = atoi(text_line);
    this->SetGridDimensions(s,r,c); //m_grid.SetDimensions(s,r,c);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	if(text_line[0]!='p') { input_file.close(); return 0; }
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	s = atoi(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	r = atoi(text_line);
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	c = atoi(text_line);
    this->SetVectorDimensions(s,r,c);
	
	// Load vector labels
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	if(text_line[0]!='r') { input_file.close(); return 0; }
	for(int i=0; i<3; i++)
	{
		if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
		if(text_line[0]=='d') { input_file.close(); return 0; }
		else
		{
			if(text_line[0]!='%'){ input_file.close(); return 0; }
            if(i==0) this->SetVectorDimensionLabel_S(&(text_line[1]));
            if(i==1) this->SetVectorDimensionLabel_R(&(text_line[1]));
            if(i==2) this->SetVectorDimensionLabel_C(&(text_line[1]));
		}
	}

	//Loading vector value descriptions
	if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
	if(text_line[0]!='d') { input_file.close(); return 0; }
	int is_loading_strings = 1;
	int values_to_load = 0;//0,1,2 reresent s,r,c values respectively.
	int i=0;
	mxString bds;
	while(is_loading_strings)
	{
		if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
		if(text_line[0]=='v') is_loading_strings = 0;
		else
		{
			if(text_line[0]!='%'){ input_file.close(); return 0; }
			if(values_to_load==2) 
			{
                this->SetVectorValuesLabel_C(i, &(text_line[1])); //this->SetVectors_C_ValuesDescription(i, bds(&(text_line[1])));
				if(i==this->m_vector_values_labels[0].GetNumberOfElements()-1)//if(i==this->vector_c_values_desc.NumberOfElements()-1)
				{
					is_loading_strings = 0;
					if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
					if(text_line[0]!='v') { input_file.close(); return 0; }
				}
			}
			if(values_to_load==1) 
			{
				this->SetVectorValuesLabel_R(i, &(text_line[1])); //this->SetVectors_R_ValuesDescription(i, bds(&(text_line[1])));
				if(i==this->m_vector_values_labels[1].GetNumberOfElements()-1)//if(i==this->vector_r_values_desc.NumberOfElements()-1)
				{
					values_to_load = 2;
					i = -1;
				}
			}
			if(values_to_load==0) 
			{
				this->SetVectorValuesLabel_S(i, &(text_line[1])); //this->SetVectors_S_ValuesDescription(i, bds(&(text_line[1])));
				if(i==this->m_vector_values_labels[2].GetNumberOfElements()-1)//if(i==this->vector_s_values_desc.NumberOfElements()-1)
				{
					values_to_load = 1;
					i = -1;
				}
			}
			i++;
		}
	}

	//Load position and float values
	for(int j=0; j<n; j++)
	{
		input_file.read((char*) &s, sizeof(int));
		input_file.read((char*) &r, sizeof(int));
		input_file.read((char*) &c, sizeof(int));
        this->CreateVector(s,r,c); //if(!this->CreateVectorAtIndexes(s,r,c)) { input_file.close(); return 0; }
		unsigned short u;
		input_file.read((char*) &u, sizeof(unsigned short));
		this->m_grid(s,r,c)->m_original_voxel_value = u;

		for(int q=0; q<this->m_vector_dimensions[2]; q++)//for(int q=0; q<this->m_grid[s][r][c]->m_vector.NumberOfSlices(); q++)
		{
			for(int w=0; w<this->m_vector_dimensions[1]; w++)
			{
				for(int e=0; e<this->m_vector_dimensions[0]; e++)
				{
					float value;
					input_file.read((char*) &value, sizeof(float));
                    this->GetVector(s,r,c)->operator()(q,w,e)  = value; //m_grid(s,r,c)->m_ m_vector[q][w][e] = value;
				}
			}
		}
	}

	input_file.close();

	return 1;
}
