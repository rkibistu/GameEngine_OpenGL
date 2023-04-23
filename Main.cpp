// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"

#include "Camera.h"
#include <iostream>

#define PI 3.14f

GLuint g_vboId;
GLuint g_iboId;
Shaders g_myShaders;

GLfloat g_rotationAngle = 0.0f;
GLfloat g_pasAngle = 0.5f;

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

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );


	//triangle data (heap)
	Vertex verticesData[4];

	verticesData[0].pos.x =  0.5f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  2.0f;
	verticesData[1].pos.x =  -0.5f; verticesData[1].pos.y = 0.5f;   verticesData[1].pos.z =  2.0f;
	verticesData[2].pos.x = -0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  2.0f;
	verticesData[3].pos.x =  0.5f;  verticesData[3].pos.y = -0.5f;  verticesData[3].pos.z =  2.0f;

	verticesData[0].color.x = 1.0f; verticesData[0].color.y = 0.0f; verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f; verticesData[1].color.y = 1.0f; verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f; verticesData[2].color.y = 0.0f; verticesData[2].color.z = 1.0f;
	verticesData[3].color.x = 1.0f; verticesData[3].color.y = 1.0f; verticesData[3].color.z = 1.0f;

	//buffer object
	glGenBuffers(1, &g_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, g_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	GLuint indexBuffer[] = {
		0, 1, 2,
		2, 3, 0
	};

	//index array
	glGenBuffers(1, &g_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);

	//intit global values
	g_moveDirection = Vector3();
	g_rotationDirection = Vector3();
	g_deltaTimer = g_deltaThreshold;

	//creation of shaders and program 
	return g_myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, g_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_iboId);

	
	if(g_myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(g_myShaders.positionAttribute);
		glVertexAttribPointer(g_myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (g_myShaders.positionAttribute != -1) {

		glEnableVertexAttribArray(g_myShaders.colorAttribute);
		glVertexAttribPointer(g_myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	Matrix rotateMatrix;
	rotateMatrix.SetRotationZ(g_rotationAngle);
	
	if (g_myShaders.mvpUniform != -1) {

		glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)g_camera->GetMVP().m);
	}

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	g_deltaTimer -= deltaTime;
	if (g_deltaTimer > 0)
		return;

	g_deltaTimer = g_deltaThreshold;

	if (g_mouseClickedButtons.x == 1)
		g_rotationDirection = Vector3(g_mouseMoveDirection.y, g_mouseMoveDirection.x, 0);
		
	else
		g_rotationDirection = Vector3();

	//g_camera->PrintInfo();
	g_camera->SetDeltaTime(deltaTime);
	g_camera->Move(g_moveDirection);
	g_camera->Rotate(g_rotationDirection);



	//g_camera->RotateOX(1);
	//g_camera->RotateOY(1);
	//g_camera->RotateOZ(1);
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

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

	std::cout << key << "\n";

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
		std::cout << g_mouseMoveDirection.x << " " << g_mouseMoveDirection.y << std::endl;
		g_mouseOldPos = g_mouseCurrentPos;
	}
	else {
		g_mouseMoveDirection = Vector3();
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &g_vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	g_camera = new Camera(
		Vector3(0, 0, 0),
		Vector3(0, 0, 1),
		Vector3(0, 1, 0)
	);

	

	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	delete g_camera;

	printf("Press any key...\n");
	_getch();

	


	return 0;
}

