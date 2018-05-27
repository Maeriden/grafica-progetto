#version 330 core

out vec2 uv;


void main()
{
	// https://www.saschawillems.de/?page_id=2122
	uv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
	gl_Position = vec4(uv*2.0f - 1.0f, 0.0f, 1.0f);
}
