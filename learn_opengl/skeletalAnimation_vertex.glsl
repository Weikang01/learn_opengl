#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;

void main()
{
	vec4 totalPosition = vec4(0.f);
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if (boneIds[i] == -1)
			continue;
		if (boneIds[i] >= MAX_BONES)
		{
			totalPosition = vec4(position, 1.f);
			break;
		}
		vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position, 1.f);
		totalPosition += localPosition * weights[i];
		vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
	}

	gl_Position = projection * view * model * totalPosition;
	TexCoord = texCoord;
}