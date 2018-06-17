#pragma once
#include <glm\glm.hpp>

class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	// struct for holding important gl shader variables
	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	// initialise functions
	void initialiseQuad();
	void initialiseFullscreenQuad();
	void initialise(unsigned int vertexCount, const Vertex* vertices,
		unsigned int indexCount,
		unsigned int* indices);

	// virtual draw function
	virtual void draw();

protected:
	// triangle count for mesh
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};