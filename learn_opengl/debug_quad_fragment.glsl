#version 440 core
out vec4 color;
in vec3 TexCoord;

//uniform sampler2D depthMap;
uniform samplerCube depthCubeMap;
uniform float near_plane;
uniform float far_plane;

float linearizeDepth(float depth)
{
	float z = depth * 2.f - 1.f; // back to NDC
	return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
	//float depthValue = texture(depthCubeMap, TexCoord).r;
	color = vec4(texture(depthCubeMap, TexCoord).xyz, 1.0f);
	//color = vec4(vec3(depthValue), 1.0f); // orthographic
}