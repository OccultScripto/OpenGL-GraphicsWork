// OpenGL Project Application.
// Ionut Indre

// Includes
#include "stdafx.h"
#include <stdlib.h>
#include "glut.h"
#include <gl/gl.h>
#include "glm.h"
#include "tga.h"
#include "glui.h" 

float RotMatrix[1000][1000] = { };
//Control User
int main_window;

GLUI_RadioGroup *radio;
int obj_type = 2;
int wireframe = 0;
//int segments = 8;
//Define a constant that identify the radio buttons group
#define RADIO_BUTTONS_GROUP_ID 200



int i = 0;
//Screen Resolution
int screen_width = 1024;
int screen_height = 768;

//Global rotation angles
GLfloat fGlobalAngleX, fGlobalAngleY, fGlobalAngleZ;

//Objects
GLMmodel *pModel;
GLMmodel *table;
GLMmodel *match;
GLMmodel *skull;
GLMmodel *stone;
GLMmodel *scroll;
GLMmodel *candle;
GLMmodel *candleSupport;
GLMmodel *ink;
GLMmodel *dop;
GLMmodel *feather;

//Textures
GLuint tableTexture;
GLuint matchTexture;
GLuint skullTexture;
GLuint candleTexture;
GLuint candleSupportTexture;
GLuint stoneTexture;
GLuint scrollTexture;
GLuint dopTexture;
GLuint inkTexture;
GLuint featherTexture;

//Fog
GLfloat fogColor[] = { 0.3, 0.1, 0.1 };
float fogDensity = 0.1;

//Camera Zoom
float CAMzoom = 2.0;
//Light movement angle
GLfloat angleLight;

// Room light
GLfloat mylight_ambient[] = { 0.1, 0.1, 0.1, 0.1 };
GLfloat mylight_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mylight_specular[] = { 1.0, 1.0, 1.0, 1.0 };

//Room light position
GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.1 };

//Candle Light
/*
GLfloat material_1_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat material_1_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat material_1_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat material_1_shininess[] = { 128.0 };
GLfloat material_1_emission[] = { 1.0, 1.0, 0.5, 1.0 };
GLfloat material_no_emission[] = { 0.0, 0.0, 0.0, 1.0 };
*/
//Lamp
GLfloat mylight_1_ambient[] = { 0.8, 0.8, 0, 0.8 };
GLfloat mylight_1_diffuse[] = { 1.0, 1.0, 0, 1.0 };
GLfloat mylight_1_specular[] = { 1.0, 1.0, 0, 1.0 };
GLfloat mylight_1_position[] = { 0.0, 2.5, -1.0, 8 };

GLfloat material_1_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat material_1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat material_1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat material_1_shininess[] = { 120.0 };

//int enableCandle = 0;

//Schimbare vizualizare
GLint mod = 0;

// Variablie folosite pentru animarea chibritului.
int animatie = 0;
float matchX = 0.0;
float matchY = 0.0;
float matchZ = 0.0;
int moveUp = 0;
//int done = 0;

//RAIN
#define MAX_PARTICLES 3000
#define WCX		640
#define WCY		480
#define RAIN	0

float slowdown = 2.0;
float velocity = 0.0;
float zoom = 2.5;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall;


//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;

float accum = 2.0;

int scaleSkull = 0;
float TransXYZ[] = { 0.0,0.0 };


typedef struct {
	// Life
	bool alive;	// is the particle alive?
	float life;	// particle lifespan
	float fade; // decay
	// color
	float red;
	float green;
	float blue;
	// Position/direction
	float xpos;
	float ypos;
	float zpos;
	// Velocity/Direction, only goes down in y dir
	float vel;
	// Gravity
	float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

// Initialize / Reset Particles - give them their attributes
void initParticles(int i) {
	par_sys[i].alive = true;
	par_sys[i].life = 1.0;
	par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)(rand() % 150) - 80;
	par_sys[i].ypos = 10.0;
	par_sys[i].zpos = (float)(rand() % 150) - 80;

	par_sys[i].red = 0.0;
	par_sys[i].green = 0.0;
	par_sys[i].blue = 1.0;

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -0.8;//-0.8;

}


