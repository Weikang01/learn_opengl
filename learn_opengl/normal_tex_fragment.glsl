#version 440 core
out vec4 color;

in VS_OUT{
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
vec3 lightColor = vec3(1.f);

void main()
{
    vec3 texColor = texture(texture_diffuse1, fs_in.TexCoord).rgb;
    vec3 normal = texture(texture_normal1, fs_in.TexCoord).rgb;
    normal = normalize(normal * 2.f - 1.f);

    // ambient
    vec3 ambient = vec3(.3f) * lightColor;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.f);
    vec3 diffuse = lightColor * diff;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.f), 32.f);
    vec3 specular = lightColor * spec;
    
    vec3 lighting = (ambient + diffuse + specular) * texColor;
    color = vec4(lighting, 1.f);
}
