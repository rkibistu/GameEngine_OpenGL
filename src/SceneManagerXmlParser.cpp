#include "stdafx.h"
#include "SceneManagerXmlParser.h"
#include "ResourceManager.h"
#include "Skybox.h"
#include "Fire.h"
#include "Defines.h"

#include <fstream>
#include <sstream>
#include <vector>

#define DEFAULT_MATERIAL_ID 300



int SceneManagerXmlParser::Init(std::string filepath) {

	int res;
	std::stringstream buffer;
	_doc = new rapidxml::xml_document<>();
	std::fstream file(filepath);
	if (!file.is_open()) {
		std::cout << "Configure file '" << filepath << "' doesn't exist!" << std::endl;
		return MY_ERROR_CODE;
	}

	buffer << file.rdbuf();
	file.close();

	_content = buffer.str();
	_doc->parse<0>(&_content[0]);


	_xmlRoot = _doc->first_node();
	_objectsRoot = _xmlRoot->first_node(OBJECTS_ROOT);
	_lightsRoot = _xmlRoot->first_node(LIGHTS_ROOT);
	_controlsRoot = _xmlRoot->first_node(CONTROLS_ROOT);
	_camerasRoot = _xmlRoot->first_node(CAMERAS_ROOT);
	_activeCameraNode = _xmlRoot->first_node(ACTIVE_CAMERA_NODE);
}
void SceneManagerXmlParser::Destroy() {

	delete _doc;
}

//citest toate obiectele din cml aflate dub nodul <objects>
//	creeaza un SceneObject pt fiecare <object> si il adauga in mapa data ca parametru
int SceneManagerXmlParser::ReadObjects(std::map<unsigned int, SceneObject*>& sceneObjects) {

	for (rapidxml::xml_node<>* objectNode = _objectsRoot->first_node(); objectNode; objectNode = objectNode->next_sibling()) {
		if (strcmp(objectNode->name(), COMMENT_NODE) == 0)
			continue;

		SceneObject* sceneObject = ReadSceneObject(objectNode);
		if (sceneObject != nullptr)
			sceneObjects.insert({ sceneObject->GetId() ,sceneObject });
	}

	return MY_SUCCES_CODE;
}

// citeste toate nodurile light de sub nodul lights
int SceneManagerXmlParser::ReadLights(std::unordered_map<unsigned int, Light*>& lightObjects) {

	for (rapidxml::xml_node<>* objectNode = _lightsRoot->first_node(); objectNode; objectNode = objectNode->next_sibling()) {
		if (strcmp(objectNode->name(), COMMENT_NODE) == 0)
			continue;

		Light* lightObject = ReadLightObject(objectNode);
		if (lightObject != nullptr)
			lightObjects.insert({ lightObject->GetId() ,lightObject });
	}

	return MY_SUCCES_CODE;
}


//citeste toate camerer din xml aflate sub nodul <cameras>
//	creaza o clasa Camera pt fiecare <camera> si populeaza mapa din parametru cameras
//	seteaa activeCamera sa pointeze spre camera setata in xml <activeCamera>
int SceneManagerXmlParser::ReadCameras(std::unordered_map<unsigned int, Camera*>& cameras, Camera** activeCamera) {

	unsigned int activeCameraID;
	bool activeCameraFound = false;

	if (strcmp(_activeCameraNode->name(), ACTIVE_CAMERA_NODE) != 0) {
		std::cout << "MISSING ACTIVE CAMERA FROM XML FILE!" << std::endl;
		return MY_ERROR_CODE;
	}

	activeCameraID = atoi(_activeCameraNode->value());
	for (rapidxml::xml_node<>* cameraNode = _camerasRoot->first_node(); cameraNode; cameraNode = cameraNode->next_sibling()) {
		if (strcmp(cameraNode->name(), COMMENT_NODE) == 0)
			continue;

		Camera* camera = ReadCamera(cameraNode);
		if (camera != nullptr) {

			cameras.insert({ camera->GetId(), camera });
			if (activeCameraID == camera->GetId()) {
				activeCameraFound = true;
				*activeCamera = camera;
			}
		}
	}

	if (activeCameraFound == false) {
		std::cout << "ACTIVE CAMERA HAS BAD ID!" << std::endl;
		return MY_ERROR_CODE;
	}

	return MY_SUCCES_CODE;
}

