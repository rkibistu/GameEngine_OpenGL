#include "stdafx.h"
#include "TargetLineVisual.h"
#include "ResourceManager.h"
#include "SceneManager.h"

TargetLineVisual::TargetLineVisual() {

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	_model = resourceManager.GetLineUpModel();
	_drawWired = true;
	_name = "targetLine";
}

void TargetLineVisual::Update(float deltaTime) {

	SceneManager& sceneManger = SceneManager::GetInstance();

	SetPosition(sceneManger.GetActiveCamera()->GetTarget());
	SetRotation(-sceneManger.GetActiveCamera()->GetRotation());

}

void TargetLineVisual::Draw(Camera* camera) {

	if (_drawWired) {

		DrawDebugWired(camera);
	}
}