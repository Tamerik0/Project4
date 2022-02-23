#pragma once
#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include "ShaderProgram.hpp"
class Shader
{
private:
	uint32_t id;
public:
	Shader() {};
	Shader(std::string file, GLenum type) {
		load(file, type);
	};
	uint32_t load(std::string file, GLenum type) {
		std::ifstream f;
		f.open(file);
		f.seekg(0, std::ios::end);
		size_t size = f.tellg();
		std::string source(size, ' ');
		f.seekg(0);
		f.read(&source[0], size);
		const char* _source = source.c_str();
		id = glCreateShader(type);
		glShaderSource(id, 1, &_source, NULL);
		return id;

	}
	int compile() {
		glCompileShader(id);
		int success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		char infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		if (!success)
		{
			std::cout << "ERROR:Shader compilation failed\n" << infoLog << std::endl;
		}
		else {
			std::cout << "Shader compiled successfully" << infoLog << std::endl;
		}
		return success;
	}
	friend class ShaderProgram;
};

