#pragma once

#include <GL/glew.h>

class ShadowMap
{
protected:
	unsigned int textureID;
	unsigned int FBO;
	int WIDTH, HEIGHT;

public:
	ShadowMap();

	ShadowMap(int WIDTH, int HEIGHT);

	virtual bool init();

	virtual void writeBuffer();

	virtual void readBuffer(GLenum unit);

	~ShadowMap();

};

