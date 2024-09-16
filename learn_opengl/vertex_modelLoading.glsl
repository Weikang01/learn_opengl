#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat3 normalMat;

uniform mat4 model;
layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.f);
	FragPos = vec3(model * vec4(position, 1.f));
	Normal = normalMat * normal;
	TexCoord = texCoord;
}