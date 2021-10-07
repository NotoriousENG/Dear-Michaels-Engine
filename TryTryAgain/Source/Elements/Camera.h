#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Structs/FTransform.h>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Camera_Movement {
    FORWARD = 0,
    BACKWARD = 1,
    LEFT = 2,
    RIGHT = 3,
    UP = 4,
    DOWN = 5
};

// Default camera values
const float YAW = 0;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:

    static Camera Main;
    // camera Attributes

    glm::vec3 position;
    glm::vec3 eulers;

    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
	// Useful things
    glm::mat4 projection;
    glm::mat4 view;

    float AspectRatio = 1920.f / 1080.f;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        this->position = position;
        WorldUp = up;
        this->eulers = glm::vec3(glm::radians(pitch), glm::radians(yaw), 0);
        Pitch = pitch;
        Yaw = yaw;
        Right = glm::cross(Front, WorldUp);
        Up = WorldUp;
        UpdateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        this->eulers = glm::vec3(glm::radians(pitch), glm::radians(yaw), 0);
        Pitch = pitch;
        Yaw = yaw;
        Right = glm::cross(Front, WorldUp);
        Up = WorldUp;
        UpdateCameraVectors();
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouseRel input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouseRel scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();
};

