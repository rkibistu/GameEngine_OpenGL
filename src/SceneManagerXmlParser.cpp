#include "stdafx.h"
#include "SceneManagerXmlParser.h"
#include "ResourceManager.h"

#include "Defines.h"

#include <fstream>
#include <sstream>

int SceneManagerXmlParser::Init(std::string filepath) {

	int res;
	std::stringstream buffer;
	_doc = new rapidxml::xml_document<>();
	std::fstream file(filepath);
	if (!file.is_open()) {
		std::cout << "Configure file '" << filepath << "' doesn't exist!" << std::endl;
		return MY_ERROR_CODE;
	}

	buffer << file.rdbuf();
	file.close();

	_content = buffer.str();
	_doc->parse<0>(&_content[0]);


	_xmlRoot = _doc->first_node();
	_objectsRoot = _xmlRoot->first_node(OBJECTS_ROOT);
	_controlsRoot = _xmlRoot->first_node(CONTROLS_ROOT);
	_camerasRoot = _xmlRoot->first_node(CAMERAS_ROOT);
	_activeCameraNode = _xmlRoot->first_node(ACTIVE_CAMERA_NODE);


}

int SceneManagerXmlParser::ReadObjects(std::unordered_map<unsigned int, SceneObject*>& sceneObjects) {

	for (rapidxml::xml_node<>* objectNode = _objectsRoot->first_node(); objectNode; objectNode = objectNode->next_sibling()) {
		if (strcmp(objectNode->name(), COMMENT_NODE) == 0)
			continue;

		SceneObject* sceneObject = ReadSceneObject(objectNode);
		if (sceneObject != nullptr)
			sceneObjects.insert({ sceneObject->GetId() ,sceneObject });
	}

	return MY_SUCCES_CODE;
}

int SceneManagerXmlParser::ReadCameras(std::unordered_map<unsigned int, Camera*>& cameras, Camera** activeCamera) {

	unsigned int activeCameraID;
	bool activeCameraFound = false;

	if (strcmp(_activeCameraNode->name(), ACTIVE_CAMERA_NODE) != 0) {
		std::cout << "MISSING ACTIVE CAMERA FROM XML FILE!" << std::endl;
		return MY_ERROR_CODE;
	}

	activeCameraID = atoi(_activeCameraNode->value());
	for (rapidxml::xml_node<>* cameraNode = _camerasRoot->first_node(); cameraNode; cameraNode = cameraNode->next_sibling()) {
		if (strcmp(cameraNode->name(), COMMENT_NODE) == 0)
			continue;

		Camera* camera = ReadCamera(cameraNode);
		if (camera != nullptr) {

			cameras.insert({ camera->GetId(), camera });
			if (activeCameraID == camera->GetId()) {
				activeCameraFound = true;
				*activeCamera = camera;
			}
		}
	}

	if(activeCameraFound == false) {
		std::cout << "ACTIVE CAMERA HAS BAD ID!" << std::endl;
		return MY_ERROR_CODE;
	}

	return MY_SUCCES_CODE;
}


SceneObject* SceneManagerXmlParser::ReadSceneObject(rapidxml::xml_node<>* objectNode) {

	SceneObject* sceneObject = new SceneObject();

	//ia atributele: <object id=1>
	for (rapidxml::xml_attribute<>* pAttr = objectNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
			std::cout << "Expected id attribute" << std::endl;
			delete sceneObject;
			return nullptr;
		}
		sceneObject->SetId(atoi(pAttr->value()));
	}

	//treci prin fiecare nod copil: model, textures, shader, etc.
	for (rapidxml::xml_node<>* node = objectNode->first_node(); node; node = node->next_sibling()) {

		ReadModel(node, sceneObject);
		ReadShader(node, sceneObject);
		ReadTextures(node, sceneObject);
	}

	return sceneObject;
}
void SceneManagerXmlParser::ReadModel(rapidxml::xml_node<>* node, SceneObject* sceneObject) {

	if (strcmp(node->name(), MODEL_NODE) == 0) {

		ResourceManager& resourceManager = ResourceManager::GetInstance();

		Model* model;
		model = resourceManager.GetModel(atoi(node->value()));
		if (model == nullptr) {
			std::cout << "Missing model for object in XML file" << std::endl;
		}
		sceneObject->SetModel(model);
	}
}
void SceneManagerXmlParser::ReadShader(rapidxml::xml_node<>* node, SceneObject* sceneObject) {

	if (strcmp(node->name(), SHADER_NODE) == 0) {

		ResourceManager& resourceManager = ResourceManager::GetInstance();

		Shader* shader;
		shader = resourceManager.GetShader(atoi(node->value()));
		if (shader == nullptr) {
			std::cout << "Missing shader for object in XML file" << std::endl;
		}
		sceneObject->SetShader(shader);
	}
}
void SceneManagerXmlParser::ReadTextures(rapidxml::xml_node<>* node, SceneObject* sceneObject) {

	if (strcmp(node->name(), TEXTURES_NODE) == 0) {

		ResourceManager& resourceManager = ResourceManager::GetInstance();

		for (rapidxml::xml_node<>* textureNode = node->first_node(); node; node = node->next_sibling()) {

			Texture* texture;
			rapidxml::xml_attribute<>* pAttr = textureNode->first_attribute();
			if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
				std::cout << "Missing id attribute from texture in xml file" << std::endl;
				return;
			}
			texture = resourceManager.GetTexture(atoi(pAttr->value()));
			sceneObject->AddTexture(texture);
		}
	}
}

Camera* SceneManagerXmlParser::ReadCamera(rapidxml::xml_node<>* ccameraNode) {

	Camera* camera = new Camera();
	for (rapidxml::xml_attribute<>* pAttr = ccameraNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
			std::cout << "Expected id attribute" << std::endl;
			delete camera;
			return nullptr;
		}
		camera->SetId(atoi(pAttr->value()));
	}


	Vector3 position, target, up;
	float fov, nearClip, farClip;
	for (rapidxml::xml_node<>* node = ccameraNode->first_node(); node; node = node->next_sibling()) {
		if (strcmp(node->name(), COMMENT_NODE) == 0)
			continue;

		ReadVector3(node, POSITION_NODE, position);
		ReadVector3(node, TARGET_NODE, target);
		ReadVector3(node, UP_NODE, up);

		ReadFloat(node, FOV_NODE, fov);
		ReadFloat(node, NEAR_CLIP_NODE, nearClip);
		ReadFloat(node, FAR_CLIP_NODE, farClip);
	}

	camera->Init(position, target, up, fov, nearClip, farClip);
	return camera;
}
void SceneManagerXmlParser::ReadVector3(rapidxml::xml_node<>* node, std::string nodeName, Vector3& result) {

	if (strcmp(node->name(), nodeName.c_str()) == 0) {

		for (rapidxml::xml_node<>* coordonateNode = node->first_node(); coordonateNode; coordonateNode = coordonateNode->next_sibling()) {

			if (strcmp(coordonateNode->name(), X_AX_NODE) == 0)
				result.x = atof(coordonateNode->value());
			if (strcmp(coordonateNode->name(), Y_AX_NODE) == 0)
				result.y = atof(coordonateNode->value());
			if (strcmp(coordonateNode->name(), Z_AX_NODE) == 0)
				result.z = atof(coordonateNode->value());
		}
	}
}
void SceneManagerXmlParser::ReadFloat(rapidxml::xml_node<>* node, std::string nodeName, float& result) {

	if (strcmp(node->name(), nodeName.c_str()) == 0) {

		result = atof(node->value());
	}
}