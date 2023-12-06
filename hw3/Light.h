#pragma once
#include <GL/glut.h>
#include "Point.h"

class Light {
public:
	Point position;

	void setPlace(Point p) {
		this->position = p;
	}
};