#version 330 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix; 

in vec3 position;
in vec2 texCoord;
in vec3 normal; 
in vec4 tangent;
in vec4 colour;

out Vertex {
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 pos;
} OUT;

void main(void) 
{
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position , 1.0);

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix )));

	vec3 wNormal = normalize(normalMatrix * normalize(normal ));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz ));

	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent , wNormal) * tangent.w;

	vec4 worldPos = (modelMatrix * vec4(position, 1));

	OUT.texCoord = (textureMatrix * vec4(texCoord , 0.0, 1.0)).xy;
	OUT.colour = colour;
	OUT.pos = position;
}