#pragma once

#include <rapidXML/rapidxml.hpp>

#include "SceneObject.h"
#include "TerrainObject.h"
#include "Input.h"

#include <unordered_map>



#define OBJECTS_ROOT "objects"
#define CONTROLS_ROOT "controls"
#define CAMERAS_ROOT "cameras"
#define ACTIVE_CAMERA_NODE "activeCamera"
#define GAME_NAME_NODE "gameName"
#define SCREEN_SIZE_NODE "defaultScreenSize"
#define BACKGROUND_COLOR_NODE "backgroundColor"
#define CONTROLS_ROOT_NODE "controls"

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
#define R_COLOR_NODE "r"
#define G_COLOR_NODE "g"
#define B_COLOR_NODE "b"
#define POSITION_NODE "position"
#define TARGET_NODE "target"
#define UP_NODE "up"
#define TRANSLATION_SPEED_NODE "translationSpeed"
#define ROTATION_SPEED_NODE "rotationSpeed"
#define FOV_NODE "fov"
#define NEAR_CLIP_NODE "near"
#define FAR_CLIP_NODE "far"
#define SCREEN_WIDTH "width"
#define SCREEN_HEIGTH "height"
#define ACTION_NODE "action"
#define KEY_NODE "key"
#define HEIGHTS_PER_COLOR_NODE "heights"

#define ID_ATTRIBUTE "id"



struct SceneObjectXmlFormat;

class SceneManagerXmlParser {

public:
	int Init(std::string filepath);

	int ReadObjects(std::unordered_map<unsigned int, SceneObject*>& sceneObjects);
	int ReadCameras(std::unordered_map<unsigned int, Camera*>& camers, Camera** activeCamera);
	int ReadControls();
	int ReadBackgroundColor(Vector3& backgroundColor);

private:

	SceneObject* ReadSceneObject(rapidxml::xml_node<>* objectNode);
	//check if is model_node -> update sceneObject if true
	void ReadModel(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadShader(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadTextures(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadTexturesVector(rapidxml::xml_node<>* node, std::string rootNodeName, std::string nodeName, std::vector<int>& result);
	
	Camera* ReadCamera(rapidxml::xml_node<>* ccameraNode);

	//functia asta primeste nodul pentru care cuatam copii, numele nodului copil cuatat si un vector3 unde sa puen rezzultatul
	void ReadVector3_xyz(rapidxml::xml_node<>* node, std::string nodeName, Vector3& result);
	void ReadVector3_rgb(rapidxml::xml_node<>* node, std::string nodeName, Vector3& result);
	void ReadFloat(rapidxml::xml_node<>* node, std::string nodeName, float& result);
	void ReadString(rapidxml::xml_node<>* node, std::string nodeName, std::string& result);
	void ReadInt(rapidxml::xml_node<>* node, std::string nodeName, int& result);

	SceneObject* CreateSceneObject(SceneObjectXmlFormat obj);
private:
	//Pastram pointeri spre locurile importante din XML
	// Ma gandesc ca daca creez un obiect la runtime, vreau sa il scriu in fisier
	std::string _content;
	
	rapidxml::xml_document<>* _doc;
	rapidxml::xml_node<>* _xmlRoot;
	rapidxml::xml_node<>* _objectsRoot;
	rapidxml::xml_node<>* _controlsRoot;
	rapidxml::xml_node<>* _camerasRoot;
	rapidxml::xml_node<>* _activeCameraNode;


};

struct SceneObjectXmlFormat {

	int id;
	std::string name;

	std::string modelId;
	int shaderId;
	std::vector<int> texturesId;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 heights;
};