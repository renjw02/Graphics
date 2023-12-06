#pragma once
#include <GL/glut.h>
#include <math.h>
#include "Point.h"
#include "Pillar.h"

class Ball
{
	Point position;
	Point speed;
	float radius;
	
	
	GLfloat color[3] = { 0, 0, 0 };

public:
	Point box_left_bottom_back;
	Point box_right_top_front;
	Ball();
	Ball(float x, float y, float z, float r, float sx, float sy, float sz, GLfloat color[]);
	~Ball();

	void setBall(float x, float y, float z, float r, float sx, float sy, float sz, GLfloat tcolor[]);
	void move(float time);
	void collision(Pillar& p);

	float dist(Point p) {
		return sqrt((p - this->position) * (p - this->position));
	}

	Point getPos() {
		return this->position;
	}

	float getRadius() {
		return this->radius;
	}
};

