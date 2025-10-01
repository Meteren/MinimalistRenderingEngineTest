#version 330 core

layout(location = 0) in vec3 positionOS;

uniform mat4 dLightTransform;
uniform mat4 model;


void main(){

	gl_Position = dLightTransform * model * vec4(positionOS,1); 

}