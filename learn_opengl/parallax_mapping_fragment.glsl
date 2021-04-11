#version 440 core
out vec4 color;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform bool parallax;
uniform float height_scale;

vec2 ParallaxMapping(vec2 texCoord, vec3 viewDir)
{
    float height = texture(texture_height1, texCoord).r;
    vec2 p = viewDir.xy * (height * height_scale);
    return texCoord - p;
}

vec2 SteepParallaxMapping(vec2 texCoord, vec3 viewDir)
{
    const float minLayers = 8;
    const float maxLayers = 32;
    const float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.f,0.f,0.f), viewDir)));

    float layerDepth = 1.f / numLayers;

    float currentLayerDepth = 0.f;
    
    vec2 P = viewDir.xy * height_scale;
    vec2 deltaTexCoord = P / numLayers;

    vec2 currentTexCoord = texCoord;
    float currentDepthMapValue = texture(texture_height1, currentTexCoord).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoord -= deltaTexCoord;
        currentDepthMapValue = texture(texture_height1, currentTexCoord).r;
        currentLayerDepth += layerDepth;
    }

    return currentTexCoord;
}

vec2 ParallaxOcclusionMapping(vec2 texCoord, vec3 viewDir)
{
    const float minLayers = 8;
    const float maxLayers = 32;
    const float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.f, 0.f, 0.f), viewDir)));

    float layerDepth = 1.f / numLayers;

    float currentLayerDepth = 0.f;

    vec2 P = viewDir.xy * height_scale;
    vec2 deltaTexCoord = P / numLayers;

    vec2 currentTexCoord = texCoord;
    float currentDepthMapValue = texture(texture_height1, currentTexCoord).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoord -= deltaTexCoord;
        currentDepthMapValue = texture(texture_height1, currentTexCoord).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoord + deltaTexCoord;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(texture_height1, prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);

    return weight * prevTexCoords + (1.f - weight) * currentTexCoord;
}

vec3 lightColor = vec3(1.f);

void main()
{
    vec3 viewDir  = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoord = fs_in.TexCoord;
    if (parallax)
    {
        texCoord = ParallaxOcclusionMapping(fs_in.TexCoord, viewDir);
        if (texCoord.x > 1.f || texCoord.y > 1.f || texCoord.x < 0.f || texCoord.y < 0.f)
            discard;
    }

    vec3 texColor = texture(texture_diffuse1, texCoord).rgb;
    vec3 normal   = texture(texture_normal1, texCoord).rgb;
    normal        = normalize(normal * 2.f - 1.f);

    // ambient
    vec3 ambient  = vec3(.1f) * lightColor;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff    = max(dot(lightDir, normal), 0.f);
    vec3 diffuse  = lightColor * diff;

    // specular
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float spec    = pow(max(dot(halfwayDir, normal), 0.f), 32.f);
    vec3 specular = lightColor * spec;

    vec3 lighting = (ambient + diffuse + specular) * texColor;

    //color = vec4((fs_in.TexCoord + 1.f) * 0.5f, .5f, 1.f);
    color = vec4(lighting, 1.f);
    //color = texture(texture_height1, texCoord);
}
