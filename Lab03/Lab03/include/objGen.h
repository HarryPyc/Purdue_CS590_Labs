#pragma once



//#include "helper.h"
#include <vector>
#include <fstream> 
#include <vector> 
#include <GL/freeglut.h>
#include <memory.h>
#include <math.h>
#include "math/vect3d.h"
#include "math\triangle.h"
using namespace std;
#include "glm\glm.hpp"
using namespace glm;

void SaveOBJ(std::vector <TriangleC> *v, char *filename);


