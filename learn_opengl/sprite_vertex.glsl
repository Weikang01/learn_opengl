#version 440 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoord>
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	TexCoord = vertex.zw;
	gl_Position = projection * model * vec4(vertex.xy, 0.f, 1.f);
}