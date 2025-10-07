#pragma once
#include "Light.h"
#include "OmniDirectionalShadowMap.h"
#include <vector>

class PointLight : public Light
{
protected:
	float exponent;
	float linear;
	float constant;
	float farPlane;

	glm::vec3 position;
	std::vector<glm::mat4> lightTransforms;
	glm::mat4 lightPerspective;

public:
	PointLight();
	PointLight(float mainLightIntensity, float aLightIntensity, 
		glm::vec3 ambientColor, float exponent, float linear, float constant, glm::vec3 position,int shadowMapWidth,int shadowMapHeight,float farPlane);

	void useLight(Shader shader, int i) override;

	void attachShadowMap(Shader shader, GLenum unit, int unitValue, int lightIndex);

	void attachLightTransforms(Shader shader);

	~PointLight();

};

