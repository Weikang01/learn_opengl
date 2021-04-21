#version 440 core
out vec4 color;
in vec2 TexCoord;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
	color = vec4(spriteColor * texture(image, TexCoord).xyz, 1.f);
}