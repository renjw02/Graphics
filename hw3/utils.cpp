#include "utils.h"
#include <math.h>
#include <vector>


Color phong(Light& light, Camera &camera, Model::Mesh &mesh)
{
	Point d = camera.getLookAt() - camera.getPos();
	d = d / d.getDist();

	Point l = light.position - mesh.center;
	l = l / l.getDist();

	GLfloat tmp[3] = { 1.0, 1.0, 1.0 };
	Color ambient = getAmbient(tmp, mesh.ambient);
	Color diffuse = getDiffuse(tmp, mesh.diffuse, l, mesh.normal);
	Color specular = getSpecular(tmp, mesh.specular, l, d, mesh.normal);
	Color phong = ambient + diffuse + specular;
	return phong;
}

bool isInside(Point& p, std::vector<Point>& polygon)
{
	float sum_arc = 0;
	int n = polygon.size();
	for (int i = 0; i < n; i++)
	{
		Point current = polygon[i];
		Point next = polygon[(i+1) % n];

		float a = (current - p).getDist();
		float b = (next - p).getDist();
		float c = (current - next).getDist();
		float up = a * a + b * b - c * c;
		float down = 2 * a * b;
		if (down == 0) {
			return 0;
		}
		float arc = acos(up / down);
		sum_arc += arc;
	}
	if (abs(sum_arc - 2 * 3.1415926) < 0.01) return 1;
	else return 0;
}

void drawTriangle(Point &a, Point &b, Point &c)
{
	glBegin(GL_POLYGON);
	glVertex3f(a.posX(), a.posY(), a.posZ());
	glVertex3f(b.posX(), b.posY(), b.posZ());
	glVertex3f(c.posX(), c.posY(), c.posZ());
	glEnd();
}

void drawRectangle(Point &a, Point &b, Point &c, Point &d)
{
	glBegin(GL_POLYGON);
	glVertex3f(a.posX(), a.posY(), a.posZ());
	glVertex3f(b.posX(), b.posY(), b.posZ());
	glVertex3f(c.posX(), c.posY(), c.posZ());
	glVertex3f(d.posX(), d.posY(), d.posZ());
	glEnd();
}

// 环境光
Color getAmbient(GLfloat ambient_light[], GLfloat ambient_mesh[])
{
	Color ambient;
	ambient.R = ambient_light[0] * ambient_mesh[0];
	ambient.G = ambient_light[1] * ambient_mesh[1];
	ambient.B = ambient_light[2] * ambient_mesh[2];
	return ambient;
}

int getSmallestNum(std::vector<float> list)
{
	float min_num = FLT_MAX;
	int min_id = -1;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] > 0 && list[i] < min_num)
		{
			min_num = list[i];
			min_id = i;
		}
	}
	return min_id;
}


// 漫反射
Color getDiffuse(GLfloat diffuse_light[], GLfloat diffuse_mesh[], Point light_vector, Point mesh_normal)
{
	float weight = (light_vector * mesh_normal) / light_vector.getDist() / mesh_normal.getDist();
	Color diffuse;
	diffuse.R = diffuse_light[0] * diffuse_mesh[0] * weight;
	diffuse.G = diffuse_light[1] * diffuse_mesh[1] * weight;
	diffuse.B = diffuse_light[2] * diffuse_mesh[2] * weight;
	return diffuse;
}

// 镜面反射
Color getSpecular(GLfloat specular_light[], GLfloat specular_mesh[], Point light_vector, Point seeing_direction, Point mesh_normal)
{
	int n = 10;
	Point reflection = mesh_normal * 2 * (mesh_normal * light_vector) - light_vector;
	float weight = reflection * seeing_direction;
	float original_weight = weight;
	for (int i = 0; i < n - 1; i++)
	{
		weight *= original_weight;
	}

	Color specular;
	specular.R = specular_light[0] * specular_mesh[0] * weight;
	specular.G = specular_light[1] * specular_mesh[1] * weight;
	specular.B = specular_light[2] * specular_mesh[2] * weight;
	return specular;
}