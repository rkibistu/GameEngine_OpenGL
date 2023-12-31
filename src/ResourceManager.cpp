#include "stdafx.h"
#include "ResourceManager.h"
#include "Vertex.h"

#include<sstream>

std::string ResourceManager::Elements::ModelsRoot = "models";
std::string ResourceManager::Elements::ShadersRoot = "shaders";
std::string ResourceManager::Elements::TexturesRoot = "textures";
std::string ResourceManager::Elements::MaterialsRoot = "materials";
std::string ResourceManager::Elements::AudioClipsRoot = "audioClips";

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

std::string ResourceManager::Elements::FactorTextura = "factor_textura";
std::string ResourceManager::Elements::FactorReflexieSkyblox = "factor_reflexie_skybox";

std::string ResourceManager::Elements::AudioClip = "audioClip";

ResourceManager* ResourceManager::_spInstance = nullptr;

ResourceManager& ResourceManager::GetInstance() {

	if (_spInstance == nullptr)
		_spInstance = new ResourceManager();
	return *_spInstance;
}
void ResourceManager::DestroyInstance() {

	// VErifica aici daca e okay sa dai doar clear, fara sa apelzi delete pt fiecare


	Clear(_modelResources);
	Clear(_shaderResources);
	Clear(_textureResources);
	Clear(_materialResources); 

	Clear(_models);
	Clear(_textures);
	Clear(_shaders);
	Clear(_materials);

	if (_terrainModel) {

		delete _terrainModel->GetModelResource();
		delete _terrainModel;
	}
	if (_axisModel) {
		delete _axisModel->GetModelResource();
		delete _axisModel;
	}
	if (_upLine) {
		delete _upLine->GetModelResource();
		delete _upLine;
	}
	if (_audioController)
		delete _audioController;

	if (_spInstance)
		delete _spInstance;
}

int ResourceManager::Init() {

	_audioController = new AudioController();

	int res;
	std::stringstream buffer;
	std::string content;
	rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
	std::fstream file(_configureFilepath);
	if (!file.is_open()) {
		std::cout << "Configure file '" << _configureFilepath << "' doesn't exist!" << std::endl;
		delete doc;
		return MY_ERROR_CODE;
	}

	buffer << file.rdbuf();
	file.close();

	content = buffer.str();
	doc->parse<0>(&content[0]);
	rapidxml::xml_node<>* pRoot = doc->first_node();


	res = InitModels(pRoot);
	if (res != MY_SUCCES_CODE) {
		delete doc;
		return res;
	}
	res = InitShaders(pRoot);
	if (res != MY_SUCCES_CODE) {
		delete doc;
		return res;
	}
	res = InitTextures(pRoot);
	if (res != MY_SUCCES_CODE) {
		delete doc;
		return res;
	}
	res = InitMaterials(pRoot);
	if (res != MY_SUCCES_CODE) {
		delete doc;
		return res;
	}
	res = InitAudioClips(pRoot);
	if (res != MY_SUCCES_CODE) {
		delete doc;
		return res;
	}

	delete doc;
	return MY_SUCCES_CODE;
}

