/*********************************/
/* CS 590CGS Lab framework        */
/* (C) Bedrich Benes 2020        */
/* bbenes ~ at ~ purdue.edu      */
/* Press +,- to add/remove points*/
/*       r to randomize          */
/*       s to change rotation    */
/*       c to render curve       */
/*       t to render tangents    */
/*       p to render points      */
/*       s to change rotation    */
/*********************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <string>
#include <vector>			//Standard template library class
#include <GL/freeglut.h>
#include "glm\glm.hpp"
#include "Tree.h"
//in house created libraries
#include "trackball.h"
#include "CatmullClark.h"
#define Orange vec3(1.0,0.6,0.0)
#define Green vec3(0,1,0)
#pragma warning(disable : 4996)
#pragma comment(lib, "freeglut.lib")
using namespace glm;
using namespace std;

//some trackball variables -> used in mouse feedback
TrackBallC trackball;
bool mouseLeft, mouseMid, mouseRight;


GLuint points=0;  //number of points to display the object
int steps=20;     //# of subdivisions
bool needRedisplay=false;
GLfloat  Sign=+1; //diretcion of rotation
const GLfloat defaultIncrement=0.7f; //speed of rotation
GLfloat  angleIncrement=defaultIncrement;

int times = 0;
float angle = 30.f, width = 1.0f;
vector<vec3> v;
Surface *surf;
//window size
GLint wWindow=1200;
GLint hWindow=800;

//this defines what will be rendered
//see Key() how is it controlled
bool tangentsFlag = false;
bool pointsFlag = false;
bool curveFlag = true;

/*********************************
Some OpenGL-related functions DO NOT TOUCH
**********************************/
//displays the text message in the GL window
void GLMessage(char *message)
{
	static int i;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.f, 100.f, 0.f, 100.f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(0, 0, 255);
	glRasterPos2i(10, 10);
	for (i = 0; i<(int)strlen(message); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

//called when a window is reshaped
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);
	//remember the settings for the camera
	wWindow = w;
	hWindow = h;
}

//Some simple rendering routines using old fixed-pipeline OpenGL
//draws line from a to b with color 
void DrawLine(vec3 a, vec3 b, vec3 color) {

	glColor3fv(&color[0]);
	glBegin(GL_LINES);
		glVertex3fv(&a[0]);
		glVertex3fv(&b[0]);
	glEnd();
}

//draws point at a with color 
void DrawPoint(vec3 a, vec3 color) {

	glColor3fv(&color[0]);
	glPointSize(5);
	glBegin(GL_POINTS);
	 glVertex3fv(&a[0]);
	glEnd();
}

void DrawSurf(Surface *surf) {

	vector<Edge*>::iterator it = surf->edges.begin();
	for (it; it != surf->edges.end(); it++) {
		DrawLine((*it)->a->p, (*it)->b->p, Orange);
		//DrawPoint(it->a->p, Orange);
	}
}

/**********************
LAB related MODIFY
***********************/

//This defines the actual curve 
inline vec3 P(GLfloat t)
{
	const float rad = 0.2f;
	const float height = 1.f;
	const float rot = 5.f;
	return vec3(rad*(float)sin(rot*M_PI*t),height*t,rad*(float)cos(rot*M_PI*t)); //spiral with radius rad, height, and rotations rot
}


void InitSurf() {
	Tree *tree = new Tree(angle, width);
	surf = new Surface(tree->v);
}

//returns random number from <-1,1>
inline float random11() { 
	return 2.f*rand() / (float)RAND_MAX - 1.f;
}

//randomizes an existing curve by adding random number to each coordinate
void Randomize(vector <vec3> *a) {
	const float intensity = 0.01f;
	for (unsigned int i = 0; i < a->size(); i++) {
		vec3 r(random11(), random11(), random11());
		a->at(i) = a->at(i) + intensity*r;
	}
}

//display coordinate system
void CoordSyst() {
	vec3 a, b, c;
	vec3 origin(0, 0, 0);
	vec3 red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	//draw the coordinate system 
	a = vec3(1, 0, 0);
	b = vec3(0, 1, 0);
	c = vec3(cross(a, b)); //use cross product to find the last vector
	glLineWidth(4);
	DrawLine(origin, a, red);
	DrawLine(origin, b, green);
	DrawLine(origin, c, blue);
	glLineWidth(1);

}

//this is the actual code for the lab
void Lab01() {
	vec3 a,b,c;
	vec3 origin(0, 0, 0);
	vec3 red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	DrawSurf(surf);
	CoordSyst();
}

//the main rendering function
void RenderObjects()
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//set camera
	glMatrixMode(GL_MODELVIEW);
	trackball.Set3DViewCamera();
	//call the student's code from here
	Lab01();
}

//Add here if you want to control some global behavior
//see the pointFlag and how is it used
void Kbd(unsigned char a, int x, int y)//keyboard callback
{
	switch (a)
	{
	case 27: exit(0); break;
	case 't': tangentsFlag = !tangentsFlag; break;
	case 'p': pointsFlag = !pointsFlag; break;
	case 'c': curveFlag = !curveFlag; break;
	case 's': {Sign = -Sign; break; }
	case '-': {
		steps--;

		break;
	}
	case '+': {
		steps++;

		break;
	}
	case 'r': {
		Randomize(&v);
		break;
	}
	case 32: {
		surf = surf->CatmullClarkSubdivision();
		break;
	}
	}
	glutPostRedisplay();
}


/*******************
OpenGL code. Do not touch.
******************/
void Idle(void)
{
	glutPostRedisplay();
}

void Display(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLMessage("Lab 2 - CS 590CGS");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (GLfloat)wWindow / (GLfloat)hWindow, 0.01, 100); //set the camera
	glMatrixMode(GL_MODELVIEW); //set the scene
	glLoadIdentity();
	gluLookAt(0, 10, 10, 0, 0, 0, 0, 1, 0); //set where the camera is looking at and from. 
	static GLfloat angle = 0;
	angle += angleIncrement;
	if (angle >= 360.f) angle = 0.f;
	glRotatef(Sign*angle, 0, 1, 0);
	RenderObjects();
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseLeft = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		trackball.Set(false, x, y);
		mouseLeft = false;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseMid = true;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		trackball.Set(true, x, y);
		mouseMid = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseRight = true;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		trackball.Set(true, x, y);
		mouseRight = false;
	}
}

void MouseMotion(int x, int y) {
	if (mouseLeft)  trackball.Rotate(x, y);
	if (mouseMid)   trackball.Translate(x, y);
	if (mouseRight) trackball.Zoom(x, y);
	glutPostRedisplay();
}


int main(int argc, char **argv)
{ 
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(500,100);
  glutCreateWindow("Surface of Revolution");
  //GLenum err = glewInit();
  // if (GLEW_OK != err){
  // fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  //}
 /* Tree tree(30, 1.0);
  v = tree.v;
  surf = new Surface(tree.v);*/
  InitSurf();

  glutDisplayFunc(Display);
  glutIdleFunc(Idle);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Kbd); //+ and -
  glutSpecialUpFunc(NULL); 
  glutSpecialFunc(NULL);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMotion);

  glutMainLoop();
	
  return 0;        
}
