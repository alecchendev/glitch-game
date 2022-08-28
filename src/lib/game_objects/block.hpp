#include <glm/glm.hpp>

// Class representing the basic element of an environment
class Block {
  public:
    glm::vec3 position() {
      return position;
    }

    glm::vec3 size() {
      return size;
    }

  private:
    glm::vec3 position; // origin corner
    glm::vec3 size; // size of each dimension with position at origin
};