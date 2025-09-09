#version 330 core

out vec4 color;
uniform vec4 u_color;
in vec4 vertex_color;

in vec2 uv;

uniform sampler2D mainTex;


struct LightData{
    float aIntensity;
    vec3 aColor;
};

uniform LightData lightData;

void main(){

    vec4 mainColor = texture(mainTex,uv);
    color = mainColor * (vec4(lightData.aColor,1) * lightData.aIntensity);
};