#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
class VBO {
	class VertexAttrib;
private:
	uint32_t id;
	std::vector <VertexAttrib> attribs;

	GLenum drawMode;
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
		VertexAttrib(int loc, GLenum dataType, int count) : location(loc), normalize(normalize), dataType(dataType), count(count) {}
		VertexAttrib(int loc, bool normalize, int count) : location(loc), normalize(normalize), dataType(dataType), count(count) {}
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
		glGenBuffers(1, &id);
	}
	void setVertexAttribs(std::vector<VertexAttrib> a) {
		attribs = a;
		calculateVertexSize();
	}
	
	void setDrawMode(GLenum dm) {
		drawMode = dm;
	}
	static void unbindVBO() {
		glBindBuffer(GL_VERTEX_ARRAY, 0);
	}
	
	void bind() {
		glBindBuffer(GL_VERTEX_ARRAY, id);
	}
	void draw() {
		bind();
		setup();
		glDrawArrays(drawMode, 0, vertexCount);
		unbindVBO();
	}
	void setup() {
		int off = 0;
		for (int i = 0; i < attribs.size(); i++) {
			VertexAttrib current = attribs[i];
			int s = current.getSize();
			glVertexAttribPointer(current.location, s, current.dataType, current.normalize, vertexSize, (void*)off);
			glEnableVertexAttribArray(current.location);
			off += s;
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
	void loadToGPU(int size, GLenum usage) {
		bind();
		glBufferData(GL_VERTEX_ARRAY, vertexCount*vertexSize, data, usage);
		unbindVBO();
	}
};