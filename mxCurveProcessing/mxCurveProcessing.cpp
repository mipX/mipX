/*=========================================================================
 
 Program:   mipx
 Module:    mxCurveProcessing.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/




#include "mxCurveProcessing.h"



mxCurveProcessing::mxCurveProcessing()
{
}


mxCurveProcessing::~mxCurveProcessing()
{
}


int mxCurveProcessing::internalAreDoublesEqual_5(double d1, double d2)
{
    if(d1>d2-0.00001 && d1<d2+0.00001) return 1;
    return 0;
}


int mxCurveProcessing::RisingSlopePeaks(mxCurve &input, mxCurve &output)
{
    output.SetNumberOfSamples(input.GetNumberOfSamples());
    output.Get_Y_Values()->FillInWith(0.0);
    output.Get_X_Values()->CopyFrom(*(input.Get_X_Values()));
    
    for(int s=input.GetNumberOfSamples()-1; s>=1; s--)
    {
        double n = 0;
        if(input.Y(s)>input.Y(s-1))
        {
            n += 1;
            for(int s2=s; s2>=1; s2--)
            {
                if(input.Y(s2)>input.Y(s2-1)) n += 1;
                else break;
            }
        }
        output.Y(s) = n;
    }
    
    return 1;
}



int mxCurveProcessing::RisingSlopePeaksWeightedBySlopeHight(mxCurve &input, mxCurve &output)
{
    output.SetNumberOfSamples(input.GetNumberOfSamples());
    output.Get_Y_Values()->FillInWith(0.0);
    output.Get_X_Values()->CopyFrom(*(input.Get_X_Values()));
    
    for(int s=input.GetNumberOfSamples()-1; s>=1; s--)
    {
        double diff_x = 1;
        double diff_y = 1;
        
        if(input.Y(s) > input.Y(s-1))
        {
            diff_x = input.X(s) - input.X(s-1);
            diff_y = input.Y(s) - input.Y(s-1);
            
            int s2 = s;//-1;
            for(s2=s; s2>=1 && (input.Y(s2) > input.Y(s2-1)); s2--) ;
            diff_x = input.X(s) - input.X(s2-1);
            diff_y = input.Y(s) - input.Y(s2-1);
        }
        output.Y(s) = sqrt( diff_x*diff_x + diff_y*diff_y );
    }
    
    return 1;
}
