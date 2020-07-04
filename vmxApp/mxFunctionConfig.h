/*=========================================================================
 
 Program:   mipx
 Module:    mxFunctionConfig.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxFunctionConfig_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxFunctionConfig_API
#else
    #if defined(_MSC_VER)
        #ifdef mxFunctionConfig_EXPORTS
            #define mxFunctionConfig_API __declspec(dllexport)
        #else
            #define mxFunctionConfig_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxFunctionConfig_EXPORTS
            #define mxFunctionConfig_API __attribute__((visibility("default")))
        #else
            #define mxFunctionConfig_API
        #endif
    #endif
#endif


#ifndef mxFunctionConfig_H
    #define mxFunctionConfig_H



#include "mxFunctionObject.h"
#include "mxString.h"
#include <iostream>



#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#endif




// Macro to be used to abort the function (stop from further execution).
// The syntax is made in such a way to allow to use MIPX_FUNCTION_ABORT_AND_EXIT as a normal command.
#define MIPX_FUNCTION_ABORT_AND_EXIT do{ this->Abort(); return; }while(0)






class mxFunctionConfig_API mxFunctionConfigStepDataObjectInput
{
public:
    
    /// Text shown for this input.
    mxString m_text;
    
    /// Maximum (limit) number of data object that this input can accept. The actual number of input object must be equal or lower than this one.
    unsigned int m_max_number_of_data_objects;
    
    /// Constructor.
    mxFunctionConfigStepDataObjectInput() { m_max_number_of_data_objects = 100; };
    
    /// Destructor.
    ~mxFunctionConfigStepDataObjectInput() {};
};


class mxFunctionConfig_API mxFunctionConfigStepIntegerInput
{
public:
    
    /// Text shown for this input.
    mxString m_text;
    
    /// Lowest possible value for this input.
    int m_minimum_value;
    
    /// Highest possible value for this input.
    int m_maximum_value;
    
    /// Default value for this input.
    int m_default_value;
    
    /// When changing the input value, the step value is used to increment/decrement the current input value in a single iteration.
    int m_step;
    
    /// Constructor.
    mxFunctionConfigStepIntegerInput() { m_minimum_value = 0; m_maximum_value = 32565; m_default_value = 0; m_step = 1; };
    
    /// Destructor.
    ~mxFunctionConfigStepIntegerInput() {};
};


class mxFunctionConfig_API mxFunctionConfigStepDoubleInput
{
public:
    
    /// Text shown for this input.
    mxString m_text;
    
    /// Lowest possible value for this input.
    double m_minimum_value;
    
    /// Highest possible value for this input.
    double m_maximum_value;
    
    /// Default value for this input.
    double m_default_value;
    
    /// When changing the input value, the step value is used to increment/decrement the current input value in a single iteration.
    double m_step;
    
    // Precision in therms of the number of decimals allowed for this input.
    int m_number_of_decimals;
    
    /// Constructor.
    mxFunctionConfigStepDoubleInput() { m_minimum_value = 0; m_maximum_value = 32565; m_default_value = 0; m_step = 1; m_number_of_decimals = 1; };
    
    /// Destructor.
    ~mxFunctionConfigStepDoubleInput() {};
};


class mxFunctionConfig_API mxFunctionConfigStepStringInput
{
public:
    
    /// Text shown for this input.
    mxString m_text;
    
    /// Default string for this input.
    mxString m_default_string;

    /// Constructor.
	mxFunctionConfigStepStringInput() { m_default_string.Assign(" "); };
    
    /// Destructor.
    ~mxFunctionConfigStepStringInput() {};
};


class mxFunctionConfig_API mxFunctionConfigStepRadioButtonInput
{
public:
    
    /// Text shown for this input.
    mxString m_text;
    
    /// List of options from which the user will choose.
    mxList<mxString> m_list_of_options;
    
    /// Default checked radio button (option) given by its index.
    unsigned int m_index_of_default_checked_option;

    /// Constructor.
    mxFunctionConfigStepRadioButtonInput() { m_index_of_default_checked_option = 0; };
    
    /// Destructor.
    ~mxFunctionConfigStepRadioButtonInput() {};
};


class mxFunctionConfig_API mxFunctionConfigStepCheckBoxInput
{
public:
    
    /// Text shown for this input.
    mxString m_text;
    
    /// List of options from which the user will choose.
    mxList<mxString> m_list_of_options;
    
    /// List of default checked check boxes (options) given by their indexes.
    mxList<unsigned int> m_list_of_indexes_of_default_checked_options;
    
    /// Constructor.
    mxFunctionConfigStepCheckBoxInput() {};
    
    /// Destructor.
    ~mxFunctionConfigStepCheckBoxInput() {};
};


class mxFunctionConfig_API mxFunctionConfigStep
{
public:
	
    /// List of input data objects.
    mxList<mxFunctionConfigStepDataObjectInput> m_data_objects;
    
    /// List of input integer numbers.
    mxList<mxFunctionConfigStepIntegerInput> m_integers;

    /// List of input double numbers.
    mxList<mxFunctionConfigStepDoubleInput> m_doubles;
    
    /// List of input strings.
    mxList<mxFunctionConfigStepStringInput> m_strings;

    /// List of input radio button options.
    mxList<mxFunctionConfigStepRadioButtonInput> m_radio_button_options;

    /// List of input radio button options.
    mxList<mxFunctionConfigStepCheckBoxInput> m_check_box_options;

    /// Constructor.
	mxFunctionConfigStep();
    
    /// Destructor.
	~mxFunctionConfigStep();
    
    /// Reset all the lists, makes this object empty.
	void Reset();
};



class mxFunctionConfig_API mxFunctionConfig
{
public:
    
/// Define the maximum number of steps.
#define mxFunctionConfig_maximum_number_of_steps 5 

    /// Define the maximum number of steps as a static variable.
    static const unsigned int m_maximum_number_of_steps = mxFunctionConfig_maximum_number_of_steps;

	/// Array of function configuration steps.
    mxFunctionConfigStep m_steps[mxFunctionConfig_maximum_number_of_steps]; //m_step_00, m_step_01, m_step_02, m_step_03, m_step_04;

	/// The actual (real) number of steps for this function configuration.
	unsigned int m_number_of_steps;

	/// Index of the current step.
    int m_index_of_current_step;

	/// Function object linked to the function configuration.
    mxFunctionObject *m_function_object;

	/// Text containing brief description of the method and help on its use.
	mxString m_help_text;

	/// Title of the function being executed. This 'long' title will be displayed on function initialization.
    mxString m_title_long;

	/// Title of the function that is shown in the menu.
    mxString m_title_in_menu;



	/// Constructor.
	mxFunctionConfig();

	/// Destructor.
	~mxFunctionConfig();
    
    /// Setup input of data object types for the current function configuration step.
    void AddInputDataObjects(const char *text);
    
    /// Setup input of integer value for the current function configuration step.
    void AddInputIntegerValue(const char *text, int minimum_value, int maximum_value, int default_value);

    /// Setup input of double value for the current function configuration step.
    void AddInputDoubleValue(const char *text, int minimum_value, int maximum_value, int default_value, int number_of_decimals);

    /// Setup input of string for the current function configuration step.
    void AddInputString(const char *text, const char *default_string);
    
    /// Setup input of radio button options for the current function configuration step.
    void AddInputRadioButtonOptions(const char *text, mxList<mxString> *list_of_options, unsigned int index_of_default_option = 0);
    
    /// Setup input of check box options for the current function configuration step.
    void AddInputCheckBoxOptions(const char *text, mxList<mxString> *list_of_options, mxList<unsigned int> *list_of_indexes_of_default_checked_options = NULL);

    /// Decrements index of the current step.
    void DecrementCurrentStepIndex();
    
    /// Get the current step of this function configuration.
    mxFunctionConfigStep* GetCurrentStep();
    
    /// Get index of the current step.
    int GetIndexOfCurrentStep() { return m_index_of_current_step; };
    
    /// Get the actual (real) number of steps for this function configuration.
    int GetNumberOfSteps() { return m_number_of_steps; };
    
    /// Get the step defined by its index.
    mxFunctionConfigStep* GetStep(unsigned int step_index);
    
	/// Get name of the function to appear in the menu.
    mxString GetTitleInMenu() { return m_title_in_menu; };

    /// Get the title of the function shown when its being executed. This 'long' title will be displayed on function initialization.
    mxString GetTitleLong() { return m_title_long; };
    
    /// Increments index of the current step.
    void IncrementIndexOfCurrentStep();
    
    /// Reset the object to its initial state.
    void Reset();
    
    /// Reset the current step of this function configuration.
    void ResetCurrentStep();
    
    /// Set the function object linked to the function configuration.
    void SetFunctionObject(mxFunctionObject *function_object) { m_function_object = function_object; };
    
    /// Set the text containing brief description of the method and help on its use.
    void SetHelpText(const char *help_text) { m_help_text.Assign(help_text); };
    
    /// Set index of the current step.
    int SetIndexOfCurrentStep(int step_index);
    
    /// Set the actual (real) number of steps for this function configuration.
    int SetNumberOfSteps(unsigned int number_of_steps);
    
    /// Set name of the function to appear in the menu.
    void SetTitleInMenu(const char *title_in_menu) { m_title_in_menu.Assign(title_in_menu); };
    
    /// Set the title of the function shown when its being executed. This 'long' title will be displayed on function initialization.
    void SetTitleLong(const char *title_long) { m_title_long.Assign(title_long); };

};



#if defined(_MSC_VER)
#pragma warning (default : 4251)
#endif




#endif
