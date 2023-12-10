#version 330 core

uniform vec3 color;
uniform sampler2D tex;

out vec4 fColor;

void main()
{	
	fColor = vec4(color, 1.0f);
}