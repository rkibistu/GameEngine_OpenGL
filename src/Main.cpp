// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Globals.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "TextRenderer.h"
#include "EffectManager.h"

#include <iostream>
#include <conio.h>


#include "Utilities/utilities.h" // if you use STL, please include this line AFTER all other include


#define PI 3.14f

float g_deltaThreshold = 0.01f;
float g_deltaTimer;


ResourceManager& resourceManager = ResourceManager::GetInstance();
SceneManager& sceneManager = SceneManager::GetInstance();
EffectManager* effectManager = new EffectManager();

int Init(ESContext* esContext)
{

	Input::Init();
	resourceManager.Init();
	sceneManager.Init(esContext);
	effectManager->Init();

	return 0;
}

void DrawTriangles(ESContext* esContext)
{
	// DE FIECARE DATA CAND DESENEZ PE UN FRAME BUFFER
	//		IMG SE SHIFTEAZA LA DREAPTA PUTIN. nu stiu de ce
	// SI atunci apare problema cand fac blend cu img initiala

	//sceneManager.Draw(esContext);
	effectManager->Draw(esContext);

	//
}

void Update(ESContext* esContext, float deltaTime)
{
	g_deltaTimer += deltaTime;
	if (g_deltaTimer < g_deltaThreshold)
		return;

	sceneManager.Update(esContext, g_deltaTimer);

	g_deltaTimer = 0;

	Input::Update();
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	Input::UpdateKey(key, bIsPressed);	
}

void Mouse(ESContext* esContext, unsigned int mouseButton, unsigned int mosueEvent, int x, int y)
{
	// handle mouse event
	Input::UpdateMouse(mouseButton, mosueEvent,x,y);
}

void CleanUp()
{
	effectManager->Destroy();
	delete effectManager;
	resourceManager.DestroyInstance();
	sceneManager.DestroyInstance();
	Input::Destroy();
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
	
	esRegisterDrawFunc(&esContext, DrawTriangles);
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

