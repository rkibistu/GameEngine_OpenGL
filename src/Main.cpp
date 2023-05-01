// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"

#include "Camera.h"
#include <iostream>

#include "NfgParser.h"
#include "Model.h"

#define FAST_OBJ_IMPLEMENTATION
#include "fastObj/fast_obj.h"

#include "ParserXML.h"

#include "Utilities/utilities.h" // if you use STL, please include this line AFTER all other include



#define PI 3.14f




GLuint g_vboId;
GLuint g_iboId;
Shaders g_myShaders;
GLuint g_texture;

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


int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//intit global values
	g_moveDirection = Vector3();
	g_rotationDirection = Vector3();
	g_deltaTimer = 0;

	//creation of shaders and program 
	return g_myShaders.Init("Resources/Shaders/TriangleShaderVS.vs", "Resources/Shaders/TriangleShaderFS.fs");
}

void DrawFilled(ESContext* esContext) {

	glUseProgram(g_myShaders.program);


	g_currentModel->BindFilled();

	if (g_myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(g_myShaders.positionAttribute);
		glVertexAttribPointer(g_myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (g_myShaders.colorAttribute != -1) {

		glEnableVertexAttribArray(g_myShaders.colorAttribute);
		glVertexAttribPointer(g_myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}
	if (g_myShaders.uvAttrivute != -1) {

		glEnableVertexAttribArray(g_myShaders.uvAttrivute);
		glVertexAttribPointer(g_myShaders.uvAttrivute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	}


	Matrix rotateMatrix;
	rotateMatrix.SetRotationX(g_rotationAngle);



	if (g_myShaders.rotateUniform != -1) {

		Matrix identity;
		identity.SetIdentity();

		//glUniformMatrix4fv(g_myShaders.rotateUniform, 1, GL_FALSE, (GLfloat*)(rotateMatrix).m);
		glUniformMatrix4fv(g_myShaders.rotateUniform, 1, GL_FALSE, (GLfloat*)identity.m);
	}

	if (g_myShaders.mvpUniform != -1) {

		Matrix identity;
		identity.SetIdentity();

		glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)g_camera->GetMVP().m);
		//glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)identity.m);
	}

	if (g_myShaders.textureUniform != -1) {

		// aici e 0 ca avem o signura textura si am pus-o pe TEXTURE0. Vezi texture init
		glUniform1i(g_myShaders.textureUniform, 0); 
	}


	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, g_testCount, GL_UNSIGNED_INT, nullptr);
	glDrawElements(GL_TRIANGLES, g_currentModel->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

	g_currentModel->Unbind();
}

void DrawWired(ESContext* esContext) {

	glUseProgram(g_myShaders.program);


	g_currentModel->BindWired();

	if (g_myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(g_myShaders.positionAttribute);
		glVertexAttribPointer(g_myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (g_myShaders.colorAttribute != -1) {

		glEnableVertexAttribArray(g_myShaders.colorAttribute);
		glVertexAttribPointer(g_myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	Matrix rotateMatrix;
	rotateMatrix.SetRotationX(g_rotationAngle);



	if (g_myShaders.rotateUniform != -1) {

		Matrix identity;
		identity.SetIdentity();

		//glUniformMatrix4fv(g_myShaders.rotateUniform, 1, GL_FALSE, (GLfloat*)(rotateMatrix).m);
		glUniformMatrix4fv(g_myShaders.rotateUniform, 1, GL_FALSE, (GLfloat*)identity.m);
	}

	if (g_myShaders.mvpUniform != -1) {

		Matrix identity;
		identity.SetIdentity();

		glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)g_camera->GetMVP().m);
		//glUniformMatrix4fv(g_myShaders.mvpUniform, 1, GL_FALSE, (GLfloat*)identity.m);
	}



	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, g_testCount, GL_UNSIGNED_INT, nullptr);
	glDrawElements(GL_LINES, g_currentModel->GetIndicesWiredCount(), GL_UNSIGNED_SHORT, nullptr);

	g_currentModel->Unbind();
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (g_filledMode) {

		DrawFilled(esContext);
	}
	else {

		DrawWired(esContext);
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
	
}

bool loadTGA(const char* fileName, int* width, int* height, GLenum* format, unsigned char** pixels) {
	FILE* file = fopen(fileName, "rb");
	if (!file) {
		printf("Error: could not open file: %s\n", fileName);
		return false;
	}

	// Read the header
	unsigned char header[18];
	if (fread(header, sizeof(unsigned char), 18, file) != 18) {
		printf("Error: could not read TGA header\n");
		fclose(file);
		return false;
	}

	// Check if it is a supported format
	if (header[2] != 2 && header[2] != 3) {
		printf("Error: unsupported TGA format\n");
		fclose(file);
		return false;
	}

	// Get image dimensions
	*width = header[12] + (header[13] << 8);
	*height = header[14] + (header[15] << 8);
	int bpp = header[16] / 8;

	// Allocate memory for image data
	int imageSize = *width * *height * bpp;
	*pixels = (unsigned char*)malloc(imageSize);
	if (!*pixels) {
		printf("Error: could not allocate memory for image data\n");
		fclose(file);
		return false;
	}

	// Read the image data
	if (fread(*pixels, sizeof(unsigned char), imageSize, file) != imageSize) {
		printf("Error: could not read image data\n");
		fclose(file);
		free(*pixels);
		return false;
	}

	fclose(file);

	// Set the format based on the number of bytes per pixel
	if (bpp == 3) {
		*format = GL_RGB;
	}
	else {
		*format = GL_RGBA;
	}

	// We want it in RGB format for openGL
	if ((header[17] & (1 << 4)) == 0) {
		// BGR format. Convert ro RGB
		for (int i = 0; i < imageSize; i += bpp) {
			unsigned char temp = (*pixels)[i];
			(*pixels)[i] = (*pixels)[i + 2];
			(*pixels)[i + 2] = temp;
		}
	}

	
	
	
	

	return true;
}

static void InitTexture() {

	//load img
	unsigned char* imgBuffer = nullptr;
	std::string path = "Resources/Textures/Croco.tga";
	int textureWidth, textureHeight;
	GLenum BPP = 0;

	loadTGA(path.c_str(), &textureWidth, &textureHeight, &BPP, &imgBuffer);


	//create texture
	glGenTextures(1, &g_texture);
	glBindTexture(GL_TEXTURE_2D, g_texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, BPP, textureWidth, textureHeight, 0, BPP, GL_UNSIGNED_BYTE, imgBuffer);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);

	free(imgBuffer);
}
static void LoadModel() {

	g_model1 = new Model();
	g_model1->Load("Resources/Models/Croco.nfg");

	g_currentModel = g_model1;
}

static void TestXml() {

	ParserXML::TestRapidXml();
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestXml();

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


	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	delete g_camera;

	printf("Press any key...\n");
	_getch();




	return 0;
}

