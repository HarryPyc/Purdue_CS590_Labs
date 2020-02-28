#include "CatmullClark.h"

Edge::Edge(Point *a, Point *b) {
	this->a = a;
	this->b = b;
	a->edges.push_back(this);
	b->edges.push_back(this);
	midPoint = (a->p + b->p) / 2.f;
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
		R += edges[i]->midPoint;
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

	GenerateSurface(&v);
}


Surface::~Surface()
{
}

Cube::Cube(vec3 center)
{
	this->center = center;
	p0 = new Point(v0*scalar + center);
	p1 = new Point(v1*scalar + center);
	p2 = new Point(v2*scalar + center);
	p3 = new Point(v3*scalar + center);
	q0 = new Point(v4*scalar + center);
	q1 = new Point(v5*scalar + center);
	q2 = new Point(v6*scalar + center);
	q3 = new Point(v7*scalar + center);
	e0 = new Edge(p0, p1); e1 = new Edge(p1, p2); e2 = new Edge(p3, p2); e3 = new Edge(p0, p3);
	l0 = new Edge(p0, q0); l1 = new Edge(p1, q1); l2 = new Edge(p2, q2); l3 = new Edge(p3, q3);
	t0 = new Edge(q0, q1); t1 = new Edge(q1, q2); t2 = new Edge(q3, q2); t3 = new Edge(q0, q3);

	f4 = new Face(e1, l2, t1, l1);
	f5 = new Face(e3, l3, t3, l0);
}

void Cube::GenerateFace()
{
	if(Up)
		f2 = new Face(t0, t1, t2, t3);
	if(Down)
		f0 = new Face(e0, e1, e2, e3);
	if(Right)
		f3 = new Face(e2, l2, t2, l3);
	if(Left)
		f1 = new Face(e0, l1, t0, l0);
}

//push all available primitives into Surface
void Surface::PushCube(Cube * c)
{
	points.push_back(c->p0); points.push_back(c->p1); points.push_back(c->p2); points.push_back(c->p3);
	points.push_back(c->q0); points.push_back(c->q1); points.push_back(c->q2); points.push_back(c->q3);
	edges.push_back(c->e0); edges.push_back(c->e1); edges.push_back(c->e2); edges.push_back(c->e3);
	edges.push_back(c->l0); edges.push_back(c->l1); edges.push_back(c->l2); edges.push_back(c->l3);
	edges.push_back(c->t0); edges.push_back(c->t1); edges.push_back(c->t2); edges.push_back(c->t3);
	if(c->Down)  faces.push_back(c->f0); 
	if(c->Left)	 faces.push_back(c->f1); 
	if(c->Up)    faces.push_back(c->f2);
	if(c->Right) faces.push_back(c->f3); 
	faces.push_back(c->f4); faces.push_back(c->f5);
}
//Connect two cubes' edges and create 4 faces for the new cube.
void Surface::ConnectCube(Cube * c1, Cube * c2)
{
	vec3 dir = c2->center - c1->center;
	if (abs(dir.y) > abs(dir.x)) {
		if (dir.y > 0) {
			Edge *E0 = new Edge(c1->q0, c2->p0),
				*E1 = new Edge(c1->q1, c2->p1),
				*E2 = new Edge(c1->q2, c2->p2),
				*E3 = new Edge(c1->q3, c2->p3);
			edges.push_back(E0); edges.push_back(E1); edges.push_back(E2); edges.push_back(E3);
			Face *f0 = new Face(c1->t0, E1, c2->e0, E0),
				*f1 = new Face(c1->t1, E2, c2->e1, E1),
				*f2 = new Face(c1->t2, E2, c2->e2, E3),
				*f3 = new Face(c1->t3, E3, c2->e3, E0);
			faces.push_back(f0); faces.push_back(f1); faces.push_back(f2); faces.push_back(f3);
			c1->Up = false;
			c2->Down = false;
		}
		else
			ConnectCube(c2, c1);
		
	}
	else if(abs(dir.y) <= abs(dir.x)) {
		if (dir.x > 0) {
			Edge *E0 = new Edge(c1->p2, c2->p1),
				*E1 = new Edge(c1->q2, c2->q1),
				*E2 = new Edge(c1->q3, c2->q0),
				*E3 = new Edge(c1->p3, c2->p0);
			edges.push_back(E0); edges.push_back(E1); edges.push_back(E2); edges.push_back(E3);
			Face *f0 = new Face(c1->e2, E0, c2->e0, E3),
				*f1 = new Face(c1->l2, E1, c2->l1, E0),
				*f2 = new Face(c1->t2, E1, c2->t0, E2),
				*f3 = new Face(c1->l3, E2, c2->l0, E3);
			faces.push_back(f0); faces.push_back(f1); faces.push_back(f2); faces.push_back(f3);
			c1->Right = false;
			c2->Left = false;
		}
		else
			ConnectCube(c2, c1);
	}	
}

//Recursive to Generate cubes in a form of binary branches.
void Surface::GenerateCubes(vector<vec3> *v, vector<vec3>::iterator &it, Cube* lastCube)
{
	Cube* c1 = lastCube;
	for (it; it < v->end(); it++) {
		if (*it == ENTER) {
			GenerateCubes(v, ++it, c1);
		}
		else if (*it == BREAK) {
			return;
		}
		else {
			Cube *cube = new Cube(*it);
			cubes.push_back(cube);
			ConnectCube(c1, cube);
			c1 = cube;
		}
	}
}

//push back all primitives from cubes into Surface
void Surface::GenerateSurface(vector<vec3> *v)
{
	vector<vec3>::iterator it = v->begin();
	Cube *cube = new Cube(*it);
	cubes.push_back(cube);
	GenerateCubes(v, ++it, cube);
	vector<Cube*>::iterator itc = cubes.begin();
	for (itc; itc < cubes.end(); itc++) {
		(*itc)->GenerateFace();
		PushCube(*itc);
	}
}


Surface* Surface::CatmullClarkSubdivision() {
	
	Surface* result = new Surface();
	//the following three steps go throught all original primitives and 
	//generate new points. And push them back to new Surface we created
	//to store the results.
	//Note: Face must be the first one
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

	//Goes through all original edges, each edge should generate two
	//new edges, push them back to result
	for (ite = edges.begin(); ite < edges.end(); ite++) {
		(*ite)->GenerateNewSurface();
		result->edges.push_back((*ite)->nea);
		result->edges.push_back((*ite)->neb);
	}
	//Goes through all original faces, each face should generate four
	//new faces and edges. Push them back to results.
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


