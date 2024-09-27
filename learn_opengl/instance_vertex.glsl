#version 440 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 normal;

out vec3 fColor;

uniform vec2 offsets[100];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    vec2 pos = position * (gl_InstanceID / 100.f);
    gl_Position = vec4(pos + offset, 0.f, 1.f);
    fColor = normal;
}
