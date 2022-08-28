#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

// Class representing the basic element of an environment
class Block {
  public:
    Block(glm::vec3 position, glm::vec3 size): position_(position), size_(size) {}

    glm::vec3 position() {
      return position_;
    }

    glm::vec3 size() {
      return size_;
    }

  private:
    glm::vec3 position_; // origin corner
    glm::vec3 size_; // size of each dimension with position at origin
};

#endif
