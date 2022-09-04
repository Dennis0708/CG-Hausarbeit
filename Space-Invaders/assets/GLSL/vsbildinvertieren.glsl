#version 400
layout (location = 0) in vec2 inPos; // <vec2 position>
layout (location = 1) in vec2 inTexCoords; // <vec2 texCoords>

out vec2 Texcoord;

void main()
{
    gl_Position = vec4(inPos, 0.0f, 1.0f); 
    Texcoord = inTexCoords;
}

