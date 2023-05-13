
#include "stdafx.h"
#include "TerrainObject.h"

#include "SceneManager.h"
#include "ResourceManager.h"

TerrainObject::TerrainObject(int size, int cellCount) 
	: _size(size), _countCells(cellCount) {

	_cellsDepls = Vector3(1, 1,1);
}

void TerrainObject::Update(float deltaTime) {

	SceneManager& sceneManager = SceneManager::GetInstance();
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	Camera* camera = sceneManager.GetActiveCamera();
	Vector3 cameraPos = camera->GetPosition();

	_size = resourceManager._terrainSize.x;
	_countCells = resourceManager._terrainCountCells.x;
	_width = _size / _countCells;

	Vector3 origin(0, 0, 0);

	Vector3 depl = (cameraPos - _position);
	//depl.x = (int)depl.x; 
	//depl.z = (int)depl.z;

	if (depl.z >= _width) {

		SetPosition(_position.x,0.0, _position.z + _width);
		_deplasament = (_position - origin) / _width;

		std::cout << depl.z << " " << (int)_deplasament.z << " " << _position.z << "\n";
	}
	if (depl.x >= _width) {

		SetPosition(_position.x + depl.x, 0.0, _position.z);
		_deplasament = (_position - origin) / _width;
	}

	//std::cout << _position.x << " " << _position.z << "\n";
}

void TerrainObject::SetUniformsParticular(Camera* camera) {

	_shader->SetUniform1i("u_TextureBlend", 0);
	_shader->SetUniform1i("u_TextureDirt", 1);
	_shader->SetUniform1i("u_TextureRock", 2);
	_shader->SetUniform1i("u_TextureGrass", 3);

	_shader->SetUniform2f("u_deplasament", (int)_deplasament.z, (int)_deplasament.x);
	_shader->SetUniform1f("u_countCells", _countCells );
}