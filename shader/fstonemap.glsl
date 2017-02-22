#version 400

in vec2 Texcoord;

out vec4 FragColor;

uniform float offset;
uniform sampler2D colorTex;
uniform sampler2D bloomTex;

const float gamma = 2.2;
const float exposure = 1.05;

void main()
{
	vec2 Texoffset = vec2(offset, 0.0);
	
	vec3 hdrColor = vec3(
		texture(colorTex, Texcoord + Texoffset).r,
		texture(colorTex, Texcoord).g,
		texture(colorTex, Texcoord - Texoffset).b
	);
	vec3 bloomColor = texture(bloomTex, Texcoord).rgb;

	hdrColor += bloomColor;

	// exposure
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	// gamma correct       
	result = pow(result, vec3(1.0 / gamma));

	FragColor = vec4(result, 1.0);
}