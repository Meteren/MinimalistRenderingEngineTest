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

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float colors[] = { 1.0f,0.0f,0.0f,1.0f };

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colors);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, textureID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum fbStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	if (fbStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error while generating FBO %i.\n", fbStatus);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;

}

void DirectionalShadowMap::writeBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void DirectionalShadowMap::readBuffer(GLenum unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

DirectionalShadowMap::~DirectionalShadowMap() 
{
}
