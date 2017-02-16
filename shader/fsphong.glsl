#version 400

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform vec3 EmitColor = vec3(6.0);
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;
uniform sampler2D EmitTexture;

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{
    vec4 DiffTex = texture(DiffuseTexture, Texcoord);
    vec4 EmitTex = texture(EmitTexture, Texcoord);

    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPos - Position);
    vec3 E = normalize(EyePos - Position);
    vec3 R = reflect(-L,N);
    vec3 DiffuseComponent = LightColor * DiffuseColor * sat(dot(N,L));
    vec3 SpecularComponent = LightColor * SpecularColor * pow(sat(dot(R,E)), SpecularExp);

    FragColor = vec4((DiffuseComponent + AmbientColor) * DiffTex.rgb + SpecularComponent + EmitTex.rgb * EmitColor, 1.0);
    BloomColor = vec4(0, 0, 0, 1);

	// sehr helle Werte in bloom Buffer schreiben
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BloomColor = FragColor;
}
