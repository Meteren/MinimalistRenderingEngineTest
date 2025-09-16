#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>
#include "Shader.h"

class Light
{
protected:
	float directionalLightIntensity;
	float aLightIntensity;
	glm::vec3 ambientColor;

public:
	Light();
	
	Light(float mainLightIntensity, float aLightIntensity, glm::vec3 ambientColor);

	virtual void useLight(Shader shader, int i);

	glm::vec3 getAmbientColor();

	float getAmbientLightIntensity();

	~Light();
	
};

