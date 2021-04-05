#version 440 core
out vec4 color;

struct Material
{
	sampler2D texture_diffuse1;
	//sampler2D texture_diffuse2;
	//sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	//sampler2D texture_specular2;
	//sampler2D diffuse;
	//sampler2D specular;
	//sampler2D emission;
	float shininess;
};

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

struct SpotLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 front;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

# define NR_POINT_LIGHTS 3

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform mat4 view;
uniform Material material;
//uniform Light light;
uniform vec3 viewPos;
in vec3 FragPos;
in vec3 Normal;
//in vec3 LightPos;
in vec2 TexCoord;

//in vec3 VecSpec;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), .0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), .0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));
	return (ambient + diffuse + specular);
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	float distance = length(light.position.xyz - FragPos);
	float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	float diff = max(dot(normal, lightDir), .0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), .0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));

	return ((ambient + diffuse + specular) * attenuation);
}

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.front));
	if (theta >= light.outerCutOff)
	{
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

		float distance = length(light.position.xyz - FragPos);
		float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

		float diff = max(dot(normal, lightDir), .0f);

		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), .0f), material.shininess);

		vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord));
		vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));

		return ((ambient + diffuse + specular) * attenuation * intensity);
	}
	else
	{
		return ambient;
	}
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// directional light
	vec3 result = CalDirLight(dirLight, norm, viewDir);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalPointLight(pointLights[i], norm, viewDir);
	}
	result += CalSpotLight(spotLight, norm, viewDir);

	color = vec4(result, 1.f);

	//vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	//float theta = dot(lightDir, normalize(-light.front));
	//float epsilon = light.cutOff - light.outerCutOff;
	//float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);
	//if (theta >= light.outerCutOff)
	//{

	//	vec3 norm = normalize(Normal);
	//	vec3 lightDir;
	//	float attenuation = 1.f;
	//	if (light.position.w == .0f)          // directional light
	//	{
	//		lightDir = normalize(-light.position.xyz);
	//	}
	//	else if (light.position.w == 1.0f)    //  point light
	//	{
	//		lightDir = normalize(light.position.xyz - FragPos);
	//		float distance = length(light.position.xyz - FragPos);
	//		attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	//	}
	//}
	//else
	//{
	//	return vec4(ambient, 1.f);
	//}

	

	//vec3 lightDir = normalize(-light.direction);
	
	//float diff = max(dot(norm, lightDir),.0f);
	//vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

	//vec3 viewDir = normalize(viewPos - FragPos);
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), .0f), material.shininess);
	//vec3 texSpec = vec3(texture(material.specular, TexCoord));
	//vec3 specular = light.specular * spec * texSpec;
	//int f = 1;
	//if (texSpec.x > 0.f || texSpec.y > 0.f|| texSpec.z > 0.f)
	//{
	//	f = 0;
	//}
	//vec3 emission = vec3(texture(material.emission, TexCoord)) * f;
	//vec3 result = (diffuce + ambient + VecSpec) * objectColor;
	//float theta = dot(lightDir, normalize(-light.front));
	//float epsilon = light.cutOff - light.outerCutOff;
	//float intensity = clamp((theta - light.outerCutOff)/ epsilon, 0.f, 1.f);
	//if (theta >= light.outerCutOff)
	//{
	//	color = vec4(ambient + (diffuse + specular) * attenuation * intensity, 1.f);
	//	//color = vec4(light.outerCutOff, light.cutOff, light.cutOff, 1.f);
	//}
	//else
	//{
	//	color = vec4(ambient, 1.f);
	//}

}