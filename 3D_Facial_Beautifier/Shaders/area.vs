#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 vertexColor;

varying vec4 finalcolor;

void main()
{
    gl_Position = vec4(vec2(position), 0.0f, 1.0f);
	finalcolor = vertexColor;
}