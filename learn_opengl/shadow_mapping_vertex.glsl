#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(std140) uniform Matrices {
	mat4 projection;
	mat4 view;
};

uniform mat4 model;
uniform mat3 normalMat;
uniform mat4 lightSpaceMatrix;


out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.f);
	vs_out.FragPos = vec3(model * vec4(position, 1.f));
	vs_out.Normal = normalMat * normal;
	vs_out.TexCoord = texCoord;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.f);
}