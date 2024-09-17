#version 440 core
out vec4 color;

in vec2 TexCoord;
uniform sampler2D FragColor;
uniform sampler2D BrightColor;
uniform float exposure = 1.f;

vec3 GammaCorrection(vec3 mapped)
{
	const float gamma = 2.2f;
	return pow(mapped, vec3(1.f / gamma));
}

void main()
{
	const float gamma = 2.2f;
	vec3 hdrColor   = texture(FragColor, TexCoord).rgb;
	vec3 bloomColor = texture(BrightColor, TexCoord).rgb;
	hdrColor += bloomColor;

	vec3 result = vec3(1.f) - exp(-hdrColor * exposure);

	result = pow(result, vec3(1.f / gamma));
	color = vec4(result, 1.f);
}