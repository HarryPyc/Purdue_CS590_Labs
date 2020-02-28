#pragma once
#include "Tree.h"
const vec3 v0(-1, -1, -1), v1(-1, -1, 1), v2(1, -1, 1), v3(1, -1, -1),
           v4(-1, 1, -1), v5(-1, 1, 1), v6(1, 1, 1), v7(1, 1, -1);
const float scalar = 0.1f;
class Edge;
class Point;
class Face;

class Point {
public:
	Point(vec3 p);
	vec3 p;
	vector<Face*> faces;//neighbor faces
	vector<Edge*> edges;//neighbor edges
	Point* newPoint;    
	void GetNewPoint();

};
class Edge {
public:
	Edge(Point *a, Point *b);
	Point *a, *b; //two end points
	vector<Face*> faces;//neighbor faces
	Point *newEdgePoint;
	//new Edges, 'n' means new
	Edge *nea, *neb;
	vec3 GetMidPoint();
	void GetNewEdgePoint();
	void GenerateNewSurface(); //Generate two edges for each original edge
};
/*
Because I'm dumb, Faces must be formed with a specific order of four edges.
So that's why you may see some strangely inversed order while I create a Face.
But it works!!!
*/
class Face {
public:
	Edge *e0, *e1, *e2, *e3;
	Face(Edge *e0, Edge *e1, Edge *e2, Edge *e3);
	Point* newFacePoint;
	//new Edges
	Edge *ne0, *ne1, *ne2, *ne3;
	//new Faces
	Face *nf0, *nf1, *nf2, *nf3;
	void GetNewFacePoint();
	//Generate four faces and four edges for each original face
	void GenerateNewSurface();
	                           
};

struct Cube {
	Cube(vec3 center);
	vec3 center;
	//eight vertices
	Point *p0, *p1, *p2, *p3,
		*q0, *q1, *q2, *q3; 
	//12 edges
	Edge *e0, *e1, *e2, *e3,
		*l0, *l1, *l2, *l3,
		*t0, *t1, *t2, *t3;
	//6 faces
	//down:0, left:1. up:2, right:3, front:4, back:5
	//positive z is front, positive x is right
	Face *f0, *f1, *f2, *f3, *f4, *f5;
	//decide which face we should draw
	bool Up = true, Down = true, Left = true, Right = true;
	void GenerateFace();
};

class Surface
{
public:
	//vector<vector<vec3>> v;
	vector<Point*> points;
	vector<Face*> faces;
	vector<Edge*> edges;
	vector<Cube*> cubes;

	Surface();
	Surface(vector<vec3> v);
	~Surface();

	void PushCube(Cube *c);//push all available primitives into Surface
	//Connect two cubes' edges and create 4 faces for the new cube.
	void ConnectCube(Cube* c1, Cube* c2);
	//Recursive to Generate cubes in a form of binary branches.
	void GenerateCubes(vector<vec3> *v, vector<vec3>::iterator & it, Cube* lastCube);
	//push back all primitives from cubes into Surface
	void GenerateSurface(vector<vec3> *v);

	Surface* CatmullClarkSubdivision();

};



