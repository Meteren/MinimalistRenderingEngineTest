#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* verticeData, const void* indiceData, unsigned int sizeVertices, unsigned int sizeIndices)
{
	this->size = sizeVertices;
	glGenVertexArrays(1, &vao_ID);
	glBindVertexArray(vao_ID);
	glGenBuffers(1, &b_ID);
	glBindBuffer(GL_ARRAY_BUFFER, b_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices * sizeof(float), verticeData, GL_STATIC_DRAW);
	CreateElementBufferObject(indiceData,sizeIndices);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void VertexBuffer::BindVAO() const
{
	glBindVertexArray(vao_ID);
}

void VertexBuffer::UnBindVAO() const
{
	glBindVertexArray(0);
}

void VertexBuffer::CreateElementBufferObject(const void* data, unsigned int sizeIndices)
{
	glGenBuffers(1, &ebo_ID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_ID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices * sizeof(unsigned int), data, GL_STATIC_DRAW);

}

void VertexBuffer::render()
{

	glBindVertexArray(vao_ID);

	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}
