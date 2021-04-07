#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

struct Light
{
	vec3 position;
	vec3 color;
};

#define LIGHT_NR 4

uniform Light light[LIGHT_NR];
uniform vec3 viewPos;
uniform mat4 model;
uniform mat3 normalMat;

layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};


out VS_OUT{
	vec2 TexCoord;
	vec3 TangentLightPos[LIGHT_NR];
	vec3 LightColor[LIGHT_NR];
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

void main()
{
	vs_out.TexCoord   = texCoord;

	vec3 T = normalize(normalMat * tangent);
	vec3 B = normalize(normalMat * bitangent);
	vec3 N = normalize(normalMat * normal);
	
	mat3 TBN = transpose(mat3(T, B, N));

	for (int i = 0; i < LIGHT_NR; i++)
	{
		vs_out.TangentLightPos[i] = TBN * light[i].position;
		vs_out.LightColor[i] = light[i].color;
	}
	vs_out.TangentViewPos  = TBN * viewPos;
	vs_out.TangentFragPos  = TBN * vec3(model * vec4(position, 0.0));


	gl_Position = projection * view * model * vec4(position, 1.f);
}