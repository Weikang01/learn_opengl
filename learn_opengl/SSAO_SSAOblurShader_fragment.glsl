#version 440 core
out float color;
in vec2 TexCoord;

uniform sampler2D AmbientOcclusion;

const int blurSize = 4; // size of noise texture (4x4)
const int halfBlurSize = blurSize / 2;

void main()
{
	vec2 texelSize = 1.f / vec2(textureSize(AmbientOcclusion, 0));
	float result = 0.f;
	for (int x = 0; x < blurSize; x++)
	{
		for (int y = 0; y < blurSize; y++)
		{
			vec2 offset = vec2(-halfBlurSize + x, -halfBlurSize + y) * texelSize;
			result += texture(AmbientOcclusion, TexCoord + offset).r;
		}
	}
	color =  result / (float(blurSize * blurSize));
}