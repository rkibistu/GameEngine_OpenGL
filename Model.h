#pragma once

#include <vector>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

struct Vertex;

struct ModelResource {

	std::vector<Vertex> Vertices;
	std::vector<short> Indices;

	ModelResource(std::vector<Vertex> vertices, std::vector<short> indices)
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
	
private:

	

private:
	ModelResource* _modelResource;
	GLuint _iboid; //index buffer
	GLuint _iboidWired; //index buffer
	GLuint _vboid; //vertex buffer

};

