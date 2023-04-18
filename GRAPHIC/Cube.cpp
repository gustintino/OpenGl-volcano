#include "Cube.h"

GLuint Mesh::modelLocation;

struct vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 normals;
};

//hardcoded vertices
vertex cubeVertices[]
{
	//     position                      color                         normals
	{{-0.5f, -0.5f, -0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {0.0f, -1.0f, 0.0f}},  // 0
	{{ 0.5f, -0.5f, -0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {0.0f, -1.0f, 0.0f}},  // 1
	{{ 0.5f, -0.5f,  0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {0.0f, -1.0f, 0.0f}},  // 2
	{{-0.5f, -0.5f,  0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {-1.0f, 0.0f, 0.0f}},  // 3
	{{-0.5f,  0.5f, -0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {0.0f,  1.0f, 0.0f}},  // 4
	{{ 0.5f,  0.5f, -0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {0.0f,  1.0f, 0.0f}},  // 5
	{{ 0.5f,  0.5f,  0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {0.0f,  0.0f, 1.0f}},  // 6
	{{-0.5f,  0.5f,  0.5f},     {0.0f, 1.0f, 0.0f, 1.0f},       {0.0f,  0.0f, 1.0f}}   // 7
};
unsigned int cubeIndices[]
{
	3, 2, 7,  2, 7, 6,   //front
	2, 1, 6,  1, 6, 5,   //right
	0, 3, 4,  3, 4, 7,   //left
	1, 0, 5,  0, 5, 4,   //back
	0, 1, 3,  1, 3, 2,   //bottom
	7, 6, 4,  6, 4, 5    //top
};

Cube::Cube()
{
	model = glm::mat4(1.0f);
	vao = 0;
	vbo = 0;
	ibo = 0;
	numOfVert = 8;
	numOfInd = 36;

	//generation vao, vbo, ibo; passing and enabling the vertex attribute array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numOfInd, cubeIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * numOfVert, cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (GLvoid*)(sizeof(GLfloat) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (GLvoid*)(sizeof(GLfloat) * 7));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


}

//void Cube::ChangePosition(glm::vec3 offset)
//{
//	model = glm::translate(model, offset);
//}

Cube::~Cube()
{
}

void Cube::Draw(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
{
	//draws the object, apply any translation, rotation and rescaling to the model matrix
	model = glm::mat4(1.0f);
	ChangePosition(_position, _rotation, _scale);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numOfInd, GL_UNSIGNED_INT, NULL);
}

void Cube::Draw()
{
}
