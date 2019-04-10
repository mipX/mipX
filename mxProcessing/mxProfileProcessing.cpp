/*=========================================================================
 
 Program:   mipx
 Module:    mxProfileProcessing.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "mxProfileProcessing.h"



mxProfileProcessing::mxProfileProcessing()
{
    m_s_min.Assign("min");
    m_s_max.Assign("max");
    m_s_med_minus.Assign("med(-)");
    m_s_med_plus.Assign("med(+)");
    m_s_avr.Assign("avr");
    m_s_med_avr.Assign("med_avr");
    m_s_minmax_avr.Assign("minmax_avr");
    m_s_med_root.Assign("med_root");
    m_s_minmax_root.Assign("minmax_root");

}



mxProfileProcessing::~mxProfileProcessing()
{
}


int mxProfileProcessing::GetIndexArrayForOperatorStringArray(mxProfile &input, mxArray< mxString> &operators, mxArray<int> &output_index_array)
{
    output_index_array.SetNumberOfElements(operators.GetNumberOfElements());
    
    for(int i=0; i<operators.GetNumberOfElements(); i++)
    {
        int is_found = 0; //Indicator will show if the requested function was found
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements(); c++)
        {
            if(input.GetVectorValuesLabels_C()[c] == operators[i])
            {
                output_index_array[i] = c;
                is_found = 1;
                break;
            }
        }
        if(!is_found) return 0;
    }
    return 1;
}


int mxProfileProcessing::GetIndexArrayForProfileFunction(mxProfile &input, unsigned int profile_function, mxArray<int> &output_index_array)
{
    int i_min = 0;
    int i_max = 0;
    int i_med_minus = 0;
    int i_med_plus = 0;
    int i_avr = 0;
    int i_minmax_avr = 0;
    int i_med_avr = 0;
    int i_minmax_root = 0;
    int i_med_root = 0;
    
    {
    int n = 0;
    if((profile_function & 1)==1) { i_min = 1; n++;}
    if((profile_function & 2)==2) { i_max = 1; n++;}
    if((profile_function & 4)==4) { i_med_minus = 1; n++;}
    if((profile_function & 8)==8) { i_med_plus = 1; n++;}
    if((profile_function & 16)==16) { i_avr = 1; n++;}
    if((profile_function & 32)==32) { i_med_avr = 1; n++;}
    if((profile_function & 64)==64) { i_minmax_avr = 1; n++;}
    if((profile_function & 128)==128) { i_med_root = 1; n++;}
    if((profile_function & 256)==256) { i_minmax_root = 1; n++;}
    
    output_index_array.SetNumberOfElements(n);
    }
    
//    {
//    int n = 0;
//    if((profile_function & 1)==1) { output_index_array[n] = 0; n++;}
//    if((profile_function & 2)==2) { output_index_array[n] = 1; n++;}
//    if((profile_function & 4)==4) { output_index_array[n] = 2; n++;}
//    if((profile_function & 8)==8) { output_index_array[n] = 3; n++;}
//    if((profile_function & 16)==16) { output_index_array[n] = 4; n++;}
//    if((profile_function & 32)==32) { output_index_array[n] = 5; n++;}
//    if((profile_function & 64)==64) { output_index_array[n] = 6; n++;}
//    if((profile_function & 128)==128) { output_index_array[n] = 7; n++;}
//    if((profile_function & 256)==256) { output_index_array[n] = 8; n++;}
//    }
    
//    for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements(); c++) //vector_c_values_desc.NumberOfElements() && !is_found
//    {
//        std::cout<<"  ^"<<input.GetVectorValuesLabels_C()[c].Get_C_String()<<"^ ";
//    }

    
    int index = 0;
    if(i_min)
    {
        int is_found = 0;//Indicator will show if the requested function was found
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++) //vector_c_values_desc.NumberOfElements() && !is_found
        {
            if(input.GetVectorValuesLabels_C()[c] == this->m_s_min)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;//If the requested basic function was not found, the combined function can NOT be calculated, so return 0.
    }
    if(i_max)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_max)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
    if(i_med_minus)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_med_minus)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
    if(i_med_plus)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_med_plus)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
    if(i_avr)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_avr)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
    if(i_med_avr)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_med_avr)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
    if(i_minmax_avr)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_minmax_avr)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
    if(i_med_root)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_med_root)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
    if(i_minmax_root)
    {
        int is_found = 0;
        for(int c=0; c<input.GetVectorValuesLabels_C().GetNumberOfElements() && !is_found; c++)
        {
            if(input.GetVectorValuesLabels_C()[c]==this->m_s_minmax_root)
            {
                output_index_array[index] = c;
                is_found = 1;
                index++;
            }
        }
        if(!is_found) return 0;
    }
//
//
//    for(int i=0; i<output_index_array.GetNumberOfElements(); i++)
//    {
//        std::cout<<" "<<output_index_array[i];
//    }
//
    
    return 1;
}



int mxProfileProcessing::Make_R_Profile_SphereDSE(mxImage &input_image, mxImage &mask, mxProfile &output_profile, int radius_limit, unsigned int functions)
{
    //cout<<"in img: s="<<input_image.NumberOfSlices()<<",r="<<input_image.NumberOfRows()<<"c="<<input_image.NumberOfColumns()<<endl;
    if(radius_limit<0) return 0;
    if(radius_limit > 20) radius_limit = 20;
    if(functions==0) return 0;
    
    output_profile.Reset();
    output_profile.SetGridDimensions(input_image.GetDimension_S(), input_image.GetDimension_R(), input_image.GetDimension_C());
    //cout<<"out pfr: s="<<output_profile.NumberOfSlices()<<",r="<<output_profile.NumberOfRows()<<"c="<<output_profile.NumberOfColumns()<<endl;
    
    output_profile.SetVectorDimensionLabel_S("n/a");
    output_profile.SetVectorDimensionLabel_R("r");
    output_profile.SetVectorDimensionLabel_C("Operators");//Functions
    output_profile.GetGrid().SetOrigin(input_image.GetOrigin_T(), input_image.GetOrigin_S(), input_image.GetOrigin_R(), input_image.GetOrigin_C());
    output_profile.GetGrid().SetSpacing(input_image.GetSpacing_T(), input_image.GetSpacing_S(), input_image.GetSpacing_R(), input_image.GetSpacing_C());
    
    mxGeometry g;
    g.SetDimensions(input_image.GetDimension_S(), input_image.GetDimension_R(), input_image.GetDimension_C());
    
    int i_min = 0; int i_max = 0; int i_med_minus = 0; int i_med_plus = 0; int i_avr = 0; int i_minmax_avr = 0;
    int i_med_avr = 0; int i_minmax_root = 0; int i_med_root = 0;
    
    mxList< mxString> function_string_list;
    
    if((functions & 1)==1) { i_min = 1; function_string_list.AddToEnd(m_s_min);}
    if((functions & 2)==2) { i_max = 1; function_string_list.AddToEnd(m_s_max);}
    if((functions & 4)==4) { i_med_minus = 1; function_string_list.AddToEnd(m_s_med_minus);}
    if((functions & 8)==8) { i_med_plus = 1; function_string_list.AddToEnd(m_s_med_plus);}
    if((functions & 16)==16) { i_avr = 1; function_string_list.AddToEnd(m_s_avr);}
    if((functions & 32)==32) { i_med_avr = 1; function_string_list.AddToEnd(m_s_med_avr);}
    if((functions & 64)==64) { i_minmax_avr = 1; function_string_list.AddToEnd(m_s_minmax_avr);}
    if((functions & 128)==128) { i_med_root = 1; function_string_list.AddToEnd(m_s_med_root);}
    if((functions & 256)==256) { i_minmax_root = 1; function_string_list.AddToEnd(m_s_minmax_root);}
    
    output_profile.SetVectorDimensions(1, radius_limit, function_string_list.GetNumberOfElements());
    
    // Set the descriptions for vector values
    output_profile.SetVectorValuesLabels_C(function_string_list);
    
    
    for(int sm=0; sm<output_profile.GetGridDimensions_S(); sm++)
    {
        std::cout<<"*"<<sm<<"*";
        for(int rm=0; rm<output_profile.GetGridDimensions_R(); rm++)
        {
            std::cout<<"-";
            for(int cm=0; cm<output_profile.GetGridDimensions_C(); cm++)
            {
                if(mask(sm,rm,cm)!=0)
                {
                    //For each point we do the following:
                    for(int radius = 0; radius<radius_limit-1 && (radius*radius)<g.GetMaxSphereSquaredRadius(); radius++)
                    {
                        unsigned short min = 65535, max = 0;
                        unsigned int sum = 0;
                        mxList< unsigned short > values_list;
                        
                        int sn,rn,cn;
                        for(g.ForSphere(sm,rm,cm, radius*radius); g.GetSphere((radius+1)*(radius+1), sn,rn,cn); )
                        {
                            values_list.AddToEnd(input_image(sn,rn,cn));
                            
                            if(input_image(sn,rn,cn) < min) min = input_image(sn,rn,cn);
                            if(input_image(sn,rn,cn) > max) max = input_image(sn,rn,cn);
                            
                            sum += input_image(sn,rn,cn);
                        }
                        
                        //If the list of values is not empty (watchout, because it can be!)
                        if(!values_list.IsEmpty())
                        {
                            
                            //If the array requires sorting, sort it.
                            float med_minus = 0, med_plus = 0;
                            if(i_med_minus || i_med_plus || i_med_avr || i_med_root)
                            {
                                mxListFunctions::SortAscending(values_list);
                                med_minus = (float) (values_list[values_list.GetNumberOfElements()/2-1]);
                                med_plus = (float) (values_list[values_list.GetNumberOfElements()/2]);
                            }
                            output_profile.CreateVector(sm,rm,cm);
                            
                            output_profile.GetVector(sm,rm,cm)->m_original_voxel_value = input_image(sm,rm,cm);
                            
                            //Assign calculated vector values
                            int array_index = 0;
                            if(i_min)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float) min;
                                array_index++;
                            }
                            if(i_max)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float) max;
                                array_index++;
                            }
                            if(i_med_minus)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = med_minus;
                                array_index++;
                            }
                            if(i_med_plus)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = med_plus;
                                array_index++;
                            }
                            if(i_avr)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = ((float)sum)/((float) values_list.GetNumberOfElements());
                                array_index++;
                            }
                            if(i_med_avr)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (med_minus+med_plus)/2.0;
                                array_index++;
                            }
                            if(i_minmax_avr)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = ((float)(min+max))/2.0;
                                array_index++;
                            }
                            if(i_med_root)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float)(sqrt((double)(med_minus*med_plus)));
                                array_index++;
                            }
                            if(i_minmax_root)
                            {
                                if(min==0) output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float)(sqrt((double)(max)));
                                else output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float)(sqrt((double)(min*max)));
                                array_index++;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 1;
}




int mxProfileProcessing::Make_R_Profile_CircleDSE(mxImage &input_image, mxImage &mask, mxProfile &output_profile, int radius_limit, unsigned int functions)
{
    //cout<<"in img: s="<<input_image.NumberOfSlices()<<",r="<<input_image.NumberOfRows()<<"c="<<input_image.NumberOfColumns()<<endl;
    if(radius_limit<0) return 0;
    if(radius_limit > 20) radius_limit = 20;
    if(functions==0) return 0;
    
    output_profile.Reset();
    output_profile.SetGridDimensions(input_image.GetDimension_S(), input_image.GetDimension_R(), input_image.GetDimension_C());
    //cout<<"out pfr: s="<<output_profile.NumberOfSlices()<<",r="<<output_profile.NumberOfRows()<<"c="<<output_profile.NumberOfColumns()<<endl;
    
    output_profile.SetVectorDimensionLabel_S("n/a");
    output_profile.SetVectorDimensionLabel_R("r");
    output_profile.SetVectorDimensionLabel_C("Operators");//Functions
    output_profile.GetGrid().SetOrigin(input_image.GetOrigin_T(), input_image.GetOrigin_S(), input_image.GetOrigin_R(), input_image.GetOrigin_C());
    output_profile.GetGrid().SetSpacing(input_image.GetSpacing_T(), input_image.GetSpacing_S(), input_image.GetSpacing_R(), input_image.GetSpacing_C());
    
    mxGeometry g;
    g.SetDimensions(input_image.GetDimension_S(), input_image.GetDimension_R(), input_image.GetDimension_C());
    
    int i_min = 0;
    int i_max = 0;
    int i_med_minus = 0;
    int i_med_plus = 0;
    int i_avr = 0;
    int i_minmax_avr = 0;
    int i_med_avr = 0;
    int i_minmax_root = 0;
    int i_med_root = 0;
    
    mxList< mxString> function_string_list;
    
    if((functions & 1)==1) { i_min = 1; function_string_list.AddToEnd(m_s_min);}
    if((functions & 2)==2) { i_max = 1; function_string_list.AddToEnd(m_s_max);}
    if((functions & 4)==4) { i_med_minus = 1; function_string_list.AddToEnd(m_s_med_minus);}
    if((functions & 8)==8) { i_med_plus = 1; function_string_list.AddToEnd(m_s_med_plus);}
    if((functions & 16)==16) { i_avr = 1; function_string_list.AddToEnd(m_s_avr);}
    if((functions & 32)==32) { i_med_avr = 1; function_string_list.AddToEnd(m_s_med_avr);}
    if((functions & 64)==64) { i_minmax_avr = 1; function_string_list.AddToEnd(m_s_minmax_avr);}
    if((functions & 128)==128) { i_med_root = 1; function_string_list.AddToEnd(m_s_med_root);}
    if((functions & 256)==256) { i_minmax_root = 1; function_string_list.AddToEnd(m_s_minmax_root);}
    
    output_profile.SetVectorDimensions(1, radius_limit, function_string_list.GetNumberOfElements());
    
    // Set the descriptions for vector values
    output_profile.SetVectorValuesLabels_C(function_string_list);
    
    
    for(int sm=0; sm<output_profile.GetGridDimensions_S(); sm++)
    {
        std::cout<<"*"<<sm<<"*";
        for(int rm=0; rm<output_profile.GetGridDimensions_R(); rm++)
        {
            std::cout<<"-";
            for(int cm=0; cm<output_profile.GetGridDimensions_C(); cm++)
            {
                if(mask(sm,rm,cm)!=0)
                {
                    //For each point we do the following:
                    for(int radius = 0; radius<radius_limit-1 && (radius*radius)<g.GetMaxSphereSquaredRadius(); radius++)
                    {
                        unsigned short min = 65535, max = 0;
                        unsigned int sum = 0;
                        mxList< unsigned short > values_list;
                        
                        int rn,cn;
                        for(g.ForCircle(rm,cm, radius*radius); g.GetCircle((radius+1)*(radius+1), rn,cn); )
                        {
                            values_list.AddToEnd(input_image(sm,rn,cn));
                            
                            if(input_image(sm,rn,cn) < min) min = input_image(sm,rn,cn);
                            if(input_image(sm,rn,cn) > max) max = input_image(sm,rn,cn);
                            
                            sum += input_image(sm,rn,cn);
                        }
                        
                        //If the list of values is not empty (watchout, because it can be!)
                        if(!values_list.IsEmpty())
                        {
                            
                            //If the array requires sorting, sort it.
                            float med_minus = 0, med_plus = 0;
                            if(i_med_minus || i_med_plus || i_med_avr || i_med_root)
                            {
                                mxListFunctions::SortAscending(values_list);
                                med_minus = (float) (values_list[values_list.GetNumberOfElements()/2-1]);
                                med_plus = (float) (values_list[values_list.GetNumberOfElements()/2]);
                            }
                            output_profile.CreateVector(sm,rm,cm);
                            
                            output_profile.GetVector(sm,rm,cm)->m_original_voxel_value = input_image(sm,rm,cm);
                            
                            //Assign calculated vector values
                            int array_index = 0;
                            if(i_min)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float) min;
                                array_index++;
                            }
                            if(i_max)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float) max;
                                array_index++;
                            }
                            if(i_med_minus)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = med_minus;
                                array_index++;
                            }
                            if(i_med_plus)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = med_plus;
                                array_index++;
                            }
                            if(i_avr)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = ((float)sum)/((float) values_list.GetNumberOfElements());
                                array_index++;
                            }
                            if(i_med_avr)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (med_minus+med_plus)/2.0;
                                array_index++;
                            }
                            if(i_minmax_avr)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = ((float)(min+max))/2.0;
                                array_index++;
                            }
                            if(i_med_root)
                            {
                                output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float)(sqrt((double)(med_minus*med_plus)));
                                array_index++;
                            }
                            if(i_minmax_root)
                            {
                                if(min==0) output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float)(sqrt((double)(max)));
                                else output_profile.GetVector(sm,rm,cm)->operator()(0,radius,array_index) = (float)(sqrt((double)(min*max)));
                                array_index++;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 1;
}





//int mxProfileProcessing::ProfileMeasure(mxProfile &input, mxImage &output, unsigned int profile_function, mxComparisonOperator GP_operator, int start_r_or_n_value)
//{
//    if(input.IsEmpty()) return 0;
//    if(profile_function==0) return 0;
//
//    if(start_r_or_n_value<1) start_r_or_n_value = 1;
//
//    output.SetDimensions(1,input.GetGridDimensions_S(), input.GetGridDimensions_R(), input.GetGridDimensions_C());
//    output.SetOrigin(input.GetGrid().GetOrigin_T(), input.GetGrid().GetOrigin_S(), input.GetGrid().GetOrigin_R(),input.GetGrid().GetOrigin_C());
//    output.SetSpacing(input.GetGrid().GetSpacing_T(), input.GetGrid().GetSpacing_S(), input.GetGrid().GetSpacing_R(), input.GetGrid().GetSpacing_C());
//
//    //Get the indexes of functions involved in combined function from profile vector
//    mxArray< int > index_array;
//    if(!this->GetIndexArrayForProfileFunction(input, profile_function, index_array))
//    {
//        std::cout<<std::endl<<"mxProfileProcessing::ProfileMeasure() error: mxProfileProcessing::GetIndexArrayForProfileFunction() returned 0.";
//        return 0;
//    }
//    //cout<<"index_array: "<<index_array<<endl;
//
//    for(int i=0; i<index_array.GetNumberOfElements(); i++)
//    {
//        std::cout<<" "<<index_array[i];
//    }
//
//
//    for(int s=0; s<input.GetGridDimensions_S(); s++)
//    {
//        for(int r=0; r<input.GetGridDimensions_R(); r++)
//        {
//            for(int c=0; c<input.GetGridDimensions_C(); c++)
//            {
//                if(input.IsVectorCreated(s,r,c))//If the vector exists
//                {
//                    int is_condition_fulfilled = 1;
//                    int p_r;
//                    //for(p_r = start_r_or_n_value; p_r<input.vector_size_r && is_condition_fulfilled; p_r++)
//                    for(p_r = start_r_or_n_value; p_r<input.GetVectorDimensions_R() && is_condition_fulfilled; p_r++)
//                    {
//                        //We sum according to indexes in index_array
//                        double sum = 0;
//                        //cout<<input.matrix3d[s][r][c]->vector[0][p_r][(index_array[0])]<<",";
//                        for(int i=0; i<index_array.GetNumberOfElements(); i++)
//                        {
//                            sum = sum + input.GetVector(s,r,c)->operator()(0, p_r, index_array[i]);// matrix3d[s][r][c]->vector[0][p_r][(index_array[i])];
//                        }
//
//                        if(GP_operator==GREATER_OR_EQUAL)
//                        {
//                            ////if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) >= (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                            //if(!( (input.matrix3d[s][r][c]->original_pixel_value) >= (unsigned short)(sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                            if(!( (input.GetVector(s,r,c)->m_original_voxel_value) >= (unsigned short)(sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
//                        }
//                        else
//                        {
//                            if(GP_operator==SMALLER_OR_EQUAL)
//                            {
//                                //if(!( (input.GetVector(s,r,c)->m_original_voxel_value) <= (unsigned short)(sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
//                                if(!( ((double)(input.GetVector(s,r,c)->m_original_voxel_value)) <= (sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
//                            }
//                            else
//                            {
//                                if(GP_operator==GREATER)
//                                {
//                                    //if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) > (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                                    if(!( ((double)(input.GetVector(s,r,c)->m_original_voxel_value)) > (sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
//                                }
//                                else
//                                {
//                                    if(GP_operator==SMALLER)
//                                    {
//                                        //if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) < (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                                        if(!( ((double)(input.GetVector(s,r,c)->m_original_voxel_value)) < (sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
//                                    }
//                                    else
//                                    {
//                                        std::cout<<"mxProfileProcessing::ProfileMeasure(): Invalid comparison operator entered!"<<std::endl;
//                                        return 0;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                    output(s,r,c) = (unsigned short) p_r;
//                }
//                else output(s,r,c) = 0;//If the vector does not exist, assign to pixel value 0.
//            }
//        }
//    }
//
//    return 1;
//}


int mxProfileProcessing::ProfileMeasure(mxProfile &input, mxImage &output, unsigned int profile_function, mxComparisonOperator comparison_operator, int start_r_or_n_value)
{
    if(profile_function==0) return 0;

    //Get the indexes of operators involved in combined function from profile vector
    mxArray< int > index_array;
    if(!this->GetIndexArrayForProfileFunction(input, profile_function, index_array))
    {
        std::cout<<std::endl<<"mxProfileProcessing::ProfileMeasure() error: mxProfileProcessing::GetIndexArrayForProfileFunction() returned 0.";
        return 0;
    }
    //cout<<"index_array: "<<index_array<<endl;

//    for(int i=0; i<index_array.GetNumberOfElements(); i++)
//    {
//        std::cout<<" "<<index_array[i];
//    }

    return this->ProfileMeasure(input, output, index_array, comparison_operator, start_r_or_n_value);
}



int mxProfileProcessing::ProfileMeasure(mxProfile &input, mxImage &output, mxArray<mxString> &array_of_operators, mxComparisonOperator comparison_operator, int start_r_or_n_value)
{
    if(array_of_operators.IsEmpty()) return 0;
    
    //Get the indexes of operators involved in combined function from profile vector
    mxArray< int > index_array;
    if(!this->GetIndexArrayForOperatorStringArray(input, array_of_operators, index_array))
    {
        std::cout<<std::endl<<"mxProfileProcessing::ProfileMeasure() error: mxProfileProcessing::GetIndexArrayForProfileFunction() returned 0.";
        return 0;
    }

    return this->ProfileMeasure(input, output, index_array, comparison_operator, start_r_or_n_value);
}



int mxProfileProcessing::ProfileMeasure(mxProfile &input, mxImage &output, mxArray<int> &index_array, mxComparisonOperator GP_operator, int start_r_or_n_value)
{
    if(input.IsEmpty()) return 0;
    //if(profile_function==0) return 0;
    
    if(start_r_or_n_value<1) start_r_or_n_value = 1;
    
    output.SetDimensions(1,input.GetGridDimensions_S(), input.GetGridDimensions_R(), input.GetGridDimensions_C());
    output.SetOrigin(input.GetGrid().GetOrigin_T(), input.GetGrid().GetOrigin_S(), input.GetGrid().GetOrigin_R(),input.GetGrid().GetOrigin_C());
    output.SetSpacing(input.GetGrid().GetSpacing_T(), input.GetGrid().GetSpacing_S(), input.GetGrid().GetSpacing_R(), input.GetGrid().GetSpacing_C());
    
    //Get the indexes of functions involved in combined function from profile vector
//    mxArray< int > index_array;
//    if(!this->GetIndexArrayForProfileFunction(input, profile_function, index_array))
//    {
//        std::cout<<std::endl<<"mxProfileProcessing::ProfileMeasure() error: mxProfileProcessing::GetIndexArrayForProfileFunction() returned 0.";
//        return 0;
//    }
    //cout<<"index_array: "<<index_array<<endl;
    
    for(int i=0; i<index_array.GetNumberOfElements(); i++)
    {
        std::cout<<" "<<index_array[i];
    }
    
    
    for(int s=0; s<input.GetGridDimensions_S(); s++)
    {
        for(int r=0; r<input.GetGridDimensions_R(); r++)
        {
            for(int c=0; c<input.GetGridDimensions_C(); c++)
            {
                if(input.IsVectorCreated(s,r,c))//If the vector exists
                {
                    int is_condition_fulfilled = 1;
                    int p_r;
                    //for(p_r = start_r_or_n_value; p_r<input.vector_size_r && is_condition_fulfilled; p_r++)
                    for(p_r = start_r_or_n_value; p_r<input.GetVectorDimensions_R() && is_condition_fulfilled; p_r++)
                    {
                        //We sum according to indexes in index_array
                        double sum = 0;
                        //cout<<input.matrix3d[s][r][c]->vector[0][p_r][(index_array[0])]<<",";
                        for(int i=0; i<index_array.GetNumberOfElements(); i++)
                        {
                            sum = sum + input.GetVector(s,r,c)->operator()(0, p_r, index_array[i]);// matrix3d[s][r][c]->vector[0][p_r][(index_array[i])];
                        }
                        
                        if(GP_operator==GREATER_OR_EQUAL)
                        {
                            ////if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) >= (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
                            //if(!( (input.matrix3d[s][r][c]->original_pixel_value) >= (unsigned short)(sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
                            if(!( (input.GetVector(s,r,c)->m_original_voxel_value) >= (unsigned short)(sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
                        }
                        else
                        {
                            if(GP_operator==SMALLER_OR_EQUAL)
                            {
                                //if(!( (input.GetVector(s,r,c)->m_original_voxel_value) <= (unsigned short)(sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
                                if(!( ((double)(input.GetVector(s,r,c)->m_original_voxel_value)) <= (sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
                            }
                            else
                            {
                                if(GP_operator==GREATER)
                                {
                                    //if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) > (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
                                    if(!( ((double)(input.GetVector(s,r,c)->m_original_voxel_value)) > (sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
                                }
                                else
                                {
                                    if(GP_operator==SMALLER)
                                    {
                                        //if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) < (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
                                        if(!( ((double)(input.GetVector(s,r,c)->m_original_voxel_value)) < (sum/((double)index_array.GetNumberOfElements())) )) is_condition_fulfilled = 0;
                                    }
                                    else
                                    {
                                        std::cout<<"mxProfileProcessing::ProfileMeasure(): Invalid comparison operator entered!"<<std::endl;
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    output(s,r,c) = (unsigned short) p_r;
                }
                else output(s,r,c) = 0;//If the vector does not exist, assign to pixel value 0.
            }
        }
    }
    
    return 1;
}




//int mxProfileProcessing::ProfileMeasure_ProfileFunctionBySmallestDeviation(Profile &input, mxImage &output, GPComparisonOperator GP_operator, int start_r_or_n_value)
//{
//    if(input.IsEmpty()) return 0;
//
//    if(start_r_or_n_value<1) start_r_or_n_value = 1;
//
//    output.Set(input.NumberOfSlices(), input.NumberOfRows(), input.NumberOfColumns());
//    output.SetOrigin(input.Get_S_Origin(), input.Get_R_Origin(), input.Get_C_Origin());
//    output.SetSpacing(input.Get_S_Spacing(), input.Get_R_Spacing(), input.Get_C_Spacing());
//    output.FillInWith(0);
//
//    for(int s=0; s<input.NumberOfSlices(); s++)
//    {
//        double average;
//        this->_AverageValueInSlice(input,s,average);
//        //cout<<"average= "<<average<<" ";
//
//        Matrix<double> basic_average_profile;
//        this->_BasicAverageProfileInSlice(input,s,input.GetVectorSize_R(),basic_average_profile);
//        //this->_BasicAverageProfileInSlice(input,s,input.GetVectorSize_C(),basic_average_profile);//IZMENA!!!!!!!!
//        //cout<<"basic_average_profile.NumberOfRows()= "<<basic_average_profile.NumberOfRows()<<" ";
//        //cout<<"basic_average_profile.NumberOfColumns()= "<<basic_average_profile.NumberOfColumns()<<" ";
//
//        for(int i=0; i<basic_average_profile.NumberOfRows(); i++)
//        {
//            for(int j=0; j<basic_average_profile.NumberOfColumns(); j++)
//            {
//                cout<<basic_average_profile[i][j]<<" ";
//            }
//            cout<<endl;
//        }
//
//        Array<double> deviation_array;
//        this->_AbsoluteDeviationArrayFromBasicAverageProfile(basic_average_profile,average,deviation_array);
//
//        //for(int i=0; i<deviation_array.NumberOfElements(); i++)
//        //{
//        //    cout<<deviation_array[i]<<" ";
//        //}
//
//        ////Find the smallest abs deviation which is not -1, starting from index 1.
//        //double min = -1;//initialize with -1, means no value.
//        //unsigned int function;
//        //for(int i=1; i<deviation_array.NumberOfElements(); i++)
//        //{
//        //    if(deviation_array[i]>=0)
//        //    {
//        //        if(min==-1 || deviation_array[i]<min)
//        //        {
//        //            function = i;
//        //            min = deviation_array[i];
//        //        }
//        //    }
//        //}
//        //if(min<0) return 0;//If error, return 0.
//
//        //Find up to 10 smallest abs deviatios which are not -1, starting from index 1.
//        Array<int> index_change_order;
//        deviation_array.SortAscending(index_change_order);
//        //bdFunctions bdf;
//        Array<int> functions_array;
//        functions_array.Set(10);
//        int j=0;
//        //cout<<"deviation_array.NumberOfElements()= "<<deviation_array.NumberOfElements()<<" ";
//        for(int i=0; i<deviation_array.NumberOfElements() && j<10; i++)
//        {
//            //cout<<deviation_array[i]<<" ";
//            if(deviation_array[i]>=0)
//            {
//                functions_array[j] = index_change_order[i];
//                j++;
//            }
//        }
//        cout<<"mxProfileProcessing::ProfileMeasure_ProfileFunctionBySmallestDeviation(): Number of candidate functions [max 10]: "<<j<<endl;
//
//        //The slice of the profile might be empty (contains no values) in which case j==0. If so, we skip adding values
//        // to this slice (it is left with zero values - empty slice).
//        if(j!=0)
//        {
//
//            int max_number_of_basic_functions = 0;
//            int index_of_max = 0;
//            for(int i=0; i<j; i++)
//            {
//                int n = bdStandard::NumberOfNonZeroBits(functions_array[i]);
//                if(n > max_number_of_basic_functions)
//                {
//                    max_number_of_basic_functions = n;
//                    index_of_max = i;
//                }
//            }
//            cout<<"mxProfileProcessing::ProfileMeasure_ProfileFunctionBySmallestDeviation(): Number of basic functions for selected function: "<<max_number_of_basic_functions<<endl;
//
//            unsigned int function = (unsigned int) (functions_array[index_of_max]);
//
//            cout<<"mxProfileProcessing::ProfileMeasure_ProfileFunctionBySmallestDeviation(): selected function: "<<function<<endl;
//
//
//            //Get the indexes of functions involved in combined function from profile vector
//            Array<int> index_array;
//            this->GetIndexArrayForProfileFunction(input, function, index_array);
//
//            for(int r=0; r<input.NumberOfRows(); r++)
//            {
//                for(int c=0; c<input.NumberOfColumns(); c++)
//                {
//                    if(input.IsVectorCreated(s,r,c))//If the vector exists
//                    {
//                        int is_condition_fulfilled = 1;
//                        int p_r;
//                        for(p_r = start_r_or_n_value; p_r<input.vector_size_r && is_condition_fulfilled; p_r++)
//                        {
//                            //We sum according to indexes in index_array
//                            double sum = 0;
//                            //cout<<input.matrix3d[s][r][c]->vector[0][p_r][(index_array[0])]<<",";
//                            for(int i=0; i<index_array.NumberOfElements(); i++)
//                            {
//                                sum = sum + input.matrix3d[s][r][c]->vector[0][p_r][(index_array[i])];
//                            }
//
//                            if(GP_operator==GREATER_OR_EQUAL)
//                            {
//                                //if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) >= (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                                if(!( (input.matrix3d[s][r][c]->original_pixel_value) >= (unsigned short)(sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                            }
//                            else
//                            {
//                                if(GP_operator==SMALLER_OR_EQUAL)
//                                {
//                                    if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) <= (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                                }
//                                else
//                                {
//                                    if(GP_operator==GREATER)
//                                    {
//                                        if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) > (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                                    }
//                                    else
//                                    {
//                                        if(GP_operator==SMALLER)
//                                        {
//                                            if(!( ((double)(input.matrix3d[s][r][c]->original_pixel_value)) < (sum/((double)index_array.NumberOfElements())) )) is_condition_fulfilled = 0;
//                                        }
//                                        else
//                                        {
//                                            cout<<"mxProfileProcessing::ProfileMeasure_ProfileFunctionBySmallestDeviation(): Invalid GPComparisonOperator entered!"<<endl;
//                                            return 0;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        output[s][r][c] = (unsigned short) p_r;
//                    }
//                    else output[s][r][c] = 0;//If the vector does not exist, assign to pixel value 0.
//                }
//            }
//        }
//    }
//
//    return 1;
//}
//
//
//
//int mxProfileProcessing::_AverageValueInSlice(Profile &input, int slice_index, double &output_average)
//{
//    if(input.IsEmpty()) return 0;
//    if(slice_index<0 || slice_index>=input.NumberOfSlices()) return 0;
//
//    int sum = 0;
//    int n_of_positions = 0;
//    for(int r=0; r<input.NumberOfRows(); r++)
//    {
//        for(int c=0; c<input.NumberOfColumns(); c++)
//        {
//            if(input.IsVectorCreated(slice_index,r,c))
//            {
//                sum += input[slice_index][r][c]->original_pixel_value;
//                n_of_positions++;
//            }
//        }
//    }
//
//    output_average = ((double)sum)/((double)n_of_positions);
//
//    return 1;
//}
//
//
//
//int mxProfileProcessing::_BasicAverageProfileInSlice(Profile &input, int slice_index, int r_or_n_max_size, Matrix<double> &basic_average_profile)
//{
//    if(input.IsEmpty()) return 0;
//    if(slice_index<0 || slice_index>=input.NumberOfSlices()) return 0;
//    //if(average_value_in_slice<0) return 0;
//    if(r_or_n_max_size<=0) return 0;
//
//    if(r_or_n_max_size>input.GetVectorSize_R()) basic_average_profile.Set(9,input.GetVectorSize_R());
//    else basic_average_profile.Set(9,r_or_n_max_size);
//    basic_average_profile.FillInWith(-1);
//
//    cout<<"basic_average_profile.NumberOfColumns()="<<basic_average_profile.NumberOfColumns()<<endl;
//    cout<<"basic_average_profile.NumberOfRows()="<<basic_average_profile.NumberOfRows()<<endl;
//
//
//    //For each of the basic functions...
//    for(int f = 0; f<9; f++)
//    {
//        //cout<<"f="<<f<<" ";
//
//        //If the basic function is in the profile...
//        //... for each of the 'r' or 'n' parameter values...
//        for(int p=0; p<basic_average_profile.NumberOfColumns(); p++)
//            //for(int p=0; p<basic_average_profile.NumberOfRows(); p++)//IZMENA!!!!
//        {
//            //... calculate function average over the given slice...
//            double n_of_elements = 0.0;
//            double sum = 0.0;
//            int is_vector_existing = 0;
//            for(int r=0; r<input.NumberOfRows(); r++)
//            {
//                for(int c=0; c<input.NumberOfColumns(); c++)
//                {
//                    if(input.IsVectorCreated(slice_index,r,c))
//                    {
//                        is_vector_existing = 1;
//                        sum += (double) (input[slice_index][r][c]->vector[0][p][f]);//index_array has only 1 element if basic function is calculated
//                        //cout<<(input[slice_index][r][c]->vector[0][p][f])<<" ";
//                        //sum += input[slice_index][r][c]->vector[0][(index_array[0])][p];//index_array has only 1 element if basic function is calculated
//                        n_of_elements += 1;
//                    }
//                }
//            }
//            double v = sum/n_of_elements;
//            //!!!!!!!!!!IT HAPPENS THAT 'v' IS NAN. CHECK WHY!!!!!!!!!!
//            if(isnan(v)) v=-1;
//            //cout<<"is_vector_existing="<<is_vector_existing<<" ";
//            //basic_average_profile[f][p] = sum/n_of_elements;
//            basic_average_profile[f][p] = v;
//            //cout<<"("<<sum<<","<<n_of_elements<<","<<basic_average_profile[f][p]<<")";
//
//        }
//    }
//
//    return 1;
//}
//
//
//
////int mxProfileProcessing::_BasicAverageProfileInSlice(Profile &input, int slice_index, int r_or_n_max_size, Matrix<double> &basic_average_profile)
////{
////    if(input.IsEmpty()) return 0;
////    if(slice_index<0 || slice_index>=input.NumberOfSlices()) return 0;
////    //if(average_value_in_slice<0) return 0;
////    if(r_or_n_max_size<=0) return 0;
////
////    if(r_or_n_max_size>input.GetVectorSize_R()) basic_average_profile.Set(9,input.GetVectorSize_R());
////    else basic_average_profile.Set(9,r_or_n_max_size);
////
////    cout<<"basic_average_profile.NumberOfColumns()="<<basic_average_profile.NumberOfColumns()<<endl;
////    cout<<"basic_average_profile.NumberOfRows()="<<basic_average_profile.NumberOfRows()<<endl;
////
////
////    //For each of the basic functions...
////    unsigned int function;
////    int f = 0;
////    for(function = 1; function<512 && f<9; function = function*2)
////    {
////        cout<<"f="<<f<<" ";
////        //Since we will be calculating only for BASIC functions, the index_array will always have only 1 element!
////        Array<int> index_array;
////        if(!this->GetIndexArrayForProfileFunction(input,function,index_array))
////        {
////            cout<<"(!) ";
////            //If the basic function is not in the profile, just enter -1 to each of its row values...
////            for(int j=0; j<basic_average_profile.NumberOfColumns(); j++) basic_average_profile[f][j] = -1;
////        }
////        else
////        {
////            cout<<"index_array= "<<index_array<<endl;
////
////            //If the basic function is in the profile...
////            //... for each of the 'r' or 'n' parameter values...
////            for(int p=0; p<basic_average_profile.NumberOfColumns(); p++)
////            //for(int p=0; p<basic_average_profile.NumberOfRows(); p++)//IZMENA!!!!
////            {
////                //... calculate function average over the given slice...
////                int n_of_elements = 0;
////                double sum = 0;
////                for(int r=0; r<input.NumberOfRows(); r++)
////                {
////                    for(int c=0; c<input.NumberOfColumns(); c++)
////                    {
////                        if(input.IsVectorCreated(slice_index,r,c))
////                        {
////                            sum += input[slice_index][r][c]->vector[0][p][(index_array[0])];//index_array has only 1 element if basic function is calculated
////                            //sum += input[slice_index][r][c]->vector[0][(index_array[0])][p];//index_array has only 1 element if basic function is calculated
////                            n_of_elements++;
////                        }
////                    }
////                }
////                basic_average_profile[f][p] = sum / ((double) n_of_elements);
////            }
////        }
////        f++;
////    }
////
////    return 1;
////}
//
//
//
//int mxProfileProcessing::_AbsoluteDeviationForFunctionFromBasicAverageProfile(unsigned int profile_function, Matrix<double> &basic_average_profile, double average_value_in_slice, double &abs_deviation)
//{
//    if(basic_average_profile.NumberOfRows()!=9) return 0;
//    if(basic_average_profile.NumberOfColumns()<1) return 0;
//
//    //Calculate the number of basic functions involved and check if the combined function requested can be achieved (exists in basic_average_profile)
//    int number_of_functions = 0;
//    if((profile_function & 1) == 1)
//    {
//        if(basic_average_profile[0][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 2) == 2)
//    {
//        if(basic_average_profile[1][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 4) == 4)
//    {
//        if(basic_average_profile[2][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 8) == 8)
//    {
//        if(basic_average_profile[3][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 16) == 16)
//    {
//        if(basic_average_profile[4][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 32) == 32)
//    {
//        if(basic_average_profile[5][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 64) == 64)
//    {
//        if(basic_average_profile[6][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 128) == 128)
//    {
//        if(basic_average_profile[7][0]<0) return 0;
//        number_of_functions++;
//    }
//    if((profile_function & 256) == 256)
//    {
//        if(basic_average_profile[8][0]<0) return 0;
//        number_of_functions++;
//    }
//
//    abs_deviation = 0;
//    double combined_profile_function_value = 0;
//    for(int i=0; i<basic_average_profile.NumberOfColumns(); i++)
//    {
//        if((profile_function & 1) == 1) combined_profile_function_value += basic_average_profile[0][i];
//        if((profile_function & 2) == 2) combined_profile_function_value += basic_average_profile[1][i];
//        if((profile_function & 4) == 4) combined_profile_function_value += basic_average_profile[2][i];
//        if((profile_function & 8) == 8) combined_profile_function_value += basic_average_profile[3][i];
//        if((profile_function & 16) == 16) combined_profile_function_value += basic_average_profile[4][i];
//        if((profile_function & 32) == 32) combined_profile_function_value += basic_average_profile[5][i];
//        if((profile_function & 64) == 64) combined_profile_function_value += basic_average_profile[6][i];
//        if((profile_function & 128) == 128) combined_profile_function_value += basic_average_profile[7][i];
//        if((profile_function & 256) == 256) combined_profile_function_value += basic_average_profile[8][i];
//
//        combined_profile_function_value = combined_profile_function_value / ((double) number_of_functions);
//
//        double difference = combined_profile_function_value - average_value_in_slice;
//        double abs_difference;
//        if(difference<0) abs_difference = -difference;
//        else abs_difference = difference;
//
//        abs_deviation = abs_deviation + difference;
//    }
//
//    abs_deviation = abs_deviation / ((double) (basic_average_profile.NumberOfColumns()));
//
//    return 1;
//}
//
//
//
//int mxProfileProcessing::_AbsoluteDeviationArrayFromBasicAverageProfile(Matrix<double> &basic_average_profile, double average_value_in_slice, Array<double> &output_deviation_array)
//{
//    if(basic_average_profile.NumberOfRows()!=9) return 0;
//    if(basic_average_profile.NumberOfColumns()<1) return 0;
//
//    output_deviation_array.Set(512);
//    output_deviation_array[0] = -1;
//
//    double deviation;
//    for(int i=1; i<output_deviation_array.NumberOfElements(); i++)
//    {
//        if(!this->_AbsoluteDeviationForFunctionFromBasicAverageProfile(i,basic_average_profile,average_value_in_slice,deviation)) output_deviation_array[i] = -1;
//        else output_deviation_array[i] = deviation;
//    }
//
//    return 1;
//}
//
//
//
//
//

















int mxProfileProcessing::Threshold(mxProfile &profile, mxImage &output_image, mxMatrix<int> &index_matrix, const char *comparison)
{
    if(profile.IsEmpty()) return 0;
    if(index_matrix.GetNumberOfColumns()!=3) return 0;
    if(index_matrix.IsEmpty()) return 0;
    
    output_image.Set(profile.GetGridDimensions_S(), profile.GetGridDimensions_R(), profile.GetGridDimensions_C());
    output_image.SetOrigin(profile.GetGrid().GetOrigin_T(),profile.GetGrid().GetOrigin_S(), profile.GetGrid().GetOrigin_R(), profile.GetGrid().GetOrigin_C());
    output_image.SetSpacing(profile.GetGrid().GetSpacing_T(), profile.GetGrid().GetSpacing_S(), profile.GetGrid().GetSpacing_R(), profile.GetGrid().GetSpacing_C());
    
    
    mxString s_comparison;
    s_comparison.Assign(comparison);
    
    if(s_comparison==">")
    {
        //Now that we have the index array, we can calculate the function and threshold the original values based on it
        for(int s=0; s<profile.GetGrid().GetNumberOfSlices(); s++)
        {
            for(int r=0; r<profile.GetGrid().GetNumberOfRows(); r++)
            {
                for(int c=0; c<profile.GetGrid().GetNumberOfColumns(); c++)
                {
                    if(profile.IsVectorCreated(s,r,c))
                    {
                        double sum  = 0;
                        for(int i=0; i<index_matrix.GetNumberOfRows(); i++)
                        {
                            sum += profile.GetVector(s,r,c)->operator()( (index_matrix(i,0)), (index_matrix(i,1)), (index_matrix(i,2)) );
                        }
                        
                        if( ((double)(profile.GetVector(s,r,c)->m_original_voxel_value)) > (sum/((double)index_matrix.GetNumberOfRows())) )
                            output_image(s,r,c) = 255;
                        else output_image(s,r,c) = 0;
                    }
                    else output_image(s,r,c) = 0;
                }
            }
        }
        return 1;
    }
    
    if(s_comparison=="<")
    {
        //Now that we have the index array, we can calculate the function and threshold the original values based on it
        for(int s=0; s<profile.GetGrid().GetNumberOfSlices(); s++)
        {
            for(int r=0; r<profile.GetGrid().GetNumberOfRows(); r++)
            {
                for(int c=0; c<profile.GetGrid().GetNumberOfColumns(); c++)
                {
                    if(profile.IsVectorCreated(s,r,c))
                    {
                        double sum  = 0;
                        for(int i=0; i<index_matrix.GetNumberOfRows(); i++)
                        {
                            sum += profile.GetVector(s,r,c)->operator()( (index_matrix(i,0)), (index_matrix(i,1)), (index_matrix(i,2)) );
                        }
                        
                        if( ((double)(profile.GetVector(s,r,c)->m_original_voxel_value)) < (sum/((double)index_matrix.GetNumberOfRows())) )
                            output_image(s,r,c) = 255;
                        else output_image(s,r,c) = 0;
                    }
                    else output_image(s,r,c) = 0;
                }
            }
        }
        return 1;
    }
    
    if(s_comparison=="<")
    {
        //Now that we have the index array, we can calculate the function and threshold the original values based on it
        for(int s=0; s<profile.GetGrid().GetNumberOfSlices(); s++)
        {
            for(int r=0; r<profile.GetGrid().GetNumberOfRows(); r++)
            {
                for(int c=0; c<profile.GetGrid().GetNumberOfColumns(); c++)
                {
                    if(profile.IsVectorCreated(s,r,c))
                    {
                        double sum  = 0;
                        for(int i=0; i<index_matrix.GetNumberOfRows(); i++)
                        {
                            sum += profile.GetVector(s,r,c)->operator()( (index_matrix(i,0)), (index_matrix(i,1)), (index_matrix(i,2)) );
                        }
                        
                        if( ((double)(profile.GetVector(s,r,c)->m_original_voxel_value)) <= (sum/((double)index_matrix.GetNumberOfRows())) )
                            output_image(s,r,c) = 255;
                        else output_image(s,r,c) = 0;
                    }
                    else output_image(s,r,c) = 0;
                }
            }
        }
        return 1;
    }
    
    if(s_comparison==">=")
    {
        //Now that we have the index array, we can calculate the function and threshold the original values based on it
        for(int s=0; s<profile.GetGrid().GetNumberOfSlices(); s++)
        {
            for(int r=0; r<profile.GetGrid().GetNumberOfRows(); r++)
            {
                for(int c=0; c<profile.GetGrid().GetNumberOfColumns(); c++)
                {
                    if(profile.IsVectorCreated(s,r,c))
                    {
                        double sum  = 0;
                        for(int i=0; i<index_matrix.GetNumberOfRows(); i++)
                        {
                            sum += profile.GetVector(s,r,c)->operator()( (index_matrix(i,0)), (index_matrix(i,1)), (index_matrix(i,2)) );
                        }
                        
                        if( ((double)(profile.GetVector(s,r,c)->m_original_voxel_value)) >= (sum/((double)index_matrix.GetNumberOfRows())) )
                            output_image(s,r,c) = 255;
                        else output_image(s,r,c) = 0;
                    }
                    else output_image(s,r,c) = 0;
                }
            }
        }
        return 1;
    }
    
    //It should not come to here- if it did, the comparison string was not recognized!
    return 0;
    
}


