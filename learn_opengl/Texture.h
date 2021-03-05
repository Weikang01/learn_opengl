#pragma once
#include "Libs.h"
#include "Shader.h"
#include "Structures.h"

class Texture
{
private:
	unsigned int textureId;
	unsigned int texIdx;
public:
	Texture(const char* fileName, GLenum texWrapSMode = GL_CLAMP_TO_BORDER,
		GLenum texWrapTMode = GL_CLAMP_TO_BORDER, GLenum texMinFilterMode = GL_NEAREST, GLenum texMagFilterMode = GL_NEAREST);
	~Texture();
	unsigned int getId()const { return textureId; }
	int bindShader(Shader& shader, const char* texUniformName, const char* matUniformName = nullptr);
	void bind();
};
