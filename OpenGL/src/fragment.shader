#version 330 core


const int MAX_POINT_LIGHT_COUNT = 3;
const int MAX_SPOT_LIGHT_COUNT = 3;

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

struct SpotLight{
    PointLight pointLight;
    vec3 direction;
    float cutOff;
};

uniform int pointLightCount;
uniform int spotLightCount; //dont forget to send from cpu side as uniform value
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform SpotLight spotLights[MAX_SPOT_LIGHT_COUNT];


vec4 CalculatePhongLighting(Light base, vec3 direction){

    float lightFactor = max(dot(normalize(normalWS), normalize(direction)),0.0f);

    float lightImpact = lightFactor * base.directionalLightIntensity;

    vec4 mainLightWithColor = vec4(base.aColor,1) * lightImpact;

    vec4 ambientLight = vec4(base.aColor,1) * base.aIntensity;

    vec4 specColor = vec4(0,0,0,0);

    if(lightImpact > 0){
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

vec4 CalculatePointLight(PointLight pointLight){
    vec4 color = vec4(0,0,0,0);

    vec3 direction = fragWS - pointLight.position;

    float distance = length(direction);

    direction = normalize(direction);

    color += CalculatePhongLighting(pointLight.base,direction);

    float attenuation = pointLight.exponent * distance * distance + pointLight.linear * distance + pointLight.constant;
        
    color *= 1 / attenuation;

    return color;
}

vec4 CalculateSpotLight(SpotLight spotLight){
    vec4 color = vec4(0,0,0,0);
    color += CalculatePointLight(spotLight.pointLight);

    vec3 fragDir = normalize(fragWS - spotLight.pointLight.position);  

    float angle = dot(fragDir,normalize(spotLight.direction));

    //float lit = step(spotLight.cutOff,angle);
    float lit = smoothstep(spotLight.cutOff,1,angle);

    float normalizedVal = pow(lit * ((angle - spotLight.cutOff) * (1.0f / (1-spotLight.cutOff))),2);
    
    color *= normalizedVal;

    return color;
}

vec4 CalculatePointLights(){
    vec4 finalLighting = vec4(0,0,0,0);

    for(int i = 0; i < pointLightCount; i++){

        finalLighting += CalculatePointLight(pointLights[i]);
    }

    return finalLighting;

}

vec4 CalculateSpotLights(){
    vec4 finalLighting = vec4(0,0,0,0);

    for(int i = 0; i < spotLightCount; i++){
        finalLighting += CalculateSpotLight(spotLights[i]);
    }

    return finalLighting;
}

void main(){

    vec4 overallLightCol = vec4(0,0,0,0);

    overallLightCol += CalculateDirectionalLight(directionalLight);

    overallLightCol += CalculatePointLights();

    overallLightCol += CalculateSpotLights();

    vec4 mainColor = texture(mainTex,uv);

    color = mainColor * overallLightCol;
    //color = mainColor;
};