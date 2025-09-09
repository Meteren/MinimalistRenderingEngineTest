#version 330 core

out vec4 color;
uniform vec4 u_color;
in vec4 vertex_color;

in vec2 uv;
in vec3 normalWS;

uniform sampler2D mainTex;

struct LightData{
    float aIntensity;
    float mainLightIntensity;
    vec3 aColor;
    vec3 mainLightDir;
};

uniform LightData lightData;

void main(){

    float lightFactor = max(dot(normalize(normalWS), normalize(lightData.mainLightDir)),0.0f);

    float lightImpact = lightFactor * lightData.mainLightIntensity;

    vec4 mainLightWithColor = vec4(lightData.aColor,1) * lightImpact;

    vec4 ambientLight = vec4(lightData.aColor,1) * lightData.aIntensity;

    vec4 mainColor = texture(mainTex,uv);
    color = mainColor * (mainLightWithColor + ambientLight);
    //color = mainColor;
};