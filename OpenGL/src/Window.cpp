#include "Window.h"

#include <stdio.h>

Window::Window()
	: fbWidth(0), fbHeight(0), lastPosX(0), lastPosY(0),
	changeX(0.0f),changeY(0.0f),initStart(true), window(nullptr)
{
	for (int i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int Window::createWindow(int width, int height)
{

	if (!glfwInit()) {
		printf("An error occured while initting the window.");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(width, height, "Main Window", NULL, NULL);

	if (!window) {
		Clear();
		return 1;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	glfwSetWindowUserPointer(window, this);

	glfwSetCursorPosCallback(window, handleMouseMovement);

	glfwSetKeyCallback(window, getKeyboardInput);

	glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return 0;
}

bool Window::shouldWindowClosed()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* Window::getWindow()
{
	return window;
}

void Window::getKeyboardInput(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	if (action == GLFW_PRESS) {
		p_window->keys[key] = true;
		//printf("Tbe button pressed: %d\n", key);
	}

	else if (action == GLFW_RELEASE) {
		p_window->keys[key] = false;
		//printf("Tbe button released: %d\n", key);
	}
	
}

void Window::handleMouseMovement(GLFWwindow* window, double xPos, double yPos)
{
	Window* p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));


	if (p_window->initStart) {

		p_window->lastPosX = (float)xPos;
		p_window->lastPosY = (float)yPos;

		p_window->initStart = false;
	}

	p_window->changeX =(float) xPos - p_window->lastPosX;
	p_window->changeY = p_window->lastPosY - (float) yPos;

	p_window->lastPosX = (float)xPos;
	p_window->lastPosY = (float)yPos;

	//printf("xChange:%f ---", p_window->changeX);
	//printf("yChange:%f\n", p_window->changeY);

}

Window::~Window()
{
	Clear();
}
