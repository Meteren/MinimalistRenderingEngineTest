#include "Shader.h"

Shader::Shader()
{
    program = 0;
    view_loc = 0;
    model_loc = 0;
    projection_loc = 0;
    pointLightCount = 0;
    spotLightCount = 0;
    u_pointLightCount = 0;
    u_spotLightCount = 0;
    u_dLightTransform = 0;
    u_dShadowMap = 0;
    u_camPos = 0;
    u_farPlane = 0;
    uniformDirectionalLight = {0};
    u_pointLightPos = 0;
    
    for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {
        uniformPointLights[i] = { 0 };
    }

    for (int i = 0; i < MAX_SPOT_LIGHT_COUNT; i++) {
        uniformSpotLights[i] = { 0 };
    }

    for (int i = 0; i < 6; i++) {
        u_odLightTransformMatrixLocs[i] = 0;
    }
    
}

unsigned int Shader::compileShader(unsigned int type, const char* data)
{
    unsigned int s_ID;

    int success;
    char infoLog[512];

    s_ID = glCreateShader(type);
    glShaderSource(s_ID, 1, &data, NULL);
    glCompileShader(s_ID);

    glGetShaderiv(s_ID, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(s_ID, 512, NULL, infoLog);

        std::cout << "Error while compiling shader:\n" << infoLog << std::endl;
        return 0;
    }
    return s_ID;
}

void Shader::createShaderProgram(const char* vShaderData, const char* fShaderData)
{

    int success;
    char infoLog[512];
    program = glCreateProgram();

    unsigned int v_shader = compileShader(GL_VERTEX_SHADER, vShaderData);
    unsigned int f_shader = compileShader(GL_FRAGMENT_SHADER, fShaderData);

    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);

    glLinkProgram(program);

    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);

        std::cout << "Error occured while linking program.\n" << std::endl;
        deleteProgram();
        return;
    }

    validateProgram();

    model_loc = glGetUniformLocation(program, "model");
    view_loc = glGetUniformLocation(program, "view");
    projection_loc = glGetUniformLocation(program, "projection");

    u_pointLightCount = glGetUniformLocation(program, "pointLightCount");

    u_spotLightCount = glGetUniformLocation(program, "spotLightCount");

    u_dLightTransform = glGetUniformLocation(program, "dLightTransform");
    printf("Light transform: %i\n", u_dLightTransform);
    u_dShadowMap = glGetUniformLocation(program, "dShadowMap");
    printf("Shadow map: %i\n", u_dShadowMap);

    u_camPos = glGetUniformLocation(program, "camPos");

    u_mainTex = glGetUniformLocation(program, "mainTex");

    uniformDirectionalLight.u_aColor = glGetUniformLocation(program, "directionalLight.base.aColor");
    uniformDirectionalLight.u_aIntensity = glGetUniformLocation(program, "directionalLight.base.aIntensity");
    uniformDirectionalLight.u_directionalLightIntensity = glGetUniformLocation(program, "directionalLight.base.directionalLightIntensity");
    uniformDirectionalLight.u_direction = glGetUniformLocation(program, "directionalLight.direction");
    
    u_farPlane = glGetUniformLocation(program, "farPlane");

    for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {

        char buffer[100] = { '\0' };

        snprintf(buffer, sizeof(buffer), "pointLights[%d].base.aColor", i);
        uniformPointLights[i].u_aColor = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_aColor);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].base.aIntensity", i);
        uniformPointLights[i].u_aIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_aIntensity);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].base.directionalLightIntensity", i);
        uniformPointLights[i].u_directionalLightIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_directionalLightIntensity);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].exponent", i);
        uniformPointLights[i].u_exponent = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_exponent);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].linear", i);
        uniformPointLights[i].u_linear = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_linear);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].constant", i);
        uniformPointLights[i].u_constant = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_constant);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].position", i);
        uniformPointLights[i].u_position = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformPointLights[i].u_position);

    }

    for (int i = 0; i < MAX_SPOT_LIGHT_COUNT; i++) {

        char buffer[100] = { '\0' };

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.base.aColor", i);
        uniformSpotLights[i].u_aColor = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_aColor);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.base.aIntensity", i);
        uniformSpotLights[i].u_aIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_aIntensity);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.base.directionalLightIntensity", i);
        uniformSpotLights[i].u_directionalLightIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_directionalLightIntensity);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.exponent", i);
        uniformSpotLights[i].u_exponent = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_exponent);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.linear", i);
        uniformSpotLights[i].u_linear = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_linear);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.constant", i);
        uniformSpotLights[i].u_constant = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_constant);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.position", i);
        uniformSpotLights[i].u_position = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformSpotLights[i].u_position);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].direction", i);
        uniformSpotLights[i].u_direction = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformSpotLights[i].u_position);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].cutOff", i);
        uniformSpotLights[i].u_cutOff = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformSpotLights[i].u_position);

    }

    for (int i = 0; i < MAX_POINT_LIGHT_COUNT + MAX_SPOT_LIGHT_COUNT; i++) {
        
        char buffer[100] = { '\0' };
        snprintf(buffer, sizeof(buffer), "oDShadowMap[%i].shadowMap", i);
        u_oDShadowMap[i].u_oDShadowMap = glGetUniformLocation(program, buffer);

        snprintf(buffer, sizeof(buffer), "oDShadowMap[%i].farPlane", i);
        u_oDShadowMap[i].u_farPlane = glGetUniformLocation(program, buffer);

    }

}

