#pragma once

#include <rapidXML/rapidxml.hpp>

#include "SceneObject.h"

#include <unordered_map>



#define OBJECTS_ROOT "objects"
#define CONTROLS_ROOT "controls"
#define CAMERAS_ROOT "cameras"

#define COMMENT_NODE "comment"
#define OBJECT_NODE "object"

#define MODEL_NODE "model"
#define SHADER_NODE "shader"
#define TYPE_NODE "type"
#define NAME_NODE "name"
#define TEXTURES_NODE "textures"
#define TEXTURE_NODE "texture"
#define POSITION_NODE "position"
#define ROTATION_NODE "rotation"
#define SCALE_NODE "scale"
#define X_AX_NODE "x"
#define Y_AX_NODE "y"
#define Z_AX_NODE "z"

#define ID_ATTRIBUTE "id"

class SceneManagerXmlParser {

public:
	int Init(std::string filepath);

	int ReadObjects(std::unordered_map<unsigned int, SceneObject*>& sceneObjects);

private:

	SceneObject* ReadSceneObject(rapidxml::xml_node<>* objectNode);

	//check if is model_node -> update sceneObject if true
	void ReadModel(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadShader(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadTextures(rapidxml::xml_node<>* node, SceneObject* sceneObject);


private:
	//Pastram pointeri spre locurile importante din XML
	// Ma gandesc ca daca creez un obiect la runtime, vreau sa il scriu in fisier
	std::string _content;
	
	rapidxml::xml_document<>* _doc;
	rapidxml::xml_node<>* _xmlRoot;
	rapidxml::xml_node<>* _objectsRoot;
	rapidxml::xml_node<>* _controlsRoot;
	rapidxml::xml_node<>* _camerasRoot;

	

	std::unordered_map<std::string, int> _objectNodes = {
		{MODEL_NODE,	1},
		{SHADER_NODE,	2},
		{TYPE_NODE,		3},
		{NAME_NODE,		4},
		{TEXTURES_NODE,	5},
		{POSITION_NODE,	6},
		{ROTATION_NODE,	7},
		{SCALE_NODE,	8},
	};
};