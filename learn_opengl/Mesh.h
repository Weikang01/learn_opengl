#pragma once
#include "Libs.h"
#include "Shader.h"
//#include "Texture.h"
//#include "Structures.h"

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	//vector<Texture> textures;
	//vector<int> texIdxs;
	//vector<string> texName;
private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

public:
	Mesh(vector<Vertex> vertices);
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
	//Mesh(vector<Vertex> vertices, vector<Texture> textures);
	void bindVAO();
	//void bindShader(Shader& shader);
	void draw();
	void draw(Shader& shader);
	unsigned int getVAO()const { return VAO; }
	~Mesh();
};