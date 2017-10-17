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


int mxBIPX::OpenWithSphereSE(mxBasicImage &input, mxBasicImage &output, unsigned int squared_radius_of_SE, unsigned int t)
{
    mxDataObjectFactory *f = input.GetFactory();
    mxDataObject *obj = f->Create();
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



