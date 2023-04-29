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

#include "NfgParser.h"

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define PI 3.14f


/*
	INTREBARE!!! PROBLEMA!!

		g_camera = new Camera(
		Vector3(0, 0, 0),
		Vector3(0, 0, 1),
		Vector3(0, 1, 0)
	);
	Cu camera la pos 0,0,0 merg toate roatiile.

	Insa daca modific pozitia camerei (ori din cod, ori prin translate la runtime), nu mai merg rotatiile in jur la Ox si la Oz.\

	De ce?

*/

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

int g_testCount;

// INTREBAREEEE

/*
Credeam ca sunt ceva probleme cu rotatia. Cand roteam ceva 2d, totul bine. Daca roteam un cub incarcat din model -> nu arata bine

Asa ca am scris manual un cub pentru a putea sa colorez diferit fiecare fata.
Insa se intampla un lucru ciudat.

Chiar daca rulez doar cu 2 fete, primele 2, rosu si verde ( indexii 0-3 si 4-7) 
		-> se roteste. Insa, mereu ramane in fata aceeasi culoare. Ea nu trece in spate. De ce?

Inteleg ca e vedere ortogonala si obiectele nu se fac mai mici. Dar tot ar trb sa fie vivibile cele mai din fata.


Chestii de luat in calcul pentru a replica:
	- foloseste functia initCube in main
	- in draw, trimite pentru u_mvp matricea identitate ca rotatia sa nu se faca pentru camera
	- in main seteaza g_testCount = 12; (12 = 2 fete; 36 = toate fetele)



Si de ce, chiar daca adaug si matricea de proiectie, pot sa vad mai multe laturi ale cubului?
M-am uitat la tot cubul, fara sa includ matriciea de proiectie, si pare ca vad prin unele laturi.
Ma gandesc ca deaia pare si ca, atunci cand rotesc doar 2 laturi, ramane mereu aceeasi deasupra. Dar de ce?
	
*/

