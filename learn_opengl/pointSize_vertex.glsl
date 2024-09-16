#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};
//uniform mat4 view;
//uniform mat4 projection;

out VS_OUT
{
	vec2 TexCoord;
} vs_out;


void main()
{
	gl_Position = projection * view * model * vec4(position, 1.f);
	gl_PointSize = gl_Position.z;
	vs_out.TexCoord = texCoord;
}