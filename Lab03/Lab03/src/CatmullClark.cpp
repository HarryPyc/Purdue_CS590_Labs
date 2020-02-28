#include "CatmullClark.h"

Edge::Edge(Point *a, Point *b) {
	this->a = a;
	this->b = b;
	a->edges.push_back(this);
	b->edges.push_back(this);
	
}

void Edge::GetNewEdgePoint() {
	vec3 result = (faces[0]->newFacePoint->p + faces[1]->newFacePoint->p
		+ a->p + b->p) / 4.f;
	newEdgePoint = new Point(result);
}
void Edge::GenerateNewSurface()
{
	nea = new Edge(a->newPoint, newEdgePoint);
	neb = new Edge(b->newPoint, newEdgePoint);
}
vec3 Edge::GetMidPoint() {
	return (a->p + b->p) / 2.f;
}

Face::Face(Edge *e0, Edge *e1, Edge *e2, Edge *e3){
	this->e0 = e0;
	this->e1 = e1;
	this->e2 = e2;
	this->e3 = e3;
	e0->faces.push_back(this);
	e1->faces.push_back(this);
	e2->faces.push_back(this);
	e3->faces.push_back(this);
	e0->a->faces.push_back(this);
	e0->b->faces.push_back(this);
	e2->a->faces.push_back(this);
	e2->b->faces.push_back(this);

}
void Face::GetNewFacePoint() {
	vec3 result = (e0->a->p + e0->b->p + e2->a->p + e2->b->p) / 4.f;
	newFacePoint = new Point(result);
}

void Face::GenerateNewSurface()
{
	ne0 = new Edge(e0->newEdgePoint, newFacePoint);
	ne1 = new Edge(e1->newEdgePoint, newFacePoint);
	ne2 = new Edge(e2->newEdgePoint, newFacePoint);
	ne3 = new Edge(e3->newEdgePoint, newFacePoint);

	nf0 = new Face(e1->nea, ne1, ne0, e0->neb);
	nf1 = new Face(e2->neb, ne2, ne1, e1->neb);
	nf2 = new Face(e3->neb, ne3, ne2, e2->nea);
	nf3 = new Face(e0->nea, ne0, ne3, e3->nea);
}


Point::Point(vec3 p) {
	this->p = p;
}
void Point::GetNewPoint()
{
	vec3 R(0), F(0);
	int n = faces.size();
	if (n != edges.size()) {
		cout << "error: faces size doesn't match edges size" << endl;
		return;
	}
	for (int i = 0; i < n; i++) {
		F += faces[i]->newFacePoint->p;
		R += edges[i]->GetMidPoint();
	}
	float N = n;
	vec3 result = (F/N + 2.f * R/N + (N - 3.f)*p) / N;

	newPoint = new Point(result);
}




Surface::Surface()
{
	edges.clear();
	faces.clear();
	points.clear();
}

Surface::Surface(vector<vec3> v)
{
	GenerateSurfaceFromSkeleton(v);
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

	vec3 v0(-1, 0, -1), v1(-1, 0, 1), v2(1, 0, 1), v3(1, 0, -1);
	Point *p0, *p1, *p2, *p3, *q0, *q1, *q2, *q3;
	Edge *e0, *e1, *e2, *e3, *l0, *l1, *l2, *l3, *t0, *t1, *t2, *t3;
	const float scalar = 0.2f;
	for (i; i < s.end() - 1; i++) {
		if (*i == BREAK) {
			continue;
		}


		if (i == s.begin()) {
			
			p0 = new Point(v0 * scalar + *i);
			p1 = new Point(v1 * scalar + *i);
			p2 = new Point(v2 * scalar + *i);
			p3 = new Point(v3 * scalar + *i);
			e0 = new Edge(p0, p1);
			e1 = new Edge(p1, p2);
			e2 = new Edge(p3, p2);
			e3 = new Edge(p0, p3);

			edges.push_back(e0);
			edges.push_back(e1);
			edges.push_back(e2);
			edges.push_back(e3);
			points.push_back(p0);
			points.push_back(p1);
			points.push_back(p2);
			points.push_back(p3);

		}
		else{
			p0 = q0;
			p1 = q1;
			p2 = q2;
			p3 = q3;
			e0 = t0;
			e1 = t1;
			e2 = t2;
			e3 = t3;
		}

		if (*(i + 1) != BREAK && *(i + 1) != ENTER) {
			q0 = new Point(v0 * scalar + *(i + 1)); 
			q1 = new Point(v1 * scalar + *(i + 1));
			q2 = new Point(v2 * scalar + *(i + 1));
			q3 = new Point(v3 * scalar + *(i + 1));

			l0 = new Edge(p0, q0); 
			l1 = new Edge(p1, q1);
			l2 = new Edge(p2, q2);
			l3 = new Edge(p3, q3);

			t0 = new Edge(q0, q1); 
			t1 = new Edge(q1, q2);
			t2 = new Edge(q3, q2);
			t3 = new Edge(q0, q3);

			points.push_back(q0);
			points.push_back(q1);
			points.push_back(q2);
			points.push_back(q3);
			edges.push_back(l0);
			edges.push_back(l1);
			edges.push_back(l2);
			edges.push_back(l3);
			edges.push_back(t0);
			edges.push_back(t1);
			edges.push_back(t2);
			edges.push_back(t3);

			Face *f0 = new Face(e0, l1, t0, l0),
				 *f1 = new Face(e1, l2, t1, l1),
			 	 *f2 = new Face(e2, l2, t2, l3), 
				 *f3 = new Face(e3, l3, t3, l0);
			faces.push_back(f0);
			faces.push_back(f1);
			faces.push_back(f2);
			faces.push_back(f3);
		}
		if (i == s.begin() || ( *(i + 1) == BREAK)) {
			Face *f = new Face(e0, e1, e2, e3);
			faces.push_back(f);
		}

	}
}

Surface* Surface::CatmullClarkSubdivision() {
	
	Surface* result = new Surface();
	vector<Face*>::iterator itf = faces.begin();
	for (itf; itf < faces.end(); itf++) {
		(*itf)->GetNewFacePoint();
		result->points.push_back((*itf)->newFacePoint);
	}
	vector<Edge*>::iterator ite = edges.begin();
	for (ite; ite < edges.end(); ite++) {
		(*ite)->GetNewEdgePoint();
		result->points.push_back((*ite)->newEdgePoint);
	}
	vector<Point*>::iterator itp = points.begin();
	for (itp; itp < points.end(); itp++) {
		(*itp)->GetNewPoint();
		result->points.push_back((*itp)->newPoint);
	}

	for (ite = edges.begin(); ite < edges.end(); ite++) {
		(*ite)->GenerateNewSurface();
		result->edges.push_back((*ite)->nea);
		result->edges.push_back((*ite)->neb);
	}
	for (itf = faces.begin(); itf < faces.end(); itf++) {
		(*itf)->GenerateNewSurface();
		result->edges.push_back((*itf)->ne0);
		result->edges.push_back((*itf)->ne1);
		result->edges.push_back((*itf)->ne2);
		result->edges.push_back((*itf)->ne3);

		result->faces.push_back((*itf)->nf0);
		result->faces.push_back((*itf)->nf1);
		result->faces.push_back((*itf)->nf2);
		result->faces.push_back((*itf)->nf3);
	}
	return result;
}


