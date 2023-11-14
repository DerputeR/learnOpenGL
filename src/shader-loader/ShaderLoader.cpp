#include "ShaderLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <errno.h>

ShaderLoader::ShaderSources ShaderLoader::ParseCombinedShaderSource(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::string line;
    std::stringstream stringStreams[2];

    ShaderType type = ShaderType::NONE;

    stream.open(filepath, std::ios_base::in);
    if (!stream.is_open()) {
        std::cerr << "Error opening file at " << filepath;
        perror(" ");
    }
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
    if (stream.bad()) {
        std::cerr << "Error while reading file at " << filepath;
        perror(" ");
    }

    return { stringStreams[(int) ShaderType::VERTEX].str(), stringStreams[(int)ShaderType::FRAGMENT].str() };
}

ShaderLoader::ShaderSources ShaderLoader::ParseShaderSources(const std::string& vertFilepath, const std::string& fragFilepath) {
    return { ReadShaderSource(vertFilepath), ReadShaderSource(fragFilepath) };
}

std::string ShaderLoader::ReadShaderSource(const std::string& filepath) {
    std::ifstream stream;
    std::string line;
    std::stringstream stringStream;

    // todo: find out best way to read in text file
    // ref: https://stackoverflow.com/questions/34464705/rdbuf-vs-getline-vs
    // article on ifstream error detection : https://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
    stream.open(filepath, std::ios_base::in);
    if (!stream.is_open()) {
        std::cerr << "Error opening file at " << filepath;
        perror(" ");
    }
    while (getline(stream, line)) {
        stringStream << line << '\n';
    }
    // failbit can be set by getline if it cannot extract data
    // this can often happen if the last char in a file is a lin delimiter
    // only when bad bit is set will errno be updated and we know there's an exception
    if (stream.bad()) {
        std::string errorBuffer;
        //size_t buffSize = strerrorlen_s();
        // strerror is not threadsafe, but can't use strerror_s:
        // msvc seems to be missing strerrorlen_s
        std::cerr << "Error while reading file at " << filepath;
        perror(" ");
    }
    //stream.close(); // not needed, destructor will call this automatically
    return stringStream.str();
}

std::string ShaderLoader::TypeToName(unsigned int type) {
    switch (type) {
    case GL_VERTEX_SHADER:
        return "vertex shader";
    case GL_FRAGMENT_SHADER:
        return "fragment shader";
    default:
        return "unknown shader type";
    }
}

unsigned int ShaderLoader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL); // returns a non-zero reference ID for the shader
    glCompileShader(id);

    // check if compilation had any errors
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        int msgLen;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &msgLen);
        GLchar* msg = (GLchar*)_malloca(msgLen * sizeof(GLchar));

        glGetShaderInfoLog(id, msgLen, &msgLen, msg);
        std::cout << "Failed to compile " << (TypeToName(type)) << "! " << msg << std::endl;

        // cleanup failed shader compile
        glDeleteShader(id);
        
        _freea(msg);
        return 0;
    }

    return id;
}

unsigned int ShaderLoader::CreateShaderProgram(const std::string& vertShader, const std::string& fragShader) {
    unsigned int program = glCreateProgram(); // non-zero id

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertShader);
    if (!vs) {
        std::cout << "Vertex shader failed to compile, aborting shader program creation." << std::endl;
        return 0;
    }

    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);
    if (!fs) {
        std::cout << "Fragment shader failed to compile, aborting shader program creation." << std::endl;
        return 0;
    }
    

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    
    // linking done, detach shader intermediates
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    
    // delete shader intermediates once detached
    glDeleteShader(vs);
    glDeleteShader(fs);


    // check if linking had errors
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        int msgLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &msgLen);
        GLchar* msg = (GLchar*)_malloca(msgLen * sizeof(GLchar));

        glGetProgramInfoLog(program, msgLen, &msgLen, msg);
        std::cout << "Shader program linking failed: " << msg << std::endl;

        _freea(msg);
        return 0;
    }

    glValidateProgram(program);

    // check if validation had errors
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE) {
        int msgLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &msgLen);
        GLchar* msg = (GLchar*)_malloca(msgLen * sizeof(GLchar));

        glGetProgramInfoLog(program, msgLen, &msgLen, msg);
        std::cout << "Shader program validation failed: " << msg << std::endl;

        _freea(msg);
        return 0;
    }


    return program;
}
