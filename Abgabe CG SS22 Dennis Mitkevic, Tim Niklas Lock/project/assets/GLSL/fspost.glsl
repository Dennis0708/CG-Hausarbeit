#version 400
/**
* https://learnopengl.com/In-Practice/2D-Game/Postprocessing
*/

in vec2 Texcoord;

out vec4 FragColor;

uniform sampler2D Tex0;
uniform vec2 offsets[9];
uniform float blur_kernel[9];

uniform bool shake;

void main()
{
	FragColor = vec4(0.0f);
    vec3 sampl[9];
    // sample from texture offsets if using convolution matrix
	if (shake)
    {
        for(int i = 0; i < 9; i++) {
            sampl[i] = vec3(texture(Tex0, Texcoord.xy + offsets[i]));
        }
        for(int i = 0; i < 9; i++) {
            FragColor += vec4(sampl[i] * blur_kernel[i], 0.0f);
        }
        FragColor.a = 1.0f;
    }
    else
    {
        FragColor =  texture(Tex0, Texcoord);
    }
}
