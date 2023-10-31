#pragma once
#include <iostream>
#include <string>

class ShaderLoader {
	struct ShaderSources {
		std::string vertShaderSrc;
		std::string fragShaderSrc;
	};

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	
	ShaderSources ParseCombinedShaderSource(const std::string& filepath);
	ShaderSources ParseVertAndFragShaderSource(const std::string& vertFilepath, const std::string& fragFilepath);
	ShaderSources ParseVertShaderSource(const std::string& filepath);
	ShaderSources ParseFragShaderSource(const std::string& filepath);
	std::string TypeToName(unsigned int type);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShaderProgram(const std::string& vertShader, const std::string& fragShader);
};




