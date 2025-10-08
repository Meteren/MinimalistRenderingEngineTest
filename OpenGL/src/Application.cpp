
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexBuffer.h"

#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <cmath>

#include <stdio.h>

#include "Window.h"
#include "Camera.h"
#include <fstream>
#include <string>

#include "Texture.h"

#include "DirectionalLight.h"

#include "Material.h"

#include "Shader.h"

#include "PointLight.h"

#include "SpotLight.h"

#include "Model.h"

#include <vector>


std::vector<Model*> models;
std::vector<VertexBuffer*> meshes;

std::vector<Shader*> shaders;

std::vector<Texture*> textures;

std::vector<Material*> materials;

float degreeToRad = 3.1415f / 180;
float lastTime = 0;

Shader mainShader = Shader();

Shader dDepthShader = Shader();

Shader oDepthShader = Shader();


static float ColorVal() {
    float currentTime = (float)glfwGetTime();

    float value = cos(currentTime);

    return value;
}

static glm::mat4x4 ApplyTransform(float rVal,glm::vec3 rotationAxis, glm::vec3 positionToMove, glm::vec3 scaleVector) {
    glm::mat4x4 model(1.0f);
    model = glm::translate(model, positionToMove);
    model = glm::rotate(model, rVal * degreeToRad, rotationAxis);
    model = glm::scale(model, scaleVector);
    return model;
}

float incrementVal = 0.02f;
float curVal = 0;

static float Angle() {
    curVal += incrementVal;

    if (curVal > 360)
        curVal = 0;
    return curVal;
}

void setNormals(float* verticeData, int stride, int indiceCount, int normalOffset, int verticeCount, unsigned int* indices) {

    for (int i = 0; i < indiceCount * verticeCount; i += 3) {
        int verticeStart = indices[i] * stride;
        glm::vec3 verticeFirst = glm::vec3(verticeData[verticeStart], verticeData[verticeStart + 1], verticeData[verticeStart + 2]);

        verticeStart = indices[i + 1] * stride;
        glm::vec3 verticeSecond = glm::vec3(verticeData[verticeStart], verticeData[verticeStart + 1], verticeData[verticeStart + 2]);

        verticeStart = indices[i + 2] * stride;
        glm::vec3 verticeThird = glm::vec3(verticeData[verticeStart], verticeData[verticeStart + 1], verticeData[verticeStart + 2]);

        glm::vec3 verticeDiffOne = verticeSecond - verticeFirst;
        glm::vec3 verticeDiffTwo = verticeThird - verticeFirst;

        glm::vec3 normal = glm::cross(verticeDiffOne, verticeDiffTwo);
        normal = glm::normalize(normal);

        for (int j = i; j < i + 3; j++) {
            int normalStart = indices[j] * stride + normalOffset;
            verticeData[normalStart] += normal.x;
            verticeData[normalStart + 1] += normal.y;
            verticeData[normalStart + 2] += normal.z;
        }
    }
    for (int i = 0; i < indiceCount; i++) {
        int nOffset = i * stride + normalOffset;

        glm::vec3 normal(verticeData[nOffset], verticeData[nOffset + 1], verticeData[nOffset + 2]);

        normal = glm::normalize(normal);

        verticeData[nOffset] = normal.x;
        verticeData[nOffset + 1] = normal.y;
        verticeData[nOffset + 2] = normal.z;

    }

}

