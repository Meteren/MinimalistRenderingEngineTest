#pragma once


#include <GL/glew.h>

class Material
{
private:
	float metallic;
	float smoothness;
public:
	Material();

	Material(float metallic, float smoothness);

	void setMaterial(unsigned int shader, const char* metallic, const char* smoothness);

	~Material();
	
};

