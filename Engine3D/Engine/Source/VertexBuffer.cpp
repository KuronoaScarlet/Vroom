#include "VertexBuffer.h"

#include "Profiling.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int s, int bufferType) : size (s), buffer(0)
{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, s, data, bufferType);
}

VertexBuffer::~VertexBuffer()
{
	if (buffer != 0) glDeleteBuffers(1, &buffer);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}