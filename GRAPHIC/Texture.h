#pragma once



#include "GL/gl3w.h"
#include <iostream>

class Texture
{
public:
	int width;
	int height;
	int channel;
	GLuint textureID;
	int current = 1;

	static int total;
	static GLuint colourTextureLocation;


	Texture(const char* _path);
	Texture(const char* _path, GLuint _colourTextureLocation);
	~Texture();
	void UseTexture();
};

