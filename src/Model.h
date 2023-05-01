#pragma once

#include <vector>
#include "Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

struct Vertex;

struct ModelResource {

	std::vector<Vertex> Vertices;
	std::vector<GLushort> Indices;
	std::vector<GLushort> WiredIndices;

	ModelResource(std::vector<Vertex> vertices, std::vector<GLushort> indices, std::vector<GLushort> wiredIndices)
	 : Vertices(vertices), Indices(indices), WiredIndices(wiredIndices) {

	}
};

class Model{

public:
	Model();
	~Model();

	void Load(std::string filepath);
	void BindFilled();
	void BindWired();
	void Unbind();

	inline unsigned int GetIndicesFilledCount() { return (_modelResource != nullptr) ? _modelResource->Indices.size() : 0; }
	inline unsigned int GetIndicesWiredCount() { return (_modelResource != nullptr) ? _modelResource->WiredIndices.size() : 0; }
	
private:

	void CreateWiredindicesBuffer(std::vector<GLushort>& indices, std::vector<GLushort>& wiredIndices);

	void FillVerticesColor();

private:
	ModelResource* _modelResource;
	GLuint _iboid; //index buffer
	GLuint _iboidWired; //index buffer
	GLuint _vboid; //vertex buffer

};

