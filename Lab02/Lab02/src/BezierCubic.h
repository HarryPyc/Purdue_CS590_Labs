#pragma once

#include "glm\glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm\gtx\norm.hpp"
#include <vector>
#include <algorithm>
#define MAX_ERROR 1.0f
using namespace glm;
using namespace std;

class BezierCubic
{
public:
	BezierCubic();
	BezierCubic(vec3 p0, vec3 p1, vec3 p2, vec3 p3, int n);
	~BezierCubic();
	vec3 p0, p1, p2, p3;
	int n;
	int start, end;
	vector<vec3> b;

	float B0(float t) { return pow(1 - t, 3); }
	float B1(float t) { return 3 * t*(1 - t)*(1 - t); }
	float B2(float t) { return 3 * t*t*(1 - t); }
	float B3(float t) { return t * t*t; }

	vec3 GetBezierPoint(float t);
	vec3 GetBezier(float t);
	vec3 GetD1Bezier(float t);
	vec3 GetD2Bezier(float t);

	void InitBezierCubic();
};

const int max_iteration = 10;
struct Error {
	int index;
	float error;
};


BezierCubic C1BezierCubic(BezierCubic *b1, vec3 p2, vec3 p3, int n);
BezierCubic G1BezierCubic(BezierCubic *b1, vec3 p2, vec3 p3, int n, float k);

void BezierApproximation(vector<vec3> *points, vector<BezierCubic> *beziers, int num);