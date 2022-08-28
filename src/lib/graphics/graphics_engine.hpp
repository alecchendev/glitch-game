#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include "camera.hpp"

enum CameraMode {
  MenuView,
  FirstPerson,
  ThirdPerson
};

// Class that handles the window, moving the camera, drawing objects on screen
class GraphicsEngine {
  public:
    GraphicsEngine(): camera_(Camera()), camera_mode_(FirstPerson) {}

  private:
    Camera camera_;
    CameraMode camera_mode_;

};

#endif