//Citeste toate controale de sub nodul <controls>
//	Actualizeaza direct valorile corespunzatoare axelor de miscare din clasa Input
int SceneManagerXmlParser::ReadControls() {

	for (rapidxml::xml_node<>* controlNode = _controlsRoot->first_node(); controlNode; controlNode = controlNode->next_sibling()) {
		if (strcmp(controlNode->name(), COMMENT_NODE) == 0)
			continue;

		unsigned char currentKey;
		for (rapidxml::xml_node<>* node = controlNode->first_node(); node; node = node->next_sibling()) {

			if (strcmp(node->name(), KEY_NODE) == 0) {
				currentKey = node->value()[0];
			}
			if (strcmp(node->name(), ACTION_NODE) == 0) {

				if (strcmp(node->value(), "MOVE_CAMERA_POSITIVE_Z") == 0) {

					Input::SetMovementAxisPozitiveKey("Depth", currentKey);
				}
				if (strcmp(node->value(), "MOVE_CAMERA_NEGATIVE_Z") == 0) {

					Input::SetMovementAxisNegativeKey("Depth", currentKey);
				}

				if (strcmp(node->value(), "MOVE_CAMERA_POSITIVE_X") == 0) {

					Input::SetMovementAxisPozitiveKey("Horizontal", currentKey);
				}
				if (strcmp(node->value(), "MOVE_CAMERA_NEGATIVE_X") == 0) {

					Input::SetMovementAxisNegativeKey("Horizontal", currentKey);
				}


				if (strcmp(node->value(), "MOVE_CAMERA_POSITIVE_Y") == 0) {

					Input::SetMovementAxisPozitiveKey("Vertical", currentKey);
				}
				if (strcmp(node->value(), "MOVE_CAMERA_NEGATIVE_Y") == 0) {

					Input::SetMovementAxisNegativeKey("Vertical", currentKey);
				}
			}
		}
	}

	return MY_SUCCES_CODE;
}

//Citeste nodul <backgroundColor> si pune val in parametru
int SceneManagerXmlParser::ReadBackgroundColor(Vector3& backgroundColor) {

	rapidxml::xml_node<>* backgroundNode = _xmlRoot->first_node(BACKGROUND_COLOR_NODE);
	ReadVector3_rgb(backgroundNode, BACKGROUND_COLOR_NODE, backgroundColor);

	return MY_SUCCES_CODE;
}
int SceneManagerXmlParser::ReadFog(Fog& fog) {

	rapidxml::xml_node<>* fogNode = _xmlRoot->first_node(FOG_ROOT_NODE);
	for (rapidxml::xml_node<>* node = fogNode->first_node(); node; node = node->next_sibling()) {
	
		ReadFloat(node, FOG_NEAR_PLANE, fog.NearPlane);
		ReadFloat(node, FOG_FAR_PLANE, fog.FarPlane);
		ReadVector3_rgb(node, COLOR_NODE, fog.Color);
	}
	return MY_SUCCES_CODE;
}

