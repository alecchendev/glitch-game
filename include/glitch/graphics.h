#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace gfx {

const unsigned int N_CUBE_SOLID_COLOR_VERTICES = 8;
const unsigned int N_CUBE_TEXTURE_VERTICES = 24 * 5; // * length of single vertex data
const unsigned int N_CUBE_INDICES = 12 * 3; // * 3 xyz coords
const unsigned int N_CUBE_DRAW_VERTICES = 36;

class Block {
  public:
    
    Block(glm::vec3 position, glm::vec3 size):
        position_(position),
        size_(size)
    {}

    virtual std::vector<float> vertices() = 0;
    virtual std::vector<unsigned int> indices() = 0;

    glm::vec3 position() const {
        return position_;
    }

    glm::vec3 size() const {
        return size_;
    }

  private:
    glm::vec3 position_;
    glm::vec3 size_;
};

// class SolidColorBlock : public Block {
//   public:

//     std::vector<float> vertices() {
//         return {};
//     }

//     std::vector<unsigned int> indices() {
//         return {};
//     }
// }

class TextureBlock : public Block {
  public:
    TextureBlock(glm::vec3 position, glm::vec3 size):
        Block(position, size)
    {}

    std::vector<float> vertices() {

        const glm::vec3 size = this->size();
        const float x_size = size.x / 2;
        const float y_size = size.y / 2;
        const float z_size = size.z / 2;
        const std::vector<float> vertices = {
            0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            x_size, 0.0f, 0.0f,  1.0f, 0.0f,
            x_size,  y_size, 0.0f,  1.0f, 1.0f,
            0.0f,  y_size, 0.0f,  0.0f, 1.0f,

            0.0f, 0.0f,  z_size,  0.0f, 0.0f,
            x_size, 0.0f,  z_size,  1.0f, 0.0f,
            x_size,  y_size,  z_size,  1.0f, 1.0f,
            0.0f,  y_size,  z_size,  0.0f, 1.0f,

            0.0f,  y_size,  z_size,  1.0f, 0.0f,
            0.0f,  y_size, 0.0f,  1.0f, 1.0f,
            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            0.0f, 0.0f,  z_size,  0.0f, 0.0f,

            x_size,  y_size,  z_size,  1.0f, 0.0f,
            x_size,  y_size, 0.0f,  1.0f, 1.0f,
            x_size, 0.0f, 0.0f,  0.0f, 1.0f,
            x_size, 0.0f,  z_size,  0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            x_size, 0.0f, 0.0f,  1.0f, 1.0f,
            x_size, 0.0f,  z_size,  1.0f, 0.0f,
            0.0f, 0.0f,  z_size,  0.0f, 0.0f,

            0.0f,  y_size, 0.0f,  0.0f, 1.0f,
            x_size,  y_size, 0.0f,  1.0f, 1.0f,
            x_size,  y_size,  z_size,  1.0f, 0.0f,
            0.0f,  y_size,  z_size,  0.0f, 0.0f,
        };
        return vertices;
    }

    std::vector<unsigned int> indices() {
        const std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20,
        };
        return indices;
    }
};

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

}


#endif
