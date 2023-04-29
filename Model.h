#pragma once

#include <vector>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

struct Vertex;

struct ModelResource {

	std::vector<Vertex> Vertices;
	std::vector<GLushort> Indices;

	ModelResource(std::vector<Vertex> vertices, std::vector<GLushort> indices)
	 : Vertices(vertices), Indices(indices) {

	}
};

class Model{

public:
	Model();
	~Model();

	void Load(std::string filepath);
	void Bind();
	void Unbind();

	inline unsigned int GetIndicesCount() { return (_modelResource != nullptr) ? _modelResource->Indices.size() : 0; }
	
private:

	void FillVerticesColor();

private:
	ModelResource* _modelResource;
	GLuint _iboid; //index buffer
	GLuint _iboidWired; //index buffer
	GLuint _vboid; //vertex buffer

};