// For Rain
void drawRain() {
	float x, y, z;
	//glTranslatef(5, 5, 5);
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos + 1.5;
			y = par_sys[loop].ypos + 1.5;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glColor3f(0.0, 0.0, 1.0);

			glBegin(GL_LINES);
			//glTranslatef(0.5, 0.5, 0.5);
			glVertex3f(x, y, z);
			glVertex3f(x, y - 1.5, z);
			glEnd();

			// Update values
			//Move
			// Adjust slowdown for speed!
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 5);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].life = -1.0;
			}
			//Revive 
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}

// For Snow
void drawSnow() {
	float x, y, z;
	//glTranslatef(1, 1, 1);
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos + 6.5;
			y = par_sys[loop].ypos + 9.0;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glColor3f(1.0, 1.0, 1.0);
			glPushMatrix();
			glTranslatef(x, y, z);
			glutSolidSphere(0.2, 5, 5);
			glPopMatrix();

			// Update values
			//Move
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 24);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;


			//Revive 
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}




void moveToRock()
{
	if (moveUp == 0)
	{
		if (matchX > -0.5)
		{
			matchX -= 0.1;

		}

		if (matchY > -2.0)
		{
			matchY -= 0.08;
		}

		if (matchZ < 1.5)
		{
			matchZ += 0.1;
		}
	}
}

void moveToCandle()
{
	if (moveUp == 0)
	{
		if (matchX > -4.0)
		{
			matchX -= 0.1;

		}

		if (matchY > -7.0)
		{
			matchY -= 0.04;
		}

		if (matchZ < 2.0)
		{
			matchZ += 0.01;
		}
	}
}

void moveBackMatch()
{
	if (moveUp == 1){
		if (matchX < 0.0) matchX += 0.05;
		if (matchY < -0.3)matchY += 0.05;
		if (matchZ > 0.0)matchZ -= 0.05;
		if (matchY < 0.0)matchY += 0.02;
	}
	//if ((matchZ < 0.0)&(matchX > 0.0)&(matchY > 0.0)&(moveUp=1)) done = 1;
}

void moveMatch()
{	
	if (animatie == 1)
	{
		moveToRock();
		moveToCandle();
		if (matchY < -7.0 ) moveUp = 1;
		moveBackMatch();
	/*	if (done == 1)
		{
			animatie = 0;
			moveUp = 0;
		}
	*/
	}
}

float featherX = 0.0;
float featherY = 0.0;
float featherZ = 0.0;
int Feather = 0;
int moveDownff = 0;

void moveDownf(){
	if (moveDownff == 1)
	{
		if (featherY > 0.0)
		{
			featherY -= 0.01;
		}
	}
}

void moveUpf(){
	if (moveDownff == 0)
	{
		if (featherY < 0.5) {
			featherY += 0.01;
		}
	}
	if (featherY >= 0.4)	{
		moveDownff = 1;
		
	}
}

void moveFeather()
{
	if (Feather == 1)
	{
		moveUpf();
		moveDownf();
	}
}


