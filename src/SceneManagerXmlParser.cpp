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

	
}

int SceneManagerXmlParser::ReadObjects(std::unordered_map<unsigned int, SceneObject*>& sceneObjects) {

	for (rapidxml::xml_node<>* objectNode = _objectsRoot->first_node(); objectNode; objectNode = objectNode->next_sibling()){
		if (strcmp(objectNode->name(), COMMENT_NODE) == 0)
			continue;

		SceneObject* sceneObject = ReadSceneObject(objectNode);
		if (sceneObject != nullptr)
			sceneObjects.insert({ sceneObject->GetId() ,sceneObject});
	}

	return MY_SUCCES_CODE;
}

SceneObject* SceneManagerXmlParser::ReadSceneObject(rapidxml::xml_node<>* objectNode) {

	SceneObject* sceneObject = new SceneObject();
	
	for (rapidxml::xml_attribute<>* pAttr = objectNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
			std::cout << "Expected id attribute" << std::endl;
			delete sceneObject;
			return nullptr;
		}
		sceneObject->SetId(atoi(pAttr->value()));
	}
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
			if (strcmp(pAttr->name(),ID_ATTRIBUTE) != 0) {
				std::cout << "Missing id attribute from texture in xml file" << std::endl;
				return;
			}
			texture = resourceManager.GetTexture(atoi(pAttr->value()));
			sceneObject->AddTexture(texture);
		}
	}
}