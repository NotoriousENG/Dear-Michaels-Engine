#include "Camera.h"
// #include <Panels/Console.h>
#include <glm/gtx/matrix_decompose.hpp>

Camera Camera::Main = Camera(glm::vec3(0, 0, 4), glm::vec3(0, 1, 0), -90);

void Camera::ProcessKeyboard(glm::vec3 input, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    position += input * velocity;

    UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    //glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(transform.rotation));
    float Yaw = glm::degrees(eulers.y);
    float Pitch = glm::degrees(eulers.x);

    Yaw += xoffset;
    Pitch += yoffset;


    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    eulers.y = glm::radians(Yaw);
    eulers.x = glm::radians(Pitch);

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    //glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(transform.rotation));
    float Yaw = glm::degrees(eulers.y);
    float Pitch = glm::degrees(eulers.x);

    // calculate the new Front vector
    glm::vec3 front = glm::vec3(0);
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));

    projection = glm::perspective(glm::radians(this->Zoom), AspectRatio, 0.1f, 1000.0f);

    view = glm::lookAt(position, position + Front, Up);
}
