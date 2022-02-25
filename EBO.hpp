#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
class EBO
{
private:
	uint32_t id;
	GLenum usage = GL_STATIC_DRAW;
	int verticesCount;
public:
	static void unbindEBO() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	EBO() {
		gen();
	}
	EBO(void* indices,int count) {
		gen();
		setData(indices, count);
	}
	EBO(void* indices, int count, GLenum usage) {
		gen();
		setData(indices, count);
		setUsage(usage);
	}
	void gen() {
		glGenBuffers(1, &id);
	}
	int getVerticesCount() {
		return verticesCount;
	}
	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	void setData(void* indices, int count){
		bind();
		verticesCount = count;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, usage);
		unbindEBO();
	}
	void setUsage(GLenum u) {
		usage = u;
	}
};

