#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <GLM/glm.hpp>
#include <stdio.h>

class Shader
{ 
private:
	unsigned int program;

	int model_loc;
	int view_loc;
	int projection_loc;	

public:

	struct {
		int u_aColor;
		int u_aIntensity;
		int u_directionalLightIntensity;
		int u_direction;
	} directionalLightUniform;

	Shader();

	unsigned int compileShader(unsigned int type, const char* data);

	void createShaderProgram(const char* vShaderData, const char* fShaderData);

	static const char* readShader(const char* path);

    unsigned int getProgram() const;

	int getModelLoc() const;
	int getViewLoc() const;
	int getProjectionLoc() const;

	void deleteProgram();

	~Shader();

};

