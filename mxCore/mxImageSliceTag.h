/*=========================================================================
 
 Program:   mipx
 Module:    mxImageSliceTag.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#if defined(mxImageSliceTag_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxImageSliceTag_API
#else
    #if defined(_MSC_VER)
        #ifdef mxImageSliceTag_EXPORTS
            #define mxImageSliceTag_API __declspec(dllexport)
        #else
            #define mxImageSliceTag_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxImageSliceTag_EXPORTS
            #define mxImageSliceTag_API __attribute__((visibility("default")))
        #else
            #define mxImageSliceTag_API
        #endif
    #endif
#endif



#ifndef mxImageSliceTag_H
    #define mxImageSliceTag_H



#include <iostream>
#include "mxArray.h"
#include "mxList.h"
#include "mxMatrix.h"
#include "mxString.h"




#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif



/// Dicom tag number.

class mxImageSliceTag_API mxImageSliceTagNumber
{
public:
    
    /// Hexadecimal number (e.g. "0008)
    mxString m_number1;
    
    /// Hexadecimal number (e.g. "0013)
    mxString m_number2;
    
    /// Description of dicom tag (e.g. "Creation Time").
    mxString m_description;
};



/// Image slice tags of DICOM (and possibly other) tags and their values.

class mxImageSliceTag_API mxImageSliceTag
{
public:
    
    /// Array of tag numbers.
    static mxArray< mxImageSliceTagNumber > m_dicom_tag_names;

    /// Array of tag values.
    mxArray < mxString > m_dicom_tag_values;
    
    

	/// Constructor.
	mxImageSliceTag();

	/// Destructor.
    virtual ~mxImageSliceTag() {};

	/// For input tag numbers get value. If none found returns NULL.
	mxString* GetValue(const char *tag_number1, const char *tag_number2);

    /// For input tag numbers get value as double. Return success 1, fail 0.
    int GetValueAsDouble(const char *tag_number1, const char *tag_number2, double &output_value);

    /// For input tag numbers get a list of double values. Return success 1, fail 0.
    int GetValueAsListOfDoubles(const char *tag_number1, const char *tag_number2, mxList< double > &output_values);

    /// For input tag numbers get an array of double values. Return success 1, fail 0.
    int GetValueAsArrayOfDoubles(const char *tag_number1, const char *tag_number2, mxArray< double > &output_values);

	/// For input DICOM tag numbers, return the index. If none found returns -1.
	int IndexOfDicomTagNumber(const char *tag_number1, const char *tag_number2);

	/// Saving (printing) to a string to be used when saving to a file.
	void SaveValuesToString(mxString &s);
    
    /// Load tag numbers and descriptions from a string. Each tag should be enclosed with '<' and '>'.
    /// E.g.: <(0018,1060)[Trigger time]><(0020,0013)[Instance number]>.
    int LoadNumbersFromString(mxString &s);

	/// Load tag values from a string. Each tag should be enclosed with '<' and '>'.
	/// E.g.: <(0018,1060){0}><(0020,0013){5}>.
	int LoadValuesFromString(mxString &s);

};



/// Image tags of DICOM (and possibly other) tags and their values.

class mxImageSliceTag_API mxImageTag
{
public:
    
    /// Array of tag values.
    mxMatrix < mxImageSliceTag > m_tags;
    
    
    /// Constructor.
    mxImageTag();
    
    /// Destructor.
    virtual ~mxImageTag() {};
    
    /// Get image slice tag for indexes t,s.
    mxImageSliceTag& GetTag(int t, int s);

    /// Get image slice tag for index i.
    mxImageSliceTag& GetTag(int i);
    
    /// Reset the object to initial state.
    void Reset()
    {
        m_tags.Reset();
    }
    
    /// Set dimensions T and S for tags.
    void SetDimensions(int t, int s);
    
    /// Save tags to a file.
    void SaveToFile(const char *file_name);
    
    /// Load tags from a file.
    int LoadFromFile(const char *file_name);
    
};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif


#endif