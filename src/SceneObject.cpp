#include "stdafx.h"

#include "SceneObject.h"
#include "Vertex.h"
#include "Input.h"
#include "SceneManager.h"

SceneObject::SceneObject()
	: _model(nullptr), _shader(nullptr) {
}

SceneObject::~SceneObject() {

	//i dont want to free the pointers here, because they are create in Resource manager
	/// they can be used later or by more SceneObjects
	//	they will be destroyd bu ResourceManager when it is the case
}

void SceneObject::Update(float deltaTime) {

	//_rotation.y += 0.01f;

	if (_rotation.y > 180 * 2 * 3.14) {
		_rotation.y -= 180 * 2 * 3.14;
	}

	if (Input::GetKeyDown(KeyCode::K)) {

		_rotation.y += 0.25f;
	}

}

void SceneObject::Draw(Camera* camera) {

	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;


	_model->BindFilled();
	_shader->Bind();
	for (unsigned int i = 0; i < _textureResources.size(); i++) {
		_textureResources[i]->Bind(i);
	}

	_shader->SetAttributes();

	SetUniformsCommon(camera);
	SetUniformsParticular(camera);

	glDrawElements(GL_TRIANGLES, _model->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}
void SceneObject::DrawWired(Camera* camera) {

	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;


	_shader->Bind();
	_model->BindWired();
	for (unsigned int i = 0; i < _textureResources.size(); i++) {
		_textureResources[i]->Bind(i);
	}

	_shader->SetAttributes();

	SetUniformsCommon(camera);
	SetUniformsParticular(camera);

	glDrawElements(GL_LINES, _model->GetIndicesWiredCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}

void SceneObject::SetModel(Model* model) {

	_model = model;
}

void SceneObject::SetShader(Shader* shader) {

	_shader = shader;
}

void SceneObject::AddTexture(Texture* texture) {

	_textureResources.push_back(texture);
}

void SceneObject::SetMaterial(Material* material) {

	_material = material;
}

// PROTECTED 

void SceneObject::FollowCamera() {

	SceneManager& sceneManager = SceneManager::GetInstance();
	Vector3 cameraPos = sceneManager.GetActiveCamera()->GetPosition();

	Vector3 move = _position;
	move.x = _followCameraDirections.x != 0 ? cameraPos.x + _followCameraOffset.x : move.x;
	move.y = _followCameraDirections.y != 0 ? cameraPos.y + _followCameraOffset.y : move.y;
	move.z = _followCameraDirections.z != 0 ? cameraPos.z + _followCameraOffset.z : move.z;


	SetPosition(move);
}

Matrix SceneObject::GetModelMatrix() {

	Matrix positionMat;
	positionMat.SetTranslation(_position);

	Matrix rotationMatOY;
	rotationMatOY.SetRotationY(_rotation.y * PI / 180.f);
	Matrix rotationMatOX;
	rotationMatOX.SetRotationX(_rotation.x * PI / 180.f);
	Matrix rotationMatOZ;
	rotationMatOZ.SetRotationZ(_rotation.z * PI / 180.f);

	Matrix scaleMat;
	scaleMat.SetScale(_scale);

	return scaleMat * rotationMatOX * rotationMatOZ * rotationMatOY * positionMat;
}

void SceneObject::SetUniformsCommon(Camera* camera) {

	Matrix model = GetModelMatrix();
	Matrix mvp = model * camera->GetMVP();
	_shader->SetUniformMatrix4fv("u_mvp", mvp);
	_shader->SetUniform3f("u_cameraPos", camera->GetPosition());

	_shader->SetUniformMatrix4fv("u_model", model);
	_shader->SetUniform1i("u_TextureCube", 1);
	_shader->SetUniform1f("u_factorTexture", _material->GetFactorTextura());
	_shader->SetUniform1f("u_factorReflect", _material->GetFactorReflexieTextura());

	SceneManager& sceneManager = SceneManager::GetInstance();
	Fog fog = sceneManager.GetFog();
	_shader->SetUniform1f("u_fogNear", fog.NearPlane);
	_shader->SetUniform1f("u_fogFar", fog.FarPlane);
	_shader->SetUniform3f("u_fogColor", fog.Color);

	Light* debugLight = sceneManager.GetLight(2);
	_shader->SetUniform3f("u_objectColor", 1.0, 1.0, 1.0);
	_shader->SetUniform3f("u_cameraPos", camera->GetPosition());
	_shader->SetUniform3f("u_lightColor", debugLight->GetDiffuseColor());
	_shader->SetUniform3f("u_lightPos", 0.0, 0.0, 0.0);
	_shader->SetUniform1f("u_ambientFactor", 0.2);
	_shader->SetUniform1f("u_specularFactor", 0.8);
	_shader->SetUniform1f("u_diffuseFactor", 0.5);

}
void SceneObject::SetUniformsParticular(Camera* camera) {

	_shader->SetUniform1i("u_Texture", 0);
}