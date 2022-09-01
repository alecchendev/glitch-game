#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

enum PlayerMovement {
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
        if (movement == PlayerMovement::Forward) {
            position_ += front_ * speed;
        }
        if (movement == PlayerMovement::Backward) {
            position_ -= front_ * speed;
        }
        if (movement == PlayerMovement::Left) {
            position_ -= right() * speed / 2.0f;
            turn(-turn_speed_);
        }
        if (movement == PlayerMovement::Right) {
            position_ += right() * speed / 2.0f;
            turn(turn_speed_);
        }
    }

    void turn(float angle) {
        // calculate angles
        const float pitch = 0.0f;
        yaw_ += angle;
        // std::cout << yaw << std::endl;
        // if (yaw <= 0.0f) {
        //     yaw += 2 * glm::pi<float>();
        // } else if (yaw >= 2 * glm::pi<float>()) {
        //     yaw -= 2 * glm::pi<float>();
        // }
        // std::cout << yaw << std::endl;

        // calculate front
        glm::vec3 front;
        front.x = cos((yaw_)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin((yaw_)) * cos(glm::radians(pitch));
        front_ = glm::normalize(front);

        // also re-calculate right
        // const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // right_ = glm::normalize(glm::cross(front_, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    }

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
