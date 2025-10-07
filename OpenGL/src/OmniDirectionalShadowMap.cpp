#include "OmniDirectionalShadowMap.h"

OmniDirectionalShadowMap::OmniDirectionalShadowMap() : ShadowMap()
{
}

OmniDirectionalShadowMap::OmniDirectionalShadowMap(int WIDTH, int HEIGHT) : ShadowMap(WIDTH, HEIGHT)
{

}

bool OmniDirectionalShadowMap::init()
{
	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT,0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error while generating FBO %i.\n", status);
		glBindBuffer(GL_FRAMEBUFFER, 0);
		return false;
	}

	glBindBuffer(GL_FRAMEBUFFER,0);

	return true;

}

void OmniDirectionalShadowMap::writeBuffer()
{
	glBindBuffer(GL_FRAMEBUFFER, FBO);
}

void OmniDirectionalShadowMap::readBuffer(GLenum unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

OmniDirectionalShadowMap::~OmniDirectionalShadowMap()
{
}
