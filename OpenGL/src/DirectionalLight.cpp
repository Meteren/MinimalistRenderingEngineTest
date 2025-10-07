#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() //ortho(glm::mat4()), dLightTransform(glm::mat4())
{

}

DirectionalLight::DirectionalLight(float directionalLightIntensity, float aLightIntensity, glm::vec3 ambientColor, glm::vec3 direction, int shadowMapWidth, int shadowMapHeight)
	: Light(directionalLightIntensity, aLightIntensity, ambientColor,shadowMapWidth,shadowMapHeight)
{
	this->direction = direction;

	glm::mat4 view = glm::lookAt(-direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	ortho = glm::ortho(-100.0f,100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);

	this->dLightTransform = ortho * view;

	shadowMap = new DirectionalShadowMap(shadowMapWidth, shadowMapHeight);

	if (shadowMap->init()){}
	else
	{
		printf("Directional shadow mapping failure!\n");
		
	}

}

void DirectionalLight::useLight(Shader shader,int i)
{
	glUniform3f(shader.uniformDirectionalLight.u_aColor, ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform1f(shader.uniformDirectionalLight.u_aIntensity, aLightIntensity);
	glUniform1f(shader.uniformDirectionalLight.u_directionalLightIntensity, directionalLightIntensity);
	glUniform3f(shader.uniformDirectionalLight.u_direction, direction.x, direction.y, direction.z);
	
}

glm::mat4 DirectionalLight::calculateLightTransform()
{
	glm::mat4 view = glm::lookAt(-direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	dLightTransform = ortho * view;
	return dLightTransform;
}

glm::mat4 DirectionalLight::getLightTransform() const
{
	return dLightTransform;
}

void DirectionalLight::attachdShadowMap(Shader shader)
{
	shadowMap->readBuffer(GL_TEXTURE2);
	glUniform1i(shader.getdShadowMapLoc(), 2);
}

void DirectionalLight::attachDepthElement(Shader* shader)
{
	glUniformMatrix4fv(shader->getdLightTransformLoc(), 1, GL_FALSE, glm::value_ptr(calculateLightTransform()));
}

DirectionalLight::~DirectionalLight()
{

}
