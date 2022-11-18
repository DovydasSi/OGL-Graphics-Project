#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec4 tangent;



out Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 pos;
} OUT;

void main(void) {
	vec4 localPos = vec4(position , 1.0f);

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(localPos.xyz , 1.0);

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix )));

	vec3 wNormal = normalize(normalMatrix * normalize(normal ));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz ));

	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent , wNormal) * tangent.w;

	OUT.texCoord = texCoord;
	OUT.pos = position;
}