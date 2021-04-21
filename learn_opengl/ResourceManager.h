#pragma once
#include "Libs.h"
#include "Shader.h"
#include "Texture.h"

class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader    GetShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    // properly de-allocates all loaded resources
    static void      Clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

//#include "ResourceManager.h"

//Shader ResourceManager::loadShader(const string name, const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometricShaderFile)
//{
//	Shader shader(vertexShaderFile, fragmentShaderFile, geometricShaderFile);
//	shaders.insert(std::pair<string, Shader>(name, shader));
//	return shader;
//}
//Shader ResourceManager::loadShader(const string name, int dummy, const char* vertexShaderCode, const char* fragmentShaderCode, const char* geometricShaderCode)
//{
//	Shader shader(0, vertexShaderCode, fragmentShaderCode, geometricShaderCode);
//	shaders.insert(std::pair<string, Shader>(name, shader));
//	return shader;
//}
//Shader ResourceManager::getShader(const string name)
//{
//	return shaders[name];
//}
//Texture2D ResourceManager::loadTexture(const string name, const GLchar* path, GLboolean alpha, GLboolean gamma)
//{
//	Texture2D texture = loadTexture2DFromFile(path, alpha, gamma);
//	textures.insert(std::pair<string, Texture2D>(name, texture));
//	return texture;
//}
//Texture2D ResourceManager::getTexture(const string name)
//{
//	return textures[name];
//}
//Texture2D ResourceManager::loadTexture2DFromFile(const GLchar* path, GLboolean alpha, GLboolean gamma)
//{
//	Texture2D texture;
//	unsigned int texID;
//	glGenTextures(1, &texID);
//	int t_width, t_height, t_nrChannels;
//	unsigned char* data = stbi_load(path, &t_width, &t_height, &t_nrChannels, 0);
//	if (alpha && t_nrChannels == 3)t_nrChannels = 4;
//	if (!alpha && t_nrChannels == 4)t_nrChannels = 3;
//
//	if (data)
//	{
//		GLenum colorChannelArray[]{ GL_FALSE, GL_RED, GL_RG, GL_RGB, GL_RGBA };
//		GLenum colorChannelArrayGamma[]{ GL_FALSE, GL_FALSE, GL_FALSE, GL_SRGB, GL_SRGB_ALPHA };
//		texture.imageFormat = colorChannelArray[t_nrChannels];
//		texture.internalFormat = gamma ? colorChannelArrayGamma[t_nrChannels] : colorChannelArray[t_nrChannels];
//		texture.generate(t_width, t_height, data);
//	}
//	else
//	{
//		cout << "ERROR::TEXTURE_IMAGE_IMPORT_FAILED::" << path << endl;
//	}
//	stbi_image_free(data);
//	return texture;
//}
