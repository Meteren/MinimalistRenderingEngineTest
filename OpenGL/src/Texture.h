#pragma once

#include <iostream>
#include <GL\glew.h>

#include "stb_image.h"
#include "Shader.h"

class Texture
{
private:
	unsigned int tex_ID;
	int width, height, bitDepth;
	const char* path;

public:

	Texture();

	Texture(const char* path);

	bool loadTexture();
	bool loadTextureA();

	void useTexture(Shader& shader, GLenum unit);

	void bindTexture() const;
	void unbindTexture() const;

	~Texture();
};

