#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoord;

void main()
{
	vec4 pos = projection * view * vec4(position, 1.f);
	gl_Position = pos.xyww;
	TexCoord = position;
}