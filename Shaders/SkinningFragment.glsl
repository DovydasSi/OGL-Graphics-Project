#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;

uniform vec3 lightDirection;
uniform vec3 cameraPos;
uniform vec4 specColour;
uniform vec4 lightColour;

in Vertex {
	vec2 texCoord;
	vec4 weights;
	vec3 normal;
	vec3 pos;
} IN;

out vec4 fragColour;
void main(void) {
	fragColour = texture(diffuseTex , IN.texCoord );

	// Directional light
	vec3 incident = normalize(lightDirection);
	vec3 viewDir = normalize(cameraPos - IN.pos );
	vec3 halfDir = normalize(incident + viewDir );
	vec4 diffuse = fragColour;

	float lambert = max(dot(incident , IN.normal), 0.0f);
	float specFactor = clamp(dot(halfDir , IN.normal ) ,0.0 ,1.0);
	specFactor = pow(specFactor , 50.0 );

	vec3 surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert;
	fragColour.rgb += (specColour.rgb * specFactor ) *0.25;
	fragColour.rgb += surface * 0.1f; 
	fragColour.a = diffuse.a;
}