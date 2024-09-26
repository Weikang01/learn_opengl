#version 440 core
out vec4 color;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

#define LIGHT_SIZE 4

uniform vec3[LIGHT_SIZE] lightsPos;
uniform vec3[LIGHT_SIZE] lightsColors;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
vec3 lightColor = vec3(1.f);

vec3 normal;
vec3 viewDir;


// attenuation coefficients
float const_ = 1.f;
float linear_ = .14f;
float quadratic_ = .07f;


vec3 CalculateLighting(vec3 lightPos, vec3 lightColor)
{
    vec3 ambient = vec3(.1f) * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.f);
    vec3 diffuse = lightColor * diff;

    float distance = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0f / (const_ + linear_ * distance + quadratic_ * (distance * distance));

    return (diffuse + ambient) * attenuation;
}


void main()
{
    normal = normalize(fs_in.Normal);
    viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 texColor = texture(texture_diffuse1, fs_in.TexCoord).rgb;

    vec3 lighting = vec3(0.f);

    for (int i = 0; i < LIGHT_SIZE; i++)
        lighting += CalculateLighting(lightsPos[i], lightsColors[i]);
    lighting *= texColor;

    color = vec4(lighting, 1.f);
}
