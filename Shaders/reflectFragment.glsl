#version 330 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform vec3 lightDirection;
uniform vec3 cameraPos;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {
	vec4 diffuse = texture(diffuseTex , IN.texCoord );
	vec3 viewDir = normalize(cameraPos - vec3(IN.worldPos.x, IN.worldPos.y,IN.worldPos.z) );


	vec3 reflectDir = reflect(-viewDir, normalize(IN.normal ));
	vec4 reflectTex = texture(cubeTex, reflectDir );

	vec3 lightDir = lightDirection;
	lightDir.y = lightDir.y * -1;
	vec3 incident = normalize(lightDir);
	vec3 halfDir = normalize(incident + viewDir );

	float R = 0.05;
	float F = R + (1 - R) * pow(1 - dot(halfDir, viewDir), 5); 

	fragColour = reflectTex * (1-F) + (diffuse * 0.4f);
}