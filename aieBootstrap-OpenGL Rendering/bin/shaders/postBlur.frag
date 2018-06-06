#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

// simple box blur
vec4 BoxBlur(vec2 texCoord) {

	vec2 texel = 1.0f / textureSize(colourTarget, 0);
	
	// 9-tap box kernel
	vec4 colour = texture(colourTarget, texCoord);
	colour += texture(colourTarget, texCoord + texel * vec2(-1,1));
	colour += texture(colourTarget, texCoord + texel * vec2(-1,0));
	colour += texture(colourTarget, texCoord + texel * vec2(-1,-1));
	colour += texture(colourTarget, texCoord + texel * vec2(0,1));
	colour += texture(colourTarget, texCoord + texel * vec2(0,-1));
	colour += texture(colourTarget, texCoord + texel * vec2(1,1));
	colour += texture(colourTarget, texCoord + texel * vec2(1,0));
	colour += texture(colourTarget, texCoord + texel * vec2(1,-1));
	
	return colour / 9;
}

void main() {
	// calculate texel size
	vec2 texSize = textureSize( colourTarget, 0);
	vec2 texelSize = 1.0f / texSize;
	
	//adjust texture coordinate
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;
	
	//simple post effect
	FragColour = BoxBlur(texCoord);
}