#version 440 core
out vec4 color;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
}fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 normal;
vec3 lightDir;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoord = projCoord * .5f + .5f;
	float closestDepth = texture(shadowMap, projCoord.xy).r;
	float currentDepth = projCoord.z;
	float bias = max(0.05f * (1.f - dot(normal, lightDir)), 0.005f);
	float shadow = 0.f;
	if (projCoord.z < 1.f)
	{
		vec2 texelSize = 1.f / textureSize(shadowMap, 0);
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				float pcfDepth = texture(shadowMap, projCoord.xy + vec2(x, y) * texelSize).r;
				shadow += currentDepth - bias > pcfDepth ? 1.f : 0.f;
			}
		}
		shadow /= 9.f;
	}

	return shadow;
}

void main()
{
	vec3 textColor  = texture(texture_diffuse1, fs_in.TexCoord).rgb;
	normal     = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.f);
	// ambient
	vec3 ambient    = vec3(.3f);

	// diffuse
	lightDir   = normalize(lightPos - fs_in.FragPos);
	float diff      = max(dot(lightDir, normal), 0.f);
	vec3 diffuse    = lightColor * diff;

	// specular
	vec3 viewDir    = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec      = pow(max(dot(halfwayDir, normal), 0.f), 32.f);
	vec3 specular   = lightColor * spec;

	// shadow
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	vec3 lighting = (ambient + (1.f - shadow) * (diffuse + specular)) * textColor;

	color = vec4(lighting, 1.0f);
}