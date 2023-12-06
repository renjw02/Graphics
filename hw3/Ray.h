#pragma once
#include "Point.h"
#include "Model.h"

class Ray
{
public:
	Point startPlace;
	Point direction;
	float intensity;
	float refractionRate;

	Ray() {}
	Ray(Point start, Point direction, float intensity, float refraction) :
		startPlace(start), direction(direction), intensity(intensity), refractionRate(refraction) {}

	void getIntersection(Model &model, int &i, float &t);
	void getIntersection(Model::Mesh &m, float &t);
	Ray getReflection(Model& m, int i, float t);
	Ray getRefraction(Model& m, int i, float t);
};

