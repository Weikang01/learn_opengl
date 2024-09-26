#version 440 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environment_cubemap;

const float PI = 3.14159265359f;


void main()
{
    vec3 normal = normalize(WorldPos);

    vec3 up = vec3(0.f, 1.f, 0.f);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.025f;
    float nrSamples = 0.f;
    vec3 irradiance = vec3(0.f);
    for (float phi = 0; phi < 2.f * PI; phi+= sampleDelta)
    {
        for (float theta = 0; theta < .5f * PI; theta+= sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            irradiance += texture(environment_cubemap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples+= 1.f;
        }
    }
    irradiance = PI * irradiance * (1.f / nrSamples);
    FragColor = vec4(irradiance, 1.f);
}
