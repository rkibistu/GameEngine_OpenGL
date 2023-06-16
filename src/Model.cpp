#include "stdafx.h"
#include "Model.h"
#include "NfgParser.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <climits>

Model::Model() 
	: _modelResource(nullptr){

	glGenBuffers(1, &_iboid);
	glGenBuffers(1, &_iboidWired);
	glGenBuffers(1, &_vboid);

	_aabbCollider = nullptr;
}
Model::~Model() {

	glDeleteBuffers(1, &_iboid);
	glDeleteBuffers(1, &_iboidWired);
	glDeleteBuffers(1, &_vboid);

	if (_aabbCollider) {
		delete _aabbCollider;
		_aabbCollider = nullptr;
	}

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
	
	BindAndLoadVertices();

	return MY_SUCCES_CODE;
}

int Model::LoadFlatTerrain(int sizeWidht, int sizeHeight, int cellCountWidth, int cellCountHeight) {

	int res;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	std::vector<GLushort> wiredIndices;
	
	GenerateFlatTerrain(sizeWidht, sizeHeight, cellCountWidth, cellCountHeight, vertices, indices);

	_modelResource = new ModelResource();

	CreateWiredindicesBuffer(indices, wiredIndices);
	_modelResource->Vertices = vertices;
	_modelResource->Indices = indices;
	_modelResource->WiredIndices = wiredIndices;

	//FillVerticesColor();

	BindAndLoadVertices();

	return MY_SUCCES_CODE;
}

int Model::LoadSystemAxis() {

	int res;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	std::vector<GLushort> wiredIndices;

	Vertex vertex;

	//OZ
	vertex.pos = Vector3(0.0, 0.0, 0.0);
	vertex.color = Vector3(0.0, 0.0, 1.0);
	vertices.push_back(vertex);
	vertex.pos = Vector3(0.0, 0.0, 1.0);
	vertex.color = Vector3(0.0, 0.0, 1.0);
	vertices.push_back(vertex);


	//OY
	vertex.pos = Vector3(0.0, 0.0, 0.0);
	vertex.color = Vector3(0.0, 1.0, 0.0);
	vertices.push_back(vertex);
	vertex.pos = Vector3(0.0, 1.0, 0.0);
	vertex.color = Vector3(0.0, 1.0, 0.0);
	vertices.push_back(vertex);

	//OX
	vertex.pos = Vector3(0.0, 0.0, 0.0);
	vertex.color = Vector3(1.0, 0.0, 0.0);
	vertices.push_back(vertex);
	vertex.pos = Vector3(1.0, 0.0, 0.0);
	vertex.color = Vector3(1.0, 0.0, 0.0);
	vertices.push_back(vertex);

	//indices
	wiredIndices.push_back(0);
	wiredIndices.push_back(1);
	wiredIndices.push_back(2);
	wiredIndices.push_back(3);
	wiredIndices.push_back(4);
	wiredIndices.push_back(5);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	_modelResource = new ModelResource();
	_modelResource->Vertices = vertices;
	_modelResource->Indices = indices;
	_modelResource->WiredIndices = wiredIndices;

	BindAndLoadVertices();	

	return MY_SUCCES_CODE;
} 

int Model::LoadNormalModel(std::vector<Vertex>& vertices) {

	std::vector<Vertex> normalVerteces;
	std::vector<unsigned short> normalIndices;

	int lineLength = 10;

	Vector3 startPoint;
	Vector3 endPoint;
	Vertex temp;
	int index = 0;
	for (auto it = vertices.begin(); it != vertices.end(); it++) {

		startPoint = it->pos;
		endPoint = startPoint + it->norm * lineLength;

		temp.pos = startPoint;
		temp.color = Vector3(1.0f, 1.0f, 0.0f);
		normalVerteces.push_back(temp);
		normalIndices.push_back(index++);
		
		temp.pos = endPoint;
		temp.color = Vector3(1.0f, 1.0f, 0.0f);
		normalVerteces.push_back(temp);
		normalIndices.push_back(index++);
	}

	_modelResource = new ModelResource();
	_modelResource->Vertices = normalVerteces;
	_modelResource->WiredIndices = normalIndices;

	BindAndLoadVertices();

	return MY_SUCCES_CODE;
}

