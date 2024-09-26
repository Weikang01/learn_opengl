#version 440
out vec4 color;

uniform vec3 viewPos;
in vec3 FragPos;
in vec3 Normal;

in vec2 TexCoord;

uniform float shininess;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;
uniform samplerCube texture_cube;
struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform DirLight dirLight;
uniform PointLight pointLight;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), .0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), .0f), shininess);

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));
	return (ambient + diffuse + specular);
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	float distance = length(light.position.xyz - FragPos);
	float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	float diff = max(dot(normal, lightDir), .0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), .0f), shininess);

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));

	return ((ambient + diffuse + specular) * attenuation);
}

float LinearizeDepth(float depth)
{
	float near = 0.1;
	float far = 100.0;
	float z = depth * 2.0 - 1.0; // Back to NDC
	return (2.0 * near) / (far + near - z * (far - near));
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalDirLight(dirLight, norm, viewDir);
	result += CalPointLight(pointLight, norm, viewDir);
	color = texture(texture_diffuse1, TexCoord);
	//float depth = LinearizeDepth(gl_FragCoord.z);

	//color = vec4(depth, depth, depth,1.f);
}