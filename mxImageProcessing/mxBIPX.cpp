/*=========================================================================
 
 Program:   mipx
 Module:    mxBIPX.cpp
 
 Authors: Danilo Babin.
 Copyright (c) Danilo Babin.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#include "mxBIPX.h"



mxBIPX::mxBIPX()
{
}


mxBIPX::~mxBIPX()
{
}


int mxBIPX::CloseWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    mxDataObjectFactory *f = input.GetFactory();
    mxBasicImage *bi = dynamic_cast<mxBasicImage*> (f->Create());
    if(!bi) return 0;
    if(!this->DilateWithSphereSE(input,*bi,squared_radius_of_SE,t))
    {
        f->Release(bi);
        return 0;
    }
    if(!this->ErodeWithSphereSE(*bi,output,squared_radius_of_SE,t))
    {
        f->Release(bi);
        return 0;
    }
    
    f->Release(bi);
    return 1;
}


int mxBIPX::DilateWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    output.CopyFromDataObject(&input);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                mxImageScalar v = input.Get(t,s,r,c);
                if(v!=0)
                {
                    int sn,rn,cn;
                    for(g.For_6_Neighborhood(s,r,c); g.Get_6_Neighborhood(sn,rn,cn); )
                    {
                        if(input.Get(t,sn,rn,cn)==0)
                        {
                            int snn,rnn,cnn;
                            for(g.ForSphere(s,r,c,0); g.GetSphere(squared_radius_of_SE,snn,rnn,cnn); )
                            {
                                output.Set(t,snn,rnn,cnn,v);
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


int mxBIPX::DistanceTransformForSphere(mxBasicImage &input, mxBasicImage &mask, mxBasicImage &output, unsigned int t)
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
                if(mask.Get(t,s,r,c)!=0)
                {
                    if(input.Get(t,s,r,c)!=0)
                    {
                        int is_radius_found = 0;
                        for(int radius_squared = 1; !is_radius_found && radius_squared<g.GetMaxSphereSquaredRadius()-1; radius_squared++)
                        {
                            int sn,rn,cn;
                            for(g.ForSphere(s,r,c,radius_squared); g.GetSphere(radius_squared+1,sn,rn,cn); )
                            {
                                if(input.Get(t,sn,rn,cn)==0)
                                {
                                    output.Set(t,s,r,c, radius_squared);
                                    is_radius_found = 1;
                                    break;
                                }
                            }
                        }
                        if(!is_radius_found) output.Set(t,s,r,c, g.GetMaxSphereSquaredRadius());
                    }
                }
            }
        }
    }

    return 1;
}


int mxBIPX::ExtractConnectedComponent26(mxBasicImage &input, mxBasicImage &output, unsigned int seed_s, unsigned int seed_r, unsigned int seed_c, mxImageScalar threshold, unsigned int t)
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
            if( (input.Get(t,sn,rn,cn) > threshold) && (output.Get(t,sn,rn,cn) == 0) )
            {
                mxIndex *pi = temp_list.AddNewToEnd();
                pi->SetIndex(t,sn,rn,cn);
                output.Set(t,sn,rn,cn, input.Get(t,sn,rn,cn));
            }
        }
    }
    
    return 1;
}


int mxBIPX::ErodeWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    if(input.IsEmpty()) return 0;
    output.CopyFromDataObject(&input);
    
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    
    for(unsigned int s=0; s<input.GetDimension_S(); s++)
    {
        for(unsigned int r=0; r<input.GetDimension_R(); r++)
        {
            for(unsigned int c=0; c<input.GetDimension_C(); c++)
            {
                mxImageScalar v = input.Get(t,s,r,c);
                if(v!=0)
                {
                    int sn,rn,cn;
                    for(g.For_6_Neighborhood(s,r,c); g.Get_6_Neighborhood(sn,rn,cn); )
                    {
                        if(input.Get(t,sn,rn,cn)==0)
                        {
                            int snn,rnn,cnn;
                            for(g.ForSphere(sn,rn,cn,0); g.GetSphere(squared_radius_of_SE,snn,rnn,cnn); )
                            {
                                output.Set(t,snn,rnn,cnn,0);
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 1;
}


int mxBIPX::NumberOfForegroundVoxelsInNeighborhood8_Iterative(mxBasicImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    int rn,cn;
    int n=0;
    for(geometry.For_8_Neighborhood(r,c); geometry.Get_8_Neighborhood(rn,cn); )
    {
        if(input.Get(t,s,rn,cn)) n++;
    }
    return n;
}


int mxBIPX::NumberOfForegroundVoxelsInNeighborhood26_Iterative(mxBasicImage &input, mxGeometry &geometry, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    int sn,rn,cn;
    int n=0;
    for(geometry.For_26_Neighborhood(s,r,c); geometry.Get_26_Neighborhood(sn,rn,cn); )
    {
        if(input.Get(t,sn,rn,cn)) n++;
    }
    return n;
}


int mxBIPX::NumberOfForegroundVoxelsInNeighborhood8(mxBasicImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(input.IsEmpty()) return 0;
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    return this->NumberOfForegroundVoxelsInNeighborhood8_Iterative(input,g,t,s,r,c);
}


int mxBIPX::NumberOfForegroundVoxelsInNeighborhood26(mxBasicImage &input, unsigned int t, unsigned int s, unsigned int r, unsigned int c)
{
    if(input.IsEmpty()) return 0;
    mxGeometry g;
    g.SetDimensions(input.GetDimension_S(), input.GetDimension_R(), input.GetDimension_C());
    return this->NumberOfForegroundVoxelsInNeighborhood26_Iterative(input,g,t,s,r,c);
}


int mxBIPX::OpenWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    mxDataObjectFactory *f = input.GetFactory();
    mxBasicImage *bi = dynamic_cast<mxBasicImage*> (f->Create());
    if(!bi) return 0;
    if(!this->DilateWithSphereSE(input,*bi,squared_radius_of_SE,t))
    {
        f->Release(bi);
        return 0;
    }
    if(!this->ErodeWithSphereSE(*bi,output,squared_radius_of_SE,t))
    {
        f->Release(bi);
        return 0;
    }
    
    f->Release(bi);
    return 1;
}



