#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

enum KEY
{
    W, //Forward
    S, //Backward
    A, //Left
    D, //Right
    Q, //Up
    Z  //Down
};

class Camera
{
    private:
    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 2.5f);
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float fov = 90.0f;
    int screen_height = 800, screen_width = 600;
    float last_x = screen_height/2;
    float last_y = screen_width/2;
    bool first_mouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::mat4 view;
    glm::mat4 projection;

    public:
    Camera(int SCREEN_HEIGHT, int SCREEN_WIDTH)
    {
        this->screen_height = SCREEN_HEIGHT;
        this->screen_width = SCREEN_WIDTH;
    }

    Camera(int SCREEN_HEIGHT, int SCREEN_WIDTH, glm::vec3 camera_pos)
    {
        this->screen_height = SCREEN_HEIGHT;
        this->screen_width = SCREEN_WIDTH;
        this->camera_pos = camera_pos;
    }

    void SetFOV(float fov);
    void MoveCamera(float camera_speed, KEY key);
    void Rotate(double x_pos, double y_pos);
    void Update();

    glm::mat4 GetView() {return view;}
    glm::mat4 GetProjection() {return projection;}
};

void Camera::SetFOV(float fov)
{
    this->fov = fov;
}

void Camera::MoveCamera(float camera_speed, KEY key)
{
    switch (key)
    {
        case W:
        camera_pos += camera_speed * camera_front;
        break;

        case S:
        camera_pos -= camera_speed * camera_front;
        break;

        case A:
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
        break;

        case D:
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
        break;

        case Q:
        camera_pos += camera_speed * glm::vec3(0.0f, 1.0f, 0.0f);
        break;

        case Z:
        camera_pos -= camera_speed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

void Camera::Rotate(double x_pos, double y_pos)
{
    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float xoffset = x_pos - last_x;
    float yoffset = last_y - y_pos;
    last_x = x_pos;
    last_y = y_pos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}

void Camera::Update()
{
    view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    projection = glm::perspective(glm::radians(fov), float(screen_height)/float(screen_width), 0.1f, 100.0f);
}