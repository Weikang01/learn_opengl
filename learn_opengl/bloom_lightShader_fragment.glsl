#version 440 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0f)
        BrightColor = vec4(lightColor, 1.f);
    else
        BrightColor = vec4(0.f, 0.f, 0.f, 1.f);
}