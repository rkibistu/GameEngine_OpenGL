// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"

#include "Camera.h"

#include "NfgParser.h"
#include "Model.h"

#define FAST_OBJ_IMPLEMENTATION
#include "fastObj/fast_obj.h"

#include "ParserXML.h"

#include "Texture.h"

#include "SceneObject.h"

#include <cstddef> // for offsetof

#include "Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

#include "ResourceManager.h"


#define PI 3.14f




GLuint g_vboId;
GLuint g_iboId;
Shader* g_myShaders;

GLfloat g_rotationAngle = 0.0f;
GLfloat g_pasAngle = 0.005f;

Camera* g_camera;
Vector3 g_moveDirection;
Vector3 g_rotationDirection;

float g_deltaThreshold = 0.01f;
float g_deltaTimer;


//Mouse
Vector3 g_mouseClickedButtons; // (left, middle, right) 1 for being clicked, 0 for not
Vector3 g_mouseOldPos;
Vector3 g_mouseCurrentPos;
Vector3 g_mouseMoveDirection;



Model* g_model1;

Model* g_currentModel;
bool g_filledMode = true;

Texture* g_texture;
SceneObject* g_sceneObject;

Model* g_model2;
Texture* g_texture2;
SceneObject* g_sceneObject2;


ResourceManager& resourceManager = ResourceManager::GetInstance();

int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//intit global values
	g_moveDirection = Vector3();
	g_rotationDirection = Vector3();
	g_deltaTimer = 0;

	//creation of shaders and program 
	g_myShaders = resourceManager.GetShader(10);
	return 0;
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (g_filledMode) {

		g_sceneObject->Draw(g_camera);
		
		//g_sceneObject2->Draw(g_camera);
		
	}
	else {
		
		g_sceneObject->DrawWired(g_camera);
		//g_sceneObject2->DrawWired(g_camera);
	}

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, float deltaTime)
{
	g_deltaTimer += deltaTime;
	if (g_deltaTimer < g_deltaThreshold)
		return;

	g_camera->SetDeltaTime(g_deltaTimer);
	g_deltaTimer = 0;


	g_rotationAngle += g_pasAngle;
	if (g_rotationAngle > 180 * 2 * PI)
		g_rotationAngle -= 180 * 2 * PI;

	if (g_mouseClickedButtons.x == 1)
		g_rotationDirection = Vector3(g_mouseMoveDirection.y, g_mouseMoveDirection.x, 0);
	else
		g_rotationDirection = Vector3();

	//g_camera->PrintInfo();

	g_camera->Move(g_moveDirection);
	g_camera->Rotate(g_rotationDirection);



	//g_camera->RotateOX(1);
	//g_camera->RotateOY(1);
	//g_camera->RotateOZ(1);
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{

	//std::cout << key << " " << bIsPressed << std::endl;

	if (key == 'W') {

		//g_camera->MoveOY(1);
		if (bIsPressed)
			g_moveDirection.z = -1;
		else
			g_moveDirection.z = 0;
	}
	if (key == 'S') {

		//g_camera->MoveOY(-1);
		if (bIsPressed)
			g_moveDirection.z = 1;
		else
			g_moveDirection.z = 0;
	}

	if (key == 'A') {

		//g_camera->MoveOX(1);
		if (bIsPressed)
			g_moveDirection.x = -1;
		else
			g_moveDirection.x = 0;
	}
	if (key == 'D') {

		//g_camera->MoveOX(-1);
		if (bIsPressed)
			g_moveDirection.x = 1;
		else
			g_moveDirection.x = 0;
	}

	if (key == 'Q') {

		if (bIsPressed)
			g_moveDirection.y = -1;
		else
			g_moveDirection.y = 0;
	}
	if (key == 'E') {

		if (bIsPressed)
			g_moveDirection.y = 1;
		else
			g_moveDirection.y = 0;
	}

	if (key == 0x25) {

		if (bIsPressed)
			g_rotationDirection.x = 1;
		else
			g_rotationDirection.x = 0;
	}
	if (key == 0x27) {

		if (bIsPressed)
			g_rotationDirection.x = -1;
		else
			g_rotationDirection.x = 0;
	}

	if (key == 0x26) {

		if (bIsPressed)
			g_rotationDirection.y = 1;
		else
			g_rotationDirection.y = 0;
	}
	if (key == 0x28) {

		if (bIsPressed)
			g_rotationDirection.y = -1;
		else
			g_rotationDirection.y = 0;
	}

	if (key == 'K') {

		if (bIsPressed)
			g_rotationDirection.z = 1;
		else
			g_rotationDirection.z = 0;
	}
	if (key == 'L') {

		if (bIsPressed)
			g_rotationDirection.z = -1;
		else
			g_rotationDirection.z = 0;
	}
	if (key == 'P') {

		if(bIsPressed)
			g_filledMode = !g_filledMode;
	}	

}

void Mouse(ESContext* esContext, unsigned int mouseButton, unsigned int mosueEvent, int x, int y)
{
	// handle mouse event

	if (mouseButton == MouseButtons::LeftButton) {
		if (mosueEvent == MouseEvents::Down) {
			g_mouseClickedButtons.x = 1;
		}
		if (mosueEvent == MouseEvents::Up) {
			g_mouseClickedButtons.x = 0;
		}
	}
	if (mouseButton == MouseButtons::RightButton) {
		if (mosueEvent == MouseEvents::Down) {
			g_mouseClickedButtons.z = 1;
		}
		if (mosueEvent == MouseEvents::Up) {
			g_mouseClickedButtons.z = 0;
		}
	}

	if (mosueEvent == MouseEvents::MoveStart) {
		g_mouseCurrentPos = Vector3(x, y, 0);
		g_mouseMoveDirection = (g_mouseOldPos - g_mouseCurrentPos);
		//std::cout << g_mouseMoveDirection.x << " " << g_mouseMoveDirection.y << std::endl;
		g_mouseOldPos = g_mouseCurrentPos;
	}
	else {
		g_mouseMoveDirection = Vector3();
	}
}

void CleanUp()
{

	resourceManager.DestroyInstance();
}

static void InitTexture() {


	g_texture = resourceManager.GetTexture(4);

}
static void LoadModel() {

	g_model1 = resourceManager.GetModel(1);
}

static void TestXml() {

	//ParserXML::TestRapidXml();


	
}

int _tmain(int argc, _TCHAR* argv[])
{
	resourceManager.Init();


	g_camera = new Camera(
		Vector3(0, 0, 0),
		Vector3(0, 0, 1),
		Vector3(0, 1, 0)
	);

	//identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	LoadModel();
	InitTexture();



	g_sceneObject = new SceneObject();
	g_sceneObject->SetModel(g_model1);
	g_sceneObject->SetShader(g_myShaders);
	g_sceneObject->AddTexture(g_texture);

	//g_sceneObject2 = new SceneObject();
	//g_sceneObject2->SetModel(g_model2);
	//g_sceneObject2->SetShader(g_myShaders);
	//g_sceneObject2->AddTexture(g_texture2);


	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();
;

	

	printf("Press any key...\n");
	_getch();




	return 0;
}

