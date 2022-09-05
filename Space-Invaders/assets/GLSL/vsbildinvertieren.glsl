#version 400
layout (location = 0) in vec3 inPos; // <vec3 position>
layout (location = 1) in vec2 inTexCoords; // <vec2 texCoords>

out vec2 Texcoord;

void main()
{
    Texcoord = inTexCoords;
    gl_Position = vec4(inPos.xy, 0.0f, 1.0f);
}