void Shader::createShaderProgram(const char* vShaderData, const char* gShaderData, const char* fShaderData)
{

    int success;
    char infoLog[512];
    program = glCreateProgram();

    unsigned int v_shader = compileShader(GL_VERTEX_SHADER, vShaderData);
    unsigned int g_shader = compileShader(GL_GEOMETRY_SHADER, gShaderData);
    unsigned int f_shader = compileShader(GL_FRAGMENT_SHADER, fShaderData);
    
    glAttachShader(program, v_shader);
    glAttachShader(program, g_shader);
    glAttachShader(program, f_shader);

    glLinkProgram(program);

    glDeleteShader(v_shader);
    glDeleteShader(g_shader);
    glDeleteShader(f_shader);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);

        std::cout << "Error occured while linking program.\n" << std::endl;
        deleteProgram();
        return;
    }

    validateProgram();

    model_loc = glGetUniformLocation(program, "model");
    view_loc = glGetUniformLocation(program, "view");
    projection_loc = glGetUniformLocation(program, "projection");

    u_pointLightCount = glGetUniformLocation(program, "pointLightCount");

    u_spotLightCount = glGetUniformLocation(program, "spotLightCount");

    u_dLightTransform = glGetUniformLocation(program, "dLightTransform");
    printf("Light transform: %i\n", u_dLightTransform);
    u_dShadowMap = glGetUniformLocation(program, "dShadowMap");
    printf("Shadow map: %i\n", u_dShadowMap);

    u_camPos = glGetUniformLocation(program, "camPos");

    uniformDirectionalLight.u_aColor = glGetUniformLocation(program, "directionalLight.base.aColor");
    uniformDirectionalLight.u_aIntensity = glGetUniformLocation(program, "directionalLight.base.aIntensity");
    uniformDirectionalLight.u_directionalLightIntensity = glGetUniformLocation(program, "directionalLight.base.directionalLightIntensity");
    uniformDirectionalLight.u_direction = glGetUniformLocation(program, "directionalLight.direction");

    u_farPlane = glGetUniformLocation(program, "farPlane");
    u_pointLightPos = glGetUniformLocation(program, "lightPos");

    for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {

        char buffer[100] = { '\0' };

        snprintf(buffer, sizeof(buffer), "pointLights[%d].base.aColor", i);
        uniformPointLights[i].u_aColor = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_aColor);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].base.aIntensity", i);
        uniformPointLights[i].u_aIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_aIntensity);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].base.directionalLightIntensity", i);
        uniformPointLights[i].u_directionalLightIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_directionalLightIntensity);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].exponent", i);
        uniformPointLights[i].u_exponent = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_exponent);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].linear", i);
        uniformPointLights[i].u_linear = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_linear);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].constant", i);
        uniformPointLights[i].u_constant = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformPointLights[i].u_constant);

        snprintf(buffer, sizeof(buffer), "pointLights[%d].position", i);
        uniformPointLights[i].u_position = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformPointLights[i].u_position);
    }

    for (int i = 0; i < MAX_SPOT_LIGHT_COUNT; i++) {

        char buffer[100] = { '\0' };

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.base.aColor", i);
        uniformSpotLights[i].u_aColor = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_aColor);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.base.aIntensity", i);
        uniformSpotLights[i].u_aIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_aIntensity);
        //printf("%s ** ", buffer);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.base.directionalLightIntensity", i);
        uniformSpotLights[i].u_directionalLightIntensity = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_directionalLightIntensity);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.exponent", i);
        uniformSpotLights[i].u_exponent = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_exponent);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.linear", i);
        uniformSpotLights[i].u_linear = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_linear);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.constant", i);
        uniformSpotLights[i].u_constant = glGetUniformLocation(program, buffer);
        printf("%d ** ", uniformSpotLights[i].u_constant);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].pointLight.position", i);
        uniformSpotLights[i].u_position = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformSpotLights[i].u_position);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].direction", i);
        uniformSpotLights[i].u_direction = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformSpotLights[i].u_position);

        snprintf(buffer, sizeof(buffer), "spotLights[%d].cutOff", i);
        uniformSpotLights[i].u_cutOff = glGetUniformLocation(program, buffer);
        printf("%d \n", uniformSpotLights[i].u_position);

    }

    for (int i = 0; i < 6; i++) {

        char buffer[100] = { '\0' };
        snprintf(buffer, sizeof(buffer), "odLightTransformMatrix[%i]", i);
        u_odLightTransformMatrixLocs[i] = glGetUniformLocation(program, buffer);
    }

}

