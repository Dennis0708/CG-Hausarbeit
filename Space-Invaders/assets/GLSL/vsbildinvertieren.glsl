#version 400
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 Texcoord;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f); 
    vec2 Texcoord = vertex.zw;
}

