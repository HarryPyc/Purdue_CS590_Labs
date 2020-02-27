#include "CatmullClark.h"

Edge::Edge(Point *a, Point *b) {
	this->a = a;
	this->b = b;
	a->edges.push_back(*this);
	b->edges.push_back(*this);
	
}
Edge::Edge()
{
}
vec3 Edge::GetNewEdgePoit(Face f1, Face f2) {
	return (f1.GetNewFacePoint() + f2.GetNewFacePoint() + a->p + b->p) / 4.f;
}
vec3 Edge::GetMidPoint() {
	return vec3((a->p + b->p) / 2.f);
}

Face::Face(Edge *e0, Edge *e1, Edge *e2, Edge *e3){
	this->e0 = e0;
	this->e1 = e1;
	this->e2 = e2;
	this->e3 = e3;
	e0->faces.push_back(*this);
	e1->faces.push_back(*this);
	e2->faces.push_back(*this);
	e3->faces.push_back(*this);
	e0->a->faces.push_back(*this);
	e0->b->faces.push_back(*this);
	e2->a->faces.push_back(*this);
	e2->b->faces.push_back(*this);

}
vec3 Face::GetNewFacePoint() {
	return vec3();
}

Point::Point()
{
}

Point::Point(vec3 p) {
	this->p = p;
}
vec3 Point::GetNewVertexPoint()
{
	return vec3(0);
}

Surface::Surface()
{
}


Surface::~Surface()
{
}

void Surface::GenerateSurfaceFromSkeleton(vector<vec3> s)
{
	edges.clear();
	faces.clear();
	points.clear();
	vector<vec3>::iterator i = s.begin();
	int itp = 0, ite = 0;
	vec3 v0(-1, 0, -1), v1(-1, 0, 1), v2(1, 0, 1), v3(1, 0, -1);
	const float scalar = 0.1f;
	for (i; i < s.end() - 1; i++) {
		if (*i == BREAK) {
			continue;
		}
		Point p0(v0 * scalar + *i), p1(v1 * scalar + *i),
			  p2(v2 * scalar + *i), p3(v3 * scalar + *i);
		if (i == s.begin() || *(i - 1) == BREAK) {
			points.push_back(p0);
			points.push_back(p1);
			points.push_back(p2);
			points.push_back(p3);
			Edge e0(&points[itp], &points[itp + 1]), e1(&points[itp + 1], &points[itp + 2]),
				 e2(&points[itp + 2], &points[itp + 3]), e3(&points[itp + 3], &points[itp]);
			itp += 4;
			edges.push_back(e0);
			edges.push_back(e1);
			edges.push_back(e2);
			edges.push_back(e3);

		}
		if (*(i + 1) == BREAK || i == s.begin()) {
			/*Face f(&edges[ite], &edges[ite + 1], &edges[ite + 2], &edges[ite + 3]);
			faces.push_back(f);*/
		}
		if (*(i + 1) != BREAK) {
			Point q0(v0 * scalar + *(i + 1)), q1(v1 * scalar + *(i + 1)),
				  q2(v2 * scalar + *(i + 1)), q3(v3 * scalar + *(i + 1));
			points.push_back(q0);
			points.push_back(q1);
			points.push_back(q2);
			points.push_back(q3);

			Edge l0(&points[itp - 4], &points[itp]), l1(&points[itp - 3], &points[itp + 1]),
			 	 l2(&points[itp - 2], &points[itp + 2]), l3(&points[itp - 1], &points[itp + 3]),
				 t0(&points[itp], &points[itp + 1]), t1(&points[itp + 1], &points[itp + 2]),
				 t2(&points[itp + 2], &points[itp + 3]), t3(&points[itp + 3], &points[itp]);
			itp += 4;
			edges.push_back(l0);
			edges.push_back(l1);
			edges.push_back(l2);
			edges.push_back(l3);
			edges.push_back(t0);
			edges.push_back(t1);
			edges.push_back(t2);
			edges.push_back(t3);
		/*	Face f0(&e0, &l0, &t0, &l1),
				 f1(&e1, &l1, &t1, &l2),
			 	 f2(&e2, &l2, &t2, &l3), 
				 f3(&e3, &l3, &t3, &l0);
			faces.push_back(f0);
			faces.push_back(f1);
			faces.push_back(f2);
			faces.push_back(f3);*/
		}

	}
}

Surface Surface::CatmullClarkSubdivision() {
	return Surface();
}


