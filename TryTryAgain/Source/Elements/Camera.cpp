#include "Camera.h"
#include <Panels/Console.h>
#include <glm/gtx/matrix_decompose.hpp>

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        transform.position += Front * velocity;
    if (direction == BACKWARD)
        transform.position -= Front * velocity;
    if (direction == LEFT)
        transform.position -= Right * velocity;
    if (direction == RIGHT)
        transform.position += Right * velocity;
    if (direction == UP)
        transform.position += Up * velocity;
    if (direction == DOWN)
        transform.position -= Up * velocity;

    UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    //glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(transform.rotation));
    float Yaw = glm::degrees(transform.rotation.y);
    float Pitch = glm::degrees(transform.rotation.x);
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

    transform.rotation.y = glm::radians(Yaw);
    transform.rotation.x = glm::radians(Pitch);

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
    float Yaw = glm::degrees(transform.rotation.y);
    float Pitch = glm::degrees(transform.rotation.x);

    // calculate the new Front vector
    glm::vec3 front = glm::vec3(0);
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));

    projection = glm::perspective(glm::radians(this->Zoom), AspectRatio, 0.1f, 100.0f);

    view = glm::lookAt(transform.position, transform.position + Front, Up);
}