SceneObject* SceneManagerXmlParser::ReadSceneObject(rapidxml::xml_node<>* objectNode) {


	SceneObjectXmlFormat obj;

	//ia atributele: <object id=1>
	for (rapidxml::xml_attribute<>* pAttr = objectNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
			std::cout << "Expected id attribute" << std::endl;
			return nullptr;
		}
		obj.id = atoi(pAttr->value());
	}

	//treci prin fiecare nod copil: model, textures, shader, etc.
	for (rapidxml::xml_node<>* node = objectNode->first_node(); node; node = node->next_sibling()) {

		ReadString(node, MODEL_NODE, obj.modelId);
		ReadString(node, TYPE_NODE, obj.type);
		ReadInt(node, SHADER_NODE, obj.shaderId);
		ReadTexturesVector(node, TEXTURES_NODE, TEXTURE_NODE, obj.texturesId);
		ReadString(node, MATERIAL_NODE, obj.materialId);
		ReadVector3_xyz(node, POSITION_NODE, obj.position);
		ReadVector3_xyz(node, ROTATION_NODE, obj.rotation);
		ReadVector3_xyz(node, SCALE_NODE, obj.scale);
		ReadVector3_rgb(node, HEIGHTS_PER_COLOR_NODE, obj.heights);
		ReadString(node, NAME_NODE, obj.name);
		ReadFollowingCamera(node, FOLLOWING_CAMERA_NODE, obj.followCameraDirections);
		ReadFloat(node, DISPLACEMENT_MAX_VALUE_NODE, obj.fireDisplMax);
	}

	SceneObject* sceneObject = CreateSceneObject(obj);
	return sceneObject;
}
void SceneManagerXmlParser::ReadModel(rapidxml::xml_node<>* node, SceneObject* sceneObject) {

	if (strcmp(node->name(), MODEL_NODE) == 0) {

		ResourceManager& resourceManager = ResourceManager::GetInstance();

		Model* model;
		model = resourceManager.GetModel(atoi(node->value()));
		if (model == nullptr) {
			std::cout << "Missing model for object in XML file" << std::endl;
		}
		sceneObject->SetModel(model);
	}
}
void SceneManagerXmlParser::ReadShader(rapidxml::xml_node<>* node, SceneObject* sceneObject) {

	if (strcmp(node->name(), SHADER_NODE) == 0) {

		ResourceManager& resourceManager = ResourceManager::GetInstance();

		Shader* shader;
		shader = resourceManager.GetShader(atoi(node->value()));
		if (shader == nullptr) {
			std::cout << "Missing shader for object in XML file" << std::endl;
		}
		sceneObject->SetShader(shader);
	}
}
void SceneManagerXmlParser::ReadTextures(rapidxml::xml_node<>* node, SceneObject* sceneObject) {

	if (strcmp(node->name(), TEXTURES_NODE) == 0) {

		ResourceManager& resourceManager = ResourceManager::GetInstance();

		for (rapidxml::xml_node<>* textureNode = node->first_node(); node; node = node->next_sibling()) {

			Texture* texture;
			rapidxml::xml_attribute<>* pAttr = textureNode->first_attribute();
			if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
				std::cout << "Missing id attribute from texture in xml file" << std::endl;
				return;
			}
			texture = resourceManager.GetTexture(atoi(pAttr->value()));
			sceneObject->AddTexture(texture);
		}
	}
}
void SceneManagerXmlParser::ReadTexturesVector(rapidxml::xml_node<>* node, std::string rootNodeName, std::string nodeName, std::vector<int>& result) {

	int element;
	if (strcmp(node->name(), rootNodeName.c_str()) == 0) {

		for (rapidxml::xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {

			rapidxml::xml_attribute<>* pAttr = childNode->first_attribute();
			if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
				std::cout << "Missing id attribute from texture in xml file" << std::endl;
				return;
			}
			element = atoi(pAttr->value());
			result.push_back(element);
		}
	}
}
void SceneManagerXmlParser::ReadFollowingCamera(rapidxml::xml_node<>* node, std::string rootNodeName, Vector3& directions) {

	if (strcmp(node->name(), rootNodeName.c_str()) == 0) {

		for (rapidxml::xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {

			if (strcmp(childNode->name(), OX_AXIS_NODE) == 0) {
				directions.x = 1;
			}
			if (strcmp(childNode->name(), OY_AXIS_NODE) == 0) {
				directions.y = 1;
			}
			if (strcmp(childNode->name(), OZ_AXIS_NODE) == 0) {
				directions.z = 1;
			}
		}

	}
}

Light* SceneManagerXmlParser::ReadLightObject(rapidxml::xml_node<>* objectNode) {

	LightObjectXmlFormat obj;

	//ia atributele: <object id=1>
	for (rapidxml::xml_attribute<>* pAttr = objectNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
			std::cout << "Expected id attribute" << std::endl;
			return nullptr;
		}
		obj.id = atoi(pAttr->value());
	}

	//treci prin fiecare nod copil: associatedobject, diffusecolor, specularcolor, etc
	int associatedObjId;
	for (rapidxml::xml_node<>* node = objectNode->first_node(); node; node = node->next_sibling()) {

		if (ReadInt(node, ASSOCIATED_OBJECTED_NODE, associatedObjId)) {
			obj.associatedObjects.push_back(associatedObjId);
		}
		ReadVector3_rgb(node, DIFFUSE_COLOR_NODE, obj.diffuseColor);
		ReadVector3_rgb(node, SPECULAR_COLOR_NODE, obj.specularColor);
		ReadString(node, TYPE_NODE, obj.type);
	}

	Light* lightObject = CreateLightObject(obj);
	return lightObject;
}

