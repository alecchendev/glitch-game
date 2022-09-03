#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

enum class PlayerMovement {
    Forward,
    Backward,
    Left,
    Right
};

class Player {
  public:

    Player(glm::vec3 front, glm::vec3 position, glm::vec3 hurtbox_size):
        front_(front), position_(position), hurtbox_size_(hurtbox_size), yaw_(-glm::pi<float>() / 2)
    {}

    glm::vec3 position() const {
        return position_;
    }

    glm::vec3 front() const {
        return front_;
    }

    glm::vec3 right() const {
        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(front_, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        return right;
    }

    float yaw() const {
        return yaw_;
    }

    glm::vec3 hurtboxSize() const {
        return hurtbox_size_;
    }

    void move(PlayerMovement movement, float delta_time) {
        float speed = move_speed_ * delta_time;
        glm::vec3 move_dir = glm::vec3(0.0f);
        if (movement == PlayerMovement::Forward) {
            move_dir += front_;
        }
        if (movement == PlayerMovement::Backward) {
            move_dir -= front_;
        }
        if (movement == PlayerMovement::Left) {
            move_dir -= right();
        }
        if (movement == PlayerMovement::Right) {
            move_dir += right();
        }
        position_ += glm::normalize(move_dir) * speed;
    }

    void turn(float yaw) {
        float pitch = 0.0f;
        glm::vec3 front;
        yaw_ = glm::radians(yaw);
        front.x = cos((yaw_)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin((yaw_)) * cos(glm::radians(pitch));
        front_ = glm::normalize(front);
    }

    // void turn(float angle) {
    //     // calculate angles
    //     const float pitch = 0.0f;
    //     yaw_ += angle;
    //     // calculate front
    //     glm::vec3 front;
    //     front.x = cos((yaw_)) * cos(glm::radians(pitch));
    //     front.y = sin(glm::radians(pitch));
    //     front.z = sin((yaw_)) * cos(glm::radians(pitch));
    //     front_ = glm::normalize(front);
    // }

  private:
    glm::vec3 front_;
    // glm::vec3 right_;
    float yaw_;
    glm::vec3 position_;
    glm::vec3 hurtbox_size_;
    const float move_speed_ = 3.0f;
    const float turn_speed_ = 0.025f;

};

#endif
