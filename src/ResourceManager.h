#include <string>
#include <fstream>
#include <rapidXML/rapidxml.hpp>
class ResourceManager {
public:
	class Elements {
		public:
			static std::string ModelsRoot;
			static std::string ShadersRoot;
			static std::string TexturesRoot;

			static std::string Filename;
			static std::string VertexShaderFilename;
			static std::string FragmentShaderFilename;

			static std::string MinFilterMode;
			static std::string MagFilterMode;
			static std::string WrapS;
			static std::string WrapT;
	};
public:

	void LoadResources();

private:

	void LoadAll();

	void LoadModels();
	void LoadShaders();
	void LoadTextures();
private:
	std::string _configureFilepath = "Resources/XMLs/resourceManager.xml";
	std::fstream _configureFile;

	rapidxml::xml_node<>* _pRoot;
};
