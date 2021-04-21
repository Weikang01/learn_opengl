#include "Texture.h"

Texture2D::Texture2D()
	:internalFormat(GL_RGB), imageFormat(GL_RGB),
	wrap_s(GL_CLAMP_TO_EDGE), wrap_t(GL_CLAMP_TO_EDGE),
	filter_min(GL_LINEAR), filter_max(GL_LINEAR),
	width(0), height(0)
{
	glGenTextures(1, &id);
}

Texture2D::Texture2D(const Texture2D& o)
	:id(o.id), internalFormat(o.internalFormat), imageFormat(o.imageFormat),
	wrap_s(o.wrap_s), wrap_t(o.wrap_t),
	filter_min(o.filter_min), filter_max(o.filter_max),
	width(o.width), height(o.height)
{}

void Texture2D::generate(GLuint width, GLuint height, unsigned char* data)
{
	this->width = width;
	this->height = height;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}
