#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glm/glm.hpp>

namespace gfx {



class Model {
  public:

    Model(): model_(glm::mat4(1.0f)) {}

    Model translate(glm::vec3 translation) {
        model_ = glm::translate(model_, translation);
        return *this;
    }

    Model rotate(float angle_in_radians, glm::vec3 axis) {
        model_ = glm::rotate(model_, angle_in_radians, axis);
        return *this;
    }
    
    glm::mat4 mat4() {
        return model_;
    }

  private:
    glm::mat4 model_;
};


}


#endif
