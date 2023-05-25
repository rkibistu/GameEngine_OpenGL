#pragma once

#include "Defines.h"

#include <vector>
#include "Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

struct Vertex;

struct ModelResource {

	std::vector<Vertex> Vertices;
	std::vector<GLushort> Indices;
	std::vector<GLushort> WiredIndices;

	unsigned int ID;
	std::string Path;
	std::string Filename;

	std::string FilePath() { return Path + Filename; }
};

class Model{

public:
	Model();
	~Model();

	int Load(ModelResource* modelResurce);
	int LoadFlatTerrain(int sizeWidht, int sizeHeight, int cellCountWidth, int cellCountHeight);
	int LoadSystemAxis();
	int LoadNormalModel(std::vector<Vertex>& vertices);
	int LoadAabbModel(std::vector<Vertex>& vertices);

	int UpdateAabbModel(std::vector<Vertex> vertices, Vector3 scale, Vector3 rotation);

	void BindFilled();
	void BindWired();
	void Unbind();

	inline unsigned int GetIndicesFilledCount() { return (_modelResource != nullptr) ? _modelResource->Indices.size() : 0; }
	inline unsigned int GetIndicesWiredCount() { return (_modelResource != nullptr) ? _modelResource->WiredIndices.size() : 0; }
	inline ModelResource* GetModelResource() { return _modelResource; }
private:

	//get indices for triangle mode and create indices for wired mode
	void CreateWiredindicesBuffer(std::vector<GLushort>& indices, std::vector<GLushort>& wiredIndices);

	//Bind and load to opengl the vertices and idncies buffers
	//	get values from _modelResource
	void BindAndLoadVertices();

	//fill the color attribute of vertex (most of models don't have this in nfg)
	// DebugShader use color to draw lines
	void FillVerticesColor(Vector3 fillColor = Vector3(1.0,1.0,1.0));

	//generate the terrain
	void GenerateFlatTerrain(float _width, float depth, int numCellsWidth, int numCellsDepth, std::vector<Vertex>& vertices, std::vector<GLushort>& indices);


private:
	ModelResource* _modelResource;
	
	GLuint _iboid; //index buffer
	GLuint _iboidWired; //index buffer
	GLuint _vboid; //vertex buffer

};

