#include "Camera.h"

Camera::Camera()
{
    
}

Camera::Camera(GLuint _projLocation, GLuint _viewLocation)
{
    //sets the initial values and sets the perspective projection as the default
    projLocation = _projLocation;
    viewLocation = _viewLocation;

    cameraPosition = glm::vec3(0, 100, 1);
    cameraTarget = glm::vec3(0, 100, 2);
    cameraUpVector = glm::vec3(0, 1, 0);

    viewMatrix = glm::mat4(1.0f);

    forward = glm::normalize(cameraPosition - cameraTarget);
    side = glm::normalize(glm::cross(cameraUpVector, forward));
    newUp = glm::normalize(glm::cross(forward, side));



    ChangePerspective(perspective);
}

Camera::~Camera()
{
}

void Camera::UpdateCamera(float deltaTime)
{
    //key input moving
    float speed = 100.0f;
    if (keyMap[GLFW_KEY_W] == true)
    {
        cameraPosition += forward * speed * deltaTime;
    }
    if (keyMap[GLFW_KEY_S] == true)
    {
        cameraPosition += -forward * speed * deltaTime;
    }
    if (keyMap[GLFW_KEY_A] == true)
    {
        cameraPosition += side * speed * deltaTime;
    }
    if (keyMap[GLFW_KEY_D] == true)
    {
        cameraPosition += -side * speed * deltaTime;
    }
    if (keyMap[GLFW_KEY_SPACE] == true)
    {
        cameraPosition += glm::vec3(0.0f, 0.1f, 0.0f) * speed * 5.0f * deltaTime;
    }
    if (keyMap[GLFW_KEY_LEFT_SHIFT] == true)
    {
        cameraPosition += glm::vec3(0.0f, -0.1f, 0.0f) * speed * 5.0f * deltaTime;
    }


    //forward =
    //forward = glm::normalize(cameraPosition - cameraTarget);
    //side = glm::normalize(glm::cross(cameraUpVector, forward));

    
    side = glm::normalize(glm::cross(cameraUpVector, forward));
    newUp = glm::normalize(glm::cross(forward, side));

   // newUp = glm::normalize(-glm::cross(forward, side));
    //view matrix


    viewMatrix = glm::lookAt(cameraPosition, forward + cameraPosition, newUp);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    //changes the perspective
    if (keyMap[GLFW_KEY_I] == true)
    {
        ChangePerspective(orthographic);
    }
    if (keyMap[GLFW_KEY_O] == true)
    {
        ChangePerspective(frustum);
    }
    if (keyMap[GLFW_KEY_P] == true)
    {
        ChangePerspective(perspective);
    }
}

void Camera::ChangePerspective(Perspectives p)
{
    switch (p)
    {
    case orthographic:
        projMatrix = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 500.0f);
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));
        break;
    case frustum:
        projMatrix = glm::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 500.0f);
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));
        break;
    case perspective:
        projMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 500.0f);
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));
        break;
    }
}