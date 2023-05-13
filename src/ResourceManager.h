#pragma once

#include <string>
#include <fstream>
#include <rapidXML/rapidxml.hpp>
#include <unordered_map>

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Defines.h"

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

	int Init();

	Model* GetModel(unsigned int id);
	Shader* GetShader(unsigned int id);
	Texture* GetTexture(unsigned int id);

	//return terrain if exists or create one with given dimension
	Model* GetTerrainModel(int sizeWidht, int sizeHeight, int cellCountWidth, int cellCountHeight);
	
	//return nullptr if terrain is not loaded
	Model* GetTerrainModel();

private:

	//read them from xml
	int InitModels(rapidxml::xml_node<>* pRoot);
	int InitShaders(rapidxml::xml_node<>* pRoot);
	int InitTextures(rapidxml::xml_node<>* pRoot);

	Model* LoadModel(unsigned int id);
	Shader* LoadShader(unsigned int id);
	Texture* LoadTexture(unsigned int id);

	template <typename T>
	void Clear(std::unordered_map<unsigned int, T>& unorderedMap) {

		for (auto it = unorderedMap.begin(); it != unorderedMap.end(); ++it) {
			delete it->second;
		}
		unorderedMap.clear();
	}

private:

	static ResourceManager* _spInstance;
	ResourceManager() { ; }
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator= (const ResourceManager&) = delete;

	std::string _configureFilepath = "Resources/XMLs/resourceManager.xml";

	

	std::unordered_map<unsigned int, ModelResource*> _modelResources;
	std::unordered_map<unsigned int, TextureResource*> _textureResources;
	std::unordered_map<unsigned int, ShaderResource*> _shaderResources;

	std::unordered_map<unsigned int, Model*> _models;
	std::unordered_map<unsigned int, Shader*> _shaders;
	std::unordered_map<unsigned int, Texture*> _textures;

	Model* _terrainModel = nullptr;
public:
	Vector2 _terrainSize;
	Vector2 _terrainCountCells;
};