void initCube() {

	//triangle data (heap)
	Vertex verticesData[24];

	verticesData[0].pos.x =  0.3f;  verticesData[0].pos.y =  0.3f;   verticesData[0].pos.z = 0.3f; //front
	verticesData[1].pos.x = -0.3f;  verticesData[1].pos.y =  0.3f;   verticesData[1].pos.z = 0.3f;
	verticesData[2].pos.x = -0.3f;  verticesData[2].pos.y = -0.3f;   verticesData[2].pos.z = 0.3f;
	verticesData[3].pos.x =  0.3f;  verticesData[3].pos.y = -0.3f;   verticesData[3].pos.z = 0.3f;

	verticesData[4].pos.x =  0.3f;  verticesData[4].pos.y =  0.3f;   verticesData[4].pos.z = -0.3f; //back
	verticesData[5].pos.x = -0.3f;  verticesData[5].pos.y =  0.3f;   verticesData[5].pos.z = -0.3f;
	verticesData[6].pos.x = -0.3f;  verticesData[6].pos.y = -0.3f;   verticesData[6].pos.z = -0.3f;
	verticesData[7].pos.x =  0.3f;  verticesData[7].pos.y = -0.3f;   verticesData[7].pos.z = -0.3f;

	verticesData[8].pos.x =  -0.3f;  verticesData[8].pos.y =  0.3f;   verticesData[8].pos.z =   0.3f; //left
	verticesData[9].pos.x =  -0.3f;  verticesData[9].pos.y =  0.3f;   verticesData[9].pos.z =  -0.3f;
	verticesData[10].pos.x = -0.3f;  verticesData[10].pos.y = -0.3f;  verticesData[10].pos.z =  0.3f;
	verticesData[11].pos.x = -0.3f;  verticesData[11].pos.y = -0.3f;  verticesData[11].pos.z = -0.3f;

	verticesData[12].pos.x = +0.3f;  verticesData[12].pos.y =  0.3f;   verticesData[12].pos.z =  0.3f; //right
	verticesData[13].pos.x = +0.3f;  verticesData[13].pos.y =  0.3f;   verticesData[13].pos.z = -0.3f;
	verticesData[14].pos.x = +0.3f;  verticesData[14].pos.y = -0.3f;   verticesData[14].pos.z =  0.3f;
	verticesData[15].pos.x = +0.3f;  verticesData[15].pos.y = -0.3f;   verticesData[15].pos.z = -0.3f;

	verticesData[16].pos.x = -0.3f;   verticesData[16].pos.y = 0.3f;   verticesData[16].pos.z =  0.3f; //up
	verticesData[17].pos.x = -0.3f;   verticesData[17].pos.y = 0.3f;   verticesData[17].pos.z = -0.3f;
	verticesData[18].pos.x =  0.3f;   verticesData[18].pos.y = 0.3f;   verticesData[18].pos.z =  0.3f;
	verticesData[19].pos.x =  0.3f;   verticesData[19].pos.y = 0.3f;   verticesData[19].pos.z = -0.3f;

	verticesData[20].pos.x = -0.3f;  verticesData[20].pos.y = -0.3f;   verticesData[20].pos.z =  0.3f; //down
	verticesData[21].pos.x = -0.3f;  verticesData[21].pos.y = -0.3f;   verticesData[21].pos.z = -0.3f;
	verticesData[22].pos.x =  0.3f;  verticesData[22].pos.y = -0.3f;   verticesData[22].pos.z =  0.3f;
	verticesData[23].pos.x =  0.3f;  verticesData[23].pos.y = -0.3f;   verticesData[23].pos.z = -0.3f;



	//colors
	verticesData[0].color.x = 1.0f;   verticesData[0].color.y = 0.0f;    verticesData[0].color.z = 0.0f; //front red
	verticesData[1].color.x = 1.0f;   verticesData[1].color.y = 0.0f;    verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 1.0f;   verticesData[2].color.y = 0.0f;    verticesData[2].color.z = 0.0f;
	verticesData[3].color.x = 1.0f;   verticesData[3].color.y = 0.0f;    verticesData[3].color.z = 0.0f;

	verticesData[4].color.x = 0.0f;   verticesData[4].color.y = 1.0f;    verticesData[4].color.z = 0.0f; //back green
	verticesData[5].color.x = 0.0f;   verticesData[5].color.y = 1.0f;    verticesData[5].color.z = 0.0f;
	verticesData[6].color.x = 0.0f;   verticesData[6].color.y = 1.0f;    verticesData[6].color.z = 0.0f;
	verticesData[7].color.x = 0.0f;   verticesData[7].color.y = 1.0f;    verticesData[7].color.z = 0.0f;

	verticesData[8].color.x = 0.0f;  verticesData[8].color.y = 0.0f;   verticesData[8].color.z = 1.0f; //left blue
	verticesData[9].color.x = 0.0f;  verticesData[9].color.y = 0.0f;   verticesData[9].color.z = 1.0f;
	verticesData[10].color.x = 0.0f;  verticesData[10].color.y = 0.0f;   verticesData[10].color.z = 1.0f;
	verticesData[11].color.x = 0.0f;  verticesData[11].color.y = 0.0f;   verticesData[11].color.z = 1.0f;

	verticesData[12].color.x = 1.0f;  verticesData[12].color.y = 1.0f;   verticesData[12].color.z = 0.0f; //right galben
	verticesData[13].color.x = 1.0f;  verticesData[13].color.y = 1.0f;   verticesData[13].color.z = 0.0f;
	verticesData[14].color.x = 1.0f;  verticesData[14].color.y = 1.0f;   verticesData[14].color.z = 0.0f;
	verticesData[15].color.x = 1.0f;  verticesData[15].color.y = 1.0f;   verticesData[15].color.z = 0.0f;

	verticesData[16].color.x = 0.0f;  verticesData[16].color.y = 1.0f;   verticesData[16].color.z = 1.0f; //up mov
	verticesData[17].color.x = 0.0f;  verticesData[17].color.y = 1.0f;   verticesData[17].color.z = 1.0f;
	verticesData[18].color.x = 0.0f;  verticesData[18].color.y = 1.0f;   verticesData[18].color.z = 1.0f;
	verticesData[19].color.x = 0.0f;  verticesData[19].color.y = 1.0f;   verticesData[19].color.z = 1.0f;

	verticesData[20].color.x = 1.0f;  verticesData[20].color.y = 0.0f;   verticesData[20].color.z = 1.0f; //down
	verticesData[21].color.x = 1.0f;  verticesData[21].color.y = 0.0f;   verticesData[21].color.z = 1.0f;
	verticesData[22].color.x = 1.0f;  verticesData[22].color.y = 0.0f;   verticesData[22].color.z = 1.0f;
	verticesData[23].color.x = 1.0f;  verticesData[23].color.y = 0.0f;   verticesData[23].color.z = 1.0f;

	//buffer object
	glGenBuffers(1, &g_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, g_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// SI AICI! De ce conteaza daca pun setul 2 de index sa fie primul? 
	// In functie de ce set d eindexi pun primul, o alta altura va sta "deasupra"
	GLuint indexBuffer[] = {

		
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,


		8,9,10,
		10,11,9,

		12,13,14,
		14,15,13,

		16,17,18,
		18,19,17,

		20,22,21,
		22,23,21,


	};

	//index array
	glGenBuffers(1, &g_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 12 * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);
}

int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	//triangle data (heap)
	Vertex verticesData[4];

	verticesData[0].pos.x = 0.5f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
	verticesData[1].pos.x = -0.5f; verticesData[1].pos.y = 0.5f;   verticesData[1].pos.z = 0.0f;
	verticesData[2].pos.x = -0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;
	verticesData[3].pos.x = 0.5f;  verticesData[3].pos.y = -0.5f;  verticesData[3].pos.z = 0.0f;

	verticesData[0].color.x = 1.0f; verticesData[0].color.y = 0.0f; verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f; verticesData[1].color.y = 1.0f; verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f; verticesData[2].color.y = 0.0f; verticesData[2].color.z = 1.0f;
	verticesData[3].color.x = 1.0f; verticesData[3].color.y = 1.0f; verticesData[3].color.z = 1.0f;

	verticesData[0].texture.x = 1.0f;   verticesData[0].texture.y = 1.0f;   
	verticesData[1].texture.x = 0.0f;  verticesData[1].texture.y = 1.0f;   
	verticesData[2].texture.x = 0.0f;  verticesData[2].texture.y = 0.0f;  
	verticesData[3].texture.x = 1.0f;   verticesData[3].texture.y = 0.0f;  

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
	g_deltaTimer = 0;


	glEnable(GL_DEPTH_TEST);

	//creation of shaders and program 
	return g_myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glUseProgram(g_myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, g_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_iboId);


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
	glDrawElements(GL_TRIANGLES, g_testCount, GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

	//if (g_mouseClickedButtons.x == 1)
	//	g_rotationDirection = Vector3(g_mouseMoveDirection.y, g_mouseMoveDirection.x, 0);

	//else
	//	g_rotationDirection = Vector3();

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
	glDeleteBuffers(1, &g_vboId);
}

static void TestLoadObj() {

	fastObjMesh* m = fast_obj_read("cube.obj");

	int count = m->index_count;
	fastObjIndex* indeces = m->indices;



	// apply
	count = m->position_count;
	//Vertex testVertexes[8];
	Vertex* testVertexes = new Vertex[count - 1];
	int j = 0;
	for (size_t i = 3; i < m->position_count * 3; i += 3) {
		float x = m->positions[i];
		float y = m->positions[i + 1];
		float z = m->positions[i + 2];

		//printf("Vertex %zu: (%f, %f, %f)\n", i / 3, x, y, z);

		testVertexes[j].pos.x = x;
		testVertexes[j].pos.y = y;
		testVertexes[j].pos.z = z;

		testVertexes[j].color.x = j / 10.;
		testVertexes[j].color.y = 0.0f;
		testVertexes[j].color.z = j / 10.;

		j++;
	}

	testVertexes[0].color.x = 1.0f; testVertexes[0].color.y = 0.0f; testVertexes[0].color.z = 0.0f;
	testVertexes[1].color.x = 1.0f; testVertexes[1].color.y = 0.0f; testVertexes[1].color.z = 0.0f;
	testVertexes[2].color.x = 0.0f; testVertexes[2].color.y = 0.0f; testVertexes[2].color.z = 1.0f;
	testVertexes[3].color.x = 0.0f; testVertexes[3].color.y = 0.0f; testVertexes[3].color.z = 1.0f;
	testVertexes[4].color.x = 0.0f; testVertexes[4].color.y = 1.0f; testVertexes[4].color.z = 0.0f;
	testVertexes[5].color.x = 0.0f; testVertexes[5].color.y = 1.0f; testVertexes[5].color.z = 0.0f;
	testVertexes[6].color.x = 1.0f; testVertexes[6].color.y = 1.0f; testVertexes[6].color.z = 0.0f;
	testVertexes[7].color.x = 1.0f; testVertexes[7].color.y = 1.0f; testVertexes[7].color.z = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, g_vboId);
	glBufferData(GL_ARRAY_BUFFER, (count - 1) * sizeof(Vertex), testVertexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);





	count = m->index_count;

	GLuint* indexBuffer = new GLuint[count];

	for (int i = 0; i < count; i++) {

		//std::cout << indeces[i].p << " ";
		indexBuffer[i] = indeces[i].p - 1;
	}
	std::cout << std::endl;

	g_testCount = count;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);
}

static void InitTexture() {

	//load img
	unsigned char* imgBuffer = nullptr;
	std::string path = "border.png";
	int textureWidth, textureHeight;
	int BPP = 0;
	stbi_set_flip_vertically_on_load(1);
	imgBuffer = stbi_load(path.c_str(), &textureWidth, &textureHeight, &BPP, 4); //load img


	//create texture
	glGenTextures(1, &g_texture);
	glBindTexture(GL_TEXTURE_2D, g_texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgBuffer);
	glActiveTexture(GL_TEXTURE0);

	if (imgBuffer)
		stbi_image_free(imgBuffer);
}

static void TestParser() {

	NfgParser parser;
	parser.Load("House.nfg");
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestParser();

	g_camera = new Camera(
		Vector3(0, 0, -3),
		Vector3(0, 0, -2),
		Vector3(0, 1, 0)
	);




	//identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	// ASTA E PT LOAD LA CUBE
	TestLoadObj();
	

	// ASTEA 2 SUNT PENTRU A TESTA CUBUL FACUT MANUAL! AU LEAGTURA CU INTREBAREA DE SUS!
	//initCube();
	//g_testCount = 36;


	//normal square
	//g_testCount = 6;

	//InitTexture();

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

