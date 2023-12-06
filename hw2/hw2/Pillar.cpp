#include "Pillar.h"

Pillar::Pillar()
{
}

void Pillar::setPillar(Point base, float width, float height, GLfloat tcolor[])
{
	float x = base.posX();
	float y = base.posY();
	float z = base.posZ();

	this->points[0].setPoint(base);
	this->points[1].setPoint(x, y, z - width);
	this->points[2].setPoint(x + width, y, z - width);
	this->points[3].setPoint(x + width, y, z);
	this->points[4].setPoint(x, y + height, z);
	this->points[5].setPoint(x, y + height, z - width);
	this->points[6].setPoint(x + width, y + height, z - width);
	this->points[7].setPoint(x + width, y + height, z);

	for (int i = 0; i < 3; i++) {
		this->color[i] = tcolor[i];
	}
}


Pillar::~Pillar()
{
}
