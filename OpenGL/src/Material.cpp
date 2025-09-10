#include "Material.h"

Material::Material() : smoothness(0), metallic(0)
{
}

Material::Material(float metallic, float smoothness)
{
	this->metallic = metallic;
	this->smoothness = smoothness;
}

void Material::setMaterial(unsigned int shader, const char* metallic, const char* smoothness)
{
	int metallicLoc = glGetUniformLocation(shader, metallic);
	int smoothnessLoc = glGetUniformLocation(shader, smoothness);

	glUniform1f(metallicLoc,this->metallic);
	glUniform1f(smoothnessLoc, this->smoothness);
}

Material::~Material()
{
}
