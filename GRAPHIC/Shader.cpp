#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    //loads the shaders in creates the shader program
    programID = glCreateProgram();

    MakeShader(GL_VERTEX_SHADER, vertexPath);
    MakeShader(GL_FRAGMENT_SHADER, fragmentPath);

    glLinkProgram(programID);
    UseShader();
}

GLuint Shader::GetProgramID()
{
    return programID;
}

void Shader::MakeShader(GLenum shaderType, const char* shaderPath)
{
    //makes the shader with its type and attaches it
    char* shaderCode = ReadTextFile(shaderPath);
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);
    CompileChecker(shaderID);
    glAttachShader(programID, shaderID);
}

void Shader::UseShader()
{
    //uses the shader, because of multiple shaders programs (main + skybox + others)
    glUseProgram(programID);
}

GLuint Shader::ID()
{
    return shaderID;
}

void Shader::CompileChecker(GLuint shader)
{
    //check if the shaders compile correctly, if not throws an error in the cmd
    GLint result = GL_FALSE;
    int logLength;

    try
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
        glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
        std::cout << &vertShaderError[0] << std::endl;

        if (result == GL_FALSE)
        {
            throw - 1;
        }
    }
    catch (int e)
    {
        std::cout << "Shader acting up." << e << std::endl;
    }
}

char* Shader::ReadTextFile(const char* textFilepath)
{
    //reads a shader from a file and puts in a string
    FILE* filePointer;
    fopen_s(&filePointer, textFilepath, "rb");
    char* content = NULL;
    long numVal = 0;

    fseek(filePointer, 0L, SEEK_END);
    numVal = ftell(filePointer);
    fseek(filePointer, 0L, SEEK_SET);
    content = (char*)malloc((numVal + 1) * sizeof(char));
    fread(content, 1, numVal, filePointer);
    content[numVal] = '\0';
    fclose(filePointer);
    return content;
}