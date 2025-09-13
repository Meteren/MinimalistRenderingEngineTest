#pragma once
#include "Light.h"

class PointLight : public Light
{
private:

	float exponent;
	float linear;
	float constant;

	glm::vec3 position;

public:
	PointLight();
	PointLight(float mainLightIntensity, float aLightIntensity, glm::vec3 ambientColor, float exponent, float linear, float constant, glm::vec3 position);


	void useLight(Shader shader, int pointLightIndex);


	~PointLight();


};

