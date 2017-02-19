#version 400

in vec2 Texcoord;

out vec4 FragColor;

uniform sampler2D Tex;

void main()
{
    FragColor = texture(Tex, Texcoord);
}