#pragma once
#include "Tree.h"
class Edge;
class Point;
class Face;

class Point {
public:
	Point(vec3 p);
	vec3 p;
	vector<Face*> faces;
	vector<Edge*> edges;
	Point* newPoint;
	void GetNewPoint();

};
class Edge {
public:
	Edge(Point *a, Point *b);
	Point *a, *b;
	vector<Face*> faces;
	Point *newEdgePoint;
	//new Edges
	Edge *nea, *neb;
	vec3 GetMidPoint();
	void GetNewEdgePoint();
	void GenerateNewSurface();
};
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
	void GenerateNewSurface();
};


class Surface
{
public:
	//vector<vector<vec3>> v;
	vector<Point*> points;
	vector<Face*> faces;
	vector<Edge*> edges;
	Surface();
	Surface(vector<vec3> v);
	~Surface();
	void GenerateSurfaceFromSkeleton(vector<vec3> v);
	Surface* CatmullClarkSubdivision();

};



