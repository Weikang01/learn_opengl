#include "Texture.h"

Texture::Texture(const char* fileName, GLenum texWrapSMode,
	GLenum texWrapTMode, GLenum texMinFilterMode, GLenum texMagFilterMode)
	:texIdx(-1)
{
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texWrapSMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texWrapTMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texMinFilterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texMagFilterMode);

	GLenum colorChannelArray[]{ GL_FALSE, GL_R, GL_RG, GL_RGB, GL_RGBA };
	int t_width, t_height, t_nrChannels;
	unsigned char* data = stbi_load(fileName, &t_width, &t_height, &t_nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, colorChannelArray[t_nrChannels], t_width, t_height, 0, colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR::TEXTURE_IMAGE_IMPORT_FAILED::"<< fileName << endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

int Texture::bindShader(Shader& shader, 
	const char* texUniformName, const char* matUniformName)
{
	
	int location;
	shader.use();
	if (matUniformName)
	{
		string name = matUniformName;
		name.append(".");
		name += texUniformName;
		location = shader.getUniformLocation(name.c_str());
		
	} else
		location = shader.getUniformLocation(texUniformName);

	if (location>=0)
	{
		this->texIdx = shader.texSlotCounter++;
		shader.setInt(location, texIdx);
	}
	return location;
}

void Texture::bind()
{
	if (texIdx<0)
	{
		cout << "YOU MUST BIND TEXTURE TO SHADER::" << textureId << endl;
		throw("");
	}
	glActiveTexture(GL_TEXTURE0 + texIdx);
	glBindTexture(GL_TEXTURE_2D, textureId);
}
