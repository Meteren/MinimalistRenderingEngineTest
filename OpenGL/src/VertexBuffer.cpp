#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenVertexArrays(1, &vao_ID);
	glBindVertexArray(vao_ID);
	glGenBuffers(1, &b_ID);
	glBindBuffer(GL_ARRAY_BUFFER, b_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);	
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &b_ID);
}

void VertexBuffer::BindVBO() const{
	glBindBuffer(GL_ARRAY_BUFFER, b_ID);
}

void VertexBuffer::UnBindVBO() const{
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void VertexBuffer::BindEBO() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_ID);
}

void VertexBuffer::UnbindEBO() const
{
}

void VertexBuffer::BindVAO() const
{
	glBindVertexArray(vao_ID);
}

void VertexBuffer::UnBindVAO() const
{
	glBindVertexArray(0);
}

void VertexBuffer::CreateElementBufferObject(unsigned int* data,int count)
{
	glGenBuffers(1, &ebo_ID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_ID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);

}
