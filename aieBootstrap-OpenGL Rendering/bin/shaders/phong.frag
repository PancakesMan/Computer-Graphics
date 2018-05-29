#version 410

in vec3 cameraPosition;
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D diffuseTexture;

uniform vec3 Ia; // ambient light colour
uniform vec3 Id; // diffuse light colour
uniform vec3 Is; // specular light colour
uniform vec3 LightDirection;

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power

out vec4 FragColour;

void main() {
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	
	float lambertTerm = max(0, min(1, dot(N, -L)));
	
	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );
	
	// calculate specular term
	float specularTerm = pow(max(0, dot(R, V)), specularPower);
	
	// calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm;
	
	FragColour = vec4(ambient + diffuse, 1) * texture( diffuseTexture, vTexCoord );
}