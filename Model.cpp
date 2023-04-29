#include "stdafx.h"
#include "Model.h"
#include "NfgParser.h"
#include "Vertex.h"

Model::Model() 
	: _modelResource(nullptr){

	glGenBuffers(1, &_iboid);
	glGenBuffers(1, &_iboidWired);
	glGenBuffers(1, &_vboid);
}
Model::~Model() {

	glDeleteBuffers(1, &_iboid);
	glDeleteBuffers(1, &_iboidWired);
	glDeleteBuffers(1, &_vboid);

	if (_modelResource)
		delete _modelResource;
}

void Model::Load(std::string filepath) {

	int res;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;

	NfgParser parser;
	res = parser.Load(filepath, vertices, indices);
	if (res != NFG_SUCCES) {
		std::cout << parser.GetError() << std::endl;
		return;
	}

	_modelResource = new ModelResource(vertices,indices);

	FillVerticesColor();
	
	//bind and load vertices buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBufferData(GL_ARRAY_BUFFER, _modelResource->Vertices.size() * sizeof(Vertex), _modelResource->Vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bind and load indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _modelResource->Indices.size() * sizeof(GLushort), _modelResource->Indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::Bind() {

	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboid);
}
void Model::Unbind() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::FillVerticesColor() {

	for (int i = 0; i < _modelResource->Vertices.size(); i++) {

		_modelResource->Vertices[i].color.x = 1.0f;
		_modelResource->Vertices[i].color.y = 1.0f;
		_modelResource->Vertices[i].color.z = 1.0f;
	}
}