#version 440 core
in vec2 TexCoord;
out vec2 FragColor;

const float PI = 3.14159265359f;

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

float geometrySchlick(float NdotV, float roughness)
{
	float r = roughness;
	float k = (r * r) / 2.f;

	float num = NdotV;
	float denom = NdotV * (1.f - k) + k;
	return num / denom;
}

float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness)
{
	float NdotV = max(dot(normal, viewDir), 0.f);
	float NdotL = max(dot(normal, lightDir), 0.f);

	float ggx1 = geometrySchlick(NdotV, roughness);
	float ggx2 = geometrySchlick(NdotL, roughness);
	return ggx1 * ggx2;
}

vec2 integrateBRDF(float NdotV, float roughness)
{
	vec3 viewDir;
	viewDir.x = sqrt(1.f - NdotV * NdotV);
	viewDir.y = 0.f;
	viewDir.z = NdotV;

	float A = 0.f;
	float B = 0.f;

	vec3 normal = vec3(0.f, 0.f, 1.f);
	const uint SAMPLE_COUNT = 1024u;

	for (uint i = 0u; i < SAMPLE_COUNT; i++)
	{
		vec2 Xi = hammersley(i, SAMPLE_COUNT);
		vec3 halfwayDir = importanceSampleGGX(Xi, normal, roughness);
		vec3 lightDir = normalize(2.f * dot(viewDir, halfwayDir) * halfwayDir - viewDir);

		float NdotL = max(lightDir.z, 0.f);

		if (NdotL > 0.f)
		{
			float NdotH = max(halfwayDir.z, 0.f);
			float HdotV = max(dot(halfwayDir, viewDir), 0.f);
			float G = geometrySmith(normal, viewDir, lightDir, roughness);
			float G_Vis = (G * HdotV) / (NdotH * NdotV);
			float Fc = pow(1.0 - HdotV, 5.0);

			A += (1.0 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}

	}

	A /= float(SAMPLE_COUNT);
	B /= float(SAMPLE_COUNT);
	return vec2(A, B);
}

void main()
{
	vec2 integratedBRDF = integrateBRDF(TexCoord.x, TexCoord.y);
	FragColor = integratedBRDF;
}