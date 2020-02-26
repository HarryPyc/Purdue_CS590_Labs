#include "CatmullClark.h"



Surface::Surface()
{
}


Surface::~Surface()
{
}

void Surface::GenerateSurfaceFromSkeleton(vector<vec3> s)
{
	v.clear();
	vector<vec3>::iterator i = s.begin();
	vec3 v0(-1, 0, -1), v1(-1, 0, 1), v2(1, 0, 1), v3(1, 0, -1);
	const float scalar = 0.05f;
	vector<vec3> u;
	for (i; i != s.end(); i++) {
		u.clear();
		if (*i != BREAK) {
			u.push_back(v0*scalar + *i);
			u.push_back(v1*scalar + *i);
			u.push_back(v2*scalar + *i);
			u.push_back(v3*scalar + *i);
		}
		else
			u.push_back(BREAK);
		v.push_back(u);
	}
}
