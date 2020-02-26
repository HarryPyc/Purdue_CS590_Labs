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

class Tree
{
public:
	Tree(float angle, float width);
	~Tree();
	vector<vec3> v;

	const char* rule = "F-[[f]+f]+F[+Ff]-f";
	float angle;
	float width;
	void DrawTree(float dir);
	vec3 GetNewVertex(vec3 v, float dir);
};


