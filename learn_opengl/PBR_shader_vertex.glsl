#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat3 normalMat;

layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};


out VS_OUT{
	vec2 TexCoord;
	vec3 FragPos;
	vec3 Normal;
} vs_out;

void main()
{
	vs_out.TexCoord = texCoord;
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.Normal = normalMat * normal;

	gl_Position = projection * view * model * vec4(position, 1.f);
}