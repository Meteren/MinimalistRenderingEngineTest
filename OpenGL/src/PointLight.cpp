#include "PointLight.h"

PointLight::PointLight()
{
	exponent = 0;
	linear = 0;
	constant = 1;
	position = glm::vec3(0, 0, 0);
}

PointLight::PointLight(float mainLightIntensity, float aLightIntensity, glm::vec3 ambientColor, float exponent, 
	float linear, float constant, glm::vec3 position,int shadowMapWidth,int shadowMapHeight)
	: Light(mainLightIntensity,aLightIntensity,ambientColor,shadowMapWidth,shadowMapHeight)
{
	this->exponent = exponent;
	this->linear = linear;
	this->constant = constant;
	this->position = position;
}

void PointLight::useLight(Shader shader, int i)
{
	glUniform3f(shader.uniformPointLights[i].u_aColor, ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform1f(shader.uniformPointLights[i].u_aIntensity, aLightIntensity);
	glUniform1f(shader.uniformPointLights[i].u_directionalLightIntensity, directionalLightIntensity);
	glUniform3f(shader.uniformPointLights[i].u_position, position.x, position.y, position.z);
	glUniform1f(shader.uniformPointLights[i].u_exponent, exponent);
	glUniform1f(shader.uniformPointLights[i].u_linear, linear);
	glUniform1f(shader.uniformPointLights[i].u_constant, constant);

}

PointLight::~PointLight()
{
}
