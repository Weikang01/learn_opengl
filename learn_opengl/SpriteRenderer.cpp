#include "SpriteRenderer.h"

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &quadVAO);
}

SpriteRenderer::SpriteRenderer(const Shader& shader)
	:shader(shader), quadVAO(0)
{
	this->initRenderData();
}

void SpriteRenderer::drawSprite(const Texture2D& texture, const glm::vec2 position, const glm::vec2 size, const GLfloat rotate, const glm::vec3 color)
{
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glm::mat4 model(1.f);
	model = glm::translate(model, glm::vec3(position, 0.f));
	model = glm::translate(model, glm::vec3( 0.5f * size.x,  0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader.setMat4fv("model", model);
	shader.set3fv("spriteColor", color);

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	GLuint VBO;
	glm::vec4 vertices[] =
	{
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 1.0f, 0.0f)
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}