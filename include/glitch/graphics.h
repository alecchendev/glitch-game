#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace gfx {

class VAO {

  public:
    
    VAO() {
        glGenVertexArrays(1, &addr_);
        glBindVertexArray(addr_);
    }

    void addVertexBuffer(unsigned int data_size, float vertices[], int draw_type) {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data_size, vertices, draw_type);
        buffer_addrs_.push_back(VBO);
    }

    void addElementBuffer(unsigned int data_size, unsigned int indices[], int draw_type) {
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, indices, draw_type);
        buffer_addrs_.push_back(EBO);
    }

    // n_values = how many values in this single attribute
    // value_count = how many values in one whole unit (this attribute is just a part of one unit)
    // start_idx
    void addVertexAttribute(unsigned int attribute_id, unsigned int n_values, unsigned int value_count, unsigned int start_idx) {
        glVertexAttribPointer(attribute_id, n_values, GL_FLOAT, GL_FALSE, value_count * sizeof(float), (void*)(start_idx * sizeof(float)));
        glEnableVertexAttribArray(attribute_id);
    }

    void bind() {
        glBindVertexArray(addr_);
    }

    void drawArrays(unsigned int n_vertices) {
        glDrawArrays(GL_TRIANGLES, 0, n_vertices);
    }

    void drawElements(unsigned int n_vertices) {
        glDrawElements(GL_TRIANGLES, n_vertices, GL_UNSIGNED_INT, 0);
    }

    void deallocate() {
        glDeleteVertexArrays(1, &addr_);
        for (unsigned int buffer_addr : buffer_addrs_) {
            // not really sure if this is doing what I think it does
            glDeleteBuffers(1, &buffer_addr);
        }
    }

  private:
    unsigned int addr_;
    std::vector<unsigned int> buffer_addrs_;
};

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

// Unnecessary class
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
