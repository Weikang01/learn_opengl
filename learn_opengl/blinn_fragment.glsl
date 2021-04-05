#version 440

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}fs_in;

layout(std140) uniform Gamma
{
	float gamma;
};

uniform vec3 viewPos;
uniform vec3 lightPos;  // this version is for dir light
uniform sampler2D texture_diffuse1;
uniform bool blinn;

out vec4 color;

void main()
{
	vec3 text_color = texture(texture_diffuse1, fs_in.TexCoord).rgb;

	//float distance = length(lightPos - fs_in.FragPos);
	//float attenuation = 1.f / (distance);
	//vec3 text_color = vec3(fs_in.TexCoord,0.f);
	// Ambient
	vec3 ambient    = .05f * text_color;
	// Diffuse
	vec3 lightDir   = normalize(lightPos);
	vec3 normal     = normalize(fs_in.Normal);
	float diff      = max(dot(lightDir, normal), 0.f);
	vec3 diffuse    = text_color * diff;

	// Specular
	//vec3 viewDir    = normalize(viewPos - fs_in.FragPos);
	//vec3 reflectDir = reflect(-lightDir, normal);
	//float spec      = 0.f;
	//if (blinn)
	//{
	//	vec3 halfwayDir = normalize(lightDir + viewDir);
	//	spec = pow(max(dot(normal, halfwayDir), 0.f), 32.f);
	//}
	//else {
	//	vec3 reflectDir = reflect(-lightDir, normal);
	//	spec = pow(max(dot(viewDir, reflectDir), 0.f), 32.f);
	//}
	//vec3 specular = vec3(0.3) * spec;

	color = vec4(ambient + diffuse, 1.f);
	color.rgb = pow(color.rgb, vec3(1.f/ gamma));
}