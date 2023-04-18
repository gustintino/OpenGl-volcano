#pragma once

#include "glm/glm.hpp"
#include "GL/gl3w.h"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLFW/glfw3.h"
#include <map>

class Camera
{
public:
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUpVector;

    glm::vec3 forward;
    glm::vec3 side;
    glm::vec3 newUp;

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    GLuint projLocation;
    GLuint viewLocation;

    std::map<int, bool> keyMap;

    enum Perspectives
    {
        orthographic,
        frustum,
        perspective
    };

    Camera();
    Camera(GLuint _projLocation, GLuint _viewLocation);

    ~Camera();

    void UpdateCamera(float deltaTime);

    void ChangePerspective(Perspectives p);
};

