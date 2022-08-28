#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "block.hpp"

// Class that stores information about an environment the player is in
class World {
  public:
    World(glm::vec3 start_position, glm::vec3 start_direction, std::vector<Block> blocks):
        start_position_(start_position), start_direction_(start_direction), blocks_(blocks) {}

    std::vector<Block> blocks() {
        return blocks_;
    }

    glm::vec3 startPosition() {
        return start_position_;
    }

    glm::vec3 startDirection() {
        return start_direction_;
    }

  private:
    glm::vec3 start_position_;
    glm::vec3 start_direction_;
    std::vector<Block> blocks_;
};

#endif
