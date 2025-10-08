#include "PointLight.h"

PointLight::PointLight()
{
	exponent = 0;
	linear = 0;
	constant = 1;
	position = glm::vec3(0, 0, 0);
	farPlane = 0;
	lightPerspective = glm::mat4(1.0f);
	
}

PointLight::PointLight(float mainLightIntensity, float aLightIntensity, glm::vec3 ambientColor, float exponent, 
	float linear, float constant, glm::vec3 position,int shadowMapWidth,int shadowMapHeight,float farPlane)
	: Light(mainLightIntensity,aLightIntensity,ambientColor,shadowMapWidth,shadowMapHeight)
{
	this->exponent = exponent;
	this->linear = linear;
	this->constant = constant;
	this->position = position;

	this->farPlane = farPlane;

	lightPerspective = glm::perspective(glm::radians(90.0f), (float)shadowMapWidth / (float)shadowMapHeight, 0.01f, farPlane);

	shadowMap = new OmniDirectionalShadowMap(shadowMapWidth, shadowMapHeight);

	if (shadowMap->init()){}
	else {
		printf("Omnidirectional shadow mapping failure!\n");
		delete shadowMap;
		shadowMap = nullptr;
	}

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

void PointLight::attachShadowMap(Shader& shader, unsigned int unit, unsigned int unitValue, unsigned int lightIndex)
{
	shadowMap->readBuffer(GL_TEXTURE0 + unit + lightIndex);
	glUniform1i(shader.u_oDShadowMap[lightIndex].u_oDShadowMap, unitValue);
	glUniform1f(shader.u_oDShadowMap[lightIndex].u_farPlane, farPlane);

}

void PointLight::attachLightTransforms(Shader& shader)
{
	std::vector<glm::mat4> lightTransforms = calculateLightTransforms();
	for (int i = 0; i < lightTransforms.size(); i++) {
		glUniformMatrix4fv(shader.u_odLightTransformMatrixLocs[i],1, GL_FALSE,glm::value_ptr(lightTransforms[i]));
	}
}

void PointLight::attachLightPosition(Shader& shader)
{
	glUniform3f(shader.getPointLightPosLoc(), position.x, position.y, position.z);
}

void PointLight::attachFarPlane(Shader& shader)
{
	glUniform1f(shader.getFarPlaneLoc(), farPlane);
}

std::vector<glm::mat4> PointLight::calculateLightTransforms()
{
	std::vector<glm::mat4> lightTransforms;
	lightTransforms.push_back(lightPerspective * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightPerspective * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightPerspective * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightTransforms.push_back(lightPerspective * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	lightTransforms.push_back(lightPerspective * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightPerspective * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	return lightTransforms;
}

PointLight::~PointLight()
{
}
