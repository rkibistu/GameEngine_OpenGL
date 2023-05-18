
#include "stdafx.h"
#include "TerrainObject.h"

#include "SceneManager.h"
#include "ResourceManager.h"

TerrainObject::TerrainObject(int size, int cellCount)
	: _size(size), _countCells(cellCount) {

	_width = _size / _countCells;

	if (_size % _countCells != 0) {
		std::cout << "ATENTIE!! Pentru teren size/_countCells ar trb sa fie un rezultat INTREG!\n";
	}
}
TerrainObject::TerrainObject(int size, int cellCount, Vector3 heights)
	: TerrainObject(size,cellCount) {

	_heigths = heights;
}
void TerrainObject::Update(float deltaTime) {

	SceneManager& sceneManager = SceneManager::GetInstance();

	Vector3 origin(0, 0, 0);
	Camera* camera = sceneManager.GetActiveCamera();
	Vector3 cameraPos = camera->GetPosition();

	Vector3 depl = (cameraPos - _position);

	if (depl.z >= _width) {

		SetPosition(_position.x, 0.0, _position.z + _width);
	}
	if (depl.z <= -_width) {

		SetPosition(_position.x, 0.0, _position.z - _width);
	}
	if (depl.x >= _width) {

		SetPosition(_position.x + _width, 0.0, _position.z);
	}
	if (depl.x <= -_width) {

		SetPosition(_position.x - _width, 0.0, _position.z);
	}
	_deplasament = (_position - origin) / _width;
	_deplasament.x = (int)_deplasament.x;
	_deplasament.y = (int)_deplasament.y;
}

void TerrainObject::SetUniformsParticular(Camera* camera) {

	_shader->SetUniform1i("u_TextureBlend", 0);
	_shader->SetUniform1i("u_TextureDirt", 1);
	_shader->SetUniform1i("u_TextureRock", 2);
	_shader->SetUniform1i("u_TextureGrass", 3);
	_shader->SetUniform1i("u_TextureCube", 4);

	_shader->SetUniform2f("u_deplasament", _deplasament.z, _deplasament.x);
	_shader->SetUniform1f("u_countCells", _countCells);

	//_heigths = Vector3(15, 5, -5);
	_shader->SetUniform3f("u_heights", _heigths);
}