#pragma once

#include "glew/include/GL/glew.h"

class VertexBuffer
{
public:
	VertexBuffer() : buffer(0), size(0) {}
	VertexBuffer(const void* data, unsigned int s, int bufferType = GL_STATIC_DRAW);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetSize() const { return size; }

	unsigned int buffer;
	unsigned int size;

};