
#ifndef VMX_VISUALIZER_HRKI
#define VMX_VISUALIZER_HRKI

#include "vmxVisualizerInteractorStyle.h"

#include "vtkRegularPolygonSource.h"
#include "vtkLineSource.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkActorCollection.h"
#include "mxImage.h"
#include "mxString.h"
#include "mxPosition.h"
#include "mxList.h"
#include "vmxMesh.h"
#include "vmxImage.h"
#include "vmxDataListWidget.h"

#define logg(XXXX) std::cout << "=====> " << XXXX << std::flush << std::endl;
#define logn(XXXX, NNNN) std::cout << "=====> " << XXXX << ": " << NNNN << std::flush << std::endl;
#define VISUALIZE_IN_ITKSNAP(XXXX) system("/home/levara/versioned/itksnap-new/bin/itksnap " XXXX );




class vmxVisualizerInteractorStyle;



class vmxVisualizer {
  private:
    vmxGUIMainWidget main_widget;
    vmxDataListWidget v_tree;
    vtkRegularPolygonSource *planes[10];
    vtkActor *plane_actors[10];
    vtkLineSource *lines[100];
    vtkActor *line_actors[100];

    vmxImage16U *images[10];
    vmxMesh *meshes[10];
    int active_meshes[10];
    int active_planes[10];
    int active_lines[100];
    int show; //-1 shows all
    vmxImage mesh_image;
    double colors[12][3];

    mxImage *image_for_centerline;
    mxImage *radius_image;
    mxPosition selectedPlaneCenter;
    mxPosition selectedPlaneDirection;
    int active_centerline_plane;
    int centerline_plane_index;
    int selected_centerline_plane_index;
    int centerline_plane_radius;
    char active_direction_axis;

    void setColors();

    vtkSmartPointer<vmxVisualizerInteractorStyle> style;
    mxList<mxPosition> centerline_centers;
    mxList<mxPosition> centerline_directions;

    vtkSmartPointer<vtkWindowToImageFilter> wimg_filter;
    vtkSmartPointer<vtkPNGWriter> png;
    int saved_screenshots;
    int using_irlname;
    mxString screenshot_save_location;


  public:
    vmxVisualizer();
    ~vmxVisualizer();


    int getEnvBool(const char * var);
    const char * getEnv( const char * var );

    void SetMesh(int number, mxImage &img, int mesh_value, double opacity, double red, double green, double blue, const char *img_name);
    //TODO:
    //void SetMeshColorDisconnectedRegions(int number, mxImage &img, int mesh_value, double opacity);
    void SetMesh(int number, mxImage &img, int mesh_value, double opacity, const char *img_name);
    void SetMeshByLabels(int number, mxImage &img, int mesh_value, double opacity, const char *img_name);
    
    void ShowImage(int number);
    void HideImage(int number);
    void ShowMesh(int number);
    void HideMesh(int number);

    void KillMesh(int number);

    void SetPlane(int number, mxImage &image, double normal_xyz[3], double center_xyz[3], int radius=10);
    void SetPlane(int number, mxImage &image, mxPosition normal, mxPosition center, int radius=10);
    void SetPlaneWithCenterlineIndex(int number, int centerline_position_index, int radius=10);
    void SetPlaneOpacity(int number, double opacity);

    void SetLine(int number, mxImage &image, double normal_xyz[3], double center_xyz[3], int width=1);
    void SetLine(int number, mxImage &image, mxPosition normal, mxPosition center, int width=1);

    void SetCenterlinePositions(mxImage &image,  mxList<mxPosition> &centers, mxList<mxPosition> &dirs, mxImage *radius_image=NULL);
    void IncrementCenterlineBasedPlaneLocation();
    void DecrementCenterlineBasedPlaneLocation();

    void SetSelectedPlaneLocation();
    int GetSelectedPlaneVectors(mxPosition &center, mxPosition &direction);
    int GetSelectedPlaneCenterlineIndex();

    void ToggleActiveDirectionAxis(char axis);
    void IncrementDirectionOnActiveAxis();
    void DecrementDirectionOnActiveAxis();


    void Update();
    void UpdateNoninteractive();
    void UpdateOnlyPlanes();

    void setTitle(const char * title);
    void setSizeAndPosition(int sizex, int sizey, int posx, int posy) ;
    void setLightBg();

    void initalizeScreenshotSaveLocation(const char * save_location, int use_irlname = 1);
    void SaveScreenshot();
};



#endif /* ifndef VMX_VISUALIZER_H */
