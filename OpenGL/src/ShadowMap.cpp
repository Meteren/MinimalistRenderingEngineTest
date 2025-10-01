#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	FBO = 0;
	textureID = 0;
	WIDTH = 0;
	HEIGHT = 0;
}

ShadowMap::ShadowMap(int WIDTH, int HEIGHT)
{
	this->WIDTH = WIDTH;
	this->HEIGHT = HEIGHT;
}

bool ShadowMap::init()
{
	return false;
}

void ShadowMap::writeBuffer()
{
	return;
}

void ShadowMap::readBuffer(GLenum unit)
{
	return;
}

ShadowMap::~ShadowMap()
{
	if(FBO)
		glDeleteFramebuffers(1, &FBO);

	if(textureID)
		glDeleteTextures(1, &textureID);

	FBO = 0;
	textureID = 0;
	WIDTH = 0;
	HEIGHT = 0;
}
