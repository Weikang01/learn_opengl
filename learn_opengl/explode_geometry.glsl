#version 440 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float time;

out vec2 TexCoords;

in VS_OUT{
	vec2 texCoords;
}gs_in[];

float pi = 3.14159f;

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = .03f;
	float freq = 3.f / 2.f + 6.f / pi * sin(time) + 6.f / (3 * pi) * sin(3 * time) + 6.f / (5 * pi) * sin(5 * time);
	vec3 direction = normal * freq * magnitude;
	return position + vec4(direction, 0.f);
}

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	TexCoords = gs_in[0].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	TexCoords = gs_in[1].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	TexCoords = gs_in[2].texCoords;
	EmitVertex();
	EndPrimitive();
}