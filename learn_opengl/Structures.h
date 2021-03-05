#pragma once
#include "Libs.h"
#include "Shader.h"

struct Material
{
	string    name     ;
	glm::vec3 ambient  ;
	glm::vec3 diffuse  ;
	glm::vec3 specular ;
	float     shininess;
	Material(
		const string& name,
		glm::vec3 ambient  = glm::vec3(),
		glm::vec3 diffuse  = glm::vec3(),
		glm::vec3 specular = glm::vec3(),
		float shininess    = 0.0f)
		:ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
	{}
	void setAsUniform(Shader& shader);
};

struct DirLight
{
	string    name     ;
	glm::vec3 direction;
	glm::vec3 ambient  ;
	glm::vec3 diffuse  ;
	glm::vec3 specular ;
	DirLight(const string& name,
		glm::vec3 direction = glm::vec3(),
		glm::vec3 ambient   = glm::vec3(),
		glm::vec3 diffuse   = glm::vec3(),
		glm::vec3 specular  = glm::vec3())
		:
		name     (name     ),
		direction(direction),
		ambient  (ambient  ),
		diffuse  (diffuse  ),
		specular (specular )
	{}
	void setAsUniform         (Shader& shader);

	void setDirectionAsUniform(Shader& shader, const GLfloat* value);
	void setAmbientAsUniform  (Shader& shader, const GLfloat* value);
	void setDiffuseAsUniform  (Shader& shader, const GLfloat* value);
	void setSpecularAsUniform (Shader& shader, const GLfloat* value);
};

struct PointLight
{
	string    name     ;
	glm::vec3 position ;
	glm::vec3 ambient  ;
	glm::vec3 diffuse  ;
	glm::vec3 specular ;

	float     constant ;
	float     linear   ;
	float     quadratic;
	PointLight(
		//glm::vec3 direction = glm::vec3(),
		const string& name,
		glm::vec3 position  = glm::vec3(),
		glm::vec3 ambient   = glm::vec3(),
		glm::vec3 diffuse   = glm::vec3(),
		glm::vec3 specular  = glm::vec3(),
		float     constant  = 1.f        ,
		float     linear    = .09f       ,
		float     quadratic = .032f
	)
		:
		name     (name     ),
		position (position ),
		ambient  (ambient  ),
		diffuse  (diffuse  ),
		specular (specular ),
		constant (constant ),
		linear   (linear   ),
		quadratic(quadratic)
	{}
	void setAsUniform(Shader& shader);

	void setPositionAsUniform(Shader& shader, const GLfloat* value);
	void setAmbientAsUniform(Shader& shader, const GLfloat* value);
	void setDiffuseAsUniform(Shader& shader, const GLfloat* value);
	void setSpecularAsUniform(Shader& shader, const GLfloat* value);
	void setAttenuationCoefficients(Shader& shader, float constant, float linear, float quadratic);
};

struct SpotLight
{
	string    name       ;
	glm::vec3 position   ;
	glm::vec3 ambient    ;
	glm::vec3 diffuse    ;
	glm::vec3 specular   ;
	glm::vec3 front      ;
	float     cutOff     ;
	float     outerCutOff;
	float     constant   ;
	float     linear     ;
	float     quadratic  ;
	SpotLight(
		const string& name,
		glm::vec3     position    = glm::vec3(),
		glm::vec3     ambient     = glm::vec3(),
		glm::vec3     diffuse     = glm::vec3(),
		glm::vec3     specular    = glm::vec3(),
		glm::vec3     front       = glm::vec3(),
		float         cutOff      = cos(glm::radians(12.5f)),
		float         outerCutOff = cos(glm::radians(15.f)),
		float         constant    = 1.f,
		float         linear      = .09f,
		float         quadratic   = .032f
	)
		:
		//direction(direction),
		name(name),
		position(position),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		front(front),
		cutOff(cutOff),
		outerCutOff(outerCutOff),
		constant(constant),
		linear(linear),
		quadratic(quadratic)
	{}

	void setAsUniform(Shader& shader);

	void setPositionAsUniform(Shader& shader, const GLfloat* value);
	void setAmbientAsUniform(Shader& shader, const GLfloat* value);
	void setDiffuseAsUniform(Shader& shader, const GLfloat* value);
	void setSpecularAsUniform(Shader& shader, const GLfloat* value);
	void setFrontAsUniform(Shader& shader, const GLfloat* value);
	void setAttenuationCoefficients(Shader& shader, float constant, float linear, float quadratic);
};

struct Light
{
	//glm::vec3 direction;
	glm::vec4 position   ;
	glm::vec3 ambient    ;
	glm::vec3 diffuse    ;
	glm::vec3 specular   ;
	glm::vec3 front      ;
	float     cutOff     ;
	float     outerCutOff;
	float     constant   ;
	float     linear     ;
	float     quadratic  ;
	Light(
		//glm::vec3 direction = glm::vec3(),
		glm::vec4 position = glm::vec4(),
		glm::vec3 ambient = glm::vec3(),
		glm::vec3 diffuse = glm::vec3(),
		glm::vec3 specular = glm::vec3(),
		glm::vec3 front = glm::vec3(),
		float cutOff = cos(glm::radians(12.5f)),
		float outerCutOff = cos(glm::radians(15.f)),
		float constant = 1.f,
		float linear = .09f,
		float quadratic = .032f
	)
		:
		//direction(direction),
		position(position),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		front(front),
		cutOff(cutOff),
		outerCutOff(outerCutOff),
		constant(constant),
		linear(linear),
		quadratic(quadratic)
	{}
	void setAsUniform(Shader& shader);

	void setPositionAsUniform(Shader& shader, const GLfloat* value);
	void setAmbientAsUniform(Shader& shader, const GLfloat* value);
	void setDiffuseAsUniform(Shader& shader, const GLfloat* value);
	void setSpecularAsUniform(Shader& shader, const GLfloat* value);
	void setFrontAsUniform(Shader& shader, const GLfloat* value);
	void setAttenuationCoefficients(Shader& shader, float constant, float linear, float quadratic);
};