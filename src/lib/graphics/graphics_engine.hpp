#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <unordered_map>
#include <glad/glad.h>

#include "camera.hpp"
#include "shader.h"
#include "../game/block.hpp"

enum CameraMode {
  MenuView,
  FirstPerson,
  ThirdPerson
};

const std::string color_vshader = "src/shaders/vertex.glsl";
const std::string color_fshader = "src/shaders/f_color.glsl";
const std::string texture_vshader = "src/shaders/vertex.glsl";
const std::string texture_fshader = "src/shaders/f_texture.glsl";
const int n_vertices = 8;
const glm::vec4 color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);

// Class that handles the window, moving the camera, drawing objects on screen
class GraphicsEngine {
  public:
    GraphicsEngine();
    void init();
    void checkGlad();

    void addBlock(const Block& block);
    void useShader(const std::string& shader);
    void freeShader(const std::string& shader);

    void drawBackground(float r, float g, float b, float a);
    void drawElement(const std::string& shader, unsigned int vao);
    void drawElements();

  private:
    Camera camera_;
    CameraMode camera_mode_;
    std::unordered_map<std::string, Shader*> shaders_;
    std::vector<unsigned int> block_vaos_;

};

GraphicsEngine::GraphicsEngine(): camera_(Camera()), camera_mode_(FirstPerson) {
}

void GraphicsEngine::init() {

  // Load OpenGL
  checkGlad();
	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
  // Load shaders
  shaders_.insert(std::pair<std::string, Shader*>("color", new Shader(color_vshader.c_str(), color_fshader.c_str())));
  shaders_.insert(std::pair<std::string, Shader*>("texture", new Shader(texture_vshader.c_str(), texture_fshader.c_str())));

}

void GraphicsEngine::addBlock(const Block& block) {
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  block_vaos_.push_back(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  const glm::vec3 pos = block.position();
  const glm::vec3 size = block.size();

  float vertices[] = {
    pos.x, pos.y, pos.z, // 0, 0, 0
    pos.x + size.x, pos.y, pos.z, // x, 0, 0
    pos.x, pos.y + size.y, pos.z, // 0, y, 0
    pos.x, pos.y, pos.z + size.z, // 0, 0, z
    pos.x + size.x, pos.y + size.y, pos.z, // x, y, 0
    pos.x + size.x, pos.y, pos.z + size.z, // x, 0, z
    pos.x, pos.y + size.y, pos.z + size.z, // 0, y, z
    pos.x + size.x, pos.y + size.y, pos.z + size.z, // x, y, z
  };
  unsigned int indices[] = {
    0, 1, 4, // 0xy
    0, 2, 4,
    0, 1, 5, // 0xz
    0, 3, 5,
    0, 2, 6, // 0yz
    0, 3, 6,
    7, 5, 3, // 1xy
    7, 6, 3,
    7, 4, 2, // 1xz
    7, 6, 2,
    7, 4, 1, // 1yz
    7, 5, 1,
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void GraphicsEngine::useShader(const std::string& shader) {
  shaders_[shader]->use();
}

void GraphicsEngine::freeShader(const std::string& shader) {
  delete shaders_[shader];
}

void GraphicsEngine::drawBackground(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::drawElement(const std::string& shader, unsigned int vao) {
  useShader(shader);
  glBindVertexArray(vao);
  shaders_[shader]->setVec4("color", color);
  glDrawElements(GL_TRIANGLES, n_vertices, GL_UNSIGNED_INT, 0);
}

void GraphicsEngine::drawElements() {
  for (unsigned int vao : block_vaos_) {
    drawElement("color", vao);
  }
}

void GraphicsEngine::checkGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
    exit(1);
	}
}

#endif
