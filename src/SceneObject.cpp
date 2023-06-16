#include "stdafx.h"

#include "SceneObject.h"
#include "Vertex.h"
#include "Input.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "DebugObjects/LocalCoordonatesAxeObject.h"
#include "DebugObjects/HitboxVisualObject.h"
#include "DebugObjects/NormalsVisualObject.h"

SceneObject::SceneObject(bool isDebugObj) {

	SceneManager& sceneManager = SceneManager::GetInstance();
	SetShader(sceneManager.GetDefaultShader());
	SetDebugShader(sceneManager.GetWiredShader());
	SetMaterial(sceneManager.GetDefaultMaterial());
	_model = nullptr;
	_trajectory = nullptr;
	_parent = nullptr;

	_wiredShader = sceneManager.GetWiredShader();

	_position = Vector3(0.0f, 0.0f, 0.0f);
	_rotation = Vector3(0.0f, 0.0f, 0.0f);
	_scale = Vector3(1.0f, 1.0f, 1.0f);

	_parent = nullptr;

	_isDebug = isDebugObj;
	_drawWired = false;
	_name = "sceneObject";

	_collidable = true;
	_collisionController = nullptr;

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

	if (_trajectory)
		delete _trajectory;
	if (_collisionController)
		delete _collisionController;
}

void SceneObject::Start() {

	//call start for children maybe
	if (_trajectory)
		_trajectory->Start();
	if (_collisionController)
		_collisionController->Start();
}

void SceneObject::Update(float deltaTime) {

	//_rotation.y += 0.01f;

	if (_rotation.y > 180 * 2 * 3.14) {
		_rotation.y -= 180 * 2 * 3.14;
	}
	if (Input::GetKeyDown(KeyCode::K)) {

		_rotation.y += 2.5f;
	}


	if (_trajectory) {

		_trajectory->Update(deltaTime, _position);
	}
	if (_collisionController) {

		_collisionController->Update(deltaTime);
	}
}

void SceneObject::Draw(Camera* camera) {

	if (_drawWired)
		DrawWired(camera);
	else
		DrawTriangles(camera);
}

void SceneObject::UpdateDebugObjects(float deltaTime) {

	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

		it->second->Update(deltaTime);
	}
}

void SceneObject::DrawDebugObjects(Camera* camera) {

	for (auto it = _debugObjects.begin(); it != _debugObjects.end(); it++) {

		it->second->Draw(camera);
	}
}

void SceneObject::OnCollisionEnter(SceneObject* collisionObj) {

	std::cout << "Enter collide " << GetName() << " with " << collisionObj->GetName() << std::endl;

}
void SceneObject::OnCollisionStay(SceneObject* collisionObj) {

	//std::cout << "Stay collide " << GetName() << " with " << collisionObj->GetName() << std::endl;
}
void SceneObject::OnCollisionExit(SceneObject* collisionObj) {

	std::cout << "Exit collide " << GetName() << " with " << collisionObj->GetName() << std::endl;
}

void SceneObject::DrawTriangles(Camera* camera) {

	if (_model == nullptr)
		return;
	if (_shader == nullptr)
		return;

	if (_name == "textquad")
		int x = 3;

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
	if (_wiredShader == nullptr)
		return;

	_wiredShader->Bind();
	_model->BindWired();

	_wiredShader->SetAttributes();

	SetUniformsCommon(camera);
	SetUniformsParticular(camera);

	glDrawElements(GL_LINES, _model->GetIndicesWiredCount(), GL_UNSIGNED_SHORT, nullptr);

	_model->Unbind();
}

void SceneObject::SetModel(Model* model) {

	_model = model;

	if (_isDebug)
		return;

	//create normal mode
	SceneObject* normalsObject = new NormalsVisualObject(_model);
	normalsObject->SetParent(this);
	_debugObjects.insert({ _debugObjects.size() + 1,normalsObject });

	//create AABB
	_collisionController = new CollisionControllerSceneObject(this, _model);

	SceneObject* aabbObject = new HitboxVisualObject(this, _collisionController->GetAabbModel());
	//aabbObject->SetParent(this);
	_debugObjects.insert({ _debugObjects.size() + 1,aabbObject });
}

