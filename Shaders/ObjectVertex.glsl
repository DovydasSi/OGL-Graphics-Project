#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;
in vec4 jointWeights;
in ivec4 jointIndices;

uniform mat4 joints [128];

out Vertex {
vec2 texCoord;
vec4 weights;
} OUT;

void main(void) {
	vec4 localPos = vec4(position , 1.0f);

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(localPos.xyz , 1.0);
	OUT.texCoord = texCoord;
}