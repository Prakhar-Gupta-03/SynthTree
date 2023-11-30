#version 330 core

layout (location = 0) in vec3 vVertex;
layout (location = 1) in vec2 texCord;

out vec2 texcord;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

void main() {
	texcord = texCord;
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
}
