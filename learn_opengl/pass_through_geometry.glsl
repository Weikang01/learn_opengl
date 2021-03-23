#version 440
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT{
	vec3 color;
	vec2 TexCoord;
}gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{
	fColor = gs_in[0].color;
	gl_Position = position + vec4(-.2f, -.2f, 0.f, 0.f);
	EmitVertex();

	gl_Position = position + vec4(.2f, -.2f, 0.f, 0.f);
	EmitVertex();

	gl_Position = position + vec4(-.2f, .2f, 0.f, 0.f);
	EmitVertex();

	gl_Position = position + vec4(.2f, .2f, 0.f, 0.f);
	EmitVertex();

	gl_Position = position + vec4(.0f, .4f, 0.f, 0.f);
	fColor = vec3(1.f);
	EmitVertex();
}


void main()
{

	build_house(gl_in[0].gl_Position);
	EndPrimitive();
}