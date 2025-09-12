#pragma once
#include "Light.h"

#include <GLM/glm.hpp>

#include "Shader.h"

#include <stdio.h>

class DirectionalLight : 
	public Light
{
private:
	glm::vec3 direction;

public:
	DirectionalLight(float directionalLightIntensity, float aLightIntensity, glm::vec3 ambientColor, glm::vec3 direction);

    void useLight(Shader* shader) override;

	~DirectionalLight();


};

