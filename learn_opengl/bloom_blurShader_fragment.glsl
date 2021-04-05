#version 440 core
out vec4 color;

in vec2 TexCoord;
uniform sampler2D BrightColor;

uniform bool horizontal;

#define KERNEL_SIZE 5

uniform float weight[KERNEL_SIZE] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 tex_offset = 1.f / textureSize(BrightColor, 0);
	vec3 result = texture(BrightColor, TexCoord).rgb * weight[0];
	if (horizontal)
		for (int i = 1; i < KERNEL_SIZE; i++)
		{
			result += texture(BrightColor, TexCoord + vec2(tex_offset.x * i, 0.f)).rgb * weight[i];
			result += texture(BrightColor, TexCoord - vec2(tex_offset.x * i, 0.f)).rgb * weight[i];
		}
	else
		for (int i = 1; i < KERNEL_SIZE; i++)
		{
			result += texture(BrightColor, TexCoord + vec2(0.f, tex_offset.y * i)).rgb * weight[i];
			result += texture(BrightColor, TexCoord - vec2(0.f, tex_offset.y * i)).rgb * weight[i];
		}
	color = vec4(result, 1.f);
}