int ResourceManager::InitModels(rapidxml::xml_node<>* pRoot) {

	if (pRoot == nullptr) {
		std::cout << "pRoot for xml parser is nullptr" << std::endl;
		return MY_ERROR_CODE;
	}

	std::string path;
	rapidxml::xml_node<>* pNode = pRoot->first_node(ResourceManager::Elements::ModelsRoot.c_str());
	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (pAttr->name() != Elements::Path) {
				std::cout << "Wrong path attribute!" << std::endl;
				return MY_ERROR_CODE;
			}
			path = pAttr->value();
		}

		//get in every model having this path
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			ModelResource* tempModel = new ModelResource();
			tempModel->Path = path;

			//get the IDs
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (pAttr->name() != Elements::Id) {
					std::cout << "Wrong id attribute!" << std::endl;
					return MY_ERROR_CODE;
				}
				tempModel->ID = atoi(pAttr->value());
			}

			// get in every property
			for (rapidxml::xml_node<>* fileNode = modelNode->first_node(); fileNode; fileNode = fileNode->next_sibling())
			{
				//get the files
				if (fileNode->name() != Elements::Filename) {
					std::cout << "Wrong node, expecting file!" << std::endl;
					return MY_ERROR_CODE;
				}
				tempModel->Filename = fileNode->value();
			}

			_modelResources.insert({ tempModel->ID, tempModel });
		}
	}

	return MY_SUCCES_CODE;
}
int ResourceManager::InitShaders(rapidxml::xml_node<>* pRoot) {

	if (pRoot == nullptr) {
		std::cout << "pRoot for xml parser is nullptr" << std::endl;;
		return MY_ERROR_CODE;
	}

	std::string path;
	rapidxml::xml_node<>* pNode = pRoot->first_node(ResourceManager::Elements::ShadersRoot.c_str());

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
				return MY_ERROR_CODE;
			}
			path = pAttr->value();
		}

		//get in every shader
		for (rapidxml::xml_node<>* modelNode = resourceNode->first_node(); modelNode; modelNode = modelNode->next_sibling())
		{
			ShaderResource* tempShader = new ShaderResource();
			tempShader->Path = path;

			//get the IDs
			for (rapidxml::xml_attribute<>* pAttr = modelNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (pAttr->name() != Elements::Id) {
					std::cout << "Wrong id attribute!" << std::endl;
					return MY_ERROR_CODE;
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
					return MY_ERROR_CODE;
				}
			}

			_shaderResources.insert({ tempShader->ID,tempShader });
		}
	}

	return MY_SUCCES_CODE;
}
int ResourceManager::InitTextures(rapidxml::xml_node<>* pRoot) {

	if (pRoot == nullptr) {
		std::cout << "pRoot for xml parser is nullptr" << std::endl;;
		return MY_ERROR_CODE;
	}

	std::string path;

	rapidxml::xml_node<>* pNode = pRoot->first_node(ResourceManager::Elements::TexturesRoot.c_str());

	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (pAttr->name() != Elements::Path) {
				std::cout << "Wrong path attribute!" << std::endl;
				return MY_ERROR_CODE;
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
					tempTexture->Type = pAttr->value();
				}
				else {
					std::cout << "Wrong texture attribute, expected if or type!" << std::endl;
					return MY_ERROR_CODE;
				}

			}

			// get in every node
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
					return MY_ERROR_CODE;
				}
			}

			_textureResources.insert({ tempTexture->ID,tempTexture });
		}
	}

	return MY_SUCCES_CODE;
}
int ResourceManager::InitMaterials(rapidxml::xml_node<>* pRoot) {

	if (pRoot == nullptr) {
		std::cout << "pRoot for xml parser is nullptr" << std::endl;;
		return MY_ERROR_CODE;
	}

	std::string path;

	rapidxml::xml_node<>* pNode = pRoot->first_node(ResourceManager::Elements::MaterialsRoot.c_str());

	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (pAttr->name() != Elements::Path) {
				std::cout << "Wrong path attribute!" << std::endl;
				return MY_ERROR_CODE;
			}
			path = pAttr->value();

		}

		// get in every texture
		for (rapidxml::xml_node<>* materialNode = resourceNode->first_node(); materialNode; materialNode = materialNode->next_sibling())
		{
			MaterialResource* tempResource = new MaterialResource();
			tempResource->Path = path;

			//get the IDs
			for (rapidxml::xml_attribute<>* pAttr = materialNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (pAttr->name() == Elements::Id) {
					tempResource->ID = atoi(pAttr->value());
				}
				else if (pAttr->name() == Elements::TextureType) {
					tempResource->Type = pAttr->value();
				}
				else {
					std::cout << "Wrong texture attribute, expected if or type!" << std::endl;
					return MY_ERROR_CODE;
				}

			}

			// get in every node
			for (rapidxml::xml_node<>* node = materialNode->first_node(); node; node = node->next_sibling())
			{
				//get the IDs
				if (node->name() == Elements::FactorTextura) {

					tempResource->FactorTextura = node->value();
				}
				else if (node->name() == Elements::FactorReflexieSkyblox) {

					tempResource->FactorReflexieSkybox = node->value();
				}
			}

			_materialResources.insert({tempResource->ID,tempResource});
		}

	}
	return MY_SUCCES_CODE;

}
int ResourceManager::InitAudioClips(rapidxml::xml_node<>* pRoot) {

	if (pRoot == nullptr) {
		std::cout << "pRoot for xml parser is nullptr" << std::endl;;
		return MY_ERROR_CODE;
	}



	std::string path;

	rapidxml::xml_node<>* pNode = pRoot->first_node(ResourceManager::Elements::AudioClipsRoot.c_str());

	//get in every <folder>
	for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
	{
		//get the paths
		for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (pAttr->name() != Elements::Path) {
				std::cout << "Wrong path attribute!" << std::endl;
				return MY_ERROR_CODE;
			}
			path = pAttr->value();
		}

		// get in every audioclip
		for (rapidxml::xml_node<>* audioClipNode = resourceNode->first_node(); audioClipNode; audioClipNode = audioClipNode->next_sibling())
		{
			unsigned int id;
			std::string filename;

			//get the IDs
			for (rapidxml::xml_attribute<>* pAttr = audioClipNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (pAttr->name() == Elements::Id) {
					id = atoi(pAttr->value());
				}
				else {
					std::cout << "Wrong audio clip attribute!" << std::endl;
					return MY_ERROR_CODE;
				}

			}

			//get the filenames
			if (audioClipNode->name() == Elements::AudioClip) {

				filename = audioClipNode->value();
			}

			

			_audioController->AddSound(id, path + filename);
		}

	}

	return MY_SUCCES_CODE;

}