Camera* SceneManagerXmlParser::ReadCamera(rapidxml::xml_node<>* ccameraNode) {

	Camera* camera = new Camera();
	for (rapidxml::xml_attribute<>* pAttr = ccameraNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), ID_ATTRIBUTE) != 0) {
			std::cout << "Expected id attribute" << std::endl;
			delete camera;
			return nullptr;
		}
		camera->SetId(atoi(pAttr->value()));
	}


	Vector3 position, target, up;
	float fov, nearClip, farClip;
	float translationSpeed, rotationSpeed;
	for (rapidxml::xml_node<>* node = ccameraNode->first_node(); node; node = node->next_sibling()) {
		if (strcmp(node->name(), COMMENT_NODE) == 0)
			continue;

		ReadVector3_xyz(node, POSITION_NODE, position);
		ReadVector3_xyz(node, TARGET_NODE, target);
		ReadVector3_xyz(node, UP_NODE, up);

		ReadFloat(node, FOV_NODE, fov);
		ReadFloat(node, NEAR_CLIP_NODE, nearClip);
		ReadFloat(node, FAR_CLIP_NODE, farClip);

		ReadFloat(node, TRANSLATION_SPEED_NODE, translationSpeed);
		ReadFloat(node, ROTATION_SPEED_NODE, rotationSpeed);
	}

	camera->Init(position, target, up, fov, nearClip, farClip);
	camera->SetMoveSpeed(translationSpeed);
	camera->SetRotationSpeed(rotationSpeed);
	return camera;
}
bool SceneManagerXmlParser::ReadVector3_xyz(rapidxml::xml_node<>* node, std::string nodeName, Vector3& result) {

	if (strcmp(node->name(), nodeName.c_str()) == 0) {

		for (rapidxml::xml_node<>* coordonateNode = node->first_node(); coordonateNode; coordonateNode = coordonateNode->next_sibling()) {

			if (strcmp(coordonateNode->name(), X_AX_NODE) == 0)
				result.x = atof(coordonateNode->value());
			if (strcmp(coordonateNode->name(), Y_AX_NODE) == 0)
				result.y = atof(coordonateNode->value());
			if (strcmp(coordonateNode->name(), Z_AX_NODE) == 0)
				result.z = atof(coordonateNode->value());
		}
		return true;
	}
	return false;
}
bool SceneManagerXmlParser::ReadVector3_rgb(rapidxml::xml_node<>* node, std::string nodeName, Vector3& result) {

	if (strcmp(node->name(), nodeName.c_str()) == 0) {

		for (rapidxml::xml_node<>* coordonateNode = node->first_node(); coordonateNode; coordonateNode = coordonateNode->next_sibling()) {

			if (strcmp(coordonateNode->name(), R_COLOR_NODE) == 0)
				result.x = atof(coordonateNode->value());
			if (strcmp(coordonateNode->name(), G_COLOR_NODE) == 0)
				result.y = atof(coordonateNode->value());
			if (strcmp(coordonateNode->name(), B_COLOR_NODE) == 0)
				result.z = atof(coordonateNode->value());
		}
		return true;
	}
	return false;
}
bool SceneManagerXmlParser::ReadFloat(rapidxml::xml_node<>* node, std::string nodeName, float& result) {

	if (strcmp(node->name(), nodeName.c_str()) == 0) {

		result = atof(node->value());
		return true;
	}
	return false;
}
bool SceneManagerXmlParser::ReadString(rapidxml::xml_node<>* node, std::string nodeName, std::string& result) {

	if (strcmp(node->name(), nodeName.c_str()) == 0) {

		result = node->value();
		return true;
	}
	return false;
}
bool SceneManagerXmlParser::ReadInt(rapidxml::xml_node<>* node, std::string nodeName, int& result) {

	if (strcmp(node->name(), nodeName.c_str()) == 0) {

		result = atoi(node->value());
		return true;
	}
	return false;
}


SceneObject* SceneManagerXmlParser::CreateSceneObject(SceneObjectXmlFormat obj) {

	ResourceManager& resourceManager = ResourceManager::GetInstance();


	SceneObject* sceneObject = nullptr;
	if (obj.modelId == "generated") {
		//specific for generated terrain
		int size = 25000;
		int cells = 250;
		sceneObject = new TerrainObject(size, cells, obj.heights);
		sceneObject->SetModel(resourceManager.GetTerrainModel(size, size, cells, cells));
	}
	else {

		if (obj.type == "skybox") {

			//specific for skybox
			sceneObject = new Skybox();
			sceneObject->SetFollowCameraDirections(obj.followCameraDirections);
			sceneObject->SetFollowCameraOffset(obj.position);
		}
		else if (obj.type == "fire") {

			sceneObject = new Fire(obj.fireDisplMax);
		}
		else {

			//general scene objects
			sceneObject = new SceneObject();
		}
		sceneObject->SetModel(resourceManager.GetModel(atoi(obj.modelId.c_str())));
	}

	sceneObject->SetId(obj.id);
	sceneObject->SetShader(resourceManager.GetShader(obj.shaderId));
	for (auto it = obj.texturesId.begin(); it != obj.texturesId.end(); it++) {

		sceneObject->AddTexture(resourceManager.GetTexture(*it));
	}

	if (obj.materialId.empty()) {

		sceneObject->SetMaterial(resourceManager.GetMaterial(DEFAULT_MATERIAL_ID));
	}
	else {
		sceneObject->SetMaterial(resourceManager.GetMaterial(atoi(obj.materialId.c_str())));
	}

	sceneObject->SetPosition(obj.position);
	sceneObject->SetRotation(obj.rotation);
	sceneObject->SetScale(obj.scale);
	sceneObject->SetName(obj.name);

	return sceneObject;
}
Light* SceneManagerXmlParser::CreateLightObject(LightObjectXmlFormat obj) {

	Light::Type type;
	if (obj.type == "point")
		type = Light::Type::Point;

	Light* lightObject = new Light(type,obj.diffuseColor,obj.specularColor);
	lightObject->SetId(obj.id);
	
	return lightObject;
}



