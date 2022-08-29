
#include <glm/glm.hpp>

#include <vector>

namespace world_data {

    const float length = 5.0f;
    const glm::vec3 world0[] = {
        // start position + direction
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        // block data
        glm::vec3(-length / 2, -length, -length / 2),
        glm::vec3(length, length, length),
        glm::vec3(-length / 3, -1.0f, -length / 4),
        glm::vec3(1.0f, length / 3, length / 3)
    };

}