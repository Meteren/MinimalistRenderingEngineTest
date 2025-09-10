#version 330 core

out vec4 color;
uniform vec4 u_color;
in vec4 vertex_color;

in vec2 uv;
in vec3 normalWS;
in vec3 fragWS;

uniform sampler2D mainTex;

uniform vec3 camPos;

struct LightData{
    float aIntensity;
    float mainLightIntensity;
    vec3 aColor;
    vec3 mainLightDir;
};

struct Material{
    float metallic;
    float smoothness;

};

uniform LightData lightData;
uniform Material material;

void main(){

    float lightFactor = max(dot(normalize(normalWS), normalize(lightData.mainLightDir)),0.0f);

    float lightImpact = lightFactor * lightData.mainLightIntensity;

    vec4 mainLightWithColor = vec4(lightData.aColor,1) * lightImpact;

    vec4 ambientLight = vec4(lightData.aColor,1) * lightData.aIntensity;

    vec4 specColor = vec4(0,0,0,0);

    if(lightFactor > 0){
        vec3 reflectedLight = normalize(reflect(lightData.mainLightDir,normalize(normalWS)));
        vec3 camDir = normalize(camPos - fragWS);

        float specFactor = dot(camDir,reflectedLight);

        specFactor = max(pow(specFactor,material.smoothness),0.0f);

        specColor = vec4(lightData.aColor * specFactor * material.metallic,1);

    }

    vec4 mainColor = texture(mainTex,uv);

    color = mainColor * (mainLightWithColor + ambientLight + specColor);
    //color = mainColor;
};