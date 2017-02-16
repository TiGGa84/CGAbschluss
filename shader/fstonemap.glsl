#version 400

in vec2 Texcoord;

out vec4 FragColor;

uniform sampler2D colorTex;
uniform sampler2D bloomTex;


void main()
{             
    const float gamma = 2.2;
	const float exposure = 1.0;

    vec3 hdrColor = texture(colorTex, Texcoord).rgb;
	vec3 bloomColor = texture(bloomTex, Texcoord).rgb;

	hdrColor += bloomColor;

    // reinhard
    //vec3 result = hdrColor / (hdrColor + vec3(1.0));

    // exposure
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correct       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}