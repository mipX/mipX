/*=========================================================================
 
 Program:   mipx
 Module:    laa.cpp
 
 Authors: Hrvoje Leventic, Danilo Babin
 Copyright (c) Hrvoje Leventic, Danilo Babin
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/


/*
 
 This example shows how to write new functions for image processing.
 It also provides viewing of signals.
 
 */


#include <iostream>
//#include "mxImage.h"
#include "mxString.h"
#include "mxPoint.h"
#include "mxDataObject.h"
#include "mxObject.h"
//#include "mxBasicImage.h"
#include "mxImage.h"

int main()
{
  std::cout << "It werks" << std::endl;
  mxImage img;
  img.SetDimensions(1,10,10,10);
  img.FillInWith(1);
  //for (unsigned int s = 0; s < img.GetDimension_S(); ++s) {
    //for (unsigned int r = 0; r < img.GetDimension_R(); ++r) {
      //for (unsigned int c = 0; c < img.GetDimension_C(); ++c) {
        //img(s,r,c) = 1;
      //}
    //}
  //}

  int sum=0;
  for (unsigned int s = 0; s < img.GetDimension_S(); ++s) {
    for (unsigned int r = 0; r < img.GetDimension_R(); ++r) {
      for (unsigned int c = 0; c < img.GetDimension_C(); ++c) {
        sum += img(s,r,c);
      }
    }
  }

  std::cout << "Sum of the image is: " << sum << std::endl;
  
  mxString test;
  test.Assign("Test mxStringa");
  std::cout << test << std::endl;

  mxPoint p;
  p.S() = 100;
  p.R() = 90;
  p.C() = 80;

  std::cout << "Point: " << p.S() << " " << p.R() << " " << p.C() << std::endl;
    
  mxDataObject obj;
  obj.GetObjectName().Assign("Neki object");

  std::cout << "mxDO: " << obj.GetObjectName() << std::endl;


    
  return 1;
}

