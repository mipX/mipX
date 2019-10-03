#include <vtkGenericDataObjectReader.h>
#include <vtkStructuredPointsReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkProperty.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkImageReslice.h>
#include <vtkCamera.h>

int main(int argc, char *argv[])
{
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    renderWindowInteractor->SetInteractorStyle(style);
    
//    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//    renderWindowInteractor->SetInteractorStyle(style);
    
    // Read all the data from the file
    vtkSmartPointer<vtkGenericDataObjectReader> reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
    reader->SetFileName("/Users/danilobabin/Dropbox/-DIPn/-Teaching/Computer_Graphics/Computer_Graphics_2018_2019/Projects/Project_Cardiovascular_Visualization/Data/vessels_data.vtk");
    reader->Update();
    

    
    vtkSmartPointer<vtkImagePlaneWidget> w = vtkSmartPointer<vtkImagePlaneWidget>::New();
    w->SetInputConnection(reader->GetOutputPort());
    w->SetCurrentRenderer(renderer);
    w->SetInteractor(renderWindowInteractor);
    w->PlaceWidget();
    w->SetPlaneOrientationToYAxes();
    w->SetSliceIndex(0);
    w->DisplayTextOn();
    w->On();
    

    
    
    
    vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
    
    lookupTable->SetNumberOfTableValues(256);
    lookupTable->SetRange(0.0, 255.0);
    lookupTable->SetAlphaRange(0,1); // transparency to opaque
    lookupTable->SetValueRange(1,1); // full brightness
    lookupTable->SetSaturationRange(1,1); // full color saturation
    lookupTable->SetHueRange(0,0.66666);
    lookupTable->SetRampToSQRT();// SetRampToLinear();
    lookupTable->Build();
    
    
    lookupTable->SetNumberOfTableValues(256);
    lookupTable->SetRange(0,255);
//    lookupTable->SetTableValue( 0, 0.0, 0.0, 0.0, 0.0 );
//    lookupTable->SetTableValue( 100, 0.0, 0.0, 0.0, 0.0 );
//    lookupTable->SetTableValue( 101, 0.0, 1.0, 0.0, 1.0 );
//    lookupTable->SetTableValue( 255, 0.0, 1.0, 0.0, 1.0 );
    // m_overlay_lookup_table->SetTableValue( min_max[0], 0, 0, 0, 0 );
    for(unsigned int i=0; i<30; i++)
    {
        lookupTable->SetTableValue( i, 0.0, 0.0, 0.0, 0.0 );
    }
    for(unsigned int i=30; i<=255; i++)
    {
        lookupTable->SetTableValue( i, 0.0, 1.0, 0.0, 0.5 );
    }
    lookupTable->Build();

    
    // Pass the original image and the lookup table to a filter to create
    // a color image:
    vtkSmartPointer<vtkImageMapToColors> scalarValuesToColors = vtkSmartPointer<vtkImageMapToColors>::New();
    scalarValuesToColors->SetLookupTable(lookupTable);
    scalarValuesToColors->PassAlphaToOutputOn();
    scalarValuesToColors->SetInputData(reader->GetOutputDataObject(0));

    
    vtkSmartPointer<vtkImagePlaneWidget> w2 = vtkSmartPointer<vtkImagePlaneWidget>::New();
    //w2->SetInputConnection(scalarValuesToColors->GetOutputPort());// reader->GetOutputPort());
    w2->SetInputConnection(reader->GetOutputPort());
    w2->SetCurrentRenderer(renderer);
    w2->SetInteractor(renderWindowInteractor);
    w2->PlaceWidget();
    w2->SetPlaneOrientationToYAxes();
    w2->SetSliceIndex(0);
    w2->DisplayTextOff();
    //w2->SetColorMap(scalarValuesToColors);
    w2->SetLookupTable(lookupTable);
    w2->On();

    
//    //---- this is for X axis -----
//    renderer->GetActiveCamera()->Azimuth(90);
//    renderer->ResetCamera();//GetActiveCamera()->
//    //---------

    //---- this is for Y axis -----
    renderer->GetActiveCamera()->Elevation(90);
    renderer->ResetCamera();//GetActiveCamera()->
    //---------

    
    renderWindowInteractor->Initialize();
    renderWindow->Render();
    
    
    renderWindowInteractor->Start();
    
    return EXIT_SUCCESS;
}