//Animate Scene
int animateEntry = 0;
float CAMzoom2 = 0.0;
int lightOf = 0;
int fog = 0;
int rotate = 0;
//int fogDensity = 0.0;
void animateScene()
{

	lightOf += 1;
	//moveMatch();
	//moveFeather();
	if (animateEntry == 1) {
		if (CAMzoom < 2.5){
			CAMzoom += 0.001;
		}

		if (rotate < 360)
		{
			rotate++;

			fGlobalAngleX -= 1;
			if (fGlobalAngleX <= -360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleX = (GLint)fGlobalAngleX % 360;

			fGlobalAngleY -= 1;
			if (fGlobalAngleY <= -360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleY = (GLint)fGlobalAngleY % 360;
		}

		if (((lightOf % 2) == 1) && (lightOf <500))
		{
			glDisable(GL_LIGHT0);
			glEnable(GL_LIGHT1);

			fogDensity += 0.005;
		}
		else
		{
			glDisable(GL_LIGHT1);
			glEnable(GL_LIGHT0);

		}
		if (lightOf >= 500) fogDensity = 0.0;
		/*
		if ((CAMzoom > 1.5 )){
		CAMzoom -= 0.0001;
		}*/
	}
}

//Skull Move Obj
float skullXtrans = 0.0;
float skullYtrans = 0.0;
float skullZtrans = 0.0;
float rotateXSkull = 0.0;
float scaleXSkull = 1.0;
float scaleYSkull = 1.0;
float scaleZSkull = 1.0;

void InitModels(GLMmodel **pmodel, char*filename)
{
	if (!*pmodel)
	{
		*pmodel = glmReadOBJ(filename);
		if (!*pmodel)
			exit(0);
		glmUnitize(*pmodel);
		//generate facet normal vectors for model 
		glmFacetNormals(*pmodel);
		//generate vertex normal vectors (called after generating facet normals) 
		glmVertexNormals(*pmodel, 90.0);
	}
}


// Initialiations 
void initOpenGL()
{
	//initParticles(10);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);

	//Light Enable Room
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	//Light Enable Candle
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	//Light Enable Match
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);

	InitModels(&table, "objects\\table.obj");
	//Create the texture table object 
	glGenTextures(1, &tableTexture);
	//Assign the texture file to the texture table object 
	loadTGA("textures\\table.tga", tableTexture);

	InitModels(&skull, "objects\\skull.obj");
	//Create the texture for skull object 
	glGenTextures(1, &skullTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\skull.tga", skullTexture);

	InitModels(&candle, "objects\\candle.obj");
	//Create the texture for candle object 
	glGenTextures(1, &candleTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\skull.tga", candleTexture);

	InitModels(&candleSupport, "objects\\candleSupport.obj");
	//Create the texture for candleSupport object 
	glGenTextures(1, &candleSupportTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\support.tga", candleSupportTexture);

	InitModels(&ink, "objects\\ink.obj");
	//Create the texture for ink object 
	glGenTextures(1, &inkTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\ink.tga", inkTexture);

	InitModels(&dop, "objects\\dop.obj");
	//Create the texture for dop object 
	glGenTextures(1, &dopTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\dop.tga", dopTexture);

	InitModels(&feather, "objects\\feather.obj");
	//Create the texture for feather object 
	glGenTextures(1, &featherTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\feather.tga", featherTexture);

	InitModels(&stone, "objects\\stone.obj");
	//Create the texture for stone object 
	glGenTextures(1, &stoneTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\stone.tga", stoneTexture);

	InitModels(&scroll, "objects\\scroll.obj");
	//Create the texture for scroll object 
	glGenTextures(1, &scrollTexture);
	//Assign the texture file to the texture match object 
	loadTGA("textures\\paper.jpg", scrollTexture);

	InitModels(&match, "objects\\match.obj");
	//Create the texture table object 
	glGenTextures(1, &matchTexture);
	//Assign the texture file to the texture table object 
	loadTGA("textures\\match.tga", matchTexture);

}

void drawModel(GLMmodel **pmodel, char*filename, GLuint mode)
{
	if (!*pmodel)
	{
		*pmodel = glmReadOBJ(filename);
		if (!*pmodel) exit(0);
		glmUnitize(*pmodel);
		//generate facet normal vectors for model 
		glmFacetNormals(*pmodel);
		//generate vertex normal vectors (called after generating facet normals) 
		glmVertexNormals(*pmodel, 90.0);
	}
	glmDraw(*pmodel, mode);
}

void EnableFog() {
	glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, fogDensity);
	glEnable(GL_FOG);
}

int enableRain = 0;
int enableSnow = 0;


void myGlutIdleFunction(void)
{
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);
	glutPostRedisplay();
}


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	animateScene();
	gluLookAt(0.0, 0.0, CAMzoom, 0.0, 0.0, -10, 0.0, 1.0, 0.0);

	if (mod % 3 == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (mod % 3 == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (mod % 3 == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);


	if (obj_type == 0) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (obj_type == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (obj_type == 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	scaleYSkull = scaleXSkull;
	scaleZSkull = scaleXSkull;
//	scaleZSkull = scaleSkull;

	skullXtrans = TransXYZ[0];
	skullXtrans = TransXYZ[1];

	EnableFog();

	if (enableRain == 1)
		drawRain();
	if (enableSnow == 1)
		drawSnow();

	//Draw Lamp
	glLightfv(GL_LIGHT1, GL_AMBIENT, mylight_1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, mylight_1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mylight_1_specular);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_1_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_1_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_1_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_1_shininess);
	
	/*
	glPushMatrix();
	glTranslatef(0.0, 2.0, 0.0);
	glutSolidSphere(0.5, 2.0, 2.0);
	glLightfv(GL_LIGHT1, GL_POSITION, mylight_1_position);
	glPopMatrix();
	*/


	//glScalef(1.0,zoom,1.0);
	//set global rotation on the X,Y and Z axes
	glRotatef(fGlobalAngleX, 1.0, 0.0, 0.0);
	glRotatef(fGlobalAngleY, 0.0, 1.0, 0.0);
	glRotatef(fGlobalAngleZ, 0.0, 0.0, 1.0);

	//Rotate Light Angle
	angleLight += 0.1;

	//Lights
	glLightfv(GL_LIGHT0, GL_SPECULAR, mylight_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mylight_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mylight_ambient);

	//if (enableCandle == 1) {
	//Candle Light Material
	//s	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_1_ambient);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_1_diffuse);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_1_specular);
	//s	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_1_shininess);
	
	//x glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_1_emission);
	//}

	//Candle light Object
	glPushMatrix();
	glTranslatef(0.4, 1, -0.17);
	glScaled(0.02, 0.03, 0.02);
	glutSolidSphere(1, 5, 2);
	glLightfv(GL_LIGHT1, GL_POSITION, mylight_1_position);
	glPopMatrix();
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_no_emission);

	//Draw Room Light
	glPushMatrix();
	glRotatef(angleLight, 0.05, 0, 0);
	glTranslatef(10.0, 0.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, tableTexture);
	//Draw the Obj file along with the texture 
	glmDraw(table, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, inkTexture);
	//Draw the Obj file along with the texture 
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(0.0, 1.05, -0.7);
	glmDraw(ink, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, candleSupportTexture);
	//Draw the Obj file along with the texture 
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(0.5, 0.57, -0.22);
	glmDraw(candleSupport, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
		
	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, dopTexture);
	//Draw the Obj file along with the texture 
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(-0.5, 1.05, -0.7);
	glmDraw(dop, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, stoneTexture);
	//Draw the Obj file along with the texture 
	glScalef(0.1, 0.1, 0.1);
	glTranslatef(7.0, 3.8, -1.3);
	glmDraw(stone, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, skullTexture);
	//Draw the Obj file along with the texture 
	glTranslatef(-0.7, +0.42, -0.05);
	glRotatef(180, 0.0, 1.0, 0.0);
	//Move Skull
	glRotatef(rotateXSkull,0.0,1.0,0.0);
	glScalef(scaleXSkull,scaleYSkull,scaleZSkull);
	glTranslatef(skullXtrans, skullYtrans, skullZtrans);
	//
	glmDraw(skull, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, scrollTexture);
	//Draw the Obj file along with the texture 
	glScalef(0.45, 0.45, 0.45);
	glTranslatef(0.0, 0.78, 0.3);
	glRotatef(90, 0.0, 1.0, 0.0);
	glmDraw(scroll, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, featherTexture);
	//Draw the Obj file along with the texture 
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(0.25, 1.0, -0.55);
	moveFeather();
	glTranslatef(featherX,featherY,featherZ);
	glmDraw(feather, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, matchTexture);
	//Draw the Obj file along with the texture 
	glTranslatef(0.8, 0.29, 0.15);
	glScalef(0.10, 0.10, 0.10);
	glRotatef(180, 1.0, 0.0, 0.0);
	moveMatch();
	glTranslatef(matchX, matchY, matchZ);
	glmDraw(match, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glPushMatrix();
	//Enable 2D texturing 
	glEnable(GL_TEXTURE_2D);
	//Select current texture 
	glBindTexture(GL_TEXTURE_2D, candleTexture);
	//Draw the Obj file along with the texture 
	glTranslatef(0.4, 0.8, -0.18);
	glmDraw(candle, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing 
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glDisable(GL_FOG);
	glutSwapBuffers();
}

void changeSize(int w, int h)
{
	screen_width = w;
	screen_height = h;

	if (h == 0)
		h = 1;

	float ratio = 1.0*w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f*zoom, ratio, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 50.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
}

void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 't':
		//process
		glutPostRedisplay();
		animatie = 0;
		matchX = 0.0;
		matchY = 0.0;
		matchZ = 0.0;
		moveUp = 0;
		animateEntry = 0;
		CAMzoom = 2.0;
		Feather = 0;
		moveDownff = 0;
		rotate = 0;
		fog = 0;
		lightOf = 0;
		//enableCandle = 0;
		scaleSkull = 0;
		break;
	case 27: //Esc
		exit(1);
		break;
		//control the global Y rotation angle using 'a' and 'd'
	case 'a':
		fGlobalAngleY += 1;
		if (fGlobalAngleY >= 360) //clamp the rotation angle in the [0,360) interval
			fGlobalAngleY = (GLint)fGlobalAngleY % 360;
		break;
	case 'd':
		fGlobalAngleY -= 1;
		if (fGlobalAngleY <= -360) //clamp the rotation angle in the [0,360) interval
			fGlobalAngleY = (GLint)fGlobalAngleY % 360;
		break;
		//control the global X rotation angle using 'w' and 's'
	case 'w':
		fGlobalAngleX += 1;
		if (fGlobalAngleX >= 360) //clamp the rotation angle in the [0,360) interval
			fGlobalAngleX = (GLint)fGlobalAngleX % 360;
		break;
	case 's':
		fGlobalAngleX -= 1;
		if (fGlobalAngleX <= -360) //clamp the rotation angle in the [0,360) interval
			fGlobalAngleX = (GLint)fGlobalAngleX % 360;
		break;
	case 'q':
		fGlobalAngleZ += 1;
		if (fGlobalAngleZ >= 360) //clamp the rotation angle in the [0,360) interval
			fGlobalAngleZ = (GLint)fGlobalAngleZ % 360;
		break;
	case 'e':
		fGlobalAngleZ -= 1;
		if (fGlobalAngleZ <= -360) //clamp the rotation angle in the [0,360) interval
			fGlobalAngleZ = (GLint)fGlobalAngleZ % 360;
		break;
	case 'o':
		CAMzoom += 0.1;
		break;
	case 'i':
		CAMzoom -= 0.1;
		break;

	case 'f':
		fogDensity += 0.1;
		break;
	case 'g':
		fogDensity -= 0.1;
		break;
	case 'z':
		material_1_shininess[0] -= 50.0;
		break;
	case 'x':
		material_1_shininess[0] += 50.0;
		break;
	case 'm':
		mod++;
		break;
	case ' ':

		while (i < 360){
			i++;
		}
		break;
	case'b':
		animatie = 1;
		moveMatch();
		break;
	case 'l':
		glDisable(GL_LIGHT1);
		break;

	case 'k':
		glEnable(GL_LIGHT1);
		break;
	case ',':
		glDisable(GL_LIGHT0);
		break;
	case'.':
		glEnable(GL_LIGHT0);
		break;
	case 'c':
		//Animation Entry.
		animateEntry = 1;
		animateScene();
		break;
	case'v':
		Feather = 1;
		moveFeather();
		break;
	case'1':
		// Rotate Skull
		rotateXSkull += 0.3;
		break;
	case'2':
		// Scale + Skull
		scaleXSkull += 0.1;
		scaleYSkull += 0.1;
		scaleZSkull += 0.1;
		break;
	case'3':
		// Scale - Skull
		scaleXSkull -= 0.1;
		scaleYSkull -= 0.1;
		scaleZSkull -= 0.1;
		break;
	case '4':
		// Translate X Skull +
		skullXtrans += 0.1;
		break;
	case '5':
		// Translate Y Skull +
		skullYtrans += 0.1;
		break;
	case '6':
		// Translate Z Skull +
		skullZtrans += 0.1;
		break;
	case '7':
		// Translate X Skull -
		skullXtrans -= 0.1;
		break;
	case '8':
		// Translate Y Skull -
		skullYtrans -= 0.1;
		break;
	case '9':
		// Translate Z Skull -
		skullZtrans -= 0.1;
		break;
	}
}

int light_value1 = 0;
int LIGHT_ID1 = 0;
int light_value2 = 0;
int LIGHT_ID2 = 0;
void control_light(int control)
{
	if (control == LIGHT_ID1)
	{
		if (light_value1 == 1) glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);
	}
	if (control ==LIGHT_ID2)
	{
		if (light_value2 == 1) glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);
	}
}

int RESET_ID1 = 0;
int reset_value = 0;
void control_reset(int control)
{
	if (control == RESET_ID1)
	{
		if (reset_value == 1)
		{
			glutPostRedisplay();
			animatie = 0;
			matchX = 0.0;
			matchY = 0.0;
			matchZ = 0.0;
			moveUp = 0;
			animateEntry = 0;
			CAMzoom = 2.0;
			Feather = 0;
			moveDownff = 0;
			rotate = 0;
			fog = 0;
			lightOf = 0;
			rotateXSkull = 0.0;
			scaleXSkull = 1.0;
			scaleYSkull = 1.0;
			scaleZSkull = 1.0;
			scaleSkull = 1.0;
			skullXtrans = 0.0;
			skullYtrans = 0.0;
			skullZtrans = 0.0;
			TransXYZ[0] = { 0.0};
			TransXYZ[1] = { 0.0};
		}
		
	}
}

int ANIM_ID1 = 0;
int ANIM_ID2 = 0;
int ANIM_ID3 = 0;

int animate_value1 = 0;
int animate_value2 = 0;
int animate_value3 = 0;

void control_anim1(int control)
{
	if (control == ANIM_ID1)
	{
		if (animate_value1 == 1)
		{
			animatie = 1;
			moveMatch();
		}
	}

}

void control_anim2(int control)
{
	if (control == ANIM_ID2)
	{
		if (animate_value2 == 1)
		{
			animateEntry = 1;
			animateScene();
		}
	}
}

void control_anim3(int control)
{
	if (control == ANIM_ID3)
	{
		if (animate_value3 == 1)
		{
			Feather = 1;
			moveFeather();
		}
	}
}


//float RotMatrix[][] = {};

int main(int argc, char* argv[])
{
	//Initialize the GLUT library
	glutInit(&argc, argv);
	//Set the display mode
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//Set the initial position and dimensions of the window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen_width, screen_height);
	//creates the window
	main_window= glutCreateWindow("Ionut Indre - Ancient Writer");
	//Specifies the function to call when the window needs to be redisplayed
	glutDisplayFunc(renderScene);
	//Sets the idle callback function
	glutIdleFunc(renderScene);
	//Sets the reshape callback function
	glutReshapeFunc(changeSize);
	//Keyboard callback function
	glutKeyboardFunc(processNormalKeys);

	//Initialize GLUI and create window 
	//main_window = glutCreateWindow("GLUI tutorial"); 
	//glutDisplayFunc(renderScene);
	//glutReshapeFunc(changeSize);
	GLUI *glui;
	glui = GLUI_Master.create_glui("GLUI", 0, 1150, 50);
	
	
	//Define the “Type” panel described in Figure 1 
	GLUI_Panel *type_panel = glui->add_panel("Controls");
	//Add the radio buttons group 
	//The &obj_type is the live variable attached with the radio buttons group control 
	//The RADIO_BUTTONS_GROUP_ID uniquely identify this GLUI graphical control 
	//The callback function that is not defined in this example represents the 4th parameter
	//of the add_radiogroup_to_panel function 
	radio = glui->add_radiogroup_to_panel(type_panel, &obj_type, RADIO_BUTTONS_GROUP_ID);
	glui->add_radiobutton_to_group(radio, "Smooth");
	glui->add_radiobutton_to_group(radio, "Points");
	glui->add_radiobutton_to_group(radio, "Wireframe");
	glui->add_separator_to_panel(type_panel); 
	//Add the Light control 
	//glui->add_checkbox_to_panel(type_panel, "Light Ambient", &wireframe);
	//glui->add_checkbox_to_panel(type_panel, "Light Candle", &wireframe);
	glui->add_checkbox("Light Ambiental", &light_value1, LIGHT_ID1, control_light);
	glui->add_checkbox("Light Candle", &light_value2, LIGHT_ID2, control_light);
	//glui->add_separator_to_panel(type_panel);
	//Add animate calls
	glui->add_checkbox("Animate Match", &animate_value1, ANIM_ID1, control_anim1);
	glui->add_checkbox("Animate Scene", &animate_value2, ANIM_ID2, control_anim2);
	glui->add_checkbox("Animate Feather", &animate_value3, ANIM_ID3, control_anim3);
	//glui->add_separator_to_panel(type_panel);
	// Modifiers
	GLUI_Spinner *segment_spinner1 = glui->add_spinner_to_panel(type_panel, "Zoom:", GLUI_SPINNER_FLOAT, &CAMzoom);
	GLUI_Spinner *segment_spinner2 = glui->add_spinner_to_panel(type_panel, "Fog:", GLUI_SPINNER_FLOAT, &fogDensity);
	GLUI_Spinner *segment_spinner3 = glui->add_spinner_to_panel(type_panel, "Shine:", GLUI_SPINNER_FLOAT, &material_1_shininess);
	//The segments that are used for object rendering could not exceed the max value (-10,10)
	segment_spinner1->set_float_limits(-10.0, 10.0);
	segment_spinner2->set_float_limits(-10.0, 10.0);
	segment_spinner3->set_float_limits(-10.0, 10.0);
	//Reset Coord
	glui->add_separator_to_panel(type_panel);
	glui->add_checkbox("Reset Coordonates", &reset_value, RESET_ID1, control_reset);
	//glui->add_separator_to_panel(type_panel);
	//
	GLUI_Spinner *segment_spinner4 = glui->add_spinner_to_panel(type_panel, "Rotate Skull:", GLUI_SPINNER_FLOAT, &rotateXSkull);
	segment_spinner1->set_float_limits(-360.0, 360.0);
	GLUI_Spinner *segment_spinner5 = glui->add_spinner_to_panel(type_panel, "Scalate Skull:", GLUI_SPINNER_FLOAT, &scaleXSkull);
	segment_spinner1->set_float_limits(-2.0, 2.0);
	//glMultMatrixf(RotMatrix);
	GLUI_Panel *translate1 = glui->add_panel("Translate Skull");
	//glMultMatrixf(RotMatrix);
	GLUI_Translation *trans = glui->add_translation_to_panel(translate1, "Trans XY", GLUI_TRANSLATION_XY, &TransXYZ[0]);
	//GLUI_Rotation *rot = glui->add_rotation_to_panel(translate1, "Rotation", (float *)RotMatrix);
	//The rotMatrix is first initialized with the identity matrix glMultMatrixf(RotMatrix);
	glui->set_main_gfx_window(main_window);
	GLUI_Master.set_glutIdleFunc(myGlutIdleFunction);
	
	//Initialize some OpenGL parameters
	initOpenGL();
	//Starts the GLUT infinite loop
	glutMainLoop();
	return 0;
}