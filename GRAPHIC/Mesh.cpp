#include "Mesh.h"

Mesh::Mesh()
{
	modelLocation = 0;
}

Mesh::~Mesh()
{
}

void Mesh::ChangePosition(glm::vec3 _translation, glm::vec3 _rotation,  glm::vec3 _scale)
{
	//moves, rotates and/or scales the model matrix
	position = _translation;
	rotation = _rotation;
	scale = _scale;

	model = glm::translate(glm::mat4(1.0f), _translation);

	model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0, 0, 1));

	model = glm::scale(model, _scale);
}
