#version 400

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform vec3 EmitColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;
uniform sampler2D EmitTexture;

const int MAX_LIGHTS = 10;

const uint POINT = 0;
const uint DIRECTIONAL = 1;
const uint SPOT = 2;

struct Light
{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
};

layout (std140) uniform Lights 
{
	Light lights[MAX_LIGHTS];
    int LightCount;
};

float sat(in float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{
    vec4 DiffTex = texture(DiffuseTexture, Texcoord);
    vec4 EmitTex = texture(EmitTexture, Texcoord);
    
    vec3 N = normalize(Normal);
    
    vec3 DiffuseComp = AmbientColor;
    vec3 SpecularComp = vec3(0);
	vec3 EmitComp = EmitTex.rgb * EmitColor;
    
    for(int i = 0; i < LightCount; i++) {
        Light light = lights[i];
        
        // Licht-Richtung
        vec3 L;
        // Licht intensität (Farbe)
        vec3 Li;
		// Punktlicht
        if(light.Type == POINT) {
            L = normalize(light.Position - Position);
            float r = length(light.Position - Position);
            Li = light.Color / (r * r);
        } else if(light.Type == DIRECTIONAL) {
            // Direktionales Licht
            L = -light.Direction;
            Li = light.Color;
        } else if(light.Type == SPOT) {
            // Spot Licht
            L = normalize(light.Position - Position);
            float r = length(light.Position - Position);
            // Winkel zwischen Licht-Richtung und Spot-Richtung
            float sigma = acos(dot(-L, normalize(light.Direction)));
            Li = (light.Color / (r * r)) * (1 - sat((sigma - light.SpotRadius.x) / (light.SpotRadius.y - light.SpotRadius.x)));
        } else {
            continue;
        }
        
        vec3 E = normalize(EyePos - Position);
        vec3 H = normalize(L + E);
        DiffuseComp += Li * DiffuseColor * sat(dot(N, L));
        SpecularComp += Li * SpecularColor * pow(sat(dot(N, H)), SpecularExp);
    }
    
    FragColor = vec4(DiffuseComp * DiffTex.rgb + SpecularComp + EmitComp, 1.0);
	// Emissive Farbe in Bloom Buffer schreiben
    BloomColor = vec4(EmitComp, 1.0);
	// sehr helle Werte in Bloom Buffer schreiben
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BloomColor = FragColor;
}