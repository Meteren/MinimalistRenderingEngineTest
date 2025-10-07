#version 330 core

layout(triangles) in;

layout(triangle_strip,max_vertices = 18) out;

out vec4 fragPos;

uniform mat4 odLightTransformMatrix[6];

void main(){

	for(int i = 0; i < 6; i++){
		gl_Layer = i;
		for(int j = 0; j < 3; j++){
			fragPos = gl_in[j].gl_Position;
			gl_Position = odLightTransformMatrix[i] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}