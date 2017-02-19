#version 400
layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec2 VertexTexcoord;

out vec2 Texcoord;

uniform vec2 TexScale;
uniform vec2 TexOffset;
uniform mat4 Mat;

void main()
{
    Texcoord = VertexTexcoord * TexScale + TexOffset;
    gl_Position = Mat * VertexPos;
}

