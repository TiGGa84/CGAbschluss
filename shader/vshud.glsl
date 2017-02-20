#version 400
layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec2 VertexTexcoord;

out vec2 Texcoord;

uniform vec2 TexScale = vec2(1);
uniform vec2 TexOffset = vec2(0);
uniform mat4 Mat;

void main()
{
    Texcoord = VertexTexcoord * TexScale + TexOffset;
    gl_Position = Mat * VertexPos;
}

