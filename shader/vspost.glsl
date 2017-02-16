#version 400
layout(location = 0) in vec3 VertexPos;

out vec2 Texcoord;

void main()
{
    Texcoord = VertexPos.xy * vec2(0.5) + vec2(0.5);
    gl_Position = vec4(VertexPos, 1.0);
}

