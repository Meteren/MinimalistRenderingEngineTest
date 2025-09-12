#version 330 core

out vec4 color;
uniform vec4 u_color;
in vec4 vertex_color;

in vec2 uv;
in vec3 normalWS;
in vec3 fragWS;

uniform sampler2D mainTex;

uniform vec3 camPos;

struct Material{
    float metallic;
    float smoothness;

};

struct Light{
    float aIntensity;
    float directionalLightIntensity;
    vec3 aColor;
};

struct DirectionalLight{
    Light base;
    vec3 direction;
};


uniform Material material;
uniform DirectionalLight directionalLight;

vec4 CalculatePhongLighting(Light base, vec3 direction){

    float lightFactor = max(dot(normalize(normalWS), normalize(direction)),0.0f);

    float lightImpact = lightFactor * base.directionalLightIntensity;

    vec4 mainLightWithColor = vec4(base.aColor,1) * lightImpact;

    vec4 ambientLight = vec4(base.aColor,1) * base.aIntensity;

    vec4 specColor = vec4(0,0,0,0);

    if(lightFactor > 0){
        vec3 reflectedLight = normalize(reflect(direction,normalize(normalWS)));
        vec3 camDir = normalize(camPos - fragWS);

        float specFactor = dot(camDir,reflectedLight);

        specFactor = max(pow(specFactor,material.smoothness),0.0f);

        specColor = vec4(base.aColor * specFactor * material.metallic,1);

    }

    return mainLightWithColor + ambientLight + specColor;

}

vec4 CalculateDirectionalLight(DirectionalLight light){
    vec4 finalLighting = CalculatePhongLighting(light.base,light.direction);
    return finalLighting;
}


void main(){

    vec4 overallLightCol = vec4(0,0,0,0);

    overallLightCol += CalculateDirectionalLight(directionalLight);

    vec4 mainColor = texture(mainTex,uv);

    color = mainColor * overallLightCol;
    //color = mainColor;
};