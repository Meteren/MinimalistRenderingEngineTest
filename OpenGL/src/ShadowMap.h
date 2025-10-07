#pragma once

#include <GL/glew.h>
#include <stdio.h>

class ShadowMap
{
protected:
	unsigned int textureID;
	unsigned int FBO;
	int WIDTH, HEIGHT;

public:
	ShadowMap();

	ShadowMap(int WIDTH, int HEIGHT);

	virtual bool init() = 0;

	virtual void writeBuffer() = 0;

	virtual void readBuffer(GLenum unit) = 0;

	~ShadowMap();

};

