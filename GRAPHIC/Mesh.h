#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GL/gl3w.h"

class Mesh
{
public:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	int numOfVert;
	int numOfInd;

	glm::mat4 model;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	static GLuint modelLocation;

	Mesh();
	
	virtual ~Mesh();

	virtual void Draw() = 0;
	virtual void Draw(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) = 0;

	void ChangePosition(glm::vec3 _translation, glm::vec3 _rotation, glm::vec3 _scale);

};

