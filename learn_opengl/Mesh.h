#pragma once
#include "Libs.h"
#include "Shader.h"

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	unsigned int VAO;
	unsigned int instanceVBO;
	Mesh(vector<Vertex> vertices, 
		vector<Texture> textures = vector<Texture>{},
		vector<unsigned int> indices = vector<unsigned int>{});

	void draw(Shader& shader, const GLenum mode = GL_TRIANGLES);
	void add_texture(const Texture texture);
	void set_texture(const Texture texture);
	void set_texture(const Texture texture, const unsigned int index);
	void set_textures(const vector<Texture> textures);
	void print_textures() const;
	void clear_texture();
	void instantiate(unsigned int count);
	void setInstanceUniform(Shader& shader, vector<glm::vec2> uniform, const string& name);
private:
	unsigned int VBO;
	unsigned int EBO;
	unsigned int instanceCount;
	bool instanceVBO_generated = false;
	void bindTextures(Shader& shader);
	void setupMesh();
};

Mesh drawSphere(const unsigned int x_segments, const unsigned int y_segments, const vector<Texture> textures = vector<Texture>());
