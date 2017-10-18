/*=========================================================================
 
 Program:   mipx
 Module:    mxGIPX.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxGIPX.h"



mxGIPX::mxGIPX()
{
}


mxGIPX::~mxGIPX()
{
}


int mxGIPX::Histogram(mxBasicImage &input, mxArray<int> &output)
{
    if(input.IsEmpty()) return 0;
    
    mxImageScalar minimum_value, maximum_value;
    input.GetVoxelValueFullRange(minimum_value, maximum_value);
    
    mxImageScalar value_full_range = maximum_value - minimum_value + 1;
    output.SetNumberOfElements(value_full_range);
    output.FillInWith(0);
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        output[input.Get(i)-minimum_value] += 1;
    }
    
    return 1;
}


int mxGIPX::InvertValues(mxBasicImage &input, mxBasicImage &output)
{
    if(input.IsEmpty()) return 0;
    output.SetDimensionsAndPropertiesAs(&input);
    
    mxImageScalar minimum_value, maximum_value;
    input.GetVoxelMinAndMaxValues(minimum_value, maximum_value);
    mxImageScalar value_range = maximum_value - minimum_value;
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        double coeff = 1.0 - ((double)(input.Get(i)-minimum_value)) / ((double)(value_range));
        output.Set(i, ((mxImageScalar)(coeff * ((double)maximum_value))) );
    }
    
    return 1;
}


int mxGIPX::Mask(mxBasicImage &input, mxBasicImage &mask, mxBasicImage &output)
{
    if(input.IsEmpty()) return 0;
    if(!input.IsEqualSizeAs(mask)) return 0;
    
    output.SetDimensionsAndPropertiesAs(&input);
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        if(mask.Get(i)) output.Set(i,input.Get(i));
        else output.Set(i,0);
    }
    
    return 1;
}


int mxGIPX::Threshold(mxBasicImage &input, mxImageScalar threshold, mxBasicImage &output)
{
    if(input.IsEmpty()) return 0;
    output.SetDimensionsAndPropertiesAs(&input);
        
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        if(input.Get(i) >= threshold)
        {
            output.Set(i, input.Get(i));
        }
        else
        {
            output.Set(i, 0);
        }
    }
    return 1;
}


int mxGIPX::ThresholdInRange(mxBasicImage &input, mxImageScalar threshold1, mxImageScalar threshold2, mxBasicImage &output)
{
    if(input.IsEmpty()) return 0;
    output.SetDimensionsAndPropertiesAs(&input);
    
    for(unsigned int i=0; i<input.GetNumberOfDataElements(); i++)
    {
        if(input.Get(i) >= threshold1 && input.Get(i) <= threshold2)
        {
            output.Set(i, input.Get(i));
        }
        else
        {
            output.Set(i, 0);
        }
    }

    
    return 1;
}
