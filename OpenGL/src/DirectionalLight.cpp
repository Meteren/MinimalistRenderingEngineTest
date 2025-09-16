#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(float directionalLightIntensity, float aLightIntensity, glm::vec3 ambientColor, glm::vec3 direction) 
	: Light(directionalLightIntensity, aLightIntensity, ambientColor)
{
	this->direction = direction;

}

void DirectionalLight::useLight(Shader shader,int i)
{
	glUniform3f(shader.uniformDirectionalLight.u_aColor, ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform1f(shader.uniformDirectionalLight.u_aIntensity, aLightIntensity);
	glUniform1f(shader.uniformDirectionalLight.u_directionalLightIntensity, directionalLightIntensity);
	glUniform3f(shader.uniformDirectionalLight.u_direction, direction.x, direction.y, direction.z);
	
}

DirectionalLight::~DirectionalLight()
{
}
