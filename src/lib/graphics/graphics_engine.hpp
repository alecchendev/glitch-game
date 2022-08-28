
#include "camera.hpp"

enum CameraMode {
  MenuView,
  FirstPerson,
  ThirdPerson
};

// Class that handles the window, moving the camera, drawing objects on screen
class GraphicsEngine {
  public:

  private:
    Camera camera;
    CameraMode camera_mode;

};