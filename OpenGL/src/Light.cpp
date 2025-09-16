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


void Light::useLight(Shader shader, int i) {
	return;
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

