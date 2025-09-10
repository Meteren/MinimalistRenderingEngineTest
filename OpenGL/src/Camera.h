#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Window.h"

class Camera
{
private:
	float yaw;
	float pitch;

	glm::vec3 cameraPos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 worldUp;

	float moveSpeed;
	float turnSpeed;
	float changeX;
	float changeY;

	glm::mat4 view;

public:
	Camera();
	Camera(glm::vec3 cameraPos, glm::vec3 forward, glm::vec3 worldUp, float yaw, float pitch, float moveSpeed, float turnSpeed);

	void Update();
	void TransformCamera(bool* keys, float deltaTime);
	
	void createViewMatrix();

	glm::mat4 getViewMatrix();

	glm::vec3 getCameraPos();


	void setCameraRotation(Window* window,float changeX,float changeY);

	~Camera();

};

