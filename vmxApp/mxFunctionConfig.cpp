/*=========================================================================

 Program:   mipx
 Module:    mxFunctionConfig.cpp

 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "mxFunctionConfig.h"



//--------------------------------------------------------------------------------



mxFunctionConfigStep::mxFunctionConfigStep()
{
}


mxFunctionConfigStep::~mxFunctionConfigStep()
{
	this->Reset();
}


void mxFunctionConfigStep::Reset()
{
    m_check_box_options.Reset();
    m_data_objects.Reset();
    m_doubles.Reset();
    m_integers.Reset();
    m_radio_button_options.Reset();
    m_strings.Reset();
}



//--------------------------------------------------------------------------------



mxFunctionConfig::mxFunctionConfig()
{
	m_function_object = NULL;
    m_help_text.Assign(" ");
    m_index_of_current_step = 0;
    m_number_of_steps = 1;
    m_title_in_menu.Assign(" ");
    m_title_long.Assign(" ");
}


mxFunctionConfig::~mxFunctionConfig()
{
	this->Reset();
}


void mxFunctionConfig::AddInputDataObjects(const char *text)
{
    mxFunctionConfigStep *step = this->GetCurrentStep();
    if(step)
    {
        mxFunctionConfigStepDataObjectInput step_data_object_input;
        step_data_object_input.m_text.Assign(text);
        step->m_data_objects.AddToEnd(step_data_object_input);
    }
}


void mxFunctionConfig::AddInputIntegerValue(const char *text, int minimum_value, int maximum_value, int default_value)
{
    mxFunctionConfigStep *step = this->GetCurrentStep();
    if(step)
    {
        mxFunctionConfigStepIntegerInput step_integer_input;
        step_integer_input.m_text.Assign(text);
        step_integer_input.m_minimum_value = minimum_value;
        step_integer_input.m_maximum_value = maximum_value;
        step_integer_input.m_default_value = default_value;
        step->m_integers.AddToEnd(step_integer_input);
    }
}


void mxFunctionConfig::AddInputDoubleValue(const char *text, int minimum_value, int maximum_value, int default_value, int number_of_decimals)
{
    mxFunctionConfigStep *step = this->GetCurrentStep();
    if(step)
    {
        mxFunctionConfigStepDoubleInput step_double_input;
        step_double_input.m_text.Assign(text);
        step_double_input.m_minimum_value = minimum_value;
        step_double_input.m_maximum_value = maximum_value;
        step_double_input.m_default_value = default_value;
        step_double_input.m_number_of_decimals = number_of_decimals;
        step->m_doubles.AddToEnd(step_double_input);
    }
}


void mxFunctionConfig::AddInputString(const char *text, const char *default_string)
{
    mxFunctionConfigStep *step = this->GetCurrentStep();
    if(step)
    {
        mxFunctionConfigStepStringInput step_string_input;
        step_string_input.m_text.Assign(text);
        step_string_input.m_default_string.Assign(default_string);
        step->m_strings.AddToEnd(step_string_input);
    }
}


void mxFunctionConfig::AddInputRadioButtonOptions(const char *text, mxList<mxString> *list_of_options, unsigned int index_of_default_option)
{
    mxFunctionConfigStep *step = this->GetCurrentStep();
    if(step)
    {
        mxFunctionConfigStepRadioButtonInput step_radio_button_input;
        step_radio_button_input.m_text.Assign(text);
        step_radio_button_input.m_list_of_options = (*list_of_options);
        step_radio_button_input.m_index_of_default_checked_option = index_of_default_option;
        step->m_radio_button_options.AddToEnd(step_radio_button_input);
    }
}


void mxFunctionConfig::AddInputCheckBoxOptions(const char *text, mxList<mxString> *list_of_options, mxList<unsigned int> *list_of_indexes_of_default_checked_options)
{
    mxFunctionConfigStep *step = this->GetCurrentStep();
    if(step)
    {
        mxFunctionConfigStepCheckBoxInput step_check_box_input;
        step_check_box_input.m_text.Assign(text);
        step_check_box_input.m_list_of_options = (*list_of_options);
        if(list_of_indexes_of_default_checked_options)
        {
            step_check_box_input.m_list_of_indexes_of_default_checked_options = (*list_of_indexes_of_default_checked_options);
        }
        step->m_check_box_options.AddToEnd(step_check_box_input);
    }
}


void mxFunctionConfig::DecrementCurrentStepIndex()
{
    if(m_index_of_current_step > 0) m_index_of_current_step--;
}


mxFunctionConfigStep* mxFunctionConfig::GetCurrentStep()
{
	return (this->GetStep(m_index_of_current_step));
}


mxFunctionConfigStep* mxFunctionConfig::GetStep(unsigned int step_index)
{
	if(step_index >= (unsigned int)this->GetNumberOfSteps())
    {
        return NULL;
    }
    
    return &(m_steps[step_index]);
}


void mxFunctionConfig::IncrementIndexOfCurrentStep()
{
	if(m_index_of_current_step < this->GetNumberOfSteps()-1) m_index_of_current_step++;
}


void mxFunctionConfig::Reset()
{
    for(unsigned int i=0; i<mxFunctionConfig_maximum_number_of_steps; i++)
    {
        m_steps[i].Reset();
    }
}


void mxFunctionConfig::ResetCurrentStep()
{
    mxFunctionConfigStep *step = this->GetCurrentStep();
    if(step)
    {
        step->Reset();
    }
}


int mxFunctionConfig::SetIndexOfCurrentStep(int step_index)
{
    if(step_index < (int)m_number_of_steps)
    {
        m_index_of_current_step = step_index;
        return 1;
    }
    return 0;
}


int mxFunctionConfig::SetNumberOfSteps(unsigned int number_of_steps)
{
    if(number_of_steps <= m_maximum_number_of_steps)
    {
        m_number_of_steps = number_of_steps;
        return 1;
    }
    return 0;
}

