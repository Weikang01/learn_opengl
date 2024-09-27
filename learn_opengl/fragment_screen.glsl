#version 440 core
out vec4 color;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

//const float offset = 1.f / 300.f;

void main()
{
	// inversion
	//color = vec4((vec3(1.0)- texture(texture_diffuse1, TexCoord).xyz), 1.0);

	// grayscale
	//color = texture(texture_diffuse1, TexCoord);
	//float average = (color.x + color.y + color.z) / 3.f;
	//color = vec4(average, average, average, 1.f);

	//vec2 offsets[9] = vec2[](
	//	vec2(-offset, offset ),
	//	vec2(0.f    , offset ),
	//	vec2(offset , offset ),
	//	vec2(-offset, 0.f    ),
	//	vec2(0.f    , 0.f    ),
	//	vec2(offset , 0.f    ),
	//	vec2(-offset, -offset),
	//	vec2(0.f    , -offset),
	//	vec2(offset , -offset)
	//	);
	//float kernel[9] = float[](
	//	1.0, 1.0, 1.0,
	//	1.0, -8.0, 1.0,
	//	1.0, 1.0, 1.0
	//	);
	//vec3 sampleTex[9];
	//for (int i = 0; i < 9; i++)
	//{
	//	sampleTex[i] = vec3(texture(texture_diffuse1, TexCoord.st + offsets[i]));
	//}
	//vec3 col = vec3(0.f);
	//for (int i = 0; i < 9; i++)
	//{
	//	col += sampleTex[i] * kernel[i];
	//}
	color = texture(texture_diffuse1, TexCoord);
}