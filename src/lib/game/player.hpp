#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

#include "block.hpp"

// Class that stores and modifies data related to the player
class Player {
  public:
    glm::vec3 position() {
      return position_;
    }

    glm::vec3 size() {
      return size_;
    }

    void move(glm::vec3 movement) {
        position_ += movement;
    }

    void look(glm::vec3 direction) {
        direction_ = direction;
    }

  private:
    glm::vec3 direction_;
    glm::vec3 position_;
    glm::vec3 size_;

};

#endif