void SceneObject::SetShader(Shader* shader) {

	_shader = shader;
}
void SceneObject::SetDebugShader(Shader* shader) {

	_wiredShader = shader;
}

void SceneObject::SetTrajectory(Trajectory* trajectory) {

	if (trajectory == nullptr)
		return;

	_trajectory = trajectory;
	_trajectory->SetToStartPosition(_position); //set object position to start of trajectory
	_collidable = true;
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
void SceneObject::StayOnSreen() {

	SceneManager& sceneManager = SceneManager::GetInstance();
	Camera* camera = sceneManager.GetActiveCamera();
	Vector3 cameraPos = sceneManager.GetActiveCamera()->GetPosition();

	SetPosition(camera->GetTarget() + camera->GetUp() / 2.5 + camera->GetRight() / 1.7);
}

Matrix SceneObject::GetModelMatrix() {

	Matrix positionMat;
	positionMat.SetTranslation(_position);

	Matrix rotationMatOY;
	rotationMatOY.SetRotationY(_rotation.y);
	Matrix rotationMatOX;
	rotationMatOX.SetRotationX(_rotation.x);
	Matrix rotationMatOZ;
	rotationMatOZ.SetRotationZ(_rotation.z);

	Matrix scaleMat;
	scaleMat.SetScale(_scale);

	return scaleMat * rotationMatOX * rotationMatOZ * rotationMatOY * positionMat;
}

void SceneObject::SetUniformsCommon(Camera* camera) {

	SceneManager& sceneManager = SceneManager::GetInstance();
	Shader* currentShader = (_drawWired == true) ? _wiredShader : _shader;
	//mvp + camera
	Matrix model = GetModelMatrix();
	Matrix mvp = model * camera->GetMVP();
	currentShader->SetUniformMatrix4fv("u_mvp", mvp);

	currentShader->SetUniform3f("u_cameraPos", camera->GetPosition());
	currentShader->SetUniformMatrix4fv("u_model", model);

	currentShader->SetUniform1i("u_Texture", 0);

	// skybox
	currentShader->SetUniform1i("u_TextureCube", 1);
	currentShader->SetUniform1f("u_factorTexture", _material->GetFactorTextura());
	currentShader->SetUniform1f("u_factorReflect", _material->GetFactorReflexieTextura());

	//normal mapping
	currentShader->SetUniform1i("u_normalMap", 2);

	//fog
	Fog fog = sceneManager.GetFog();
	currentShader->SetUniform1f("u_fogNear", fog.NearPlane);
	currentShader->SetUniform1f("u_fogFar", fog.FarPlane);
	currentShader->SetUniform3f("u_fogColor", fog.Color);

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
		currentShader->SetUniform3f(lightUniform, it->second->GetDiffuseColor());
		lightUniform = lightUniformBase + "specularColor";
		currentShader->SetUniform3f(lightUniform, it->second->GetSpecularColor());
		lightUniform = lightUniformBase + "type";
		currentShader->SetUniform1i(lightUniform, it->second->GetType());
		lightUniform = lightUniformBase + "position";
		currentShader->SetUniform3f(lightUniform, it->second->GetPosition());

		//add this 2 in xml!!
		lightUniform = lightUniformBase + "direction";
		currentShader->SetUniform3f(lightUniform, it->second->GetPosition());
		lightUniform = lightUniformBase + "spotAngle";
		currentShader->SetUniform1f(lightUniform, 0.7f);

		index++;
	}
	currentShader->SetUniform1i("u_lightsCount", lights.size());
	//ambiental
	currentShader->SetUniform3f("u_ambientColor", sceneManager.GetAmbientalLight().Color);
	currentShader->SetUniform1f("u_ambientRatio", sceneManager.GetAmbientalLight().Ratio);


	//mai jos sunt materiale
	currentShader->SetUniform1f("u_ambientFactor", 0.2);
	currentShader->SetUniform1f("u_specularFactor", 0.8);
	currentShader->SetUniform1f("u_diffuseFactor", 0.5);
}
void SceneObject::SetUniformsParticular(Camera* camera) {


}

void SceneObject::CreateDebugObjects() {

	SceneObject* axisObject = new LocalCoordonatesAxeObject();
	axisObject->SetParent(this);

	_debugObjects.insert({ _debugObjects.size() + 1,axisObject });
}