int Model::LoadLineUp(Vector3 up, float length) {

	Vector3 origin(0.0f, 0.0f, 0.0f);
	Vector3 color(1.0f, 1.0f, 0.0f);

	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	std::vector<GLushort> wiredIndices;

	Vertex vertex;
	vertex.pos = origin;
	vertex.color = color;
	vertices.push_back(vertex);

	vertex.pos = (origin + up) * length;
	vertex.color = color;
	vertices.push_back(vertex);

	wiredIndices.push_back(0);
	wiredIndices.push_back(1);

	_modelResource = new ModelResource();
	_modelResource->Vertices = vertices;
	_modelResource->Indices = indices;
	_modelResource->WiredIndices = wiredIndices;

	BindAndLoadVertices();

	return MY_SUCCES_CODE;
}

int Model::LoadAabbModel(std::vector<Vertex>& vertices) {

	std::vector<Vertex> aabbVertices;
	std::vector<unsigned short> aabbWiredIndices;
	std::vector<unsigned short> aabbIndices;

	//Vector2 (min_value, max_valuea)
	Vector2 ox(INT_MAX, INT_MIN);
	Vector2 oy(INT_MAX, INT_MIN);
	Vector2 oz(INT_MAX, INT_MIN);
	for (auto it = vertices.begin(); it != vertices.end(); it++) {

		//min max pe axa OX
		if (it->pos.x < ox.x)
			ox.x = it->pos.x;
		if (it->pos.x > ox.y) {
			ox.y = it->pos.x;
		}

		//min max pe axa OY
		if (it->pos.y < oy.x)
			oy.x = it->pos.y;
		if (it->pos.y > oy.y) {
			oy.y = it->pos.y;
		}

		//min max pe axa OZ
		if (it->pos.z < oz.x)
			oz.x = it->pos.z;
		if (it->pos.z > oz.y) {
			oz.y = it->pos.z;
		}
	}

	if (_aabbCollider == nullptr)
		_aabbCollider = new AabbCollider();
	_aabbCollider->OX = ox;
	_aabbCollider->OY = oy;
	_aabbCollider->OZ = oz;

	Vertex temp;
	temp.pos = Vector3(ox.x, oy.x, oz.x);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.x, oz.x);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.y, oz.x);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.x, oy.y, oz.x);
	aabbVertices.push_back(temp);

	temp.pos = Vector3(ox.x, oy.x, oz.y);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.x, oz.y);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.y, oz.y);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.x, oy.y, oz.y);
	aabbVertices.push_back(temp);


	aabbWiredIndices.push_back(0);
	aabbWiredIndices.push_back(1);
	aabbWiredIndices.push_back(0);
	aabbWiredIndices.push_back(3);
	aabbWiredIndices.push_back(0);
	aabbWiredIndices.push_back(4);
	aabbWiredIndices.push_back(1);
	aabbWiredIndices.push_back(5);
	aabbWiredIndices.push_back(1);
	aabbWiredIndices.push_back(2);
	aabbWiredIndices.push_back(3);
	aabbWiredIndices.push_back(2);
	aabbWiredIndices.push_back(3);
	aabbWiredIndices.push_back(7);
	aabbWiredIndices.push_back(7);
	aabbWiredIndices.push_back(4);
	aabbWiredIndices.push_back(7);
	aabbWiredIndices.push_back(6);
	aabbWiredIndices.push_back(6);
	aabbWiredIndices.push_back(5);
	aabbWiredIndices.push_back(6);
	aabbWiredIndices.push_back(2);
	aabbWiredIndices.push_back(4);
	aabbWiredIndices.push_back(5);

	if (_modelResource)
		delete _modelResource;
	_modelResource = new ModelResource();
	_modelResource->Vertices = aabbVertices;
	_modelResource->WiredIndices = aabbWiredIndices;

	FillVerticesColor(Vector3(0.0, 1.0, 1.0));

	BindAndLoadVertices();

	return MY_SUCCES_CODE;
}

