#version 440 core
out vec4 color;

in vec2 TexCoord;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
//uniform float exposure = 1.f;

struct light
{
	vec3 position;
	vec3 color;
	float linear;
	float quadratic;
	float radius;
};

#define NR_LIGHTS 32
uniform light lights[NR_LIGHTS];

uniform vec3 viewPos;

void main()
{
	vec3 FragPos  = texture(gPosition, TexCoord).rgb;
	vec3 Normal  = texture(gNormal, TexCoord).rgb;
	vec3 Albedo  = texture(gAlbedoSpec, TexCoord).rgb;
	float Specular = texture(gAlbedoSpec, TexCoord).a;

	vec3 lighting = vec3(0.f);
	
	// diffuse
	vec3 viewDir = normalize(viewPos - FragPos);
	for (int i = 0; i < NR_LIGHTS; i++)
	{
		float distance = length(lights[i].position - FragPos);

		if (distance < lights[i].radius)
		{
			vec3 lightDir = normalize(lights[i].position - FragPos);
			float diff = max(dot(lightDir, Normal), 0.0f);

			float attenuation = 1.f / (1.f + lights[i].linear * distance + lights[i].quadratic * distance * distance);
			lighting += diff * lights[i].color * attenuation;
		}
	}
	//lighting = lighting / (lighting + vec3(1.f));
	lighting *= Albedo;
	color = vec4(lighting, 1.f);
}