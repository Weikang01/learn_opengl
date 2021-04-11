#version 440 core
out vec4 color;

uniform vec3 lightColor;
uniform sampler2D gPosition;

void main()
{
    color = vec4(lightColor, 1.f);
}