#include "SpotLight.h"


SpotLight::SpotLight() : PointLight(), cutOff(0), procCutOff(0), direction(glm::vec3(0, 0, 0)), active(true),holdingKey(false) {}


SpotLight::SpotLight(float mainLightIntensity, float aLightIntensity, glm::vec3 ambientColor, 
	float exponent, float linear, float constant, glm::vec3 position, float cutOff, glm::vec3 direction,int shadowMapWidth, int shadowMapHeight,float farPlane) 
	: PointLight(mainLightIntensity,aLightIntensity,ambientColor,exponent,linear,constant,position,shadowMapWidth,shadowMapHeight,farPlane)
{
	this->cutOff = cutOff;
	this->direction = direction;
	this->procCutOff = cosf(glm::radians(this->cutOff));
	this->active = true;
}

void SpotLight::useLight(Shader shader, int i)
{

	glUniform3f(shader.uniformSpotLights[i].u_aColor, ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform1f(shader.uniformSpotLights[i].u_aIntensity, aLightIntensity);
	glUniform1f(shader.uniformSpotLights[i].u_directionalLightIntensity, active ? directionalLightIntensity : 0);
	glUniform3f(shader.uniformSpotLights[i].u_position, position.x, position.y, position.z);
	glUniform1f(shader.uniformSpotLights[i].u_exponent, exponent);
	glUniform1f(shader.uniformSpotLights[i].u_linear, linear);
	glUniform1f(shader.uniformSpotLights[i].u_constant, constant);
	glUniform1f(shader.uniformSpotLights[i].u_cutOff, procCutOff);
	glUniform3f(shader.uniformSpotLights[i].u_direction, direction.x, direction.y, direction.z);

	
}

void SpotLight::setPosition(glm::vec3 newPos) 
{
	position = newPos;
}

void SpotLight::setDirection(glm::vec3 newDir) 
{
	direction = newDir;
}

glm::vec3 SpotLight::getPosition() const {
	return position;
}

glm::vec3 SpotLight::getDirection() const {
	return direction;
}

void SpotLight::setCondition(Window* window)
{
	if ((glfwGetKey(window->getWindow(), GLFW_KEY_F) == GLFW_PRESS) && !holdingKey) {
		if (!active)
			active = true;
		else
			active = false;
		holdingKey = true;
		printf("set\n");
	}
	else if (glfwGetKey(window->getWindow(), GLFW_KEY_F) == GLFW_RELEASE){
		holdingKey = false;
	}

}

bool SpotLight::getSpotLightStatus() const
{
	return active;
}


SpotLight::~SpotLight()
{

}
