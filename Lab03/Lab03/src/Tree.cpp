#include "Tree.h"



Tree::Tree(float angle, float width)
{
	v.push_back(vec3(0.5, 0, 0));
	this->angle = angle;
	this->width = width;
	DrawTree(0);
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
int i = 0;
void Tree::DrawTree(float dir) {

	float currentDir = dir;	
	for (i; i < strlen(rule); i++) {
		cout << rule[i];
		if (rule[i] == 'F' || rule[i] == 'f') {
			v.push_back(GetNewVertex(v.back(), currentDir));
		}
		else if (rule[i] == '+')
			currentDir += angle;
		else if (rule[i] == '-')
			currentDir -= angle;
		else if (rule[i] == '[') {
			i++;
			vec3 temp = v.back();
			DrawTree(currentDir);
			v.push_back(temp);
		}
		else if (rule[i] == ']') {
			//i++;
			v.push_back(BREAK);
			return;
		}
	}
	v.push_back(BREAK);
}



