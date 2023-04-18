#version 330 core

//only takes in the vertices because that's the onlyo thing that is needed
layout(location = 0) in vec3 vertex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 texCoords;

void main()
{

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);

	//passes the vertices as the texture coordinates; the cubemap uses 3d coordinates and the cube has 1x1x1 dimensions
	texCoords = vertex;
}
