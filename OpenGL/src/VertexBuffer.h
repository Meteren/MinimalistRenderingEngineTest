#pragma once
#include <GL/glew.h>

class VertexBuffer {
	private:
		unsigned int b_ID;
		unsigned int vao_ID;
		unsigned int ebo_ID;

		unsigned int size;

		void CreateElementBufferObject(const void* data, unsigned int sizeIndices);

	public:
		VertexBuffer(const void* verticeData, const void* indiceData, unsigned int sizeVertices, unsigned int sizeIndices);

		~VertexBuffer();

		void BindVBO() const;

		void UnBindVBO() const;

		void BindEBO() const;

		void UnbindEBO() const;

		void BindVAO() const;
		
		void UnBindVAO() const;

		void render();
};