#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out VS_OUT{
    vec3 normal;
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
    vs_out.normal = normalize(vec3(projection * vec4(normalMat * normal, 1.0)));
    gl_Position = projection * view * model * vec4(position, 1.0);
}
