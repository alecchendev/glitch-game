#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace gfx {

const unsigned int N_CUBE_SOLID_COLOR_VERTICES = 8 * 3;
const unsigned int N_CUBE_TEXTURE_VERTICES = 24 * 5; // * length of single vertex data
const unsigned int N_CUBE_INDICES = 12 * 3; // * 3 xyz coords
const unsigned int N_CUBE_DRAW_VERTICES = 36;

class Block {
  public:
    
    Block(glm::vec3 position, glm::vec3 size):
        position_(position),
        size_(size)
    {}

    virtual std::vector<float> vertices() const = 0;
    virtual std::vector<unsigned int> indices() const = 0;

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

class SolidColorBlock : public Block {
  public:
    SolidColorBlock(glm::vec3 position, glm::vec3 size):
        Block(position, size)
    {}

    std::vector<float> vertices() const {

        const glm::vec3 size = this->size();
        return {
            0.0f,   0.0f,   0.0f,
            size.x, 0.0f,   0.0f,
            size.x, size.y, 0.0f,
            0.0f,   size.y, 0.0f,
            0.0f,   0.0f,   size.z,
            size.x, 0.0f,   size.z,
            size.x, size.y, size.z,
            0.0f,   size.y, size.z
        };
    }

    std::vector<unsigned int> indices() const {
        return {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

            4, 5, 1,
            1, 0, 4,

            3, 2, 6,
            6, 7, 3,

            4, 0, 3,
            3, 7, 4,

            1, 5, 6,
            6, 2, 1
        };
    }
};

class TextureBlock : public Block {
  public:
    TextureBlock(glm::vec3 position, glm::vec3 size):
        Block(position, size)
    {}

    std::vector<float> vertices() const {

        const glm::vec3 size = this->size();
        return {
            0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            size.x, 0.0f, 0.0f,  1.0f, 0.0f,
            size.x,  size.y, 0.0f,  1.0f, 1.0f,
            0.0f,  size.y, 0.0f,  0.0f, 1.0f,

            0.0f, 0.0f,  size.z,  0.0f, 0.0f,
            size.x, 0.0f,  size.z,  1.0f, 0.0f,
            size.x,  size.y,  size.z,  1.0f, 1.0f,
            0.0f,  size.y,  size.z,  0.0f, 1.0f,

            0.0f,  size.y,  size.z,  1.0f, 0.0f,
            0.0f,  size.y, 0.0f,  1.0f, 1.0f,
            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            0.0f, 0.0f,  size.z,  0.0f, 0.0f,

            size.x,  size.y,  size.z,  1.0f, 0.0f,
            size.x,  size.y, 0.0f,  1.0f, 1.0f,
            size.x, 0.0f, 0.0f,  0.0f, 1.0f,
            size.x, 0.0f,  size.z,  0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            size.x, 0.0f, 0.0f,  1.0f, 1.0f,
            size.x, 0.0f,  size.z,  1.0f, 0.0f,
            0.0f, 0.0f,  size.z,  0.0f, 0.0f,

            0.0f,  size.y, 0.0f,  0.0f, 1.0f,
            size.x,  size.y, 0.0f,  1.0f, 1.0f,
            size.x,  size.y,  size.z,  1.0f, 0.0f,
            0.0f,  size.y,  size.z,  0.0f, 0.0f,
        };
    }

    std::vector<unsigned int> indices() const {
        return {
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
    }
};

class VAO {

  public:
    
    VAO() {
        glGenVertexArrays(1, &addr_);
        glBindVertexArray(addr_);
    }

    void initFromBlock(const SolidColorBlock& block) {
        std::vector<float> vtx_vec = block.vertices();
        float vertices[gfx::N_CUBE_SOLID_COLOR_VERTICES];
        std::copy(vtx_vec.begin(), vtx_vec.end(), vertices);
        std::vector<unsigned int> idx_vec = block.indices();
        unsigned int indices[gfx::N_CUBE_INDICES];
        std::copy(idx_vec.begin(), idx_vec.end(), indices);

        addVertexBuffer(sizeof(vertices), vertices, GL_STATIC_DRAW);
        addElementBuffer(sizeof(indices), indices, GL_STATIC_DRAW);
        addVertexAttribute(0, 3, 3, 0);
    }

    void initFromBlock(const TextureBlock& block) {
        std::vector<float> vtx_vec = block.vertices();
        float vertices[gfx::N_CUBE_TEXTURE_VERTICES];
        std::copy(vtx_vec.begin(), vtx_vec.end(), vertices);
        std::vector<unsigned int> idx_vec = block.indices();
        unsigned int indices[gfx::N_CUBE_INDICES];
        std::copy(idx_vec.begin(), idx_vec.end(), indices);

        addVertexBuffer(sizeof(vertices), vertices, GL_STATIC_DRAW);
        addElementBuffer(sizeof(indices), indices, GL_STATIC_DRAW);
        addVertexAttribute(0, 3, 5, 0);
        addVertexAttribute(1, 3, 5, 3);
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
