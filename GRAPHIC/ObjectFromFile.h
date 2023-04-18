#pragma once

#include "GL/gl3w.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <fstream> 
#include <sstream> 
#include "values.h"

#include "Mesh.h"

class ObjectFromFile: public Mesh
{
public:
	//GLuint vao;
	//GLuint vbo;
	//int numOfVert;

	glm::vec4 color;
	std::vector<vertex> vertices;


	ObjectFromFile(const char* path,  glm::vec4 _color);
	ObjectFromFile(const char* _path, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale);
	~ObjectFromFile();
	std::vector<vertex> LoadOBJFile(const char* filePath);
	void Draw(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) override;
	void Draw() override;
};