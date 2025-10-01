#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>
#include "Shader.h"
#include "ShadowMap.h"

class Light
{
protected:
	float directionalLightIntensity;
	float aLightIntensity;
	glm::vec3 ambientColor;
	ShadowMap* shadowMap;
	int shadowMapWidth, shadowMapHeight;

public:
	Light();
	
	Light(float mainLightIntensity, float aLightIntensity, glm::vec3 ambientColor,int shadowMapWidth,int shadowMapHeight);

	virtual void useLight(Shader shader, int i);

	glm::vec3 getAmbientColor();

	float getAmbientLightIntensity();

	ShadowMap* getShadowMap() const;

	int getShadowMapWidth() const;
	int getShadowMapHeight() const;

	~Light();
	
};

