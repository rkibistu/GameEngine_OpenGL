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

#define PI 3.14f

GLuint g_vboId;
Shaders g_myShaders;

GLfloat g_rotationAngle = 0.0f;
GLfloat g_pasAngle = 0.5f;

Camera* g_camera;
Vector3 g_moveDirection;

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	Vertex verticesData[3];

	verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  3.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  3.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  3.0f;

	verticesData[0].color.x = 1.0f; verticesData[0].color.y = 0.0f; verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f; verticesData[1].color.y = 1.0f; verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f; verticesData[2].color.y = 0.0f; verticesData[2].color.z = 1.0f;

	//buffer object
	glGenBuffers(1, &g_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, g_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//creation of shaders and program 
	return g_myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, g_vboId);

	
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

		//glUniformMatrix4fv(g_myShaders.matrixUniform, 1, GL_FALSE, (GLfloat*)rotateMatrix.m);

		Matrix test;
		test.SetPerspective(45.0f, 800.0f / 600.0f, 0.1f, 3.0f);
		

		Matrix nothing;
		nothing.SetIdentity();

		//glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)nothing.m);

		Matrix temp = g_camera->UpdateWorldView();

		Matrix res = temp * test;

		

		glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)( temp  ).m);


		//glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)g_camera->UpdateWorldView().m);
		
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{

	g_camera->SetDeltaTime(deltaTime);
	g_camera->Move(g_moveDirection);




	g_moveDirection = Vector3();
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

	if (key == 'W') {

		//g_camera->MoveOY(1);
		g_moveDirection.z = -1;
	}
	else if (key == 'S') {

		//g_camera->MoveOY(-1);
		g_moveDirection.z = 1;
	}

	if (key == 'A') {

		//g_camera->MoveOX(1);
		g_moveDirection.x = -1;
	}
	else if (key == 'D') {

		//g_camera->MoveOX(-1);
		g_moveDirection.x = 1;
	}

	if (key == 'Q') {

		g_moveDirection.y = -1;
	}
	else if (key == 'E') {

		g_moveDirection.y = 1;
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

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	delete g_camera;

	printf("Press any key...\n");
	_getch();

	


	return 0;
}

