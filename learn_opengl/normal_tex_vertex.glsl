#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT{
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

layout(std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;
uniform mat3 normalMat;

void main()
{
    vs_out.TexCoord = texCoord;

    vec3 T = normalize(normalMat * tangent);
    vec3 N = normalize(normalMat * normal);

    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * vec3(model * vec4(position, 0.0));

    gl_Position = projection * view * model * vec4(position, 1.0);
}
