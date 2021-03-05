#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

//uniform vec3 lightPos;
uniform mat3 normalMat;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform vec3 lightColor;

out vec3 FragPos;
out vec3 Normal;
//out vec3 LightPos;
out vec2 TexCoord;
//out vec3 VecSpec;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.f);
	FragPos = vec3(model * vec4(position, 1.f));
	Normal = normalMat * normal;
	//LightPos = vec3(view * vec4(lightPos, 1.f));
	TexCoord = texCoord;
	//vec3 norm = normalize(Normal);
	//float specularStrength = .5f;
	//vec3 viewDir = normalize(-FragPos);
	//vec3 lightDir = normalize(LightPos - FragPos);
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), .0f), 32.f);
	//VecSpec = specularStrength * spec * lightColor;
}