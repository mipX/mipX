/*=========================================================================
 
 Program:   mipx
 Module:    mxProfile.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



// WE MAY NEED TO IMPLEMENT PARTIAL BUILDING (CREATION) OF THE LARGE mxProfile. NOW, WHEN A LARGE mxProfile IS BEING
// CREATED, WE EASILY RUN OUT OF MEMORY AND THE PROGRAM FAILS. THE SOLUTION WOULD BE TO SAVE PARTS OF THE mxProfile TO
// HARD DRIVE TO FREE MEMORY AND TO LOAD THESE PARTS (ONE BY ONE) WHEN THE WE WORK ON THE mxProfile (E.G. THRESHOLDING).
// THIS WAY A mxProfile WOULD BE RECORDED IN MULTIPLE FILES, AND PROCESSING MIGHT GO SLOWER BECAUSE OF CONSTANT LOADING
// AND SAVING, BUT THE PROGRAM WOULD NOT CRASH.




#if defined(mxProfile_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxProfile_API
#else
    #if defined(_MSC_VER)
        #ifdef mxProfile_EXPORTS
            #define mxProfile_API __declspec(dllexport)
        #else
            #define mxProfile_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxProfile_EXPORTS
            #define mxProfile_API __attribute__((visibility("default")))
        #else
            #define mxProfile_API
        #endif
    #endif
#endif



#ifndef mxProfile_H
    #define mxProfile_H


//#include <sstream>
//#include <fstream>
//#include <iostream>
//#include <cassert>
//#include <cstdlib>
////#include <iostream>

#include "mxDataObject.h"
#include "mxImage.h"
#include "mxList.h"
#include "mxMatrix.h"
#include "mxRegularGrid.h"
#include "mxString.h"



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




// Forward declaration.
class mxProfile;



/// Container class for values of profile vector and original voxel value.

class mxProfile_API mxProfileVector : public mxDataObject
{
public:
    //mxMatrix<float> m_vector;
    
    /// Pointer to container profile of this profile vector.
    mxProfile* m_profile;
    
    /// Actual values of the vector.
    float *m_values;
    
    /// Original voxel value from the image.
    mxImageScalar m_original_voxel_value;
    
    /// Constructor.
    mxProfileVector();
    
    /// Destructor.
    ~mxProfileVector();
    
    /// Indexing operator;
    float& operator ()(int t, int s, int r, int c);
    float& operator ()(int s, int r, int c);
    
    /// Reset the obejct to initial state.
    void Reset();
    
    /// Allocates memory of size s*r*c;
    void SetDimensions(int t, int s, int r, int c);
    
    /// Set pointer to container profile.
    void SetProfile(mxProfile *profile);
    
    
};



/// Image Profile class.
/// Use SetGridDimensions() and SetVectorDimensions() before all other methods to setup the profile.

class mxProfile_API mxProfile : public mxDataObject
{
public:
    
    /// Grid of profile vectors.
    mxRegularGrid< mxProfileVector* > m_grid;

    /// Total number of created vectors.
    int m_number_of_created_vectors;
    
#define mxProfile_NUMBER_OF_DIMENSIONS 4
    
    /// Size of profile vectors;
    int m_vector_dimensions[mxProfile_NUMBER_OF_DIMENSIONS];

    /// Labels (descriptions) of each dimension of the profile vector.
    mxString m_vector_dimensions_labels[mxProfile_NUMBER_OF_DIMENSIONS];
    
    /// Labels (descriptions) of each value in each dimension of the profile vector.
    mxArray< mxString > m_vector_values_labels[mxProfile_NUMBER_OF_DIMENSIONS];


    /// Constructor.
    mxProfile();
    
    /// Destructor.
    ~mxProfile()
    {
        this->Reset();
    };
    
    /// Copy data.
    /// NOT IMPLEMENTED YET>
    int CopyFrom(mxProfile *profile) { return 0; };
    
    /// Create vector at given indexes.
    /// Note: there is no checking if specified indexes are well defined or if the dimension of the vector has been set.
    void CreateVector(int s, int r, int c);
    
    /// Delete vector at given indexes.
    /// Note: there is no checking if specified indexes are well defined or if the dimension of the vector has been set.
    void DeleteVector(int s, int r,  int c);
    
    /// Reset the object to initial state.
    void Reset();
    
    /// Get the grid of the profile.
    mxRegularGrid< mxProfileVector* >& GetGrid()
    {
        return m_grid;
    };
    
    /// Get dimension T value.
    int GetGridDimensions_T() { return m_grid.GetNumberOfTimeSeries(); };
    
    /// Get dimension S value.
    int GetGridDimensions_S() { return m_grid.GetNumberOfSlices(); };
    
    /// Get dimension R value.
    int GetGridDimensions_R() { return m_grid.GetNumberOfRows(); };
    
    /// Get dimension C value.
    int GetGridDimensions_C() { return m_grid.GetNumberOfColumns(); };
    
    /// For a given dimensions label find its index.
    /// If none found returns -1.
    int GetIndexOfDimensionLabel(const char *label);
    
    /// For a given values label and dimensions index find its values index.
    /// If none found returns -1.
    int GetIndexOfValuesLabel(int dimensions_index, const char *label);
    
    /// For a given values label and dimensions index find its values index.
    /// If none found returns -1.
    int GetIndexOfValuesLabel_T(const char *label) { return this->GetIndexOfValuesLabel(3, label); };
    int GetIndexOfValuesLabel_S(const char *label) { return this->GetIndexOfValuesLabel(2, label); };
    int GetIndexOfValuesLabel_R(const char *label) { return this->GetIndexOfValuesLabel(1, label); };
    int GetIndexOfValuesLabel_C(const char *label) { return this->GetIndexOfValuesLabel(0, label); };
    
    /// Get number of created vectors.
    int GetNumberOfCreatedVectors() { return m_number_of_created_vectors; };
    
    /// Get the grid of the profile.
    mxProfileVector* GetVector(int s, int r, int c)
    {
        return m_grid(s,r,c);
    };

    /// Get dimensions of profile vector.
    int GetVectorDimensions_T() { return m_vector_dimensions[3]; };
    int GetVectorDimensions_S() { return m_vector_dimensions[2]; };
    int GetVectorDimensions_R() { return m_vector_dimensions[1]; };
    int GetVectorDimensions_C() { return m_vector_dimensions[0]; };
    
    
    /// Get label of vector dimension.
    mxString& GetVectorDimensionLabel_T() { return this->m_vector_dimensions_labels[3]; };
    mxString& GetVectorDimensionLabel_S() { return this->m_vector_dimensions_labels[2]; };
    mxString& GetVectorDimensionLabel_R() { return this->m_vector_dimensions_labels[1]; };
    mxString& GetVectorDimensionLabel_C() { return this->m_vector_dimensions_labels[0]; };
    
    
    /// Get array of vector values labels.
    mxArray< mxString >& GetVectorValuesLabels_T() { return m_vector_values_labels[3]; };
    mxArray< mxString >& GetVectorValuesLabels_S() { return m_vector_values_labels[2]; };
    mxArray< mxString >& GetVectorValuesLabels_R() { return m_vector_values_labels[1]; };
    mxArray< mxString >& GetVectorValuesLabels_C() { return m_vector_values_labels[0]; };
    
    
    /// Check if the profile grid is empty.
    int IsEmpty(){ return m_grid.IsEmpty(); };
    
    /// Check if the vector at given indexes has already been created.
    int IsVectorCreated(int s, int r, int c);
   
    /// Set dimensions of the profile grid.
    void SetGridDimensions(int s, int r, int c);
    
    /// Set the size of the profile vector.
    void SetVectorDimensions(int s, int r, int c);
    

    /// Set label string for each dimension of the profile vector.
    void SetVectorDimensionLabel(int index, const char *label)
    {
        m_vector_dimensions_labels[index].Assign(label);
    };

    /// Set label string for each dimension of the profile vector.
    void SetVectorDimensionLabel_T(const char *label) { this->SetVectorDimensionLabel(3, label); };
    void SetVectorDimensionLabel_S(const char *label) { this->SetVectorDimensionLabel(2, label); };
    void SetVectorDimensionLabel_R(const char *label) { this->SetVectorDimensionLabel(1, label); };
    void SetVectorDimensionLabel_C(const char *label) { this->SetVectorDimensionLabel(0, label); };
    
    /// Set label string for specific value of the dimension with the given index (of the profile vector).
    void SetVectorValuesLabel(int index_of_dimension, int index_of_value, const char *label)
    {
        m_vector_values_labels[index_of_dimension][index_of_value].Assign(label);
    };
    
    /// Set label string for specific value of the dimension with the given index (of the profile vector).
    void SetVectorValuesLabel_T(int index_of_value, const char *label) { this->SetVectorValuesLabel(3, index_of_value, label); };
    void SetVectorValuesLabel_S(int index_of_value, const char *label) { this->SetVectorValuesLabel(2, index_of_value, label); };
    void SetVectorValuesLabel_R(int index_of_value, const char *label) { this->SetVectorValuesLabel(1, index_of_value, label); };
    void SetVectorValuesLabel_C(int index_of_value, const char *label) { this->SetVectorValuesLabel(0, index_of_value, label); };

    /// Set label strings from a list of strings for a specific index of the dimension.
    void SetVectorValuesLabels(int index_of_dimension, mxList< mxString > &labels);
    
    /// Set label strings from a list of strings for a specific index of the dimension.
    void SetVectorValuesLabels_T(mxList< mxString > &labels) { this->SetVectorValuesLabels(3, labels); };
    void SetVectorValuesLabels_S(mxList< mxString > &labels) { this->SetVectorValuesLabels(2, labels); };
    void SetVectorValuesLabels_R(mxList< mxString > &labels) { this->SetVectorValuesLabels(1, labels); };
    void SetVectorValuesLabels_C(mxList< mxString > &labels) { this->SetVectorValuesLabels(0, labels); };
    
    /// Save profile to a file.
    int SaveToFile(const char *file_name);
    
    /// Load profile from a file.
    int LoadFromFile(const char *file_name);

};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif


#endif