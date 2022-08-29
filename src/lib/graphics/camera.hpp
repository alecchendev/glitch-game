#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float XZ_ANGLE    = -90.0f;
const float XY_ANGLE    =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

// Camera class for representing the location and direction of the user's perspective in-game
class Camera
{
  public:

    // constructor with vectors
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f),
        glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
        float xz_angle = XZ_ANGLE,
        float xy_angle = XY_ANGLE
    ) : movement_speed_(SPEED),
        mouse_sensitivity_(SENSITIVITY),
        zoom_(ZOOM)
    {
        position_ = position;
        world_up_ = world_up;
        front_ = front;
        xy_angle_ = xy_angle;
        xz_angle_ = xz_angle;

        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position_, position_ + front_, up_);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = movement_speed_ * deltaTime;
        if (direction == FORWARD)
            position_ += front_ * velocity;
        if (direction == BACKWARD)
            position_ -= front_ * velocity;
        if (direction == LEFT)
            position_ -= right_ * velocity;
        if (direction == RIGHT)
            position_ += right_ * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainxy_angle = true)
    {
        xoffset *= mouse_sensitivity_;
        yoffset *= mouse_sensitivity_;

        xz_angle_ += xoffset;
        xy_angle_ += yoffset;

        // make sure that when xy_angle is out of bounds, screen doesn't get flipped
        if (constrainxy_angle)
        {
            if (xy_angle_ > 89.0f)
                xy_angle_ = 89.0f;
            if (xy_angle_ < -89.0f)
                xy_angle_ = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

  private:
    // camera Attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    // euler Angles
    float xz_angle_;
    float xy_angle_;
    // camera options
    float movement_speed_;
    float mouse_sensitivity_;
    float zoom_;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(xz_angle_)) * cos(glm::radians(xy_angle_));
        front.y = sin(glm::radians(xy_angle_));
        front.z = sin(glm::radians(xz_angle_)) * cos(glm::radians(xy_angle_));
        front_ = glm::normalize(front);
        // also re-calculate the Right and Up vector
        right_ = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up_    = glm::normalize(glm::cross(right_, front_));
    }
};
#endif