#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 forward, glm::vec3 worldUp, float yaw, float pitch, float moveSpeed, float turnSpeed)
	: view(glm::mat4()), right(glm::vec3()), up(glm::vec3())
{
	this->cameraPos = cameraPos;
	this->forward = forward;
	this->worldUp = worldUp;
	this->yaw = yaw;
	this->pitch = pitch;
	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;

	Update();
}

void Camera::Update()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

}

void Camera::TransformCamera(bool* keys,float deltaTime)
{

	float value = deltaTime * moveSpeed;

	if (keys[GLFW_KEY_W]) {
		cameraPos += forward * value;
	}
	if (keys[GLFW_KEY_S]) {
		cameraPos += forward * -value;
	}
	if (keys[GLFW_KEY_D]) {
		cameraPos += right * value;
	}
	if (keys[GLFW_KEY_A]) {
		cameraPos += right * -value;
	}
	if (keys[GLFW_KEY_SPACE]) {
		cameraPos += glm::vec3(0, 1, 0) * value;
	}
	if (keys[GLFW_KEY_LEFT_CONTROL]) {
		cameraPos += glm::vec3(0, 1, 0) * -value;
	}
}

void Camera::createViewMatrix()
{
	view = glm::lookAt(cameraPos, cameraPos + forward, worldUp);
}

glm::mat4 Camera::getViewMatrix()
{
	return view;
}

void Camera::setCameraRotation(Window* window,float changeX, float changeY)
{
	this->changeX = changeX;
	this->changeY = changeY;
	yaw += this->changeX * turnSpeed;
	pitch += this->changeY * turnSpeed;

	if (pitch >= 89.0f) {
		pitch = 89.0f;
	}
    if (pitch <= -89.0f) {
		pitch = -89.0f;
	}

	window->resetChangeValues();
	
	Update();
}

Camera::~Camera()
{
}
