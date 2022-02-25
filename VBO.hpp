#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "VAO.hpp"

class VBO {
	friend class VAO;
	class VertexAttrib;
private:
	uint32_t id;
	std::vector <VertexAttrib> attribs;

	GLenum drawMode;
	GLenum usage;
	int vertexCount;
	int vertexSize;
	void* data;

public:
	class VertexAttrib {
		friend class VBO;
	private:
		int location;
		bool normalize=false;
		GLenum dataType=GL_FLOAT;
		int count;
	public:
		VertexAttrib(int loc, bool normalize, GLenum dataType, int count) : location(loc), normalize(normalize), dataType(dataType), count(count) {}
		VertexAttrib(int loc, GLenum dataType, int count) : location(loc), dataType(dataType), count(count) {}
		VertexAttrib(int loc, int count) : location(loc), normalize(normalize), dataType(dataType), count(count) {}
		int getSize() {
			switch (dataType) {
			case GL_FLOAT:
				return count * sizeof(float);
				break;
			case GL_DOUBLE:
				return count * sizeof(double);
				break;
			case GL_INT:
				return count * sizeof(int);
				break;
			case GL_UNSIGNED_INT:
				return count * sizeof(unsigned int);
				break;
			case GL_BYTE:
				return count * sizeof(char);
				break;
			case GL_SHORT:
				return count * sizeof(short);
				break;
			case GL_UNSIGNED_BYTE:
				return count * sizeof(char);
				break;
			case GL_UNSIGNED_SHORT:
				return count * sizeof(unsigned short);
				break;
			case GL_FIXED:
				return count * 4;
				break;
			case GL_HALF_FLOAT:
				return count * 2;
				break;
			}
		}
	};
	VBO() {
		gen();
	}
	VBO(std::vector<VertexAttrib> a, GLenum drawMode,GLenum usage) {
		gen();
		setVertexAttribs(a);
		setDrawMode(drawMode);
		setUsage(usage);
	}
	VBO(std::vector<VertexAttrib> a, GLenum drawMode, GLenum usage, void* data, int vertexCount) {
		gen();
		setVertexAttribs(a);
		setDrawMode(drawMode);
		setUsage(usage);
		update(data, vertexCount);
	}
	void gen() {
		glGenBuffers(1, &id);
	}
	void setVertexAttribs(std::vector<VertexAttrib> a) {
		attribs = a;
		calculateVertexSize();
	}
	
	void setDrawMode(GLenum dm) {
		drawMode = dm;
	}
	void setUsage(GLenum u) {
		usage = u;
	}
	static void unbindVBO() {
		glBindBuffer(GL_VERTEX_ARRAY, 0);
	}
	
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	void draw() {
		setup();
		glDrawArrays(drawMode, 0, vertexCount);
		unbindVBO();
	}
	void setup() {
		bind();
		int off = 0;
		for (int i = 0; i < attribs.size(); i++) {
			VertexAttrib current = attribs[i];
			int s = current.count;
			glVertexAttribPointer(current.location, s, current.dataType, current.normalize, vertexSize, (void*)off);
			glEnableVertexAttribArray(current.location);
			off += current.getSize();
			
		}
	}
	int calculateVertexSize() {
		int size = 0;
		for (int i = 0; i < attribs.size(); i++) {
			size += attribs[i].getSize();
		}
		vertexSize = size;
		return size;
	}
	void setData(void* data, int vertexCount) {
		this->data = data;
		this->vertexCount = vertexCount;
	}
	void loadToGPU() {
		bind();
		glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, usage);
		unbindVBO();
	}
	void setData(void* data) {
		this->data = data;
	}
	void update(void* data) {
		setData(data);
		loadToGPU();
	}
	void update(void* data, int vertexCount) {

		setData(data, vertexCount);
		loadToGPU();
	}
};