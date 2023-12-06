#pragma once
#include <vector>
#include "Point.h"
#include "Color.h"
#include "Light.h"
#include "Camera.h"
#include "Model.h"
#include "Ray.h"

bool isInside(Point& p, std::vector<Point>& polygon);
void drawTriangle(Point &a, Point &b, Point &c);
void drawRectangle(Point &a, Point &b, Point &c, Point &d);

Color phong(Light &light, Camera &camera, Model::Mesh &mesh);
Color getSpecular(GLfloat specular_light[], GLfloat specular_mesh[], Point light_vector, Point seeing_direction, Point mesh_normal);
Color getDiffuse(GLfloat diffuse_light[], GLfloat diffuse_mesh[], Point light_vector, Point mesh_normal);
Color getAmbient(GLfloat ambient_light[], GLfloat ambient_mesh[]);

int getSmallestNum(std::vector<float> list);
