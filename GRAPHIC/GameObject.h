#pragma once

#include "Mesh.h"
#include "Texture.h"

class GameObject
{
public:
	Mesh* mesh;
	Texture* texture;

	GameObject(Mesh* _mesh, Texture* _texture);
	~GameObject();
	void Draw();
	void Draw(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale);
};

