#include "Tree.h"



Tree::Tree(float angle, float width)
{
	v.clear();
	v.push_back(START);
	this->angle = angle;
	this->width = width;
	DrawTree(0, START);
}


Tree::~Tree()
{
}
vec3 Tree::GetNewVertex(vec3 v, float dir) {
	mat4 R = glm::rotate(radians(dir), vec3(0, 0, 1));
	vec3 d = vec3(R * vec4(0, 1, 0, 0));
	d = normalize(d)*width;
	return v + d;
}


void Tree::DrawTree(float dir, vec3 lastPoint) {

	float currentDir = dir;	
	vec3 p = lastPoint;
	for (i; i < strlen(rule); i++) {
		cout << rule[i];
		if (rule[i] == 'F' || rule[i] == 'f') {
			p = GetNewVertex(p, currentDir);
			v.push_back(p);
		}
		else if (rule[i] == '+')
			currentDir += angle;
		else if (rule[i] == '-')
			currentDir -= angle;
		else if (rule[i] == '[') {
			i++;
			v.push_back(ENTER);
			DrawTree(currentDir, p);
		}
		else if (rule[i] == ']') {
			//i++;
			v.push_back(BREAK);
			return;
		}
	}
	v.push_back(BREAK);
}




