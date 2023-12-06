#pragma once
#include <GL\glut.h>
#include "Point.h"

class Pillar
{
	Point points[8];	// �������ϣ�˳ʱ�룬���¿�ʼ
	GLfloat color[3] = { 0, 0, 0 };

public:
	Pillar();
	~Pillar();
	void setPillar(Point base, float width, float height, GLfloat tcolor[]);
	Point* getPoints() {
		return this->points;
	}
	GLfloat* getColor() {
		return this->color;
	}
};

