#version 400
/**
* https://learnopengl.com/In-Practice/2D-Game/Postprocessing
*/

layout (location = 0) in vec4 inPos; // <vec2 position>
layout (location = 1) in vec4 inTexCoords; // <vec2 texCoords>

out vec2 Texcoord;

uniform bool  shake;
uniform float time;

void main()
{
    gl_Position = vec4(inPos.xy, 0.0f, 1.0f); 
    Texcoord = inTexCoords.xy;

    if (shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
}

