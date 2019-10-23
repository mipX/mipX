/*=========================================================================
 
 Program:   mipx
 Module:    mxTable.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxTable_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxTable_API
#else
    #if defined(_MSC_VER)
        #ifdef mxTable_EXPORTS
            #define mxTable_API __declspec(dllexport)
        #else
            #define mxTable_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxTable_EXPORTS
            #define mxTable_API __attribute__((visibility("default")))
        #else
            #define mxTable_API
        #endif
    #endif
#endif



#ifndef mxTable_H
    #define mxTable_H


#include "mxArray.h"
#include "mxDataObject.h"
#include "mxList.h"
#include "mxMatrix.h"
#include "mxString.h"

//#include <strstream>
#include <fstream>
//#include <iomanip>



/// Sheet of a table.

class mxTable_API mxTableSheet
{
private:
    
    /// Use this data to store the sheet as string output.
    mxString m_sheet_as_string;
    
public:
    
    /// Title of this sheet.
    mxString m_title;
    
    /// Data stored in the table. Data is stored as strings and converted from strings to other data types on request.
	mxMatrix< mxString > m_data;
	
    /// Labels for each of the rows.
	mxArray< mxString > m_labels_of_rows;
    
    /// Labels for each of the columns.
	mxArray< mxString > m_labels_of_columns;

    
	/// Constructor
    mxTableSheet();
    
    /// Destructor.
	~mxTableSheet();

    /// Copy from input object.
    void Copy(mxTableSheet *sheet);
    
    /// Assign to each field in the table the given value.
    void FillInWith(double value);
    
    /// Get labels for rows.
    mxArray< mxString >* GetLabelsOfRows() { return &m_labels_of_rows; };
    
    /// Get labels for columns.
    mxArray< mxString >* GetLabelsOfColumns() { return &m_labels_of_columns; };
    
    /// Get number of columns in the sheet.
    unsigned int GetNumberOfColumns() { return m_data.GetNumberOfColumns(); };
    
    /// Get number of rows in the sheet.
    unsigned int GetNumberOfRows() { return m_data.GetNumberOfRows(); };
    
    /// Get the total number of elements in the sheet (equals number_of_rows * number_of_columns).
    unsigned int GetNumberOfElements() { return (m_data.GetNumberOfRows()*m_data.GetNumberOfColumns()); };
    
    /// Get the sheet as string where each value is encapsulated in "{}".
    mxString& GetSheetAsString();
    
    /// Get title.
    mxString& GetTitle() { return m_title; };
    
    /// Get the element with indexes (r,c) as a double.
    int GetValue(int r, int c, double &output);
    
    /// LEGACY Load methods.
    int internalLoadFromFile(const char *file_name);
    int internalLoadFromTableDoubleTextFile(const char *file_name);
    
    /// Check if the table sheet is empty.
    int IsEmpty() { return m_data.IsEmpty(); };
    
    /// Indexing operator.
    mxString& operator()(int r, int c);
    
	/// Reset the sheet (just as after construction).
	void Reset();
    
    /// Set dimensions of the table.
    int SetDimensions(int r, int c);

    /// Set label for the column with the given index.
    int SetLabelOfColumn(unsigned int c, mxString &label);
    int SetLabelOfColumn(unsigned int c, const char *label);
    
	/// Set label for the row with the given index.
	int SetLabelOfRow(unsigned int r, mxString &label);
	int SetLabelOfRow(unsigned int r, const char *label);

    /// Set the element with indexes (r,c) from  a string.
    void SetString(int r, int c, const char *string_value);
    
	/// Set the element with indexes (r,c) from  a double.
	void SetValue(int r, int c, double value);
    
    /// Set the title of the sheet.
    int SetTitle(const char *title);
};



/// Table as a list of multiple sheets.

class mxTable_API mxTable : public mxDataObject
{
    
protected:
    
    /// List of table sheets.
    mxList< mxTableSheet > m_sheets;
    
public:
    
    /// Constructor.
    mxTable();
    
    /// Destructor.
    virtual ~mxTable();

    /// Add a new sheet to the beginning of the list of table sheets.
    mxTableSheet* AddSheetToBegin(const char *sheet_name=0);
    
    /// Add a new sheet to the end of the list of table sheets.
    mxTableSheet* AddSheetToEnd(const char *sheet_name=0);
    
    /// Legacy. Calls AddSheetToEnd().
    mxTableSheet* AddSheet(const char *sheet_name=0);
    
    /// Copy from input table.
    int Copy(mxTable *table);
    int Copy(mxTable &table);
    
    /// Export to CSV file.
    int ExportToCSVFile(const char *file_name);
    
    /// Fill the sheet with the given index (default 0) with the given value.
    int FillInWith(double value, unsigned int sheet_index=0);
    
    /// Get the labels of the columns of the sheet with the given index (default 0).
    mxArray< mxString >* GetLabelsOfColumns(unsigned int sheet_index=0);
    
    /// Get the labels of rows of the sheet with the given index (default 0).
    mxArray< mxString >* GetLabelsOfRows(unsigned int sheet_index=0);
    
    /// Get the number of columns of the sheet with the given index (default 0).
    unsigned int GetNumberOfColumns(unsigned int sheet_index=0);
    
    /// Get the total number of elements of the sheet with the given index (default 0).
    int GetNumberOfElements(unsigned int sheet_index=0);
    
    /// Get the number of rows of the sheet with the given index (default 0).
    unsigned int GetNumberOfRows(unsigned int sheet_index=0);
    
    /// Get the number of sheets.
    unsigned int GetNumberOfSheets() { return m_sheets.GetNumberOfElements(); };
    
    /// Get sheet with given index.
    mxTableSheet* GetSheet(unsigned int index);
    
    /// Get value wiht indexes (r,c) as a double of the sheet with the given index (default 0).
    int GetValue(int r, int c, double &output, unsigned int sheet_index=0);
    
    /// LEGACY Load file methods.
    int internalLoadFromFile_v1(const char *file_name);
    int internalLoadFromFile_v2(const char *file_name);
    int internalLoadFromTableDoubleTextFile(const char *file_name);
    
    /// Check if the data matrix is empty.
    int IsEmpty();
    
    /// Load from the given file.
    /// Note: the 'extension' does not need to be specified if clear from the file name.
    virtual int LoadFromFile(const char *file_name, const char *extension=0);
    
    /// Indexing operator.
    mxString& operator()(int r, int c, unsigned int sheet_index=0);
    
    /// Reset the object to its initial state (after construction).
    void Reset();
    
    /// Save to file.
    /// Note: the 'extension' does not need to be specified if clear from the file name.
    virtual int SaveToFile(const char *file_name, const char *extension=0);
    
    /// Set dimensions of the table sheet. Default is the sheet with index 0.
    int SetDimensions(int r, int c, unsigned int index_of_sheet=0);
    
    /// Set label for the column with the given index, of the sheet with the given index (default 0).
    int SetLabelOfColumn(unsigned int c, mxString &label, unsigned int sheet_index=0);
    int SetLabelOfColumn(unsigned int c, const char *label, unsigned int sheet_index=0);
    
    /// Set label for the row with the given index, of the sheet with the given index (default 0).
    int SetLabelOfRow(unsigned int r, mxString &label, unsigned int sheet_index=0);
    int SetLabelOfRow(unsigned int r, const char *label, unsigned int sheet_index=0);
    
    /// Set the number of sheets. Calls AddSheet() method requested number of times.
    /// Caution: this method will erase all existing values in the table.
    int SetNumberOfSheets(unsigned int number_of_sheets);
    
    /// Set value wiht indexes (r,c) from a double, for the sheet with the given index (default 0).
    int SetValue(int r, int c, double value, unsigned int sheet_index=0);
};


#endif