int Model::LoadAabbModelCamera(int width, int height) {

	std::vector<Vertex> aabbVertices;
	std::vector<unsigned short> aabbWiredIndices;
	std::vector<unsigned short> aabbIndices;

	Vector2 ox(-width, width);
	Vector2 oy(-height,height);
	Vector2 oz(-width,width);

	if (_aabbCollider == nullptr)
		_aabbCollider = new AabbCollider();
	_aabbCollider->OX = ox;
	_aabbCollider->OY = oy;
	_aabbCollider->OZ = oz;

	Vertex temp;
	temp.pos = Vector3(ox.x, oy.x, oz.x);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.x, oz.x);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.y, oz.x);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.x, oy.y, oz.x);
	aabbVertices.push_back(temp);

	temp.pos = Vector3(ox.x, oy.x, oz.y);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.x, oz.y);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.y, oy.y, oz.y);
	aabbVertices.push_back(temp);
	temp.pos = Vector3(ox.x, oy.y, oz.y);
	aabbVertices.push_back(temp);


	aabbWiredIndices.push_back(0);
	aabbWiredIndices.push_back(1);
	aabbWiredIndices.push_back(0);
	aabbWiredIndices.push_back(3);
	aabbWiredIndices.push_back(0);
	aabbWiredIndices.push_back(4);
	aabbWiredIndices.push_back(1);
	aabbWiredIndices.push_back(5);
	aabbWiredIndices.push_back(1);
	aabbWiredIndices.push_back(2);
	aabbWiredIndices.push_back(3);
	aabbWiredIndices.push_back(2);
	aabbWiredIndices.push_back(3);
	aabbWiredIndices.push_back(7);
	aabbWiredIndices.push_back(7);
	aabbWiredIndices.push_back(4);
	aabbWiredIndices.push_back(7);
	aabbWiredIndices.push_back(6);
	aabbWiredIndices.push_back(6);
	aabbWiredIndices.push_back(5);
	aabbWiredIndices.push_back(6);
	aabbWiredIndices.push_back(2);
	aabbWiredIndices.push_back(4);
	aabbWiredIndices.push_back(5);

	if (_modelResource)
		delete _modelResource;
	_modelResource = new ModelResource();
	_modelResource->Vertices = aabbVertices;
	_modelResource->WiredIndices = aabbWiredIndices;

	FillVerticesColor(Vector3(0.0, 1.0, 1.0));

	BindAndLoadVertices();

	return MY_SUCCES_CODE;
}

