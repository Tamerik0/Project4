#pragma once
#include <GL/glew.h>
#include "Shader.hpp"
#include <vector>


class ShaderProgram
{
private:
	uint32_t id;
	std::vector<Shader> shaders;
public:
	ShaderProgram() {};
	ShaderProgram(std::initializer_list<Shader> sh) {
		for (Shader s : sh) {
			shaders.push_back(s);

		}
		compileAndLink();
	};
	void addShader(Shader sh) {
		shaders.push_back(sh);
	}
	void removeAllShaders() {
		shaders.empty();
	}
	std::vector<Shader> getShaders() {
		return shaders;
	}
	bool compileAllShaders() {
		bool a = true;
		for (int i = 0; i < shaders.size(); i++) {
			if (shaders[i].compile())
				a = false;
		}
		return a;
	}
	void link() {
		id = glCreateProgram();
		for (int i = 0; i < shaders.size(); i++) {
			
			glAttachShader(id, shaders[i].id);
		}
		glLinkProgram(id);
	}
	void compileAndLink() {
		compileAllShaders();
		link();
	}
	void use() {
		glUseProgram(id);
	}
	
	int getId() {
		return id;
	}
};

