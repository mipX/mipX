/*=========================================================================
 
 Program:   mipx
 Module:    mxTable.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxTable.h"


mxTableSheet::mxTableSheet()
{
}


mxTableSheet::~mxTableSheet()
{
	this->Reset();
}


void mxTableSheet::Copy(mxTableSheet *sheet)
{
    m_data.Copy(&(sheet->m_data));
    m_labels_of_columns = sheet->m_labels_of_columns;
    m_labels_of_rows = sheet->m_labels_of_rows;
}


void mxTableSheet::FillInWith(double value)
{
    mxString s;
    s.AssignNumber(value);
    m_data.FillInWith(s);
}


mxString& mxTableSheet::GetSheetAsString()
{
    m_sheet_as_string.Clear();
    
    m_sheet_as_string.Append(" { }");
    
    for(unsigned int c=0; c<this->GetNumberOfColumns(); c++)
    {
        m_sheet_as_string.Append("{");
        m_sheet_as_string.Append( this->m_labels_of_columns[c]);
        m_sheet_as_string.Append("}");
    }
    
    
    for(unsigned int r=0; r<this->GetNumberOfRows(); r++)
    {
        m_sheet_as_string.Append(" {");
        m_sheet_as_string.Append( this->m_labels_of_rows[r]);
        m_sheet_as_string.Append("}");
        
        for(unsigned int c=0; c<this->GetNumberOfColumns(); c++)
        {
            m_sheet_as_string.Append("{");
            m_sheet_as_string.Append(m_data(r,c));
            m_sheet_as_string.Append("}");
        }
    }
    
    return m_sheet_as_string;
}


int mxTableSheet::GetValue(int r, int c, double &output)
{
    return m_data(r,c).ToNumber(output);
}


int mxTableSheet::internalLoadFromFile(const char *file_name)
{
    std::ifstream input_file;
    input_file.open(file_name,std::ios::binary);
    if(!input_file)
    {
        std::cout<<"mxTableSheet::LoadFromFile() : Unable to open file: "<<file_name<<std::endl;
        return 0;
    }
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    int amount_of_characters_to_load = 99999;
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='T' || text_line[4]!='a' || text_line[5]!='b' || text_line[6]!='l' ||
       text_line[7]!='e' || text_line[8]!=' ' || text_line[9]!='F' || text_line[10]!='i' || text_line[11]!='l' || text_line[12]!='e' ||
       text_line[13]!=':') { input_file.close(); return 0; }
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) return 0;
    if(text_line[0]!='v') return 0;
    if(!input_file.getline(text_line, amount_of_characters_to_load)) return 0;
    mxString bds;
    bds.Append(text_line);
    mxList<int> list;
    bds.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<"mxTableSheet::LoadFromFile(): No version value found!"<<std::endl;
        return 0;
    }
    if(list.GetBeginElement()>1)
    {
        std::cout<<"mxTableSheet::LoadFromFile(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
        return 0;
    }
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='d') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    int r = atoi(text_line);
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    int c = atoi(text_line);
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    this->SetDimensions(r,c);
    
    //Loading row strings
    if(text_line[0]!='r') { input_file.close(); return 0; }
    int is_loading_row_strings = 1;
    int i=0;
    while(is_loading_row_strings)
    {
        if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
        if(text_line[0]=='c') is_loading_row_strings = 0;
        else
        {
            if(text_line[0]!='%'){ input_file.close(); return 0; }
            this->SetLabelOfRow(i, (&(text_line[1])));
            i++;
        }
    }
    
    //Loading column strings
    int is_loading_column_strings = 1;
    i=0;
    while(is_loading_column_strings)
    {
        if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
        if(text_line[0]=='v') is_loading_column_strings = 0;
        else
        {
            if(text_line[0]!='%'){ input_file.close(); return 0; }
            this->SetLabelOfColumn(i, (&(text_line[1])));
            i++;
        }
    }
    
    //Load elements as strings
    unsigned int rn = 0, cn = 0;
    for(int j=0; j<(int)this->GetNumberOfElements() && input_file.getline(text_line, amount_of_characters_to_load); j++)
    {
        if(rn>=this->GetNumberOfRows()) { input_file.close(); return 0; }
        m_data(rn,cn).Append(text_line);
        cn = (cn+1) % (this->GetNumberOfColumns());
        if(cn==0) rn++;
    }
    
    input_file.close();
    
    return 1;
}


int mxTableSheet::internalLoadFromTableDoubleTextFile(const char *file_name)
{
    std::ifstream input_file;
    input_file.open(file_name,std::ios::binary);
    if(!input_file)
    {
        std::cout<<" mxTableSheet::LoadFromTableDoubleTextFile() : Unable to open file: "<<file_name<<std::endl;
        return 0;
    }
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    int amount_of_characters_to_load = 99999;
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='%' || text_line[1]!='T' || text_line[2]!='a' || text_line[3]!='b' || text_line[4]!='l' || text_line[5]!='e' || text_line[6]!='D' ||
       text_line[7]!='o' || text_line[8]!='u' || text_line[9]!='b' || text_line[10]!='l' || text_line[11]!='e' || text_line[12]!=' ' ||
       text_line[13]!='F' || text_line[14]!='i' || text_line[15]!='l' || text_line[16]!='e' || text_line[17]!=':') { input_file.close(); return 0; }
    
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='d') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    int r = atoi(text_line);
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    int c = atoi(text_line);
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    this->SetDimensions(r,c);
    
    
    //Loading row strings
    if(text_line[0]!='r') { input_file.close(); return 0; }
    int is_loading_row_strings = 1;
    int i=0;
    while(is_loading_row_strings)
    {
        if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
        if(text_line[0]=='c') is_loading_row_strings = 0;
        else
        {
            if(text_line[0]!='%'){ input_file.close(); return 0; }
            this->SetLabelOfRow(i, (&(text_line[1])));
            i++;
        }
    }
    
    
    //Loading column strings
    int is_loading_column_strings = 1;
    i=0;
    while(is_loading_column_strings)
    {
        if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
        if(text_line[0]=='v') is_loading_column_strings = 0;
        else
        {
            if(text_line[0]!='%'){ input_file.close(); return 0; }
            this->SetLabelOfColumn(i, (&(text_line[1])));
            i++;
        }
    }
    
    
    //Load arrays of double values
    for(unsigned int j=0; j<this->m_data.GetNumberOfRows() && input_file.getline(text_line, amount_of_characters_to_load); j++)
    {
        mxString bds, bds2;
        bds.Append(text_line);
        bds.ExtractString('[',']',bds2);
        mxList<double> list_of_doubles;
        bds.ExtractNumbers(list_of_doubles);
        
        unsigned int c=0;
        mxListIterator<double> it;
        for(it.SetToBegin(list_of_doubles); it.IsValid(); it.MoveToNext())
        {
            m_data(j,c).AssignNumber(it.GetElement());
            c++;
        }
    }
    
    input_file.close();
    
    return 1;
}


mxString& mxTableSheet::operator()(int r, int c)
{
    return m_data(r,c);
}


void mxTableSheet::Reset()
{
    m_data.Reset();
    m_labels_of_columns.Reset();
    m_labels_of_rows.Reset();
}


int mxTableSheet::SetDimensions(int r, int c)
{
    if(r<1 || c<1) return 0;
    
    m_data.SetDimensions(r,c);
    m_labels_of_columns.SetNumberOfElements(c);
    m_labels_of_rows.SetNumberOfElements(r);
    
    // We set defult row and column labels as ther index values.
    for(int i=0; i<(int)m_labels_of_columns.GetNumberOfElements(); i++)
    {
        m_labels_of_columns[i].AssignNumber(i);
    }
    for(int i=0; i<(int)m_labels_of_rows.GetNumberOfElements(); i++)
    {
        m_labels_of_rows[i].AssignNumber(i);
    }
    return 1;
}


int mxTableSheet::SetLabelOfColumn(unsigned int c, mxString &label)
{
    if(c>=m_labels_of_columns.GetNumberOfElements()) return 0;
    m_labels_of_columns[c].Assign(label);
    return 1;
}


int mxTableSheet::SetLabelOfColumn(unsigned int c, const char *label)
{
    if(label==NULL) return 0;
    if(c>=m_labels_of_columns.GetNumberOfElements()) return 0;
    m_labels_of_columns[c].Assign(label);
    return 1;
}


int mxTableSheet::SetLabelOfRow(unsigned int r, mxString &label)
{
	if(r>=m_labels_of_rows.GetNumberOfElements()) return 0;
    m_labels_of_rows[r].Assign(label);
	return 1;
}


int mxTableSheet::SetLabelOfRow(unsigned int r, const char *label)
{
	if(label==NULL) return 0;
    if(r>=m_labels_of_rows.GetNumberOfElements()) return 0;
    m_labels_of_rows[r].Assign(label);
    return 1;
}


void mxTableSheet::SetString(int r, int c, const char *string_value)
{
    m_data(r,c).Assign(string_value);
}


void mxTableSheet::SetValue(int r, int c, double value)
{
	m_data(r,c).AssignNumber(value);
}


int mxTableSheet::SetTitle(const char *title)
{
    if(title==NULL) return 0;
    m_title.Assign(title);
    return 1;
}



//-----------------------------------------------------------------------------------------------------------------------------------------



mxTable::mxTable()
{
    m_class_name.Assign("mxTable");
    this->SetExclusiveVisibility(1);
    
    // An empty table must have one created sheet.
    this->AddSheet();
}


mxTable::~mxTable()
{
    m_sheets.Reset();
}


mxTableSheet* mxTable::AddSheetToBegin(const char *sheet_name)
{
    mxTableSheet *sheet = m_sheets.AddNewToBegin();
    if(sheet_name) sheet->m_title.Assign(sheet_name);
    return sheet;
}


mxTableSheet* mxTable::AddSheetToEnd(const char *sheet_name)
{
    mxTableSheet *sheet = m_sheets.AddNewToEnd();
    if(sheet_name) sheet->m_title.Assign(sheet_name);
    else
    {
        sheet->m_title.AssignNumber((int)(m_sheets.GetNumberOfElements()-1));
    }
    return sheet;
}


mxTableSheet* mxTable::AddSheet(const char *sheet_name)
{
    return this->AddSheetToEnd(sheet_name);
}


int mxTable::Copy(mxTable *table)
{
    if(!table) return 0;
    if(this==table) return 1;
    
    this->Reset();
    this->mxDataObject::CopyFromDataObject(table);
    m_sheets = table->m_sheets;
    return 1;
}


int mxTable::Copy(mxTable &table)
{
    return this->mxTable::Copy(&table);
}


int mxTable::ExportToCSVFile(const char *file_name)
{
    mxString bds, bds_dir, bds_file_name, bds_ext;
    bds.Assign(file_name);
    bds.ExtractFilePathParts(bds_dir, bds_file_name, bds_ext);
    if(!(bds_ext=="csv" || bds_ext=="CSV"))
    {
        bds.Append(".csv");
    }
    
    std::ofstream output_file;
    output_file.open(bds.Get_C_String(),std::ios::binary);
    
    for(unsigned int sheet_index=0; sheet_index<this->GetNumberOfSheets(); sheet_index++)
    {
        //----- Write column strings -----
        output_file<<"sheet["<<sheet_index<<"],";
        for(unsigned int i=0; i<m_sheets[sheet_index].m_labels_of_columns.GetNumberOfElements(); i++)
        {
            if(i==m_sheets[sheet_index].m_labels_of_columns.GetNumberOfElements()-1)
            {
                output_file<<"\""<<(m_sheets[sheet_index].m_labels_of_columns[i])<<"\""<<std::endl;
            }
            else
            {
                output_file<<"\""<<(m_sheets[sheet_index].m_labels_of_columns[i])<<"\",";
            }
        }
        
        //----- Write all the values -----
        for(unsigned int r=0; r<m_sheets[sheet_index].m_data.GetNumberOfRows(); r++)
        {
            output_file<<"\""<<(m_sheets[sheet_index].m_labels_of_rows[r])<<"\",";
            
            for(unsigned int c=0; c<m_sheets[sheet_index].m_data.GetNumberOfColumns(); c++)
            {
                if(c==m_sheets[sheet_index].m_labels_of_columns.GetNumberOfElements()-1)
                {
                    output_file<<"\""<<((m_sheets[sheet_index].m_data)(r,c))<<"\""<<std::endl;
                }
                else
                {
                    output_file<<"\""<<((m_sheets[sheet_index].m_data)(r,c))<<"\",";
                }
            }
        }
    }
    output_file.close();
    return 1;
}


int mxTable::FillInWith(double value, unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    m_sheets[sheet_index].FillInWith(value);
    return 1;
}


mxArray<mxString>* mxTable::GetLabelsOfColumns(unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return NULL;
    return m_sheets[sheet_index].GetLabelsOfColumns();
}


mxArray<mxString>* mxTable::GetLabelsOfRows(unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return NULL;
    return m_sheets[sheet_index].GetLabelsOfRows();
}


unsigned int mxTable::GetNumberOfColumns(unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return (m_sheets[sheet_index].m_data.GetNumberOfColumns());
}


int mxTable::GetNumberOfElements(unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return m_sheets[sheet_index].GetNumberOfElements();
}


unsigned int mxTable::GetNumberOfRows(unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return (m_sheets[sheet_index].m_data.GetNumberOfRows());
}


mxTableSheet* mxTable::GetSheet(unsigned int index)
{
    if(index > this->GetNumberOfSheets()-1) return 0;
    return &(m_sheets[index]);
}


int mxTable::GetValue(int r, int c, double &output, unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return m_sheets[sheet_index].GetValue(r, c, output);
}


int mxTable::internalLoadFromFile_v1(const char *file_name)
{
    this->Reset();
    return this->GetSheet(0)->internalLoadFromFile(file_name);
}


int mxTable::internalLoadFromFile_v2(const char *file_name)
{
    this->Reset();
    
    std::ifstream input_file;
    input_file.open(file_name,std::ios::binary);
    if(!input_file)
    {
        std::cout<<"mxTable::LoadFromFile() : Unable to open file: "<<file_name<<std::endl;
        return 0;
    }
    
    char text_line[100000];//If you change this value, you must also change the amount of loaded characters, see below.
    int amount_of_characters_to_load = 99999;
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='%' || text_line[1]!='b' || text_line[2]!='d' || text_line[3]!='T' || text_line[4]!='a' || text_line[5]!='b' || text_line[6]!='l' ||
       text_line[7]!='e' || text_line[8]!=' ' || text_line[9]!='F' || text_line[10]!='i' || text_line[11]!='l' || text_line[12]!='e' ||
       text_line[13]!=':') { input_file.close(); return 0; }
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='v') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    mxString bds;
    bds.Append(text_line);
    mxList<int> list;
    bds.ExtractNumbers(list);
    if(list.IsEmpty())
    {
        std::cout<<"mxTable::LoadFromFile(): No version value found!"<<std::endl;
        input_file.close();
        return 0;
    }
    if(list.GetBeginElement()>2)
    {
        std::cout<<"mxTable::LoadFromFile(): Version higher than supported, version: "<<list.GetBeginElement()<<std::endl;
        input_file.close();
        return 0;
    }
    
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    if(text_line[0]!='s') { input_file.close(); return 0; }
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    int number_of_sheets = atoi(text_line);
    if(number_of_sheets<=0) { input_file.close(); return 0; }
    this->SetNumberOfSheets(number_of_sheets);
    
    
    // repeatedly load table sheets.
    if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
    int sheet_index = 0;
    while(text_line[0]=='d' && sheet_index<number_of_sheets)
    {
        if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
        int r = atoi(text_line);
        if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
        int c = atoi(text_line);
        if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
        this->SetDimensions(r,c,sheet_index);
        
        //Loading row strings
        if(text_line[0]!='r') { input_file.close(); return 0; }
        int is_loading_row_strings = 1;
        int i=0;
        while(is_loading_row_strings)
        {
            if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
            if(text_line[0]=='c') is_loading_row_strings = 0;
            else
            {
                if(text_line[0]!='%'){ input_file.close(); return 0; }
                this->SetLabelOfRow(i, (&(text_line[1])), sheet_index);
                i++;
            }
        }
        
        //Loading column strings
        int is_loading_column_strings = 1;
        i=0;
        while(is_loading_column_strings)
        {
            if(!input_file.getline(text_line, amount_of_characters_to_load)) { input_file.close(); return 0; }
            if(text_line[0]=='v') is_loading_column_strings = 0;
            else
            {
                if(text_line[0]!='%'){ input_file.close(); return 0; }
                this->SetLabelOfColumn(i, (&(text_line[1])), sheet_index);
                i++;
            }
        }
        
        //Load elements as strings
        unsigned int rn = 0, cn = 0;
        for(int j=0; j<this->GetNumberOfElements() && input_file.getline(text_line, amount_of_characters_to_load); j++)
        {
            if(text_line[0]!='%') { input_file.close(); return 0; }
            if(rn>=this->GetNumberOfRows(sheet_index)) { input_file.close(); return 0; }
            m_sheets[sheet_index].m_data(rn,cn).Append(&(text_line[1]));
            cn = (cn+1) % (this->GetNumberOfColumns(sheet_index));
            if(cn==0) rn++;
        }
        
        sheet_index++;
        if(!input_file.getline(text_line, amount_of_characters_to_load))
        {
            if(sheet_index >= number_of_sheets) { input_file.close(); return 1; }
        }
    }
    
    // Should not get here.
    input_file.close();
    return 0;
}


int mxTable::internalLoadFromTableDoubleTextFile(const char *file_name)
{
    this->Reset();
    return this->GetSheet(0)->internalLoadFromTableDoubleTextFile(file_name);
}


int mxTable::IsEmpty()
{
    if(m_sheets.GetNumberOfElements()==1)
    {
        return m_sheets[0].IsEmpty();
    }
    return 0;
}


int mxTable::LoadFromFile(const char *file_name, const char *extension)
{
    if(this->internalLoadFromFile_v2(file_name)) return 1;
    if(this->internalLoadFromFile_v1(file_name)) return 1;
    if(this->internalLoadFromTableDoubleTextFile(file_name)) return 1;
    return 0;
}


mxString& mxTable::operator()(int r, int c, unsigned int sheet_index)
{
    return m_sheets[sheet_index](r,c);
}


void mxTable::Reset()
{
    m_sheets.Reset();
    this->AddSheet();
}


int mxTable::SaveToFile(const char *file_name, const char *extension)
{
    if(file_name==0) return 0;
    
    mxString bds, bds_dir, bds_file_name, bds_ext;
    bds.Assign(file_name);
    bds.ExtractFilePathParts(bds_dir, bds_file_name, bds_ext);
    if(!(bds_ext=="tbl" || bds_ext=="TBL"))
    {
        bds.Append(".tbl");
    }
    
    std::ofstream output_file;
    output_file.open(bds.Get_C_String(),std::ios::binary);
    
    output_file<<"%bdTable File: "<<std::endl;
    
    //----- Write version -----
    output_file<<"v"<<std::endl;
    output_file<<"2.0"<<std::endl;
    
    //----- Write number of sheets -----
    output_file<<"s"<<std::endl;//Letter 's' indicates that from this moment on, the number of sheets are recorded
    output_file<<(this->GetNumberOfSheets())<<std::endl;
    
    for(unsigned int sheet_index=0; sheet_index<this->GetNumberOfSheets(); sheet_index++)
    {
        //----- Write dimensions -----
        output_file<<"d"<<sheet_index<<std::endl;//Letter 'd' indicates that from this moment on, the dimensions are recorded
        output_file<<(this->GetNumberOfRows())<<std::endl;
        output_file<<(this->GetNumberOfColumns())<<std::endl;
        
        //----- Write row strings -----
        output_file<<"r"<<sheet_index<<std::endl;//Letter 'r' indicates that from this moment on, the row strings are recorded (control character '%' is added to beginning)
        for(unsigned int i=0; i<m_sheets[sheet_index].m_labels_of_rows.GetNumberOfElements(); i++) output_file<<"%"<<(m_sheets[sheet_index].m_labels_of_rows[i])<<std::endl;
        
        //----- Write column strings -----
        output_file<<"c"<<sheet_index<<std::endl;//Letter 'c' indicates that from this moment on, the column strings are recorded (control character '%' is added to beginning)
        for(unsigned int i=0; i<m_sheets[sheet_index].m_labels_of_columns.GetNumberOfElements(); i++) output_file<<"%"<<(m_sheets[sheet_index].m_labels_of_columns[i])<<std::endl;
        
        //----- Write all the values -----
        output_file<<"v"<<sheet_index<<std::endl;//Letter 'v' indicates that from this moment on, the values are recorded    as row arrays.
        for(unsigned int r=0; r<m_sheets[sheet_index].m_data.GetNumberOfRows(); r++)
        {
            for(unsigned int c=0; c<m_sheets[sheet_index].m_data.GetNumberOfColumns(); c++)
            {
                output_file<<"%"<<((m_sheets[sheet_index].m_data)(r,c))<<std::endl;
            }
        }
    }
    output_file.close();
    
    return 1;
}


int mxTable::SetDimensions(int r, int c, unsigned int index_of_sheet)
{
    if(index_of_sheet > this->GetNumberOfSheets()-1) return 0;
    if(r<1 || c<1) return 0;
    
    return m_sheets[index_of_sheet].SetDimensions(r,c);
}


int mxTable::SetLabelOfColumn(unsigned int c, mxString &label, unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return m_sheets[sheet_index].SetLabelOfColumn(c, label);
}


int mxTable::SetLabelOfColumn(unsigned int c, const char *label, unsigned int sheet_index)
{
    if(label==NULL) return 0;
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return m_sheets[sheet_index].SetLabelOfColumn(c, label);
}


int mxTable::SetLabelOfRow(unsigned int r, mxString &label, unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return m_sheets[sheet_index].SetLabelOfRow(r, label);
}


int mxTable::SetLabelOfRow(unsigned int r, const char *label, unsigned int sheet_index)
{
    if(label==NULL) return 0;
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    return m_sheets[sheet_index].SetLabelOfRow(r, label);
}


int mxTable::SetNumberOfSheets(unsigned int number_of_sheets)
{
    if(number_of_sheets==0) return 0;
    m_sheets.Reset();
    for(int i=0; i<(int)number_of_sheets; i++ ) { this->AddSheet(); }
    return 1;
}


int mxTable::SetValue(int r, int c, double value, unsigned int sheet_index)
{
    if(sheet_index > this->GetNumberOfSheets()-1) return 0;
    m_sheets[sheet_index].SetValue(r,c, value);
    return 1;
}

