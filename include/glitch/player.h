#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

enum PlayerMovement {
    Forward,
    Backward,
    Left,
    Right
};

class Player {
  public:

    Player(glm::vec3 front, glm::vec3 position, glm::vec3 hurtbox_size):
        front_(front), position_(position), hurtbox_size_(hurtbox_size)
    {}

    glm::vec3 position() {
        return position_;
    }

    glm::vec3 front() {
        return front_;
    }

    glm::vec3 right() {
        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(front_, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        return right;
    }

    glm::vec3 hurtboxSize() {
        return hurtbox_size_;
    }

    void move(PlayerMovement movement, float delta_time) {
        float speed = move_speed_ * delta_time;
        if (movement == PlayerMovement::Forward)
            position_ += front_ * speed;
        if (movement == PlayerMovement::Backward)
            position_ -= front_ * speed;
        if (movement == PlayerMovement::Left)
            position_ -= right() * speed;
        if (movement == PlayerMovement::Right)
            position_ += right() * speed;
    }

    void turn(float angle) {
        // calculate angles
        const float pitch = 0.0f;
        const glm::vec3 true_right = glm::vec3(1.0f, 0.0f, 0.0f);
        const float curr_yaw = glm::angle(true_right, front_);
        const float yaw = curr_yaw + angle;

        // calculate front
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front_ = glm::normalize(front);

        // also re-calculate right
        // const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // right_ = glm::normalize(glm::cross(front_, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    }

  private:
    glm::vec3 front_;
    // glm::vec3 right_;
    glm::vec3 position_;
    glm::vec3 hurtbox_size_;
    const float move_speed_ = 2.5f;
    const float turn_speed_ = 0.25f;

};

#endif
