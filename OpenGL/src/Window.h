#pragma once 

#include <GLFW/glfw3.h>

class Window
{
private:
	
	GLFWwindow* window;
	float lastPosX;
	float lastPosY;
	float changeX;
	float changeY;
	bool initStart;
	bool keys[1024];

public:

	int fbWidth, fbHeight;

	bool* getKeys(){
		return keys;
	}

	Window();

	int createWindow(int width, int height);

	bool shouldWindowClosed();

	GLFWwindow* getWindow();

	float getChangeX() { return changeX; }
	float getChangeY() { return changeY; }

	static void getKeyboardInput(GLFWwindow* window, int key, int code, int action, int mode);

	static void handleMouseMovement(GLFWwindow* window, double xPos, double yPos);

	void resetChangeValues() {
		changeX = 0;
		changeY = 0;
	}

	void Clear() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	
	~Window();

};

