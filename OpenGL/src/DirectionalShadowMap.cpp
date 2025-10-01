#include "DirectionalShadowMap.h"

DirectionalShadowMap::DirectionalShadowMap() : ShadowMap()
{
}

DirectionalShadowMap::DirectionalShadowMap(int WIDTH, int HEIGHT) : ShadowMap(WIDTH, HEIGHT)
{
}

bool DirectionalShadowMap::init()
{
	glGenFramebuffers(1, &FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float colors[] = { 1.0f,0.0f,0.0f,1.0f };

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colors);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, HEIGHT, WIDTH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, textureID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (fbStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: %i\n", fbStatus);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;

}

void DirectionalShadowMap::writeBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void DirectionalShadowMap::readBuffer(int unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

DirectionalShadowMap::~DirectionalShadowMap() 
{
}
