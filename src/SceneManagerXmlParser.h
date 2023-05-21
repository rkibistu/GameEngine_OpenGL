#pragma once

#include <rapidXML/rapidxml.hpp>

#include "SceneObject.h"
#include "TerrainObject.h"
#include "Input.h"
#include "Fog.h"
#include "Light.h"
#include "AmbientalLight.h"

#include <unordered_map>
#include <map>

struct DefaultSettings;

#define OBJECTS_ROOT "objects"
#define CONTROLS_ROOT "controls"
#define CAMERAS_ROOT "cameras"
#define ACTIVE_CAMERA_NODE "activeCamera"
#define GAME_NAME_NODE "gameName"
#define SCREEN_SIZE_NODE "defaultScreenSize"
#define BACKGROUND_COLOR_NODE "backgroundColor"
#define CONTROLS_ROOT_NODE "controls"
#define FOG_ROOT_NODE "fog"
#define LIGHTS_ROOT "lights"
#define DEFAULT_SETTINGS_ROOT "defaultSetting"

#define COMMENT_NODE "comment"
#define OBJECT_NODE "object"

#define MODEL_NODE "model"
#define SHADER_NODE "shader"
#define LIGHT_NODE "light"
#define TYPE_NODE "type"
#define NAME_NODE "name"
#define TEXTURES_NODE "textures"
#define TEXTURE_NODE "texture"
#define MATERIAL_NODE "material"
#define POSITION_NODE "position"
#define ROTATION_NODE "rotation"
#define COLOR_NODE "color"
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
#define FOLLOWING_CAMERA_NODE "followingcamera"
#define OX_AXIS_NODE "ox"
#define OY_AXIS_NODE "oy"
#define OZ_AXIS_NODE "oz"
#define FOG_NEAR_PLANE "near"
#define FOG_FAR_PLANE "far"
#define DISPLACEMENT_MAX_VALUE_NODE "displMax"
#define DIFFUSE_COLOR_NODE "diffuseColor"
#define SPECULAR_COLOR_NODE "specularColor"
#define AMBIENTAL_LIGHT_NODE "ambientalLight"
#define RATIO_NODE "ratio"
#define ASSOCIATED_OBJECTED_NODE "associatedObject"
#define DEBUG_SHADER_NODE "debugShader"
#define DEBUG_LIGHT_MODEL_NODE "lightDebugModel"
#define DEBUG_LIGHT_SHADER_NODE "lightDebugShader"
#define ID_ATTRIBUTE "id"


struct SceneObjectXmlFormat;
struct LightObjectXmlFormat;

class SceneManagerXmlParser {

public:
	int Init(std::string filepath);
	void Destroy();

	int ReadDefaultSettings(DefaultSettings& defaultSettings);
	int ReadObjects(std::map<unsigned int, SceneObject*>& sceneObjects);
	int ReadLights(std::unordered_map<unsigned int, LightObject*>& lightObjects);
	int ReadCameras(std::unordered_map<unsigned int, Camera*>& camers, Camera** activeCamera);
	int ReadControls();
	int ReadBackgroundColor(Vector3& backgroundColor);
	int ReadFog(Fog& fog);
	int ReadAmbientalLight(AmbientalLight& ambientalLight);
private:

	SceneObject* ReadSceneObject(rapidxml::xml_node<>* objectNode);
	//check if is model_node -> update sceneObject if true
	void ReadModel(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadShader(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadTextures(rapidxml::xml_node<>* node, SceneObject* sceneObject);
	void ReadTexturesVector(rapidxml::xml_node<>* node, std::string rootNodeName, std::string nodeName, std::vector<int>& result);
	void ReadFollowingCamera(rapidxml::xml_node<>* node, std::string rootNodeName, Vector3& directions);
	

	LightObject* ReadLightObject(rapidxml::xml_node<>* objectNode);

	Camera* ReadCamera(rapidxml::xml_node<>* ccameraNode);

	//functia asta primeste nodul pentru care cuatam copii, numele nodului copil cuatat si un vector3 unde sa puen rezzultatul
	bool ReadVector3_xyz(rapidxml::xml_node<>* node, std::string nodeName, Vector3& result);
	bool ReadVector3_rgb(rapidxml::xml_node<>* node, std::string nodeName, Vector3& result);
	bool ReadFloat(rapidxml::xml_node<>* node, std::string nodeName, float& result);
	bool ReadString(rapidxml::xml_node<>* node, std::string nodeName, std::string& result);
	bool ReadInt(rapidxml::xml_node<>* node, std::string nodeName, int& result);

	SceneObject* CreateSceneObject(SceneObjectXmlFormat obj);
	LightObject* CreateLightObject(LightObjectXmlFormat obj);
private:
	//Pastram pointeri spre locurile importante din XML
	// Ma gandesc ca daca creez un obiect la runtime, vreau sa il scriu in fisier
	std::string _content;
	
	rapidxml::xml_document<>* _doc;
	rapidxml::xml_node<>* _xmlRoot;
	rapidxml::xml_node<>* _objectsRoot;
	rapidxml::xml_node<>* _lightsRoot;
	rapidxml::xml_node<>* _controlsRoot;
	rapidxml::xml_node<>* _camerasRoot;
	rapidxml::xml_node<>* _activeCameraNode;
};

struct SceneObjectXmlFormat {

	int id;
	std::string name;
	std::string type;
	std::string modelId;
	std::string materialId;
	int shaderId;
	std::vector<int> texturesId;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 heights;
	Vector3 followCameraDirections;

	float fireDisplMax;
};

struct LightObjectXmlFormat {

	int id;
	std::string type;
	Vector3 position;
	Vector3 diffuseColor;
	Vector3 specularColor;
	std::vector<int> associatedObjects;
};