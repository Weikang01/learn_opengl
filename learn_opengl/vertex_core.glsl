#version 440 core
layout(location = 0) in vec3 position;

//out vec3 vt_color;
//out vec2 vt_texCoord;
//uniform float bias;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(position, 1.f);
	//gl_Position = view * model * vec4(position, 1.f);
	//vt_color = vec3((gl_Position + 1.f) / 2.f);
	//vt_texCoord = texCoord;
}