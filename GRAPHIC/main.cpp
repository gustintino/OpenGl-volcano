#include <iostream>
#include <vector>
#include <fstream>
#include <map>

#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Cube.h"
#include "ObjectFromFile.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

//previous light, not in use
glm::vec3 ambient;
glm::vec3 diffuseIntensity;
glm::vec3 lightPosition;
glm::vec3 specularIntensity;
glm::vec3 specularColor;
float shinines;

//global for easier use
GLuint modelLocation;
GLuint viewLocation;
GLuint projLocation;

double oldTime;
double currentTime;

std::map<int, bool> keyMap;

//for ease of use
glm::vec4 red(1, 0, 0, 1);
glm::vec4 green(0, 1, 0, 1);
glm::vec4 blue(0, 0, 1, 1);

//for textures
int width;
int height;
int channel;

//mouse movement
float lastX = 400;
float lastY = 400;
float yaw = -90.0f;
float pitch = 0;

Camera* camera;

//initial jerk when loading in fix
bool firstMouse = true;

//splashscreen removing
bool moved = false;

//struct for different types of light
struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambientIntensity;
    glm::vec3 diffuseIntensity;
    glm::vec3 specularIntensity;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 ambientIntensity;
    glm::vec3 diffuseIntensity;
    glm::vec3 specularIntensity;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    glm::vec3 position;
    glm::vec3 ambientIntensity;
    glm::vec3 diffuseIntensity;
    glm::vec3 specularIntensity;

    glm::vec3 coneDirection;
    float coneAngle;

    float constant;
    float linear;
    float quadratic;
};

//key input callback
void KeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    moved = true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (action == GLFW_PRESS)
    {
        camera->keyMap[key] = true;
        //if(key != 256)
            //std::cout << glfwGetKeyName(key, scancode) << " pressed." << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        camera->keyMap[key] = false;
        //std::cout << glfwGetKeyName(key, scancode) << " de-pressed." << std::endl;
    }
}

//mouse input callback
void MouseInput(GLFWwindow* window, double xpos, double ypos)
{
    //fixes the initial mouse teleport
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xOffset *= 0.1f;
    yOffset *= 0.1f;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera->forward = glm::normalize(direction);
}

