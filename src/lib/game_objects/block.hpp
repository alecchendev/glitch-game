#include <glm/glm.hpp>

class Block {
  public:
    glm::vec3 position() {
      return position;
    }

    glm::vec3 size() {
      return size;
    }

    void move(glm::vec3 movement) {
      position += movement;
    }

  private:
    glm::vec3 position; // origin corner
    glm::vec3 size; // size of each dimension with position at origin
};