#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoords;
layout(location = 2) in vec3 normalOS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertex_color;
out vec2 uv;
out vec3 normalWS;

void main(){
	gl_Position =  projection * view * model * vec4(position,1);
	vertex_color = vec4(clamp(position,0.0f,1.0f),1);
	uv = texcoords;
	normalWS = mat3(transpose(inverse(model))) * normalOS;
};