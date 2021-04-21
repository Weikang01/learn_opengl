#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <random>
#include <map>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ft2build.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::strcmp;
using std::to_string;
using std::smatch;
using std::is_same;
using std::vector;
using std::map;
using std::ifstream;
using std::getline;
using std::regex;
using std::match_results;
using std::regex_search;
using std::regex_match;

void printVec3(glm::vec3 v);
void printMat4(glm::mat4 m);
void printMat3(glm::mat3 m);

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int boneIDs[MAX_BONE_INFLUENCE];
	float boneWeights[MAX_BONE_INFLUENCE];
	Vertex() = default;
	Vertex(const glm::vec3 p, const glm::vec3 n, const glm::vec2 t) :boneWeights{ 0.f }
	{ position = p; normal = n; texCoord = t; tangent = bitangent = glm::vec3(0.f);
	for (size_t i = 0; i < MAX_BONE_INFLUENCE; i++)
		boneIDs[i] = -1;
	};
	Vertex(const glm::vec3 p, const glm::vec2 t) :boneWeights{ 0.f }
	{
		position = p; texCoord = t;
		normal = glm::vec3(0.f, 1.f, 0.f);
		tangent = bitangent = glm::vec3(0.f);
		for (size_t i = 0; i < MAX_BONE_INFLUENCE; i++)
			boneIDs[i] = -1;
	};
	void setVertexBoneDataToDefault()
	{
		for (size_t i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			boneIDs[i] = -1;
			boneWeights[i] = 0.f;
		}
	}
	void setBoneData(int boneID, float weight)
	{
		for (size_t i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if (boneIDs[i] < 0)
			{
				boneWeights[i] = weight;
				boneIDs[i] = boneID;
				break;
			}
		}
	}
};

struct Vertex2D
{
	glm::vec4 vertex; // < position, texCoord>
	Vertex2D() = default;
	Vertex2D(const glm::vec2 position, const glm::vec2 texCoord) : vertex(position.x, position.y, texCoord.x, texCoord.y)
	{}
	Vertex2D(const Vertex& v3) : vertex(v3.position.x, v3.position.y, v3.texCoord.x, v3.texCoord.y)
	{}
	Vertex2D& operator=(const Vertex& v3)
	{
		vertex = glm::vec4(v3.position.x, v3.position.y, v3.texCoord.x, v3.texCoord.y);
		return *this;
	}
};

struct Character
{
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};


struct Texture
{
	GLuint id;
	string type;
	aiString path;
};