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

	//Do not delete ModelResource, it can be sued by another object
	//ResourceManager is going to delete it
}

int Model::Load(ModelResource* modelResurce) {

	int res;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	std::vector<GLushort> wiredIndices;

	_modelResource = modelResurce;

	NfgParser parser;
	res = parser.Load(_modelResource->FilePath(), vertices, indices);
	if (res != NFG_SUCCES) {
		std::cout << parser.GetError() << std::endl;
		return MY_ERROR_CODE;
	}

	CreateWiredindicesBuffer(indices, wiredIndices);
	_modelResource->Vertices = vertices;
	_modelResource->Indices = indices;
	_modelResource->WiredIndices = wiredIndices;

	FillVerticesColor();
	
	//bind and load vertices buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBufferData(GL_ARRAY_BUFFER, _modelResource->Vertices.size() * sizeof(Vertex), _modelResource->Vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bind and load indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _modelResource->Indices.size() * sizeof(GLushort), _modelResource->Indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//bind and load indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboidWired);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _modelResource->WiredIndices.size() * sizeof(GLushort), _modelResource->WiredIndices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return MY_SUCCES_CODE;
}

int Model::LoadFlatTerrain() {

	int res;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	std::vector<GLushort> wiredIndices;

	GenerateFlatTerrain(240, 240, 80, 80, vertices, indices);

	_modelResource = new ModelResource();

	CreateWiredindicesBuffer(indices, wiredIndices);
	_modelResource->Vertices = vertices;
	_modelResource->Indices = indices;
	_modelResource->WiredIndices = wiredIndices;

	FillVerticesColor();

	//bind and load vertices buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBufferData(GL_ARRAY_BUFFER, _modelResource->Vertices.size() * sizeof(Vertex), _modelResource->Vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bind and load indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _modelResource->Indices.size() * sizeof(GLushort), _modelResource->Indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//bind and load indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboidWired);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _modelResource->WiredIndices.size() * sizeof(GLushort), _modelResource->WiredIndices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return MY_SUCCES_CODE;
}
void Model::GenerateFlatTerrain(float width, float depth, int numVerticesWidth, int numVerticesDepth, std::vector<Vertex>& vertices, std::vector<GLushort>& indices)
{
	// Calculate the size of each grid cell
	float cellWidth = width / (numVerticesWidth - 1);
	float cellDepth = depth / (numVerticesDepth - 1);

	Vertex temp;

	// Generate the vertices
	for (int dz = 0; dz < numVerticesDepth; ++dz)
	{
		for (int wx = 0; wx < numVerticesWidth; ++wx)
		{
			float x = wx * cellWidth;
			float z = dz * cellDepth;
			float y = 0.0f; // Flat terrain, y-coordinate is 0

			// Add vertex position
			temp.pos.x = x;
			temp.pos.y = y;
			temp.pos.z = z;

			temp.color = Vector3(1.0f, 0.0f, 1.0f);
			temp.norm = Vector3(0.0f, 1.0f, 0.0f);
			
			vertices.push_back(temp);
		}
	}

	for (int dz = 0; dz < numVerticesDepth - 1; ++dz)
	{
		for (int wx = 0; wx < numVerticesWidth - 1; ++wx)
		{
			// Calculate indices for the current cell
			unsigned int topLeft = dz * numVerticesWidth + wx;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = (dz + 1) * numVerticesWidth + wx;
			unsigned int bottomRight = bottomLeft + 1;

			// Create two triangles for the current cell
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
}

void Model::BindFilled() {

	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboid);
}
void Model::BindWired() {

	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboidWired);
}
void Model::Unbind() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// PRIVATE

void Model::CreateWiredindicesBuffer(std::vector<GLushort>& indices, std::vector<GLushort>& wiredIndices) {

	wiredIndices.reserve(indices.size() * 2);

	for (int i = 0; i < indices.size(); i += 3) {
		uint32_t index1 = indices[i];
		uint32_t index2 = indices[i + 1];
		uint32_t index3 = indices[i + 2];

		wiredIndices.push_back(index1);
		wiredIndices.push_back(index2);
		wiredIndices.push_back(index2);
		wiredIndices.push_back(index3);
		wiredIndices.push_back(index3);
		wiredIndices.push_back(index1);
	}
}

void Model::FillVerticesColor() {

	for (int i = 0; i < _modelResource->Vertices.size(); i++) {

		_modelResource->Vertices[i].color.x = 1.0f;
		_modelResource->Vertices[i].color.y = 1.0f;
		_modelResource->Vertices[i].color.z = 1.0f;
	}
}