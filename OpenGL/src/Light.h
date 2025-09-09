#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

class Light
{
private:
	float mainLightIntensity;
	float aLightIntensity;
	glm::vec3 ambientColor;
	glm::vec3 mainLightDir;

public:
	Light();

	Light(float aLightIntensity, float mainLightIntensity, glm::vec3 ambientColor, glm::vec3 mainLightDir);

	void setAmbientValues(int program, const char* aIntensity, const char* aColor);
	void setMainLightValues(int program, const char* intensity, const char* direction);

	glm::vec3 getAmbientColor();

	float getAmbientLightIntensity();

	
};

