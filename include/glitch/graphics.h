#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace gfx {

class Texture {
  public:
    struct Param {
        int param;
        int value;
    };
    
    Texture(
        unsigned int type,
        const std::vector<Param>& params,
        const std::string& image_path,
        bool transparent
    ): type_(type) {

        // init texture
        glGenTextures(1, &id_);
        glBindTexture(type, id_);

        // set params
        for (Param param : params) {
            glTexParameteri(type, param.param, param.value);
        }

        // load image, create texture, generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(type, 0, GL_RGB, width, height, 0, (transparent) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(type);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

    }

    void activeBindTexture(int tx) {
        glActiveTexture(tx);
        glBindTexture(type_, id_);
    }

    unsigned int type() {
        return type_;
    }

    unsigned int id() {
        return id_;
    }

  private:
    unsigned int type_;
    unsigned int id_;

};

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
