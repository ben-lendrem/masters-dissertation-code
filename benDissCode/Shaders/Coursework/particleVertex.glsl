#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform vec4 particleColour;

in vec3 position;

out Vertex {
	vec4 colour;
} OUT;

void main(void) {
	gl_Position = (projMatrix * viewMatrix * modelMatrix) *
					vec4(position, 1.0);
	OUT.colour = particleColour;
}