#version 440 core
in vec3 Normal;
in vec3 Position;
out vec4 color;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.f / 1.52f;
	vec3 I = normalize(Position - camPos);
	//vec3 R = reflect(I, normalize(Normal));
	vec3 R = refract(I, normalize(Normal), ratio);
	color = texture(skybox, R);
}