#version 440 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;

layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(position, 1.f);
	TexCoord = texCoord;
}