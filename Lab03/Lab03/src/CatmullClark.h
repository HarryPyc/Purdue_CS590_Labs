#pragma once
#include "Tree.h"


class Surface
{
public:
	vector<vector<vec3>> v;
	Surface();
	~Surface();
	void GenerateSurfaceFromSkeleton(vector<vec3> v);
};

