#version 440 core
out vec4 color;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D AmbientOcclusion;

struct Light
{
	vec3 position;
	vec3 color;

	float linear;
	float quadratic;
};

uniform Light light;
uniform vec3 viewPos;

uniform bool occlusion;

void main()
{
	vec3 fragPos = texture(gPosition, TexCoord).xyz;
	vec3 normal = texture(gNormal, TexCoord).rgb;
	vec3 albedo = texture(gAlbedo, TexCoord).rgb;
	float noise = texture(AmbientOcclusion, TexCoord).r;

	vec3 lighting = vec3(0.f);

	vec3 ambient = occlusion? vec3(.3f * noise) : vec3(0.25f);

	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0.f);
	vec3 diffuse = albedo * light.color * diff;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(viewPos + light.position);
	float spec = pow(max(dot(halfwayDir, normal), 0.f), 32.f);
	vec3 specular = light.color * spec;

	float distance = length(light.position - fragPos);
	float attenuation = 1.f / (1.f + light.linear * distance + light.quadratic * distance * distance);
	lighting = (ambient + (diffuse + specular) * attenuation);

	//color = vec4(vec3(noise), 1.f);
	color = vec4(lighting, 1.f);
}