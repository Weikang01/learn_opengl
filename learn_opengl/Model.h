#pragma once
#include "Libs.h"
#include "Shader.h"
#include "Mesh.h"

unsigned int TextureFromFile(const char* path, const string& directory);

struct boneInfo
{
	// id is index in transforms
	int id;
	// offset matrix transforms vertex from model space to bone space
	glm::mat4 offset;
};


class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	Model(const GLchar* filePath);
	Model(const GLchar* filePath, const vector<Texture>& textures);
	void draw(Shader& shader);
	void add_texture(const Texture texture);
	void set_texture(const Texture textures);
	void set_texture(const Texture texture, const unsigned int index);
	void set_textures(const vector<Texture> textures);
	map<string, boneInfo> & getBoneInfoMap() { return boneInfoMap; }
	int & getBoneCount() { return boneCounter; }

private:
	bool customize_texture;

	// bone related
	map<string, boneInfo> boneInfoMap;
	int boneCounter = 0;
	void extractBoneWeightForVertices(vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void loadModel(string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);


public:
};