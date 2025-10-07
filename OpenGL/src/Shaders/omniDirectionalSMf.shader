#version 330 core

in vec4 fragPos;
uniform vec3 lightPos;
uniform float farPlane;

void main(){
	float fragDepth = length(fragPos.xyz - lightPos);

	fragDepth /= farPlane;

	gl_FragDepth = fragDepth;
}