void Shader::useProgram()
{
    glUseProgram(program);
}

const char* Shader::readShader(const char* path)
{
    std::string line;

    std::ifstream readFile(path);

    if (!readFile.is_open()) {
        return nullptr;
    }

    std::string content;

    while (!readFile.eof()) {
        getline(readFile, line);
        content.append(line + "\n");
    }

    readFile.close();
    char* contentConverted = new char[content.size() + 1];

    std::copy(content.begin(), content.end(), contentConverted);

    contentConverted[content.size()] = '\0';

    printf("%s", contentConverted);

    return contentConverted;
}

unsigned int Shader::getProgram() const {
    return program;
}

void Shader::validateProgram()
{
    char errorLog[512];

    int status;

    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

    if (!status) {
        glGetProgramInfoLog(program, 512, NULL, errorLog);
        printf("%s", errorLog);
    }
    else {
        printf("Validation succeeded.\n");
    }

}

int Shader::getModelLoc() const
{
    return model_loc;
}

int Shader::getViewLoc() const
{
    return view_loc;
}

int Shader::getProjectionLoc() const
{
    return projection_loc;
}

int Shader::getPointLightCountLoc() const
{
    return u_pointLightCount;
}

int Shader::getSpotLightCountLoc() const
{
    return u_spotLightCount;
}

int Shader::getdShadowMapLoc() const
{
    return u_dShadowMap;
}

int Shader::getdLightTransformLoc() const
{
    return u_dLightTransform;
}

int Shader::getCamPosLoc() const
{
    return u_camPos;
}

int Shader::getPointLightPosLoc() const
{
    return u_pointLightPos;
}

int Shader::getMainTexLoc() const
{
    return u_mainTex;
}

void Shader::deleteProgram()
{
    glDeleteProgram(program);
}

Shader::~Shader()
{
    if(!program)
	    deleteProgram();
    program = 0;
}
