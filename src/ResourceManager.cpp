#include "stdafx.h"
#include "ResourceManager.h"
#include "Vertex.h"

#include<sstream>

std::string ResourceManager::Elements::ModelsRoot = "models";
std::string ResourceManager::Elements::ShadersRoot = "shaders";
std::string ResourceManager::Elements::TexturesRoot = "textures";
std::string ResourceManager::Elements::Filename = "file";
std::string ResourceManager::Elements::Path = "path";
std::string ResourceManager::Elements::Id = "id";
std::string ResourceManager::Elements::VertexShaderFilename = "vs";
std::string ResourceManager::Elements::FragmentShaderFilename = "fs";
std::string ResourceManager::Elements::TextureType = "type";
std::string ResourceManager::Elements::MinFilterMode = "min_filter";
std::string ResourceManager::Elements::MagFilterMode = "mag_filter";
std::string ResourceManager::Elements::WrapS = "wrap_s";
std::string ResourceManager::Elements::WrapT = "wrap_t";

ResourceManager* ResourceManager::_spInstance = nullptr;

ResourceManager& ResourceManager::GetInstance() {

	if (_spInstance == nullptr)
		_spInstance = new ResourceManager();
	return *_spInstance;
}
void ResourceManager::DestroyInstance() {

	 // VErifica aici daca e okay sa dai doar clear, fara sa apelzi delete pt fiecare

	_models.clear();
	_shaders.clear();
	_textures.clear();

	if (_spInstance)
		delete _spInstance;
}

void ResourceManager::Init() {

	std::stringstream buffer;
	std::string content;
	rapidxml::xml_document<> doc;
	_configureFile.open(_configureFilepath);
	if (!_configureFile.is_open()) {
		std::cout << "Configure file '" << _configureFilepath << "' doesn't exist!" << std::endl;
		return;
	}

	buffer << _configureFile.rdbuf();
	_configureFile.close();

	content = buffer.str();
	doc.parse<0>(&content[0]);
	_pRoot = doc.first_node();


	LoadModels();
	LoadShaders();
	LoadTextures();
}

void ResourceManager::LoadModels() {

	std::string path;
	rapidxml::xml_node<>* pNode = _pRoot->first_node(ResourceManager::Elements::ModelsRoot.c_str());
	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (pAttr->name() != Elements::Path) {
				std::cout << "Wrong path attribute!" << std::endl;
				return;
			}
			path = pAttr->value();
		}

		//get in every model having this path
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			ModelResource* tempModel = new ModelResource();
			tempModel->Path = path;

			std::cout << "Node: " << modelNode->name() << " " << modelNode->value() << std::endl;

			//get the IDs
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (pAttr->name() != Elements::Id) {
					std::cout << "Wrong id attribute!" << std::endl;
					return;
				}
				tempModel->ID = atoi(pAttr->value());
			}

			// get in every property
			for (rapidxml::xml_node<>* fileNode = modelNode->first_node(); fileNode; fileNode = fileNode->next_sibling())
			{
				//get the files
				if (fileNode->name() != Elements::Filename) {
					std::cout << "Wrong node, expecting file!" << std::endl;
					return;
				}
				tempModel->Filename = fileNode->value();
			}
			
			_models.push_back(tempModel);
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;

}

void ResourceManager::LoadShaders() {

	std::string path;
	rapidxml::xml_node<>* pNode = _pRoot->first_node(ResourceManager::Elements::ShadersRoot.c_str());

	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			//AIci luam atributele. exemlpu: <obiect id="100" nume="zar">  atributele: nume 'id', value '100' si nume 'nume', value 'zar'
			//std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
			if (pAttr->name() != Elements::Path) {
				std::cout << "Wrong path attribute!" << std::endl;
				return;
			}
			path = pAttr->value();
		}

		//get in every shader
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			ShaderResources* tempShader = new ShaderResources();
			tempShader->Path = path;

			//get the IDs
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (pAttr->name() != Elements::Id) {
					std::cout << "Wrong id attribute!" << std::endl;
					return;
				}
				tempShader->ID = atoi(pAttr->value());
			}


			// get in every property
			for (rapidxml::xml_node<>* fileNode = modelNode->first_node(); fileNode; fileNode = fileNode->next_sibling())
			{
				//get the IDs
				if (fileNode->name() == Elements::VertexShaderFilename) {

					tempShader->VertexShaderFilename = fileNode->value();
				}
				else if (fileNode->name() == Elements::FragmentShaderFilename) {

					tempShader->FragmentShaderFilename = fileNode->value();
				}
				else {
					std::cout << "Wrong node, expected VS or FS" << std::endl;
				}
			}

			_shaders.push_back(tempShader);
		}
	}
}

void ResourceManager::LoadTextures() {

	std::string path;

	rapidxml::xml_node<>* pNode = _pRoot->first_node(ResourceManager::Elements::TexturesRoot.c_str());

	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (pAttr->name() != Elements::Path) {
				std::cout << "Wrong path attribute!" << std::endl;
				return;
			}
			path = pAttr->value();
		}

		//get in every texture 
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			TextureResource* tempTexture = new TextureResource();
			tempTexture->Path = path;

			//get the IDs
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (pAttr->name() == Elements::Id) {
					tempTexture->ID = atoi(pAttr->value());
				}
				else if (pAttr->name() == Elements::TextureType) {
					tempTexture->Type == pAttr->value();
				}
				else {
					std::cout << "Wrong texture attribute, expected if or type!" << std::endl;
					return;
				}
				
			}

			// get in every property
			for (rapidxml::xml_node<>* fileNode = modelNode->first_node(); fileNode; fileNode = fileNode->next_sibling())
			{
				//get the IDs
				if (fileNode->name() == Elements::Filename) {

					tempTexture->Filename = fileNode->value();
				}
				else if (fileNode->name() == Elements::MinFilterMode) {

					tempTexture->MinFilter = fileNode->value();
				}
				else if (fileNode->name() == Elements::MagFilterMode) {

					tempTexture->MagFilter = fileNode->value();
				}
				else if (fileNode->name() == Elements::WrapS) {

					tempTexture->WrapS = fileNode->value();
				}
				else if (fileNode->name() == Elements::WrapT) {

					tempTexture->WrapT = fileNode->value();
				}
				else {
					std::cout << "Wrong node, expected file, min_filter, mag_filter, wrap_s, wrap_t" << std::endl;
				}
			}

			_textures.push_back(tempTexture);
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}