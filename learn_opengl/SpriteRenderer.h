#pragma once
#include "Shader.h"
#include "Texture.h"


class SpriteRenderer
{
public:
	~SpriteRenderer();
	SpriteRenderer(const Shader& shader);

	void drawSprite(const Texture2D& texture, const glm::vec2 position,
		const glm::vec2 size = glm::vec2(10.f, 10.f), const GLfloat rotate = 0.f
		, const glm::vec3 color = glm::vec3(1.f));
private:
	Shader shader;
	GLuint quadVAO;
	void initRenderData();
};
