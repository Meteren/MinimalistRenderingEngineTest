#include "Light.h"

Light::Light() : aLightIntensity(0), directionalLightIntensity(0), shadowMap(nullptr)
{
	ambientColor = glm::vec3(0, 0, 0);

}

Light::Light(float mainLightIntensity,float aLightIntensity, glm::vec3 ambientColor,int shadowMapWidth, int shadowMapHeight)
{
	this->aLightIntensity = aLightIntensity;
	this->ambientColor = ambientColor;
	this->directionalLightIntensity = mainLightIntensity;
	this->shadowMapWidth = shadowMapWidth;
	this->shadowMapHeight = shadowMapHeight;
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

ShadowMap* Light::getShadowMap() const
{
	return this->shadowMap;
}


Light::~Light()
{
	if (shadowMap) {
		delete shadowMap;
		shadowMap = nullptr;
	}
		
}

