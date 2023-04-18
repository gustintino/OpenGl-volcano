#pragma once

#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <sstream> 

#include "glm/glm.hpp"

//GLuint modelLocation = 0;

struct vertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normals;
    glm::vec2 texture;
};
