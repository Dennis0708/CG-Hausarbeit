#version 400

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

const int MAX_LIGHTS=14;
struct Light
{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius; // SpotRadius.x = innerRadius; SpotRadius.y = outerRadius; SpotRadius.z = 0
	int ShadowIndex;
};

uniform Lights
{
	int LightCount;
	Light lights[MAX_LIGHTS];
};

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;

uniform sampler2D ShadowMapTexture[MAX_LIGHTS];
uniform mat4 ShadowMapMat[MAX_LIGHTS];

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{
    vec4 DiffTex = texture( DiffuseTexture, Texcoord);
    if(DiffTex.a <0.3f) discard;
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPos-Position);
    vec3 E = normalize(EyePos-Position);
    vec3 R = reflect(-L,N);
    vec3 H = normalize(E+L);

    vec3 Color = vec3(0,0,0);

	for(int i=0; i<LightCount;i++){
        float Dist = length(lights[i].Position - Position);
		float Intensity = 1.0/( lights[i].Attenuation.x + (lights[i].Attenuation.y*Dist) + (lights[i].Attenuation.z*Dist*Dist));

        vec3 lightColor = vec3(0,0,0);

        if(lights[i].Type == 0){ //Point Light
            L = normalize(lights[i].Position - Position);
            lightColor = Intensity * lights[i].Color;
        }else if(lights[i].Type == 1){ //Directional Light
            L = -normalize(lights[i].Direction);
            lightColor = lights[i].Color;
        }else if(lights[i].Type == 2){ //Spot Light
            L = normalize(lights[i].Position - Position);

            float delta = acos(dot(normalize(L), normalize(-lights[i].Direction)));
            lightColor = Intensity * lights[i].Color * (1 - sat((delta - lights[i].SpotRadius.x) / (lights[i].SpotRadius.y - lights[i].SpotRadius.x)));
        }

        vec3 DiffuseComponent = lightColor * DiffuseColor * sat(dot(N,L));

        // Reflexion nach Phong
        //vec3 R = reflect(-L,N);
        //vec3 SpecularComponent = lightColor * SpecularColor * pow( sat(dot(R,E)), SpecularExp);

        // Reflexion nach Blinn
        vec3 H = normalize(E+L);
        vec3 SpecularComponent = lightColor * SpecularColor * pow( sat(dot(N,H)), SpecularExp);

				if(lights[i].ShadowIndex != -1){
            vec4 PosSM = ShadowMapMat[i] * vec4(Position.xyz, 1);
            PosSM.xyz /= PosSM.w; // perspektivische Teilung vollziehen
            PosSM.xy = PosSM.xy*0.5 + 0.5; // Koordinaten von norm. Bildraum [-1,1] in Texturkoordinaten [0,1]
            vec4 DepthSM = texture(ShadowMapTexture[i], PosSM.xy);

				    if(length(DepthSM) < PosSM.z){
					    shadow += vec3(0.1,0.1,0.1);
				    }
        }

        Color += DiffuseComponent + SpecularComponent;
    }

    FragColor = vec4(DiffTex.rgb * (Color + AmbientColor - shadow),DiffTex.a);
}
