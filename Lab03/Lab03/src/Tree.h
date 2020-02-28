#pragma once
#include "glm\glm.hpp"
#include "glm\matrix.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
using namespace glm;
#include <vector>
using namespace std;

const vec3 BREAK(999);
const vec3 ENTER(99.f);
const vec3 START(1, 0, 0);
/*
This class is used for Generating Skeleton from a string
*/
class Tree
{
public:
	Tree(float angle, float width);
	~Tree();
	vector<vec3> v;

	const char* rule = "ff[+f[+f]-f]-f[+f]-f";//F-[[f]+f]+F[+Ff]-f
	float angle;
	float width;

	int i = 0;
	void DrawTree(float dir, vec3 lastPoint);
	vec3 GetNewVertex(vec3 v, float dir);
};


