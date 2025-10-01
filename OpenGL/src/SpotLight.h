#pragma once
#include "PointLight.h"
#include <GLFW/glfw3.h>
#include "Window.h"

class SpotLight : 
	public PointLight
{
private:

	glm::vec3 direction;
	float cutOff, procCutOff;
	bool active;
	bool holdingKey;

public:
	SpotLight();

	SpotLight(float mainLightIntensity, float aLightIntensity, glm::vec3 ambientColor, float exponent, 
		float linear, float constant, glm::vec3 position,float cutOff, glm::vec3 direction);

	void useLight(Shader shader, int i) override;

	void setPosition(glm::vec3 newPos);
    void setDirection(glm::vec3 newDir);

	glm::vec3 getPosition() const;

	glm::vec3 getDirection() const;

	void setCondition(Window* window);

	bool getSpotLightStatus() const;

	~SpotLight();

};