Model* ResourceManager::GetModel(unsigned int id) {

	auto itModels = _models.find(id);
	if (itModels != _models.end()) {
		return itModels->second;
	}

	return LoadModel(id);
}
Shader* ResourceManager::GetShader(unsigned int id) {

	auto itModels = _shaders.find(id);
	if (itModels != _shaders.end()) {
		return itModels->second;
	}

	return LoadShader(id);
}
Texture* ResourceManager::GetTexture(unsigned int id) {

	auto itModels = _textures.find(id);
	if (itModels != _textures.end()) {
		return itModels->second;
	}

	return LoadTexture(id);
}
Material* ResourceManager::GetMaterial(unsigned int id) {

	auto itMaterials = _materials.find(id);
	if (itMaterials != _materials.end()) {
		return itMaterials->second;
	}

	return LoadMaterial(id);
}

Model* ResourceManager::GetTerrainModel() {

	return _terrainModel;
}
Model* ResourceManager::GetTerrainModel(int sizeWidht, int sizeHeight, int cellCountWidth, int cellCountHeight) {

	if (_terrainModel == nullptr) {

		_terrainModel = new Model();
		_terrainModel->LoadFlatTerrain(sizeWidht, sizeHeight, cellCountWidth, cellCountHeight);
	}
	return _terrainModel;
}

Model* ResourceManager::GetSystemAxisModel() {

	if (_axisModel == nullptr) {
		_axisModel = new Model();
		_axisModel->LoadSystemAxis();
	}
	return _axisModel;
}
Model* ResourceManager::GetLineUpModel() {

	
	if (_upLine == nullptr) {
		_upLine = new Model();
		_upLine->LoadLineUp(Vector3(0.0f,1.0f,0.0f),0.15f);
	}
	return _upLine;
}

Model* ResourceManager::LoadModel(unsigned int id) {

	auto itResources = _modelResources.find(id);
	if (itResources == _modelResources.end()) {
		std::cout << "Resursa pentru modelul cu ID-ul " << id << " nu exista!" << std::endl;
		return nullptr;
	}

	Model* model = new Model();
	model->Load(itResources->second);

	_models.insert({ id,model });
	return model;
}
Shader* ResourceManager::LoadShader(unsigned int id) {

	auto itResources = _shaderResources.find(id);
	if (itResources == _shaderResources.end()) {
		std::cout << "Resursa pentru shader-ul cu ID-ul " << id << " nu exista!" << std::endl;
		return nullptr;
	}

	Shader* shader = new Shader();
	shader->Load(itResources->second);

	_shaders.insert({ id,shader });
	return shader;
}
Texture* ResourceManager::LoadTexture(unsigned int id) {

	auto itResources = _textureResources.find(id);
	if (itResources == _textureResources.end()) {
		std::cout << "Resursa pentru textura cu ID-ul " << id << " nu exista!" << std::endl;
		return nullptr;
	}

	Texture* texture = new Texture();
	texture->Load(itResources->second);

	_textures.insert({ id,texture });
	return texture;
}
Material* ResourceManager::LoadMaterial(unsigned int id) {

	auto itResources = _materialResources.find(id);
	if (itResources == _materialResources.end()) {
		std::cout << "Resursa pentru materialul cu ID-ul " << id << " nu exista!" << std::endl;
		return nullptr;
	}

	Material* material = new Material();
	material->Load(itResources->second);

	_materials.insert({ id,material });
	return material;
}


