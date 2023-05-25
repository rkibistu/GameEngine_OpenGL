#include "stdafx.h"

#include "SceneObject.h"
#include "Vertex.h"
#include "Input.h"
#include "SceneManager.h"
#include "ResourceManager.h"

SceneObject::SceneObject(bool isDebugObj) {

	SceneManager& sceneManager = SceneManager::GetInstance();
	SetShader(sceneManager.GetDefaultShader());
	SetMaterial(sceneManager.GetDefaultMaterial());

	_debugShader = sceneManager.GetDebugShader();

	_position = Vector3(0.0f, 0.0f, 0.0f);
	_rotation = Vector3(0.0f, 0.0f, 0.0f);
	_scale = Vector3(1.0f, 1.0f, 1.0f);

	_parent = nullptr;

	_isDebug = isDebugObj;
	_drawWired = false;
	_name = "sceneObject";

	if (!isDebugObj)
		CreateDebugObjects();
}

SceneObject::~SceneObject() {

	//i dont want to free the pointers here, because they are create in Resource manager
	/// they can be used later or by more SceneObjects
	//	they will be destroyd bu ResourceManager when it is the case

	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

		delete it->second;
	}
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
	//Draw wired + call draw for all debug objects
	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;

	_shader->Bind();
	_model->BindWired();

	_shader->SetAttributes();

	SetUniformsCommon(camera);
	SetUniformsParticular(camera);

	glDrawElements(GL_LINES, _model->GetIndicesWiredCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}
