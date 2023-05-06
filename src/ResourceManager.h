#pragma once

#include <string>
#include <fstream>
#include <rapidXML/rapidxml.hpp>
#include <vector>

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

	inline std::vector<ModelResource*>& Models()  { return _models; }
	inline std::vector<ShaderResources*>& Shaders() { return _shaders; }
	inline std::vector<TextureResource*>& Textures() { return _textures; }
private:

	void LoadModels();
	void LoadShaders();
	void LoadTextures();
private:

	static ResourceManager* _spInstance;
	ResourceManager() { ; }
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator= (const ResourceManager&) = delete;

	std::string _configureFilepath = "Resources/XMLs/resourceManager.xml";
	std::fstream _configureFile;

	std::vector<ModelResource*> _models;
	std::vector<ShaderResources*> _shaders;
	std::vector<TextureResource*> _textures;

	rapidxml::xml_node<>* _pRoot = nullptr;
};
