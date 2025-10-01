#pragma once
#include "Light.h"
#include "DirectionalShadowMap.h"
#include <stdio.h>


class DirectionalLight : 
	public Light
{
private:
	glm::vec3 direction;
	glm::mat4 dLightTransform;
	glm::mat4 ortho;

public:
	
	DirectionalLight();

	DirectionalLight(float directionalLightIntensity, float aLightIntensity, glm::vec3 ambientColor, glm::vec3 direction,int shadowMapWidth,int shadowMapHeight);

    void useLight(Shader shader,int i) override;

	glm::mat4 calculateLightTransform();

	glm::mat4 getLightTransform() const;

	void attachdShadowMap(Shader shader);

	void attachDepthElement(Shader* shader);

	~DirectionalLight();


};

