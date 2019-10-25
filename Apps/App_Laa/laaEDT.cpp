#include "laaEDT.h"

laaEDT::laaEDT() 
{
};

laaEDT::~laaEDT() 
{
};


int laaEDT::createMaxRadiusImageSITK(mxImage &image, mxImage &out_image) 
{
  mxString temp, command;
  temp.Assign("/tmp/sitk_radius_temp.vtk");
  command.Assign("python /home/levara/versioned/mipx/repo/laaApp/distanceMap.py ");
  command.Append(temp);
  vmxImage tmpimg;
  tmpimg.CopyFromDataObject(&image);
  tmpimg.SaveToVTK16UFile(temp.Get_C_String());
  system(command.Get_C_String());
  tmpimg.Reset();
  tmpimg.LoadVTKFile(temp.Get_C_String());
  out_image.CopyFromDataObject(&tmpimg);
  return 0;
};
