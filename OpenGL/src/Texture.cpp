
#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture()
{	
	width = 0;
	height = 0;
	tex_ID = 0;
	bitDepth = 0;
	this->path = "";
}

Texture::Texture(const char* path)
{

	this->path = path;
}

bool Texture::loadTexture()
{
    unsigned char* data = stbi_load(path, &width, &height, &bitDepth, 0);

	if (!data)
		return false;

	glGenTextures(1, &tex_ID);
	bindTexture();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	unbindTexture();

	return true;

}

bool Texture::loadTextureA()
{

	unsigned char* data = stbi_load(path, &width, &height, &bitDepth, 0);

	if (!data)
		return false;

	glGenTextures(1, &tex_ID);
	bindTexture();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	unbindTexture();

	return true;
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	bindTexture();
}

void Texture::bindTexture() const
{
	glBindTexture(GL_TEXTURE_2D,tex_ID);
}

void Texture::unbindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1,&tex_ID);
	width = 0;
	height = 0;
	tex_ID = 0;
	path = "";
}
