#include "Light.h"

Light::Light() : aLightIntensity(0), directionalLightIntensity(0)
{
	ambientColor = glm::vec3(0, 0, 0);

}

Light::Light(float mainLightIntensity,float aLightIntensity, glm::vec3 ambientColor)
{
	this->aLightIntensity = aLightIntensity;
	this->ambientColor = ambientColor;
	this->directionalLightIntensity = mainLightIntensity;
}

void Light::useLight(Shader* shader) {
	return;
}

void Light::setAmbientValues(int program, const char* aIntensity, const char* aColor)
{
	int intensityLoc = glGetUniformLocation(program,aIntensity);
	int aColorLoc = glGetUniformLocation(program, aColor);

	glUniform1f(intensityLoc,aLightIntensity);
	glUniform3f(aColorLoc, ambientColor.x, ambientColor.y, ambientColor.z);

}

void Light::setMainLightValues(int program, const char* intensity, const char* direction)
{

	int intensityLoc = glGetUniformLocation(program, intensity);
	int directionLoc = glGetUniformLocation(program, direction);

	glUniform1f(intensityLoc, directionalLightIntensity);
}


glm::vec3 Light::getAmbientColor()
{
	return this->ambientColor;
}

float Light::getAmbientLightIntensity()
{
	return this->aLightIntensity;
}


Light::~Light()
{
}

