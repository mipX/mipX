/*=========================================================================
 
 Program:   mipx
 Module:    mxGIP.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxGIP.h"



mxGIP::mxGIP()
{
}


mxGIP::~mxGIP()
{
}


int mxGIP::CompactValues(mxImage &input, mxImage &output)
{
    mxArray<int> histogram;
    if(!this->Histogram(input,histogram)) return 0;
    
    
    mxArray<int> map_values;
    map_values.SetNumberOfElements(histogram.GetNumberOfElements());
    map_values.FillInWith(0);
    
    int distinct_value = 0;
    for(int i=0; i<(int)histogram.GetNumberOfElements(); i++)
    {
        if(histogram[i]>0)
        {
            map_values[i] = distinct_value;
            distinct_value++;
        }
    }
    
    output.SetDimensionsAndPropertiesAs(&input);
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        output[i] = map_values[(input[i])];
    }
    
    return 1;
}


int mxGIP::DifferenceAbsolute(mxImage &input, mxImage &input2, mxImage &output)
{
    if(input.IsEmpty() || input2.IsEmpty()) return 0;
    if(!input.IsEqualInDimensions_4D(input2)) return 0;
    
    output.SetDimensionsAndPropertiesAs(&input);
        
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        int res = input[i] - input2[i];
        if(res<0) res = -res;
        
        output[i] = ((mxImageScalar)(res));
    }
    
    return 1;
}


//int mxGIP::Histogram(mxImage &input, mxArray<int> &output)
//{
//    if(input.IsEmpty()) return 0;
//
//    //mxImageScalar minimum_value, maximum_value;
//    uint64_t minimum_value, maximum_value;
//    input.GetVoxelValueFullRange(minimum_value, maximum_value);
//
//    //std::cout<<"minimum_value, maximum_value"<<minimum_value<<","<<maximum_value<<" ";
//
//    unsigned int value_full_range = ((int)maximum_value) - ((int)minimum_value) + 1;
//
//    output.SetNumberOfElements(value_full_range);
//    output.FillInWith(0);
//
//    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
//    {
//        output[input[i]-minimum_value] += 1;
//    }
//
//    return 1;
//}


int mxGIP::Histogram(mxImage &input, mxArray<int> &output)
{
    if(input.IsEmpty()) return 0;
    
    //mxImageScalar minimum_value, maximum_value;
    uint64_t minimum_value, maximum_value;
    input.GetVoxelMinAndMaxValues(minimum_value, maximum_value);
    
    //std::cout<<"minimum_value, maximum_value"<<minimum_value<<","<<maximum_value<<" ";
    
    //unsigned int value_full_range = ((int)maximum_value) - ((int)minimum_value) + 1;
    
// !!! THIS SEEMS TO BE AN ERROR, BECAUSE IF MINIMUM VAUE IS NOT 0, THEN INDEXING IS WRONG, CHECK!!!!!
    //output.SetNumberOfElements(value_full_range);
    
    output.SetNumberOfElements((unsigned int)(maximum_value+1));
    output.FillInWith(0);
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        //output[input[i]-minimum_value] += 1;
        output[input[i]] += 1;
    }
    
    return 1;
}


int mxGIP::Histogram(mxImage &input, mxCurve &output)
{
    uint64_t minimum_value, maximum_value;
    input.GetVoxelMinAndMaxValues(minimum_value, maximum_value);
    
    unsigned int value_full_range = ((int)maximum_value) - ((int)minimum_value) + 1;
    
    output.SetNumberOfSamples(value_full_range);
    double initial_value = 0;
    output.FillIn_X_ValuesAscending((double)minimum_value,1);
    output.Get_Y_Values()->FillInWith(initial_value);
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        unsigned int local_index = (unsigned int) (input[i]-minimum_value);
        output.Set(local_index, input[i], (output.Y(local_index)+1)) ;
    }
    
    return 1;
}


int mxGIP::Histogram(mxImage &input, unsigned int t, unsigned int s, mxCurve &output)
{
    if(input.IsEmpty())
    {
        output.SetNumberOfSamples(1);
        output.Set(0, 0,0);
        return 1;
    }
    
    uint64_t minimum_value = input(t,s,0,0), maximum_value = input(t,s,0,0);
    for(unsigned int r=0; r<input.GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<input.GetDimension_C(); c++)
        {
            if(input(t,s,r,c)<minimum_value) minimum_value = input(t,s,r,c);
            if(input(t,s,r,c)>maximum_value) maximum_value = input(t,s,r,c);
        }
    }
    
    unsigned int value_full_range = ((int)maximum_value) - ((int)minimum_value) + 1;
    
    output.SetNumberOfSamples(value_full_range);
    double initial_value = 0;
    output.FillIn_X_ValuesAscending((double)minimum_value,1);
    output.Get_Y_Values()->FillInWith(initial_value);
    for(unsigned int r=0; r<input.GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<input.GetDimension_C(); c++)
        {
            unsigned int local_index = (unsigned int) (input(t,s,r,c)-minimum_value);
            output.Set(local_index, input(t,s,r,c), (output.Y(local_index)+1)) ;
        }
    }
    
    return 1;
}


int mxGIP::Histogram(mxImage &input, mxImage &mask, unsigned int t, unsigned int s, mxCurve &output)
{
    if(input.IsEmpty() || mask.IsEmpty())
    {
        output.SetNumberOfSamples(1);
        output.Set(0, 0,0);
        return 1;
    }
    
    uint64_t minimum_value = input(t,s,0,0), maximum_value = input(t,s,0,0);
    for(unsigned int r=0; r<input.GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<input.GetDimension_C(); c++)
        {
            if(mask(t,s,r,c))
            {
                if(input(t,s,r,c)<minimum_value) minimum_value = input(t,s,r,c);
                if(input(t,s,r,c)>maximum_value) maximum_value = input(t,s,r,c);
            }
        }
    }
    
    unsigned int value_full_range = ((int)maximum_value) - ((int)minimum_value) + 1;
    
    output.SetNumberOfSamples(value_full_range);
    double initial_value = 0;
    output.FillIn_X_ValuesAscending((double)minimum_value,1);
    output.Get_Y_Values()->FillInWith(initial_value);
    for(unsigned int r=0; r<input.GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<input.GetDimension_C(); c++)
        {
            if(mask(t,s,r,c))
            {
                unsigned int local_index = (unsigned int) (input(t,s,r,c)-minimum_value);
                output.Set(local_index, input(t,s,r,c), (output.Y(local_index)+1)) ;
            }
        }
    }
    
    return 1;

}


int mxGIP::Histogram(mxImage &input, unsigned int t, unsigned int s, mxArray<int> &output)
{
    if(input.IsEmpty()) return 0;
    
    uint64_t minimum_value = input(t,s,0,0), maximum_value = input(t,s,0,0);
    for(unsigned int r=0; r<input.GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<input.GetDimension_C(); c++)
        {
            if(input(t,s,r,c)<minimum_value) minimum_value = input(t,s,r,c);
            if(input(t,s,r,c)>maximum_value) maximum_value = input(t,s,r,c);
        }
    }
    
    //unsigned int value_full_range = ((int)maximum_value) - ((int)minimum_value) + 1;
    
    output.SetNumberOfElements((unsigned int)(maximum_value+1));
    output.FillInWith(0);
    
    for(unsigned int r=0; r<input.GetDimension_R(); r++)
    {
        for(unsigned int c=0; c<input.GetDimension_C(); c++)
        {
            output[input(t,s,r,c)] += 1;
        }
    }
    
    return 1;
}



int mxGIP::InvertValues(mxImage &input, mxImage &output)
{
    if(input.IsEmpty()) return 0;
    output.SetDimensionsAndPropertiesAs(&input);
    
    uint64_t minimum_value, maximum_value;
    input.GetVoxelMinAndMaxValues(minimum_value, maximum_value);
    mxImageScalar value_range = (mxImageScalar) (maximum_value - minimum_value);
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        double coeff = 1.0 - ((double)(input[i]-minimum_value)) / ((double)(value_range));
        output[i] = ((mxImageScalar)(coeff * ((double)maximum_value))) ;
    }
    
    return 1;
}


int mxGIP::Mask(mxImage &input, mxImage &mask, mxImage &output)
{
    if(input.IsEmpty()) return 0;
    if(!input.IsEqualInDimensions_3D(mask)) return 0;
    
    output.SetDimensionsAndPropertiesAs(&input);
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        if(mask[i]) output[i] = input[i];
        else output[i] = 0;
    }
    
    return 1;
}


int mxGIP::RescaleValueRange(mxImage &input, mxImageScalar min_included, mxImageScalar max_included, mxImage &output)
{
    if(input.IsEmpty()) return 0;
    output.SetDimensionsAndPropertiesAs(&input);
    
    uint64_t minimum_value, maximum_value;
    input.GetVoxelMinAndMaxValues(minimum_value, maximum_value);
    mxImageScalar value_range = (mxImageScalar)(maximum_value - minimum_value);
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        double coeff = ((double)(input[i]-minimum_value)) / ((double)(value_range));
        output[i] = ((mxImageScalar)(coeff * ((double)maximum_value))) ;
    }
    
    return 1;
}


int mxGIP::Threshold(mxImage &input, mxImageScalar threshold, mxImage &output)
{
    if(input.IsEmpty()) return 0;
    output.SetDimensionsAndPropertiesAs(&input);
        
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        if(input[i] >= threshold)
        {
            output[i] = input[i];
        }
        else
        {
            output[i] = 0;
        }
    }
    return 1;
}


int mxGIP::ThresholdInRange(mxImage &input, mxImageScalar threshold1, mxImageScalar threshold2, mxImage &output)
{
    if(input.IsEmpty()) return 0;
    output.SetDimensionsAndPropertiesAs(&input);
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        if(input[i] >= threshold1 && input[i] <= threshold2)
        {
            output[i] = input[i];
        }
        else
        {
            output[i] = 0;
        }
    }

    
    return 1;
}
