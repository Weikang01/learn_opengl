#pragma once
#include "Libs.h"

class Texture2D
{
public:
	GLuint id;
	GLuint width, height;
	
	GLuint internalFormat;
	GLuint imageFormat;
	
	GLuint wrap_s;
	GLuint wrap_t;
	GLuint filter_min;
	GLuint filter_max;

	Texture2D();
	Texture2D(const Texture2D& o);

	void generate(GLuint width, GLuint height, unsigned char* data);
	void bind() const;
};
