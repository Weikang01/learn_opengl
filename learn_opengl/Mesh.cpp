#include "Mesh.h"


Mesh::Mesh(vector<Vertex> vertices)
	:EBO(0)
{
	this->vertices = vertices;
	this->indices = vector<unsigned int>();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

	// textureCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

	// textureCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Mesh::bindVAO()
{
	glBindVertexArray(VAO);
}

//void Mesh::bindShader(Shader& shader)
//{
//	for (size_t i = 0; i < textures.size(); i++)
//	{
//		texIdxs.push_back(textures[i].bindShader(shader, shader.texNameList[i].c_str(), "material"));
//	}
//}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	if (indices.size() == 0)
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	else
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::draw(Shader& shader)
{
	//for (size_t i = 0; i < textures.size(); i++)
	//{
	//	textures[i].bind();
	//}
	glBindVertexArray(VAO);
	if (indices.size() == 0)
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	else
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
