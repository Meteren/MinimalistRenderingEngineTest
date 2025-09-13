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
#include "Application.h"


float degreeToRad = 3.1415f / 180;

static float ColorVal() {
    float currentTime = (float)glfwGetTime();

    float value = cos(currentTime);

    return value;
}

static glm::mat4x4 ApplyTransform(float rVal, glm::vec3 positionToMove) {
    glm::mat4x4 model(1.0f);
    model = glm::translate(model, positionToMove);
    model = glm::rotate(model, rVal* degreeToRad, glm::vec3(0, 0, 1));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
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

void setNormals(float* verticeData,int stride,int indiceCount,int normalOffset,int verticeCount,unsigned int* indices) {

    for (int i = 0; i < indiceCount * verticeCount; i+=3) {
        int verticeStart = indices[i] * stride;
        glm::vec3 verticeFirst = glm::vec3(verticeData[verticeStart], verticeData[verticeStart + 1], verticeData[verticeStart + 2]);

        verticeStart = indices[i + 1] * stride;
        glm::vec3 verticeSecond = glm::vec3(verticeData[verticeStart], verticeData[verticeStart + 1], verticeData[verticeStart + 2]);

        verticeStart = indices[i + 2] * stride;
        glm::vec3 verticeThird = glm::vec3(verticeData[verticeStart], verticeData[verticeStart + 1], verticeData[verticeStart + 2]);

        glm::vec3 verticeDiffOne = verticeSecond - verticeFirst;
        glm::vec3 verticeDiffTwo = verticeThird - verticeFirst;

        glm::vec3 normal = glm::cross(verticeDiffOne,verticeDiffTwo);
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

void setVertexAttribPointers()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

int main(void)
{
    Window* window = new Window();

    Shader shader = Shader();

    DirectionalLight directionalLight = DirectionalLight(0.3f,0.2f,glm::vec3(1,1,1),glm::vec3(-2,1,-2));

    Material material = Material(1, 32);
    
    PointLight pointLights[MAX_POINT_LIGHT_COUNT];
     
    PointLight lightOne = PointLight(2.0f, 0.01f, glm::vec3(1, 0, 0), 0.01f, 0.03f, 1.0f,glm::vec3(-4.0f,-1.0f,0.0f));
    shader.pointLightCount++;
    PointLight lightTwo = PointLight(2.0f, 0.01f, glm::vec3(0, 0, 1), 0.01f, 0.03f, 1.0f, glm::vec3(4.0f, -1.0f, 0.0f));
    shader.pointLightCount++;

    pointLights[0] = lightOne;
    pointLights[1] = lightTwo;

    if (window->createWindow(1366, 780) == 1)
        return 1;

    if (glewInit() != GLEW_OK) {
        std::cout << "Error occured!" << std::endl;
    }

    glViewport(0,0, window->fbWidth,window->fbHeight);

    /*const char* vShaderData = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "out vec4 vertex_color;\n"
        "void main(){\n"
        "gl_Position =  projection * view * model * vec4(position,1);\n"
        "vertex_color = vec4(clamp(position,0.0f,1.0f),1);\n"
        "}";*/
    
    const char* vShaderData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/vertex.shader");

    /*const char* fShaderData = "#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 u_color;\n"
        "in vec4 vertex_color;\n"
        "void main(){\n"
        "color = vertex_color;\n"
        "}";*/

    const char* fShaderData = Shader::readShader("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/fragment.shader");

    glEnable(GL_DEPTH_TEST);

    /*float vertices[] =
    {
        0.0f, 1.0f, 0.0f,   0.5f,1.0f,   0.0f,1.0f,0.0f,//0
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  -1.0f,0.0f,0.0f,//1
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  1.0f,0.0f,0.0f,//2
        0.0f,-1.0f,1.0f,    0.5f,0.0f,   0.0f,0.0f,1.0f//3
    };*/

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

    /*for (int i = 0; i < 12; i += 3) {
        printf("%f ** ", vertices[i + 5]);
        printf("%f ** ", vertices[i + 6]);
        printf("%f\n", vertices[i + 7]);
    }*/

    VertexBuffer vb(vertices, sizeof(vertices));
    vb.CreateElementBufferObject(indices, sizeof(indices));


    shader.createShaderProgram(vShaderData, fShaderData);

    unsigned int shaderProgram = shader.getProgram();

    delete[] vShaderData;
    delete[] fShaderData;

    //locations
    int col_loc = glGetUniformLocation(shaderProgram, "u_color");
    int model_loc = shader.getModelLoc();
    int view_loc = shader.getViewLoc();
    int projection_loc = shader.getProjectionLoc();
    int camPos_loc = glGetUniformLocation(shaderProgram, "camPos");

    setVertexAttribPointers();

    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(45.0f), (float)window->fbWidth / (float)window->fbHeight, 0.01f, 100.0f);

    Camera camera = Camera(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.0f,1.0f,0.0f),-90.0f, 0.0f, 5.0f, 0.1f);

    vb.UnBindVAO();
    //vbPlane.UnBindVAO();
    VertexBuffer vbPlane(verticesPlane, sizeof(verticesPlane));
    vbPlane.CreateElementBufferObject(indicesPlane, sizeof(indicesPlane));

    setVertexAttribPointers();

    vbPlane.UnBindVAO();

    Texture mainTexture = Texture("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Textures/brick.png");
    mainTexture.loadTexture();

    float lastTime = 0;

    if (shader.pointLightCount > MAX_POINT_LIGHT_COUNT) shader.pointLightCount = MAX_POINT_LIGHT_COUNT;

    printf(" ++ Point Light Count:%d ++", shader.pointLightCount);

    /* Loop until the user closes the window */
    while (!window->shouldWindowClosed())
    {
        float currTime = (float)glfwGetTime();

        float deltaTime = currTime - lastTime;

        lastTime = currTime;
        /* Render here */
        //glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mainTexture.useTexture();
        glUseProgram(shaderProgram);
        
        float col_val = ColorVal();

        glUniform4f(col_loc, 0.0f, abs(col_val), 0.0f, 1.0f);
        glUniform1i(shader.getPointLightCountLoc(), shader.pointLightCount);

        glUniformMatrix4fv(model_loc,1, GL_FALSE, glm::value_ptr(ApplyTransform(0,glm::vec3(0,0,-2.5f))));

        directionalLight.useLight(shader);

        //the point light loop  
        for (int i = 0; i < shader.pointLightCount; i++) {
            pointLights[i].useLight(shader,i);
        }
        //-----

        camera.TransformCamera(window->getKeys(),deltaTime);
        glUniform3f(camPos_loc, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);

        camera.setCameraRotation(window,window->getChangeX(), window->getChangeY());
        camera.createViewMatrix();
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
        glUniformMatrix4fv(projection_loc,1, GL_FALSE, glm::value_ptr(projection));

        
        material.setMaterial(shaderProgram, "material.metallic", "material.smoothness");

        //glDrawArrays(GL_TRIANGLES,0,3);
        vb.render();

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(ApplyTransform(0, glm::vec3(0,-2.0f,0.0f))));

        vbPlane.render();

        glUseProgram(0);
   
        /* Swap front and back buffers */
        glfwSwapBuffers(window->getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}


