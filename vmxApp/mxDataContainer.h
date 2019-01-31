/*=========================================================================
 
 Program:   mipx
 Module:    mxDataContainer.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxDataContainer_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxDataContainer_API
#else
    #if defined(_MSC_VER)
        #ifdef mxDataContainer_EXPORTS
            #define mxDataContainer_API __declspec(dllexport)
        #else
            #define mxDataContainer_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxDataContainer_EXPORTS
            #define mxDataContainer_API __attribute__((visibility("default")))
        #else
            #define mxDataContainer_API
        #endif
    #endif
#endif


#ifndef mxDataContainer_H
#define mxDataContainer_H



#include "mxArray.h"
#include "mxDataObject.h"
#include "mxList.h"
#include "mxString.h"
#include <iostream>



//class vtkRenderWindowInteractor;



//------------------------------------------------------------------------------


/// Each container step corresponds to a step in function configuration. This way we track
/// which data was loaded in each step.

class mxDataContainer_API mxDataContainerStep
{
public:
    
    /// Data objects passed as input for the current step.
    mxList< mxDataObject* > m_data_objects;

    /// Double numbers passed as input for the current step.
    mxList< double > m_doubles;
    
    /// Integer numbers passed as input for the current step.
	mxList< int > m_integers;
    
    /// Strings passed as input for the current step.
	mxList< mxString > m_strings;
    
    /// List of titles (under which options are contained) for radio options.
    mxList< mxString > m_radio_button_titles_of_options;

    /// List of chosen radio button options (each title contains only 1 chosen option).
    mxList< mxString > m_radio_button_chosen_options;

    /// List of titles (under which options are contained) for check boxes.
    mxList< mxString > m_check_box_titles_of_options;

    /// List of chosen check box options (each title may contain multiple chosen options).
    mxList< mxList< mxString > > m_check_box_chosen_options;


	/// Constructor.
	mxDataContainerStep();
    
    /// Destructor.
	~mxDataContainerStep();

	/// Reset the container step to be empty.
    void Reset();
};



//------------------------------------------------------------------------------



class mxDataContainer_API mxDataContainer
{
public:

	/// List of loaded data containers for each of the steps.
    mxList< mxDataContainerStep > m_input_steps;
    
//    /// Tool objects passed as input.
//    mxList< mxToolObject* > m_tool_objects;


//	/// Render window used by main app.
//	vtkRenderWindowInteractor *m_interactor;

	/// Output text shown after the function finishes execution.
    /// Any textual results, remarks, warnings or errors can be shown here.
    mxString m_output_text;
    
    /// Output data of a function. This data will be addded to the data tree after function finishes execution,
    /// i.e. it was not pre-allocated before the function was started.
    mxDataContainerStep m_output_data;

    
	/// Constructor.
    mxDataContainer();
	
    /// Destructor.
    ~mxDataContainer();

	/// Resets the object to empty state.
	void Reset();
    
    /// Add a new data container step with a given index (if such a step already exists, just returns its pointer).
    mxDataContainerStep* AddStep(unsigned int index_of_step);
    
    /// Add data object produced by a function as its output.
    void AddOutputDataObject(mxDataObject *data_object);
    
    /// Append (add) output text shown after the function finishes execution.
    /// Any textual results, remarks, warnings or errors can be shown here.
    void AppendOutputText(const char *text);
    
    /// Assign (set) output text shown after the function finishes execution.
    /// Any textual results, remarks, warnings or errors can be shown here.
    void AssignOutputText(const char *text);
    
    /// Get array of input data objects of all types for all steps.
    mxArray< mxDataObject* > GetDataObjects();
    
    /// Get array of input data objects of given type for all steps.
    mxArray< mxDataObject* > GetDataObjectsOfType(const char *data_object_type_name);
    
    /// Get array of input data objects of given type for the given step.
    mxArray< mxDataObject* > GetDataObjectsOfTypeForStep(const char *data_object_type_name, unsigned int index_of_step);

    /// Get array of input double numbers for all steps.
    mxArray< double > GetDoubleNumbers();
    
    /// Get array of input integer numbers for all steps.
    mxArray< int > GetIntegerNumbers();
    
    /// Get output text shown after the function finishes execution.
    /// Any textual results, remarks, warnings or errors can be shown here.
    mxString GetOutputText();
    
    /// Get selected radio button option for the given radio button options title.
    mxString GetSelectedRadioButtonOption(const char *radio_button_options_title);

    /// Get selected check box options for the given check box options title.
    mxArray< mxString > GetSelectedCheckBoxOptions(const char *check_box_options_title);
    
    /// Get data container step with index 'step'. If fail, returns NULL.
    mxDataContainerStep* GetStep(unsigned int index_of_step);
    
    /// Get array of input strings for all steps.
    mxArray< mxString > GetStrings();

//	/// Get interactor.
//	vtkRenderWindowInteractor* GetVTKRenderWindowInteractor();

};




#endif