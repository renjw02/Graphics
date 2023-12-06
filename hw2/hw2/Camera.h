#pragma once
#include "Point.h"

class Camera
{
	Point pos;	// �������������ϵ��λ��
	Point lookAt;	// �ӵ�
	float speed; 
	float radius;	// ˮƽ�ƶ�
	float height;
	float arc;		// ŷ����

public:
	Camera();
	Camera(float r, float h);
	void move();
	void horizentalMove(int direction);
	void mouseMove(int x, int y, int& fx, int& fy);

	Point getPos() {
		return pos;
	}

	Point getLookAt() {
		return lookAt;
	}
};

