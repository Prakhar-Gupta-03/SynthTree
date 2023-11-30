#version 330 core

uniform vec3 color;
uniform sampler2D tex;

in vec2 texcord;

out vec4 fColor;

void main()
{	
	fColor = vec4(color, 1.0f);
	fColor = texture(tex, texcord);
}