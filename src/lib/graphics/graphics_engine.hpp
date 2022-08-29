#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <unordered_map>
#include <glad/glad.h>

#include "camera.hpp"
#include "shader.h"

enum CameraMode {
  MenuView,
  FirstPerson,
  ThirdPerson
};

const std::string color_vshader = "src/shaders/vertex.glsl";
const std::string color_fshader = "src/shaders/f_color.glsl";
const std::string texture_vshader = "src/shaders/vertex.glsl";
const std::string texture_fshader = "src/shaders/f_texture.glsl";

// Class that handles the window, moving the camera, drawing objects on screen
class GraphicsEngine {
  public:
    GraphicsEngine();
    void init();
    void checkGlad();
    void drawBackground(float r, float g, float b, float a);
    void drawElements();

  private:
    Camera camera_;
    CameraMode camera_mode_;
    std::unordered_map<std::string, Shader> shaders;

};

GraphicsEngine::GraphicsEngine(): camera_(Camera()), camera_mode_(FirstPerson) {
}

void GraphicsEngine::init() {

  // Load OpenGL
  checkGlad();
	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
  // Load shaders
  shaders.insert(std::pair<std::string, Shader>("color", Shader(color_vshader.c_str(), color_fshader.c_str())));
  shaders.insert(std::pair<std::string, Shader>("texture", Shader(texture_vshader.c_str(), texture_fshader.c_str())));

}

void GraphicsEngine::drawBackground(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::drawElements() {

}

void GraphicsEngine::checkGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
    exit(1);
	}
}

#endif
