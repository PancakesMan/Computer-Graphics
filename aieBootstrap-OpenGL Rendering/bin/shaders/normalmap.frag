#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoord;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ia; // ambient light colour
//uniform vec3 Id; // diffuse light colour
//uniform vec3 Is; // specular light colour
//uniform vec3 LightDirection;
uniform mat3 Light;

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power

uniform float roughness;
uniform float reflectionCoefficient;

uniform vec3 cameraPosition;

void main() {
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	//vec3 L = normalize(LightDirection);
	vec3 L = normalize(Light[0]);
	
	mat3 TBN = mat3(T, B, N);
	
	vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
	vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
	vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
	
	N = TBN * (texNormal * 2 - 1);
	float lambertTerm = max(0, min(1, dot(N, -L)));
	
	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );
	
	// calculate specular term
	float specularTerm = pow(max(0, dot(R, V)), specularPower);
	
	float NdL = max( 0.0f, dot ( N, L ) );
	float NdE = max( 0.0f, dot ( N, V ) );
	float R2 = roughness * roughness;
	vec3 H = normalize( L + V); // light and view half vector
	float NdH = max(dot(N,H), 00f);
	float NdH2 = NdH*NdH;
	float e = 2.71828182845904523536028747135f;
	float pi = 3.1415926535897932384626433833f;
	
	float exponent = -(1 - NdH2) / (NdH2 * R2);
	float D = pow(e, exponent) / (R2 * NdH2 * NdH2);
	
	float F = reflectionCoefficient + (1 - reflectionCoefficient) * pow(1 - NdE, 5);
	float X = 2.0f * NdH / dot(V, H);
	float G = min(1, min(X * NdE, X * NdL));
	
	float CookTorrance = max((D*G*F) / (NdE * pi), 0.0f);
	
	// calculate Oran-Nayar diffuse term
	float AX = 1.0f - 0.5f * R2 / (R2 + 0.33f);
	float BX = 0.45f * R2 / (R2 + 0.09f);
	
	// CX = max(theta, cos(l, e);
	vec3 lightProjected = normalize( L - N * NdL );
	vec3 viewProjected = normalize( V - N * NdE );
	float CX = max( 0.0f, dot( lightProjected, viewProjected ) );
	
	float alpha = sin(max(acos(NdE), acos(NdL)));
	float beta = tan(min(acos(NdE), acos(NdL)));
	float DX = alpha * beta;
	
	float OrenNayar = NdL * (AX + BX * CX * DX);
	
	// calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Light[1] * Kd * texDiffuse * OrenNayar; //lambertTerm;
	vec3 specular = Light[2] * Ks * texSpecular * CookTorrance; //specularTerm;
	
	FragColour = vec4(ambient + diffuse + specular, 1);
}