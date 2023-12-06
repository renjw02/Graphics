#include <iostream>
#include "Ball.h"

Ball::Ball()
{
}

Ball::Ball(float x, float y, float z, float r, float sx, float sy, float sz, GLfloat tcolor[])
{
	this->position.setPoint(x, y, z);
	this->radius = r;
	this->speed.setPoint(sx, sy, sz);
	for (int i = 0; i < 3; i++) {
		this->color[i] = tcolor[i];
	}
}

Ball::~Ball()
{
}

void Ball::setBall(float x, float y, float z, float r, float sx, float sy, float sz, GLfloat tcolor[])
{
	this->position.setPoint(x, y, z);
	this->radius = r;
	this->speed.setPoint(sx, sy, sz);
	for (int i = 0; i < 3; i++) {
		this->color[i] = tcolor[i];
	}
	this->box_left_bottom_back = this->position - Point(r, r, r);
	this->box_right_top_front = this->position + Point(r, r, r);
}

void Ball::move(float time)
{
	Point new_x = this->position + this->speed * time;
	this->position.setPoint(new_x);
	this->box_left_bottom_back = this->position - Point(this->radius, this->radius, this->radius);
	this->box_right_top_front = this->position + Point(this->radius, this->radius, this->radius);
	/*std::cout << box_left_bottom_back.posX() << ' ' << box_left_bottom_back.posY() << ' ' << box_left_bottom_back.posZ() << std::endl;
	std::cout << box_right_top_front.posX() << ' ' << box_right_top_front.posY() << ' ' << box_right_top_front.posZ() << std::endl;*/

}

void Ball::collision(Pillar& p)
{
	// 三维的AABB算法
	// 柱体的两顶点应为points[1]和points[7]
	// 球的两顶点为pos+(-r, -r, -r)和pos+(r, r, r)
	bool collision_x = this->box_left_bottom_back.posX() <= p.getPoints()[7].posX()
		&& p.getPoints()[1].posX() <= this->box_right_top_front.posX();
	bool collision_y = this->box_left_bottom_back.posY() <= p.getPoints()[7].posY()
		&& p.getPoints()[1].posY() <= this->box_right_top_front.posY();
	bool collision_z = this->box_left_bottom_back.posZ() <= p.getPoints()[7].posZ()
		&& p.getPoints()[1].posZ() <= this->box_right_top_front.posZ();

	if (collision_x && collision_y && collision_z) {
		std::cout << "发生碰撞" << std::endl;
		this->speed = this->speed * -1;
	}
}
