#pragma once

#include "GL/gl3w.h"
#include <vector>
#include <iostream>

class Shader
{
private:
    GLuint shaderID;
    GLuint programID;

public:

    Shader(const char* vertexPath, const char* fragmentPath);

    GLuint GetProgramID();

    void MakeShader(GLenum shaderType, const char* shaderPath);
    void UseShader();

    GLuint ID();

    void CompileChecker(GLuint shader);


    char* ReadTextFile(const char* textFilepath);
};