//resizing callback
void Resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
    //initialization 
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //setting up the window and the resizing callback
    GLFWwindow* window = glfwCreateWindow(800, 800, "thing", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, Resize);

    //error in case of error
    if (!window)
    {
        std::cout << "ERROR: The window has not been created!" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (gl3wInit())
    {
        std::cout << "ERROR: Gl3w has not been initialized!" << std::endl;
        return -1;
    }

    //vsynx
    glfwSwapInterval(1);

    //setting callbacks
    glfwSetKeyCallback(window, KeyboardInput);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, MouseInput);

    //shader loading
    Shader shader("vert.glsl", "frag.glsl");
    Shader skyboxShader("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl");

    //set and send everything to the main shader
    shader.UseShader();
    
    //directional light setting, getting location and sending
    DirectionalLight directLight1;
    directLight1.direction = glm::vec3(4.0f, -1.0f, 0.0f);
    directLight1.ambientIntensity = glm::vec3(0.1f);
    directLight1.diffuseIntensity = glm::vec3(0.3f);
    directLight1.specularIntensity = glm::vec3(1.0f);

    GLuint directionalDirectionLocation = glGetUniformLocation(shader.GetProgramID(), "direct1.direction");
    GLuint directionalAmbientLocation = glGetUniformLocation(shader.GetProgramID(), "direct1.ambientIntensity");
    GLuint directionalDiffuseLocation = glGetUniformLocation(shader.GetProgramID(), "direct1.diffuseIntensity");
    GLuint directionalSpecularLocation = glGetUniformLocation(shader.GetProgramID(), "direct1.specularIntensity");

    glUniform3fv(directionalDirectionLocation, 1, glm::value_ptr(directLight1.direction));
    glUniform3fv(directionalAmbientLocation, 1, glm::value_ptr(directLight1.ambientIntensity));
    glUniform3fv(directionalDiffuseLocation, 1, glm::value_ptr(directLight1.diffuseIntensity));
    glUniform3fv(directionalSpecularLocation, 1, glm::value_ptr(directLight1.specularIntensity));

    //point light setting, getting location and sending
    PointLight pointLight1;
    pointLight1.position = glm::vec3(0, 20, 0);
    pointLight1.ambientIntensity = glm::vec3(0.7f);
    pointLight1.diffuseIntensity = glm::vec3(0.8f);
    pointLight1.specularIntensity = glm::vec3(1.0f);
    pointLight1.constant = 1;
    pointLight1.linear = 0.1f;
    pointLight1.quadratic = 0.25f;

    GLuint pointPositionLocation = glGetUniformLocation(shader.GetProgramID(), "point1.direction");
    GLuint pointAmbientLocation = glGetUniformLocation(shader.GetProgramID(), "point1.ambientIntensity");
    GLuint pointDiffuseLocation = glGetUniformLocation(shader.GetProgramID(), "point1.diffuseIntensity");
    GLuint pointSpecularLocation = glGetUniformLocation(shader.GetProgramID(), "point1.specularIntensity");
    GLuint pointConstantLocation = glGetUniformLocation(shader.GetProgramID(), "point1.constant");
    GLuint pointLinearLocation = glGetUniformLocation(shader.GetProgramID(), "point1.linear");
    GLuint pointQuadraticLocation = glGetUniformLocation(shader.GetProgramID(), "point1.quadratic");
    
    glUniform3fv(pointPositionLocation, 1, glm::value_ptr(pointLight1.position));
    glUniform3fv(pointAmbientLocation, 1, glm::value_ptr(pointLight1.ambientIntensity));
    glUniform3fv(pointDiffuseLocation, 1, glm::value_ptr(pointLight1.diffuseIntensity));
    glUniform3fv(pointSpecularLocation, 1, glm::value_ptr(pointLight1.specularIntensity));
    glUniform1f(pointConstantLocation, pointLight1.constant);
    glUniform1f(pointLinearLocation, pointLight1.linear);
    glUniform1f(pointQuadraticLocation, pointLight1.quadratic);

    //spotlight setting, getting location and sending
    SpotLight spotLight1;
    spotLight1.position = glm::vec3(0, 5, 0);
    spotLight1.ambientIntensity = glm::vec3(0.01f);
    spotLight1.diffuseIntensity = glm::vec3(0.3f);
    spotLight1.specularIntensity = glm::vec3(0.8f);
    spotLight1.coneDirection = glm::vec3(0, 1, 0);
    spotLight1.coneAngle = glm::radians(90.0f);
    spotLight1.constant = 1;
    spotLight1.linear = 0.1f;
    spotLight1.quadratic = 0.1;

    GLuint spotPositionLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.direction");
    GLuint spotAmbientLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.ambientIntensity");
    GLuint spotDiffuseLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.diffuseIntensity");
    GLuint spotSpecularLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.specularIntensity");
    GLuint spotConeDirectionLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.coneDirection");
    GLuint spotConeAngleLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.coneAngle");
    GLuint spotConstantLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.constant");
    GLuint spotLinearLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.linear");
    GLuint spotQuadraticLocation = glGetUniformLocation(shader.GetProgramID(), "spot1.quadratic");

    glUniform3fv(spotPositionLocation, 1, glm::value_ptr(spotLight1.position));
    glUniform3fv(spotAmbientLocation, 1, glm::value_ptr(spotLight1.ambientIntensity));
    glUniform3fv(spotDiffuseLocation, 1, glm::value_ptr(spotLight1.diffuseIntensity));
    glUniform3fv(spotSpecularLocation, 1, glm::value_ptr(spotLight1.specularIntensity));
    glUniform3fv(spotConeDirectionLocation, 1, glm::value_ptr(spotLight1.coneDirection));
    glUniform1f(spotConeAngleLocation, spotLight1.coneAngle);
    glUniform1f(spotConstantLocation, spotLight1.constant);
    glUniform1f(spotLinearLocation, spotLight1.linear);
    glUniform1f(spotQuadraticLocation, spotLight1.quadratic);

    //getting locations for old light and camera, light uniforms not in use
    GLuint ambientLocation = glGetUniformLocation(shader.GetProgramID(), "ambientIntensity");
    GLuint diffuseLocation = glGetUniformLocation(shader.GetProgramID(), "diffuseIntensity");
    GLuint lightPosLocation = glGetUniformLocation(shader.GetProgramID(), "lightPosition");
    GLuint specIntensityLocation = glGetUniformLocation(shader.GetProgramID(), "specularIntensity");
    GLuint specColorLocation = glGetUniformLocation(shader.GetProgramID(), "specularColor");

    GLuint cameraPosLocation = glGetUniformLocation(shader.GetProgramID(), "cameraPosition");
    GLuint shininesLocation = glGetUniformLocation(shader.GetProgramID(), "shinines");
    GLuint colorTextureLocation = glGetUniformLocation(shader.GetProgramID(), "colourTexture");

    modelLocation = glGetUniformLocation(shader.GetProgramID(), "modelMatrix");
    viewLocation = glGetUniformLocation(shader.GetProgramID(), "viewMatrix");
    projLocation = glGetUniformLocation(shader.GetProgramID(), "projMatrix");


    
    //glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    


    //object creation-----------------------------------------------------------------------
    //creating/loading meshes
    Mesh* cube = new Cube();
    Mesh* planeObj = new ObjectFromFile("objs/plane.obj", glm::vec3(0, -2, 0), glm::vec3(0, 0, 0), glm::vec3(30, 1, 30));
    Mesh* cubeObj = new ObjectFromFile("objs/cube.obj", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    Mesh* cubeMap = new ObjectFromFile("objs/cube.obj", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(200, 200, 200));

    Mesh* mainLand = new ObjectFromFile("objs/mainLand.obj", glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Mesh* lizardObj = new ObjectFromFile("objs/lizard.obj", glm::vec3(-20, 16, -30), glm::vec3(5, 80, -18), glm::vec3(0.7f, 0.7f, 0.7f)); //https://free3d.com/3d-model/lizard-v1--31977.html
    Mesh* lavaFallObj = new ObjectFromFile("objs/lavaFall.obj", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Mesh* lavaPool1Obj = new ObjectFromFile("objs/lavaPool1.obj", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Mesh* lavaPool2Obj = new ObjectFromFile("objs/lavaPool2.obj", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Mesh* shrubObj = new ObjectFromFile("objs/Hazelnut.obj", glm::vec3(-50, 13, -60), glm::vec3(0, 0, 0), glm::vec3(1, 2, 1)); //https://free3d.com/3d-model/hazelnut-bush-290678.html
    Mesh* plane2 = new ObjectFromFile("objs/plane.obj", glm::vec3(0, 100, 0), glm::vec3(90, 0, 0), glm::vec3(1, 1, 1));
    Mesh* lizardObj2 = new ObjectFromFile("objs/lizard.obj", glm::vec3(-120, 2, -120), glm::vec3(0, 0, 0), glm::vec3(0.3, 0.3, 0.3));
    
    Texture* oldPaintUpArrowTexture = new Texture("textures/up arrow.png");
    Texture* paintUpArrowTexture = new Texture("textures/up arrow3.png");
    Texture* rockyGround = new Texture("textures/ground.png"); //https://ambientcg.com/view?id=Rocks008
    Texture* lava = new Texture("textures/lava.jpg"); //         https://ambientcg.com/view?id=Lava005
    Texture* bark = new Texture("textures/bark.jpg"); //         https://ambientcg.com/view?id=Bark012
    Texture* skin = new Texture("textures/skin.jpg"); //         https://ambientcg.com/view?id=Leather008
    Texture* splash = new Texture("textures/splashscreen.png");

    std::vector<GameObject*> objects;

    //creating gameobjects which hold a mesh and its texture
    GameObject* mainCube = new GameObject(cubeObj, paintUpArrowTexture);
    GameObject* plane = new GameObject(planeObj, oldPaintUpArrowTexture);
    GameObject* land = new GameObject(mainLand, rockyGround);
    GameObject* lavaPool1 = new GameObject(lavaPool1Obj, lava);
    GameObject* lavaPool2 = new GameObject(lavaPool2Obj, lava);
    GameObject* lavaFall = new GameObject(lavaFallObj, lava);
    GameObject* shrub = new GameObject(shrubObj, bark);
    GameObject* lizard = new GameObject(lizardObj, skin);
    GameObject* cubeRock = new GameObject(cube, rockyGround);
    GameObject* cubeRock2 = new GameObject(cube, rockyGround);
    GameObject* cubeRock3 = new GameObject(cube, rockyGround);
    GameObject* cubeRock4 = new GameObject(cube, rockyGround);
    GameObject* cubeRock5 = new GameObject(cube, rockyGround);
    GameObject* splashscreen = new GameObject(plane2, splash);
    GameObject* lizard2 = new GameObject(lizardObj2, skin);

    //putting all gameobjects in an array, called later to draw
    //objects.push_back(plane);
    //objects.push_back(mainCube);
    objects.push_back(land);
    objects.push_back(lavaPool1);
    objects.push_back(lizard);
    objects.push_back(lavaPool2);
    objects.push_back(lavaFall);
    objects.push_back(shrub);
    //objects.push_back(lizard2);
    
    //setting static variable, for drawing
    Mesh::modelLocation = modelLocation;

    //camera creation with the projection and view matrices
    camera = new Camera(projLocation, viewLocation);

    //basic model matrix
    glm::mat4 modelMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    
    //light input, not in use
    ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    diffuseIntensity = glm::vec3(0.8f, 0.8f, 0.8f);
    lightPosition = glm::vec3(2.0f, 3.0f, 2.0f);
    specularIntensity = glm::vec3(1.0f);
    specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    shinines = 256.0f;

    //sending values to the uniforms
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(ambientLocation, 1, glm::value_ptr(ambient));
    glUniform3fv(diffuseLocation, 1, glm::value_ptr(diffuseIntensity));
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPosition));
    glUniform3fv(specIntensityLocation, 1, glm::value_ptr(specularIntensity));
    glUniform3fv(specColorLocation, 1, glm::value_ptr(specularColor));
    glUniform3fv(cameraPosLocation, 1, glm::value_ptr(camera->cameraPosition));
    glUniform1f(shininesLocation, shinines);
    glUniform1i(colorTextureLocation, 0);

    
    //using skybox shader
    skyboxShader.UseShader();

    //sending uniforms to the new shader
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgramID(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform1i(glGetUniformLocation(skyboxShader.GetProgramID(), "skybox"), 1);
    
    //loading skybox into a texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    //textures
    stbi_set_flip_vertically_on_load(false);
    unsigned char* imageData = stbi_load("textures/skybox/front.jpg", &width, &height, &channel, 3);
    if (imageData == NULL) std::cout << "Texture not loaded." << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    
    imageData = stbi_load("textures/skybox/back.jpg", &width, &height, &channel, 3);
    if (imageData == NULL) std::cout << "Texture not loaded." << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    
    imageData = stbi_load("textures/skybox/bottom.jpg", &width, &height, &channel, 3);
    if (imageData == NULL) std::cout << "Texture not loaded." << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    
    imageData = stbi_load("textures/skybox/left.jpg", &width, &height, &channel, 3);
    if (imageData == NULL) std::cout << "Texture not loaded." << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    
    imageData = stbi_load("textures/skybox/right.jpg", &width, &height, &channel, 3);
    if (imageData == NULL) std::cout << "Texture not loaded." << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    
    imageData = stbi_load("textures/skybox/top.jpg", &width, &height, &channel, 3);
    if (imageData == NULL) std::cout << "Texture not loaded." << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    

    //depth testing for better rendering
    glEnable(GL_DEPTH_TEST);
    float depthValue = 100.0f;

    //for lizard animation
    float move = 0.0f;
    bool which = true;
    //main loop-------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        //delta tima calculation
        oldTime = currentTime;
        currentTime = glfwGetTime();
        
        double deltaTime = currentTime - oldTime;
        //std::cout << deltaTime << std::endl;

        //background color
        const float backgroundColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glClearBufferfv(GL_COLOR, 0, backgroundColour);
        glClearBufferfv(GL_DEPTH, 0, &depthValue);

        //check mouse/keyboard input, resizing
        glfwPollEvents();


        //main shader
        shader.UseShader();

        camera->UpdateCamera(deltaTime);

        //goes through every object in array, sets the texture and draws the mesh
        for (int i = 0; i < objects.size(); i++)
        {
           objects[i]->Draw();
        }

        //called individually to set the custom location and rotation
        cubeRock->Draw(glm::vec3(-125, 1, -135), glm::vec3(45,0,45), glm::vec3(1, 1, 1));
        cubeRock2->Draw(glm::vec3(-120, 1, -128), glm::vec3(89, 0, 45), glm::vec3(1, 2.1, 1));
        cubeRock3->Draw(glm::vec3(-127, 1, -121), glm::vec3(47, 0, 61), glm::vec3(1, 1, 1));
        cubeRock4->Draw(glm::vec3(-130, 1, -122), glm::vec3(234, 0, 43), glm::vec3(1, 1, 1.8));
        cubeRock5->Draw(glm::vec3(-125, 1, -130), glm::vec3(423, 0, 23), glm::vec3(1.5, 1, 1));

        //called individually to update the position for the animation
        lizard2->Draw(glm::vec3(-120 + move, 2, -120), glm::vec3(0, 0, 0), glm::vec3(0.3, 0.3, 0.3));

        //some logic for the animation
        if (which)
        {
            move += 4.0f * deltaTime;
            if (move >= 20.0f)
                which = false;
        }
        else
        {
            move -= 4.0f * deltaTime;
            if (move <= 0.0f)
                which = true;
        }

        //stops rendering the splashscreen if the player has moved
        if(!moved)
            splashscreen->Draw();


        //skybox shader
        skyboxShader.UseShader();
        
        //moves the skybox with the player to make it look better
        cubeMap->ChangePosition(camera->cameraPosition, glm::vec3(0, 0, 0), glm::vec3(200, 200, 200));

        //sending the matrices
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgramID(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(cubeMap->model));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgramID(), "viewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgramID(), "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera->projMatrix));
        
        //sets the cube map texture and draws it
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        
        cubeMap->Draw();
        

        glfwSwapBuffers(window);
    }
    //----------------------------------------------------------------------------------------------------

    //cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

