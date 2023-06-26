#include "stdafx.h"
#include "SceneManagerXmlParser.h"
#include "ResourceManager.h"
#include "Skybox.h"
#include "Fire.h"
#include "Defines.h"
#include "SceneManager.h"
#include "TrajectoryLineStrip.h"
#include "TrajectoryLineLoop.h"
#include "TrajectoryCircle.h"

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

// read default settings, this are going to be used where is not specified a specific avalue
int SceneManagerXmlParser::ReadDefaultSettings(DefaultSettings& defaultSettings) {

	rapidxml::xml_node<>* settingsRoot = _xmlRoot->first_node(DEFAULT_SETTINGS_ROOT);
	int value;
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	for (rapidxml::xml_node<>* node = settingsRoot->first_node(); node; node = node->next_sibling()) {
		if (strcmp(node->name(), COMMENT_NODE) == 0)
			continue;

		if (ReadInt(node, SHADER_NODE, value)) {
			defaultSettings.DefaultShader = resourceManager.GetShader(value);
		}
		if (ReadInt(node, DEBUG_SHADER_NODE, value)) {
			defaultSettings.WiredShader = resourceManager.GetShader(value);
		}
		if (ReadInt(node, MATERIAL_NODE, value)) {
			defaultSettings.DefaultMaterial = resourceManager.GetMaterial(value);
		}
		if (ReadInt(node, DEBUG_LIGHT_MODEL_NODE, value)) {
			defaultSettings.DebugLightModel = resourceManager.GetModel(value);
		}
		if (ReadInt(node, DEBUG_LIGHT_SHADER_NODE, value)) {
			defaultSettings.DebugLightShader = resourceManager.GetShader(value);
		}
		if (ReadInt(node, TEXT_SHADER_NODE, value)) {
			defaultSettings.TextShader = resourceManager.GetShader(value);
		}
	}

	return MY_SUCCES_CODE;
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
int SceneManagerXmlParser::ReadLights(std::unordered_map<unsigned int, LightObject*>& lightObjects) {

	for (rapidxml::xml_node<>* objectNode = _lightsRoot->first_node(); objectNode; objectNode = objectNode->next_sibling()) {
		if (strcmp(objectNode->name(), COMMENT_NODE) == 0)
			continue;

		LightObject* lightObject = ReadLightObject(objectNode);
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
int SceneManagerXmlParser::ReadAmbientalLight(AmbientalLight& ambientalLight) {

	rapidxml::xml_node<>* lightNode = _xmlRoot->first_node(AMBIENTAL_LIGHT_NODE);
	for (rapidxml::xml_node<>* node = lightNode->first_node(); node; node = node->next_sibling()) {

		ReadVector3_rgb(node, COLOR_NODE, ambientalLight.Color);
		ReadFloat(node, RATIO_NODE, ambientalLight.Ratio);
	}
	return MY_SUCCES_CODE;
}

SceneObject* SceneManagerXmlParser::ReadSceneObject(rapidxml::xml_node<>* objectNode) {


	SceneObjectXmlFormat obj;
	TrajectoryXmlFormat trajXml;

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
		ReadTrajectory(node, TRAJECTORY_NODE, trajXml);
	}

	SceneObject* sceneObject = CreateSceneObject(obj);
	sceneObject->SetTrajectory(CreateTrajectory(trajXml));

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
void SceneManagerXmlParser::ReadTrajectory(rapidxml::xml_node<>* node, std::string noodName, TrajectoryXmlFormat& trajXml) {

	if (strcmp(node->name(), TRAJECTORY_NODE) == 0) {

		//ia atributele
		for (rapidxml::xml_attribute<>* pAttr = node->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (strcmp(pAttr->name(), TRAJECTORY_TYPE_ATTRIBUTE) == 0) {

				trajXml.type = pAttr->value();
			}
			if (strcmp(pAttr->name(), TRAJECTORY_ITERATION_COUNT_ATTRIBUTE) == 0) {

				if (strcmp(pAttr->value(), "infinite") == 0) {

					trajXml.iterationInfinity = true;
				}
				else {

					trajXml.iterationInfinity = false;
					trajXml.iterationCount = atoi(pAttr->value());
				}
			}
			if (strcmp(pAttr->name(), TRAJECTORY_DIRECTION_ATTRIBUTE) == 0) {

				trajXml.direction = pAttr->value();
			}
			if (strcmp(pAttr->name(), TRAJECTORY_SPEED_ATTRIBUTE) == 0) {

				trajXml.speed = atoi(pAttr->value());
			}
		}

		for (rapidxml::xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {

			ReadVector3_xyz(childNode, TRAJECTORY_CENTER_NODE, trajXml.center);
			ReadFloat(childNode, TRAJECTORY_RADIUS_NODE, trajXml.radius);
			ReadVectors3_xyz(childNode, TRAJECTORY_POINTS_ROOT, TRAJECTORY_POINT_NODE, trajXml.checkpoints);
			ReadVector3_xyz(childNode, TRAJECTORY_CENTER_NODE, trajXml.center);
			ReadFloat(childNode, TRAJECTORY_RADIUS_NODE, trajXml.radius);
			ReadVectors3_xyz(childNode, TRAJECTORY_ROTATION_PLANE_NODE, TRAJECTORY_VECT_NODE, trajXml.planeVectors);
		}
	}
}

LightObject* SceneManagerXmlParser::ReadLightObject(rapidxml::xml_node<>* objectNode) {

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
		ReadVector3_xyz(node, POSITION_NODE, obj.position);
	}

	LightObject* lightObject = CreateLightObject(obj);
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
bool SceneManagerXmlParser::ReadVectors3_xyz(rapidxml::xml_node<>* node, std::string rootNodeName, std::string nodeName, std::vector<Vector3>& result) {

	if (strcmp(node->name(), rootNodeName.c_str()) == 0) {

		Vector3 temp;
		for (rapidxml::xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {

			if (ReadVector3_xyz(childNode, nodeName, temp)) {
				result.push_back(temp);
			}
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
	SceneManager& sceneManager = SceneManager::GetInstance();


	SceneObject* sceneObject = nullptr;
	if (obj.modelId == "generated") {
		//specific for generated terrain
		int size = 25000;
		int cells = 250;
		sceneObject = new TerrainObject(size, cells, obj.heights);
		sceneObject->SetModel(resourceManager.GetTerrainModel(size, size, cells, cells));
	}
	else if (obj.modelId == "textQuad") {
		//delete later, just for test
		sceneObject = new SceneObject();
		Model* model = new Model();
		model->LoadQuad();
		sceneObject->SetModel(model);
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

		//sceneObject->SetMaterial(resourceManager.GetMaterial(DEFAULT_MATERIAL_ID));
		sceneObject->SetMaterial(sceneManager.GetDefaultMaterial());
	}
	else {
		sceneObject->SetMaterial(resourceManager.GetMaterial(atoi(obj.materialId.c_str())));
		//sceneObject->SetMaterial(sceneManager.GetDefaultMaterial());
	}

	sceneObject->SetPosition(obj.position);
	sceneObject->SetRotation(obj.rotation);
	sceneObject->SetScale(obj.scale);
	sceneObject->SetName(obj.name);


	return sceneObject;
}
LightObject* SceneManagerXmlParser::CreateLightObject(LightObjectXmlFormat obj) {

	LightObject::Type type;
	if (obj.type == "point")
		type = LightObject::Type::Point;
	else if (obj.type == "directional")
		type = LightObject::Type::Directional;
	else if (obj.type == "spot")
		type = LightObject::Type::Spot;

	LightObject* lightObject = new LightObject(type, obj.diffuseColor, obj.specularColor);
	lightObject->SetId(obj.id);
	lightObject->SetPosition(obj.position);

	return lightObject;
}
Trajectory* SceneManagerXmlParser::CreateTrajectory(TrajectoryXmlFormat trajXml) {

	TrajectoryLinear::Type type;
	Trajectory* traj = nullptr;
	if (trajXml.type == TRAJECTORY_TYPE_LINEAR)
		traj = new TrajectoryLinear();
	else if (trajXml.type == TRAJECTORY_TYPE_LINEAR_STRIP)
		traj = new TrajectoryStripLine();
	else if (trajXml.type == TRAJECTORY_TYPE_LINEAR_LOOP)
		traj = new TrajectoryLineLoop();
	else if (trajXml.type == TRAJECTORY_TYPE_CIRCLE)
		traj = new TrajectoryCircle();
	else {
		std::cout << "Bad type of trajectory: " << trajXml.type << std::endl;
		return nullptr;
	}

	TrajectoryLinear::Direction direction;
	if (trajXml.direction == TRAJECTORY_DIRECTION_ALTERNATE)
		direction = TrajectoryLinear::Direction::Alternate;
	else if (trajXml.direction == TRAJECTORY_DIRECTION_FORWARD)
		direction = TrajectoryLinear::Direction::Forward;
	else {
		std::cout << "Bad type of trajectory direction: " << trajXml.type << std::endl;
		return nullptr;
	}

	traj->SetDirection(direction);
	traj->SetSpeed(trajXml.speed);
	traj->AddCheckpoint(trajXml.checkpoints);
	traj->SetIterationInfinity(trajXml.iterationInfinity);
	if (trajXml.iterationInfinity == false)
		traj->SetIterationCount(trajXml.iterationCount);

	if (trajXml.type == TRAJECTORY_TYPE_CIRCLE) {

		traj->SetCenter(trajXml.center);
		traj->SetRadius(trajXml.radius);
		traj->SetPlaneVectors(trajXml.planeVectors[0], trajXml.planeVectors[1]);
	}

	return traj;
}


