#include "GameObject.h"

GameObject::GameObject(Mesh* _mesh, Texture* _texture)
{
	//sets
	mesh = _mesh;
	texture = _texture;
}

GameObject::~GameObject()
{
	mesh = nullptr;
	texture = nullptr;
}

void GameObject::Draw()
{
	//uses a texture and draws the mes
	texture->UseTexture();
	mesh->Draw();
}

void GameObject::Draw(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) 
{
	//same as above but moves the object
	texture->UseTexture();
	mesh->Draw(_position, _rotation, _scale);
}
