#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

uniform vec3 lightDirection;
uniform vec3 cameraPos;
uniform vec4 specColour;
uniform vec4 lightColour;

in Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 pos;
} IN;

out vec4 fragColour;
void main(void) {
	fragColour = texture(diffuseTex , IN.texCoord );
	fragColour.a = 1.0; 

	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal ));

	vec3 incident = normalize(lightDirection);
	vec3 viewDir = normalize(cameraPos - IN.pos );
	vec3 halfDir = normalize(incident + viewDir );
	vec4 diffuse = fragColour;

	vec3 bumpNormal = texture(bumpTex , IN.texCoord ).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	float lambert = max(dot(incident, IN.normal), 0.0f);
	float specFactor = clamp(dot(halfDir, bumpNormal ) ,0.0 ,1.0);
	specFactor = pow(specFactor, 100.0 );

	vec3 surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert;
	fragColour.rgb += (specColour.rgb * specFactor ) *0.15;
	fragColour.rgb += surface * 0.1f; // ambient!
	fragColour.a = diffuse.a;
}