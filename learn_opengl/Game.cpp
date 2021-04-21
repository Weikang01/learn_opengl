#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

SpriteRenderer* renderer;

Game::Game(GLuint width, GLuint height)
	:state(GAME_ACTIVE), keys(), width(0), height(0)
{
	init();
}

Game::~Game()
{
}

void Game::init()
{
	ResourceManager::LoadShader("sprite_vertex.glsl", "sprite_fragment.glsl", nullptr, "sprite");

	//glm::mat4 projection = glm::ortho(0.f, float(width), float(height), 0.f);
	//ResourceManager::getShader("sprite").use().setInt("image", 0);
	//ResourceManager::getShader("sprite").setMat4fv("projection", projection);

	//renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	//ResourceManager::loadTexture("face", "Images/awesomeface.png", GL_TRUE, false);
}

void Game::processInput(GLfloat dt)
{
}

void Game::update(GLfloat dt)
{
}

void Game::render()
{
	//renderer->drawSprite(ResourceManager::getTexture("face"), glm::vec2(0.f));
}
