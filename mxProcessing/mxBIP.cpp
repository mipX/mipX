/*=========================================================================
 
 Program:   mipx
 Module:    mxBIP.cpp
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxBIP.h"



mxBIP::mxBIP()
{
}


mxBIP::~mxBIP()
{
}


int mxBIP::CloseWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    mxImage bi;
    if(!this->DilateWithSphereSE(input,bi,squared_radius_of_SE,t))
    {
        return 0;
    }
    if(!this->ErodeWithSphereSE(bi,output,squared_radius_of_SE,t))
    {
        return 0;
    }
    return 1;
}


int mxBIP::DilateWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    output.Copy(&input);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                mxImageScalar v = input(t,s,r,c);
                if(v!=0)
                {
                    int sn,rn,cn;
                    for(g.For_6_Neighborhood(s,r,c); g.Get_6_Neighborhood(sn,rn,cn); )
                    {
                        if(input(t,sn,rn,cn)==0)
                        {
                            int snn,rnn,cnn;
                            for(g.ForSphere(s,r,c,0); g.GetSphere(squared_radius_of_SE,snn,rnn,cnn); )
                            {
                                output(t,snn,rnn,cnn) = v;
                            }
                            
                            break;
                        }
                    }
                }
            }
        }
    }
    
    return 1;
}


int mxBIP::DistanceTransformForSphere(mxImage &input, mxImage &mask, mxImage &output, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    if(!input.IsEqualInDimensions_3D(mask)) return 0;
    
    output.SetDimensionsAndPropertiesAs(&input);
    output.FillInWith(0);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(),input.GetDimension_R(),input.GetDimension_C());
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                if(mask(t,s,r,c)!=0)
                {
                    if(input(t,s,r,c)!=0)
                    {
                        output(t,s,r,c) = g.GetMaxSphereSquaredRadius(); //initialize the output value
                        for(int radius_squared = 1; radius_squared<g.GetMaxSphereSquaredRadius()-1; radius_squared++)
                        {
                            int sn,rn,cn;
                            for(g.ForSphere(s,r,c,radius_squared); g.GetSphere(radius_squared+1,sn,rn,cn); )
                            {
                                if(input(t,sn,rn,cn)==0)
                                {
                                    output(t,s,r,c) = radius_squared;
                                    radius_squared = g.GetMaxSphereSquaredRadius();//this is set to stop the outer 'for' loop.
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}


int mxBIP::ExtractConnectedComponent26(mxImage &input, mxImage &output, unsigned int seed_s, unsigned int seed_r, unsigned int seed_c, mxImageScalar threshold, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    
    output.CopyFromDataObject(&input);
    output.FillInWith(0);
    
    mxList< mxIndex > temp_list;
    mxIndex index;
    index.SetIndex(t,seed_s,seed_r,seed_c);
    temp_list.AddToBegin(index);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(),input.GetDimension_R(),input.GetDimension_C());
    
    for( ; !temp_list.IsEmpty(); temp_list.DeleteBegin())
    {
        index = temp_list.GetBeginElement();
        
        int sn, rn, cn;
        for(g.For_26_Neighborhood(index.S(),index.R(),index.C()); g.Get_26_Neighborhood(sn,rn,cn); )
        {
            if( (input(t,sn,rn,cn) > threshold) && (output(t,sn,rn,cn) == 0) )
            {
                mxIndex *pi = temp_list.AddNewToEnd();
                pi->SetIndex(t,sn,rn,cn);
                output(t,sn,rn,cn) = input(t,sn,rn,cn);
            }
        }
    }
    
    return 1;
}


int mxBIP::ExtractLargestConnectedComponent26(mxImage &input, int threshold, mxImage &output, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    

    // Copy input to output.
    output.Copy(&input);
    
        
    mxIndex p;
    int sn, rn, cn;
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    
    mxList< mxIndex > l, l1, l2, *p_l_previous, *p_l_new, *p_l_temp;
    p_l_previous = &l1;
    p_l_new = &l2;
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                // Find the whole 3D region
                if(output(t,s,r,c)>threshold)
                {
                    p.SetIndex(s,r,c);
                    l.AddToEnd(p);//(s,r,c));
                    p_l_new->AddToEnd(p);
                    output(t,s,r,c) = 0;//We delete the pixel from the image after placing it in the list
                    
                    while(!l.IsEmpty())
                    {
                        p = l.GetEndElement();
                        //for(g.ForCoordinates_26_Neighbors(p.S(),p.R(),p.C()); g.Get_26_Neighbors(sn,rn,cn); )
                        for(g.For_26_Neighborhood(p.S(),p.R(),p.C()); g.Get_26_Neighborhood(sn,rn,cn); )
                        {
                            if(output(t,sn,rn,cn)>threshold)
                            {
                                p.SetIndex(sn,rn,cn);
                                l.AddToEnd(p);//(sn,rn,cn));
                                p_l_new->AddToEnd(p);
                                output(t,sn,rn,cn) = 0;//Delete the pixel from output_image
                            }
                        }
                        l.DeleteEnd();
                    }
                }
                else output(t,s,r,c) = 0;
                //After finding the whole 3D region, check if it is larger than the previous one...
                if(p_l_new->GetNumberOfElements()>p_l_previous->GetNumberOfElements())
                {
                    //...if so, replace the previous region with the new one
                    p_l_previous->Reset();//DeleteAll();
                    p_l_temp = p_l_previous;
                    p_l_previous = p_l_new;
                    p_l_new = p_l_temp;
                }
                else
                {
                    //...if not, just delete the new region
                    p_l_new->Reset();//DeleteAll();
                }
            }
        }
    }
    // After this part the 'p_l_previous' points to the list of the largest region and output_image is empty,
    // so we need to copy the list to the image
    
    while(!p_l_previous->IsEmpty())
    {
        p = p_l_previous->GetEndElement();
        output(t,p.S(),p.R(),p.C()) = input(t,p.S(),p.R(),p.C());//(*this)[p.s][p.r][p.c];
        p_l_previous->DeleteEnd();
    }
    return 1;
}


int mxBIP::ErodeWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    output.Copy(&input);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                mxImageScalar v = input(t,s,r,c);
                if(v!=0)
                {
                    int sn,rn,cn;
                    for(g.For_6_Neighborhood(s,r,c); g.Get_6_Neighborhood(sn,rn,cn); )
                    {
                        if(input(t,sn,rn,cn)==0)
                        {
                            int snn,rnn,cnn;
                            for(g.ForSphere(sn,rn,cn,0); g.GetSphere(squared_radius_of_SE,snn,rnn,cnn); )
                            {
                                output(t,snn,rnn,cnn) = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 1;
}


int mxBIP::FilterWith_4_Neghborhood(mxImage &input, mxImage &output, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    //output.Copy(&input);
    output.SetDimensionsAndPropertiesAs(&input);
    output.FillInWith(0);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                //mxImageScalar v = input(t,s,r,c);
                if(input(t,s,r,c)!=0)
                {
                    int rn,cn;
                    for(g.For_4_Neighborhood(r,c); g.Get_4_Neighborhood(rn,cn); )
                    {
                        if(input(t,s,rn,cn)!=0)
                        {
                            output(t,s,r,c) = input(t,s,rn,cn);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    return 1;
}


int mxBIP::NumberOfForegroundVoxelsInNeighborhood8_Iterative(mxImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    int rn,cn;
    int n=0;
    for(geometry.For_8_Neighborhood(r,c); geometry.Get_8_Neighborhood(rn,cn); )
    {
        if(input(t,s,rn,cn)) n++;
    }
    return n;
}


int mxBIP::NumberOfForegroundVoxelsInNeighborhood26_Iterative(mxImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    int sn,rn,cn;
    int n=0;
    for(geometry.For_26_Neighborhood(s,r,c); geometry.Get_26_Neighborhood(sn,rn,cn); )
    {
        if(input(t,sn,rn,cn)) n++;
    }
    return n;
}


int mxBIP::NumberOfForegroundVoxelsInNeighborhood8(mxImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(input.IsEmpty()) return 0;
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    return this->NumberOfForegroundVoxelsInNeighborhood8_Iterative(input,g,t,s,r,c);
}


int mxBIP::NumberOfForegroundVoxelsInNeighborhood26(mxImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(input.IsEmpty()) return 0;
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    return this->NumberOfForegroundVoxelsInNeighborhood26_Iterative(input,g,t,s,r,c);
}


int mxBIP::OpenWithSphereSE(mxImage &input, mxImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    mxImage bi;
    if(!this->DilateWithSphereSE(input,bi,squared_radius_of_SE,t))
    {
        return 0;
    }
    if(!this->ErodeWithSphereSE(bi,output,squared_radius_of_SE,t))
    {
        return 0;
    }
    return 1;
}


int mxBIP::ProfileVolumeTransformForSphere(mxImage &input, mxImage &mask, mxImage &output, unsigned int t, unsigned int offset_percent)
{
    if(offset_percent<1 || offset_percent>100) offset_percent = 100;
    if(mask.IsEmpty()) return 0;
    if(!input.IsEqualInDimensions_3D(mask)) return 0;
    
    output.SetDimensionsAndPropertiesAs(&input);
    output.FillInWith(0);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(),input.GetDimension_R(),input.GetDimension_C());
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                if(mask(t,s,r,c)!=0)
                {
                    int number_of_foreground_voxels = 0; // output will be the number_of_foreground_voxels
                    int is_volume_calculated = 0;
                    for(int radius_squared = 1; radius_squared<g.GetMaxSphereSquaredRadius()-1 && !is_volume_calculated; radius_squared++)
                    {
                        int sn,rn,cn;
                        for(g.ForSphere(s,r,c,radius_squared); g.GetSphere(radius_squared+1,sn,rn,cn); )
                        {
                            if(input(t,sn,rn,cn)==0)
                            {
                                is_volume_calculated = 1;
                            }
                            else
                            {
                                number_of_foreground_voxels++;
                            }
                        }
                    }
                    
                    output(t,s,r,c) = number_of_foreground_voxels;
                }
            }
        }
    }
    
    return 1;
}




