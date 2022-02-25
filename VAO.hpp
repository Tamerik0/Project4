#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "VBO.hpp"
#include "EBO.hpp"
class VAO
{
private:
	uint32_t id;
	
public:
	VBO* vbo;
	EBO* ebo;
	VAO() {
		gen();
	}
	VAO(VBO* vbo, EBO* ebo) :vbo(vbo), ebo(ebo) {
		gen();
		setup();
	}
	void gen() {
		glGenVertexArrays(1, &id);
	}

	static void unbindVAO() {
		glBindVertexArray(0);
	}


	void bind() {
		glBindVertexArray(id);
	}
	void setup() {
		bind();
		vbo->setup();
		if (ebo != nullptr) {
			ebo->bind();
		}
		unbindVAO();
		VBO::unbindVBO();
		EBO::unbindEBO();
	}

	void draw() {
		bind();
		
		if (ebo != nullptr)
			glDrawElements(vbo->drawMode, ebo->getVerticesCount(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(vbo->drawMode, 0, vbo->vertexCount);
	}
};

