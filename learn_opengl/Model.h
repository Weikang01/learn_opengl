#pragma once
#include "Libs.h"
#include "Shader.h"
#include "Mesh.h"

unsigned int TextureFromFile(const char* path, const string& directory);

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
private:
	bool customize_texture;
	void loadModel(string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
public:
};