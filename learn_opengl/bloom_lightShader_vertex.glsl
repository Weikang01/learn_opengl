#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;

layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};


void main()
{
	gl_Position = projection * view * model * vec4(position, 1.f);
}