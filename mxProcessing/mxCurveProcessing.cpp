/*=========================================================================
 
 Program:   mipx
 Module:    mxCurveProcessing.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
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


int mxCurveProcessing::FirstSlope_X_Value_ForSingleImpuls(mxCurve &input, double &output_x_value)
{
    std::cout<<" a ";
    if(input.GetNumberOfSamples()<2) return 0;
    
    std::cout<<" b ";
    
    double y_start_value = input.Y(0);
    for(unsigned int i=1; i<input.GetNumberOfSamples(); i++)
    {
        //if(input.Y(i)!=y_start_value)
        if(!this->internalAreDoublesEqual_5(input.Y(i),y_start_value))
        {
            output_x_value = input.X(i-1);
            return 1;
        }
    }
    
    std::cout<<" c ";
    
    return 0;
}


int mxCurveProcessing::PeaksByDominance(mxCurve &input, mxCurve &output)
{
    output.SetNumberOfSamples(input.GetNumberOfSamples());
    output.Get_Y_Values()->FillInWith(0.0);
    output.Get_X_Values()->CopyFrom(*(input.Get_X_Values()));
    
    for(int s=0; s<(int)input.GetNumberOfSamples(); s++)
    {
        double n = 1;
        int s2=s-1;
        int s3=s+1;
        for(s2=s-1, s3=s+1; s2>=0 || s3<(int)input.GetNumberOfSamples(); s2--, s3++, n+=1)
        {
            if(s2>=1)
            {
                if( input.Y(s) < input.Y(s2) )
                {
                    break;
                }
                //else n+=1;
            }
            if(s3<=(int)input.GetNumberOfSamples()-2)
            {
                if( input.Y(s) < input.Y(s3) )
                {
                    break;
                }
                //else n+=1;
            }
        }
        output.Y(s) = n;
        
        
        
        
//        if(s==0)
//        {
//            if(input.Y(s)>input.Y(s+1))
//            {
//                double n = 1;
//                for(int s2=s+1; s<input.GetNumberOfSamples()-1; s2++)
//                {
//                    if(input.Y(s2)>=input.Y(s2-1) && input.Y(s2)>=input.Y(s2+1)) n += 1;
//                    else break;
//                }
//                output.Y(s) = n;
//            }
//        }
//        else if(s==input.GetNumberOfSamples()-1)
//        {
//            if(input.Y(s)>input.Y(s+1))
//            {
//                double n = 1;
//                for(int s2=s+1; s<input.GetNumberOfSamples()-1; s2++)
//                {
//                    if(input.Y(s2)>=input.Y(s2-1) && input.Y(s2)>=input.Y(s2+1)) n += 1;
//                    else break;
//                }
//                output.Y(s) = n;
//            }
//        }
//        else
//        {
//            // if the current sample is a local maximum
//            if(input.Y(s)>=input.Y(s-1) && input.Y(s)>=input.Y(s+1))
//            {
//                double n = 1;
//                int s2=s-1;
//                int s3=s+1;
//                for(s2=s-1, s3=s+1; s2>=0 || s3<input.GetNumberOfSamples(); s2--, s3++)
//                {
//                    //int is_condition_fulfilled = 1;
//                    if(s2>=1)
//                    {
//                        if( !(input.Y(s2)>=input.Y(s2-1)) )
//                        {
//                            break;
//                            //is_condition_fulfilled = 0;
//                        }
//                    }
//                    if(s3<=input.GetNumberOfSamples()-2)
//                    {
//                        if( !(input.Y(s3)>=input.Y(s3+1)) )
//                        {
//                            break;
//                            //is_condition_fulfilled = 0;
//                        }
//                    }
//
//                    //if(is_condition_fulfilled)
//                    //{
//
//                    //}
//                }
//                output.Y(s) = n;
//            }
//        }
    }
    
    return 1;
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
            
            int s2 = s;
            for(s2=s; s2>=1; s2--)
            {
                if(!(input.Y(s2) > input.Y(s2-1))) break;
            }
            diff_x = input.X(s) - input.X(s2);
            diff_y = input.Y(s) - input.Y(s2);
        }
        output.Y(s) = sqrt( diff_x*diff_x + diff_y*diff_y );
    }
    
    return 1;
}


int mxCurveProcessing::SmoothByMeanFilter(mxCurve &input, int number_of_samples_in_half_window, mxCurve &output)
{
    if(number_of_samples_in_half_window<1) return 0;
    if(input.GetNumberOfSamples()<2) return 0;
    
    output.Copy(&input);
    
    for(int s=0; s<(int)input.GetNumberOfSamples(); s++)
    {
        double sum = 0;
        double n_of_samples = 0;
        for(int s1 = s-number_of_samples_in_half_window; s1<=s+number_of_samples_in_half_window; s1++)
        {
            if(s1>=0 && s1<(int)input.GetNumberOfSamples())
            {
                sum += input.Y(s1);
                n_of_samples += 1;
            }
        }
        
        output.Y(s) = sum / n_of_samples;
    }
    
    return 1;
}
