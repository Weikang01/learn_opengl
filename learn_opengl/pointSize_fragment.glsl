#version 440 core
out vec4 color;

in VS_OUT
{
	vec2 TexCoord;
} vs_in;

void main()
{
	if (gl_FragCoord.x < 400)
	{
		color = vec4(1.f, 0.28f, 0.26f, 1.0f);
	}
	else {
		color = vec4(.28f, 1.f, 0.26f, 1.0f);
	}
}