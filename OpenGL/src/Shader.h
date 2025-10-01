#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <GLM/glm.hpp>
#include <stdio.h>
#include "GlobalValues.h"

class Shader
{ 
private:
	unsigned int program;

	int model_loc;
	int view_loc;
	int projection_loc;	
	int u_pointLightCount;
	int u_spotLightCount;
	int u_dLightTransform;
	int u_dShadowMap;

public:

	int pointLightCount;
	int spotLightCount;

	struct {
		int u_aColor;
		int u_aIntensity;
		int u_directionalLightIntensity;
		int u_direction;
	} uniformDirectionalLight;

	struct {
		int u_aColor;
		int u_aIntensity;
		int u_directionalLightIntensity;
		int u_position;
		int u_exponent;
		int u_linear;
		int u_constant;
	} uniformPointLights[MAX_POINT_LIGHT_COUNT];

	struct {
		int u_aColor;
		int u_aIntensity;
		int u_directionalLightIntensity;
		int u_position;
		int u_exponent;
		int u_linear;
		int u_constant;
		int u_direction;
		int u_cutOff;
	} uniformSpotLights[MAX_SPOT_LIGHT_COUNT];

	Shader();

	unsigned int compileShader(unsigned int type, const char* data);

	void createShaderProgram(const char* vShaderData, const char* fShaderData);

	void useProgram();

	static const char* readShader(const char* path);

    unsigned int getProgram() const;

	void validateProgram();

	int getModelLoc() const;
	int getViewLoc() const;
	int getProjectionLoc() const;
	int getPointLightCountLoc() const;
	int getSpotLightCountLoc() const;
	int getdShadowMapLoc() const;
	int getdLightTransformLoc() const;


	void deleteProgram();

	~Shader();

};

