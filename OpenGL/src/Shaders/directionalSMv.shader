#version 330 core

layout(location = 0) in vec3 positionOS;

uniform mat4 dLightTranform;
uniform mat4 model;


void main(){

	gl_Position = dLightTranform * model * vec4(positionOS,1); 

}