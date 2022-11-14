#version 330 core

uniform sampler2D groundTex;
uniform sampler2D grassTex;
uniform sampler2D snowTex;

uniform vec3 cameraPos;
uniform vec4 specColour;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 lightDirection;
uniform float lightRadius;
uniform int lightType; // 0 - Point, 1 - Directional, 2 - Spot 

in Vertex {
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 pos;
} IN;

out vec4 fragColour;
void main(void) {
	
	fragColour = texture(groundTex , IN.texCoord );


	if(IN.pos.y > 170)
		fragColour = texture(snowTex, IN.texCoord);
	else if(IN.pos.y > 150)
		mix(texture(snowTex, IN.texCoord), texture(groundTex, IN.texCoord),  1-((IN.pos.y - 150)/30));

	if(IN.pos.y < 10 )
		fragColour = texture(grassTex, IN.texCoord );
	else if (IN.pos.y < 20)
		fragColour = mix( texture(grassTex, IN.texCoord ), texture(groundTex, IN.texCoord), 1-(10/IN.pos.y) );

	vec3 incident = normalize(lightDirection);
	vec3 viewDir = normalize(cameraPos - IN.pos );
	vec3 halfDir = normalize(incident + viewDir );
	vec4 diffuse = fragColour;

	float lambert = max(dot(incident , IN.normal), 0.0f);
	float specFactor = clamp(dot(halfDir , IN.normal ) ,0.0 ,1.0);
	specFactor = pow(specFactor , 50.0 );

	vec3 surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert;
	fragColour.rgb += (specColour.rgb * specFactor ) *0.33;
	fragColour.rgb += surface * 0.1f; // ambient!
	fragColour.a = diffuse.a;
}



