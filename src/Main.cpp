// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Globals.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Input.h"

#include <iostream>
#include <conio.h>


#include "Utilities/utilities.h" // if you use STL, please include this line AFTER all other include


#define PI 3.14f

GLfloat g_rotationAngle = 0.0f;
GLfloat g_pasAngle = 0.005f;

Vector3 g_moveDirection;
Vector3 g_rotationDirection;

bool g_filledMode = true;

float g_deltaThreshold = 0.01f;
float g_deltaTimer;

//Mouse
Vector3 g_mouseClickedButtons; // (left, middle, right) 1 for being clicked, 0 for not
Vector3 g_mouseOldPos;
Vector3 g_mouseCurrentPos;
Vector3 g_mouseMoveDirection;

ResourceManager& resourceManager = ResourceManager::GetInstance();
SceneManager& sceneManager = SceneManager::GetInstance();
Input& input = Input::GetInstance();

int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	resourceManager.Init();
	sceneManager.Init();

	return 0;
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager.Draw();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, float deltaTime)
{


	g_deltaTimer += deltaTime;
	if (g_deltaTimer < g_deltaThreshold)
		return;
	sceneManager.GetActiveCamera()->SetDeltaTime(g_deltaTimer);
	g_deltaTimer = 0;


	g_rotationAngle += g_pasAngle;
	if (g_rotationAngle > 180 * 2 * PI)
		g_rotationAngle -= 180 * 2 * PI;

	if (g_mouseClickedButtons.x == 1)
		g_rotationDirection = Vector3(g_mouseMoveDirection.y, g_mouseMoveDirection.x, 0);
	else
		g_rotationDirection = Vector3();


	sceneManager.GetActiveCamera()->Move(g_moveDirection);
	sceneManager.GetActiveCamera()->Rotate(g_rotationDirection);

	if (input.GetKeyDown('G')) {
		std::cout << "G\n";
	}
	if (input.GetKeyUp('H')) {
		std::cout << "H\n";
	}
	if (input.GetKey('J')) {
		std::cout << "J\n";
	}

	input.Update();
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{

	input.UpdateKey(key, bIsPressed);

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


int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;
	
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

