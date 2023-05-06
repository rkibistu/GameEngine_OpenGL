#include "stdafx.h"
#include "ResourceManager.h"


#include<sstream>

std::string ResourceManager::Elements::ModelsRoot = "models";
std::string ResourceManager::Elements::ShadersRoot = "shaders";
std::string ResourceManager::Elements::TexturesRoot = "textures";
std::string ResourceManager::Elements::Filename = "file";
std::string ResourceManager::Elements::VertexShaderFilename = "vs";
std::string ResourceManager::Elements::FragmentShaderFilename = "fs";
std::string ResourceManager::Elements::MinFilterMode = "min_filter";
std::string ResourceManager::Elements::MagFilterMode = "mag_filter";
std::string ResourceManager::Elements::WrapS = "wrap_s";
std::string ResourceManager::Elements::WrapT = "wrap_t";

void ResourceManager::LoadResources() {

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

void ResourceManager::LoadAll() {


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
			//AIci luam atributele. exemlpu: <obiect id="100" nume="zar">  atributele: nume 'id', value '100' si nume 'nume', value 'zar'
			std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
		}

		//get in every model
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			//get the IDs
			std::cout << "Node: " << modelNode->name() << " " << modelNode->value() << std::endl;
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				//AIci luam atributele. exemlpu: <obiect id="100" nume="zar">  atributele: nume 'id', value '100' si nume 'nume', value 'zar'
				std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
			}

			// get in every property
			for (rapidxml::xml_node<>* fileNode = modelNode->first_node(); fileNode; fileNode = fileNode->next_sibling())
			{
				//get the IDs
				std::cout << "Node: " << fileNode->name() << " " << fileNode->value() << std::endl;
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;

}

void ResourceManager::LoadShaders() {

	rapidxml::xml_node<>* pNode = _pRoot->first_node(ResourceManager::Elements::ShadersRoot.c_str());

	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			//AIci luam atributele. exemlpu: <obiect id="100" nume="zar">  atributele: nume 'id', value '100' si nume 'nume', value 'zar'
			std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
		}

		//get in every model
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			//get the IDs
			std::cout << "Node: " << modelNode->name() << " " << modelNode->value() << std::endl;
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
			}

			// get in every property
			for (rapidxml::xml_node<>* fileNode = modelNode->first_node(); fileNode; fileNode = fileNode->next_sibling())
			{
				//get the IDs
				std::cout << "Node: " << fileNode->name() << " " << fileNode->value() << std::endl;
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void ResourceManager::LoadTextures() {

	rapidxml::xml_node<>* pNode = _pRoot->first_node(ResourceManager::Elements::TexturesRoot.c_str());

	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			//AIci luam atributele. exemlpu: <obiect id="100" nume="zar">  atributele: nume 'id', value '100' si nume 'nume', value 'zar'
			std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
		}

		//get in every model
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			//get the IDs
			std::cout << "Node: " << modelNode->name() << " " << modelNode->value() << std::endl;
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
			}

			// get in every property
			for (rapidxml::xml_node<>* fileNode = modelNode->first_node(); fileNode; fileNode = fileNode->next_sibling())
			{
				//get the IDs
				std::cout << "Node: " << fileNode->name() << " " << fileNode->value() << std::endl;
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}