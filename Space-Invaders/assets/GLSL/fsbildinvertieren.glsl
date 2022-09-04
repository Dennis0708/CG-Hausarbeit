#version 400
in vec2 Texcoord;

out vec4 FragColor;

uniform sampler2D Tex0;

void main()
{
	//FragColor = vec4(texture(Tex0, Texcoord).rgb + vec3(30,10,0),0);
	//FragColor = vec4(1.0)-vec4(texture(Tex0, Texcoord).rgb,0);
	FragColor = vec4(1,0,1,0);
}
