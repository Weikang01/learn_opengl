#version 440 core
layout(depth_greater) out float gl_FragDepth;
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

in vec2 TexCoord;

void main()
{
	if (gl_FrontFacing)
	{
		color = texture(texture_diffuse1, TexCoord);
	}
	else {
		color = texture(texture_diffuse2, TexCoord);
	}
	gl_FragDepth = gl_FragCoord.z + 0.1f;
}