#version 330 core

out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{
	//sets a slight red hue for the skybox because it's a volcanic area, also slightly darker
	fragColor = texture(skybox, texCoords) * 0.8f * vec4(1, 0.5, 0.5,1);
}