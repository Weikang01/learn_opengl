#version 440 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environment_cubemap;
uniform float roughness;

const float PI = 3.14159265359f;

// ----------------------------------------------------------------------------
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float radicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint count)
{
    return vec2(float(i) / float(count), radicalInverse_VdC(i));
}

vec3 importanceSampleGGX(vec2 Xi, vec3 normal, float roughness)
{
    float a = roughness * roughness;
    float phi = 2.f * PI * Xi.x;
    float cosTheta = sqrt((1.f - Xi.y) / (1.f + (a * a - 1.f) * Xi.y));
    float sinTheta = sqrt(1.f - cosTheta * cosTheta);

    // from spherical coordinates to cartesian coordinates
    vec3 halfwayDir;
    halfwayDir.x = cos(phi) * sinTheta;
    halfwayDir.y = sin(phi) * sinTheta;
    halfwayDir.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    vec3 up = abs(normal.z) < 0.999f ? vec3(0.f, 0.f, 1.f) : vec3(1.f, 0.f, 0.f);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);

    vec3 sampleVec = tangent * halfwayDir.x + bitangent * halfwayDir.y + normal * halfwayDir.z;
    return normalize(sampleVec);
}

float distrubutionGGX(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float a2 = alpha * alpha;
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.f) + 1.f);
    denom = PI * denom * denom;

    return a2 / denom;
}


void main()
{
    vec3 normal = normalize(WorldPos);
    vec3 viewDir = normal;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.f;
    vec3 prefiltered = vec3(0.f);
    for (uint i = 0u; i < SAMPLE_COUNT; i++)
    {
        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 halfwayDir = importanceSampleGGX(Xi, normal, roughness);
        vec3 lightDir = normalize(2.f * dot(viewDir, halfwayDir) * halfwayDir - viewDir);

        float NdotL = max(dot(normal, lightDir), 0.f);
        if (NdotL > 0.f)
        {
            float NdotH = max(dot(normal, halfwayDir), 0.f);
            float HdotV = max(dot(halfwayDir, viewDir), 0.f);
            float D = distrubutionGGX(NdotH, roughness);
            float pdf = (D * NdotH / (4.f * HdotV));

            float resolution = 512.f; // resolution of source cubemap (per face)
            float saTexel = 4.f * PI / (6.f * resolution * resolution);
            float saSample = 1.f / (float(SAMPLE_COUNT) * pdf + 0.0001f);

            float mipLevel = roughness == 0.f ? 0.f : .5f * log2(saSample / saTexel);

            prefiltered += textureLod(environment_cubemap, lightDir, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    prefiltered = prefiltered / totalWeight;
    FragColor = vec4(prefiltered, 1.f);
}
