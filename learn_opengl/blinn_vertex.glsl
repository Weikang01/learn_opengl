#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}vs_out;

layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.f);
	vs_out.FragPos  = vec3(model * vec4(position, 1.f));
	vs_out.Normal   = normal;
	vs_out.TexCoord = texCoord;
}