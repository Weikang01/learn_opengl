#version 440 core
out float color;
in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

// parameters
#define SAMPLE_NR 64
#define radius .5f
#define bias .025f

uniform vec3 samples[SAMPLE_NR];
uniform float screen_width = 800.f;
uniform float screen_height = 600.f;

const vec2 noiseScale = vec2(screen_width / 4.f, screen_height / 4.f);

uniform mat4 projection;

void main()
{
	vec3 fragPos = texture(gPosition, TexCoord).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoord).rgb);
	vec3 noise = normalize(texture(texNoise, TexCoord * noiseScale).rgb);

	vec3 tangent = normalize(noise - normal * (dot(normal, noise)));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0f;

	for (int i = 0; i < SAMPLE_NR; i++)
	{
		// get sample position
		vec3 samplePos = TBN * samples[i]; // from tangent to view-space
		samplePos = fragPos + samplePos * radius;
	
		vec4 offset = vec4(samplePos, 1.f);
		offset = projection * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * .5f + .5f; // transform to range 0.0 - 1.0  
		float sampleDepth = texture(gPosition, offset.xy).z;

		float rangeCheck = smoothstep(0.f, 1.f, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias? 1.f : 0.f) * rangeCheck;
	}
	occlusion = 1.f - (occlusion / float(SAMPLE_NR));
	color = occlusion;
}