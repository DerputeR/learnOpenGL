#pragma once
#include <iostream>
#include <string>

class ShaderLoader {
	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	static std::string TypeToName(unsigned int type);
	static unsigned int CompileShader(unsigned int type, const std::string& source);
public:
	static struct ShaderSources {
		std::string vertShaderSrc;
		std::string fragShaderSrc;
	};

	static std::string ReadShaderSource(const std::string& filepath);
	static ShaderSources ParseShaderSources(const std::string& vertFilepath, const std::string& fragFilepath);
	static ShaderSources ParseCombinedShaderSource(const std::string& filepath);

	static unsigned int CreateShaderProgram(const std::string& vertShader, const std::string& fragShader);
};




