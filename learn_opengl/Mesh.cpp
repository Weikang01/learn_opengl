#include "Mesh.h"


void Mesh::bindTextures(Shader& shader)
{
	if (textures.size() == 0)
		return;

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int reflectionNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (size_t i = 0; i < textures.size(); i++)
	{
		string number{};
		if (textures[i].type == "texture_diffuse")
		{
			number = to_string(diffuseNr++);
		}
		else if (textures[i].type == "texture_specular")
		{
			number = to_string(specularNr++);
		}
		else if (textures[i].type == "texture_reflection")
		{
			number = to_string(reflectionNr++);
		}
		else if (textures[i].type == "texture_normal")
		{
			number = to_string(normalNr++);
		}
		else if (textures[i].type == "texture_height")
		{
			number = to_string(heightNr++);
		}
		else if (textures[i].type == "skybox" || textures[i].type == "cubemap" || textures[i].type == "depthCubeMap")
		{
			glActiveTexture(GL_TEXTURE0 + i);
			shader.setInt(textures[i].type.c_str(), i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->textures[i].id);
			continue;
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + i);
			shader.setInt(textures[i].type.c_str(), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
			continue;
		}
		string name = textures[i].type;

		glActiveTexture(GL_TEXTURE0 + i);
		shader.setInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * (sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

	if (indices.size()!=0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}
	else
		EBO = 0;

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

	// textureCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));

	// tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tangent)));

	// bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, bitangent)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::add_texture(const Texture texture)
{
	this->textures.push_back(texture);
}

void Mesh::instantiate(unsigned int count)
{
	instanceCount = count;
	glGenBuffers(1, &instanceVBO);
}

void Mesh::setInstanceUniform(Shader& shader, vector<glm::vec2> uniform, const string& name)
{
	shader.set2fv_vector(name, uniform);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instanceCount, &uniform[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(vector<Vertex> vertices, vector<Texture> textures, vector<unsigned int> indices)
	:instanceCount(1)
{
	this->vertices =  vertices;
	this->indices  =   indices;
	this->textures =  textures;
	this->setupMesh();
}

void Mesh::draw(Shader& shader, const GLenum mode)
{
	bindTextures(shader);

	glBindVertexArray(VAO);
	if (instanceCount == 1)
		if (EBO > 0)
			glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(mode, 0, vertices.size());
	else
		if (EBO > 0)
			glDrawElementsInstanced(mode, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
		else
			glDrawArraysInstanced(mode, 0, vertices.size(), instanceCount);

	glBindVertexArray(0);
	glActiveTexture(0);
}

