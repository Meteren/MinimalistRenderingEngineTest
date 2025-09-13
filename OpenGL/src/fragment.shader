#version 330 core


const int MAX_POINT_LIGHT_COUNT = 3;

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

struct PointLight{
    Light base;
    float exponent;
    float linear;
    float constant;
    vec3 position;
};

uniform int pointLightCount;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];


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

vec4 CalculatePointLights(){
    vec4 finalLighting = vec4(0,0,0,0);

    for(int i = 0; i < pointLightCount; i++){

        vec3 direction = fragWS - pointLights[i].position;

        float distance = length(direction);

        direction = normalize(direction);

        finalLighting += CalculatePhongLighting(pointLights[i].base,direction);

        float attenuation = pointLights[i].exponent * distance * distance + pointLights[i].linear * distance + pointLights[i].constant;
        
        finalLighting *= 1 / attenuation;
    }


    return finalLighting;

}

void main(){

    vec4 overallLightCol = vec4(0,0,0,0);

    overallLightCol += CalculateDirectionalLight(directionalLight);

    overallLightCol += CalculatePointLights();

    vec4 mainColor = texture(mainTex,uv);

    color = mainColor * overallLightCol;
    //color = mainColor;
};