#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT{
    vec3 color;
	vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = vec4(position, 1.f);
	vs_out.TexCoord = texCoord;
	vs_out.color = normal;
}