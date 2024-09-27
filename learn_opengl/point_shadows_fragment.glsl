#version 440 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;

bool debug = false;
bool PCF = true;

vec3 sampleOffsetDirections[20] = vec3[]
(
    vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
    vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
    vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
    vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    float shadow = 0.f;
    float bias = 0.15;
    if (PCF)
    {
        int samples = 20;
        float viewDistance = length(viewPos - fragPos);
        float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
        for (int i = 0; i < samples; i++)
        {
                float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
                closestDepth *= far_plane;
                if (currentDepth - bias > closestDepth)
                    shadow += 1.f;
        }
        shadow /= float(samples);
    }
    else {
        // ise the fragment to light vector to sample from the depth map    
        float closestDepth = texture(depthMap, fragToLight).r;
        // it is currently in linear range between [0,1], let's re-transform it back to original depth value
        closestDepth *= far_plane;

        //float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
        shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }

    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // calculate shadow
    if (debug)
    {
        float closestDepth = texture(depthMap, fs_in.FragPos - lightPos).r;
        closestDepth *= far_plane;
        FragColor = vec4(vec3(closestDepth / far_plane), 1.0);
    }
    else
    {
        float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;
        vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
        FragColor = vec4(lighting, 1.0);
    }


}