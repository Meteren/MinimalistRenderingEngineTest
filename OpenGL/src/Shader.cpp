#include "Shader.h"

Shader::Shader()
{
    program = 0;
    view_loc = 0;
    model_loc = 0;
    projection_loc = 0;
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

    model_loc = glGetUniformLocation(program, "model");
    view_loc = glGetUniformLocation(program, "view");
    projection_loc = glGetUniformLocation(program, "projection");

    directionalLightUniform.u_aColor = glGetUniformLocation(program, "directionalLight.base.aColor");
    printf("%i",directionalLightUniform.u_aColor);
    directionalLightUniform.u_aIntensity = glGetUniformLocation(program, "directionalLight.base.aIntensity");
    directionalLightUniform.u_directionalLightIntensity = glGetUniformLocation(program, "directionalLight.base.directionalLightIntensity");
    directionalLightUniform.u_direction = glGetUniformLocation(program, "directionalLight.direction");

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
