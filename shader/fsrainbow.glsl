#version 400

in vec2 Texcoord;

out vec4 FragColor;

uniform float GlobalTime;

const float dotRadius = 3.5;
const float dotSpace = 10.5;
const float PI_4 = 0.785398163;

float circleDist(vec2 p, vec2 center) {
    return distance(center, p) - dotRadius;
}

vec3 rainbowColor(float time, vec2 uv) {
    return vec3(
        abs(sin(time - uv.y)),
        abs(cos(time - uv.x)),
        abs(sin(time + PI_4 - uv.x))
    );
}

void main()
{

    vec2 uv = Texcoord;
    float time = GlobalTime / 4.0;
   
    // Find the nearest center
    vec2 dotCenter = dotSpace * floor(gl_FragCoord.xy / dotSpace) + dotSpace * 0.5;
    
    vec3 dots = mix(rainbowColor(time, uv), vec3(0.0), clamp(circleDist(gl_FragCoord.xy, dotCenter), 0.0, 1.0));
    
    // Vignette
    vec2 viguv = uv * (vec2(1.0) - uv);
    float vig = pow(viguv.x * viguv.y * 10.0, 0.5);
    
    FragColor = vec4(dots * 0.5 * vig, 1.0);
}