#pragma once
#include <GL/glew.h>

class VertexBuffer {
	private:
		unsigned int b_ID;
		unsigned int vao_ID;
		unsigned int ebo_ID;

		unsigned int size;
	public:
		VertexBuffer(const void* data, unsigned int size);

		~VertexBuffer();

		void BindVBO() const;

		void UnBindVBO() const;

		void BindEBO() const;

		void UnbindEBO() const;

		void BindVAO() const;
		
		void UnBindVAO() const;

		void CreateElementBufferObject(unsigned int* data, int size);

		void render();
};