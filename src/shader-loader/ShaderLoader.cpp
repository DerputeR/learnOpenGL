#include "ShaderLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

ShaderLoader::ShaderSources ShaderLoader::ParseCombinedShaderSource(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::string line;
    std::stringstream stringStreams[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        // if we find #shader, determine what kind it is
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            // add to appropriate shader
            if ((int)type > -1) {
                stringStreams[(int)type] << line << '\n';
            }
        }
    }

    return { stringStreams[0].str(), stringStreams[1].str() };
}

ShaderLoader::ShaderSources ShaderLoader::ParseVertShaderSource(const std::string& filepath) {
    return ShaderSources();
}

ShaderLoader::ShaderSources ShaderLoader::ParseFragShaderSource(const std::string& filepath) {
    return ShaderSources();
}

std::string ShaderLoader::TypeToName(unsigned int type) {
    switch (type) {
    case GL_VERTEX_SHADER:
        return "vertex shader";
    case GL_FRAGMENT_SHADER:
        return "fragment shader";
    default:
        return "default shader";
    }
}

unsigned int ShaderLoader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL); // returns a non-zero reference ID for the shader
    glCompileShader(id);

    // check compiler result
    int compileResult;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        int logLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        //char* message = (char*) alloca(logLength * sizeof(char)); // alloca allows for dynamic allocation on the stack
        char* message = (char*)_malloca(logLength * sizeof(char)); // _malloca allows for dynamic allocation on the stack, with security features
        glGetShaderInfoLog(id, logLength, &logLength, message);
        _freea(message); // _malloca requires us to call _freea when finished
        std::cout << "Failed to compile " << (TypeToName(type)) << "!" << std::endl;
        glDeleteShader(id);
        return 0; // 0 indicates an error since id would be non-zero if successfull
    }

    return id;
}

unsigned int ShaderLoader::CreateShaderProgram(const std::string& vertShader, const std::string& fragShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

    // TODO: check to make sure the above succeeds

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    //glDetachShader(program, vs);
    //glDetachShader(program, fs);

    // delete intermediates after shaders have been compiled, linked, and verified
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