void renderObjects(Shader* shader) {

    glUniformMatrix4fv(shader->getModelLoc(), 1, GL_FALSE, glm::value_ptr(ApplyTransform(0,glm::vec3(0,1,0), glm::vec3(0, 2.5f, 0), glm::vec3(1, 1, 1))));

    meshes[0]->render();

    glUniformMatrix4fv(shader->getModelLoc(), 1, GL_FALSE, glm::value_ptr(ApplyTransform(0, glm::vec3(0, 1, 0), glm::vec3(0, -2.0f, 0.0f), glm::vec3(1, 1, 1))));

    meshes[1]->render();

    glUniformMatrix4fv(shader->getModelLoc(), 1, GL_FALSE, glm::value_ptr(ApplyTransform(0, glm::vec3(0, 1, 0), glm::vec3(-9, 3, 13), glm::vec3(0.009f, 0.009f, 0.009f))));

    models[1]->renderModel(*shader);

    glUniformMatrix4fv(shader->getModelLoc(), 1, GL_FALSE, glm::value_ptr(ApplyTransform(90.0f, glm::vec3(0, 1, 0), glm::vec3(0, -2, -40), glm::vec3(1, 1, 1))));

    models[0]->renderModel(*shader);
}

void dShadowMapRenderPass(DirectionalLight* light) {

    shaders[1]->useProgram();

    glViewport(0, 0, light->getShadowMapWidth(), light->getShadowMapHeight());

    light->getShadowMap()->writeBuffer();
    glClear(GL_DEPTH_BUFFER_BIT);

    light->attachDepthElement(shaders[1]);

    renderObjects(shaders[1]);

    glUseProgram(0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void oDShadowMapPass(PointLight* pointLight) {
    
    shaders[2]->useProgram();

    glViewport(0, 0, pointLight->getShadowMapWidth(), pointLight->getShadowMapHeight());

    pointLight->getShadowMap()->writeBuffer();

    glClear(GL_DEPTH_BUFFER_BIT);

    pointLight->attachLightTransforms(*shaders[2]);
    
    pointLight->attachLightPosition(*shaders[2]);

    pointLight->attachFarPlane(*shaders[2]);

    renderObjects(shaders[2]);

    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void mainRenderPass(glm::mat4 projection, DirectionalLight* directionalLight, 
    PointLight** pointLights, SpotLight** spotLights, Camera &camera, Window* window) {

    float currTime = (float)glfwGetTime();

    float deltaTime = currTime - lastTime;

    lastTime = currTime;
    shaders[0]->useProgram();

    glViewport(0, 0, window->fbWidth, window->fbHeight);
    glClearColor(0.2, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    textures[0]->useTexture(*shaders[0], GL_TEXTURE1);

    glUniform1i(shaders[0]->getPointLightCountLoc(), shaders[0]->pointLightCount);
    glUniform1i(shaders[0]->getSpotLightCountLoc(), shaders[0]->spotLightCount);

    directionalLight->attachDepthElement(shaders[0]);

    directionalLight->attachdShadowMap(*shaders[0]);

    directionalLight->useLight(*shaders[0], 0);
 
    for (int i = 0; i < shaders[0]->pointLightCount; i++) {
        pointLights[i]->useLight(*shaders[0], i);
        pointLights[i]->attachShadowMap(*shaders[0],GL_TEXTURE3 + i, 3 + i,i);
    }

    spotLights[1]->setCondition(window);

    spotLights[1]->setPosition(camera.getPos()
        + glm::vec3(camera.getDir().x * 0.5, camera.getDir().y * 0.5, camera.getDir().z * 0.5)
        + glm::vec3(0, -0.2f, 0));
    spotLights[1]->setDirection(camera.getDir());

    //spot light loop
    for (int i = 0; i < shaders[0]->spotLightCount; i++) {
        spotLights[i]->useLight(*shaders[0], i);
        spotLights[i]->attachShadowMap(*shaders[0], GL_TEXTURE3 + 
         shaders[0]->pointLightCount + i, 3  + shaders[0]->pointLightCount + i, i + shaders[0]->pointLightCount);
    }
    //----
    camera.TransformCamera(window->getKeys(), deltaTime);
    glUniform3f(shaders[0]->getCamPosLoc(), camera.getPos().x, camera.getPos().y, camera.getPos().z);

    camera.setCameraRotation(window, window->getChangeX(), window->getChangeY());
    camera.createViewMatrix();

    glUniformMatrix4fv(shaders[0]->getViewLoc(), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(shaders[0]->getProjectionLoc(), 1, GL_FALSE, glm::value_ptr(projection));

    materials[0]->setMaterial(shaders[0]->getProgram(), "material.metallic", "material.smoothness");

    renderObjects(shaders[0]);

    glUseProgram(0);
}

int main(void)
{
    shaders.push_back(&mainShader);
    shaders.push_back(&dDepthShader);
    shaders.push_back(&oDepthShader);

    Window* window = new Window();

    if (window->createWindow(1366, 780) == 1)
        return 1;

    if (glewInit() != GLEW_OK) {
        std::cout << "Error occured!" << std::endl;
    }

    DirectionalLight directionalLight = DirectionalLight(0.3f, 0.1f, glm::vec3(1, 1, 1), glm::vec3(-500, -500, 500), 8192, 8192);

    Material material = Material(1, 32);

    materials.push_back(&material);

    PointLight* pointLights[MAX_POINT_LIGHT_COUNT];

    SpotLight* spotLights[MAX_SPOT_LIGHT_COUNT];

    //point light creating
    PointLight pointLightOne = PointLight(2.0f, 0.1f, glm::vec3(1, 0, 0), 0.01f, 0.03f, 1.0f, glm::vec3(-10.0f, 4.0f, 0.0f), 1024, 1024,100.0f);
    shaders[0]->pointLightCount++;

    PointLight pointLightTwo = PointLight(2.0f, 0.1f, glm::vec3(0, 0, 1), 0.01f, 0.03f, 1.0f, glm::vec3(4.0f, 8.0f, 0.0f), 1024, 1024,100.0f);
    shaders[0]->pointLightCount++;

    pointLights[0] = &pointLightOne;
    pointLights[1] = &pointLightTwo;

    //spot light creation

    SpotLight spotLightOne =
        SpotLight(2.0f, 0.01f, glm::vec3(1, 1, 1), 0.01f, 0.03f, 1.0f, glm::vec3(0, 2, 0), 60.0f, glm::vec3(0, -2, 0),1024,1024,100.0f);
    shaders[0]->spotLightCount++;

    SpotLight spotLightTwo =
        SpotLight(3.0f, 0.01f, glm::vec3(1, 0, 1), 0.01f, 0.03f, 1.0f, glm::vec3(5, -1, 0), 30.0f, glm::vec3(1, 0, 0), 1024, 1024, 100.0f);
    shaders[0]->spotLightCount++;

    SpotLight spotLightThree =
        SpotLight(3.0f, 0.01f, glm::vec3(0, 1, 0), 0.01f, 0.03f, 1.0f, glm::vec3(0, 0, 9), 60.0f, glm::vec3(0, -1, -10), 1024, 1024, 100.0f);
    shaders[0]->spotLightCount++;

    spotLights[0] = &spotLightOne;
    spotLights[1] = &spotLightTwo;
    spotLights[2] = &spotLightThree;

    glViewport(0, 0, window->fbWidth, window->fbHeight);


    //this section will be corrected later to make file paths work in every enviorenment
    //main shader
    const char* vShaderData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Shaders/vertex.shader");
    const char* fShaderData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Shaders/fragment.shader");
    //--

    //depth shader locations for directional light
    const char* dDepthShaderVData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Shaders/directionalSMv.shader");
    const char* dDepthShaderFData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Shaders/directionalSMf.shader");
    //---

    //omnidirectional depth shader locations for point and spot light
    const char* oDirectionalDepthShaderVData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Shaders/omniDirectionalSMv.shader");
    const char* oDirectionalDepthShaderGData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Shaders/omniDirectionalSMg.shader");
    const char* oDirectionalDepthShaderFata = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Shaders/omniDirectionalSMf.shader");

    glEnable(GL_DEPTH_TEST);

    float vertices[] =
    {
        0.0f, 1.0f, 0.0f,   0.5f,1.0f,   0.0f,0.0f,0.0f,//0
        -1.0f, -1.0f, -0.6f, 0.0f, 0.0f,  0.0f,0.0f,0.0f,//1
        1.0f, -1.0f, -0.6f,  1.0f, 0.0f,  0.0f,0.0f,0.0f,//2
        0.0f,-1.0f,1.0f,    0.5f,0.0f,   0.0f,0.0f,0.0f//3
    };

    unsigned int indices[] =
    {
        0,1,2,   // front
        0,3,1,   // left  (swapped 1,3)
        0,2,3,   // right
        1,3,2    // bottom (swapped 2,3)
    };

    float verticesPlane[] = {
        -10.0f,0.0f,-10.0f,   0.0f,10.0f,   0.0f,-1.0f,0.0f, //0
        10.0f, 0.0f, -10.0f,  10.0f, 10.0f,  0.0f,-1.0f,0.0f, //1
        -10.0f, 0.0f, 10.0f,  0.0f, 0.0f,    0.0f,-1.0f,0.0f, //2
        10.0f, 0.0f, 10.0f,   10.0f, 0.0f,  0.0f,-1.0f,0.0f //3
    };

    unsigned int indicesPlane[] = {
        1,0,2,
        1,2,3
    };

    setNormals(vertices, 8, 4, 5, 3, indices);

    VertexBuffer vb(vertices, indices, sizeof(vertices) / sizeof(float), sizeof(indices) / sizeof(unsigned int));
    meshes.push_back(&vb);

    mainShader.createShaderProgram(vShaderData, fShaderData);
    dDepthShader.createShaderProgram(dDepthShaderVData, dDepthShaderFData);
    oDepthShader.createShaderProgram(oDirectionalDepthShaderVData, oDirectionalDepthShaderGData, oDirectionalDepthShaderFata);

    delete[] vShaderData;
    delete[] fShaderData;
    delete[] dDepthShaderVData;
    delete[] dDepthShaderFData;
    delete[] oDirectionalDepthShaderVData;
    delete[] oDirectionalDepthShaderGData;
    delete[] oDirectionalDepthShaderFata;

    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(45.0f), (float)window->fbWidth / (float)window->fbHeight, 0.01f, 100.0f);

    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

    vb.UnBindVAO();

    VertexBuffer vbPlane(verticesPlane, indicesPlane, sizeof(verticesPlane) / sizeof(float), sizeof(indicesPlane) / sizeof(unsigned int));
    meshes.push_back(&vbPlane);

    vbPlane.UnBindVAO();

    Texture mainTexture = Texture("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Textures/brick.png");
    mainTexture.loadTexture();
    textures.push_back(&mainTexture);

    if (mainShader.pointLightCount > MAX_POINT_LIGHT_COUNT) mainShader.pointLightCount = MAX_POINT_LIGHT_COUNT;
    if (mainShader.spotLightCount > MAX_SPOT_LIGHT_COUNT) mainShader.spotLightCount = MAX_POINT_LIGHT_COUNT;

    printf(" ++ Point Light Count:%d ++", mainShader.pointLightCount);
    printf(" ++ Spot Light Count:%d ++", mainShader.spotLightCount);

    Model modelHouse = Model("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Models/cottage_obj.obj");
    models.push_back(&modelHouse);
    modelHouse.loadModel();

    Model modelXwing = Model("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Models/x-wing.obj");
    models.push_back(&modelXwing);
    modelXwing.loadModel();

    while (!window->shouldWindowClosed())
    {
        dShadowMapRenderPass(&directionalLight);

        for (int i = 0; i < shaders[0]->pointLightCount + shaders[0]->spotLightCount; i++) {
            if (i < shaders[0]->pointLightCount)
                oDShadowMapPass(pointLights[i]);
            else
                oDShadowMapPass(spotLights[i - shaders[0]->pointLightCount]);
        }

        mainRenderPass(projection, &directionalLight, pointLights, spotLights, camera, window);

        glfwSwapBuffers(window->getWindow());

        glfwPollEvents();

    }

    delete window;

    return 0;
}


