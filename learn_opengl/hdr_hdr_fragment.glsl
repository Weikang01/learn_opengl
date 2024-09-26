#version 440 core
out vec4 color;

in vec2 TexCoord;
uniform sampler2D hdrBuffer;

vec3 ReinhardMapping(vec3 hdr)
{
	return hdr / (1.f + hdr);
}

float exposure = 5.f;

vec3 ExposureMapping(vec3 hdr)
{
	return vec3(1.f) - exp(-hdr * exposure);
}

void main()
{
	const float gamma = 2.2f;
	vec3 hdrColor = texture(hdrBuffer, TexCoord).rgb;

	// Reinhard color mapping
	vec3 mapped = ExposureMapping(hdrColor);

	// gamma correction
	mapped = pow(mapped, vec3(1.f / gamma));
	color = vec4(mapped, 1.f);
}