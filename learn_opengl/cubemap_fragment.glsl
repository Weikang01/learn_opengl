#version 440
in vec3 Normal;
in vec3 Position;
in vec2 TexCoord;

uniform vec3 camPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_reflection1;
uniform samplerCube skybox;

out vec4 color;

void main()
{
	vec4 diffuse_color = texture(texture_diffuse1, TexCoord);

	vec3 I = normalize(Position - camPos);
	vec3 norm = normalize(Normal);
	vec3 R = reflect(I, norm);
	float reflect_intensity = texture(texture_reflection1, TexCoord).r;
	vec4 reflect_color;

	if (reflect_intensity > 0.1f)
		reflect_color = texture(skybox, R) * reflect_intensity;

	color = diffuse_color + reflect_color;
}