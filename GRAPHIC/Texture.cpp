#include "Texture.h"

#include "stb_image.h"

int Texture::total;
GLuint Texture::colourTextureLocation;

Texture::Texture(const char* _path)
{
    //loads in and sets the attributes
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageData = stbi_load(_path, &width, &height, &channel, 3);
    if (imageData == NULL) std::cout << "Texture not loaded." << std::endl;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
}

void Texture::UseTexture()
{
    //activates and bind the coresponding texture each time it's called in gameobject.draw()
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glUniform1ui(colourTextureLocation, current);
}

Texture::Texture(const char* _path, GLuint _colourTextureLocation) : Texture(_path)
{
    //another constructor for setting the static variable, not actually needed
    colourTextureLocation = _colourTextureLocation;
}
