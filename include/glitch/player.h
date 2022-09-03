#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>


class Player {
  public:

    // Player(glm::vec3 front, glm::vec3 position, glm::vec3 hurtbox_size):
    //     front_(front), position_(position), hurtbox_size_(hurtbox_size), yaw_(-glm::pi<float>() / 2)
    // {}

    Player(glm::vec3 position, float yaw, float pitch, glm::vec3 hurtbox_size):
        position_(position), hurtbox_size_(hurtbox_size)
    {
        turn(yaw, pitch);
    }

    glm::vec3 position() const {
        return position_;
    }

    float yaw() const {
        return yaw_;
    }

    float pitch() const {
        return pitch_;
    }

    glm::vec3 front() const {
        return front_;
    }

    glm::vec3 right() const {
        return right_;
    }

    glm::vec3 hurtboxSize() const {
        return hurtbox_size_;
    }

    void go(glm::vec3 position) {
        position_ = position;
    }

    void move(glm::vec3 displacement) {
        position_ += displacement;
    }

    // void move(PlayerMovement movement, float delta_time) {
    //     float speed = move_speed_ * delta_time;
    //     glm::vec3 move_dir = glm::vec3(0.0f);
    //     if (movement == PlayerMovement::Forward) {
    //         move_dir += front_;
    //     }
    //     if (movement == PlayerMovement::Backward) {
    //         move_dir -= front_;
    //     }
    //     if (movement == PlayerMovement::Left) {
    //         move_dir -= right();
    //     }
    //     if (movement == PlayerMovement::Right) {
    //         move_dir += right();
    //     }
    //     position_ += glm::normalize(move_dir) * speed;
    // }

    void turn(float yaw, float pitch) {
        yaw_ = yaw;
        pitch_ = pitch;
        updateDirection();
    }

    void turnh(float yaw) {
        yaw_ = yaw;
        updateDirection();
    }

    void turnv(float pitch) {
        pitch_ = pitch;
        updateDirection();
    }

  private:
    glm::vec3 position_;

    float pitch_;
    float yaw_;
    glm::vec3 front_;
    glm::vec3 right_;

    glm::vec3 hurtbox_size_;

    void updateDirection() {
        // front
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);

        // right
        const glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        right_ = glm::normalize(glm::cross(front_, world_up));
    }

};

#endif
