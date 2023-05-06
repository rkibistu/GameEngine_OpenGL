#pragma once

#include <string>
#include <fstream>
#include <rapidXML/rapidxml.hpp>
#include <unordered_map>

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"

class ResourceManager {
public:
	class Elements {
	public:
		static std::string ModelsRoot;
		static std::string ShadersRoot;
		static std::string TexturesRoot;

		static std::string Filename;
		static std::string Path;
		static std::string Id;
		static std::string VertexShaderFilename;
		static std::string FragmentShaderFilename;

		static std::string TextureType;
		static std::string MinFilterMode;
		static std::string MagFilterMode;
		static std::string WrapS;
		static std::string WrapT;
	};
public:

	static ResourceManager& GetInstance();
	void DestroyInstance();

	void Init();

	Model* GetModel(unsigned int id);
	Shader* GetShader(unsigned int id);
	Texture* GetTexture(unsigned int id);


private:

	//read them from xml
	void InitModels();
	void InitShaders();
	void InitTextures();

	Model* LoadModel(unsigned int id);
	Shader* LoadShader(unsigned int id);
	Texture* LoadTexture(unsigned int id);

	//inline std::unordered_map<unsigned int, ModelResource*>& Models()  { return _modelResources; }
	//inline std::unordered_map<unsigned int, ShaderResource*>& Shaders() { return _shaderResources; }
	//inline std::unordered_map<unsigned int, TextureResource*>& Textures() { return _textureResources; }
	//
private:

	static ResourceManager* _spInstance;
	ResourceManager() { ; }
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator= (const ResourceManager&) = delete;

	std::string _configureFilepath = "Resources/XMLs/resourceManager.xml";
	std::fstream _configureFile;

	rapidxml::xml_node<>* _pRoot = nullptr;

	std::unordered_map<unsigned int, ModelResource*> _modelResources;
	std::unordered_map<unsigned int, TextureResource*> _textureResources;
	std::unordered_map<unsigned int, ShaderResource*> _shaderResources;

	std::unordered_map<unsigned int, Model*> _models;
	std::unordered_map<unsigned int, Shader*> _shaders;
	std::unordered_map<unsigned int, Texture*> _textures;
};


