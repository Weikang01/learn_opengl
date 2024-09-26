#version 440 core
out vec4 fragColor;

#define LIGHT_NR 4

struct Light
{
    vec3 position;
    vec3 color;
};

uniform Light light[LIGHT_NR];


in VS_OUT{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
} fs_in;


uniform vec3 viewPos;
uniform vec3 albedo = vec3(.5f, 0.f, 0.f);
uniform float roughness;
uniform float metallic;
uniform float ao = 1.f;
uniform samplerCube prefilter_cubemap;
uniform samplerCube irradiance_cubemap;
uniform sampler2D texture_brdf;

vec3 normal;

const float PI = 3.14159265359f;

float blinn_phong_NDF(vec3 halfway, vec3 normal)
{
    float alpha = roughness * roughness;
    float a2 = alpha * alpha;
    float NdotH = max(dot(normal, halfway), 0.f);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.f) + 1.f);
    denom = PI * denom * denom;

    return a2 / denom;
}

float geometrySchlick(float NdotV)
{
    float r = (roughness + 1.f);
    float k = (r * r) / 8.f;

    float num = NdotV;
    float denom = NdotV * (1.f - k) + k;
    return num / denom;
}

float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir)
{
    float NdotV = max(dot(normal, viewDir), 0.f);
    float NdotL = max(dot(normal, lightDir), 0.f);

    float ggx1 = geometrySchlick(NdotV);
    float ggx2 = geometrySchlick(NdotL);
    return ggx1 * ggx2;
}

vec3 schlick_Fresnel(float cosTheta)
{
    vec3 F0 = vec3(.04f);
    F0 = mix(F0, albedo, metallic);
    return F0 + (1.f - F0) * pow(max(1.f - cosTheta, 0.f), 5.f);
}

vec3 schlick_Fresnel_roughness(float cosTheta)
{
    vec3 F0 = vec3(.04f);
    F0 = mix(F0, albedo, metallic);
    return F0 + (max(vec3(1.f - roughness), F0) - F0) * pow(max(1.f - cosTheta, 0.f), 5.f);
}

void main()
{
    normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 refl = reflect(-viewDir, normal);

    vec3 lighting = vec3(0.f);
    for (int i = 0; i < LIGHT_NR; i++)
    {
        vec3 lightDir = normalize(light[i].position - fs_in.FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        float distance = length(light[i].position - fs_in.FragPos);
        float attenuation = 1.f / (distance * distance);
        vec3 radiance = light[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = blinn_phong_NDF(halfwayDir, normal);
        float G = geometrySmith(normal, viewDir, lightDir);
        vec3 F = schlick_Fresnel(max(dot(halfwayDir, viewDir), 0.f));

        vec3 numerator = NDF * G * F;
        float denominator = 4.f * max(dot(normal, viewDir), 0.f) * max(dot(normal, lightDir), 0.f);
        vec3 specular = numerator / max(denominator, .001f);

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.f) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.f - metallic;
        // scale light by NdotL
        float NdotL = max(dot(normal, lightDir), 0.f);

        // add to outgoing radiance Lo
        lighting += (kD * albedo / PI + specular) * radiance * NdotL;
        //lighting += vec3(NDF);
        //lighting += vec3(light[i].color);
    }
    // ambient lighting (we now use IBL as the ambient term)

    vec3 fresnel = schlick_Fresnel_roughness(max(dot(normal, viewDir), 0.f));
    vec3 kS = fresnel;
    vec3 kD = 1.f - kS;
    kD *= 1.f - metallic;

    vec3 irradiance = texture(irradiance_cubemap, normal).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.f;
    vec3 prefilter = textureLod(prefilter_cubemap, refl, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(texture_brdf, vec2(max(dot(normal, viewDir), 0.f), roughness)).rg;
    vec3 specular = prefilter * (fresnel * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + lighting;
    color = color / (color + vec3(1.f));
    color = pow(color, vec3(1.f / 2.2f));

    fragColor = vec4(color, 1.f);
    //color = vec4(schlick_Fresnel(dot(normal, viewDir)), 1.f);
}