void SceneObject::DrawDebug(Camera* camera) {

	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;

	_model->BindFilled();
	_debugShader->Bind();
	for (unsigned int i = 0; i < _textureResources.size(); i++) {
		_textureResources[i]->Bind(i);
	}

	_debugShader->SetAttributes();

	SetUniformsCommonDebug(camera);
	SetUniformsParticularDebug(camera);

	glDrawElements(GL_TRIANGLES, _model->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}
void SceneObject::DrawDebugWired(Camera* camera) {
	//Draw wired + call draw for all debug objects
	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;

	_debugShader->Bind();
	_model->BindWired();

	_debugShader->SetAttributes();

	SetUniformsCommonDebug(camera);
	SetUniformsParticularDebug(camera);

	glDrawElements(GL_LINES, _model->GetIndicesWiredCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}

void SceneObject::SetModel(Model* model) {

	_model = model;

	//create normal mode
	Model* normalModel = new Model();
	normalModel->LoadNormalModel(_model->GetModelResource()->Vertices);
	SceneObject* normalsObject = new SceneObject(true);
	normalsObject->SetParent(this);
	normalsObject->_model = normalModel;
	normalsObject->SetName("normals");
	normalsObject->SetDrawWired(true);
	_debugObjects.insert({ _debugObjects.size() + 1,normalsObject });

	//create AABB
	Model* aabbModel = new Model();
	aabbModel->LoadAabbModel(_model->GetModelResource()->Vertices);
	SceneObject* aabbObject = new SceneObject(true);
	aabbObject->SetParent(this);
	aabbObject->_model = aabbModel;
	aabbObject->SetName("aabb");
	aabbObject->SetDrawWired(true);
	_debugObjects.insert({ _debugObjects.size() + 1,aabbObject });

}

void SceneObject::SetShader(Shader* shader) {

	_shader = shader;
}
void SceneObject::SetDebugShader(Shader* shader) {

	_debugShader = shader;
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

	SceneManager& sceneManager = SceneManager::GetInstance();

	//mvp + camera
	Matrix model = GetModelMatrix();
	Matrix mvp = model * camera->GetMVP();
	_shader->SetUniformMatrix4fv("u_mvp", mvp);

	_shader->SetUniform3f("u_cameraPos", camera->GetPosition());
	_shader->SetUniformMatrix4fv("u_model", model);

	_shader->SetUniform1i("u_Texture", 0);

	// skybox
	_shader->SetUniform1i("u_TextureCube", 1);
	_shader->SetUniform1f("u_factorTexture", _material->GetFactorTextura());
	_shader->SetUniform1f("u_factorReflect", _material->GetFactorReflexieTextura());

	//fog
	Fog fog = sceneManager.GetFog();
	_shader->SetUniform1f("u_fogNear", fog.NearPlane);
	_shader->SetUniform1f("u_fogFar", fog.FarPlane);
	_shader->SetUniform3f("u_fogColor", fog.Color);

	//lights
	std::string lightUniformBase = "u_lights[";
	std::string lightUniform;
	auto lights = sceneManager.GetLights();
	int index = 0;
	for (auto it = lights.begin(); it != lights.end(); it++) {

		lightUniformBase = "u_lights[";
		lightUniformBase += std::to_string(index);
		lightUniformBase += "].";

		lightUniform = lightUniformBase + "diffuseColor";
		_shader->SetUniform3f(lightUniform, it->second->GetDiffuseColor());
		lightUniform = lightUniformBase + "specularColor";
		_shader->SetUniform3f(lightUniform, it->second->GetSpecularColor());
		lightUniform = lightUniformBase + "type";
		_shader->SetUniform1i(lightUniform, it->second->GetType());
		lightUniform = lightUniformBase + "position";
		_shader->SetUniform3f(lightUniform, it->second->GetPosition());

		//add this 2 in xml!!
		lightUniform = lightUniformBase + "direction";
		_shader->SetUniform3f(lightUniform, it->second->GetPosition());
		lightUniform = lightUniformBase + "spotAngle";
		_shader->SetUniform1f(lightUniform, 0.7f);

		index++;
	}
	_shader->SetUniform1i("u_lightsCount", lights.size());
	//ambiental
	_shader->SetUniform3f("u_ambientColor", sceneManager.GetAmbientalLight().Color);
	_shader->SetUniform1f("u_ambientRatio", sceneManager.GetAmbientalLight().Ratio);


	//mai jos sunt materiale
	_shader->SetUniform1f("u_ambientFactor", 0.2);
	_shader->SetUniform1f("u_specularFactor", 0.8);
	_shader->SetUniform1f("u_diffuseFactor", 0.5);
}
void SceneObject::SetUniformsParticular(Camera* camera) {


}
void SceneObject::SetUniformsCommonDebug(Camera* camera) {

	SceneManager& sceneManager = SceneManager::GetInstance();

	//mvp + camera
	Matrix model = GetModelMatrix();
	Matrix mvp = model * camera->GetMVP();
	_shader->SetUniformMatrix4fv("u_mvp", mvp);
}
void SceneObject::SetUniformsParticularDebug(Camera* camera){
}

void SceneObject::CreateDebugObjects() {
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	//create debug objects specific to all scene objects
	SceneObject* axisObject = new SceneObject(true);
	axisObject->SetParent(this);
	axisObject->SetModel(resourceManager.GetSystemAxisModel());
	axisObject->SetName("axis");
	axisObject->SetScale(10.0f, 10.0f, 10.0f);
	axisObject->SetDrawWired(true);

	_debugObjects.insert({ _debugObjects.size() + 1,axisObject });
}

void SceneObject::UpdateDebugObjects(float deltaTime) {

	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

		it->second->SetPosition(_position);
		it->second->SetRotation(_rotation);

		//asta trb abstractizata de aici. e ok momentan
		if(it->second->GetName() == "normals")
			it->second->SetScale(_scale);

		if (it->second->GetName() == "aabb") {
			it->second->_model->UpdateAabbModel(_model->GetModelResource()->Vertices, _scale);
		}
	}
}
void SceneObject::DrawDebugObjects(Camera* camera) {

	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {
		if(it->second->GetDrawWired())
			it->second->DrawDebugWired(camera);
		else
			it->second->DrawDebug(camera);
	}
}
