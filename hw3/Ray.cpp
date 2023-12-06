#include "Ray.h"
#include "utils.h"
#include <limits.h>

void Ray::getIntersection(Model &model, int& i, float& t)
{
	float min_t = FLT_MAX;
	float min_id = -1;
	for (int j = 0; j < model.faceNum; j++)
	{
		Model::Mesh mesh = model.faceList[j];
		float current_t = -1;
		getIntersection(model.faceList[j], current_t);
		if (current_t > 0 && current_t < min_t)
		{
			min_t = current_t;
			min_id = j;
		}
	}
	if (min_id < 0)
	{
		i = -1;
		t = -1;
		return;
	}
	else {
		i = min_id;
		t = min_t;
	}
}

// 光线与面片求交
void Ray::getIntersection(Model::Mesh &m, float &t)
{
	float up = (m.normal * startPlace) - (m.v1 * m.normal);
	float down = (m.normal * direction);
	if (down == 0)
	{
		t = -1;
		return;
	}
	float get_t = -up / down;
	if (get_t <= 0)
	{
		t = -1;
		return;
	}
	Point intersection = startPlace + direction * get_t;
	std::vector<Point> triangle;
	triangle.push_back(m.v1);
	triangle.push_back(m.v2);
	triangle.push_back(m.v3);
	if (!isInside(intersection, triangle))
	{
		t = -1;
		return;
	}
	else {
		t = get_t;
	}
}

Ray Ray::getReflection(Model& m, int i, float t)
{
	Point intersection = startPlace + direction * t;
	float dist = sqrt(direction * m.faceList[i].normal);
	Point normal_speed = m.faceList[i].normal * dist;
	Point tangent_speed = direction - normal_speed;
	Point new_direction = tangent_speed - normal_speed;

	float new_intensity = intensity * m.faceList[i].reflection;

	return Ray(intersection, new_direction, new_intensity, refractionRate);
}

Ray Ray::getRefraction(Model& m, int i, float t)
{
	Point intersection = startPlace + direction * t;

	float dist = sqrt(direction * m.faceList[i].normal);
	Point normal_speed = m.faceList[i].normal * dist;
	Point tangent_speed = direction - normal_speed;

	float x = tangent_speed.getDist();
	float y = normal_speed.getDist();
	float n1 = refractionRate;
	float n2 = m.refractionRate;
	float k_x = n2 * y / sqrt(n1 * n1 * (x * x + y * y) - n2 * n2 * x * x);
	Point new_speed = normal_speed + tangent_speed * k_x;
	new_speed = new_speed / new_speed.getDist();

	float new_intensity = intensity * m.faceList[i].refraction;

	return Ray(intersection, new_speed, new_intensity, m.refractionRate);
}
