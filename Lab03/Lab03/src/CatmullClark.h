#pragma once
#include "Tree.h"
class Edge;
class Point;
class Face;

class Point {
public:
	Point();
	Point(vec3 p);
	vec3 p;
	vector<Face> faces;
	vector<Edge> edges;
	vec3 GetNewVertexPoint();
};
class Edge {
public:
	Edge(Point *a, Point *b);
	Edge();
	Point *a, *b;
	vector<Face> faces;
	vec3 GetNewEdgePoit(Face f1, Face f2);
	vec3 GetMidPoint();
};
class Face {
public:
	Edge *e0, *e1, *e2, *e3;
	Face(Edge *e0, Edge *e1, Edge *e2, Edge *e3);
	vec3 GetNewFacePoint();
};


class Surface
{
public:
	//vector<vector<vec3>> v;
	vector<Point> points;
	vector<Face> faces;
	vector<Edge> edges;
	Surface();
	~Surface();
	void GenerateSurfaceFromSkeleton(vector<vec3> v);
	Surface CatmullClarkSubdivision();

};



