#version 440 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

#define LIGHT_SIZE 4

uniform vec3[LIGHT_SIZE] lightPositions;
uniform vec3[LIGHT_SIZE] lightColors;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

// attenuation coefficients
float const_ = 1.f;
float linear_ = .14f;
float quadratic_ = .07f;


void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 texColor = texture(texture_diffuse1, fs_in.TexCoord).rgb;

    vec3 lighting = vec3(0.f);

    for (int i = 0; i < LIGHT_SIZE; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lightPositions[i] - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.f);
        vec3 diffuse = lightColors[i] * diff * texColor;

        float distance = length(lightPositions[i] - fs_in.FragPos);
        //float attenuation = 1.0f / (const_ + linear_ * distance + quadratic_ * (distance * distance));
        float attenuation = 1.0f / (distance * distance);

        lighting += diffuse * attenuation;
    }
    //lighting *= texColor;
    float brightness = dot(lighting, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.f)
        BrightColor = vec4(lighting, 1.f);
    else
        BrightColor = vec4(0.f, 0.f, 0.f, 1.f);
    FragColor = vec4(lighting, 1.f);
}
