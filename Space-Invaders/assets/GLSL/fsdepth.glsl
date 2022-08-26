#version 400
in vec4 Position;
out vec4 FragColor;
const int MAX_LIGHTS=14;
uniform vec3 EyePos;
uniform sampler2D ShadowMapTexture[MAX_LIGHTS];
uniform mat4 ShadowMapMat[MAX_LIGHTS];

void main()
{
	vec4 Pos = Position / Position.w;
	for(int i = 0; i < MAX_LIGHTS; i++){
		vec4 PosSM = ShadowMapMat[i] * vec4(Position.xyz,1);
		PosSM.xyz /= PosSM.w;
		PosSM = PosSM.xy * 0.5 + 0.5;
		vec4 DepthSM = texture(ShadowMapTexture[i], PosSM.xy);
		if(length(DepthSM) < length(PosSM.z)){
			?
		}
	}
	FragColor = vec4(Pos.z,0,0,1);
}
