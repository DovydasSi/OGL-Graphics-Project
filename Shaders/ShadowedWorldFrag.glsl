#version 330 core

uniform sampler2D groundTex;
uniform sampler2D grassTex;
uniform sampler2D snowTex;
uniform sampler2D bumpTex;
uniform sampler2D shadowTex;

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
	vec3 tangent;
	vec3 binormal;
	vec3 pos;
	vec4 shadowProj;
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


	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal ));

	vec3 incident = normalize(lightDirection);
	vec3 viewDir = normalize(cameraPos - IN.pos );
	vec3 halfDir = normalize(incident + viewDir );
	vec4 diffuse = fragColour;

	vec3 bumpNormal = texture(bumpTex , IN.texCoord ).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	float lambert = max(dot(incident , IN.normal), 0.0f);
	float specFactor = clamp(dot(halfDir , bumpNormal ), 0.0, 1.0);
	specFactor = pow(specFactor , 50.0 );

	float shadow = 1.0; //New!
		
	vec3 shadowNDC = IN.shadowProj.xyz / IN.shadowProj.w;
	if(abs(shadowNDC.x) < 1.0f && abs(shadowNDC.y) < 1.0f && abs(shadowNDC.z) < 1.0f) 
	{
		vec3 biasCoord = shadowNDC *0.5f + 0.5f;
		float shadowZ = texture(shadowTex , biasCoord.xy).x;

		if(shadowZ < biasCoord.z) 
		{
			shadow = 0.0f;
		}
	}

	vec3 surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert;
	fragColour.rgb += (specColour.rgb * specFactor ) * 0.33;
	fragColour.rgb *= shadow;
	fragColour.rgb += surface * 0.1f; 
	fragColour.a = diffuse.a;
}