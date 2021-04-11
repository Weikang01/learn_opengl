#version 440 core
layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoord;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vertex.xy, 0.f, 1.f);
	TexCoord = vertex.zw;
}