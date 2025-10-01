#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(), ortho(glm::mat4()), dLightTranfsorm(glm::mat4())
{

}

DirectionalLight::DirectionalLight(float directionalLightIntensity, float aLightIntensity, glm::vec3 ambientColor, glm::vec3 direction, int shadowMapWidth, int shadowMapHeight)
	: Light(directionalLightIntensity, aLightIntensity, ambientColor,shadowMapWidth,shadowMapHeight)
{
	this->direction = direction;

	glm::mat4 view = glm::lookAt(-direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	ortho = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 20.0f);

	this->dLightTranfsorm = ortho * view;

	shadowMap = new DirectionalShadowMap(shadowMapWidth, shadowMapHeight);

	if (shadowMap->init()){}
	else
	{
		delete shadowMap;
		shadowMap = nullptr;
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
	return ortho * view;
}

glm::mat4 DirectionalLight::getLightTransform() const
{
	return dLightTranfsorm;
}

void DirectionalLight::attachdShadowMap(Shader shader)
{
	shadowMap->readBuffer(GL_TEXTURE1);
	glUniform1i(shader.getdShadowMapLoc(), 1);
}

void DirectionalLight::attachDepthElements(Shader shader, glm::mat4 model)
{
	glUniformMatrix4fv(shader.getModelLoc(),0,GL_FALSE,glm::value_ptr(model));
	glUniformMatrix4fv(shader.getdLightTransformLoc(),0,GL_FALSE,glm::value_ptr(dLightTranfsorm));
}

DirectionalLight::~DirectionalLight()
{
}
