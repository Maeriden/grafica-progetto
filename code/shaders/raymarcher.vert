#version 330 core

out vec2 uv;


void main()
{
	// https://www.saschawillems.de/?page_id=2122
	float u = (gl_VertexID << 1) & 2;
	float v = gl_VertexID & 2;
	
	uv = vec2(u, v);
	gl_Position = vec4(u*2.0f - 1.0f,
	                   v*2.0f - 1.0f,
	                   1.0f,
	                   1.0f);
}