int Model::LoadTextQuad() {

	int res;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	std::vector<GLushort> wiredIndices;

	Vertex vertex;
	vertex.pos = Vector3(-10.0, -10.0, 0.0);
	vertex.color = Vector3(0.0, 0.0, 1.0);
	vertices.push_back(vertex);
	vertex.pos = Vector3(10.0, -10.0, 0.0);
	vertex.color = Vector3(0.0, 0.0, 1.0);
	vertices.push_back(vertex);
	vertex.pos = Vector3(10.0, 10.0, 0.0);
	vertex.color = Vector3(0.0, 0.0, 1.0);
	vertices.push_back(vertex);
	vertex.pos = Vector3(-10.0, 10.0, 0.0);
	vertex.color = Vector3(0.0, 0.0, 1.0);
	vertices.push_back(vertex);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	CreateWiredindicesBuffer(indices, wiredIndices);

	_modelResource = new ModelResource();
	_modelResource->Vertices = vertices;
	_modelResource->Indices = indices;
	_modelResource->WiredIndices = wiredIndices;

	BindAndLoadVertices();

	return MY_SUCCES_CODE;
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
 
int Model::UpdateAabbModel(std::vector<Vertex> vertices, Vector3 scale, Vector3 rotation) {

	//std::vector<Vertex> aabbVertices;
	//aabbVertices = _modelResource->Vertices;

	Matrix scaleMatrix;
	scaleMatrix.SetScale(scale);
	Matrix rotationOx;
	rotationOx.SetRotationX(- rotation.x);
	Matrix rotationOy;
	rotationOy.SetRotationY(- rotation.y);
	Matrix rotationOz;
	rotationOz.SetRotationZ(- rotation.z);

	Vector4 temp;
	for (auto it = vertices.begin(); it != vertices.end(); it++) {

		temp = scaleMatrix *  rotationOy * rotationOx * rotationOz *   Vector4(it->pos, 1.0f);
		it->pos = Vector3(temp);
	}
	LoadAabbModel(vertices); 
	//BindAndLoadVertices();

	return MY_SUCCES_CODE;
}

// PRIVATE

void Model::BindAndLoadVertices() {
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
}

void Model::CreateWiredindicesBuffer(std::vector<GLushort>& indices, std::vector<GLushort>& wiredIndices) {

	wiredIndices.reserve(indices.size() * 2);

	for (unsigned int i = 0; i < indices.size(); i += 3) {
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

void Model::FillVerticesColor(Vector3 fillColor) {

	Vector3 color;
	for (unsigned int i = 0; i < _modelResource->Vertices.size(); i++) {

		color = _modelResource->Vertices[i].color;
		if (color.x == 0 && color.y == 0 && color.z == 0) {
			_modelResource->Vertices[i].color.x = fillColor.x;
			_modelResource->Vertices[i].color.y = fillColor.y;
			_modelResource->Vertices[i].color.z = fillColor.z;
		}
		
	}
}

// IMPORTANT: numBerticesWidth * numVerticesDepth sa nu fie mai mare decat range-ul pt unsigned short!!
void Model::GenerateFlatTerrain(float width, float depth, int numCellsWidth, int numCellsDepth, std::vector<Vertex>& vertices, std::vector<GLushort>& indices)
{
	// Calculate the size of each grid cell

	int numVerticesWidth = numCellsWidth + 1;
	int numVerticesDepth = numCellsDepth + 1;

	float cellWidth = width / (numVerticesWidth - 1);
	float cellDepth = depth / (numVerticesWidth - 1);

	Vertex temp;

	// Generate the vertices
	for (int dz = 0; dz < numVerticesDepth; ++dz)
	{
		for (int wx = 0; wx < numVerticesWidth; ++wx)
		{
			float x = wx * cellWidth -width / 2;
			float z = dz * cellDepth  -depth / 2;
			float y = 0.0f; // Flat terrain, y-coordinate is 0

			// Add vertex position
			temp.pos.x = x;
			temp.pos.y = y;
			temp.pos.z = z;

			temp.color = Vector3(0.5f, 0.0f, 0.5f);
			temp.norm = Vector3(0.0f, 1.0f, 0.0f);
			
			//asta e textura mare, ai nevoie pentru harta de culori
			float u = static_cast<float>(dz) / (numVerticesWidth);
			float v = static_cast<float>(wx) / (numVerticesDepth);
			temp.uv = Vector2(u, v);

			float u2 = u * numCellsWidth;
			float v2 = v * numCellsDepth;
			temp.uv2 = Vector2(u2, v2);
			

			vertices.push_back(temp);
		}
	}

	for (int dz = 0; dz < numVerticesDepth - 1; ++dz)
	{
		for (int wx = 0; wx < numVerticesWidth - 1; ++wx)
		{
			// Calculate indices for the current cell
			unsigned short topLeft = dz * numVerticesWidth + wx;
			unsigned short topRight = topLeft + 1;
			unsigned short bottomLeft = (dz + 1) * numVerticesWidth + wx;
			unsigned short bottomRight = bottomLeft + 1;

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


