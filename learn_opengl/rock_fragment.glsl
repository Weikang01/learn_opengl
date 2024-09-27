#version 440 core
out vec4 color;

in vec2 TexCoord;
uniform sampler2D texture_diffuse1;

void main()
{
	color = texture(texture_diffuse1, TexCoord);
}