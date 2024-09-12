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

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	Vertex() = default;
	Vertex(const glm::vec3 p, const glm::vec3 n, const glm::vec2 t) { position = p; normal = n; texCoord = t; tangent = bitangent = glm::vec3(0.f); };
	Vertex(const glm::vec3 p, const glm::vec2 t) {
		position = p; texCoord = t; 
		normal = glm::vec3(0.f, 1.f, 0.f); 
		tangent = bitangent = glm::vec3(0.f);
	};
};

struct Texture
{
	GLuint id;
	string type;
	aiString path;
};