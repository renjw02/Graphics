#pragma once
#include <GL/glut.h>
#include <string>
#include <vector>
#include <fstream>
#include "Point.h"

class Model
{
public:
	class Mesh
	{
	public:
		Point v1;
		Point v2;
		Point v3;
		Point center;
		Point normal;


		GLfloat color[3] = { 0, 0, 0 }; 
		GLfloat ambient[4] = { 0, 0, 0, 1.0 }; 
		GLfloat diffuse[4] = { 0, 0, 0, 1.0 }; 
		GLfloat specular[4] = { 0, 0, 0, 1.0 }; 
		GLfloat shininess; 
		float reflection = 0.0;
		float refraction = 0.0;
		float refractionRate = 0.0;

		void initPlace(Point a, Point b, Point c);
		void initColor(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess,
			float k_reflection, float k_refraction, float refraction_rate);
	};


private:
	std::string storagePath = "./ply/";

public:
	float size;
	Point position;	// 中心位置
	int vertexNum;
	int faceNum;
	float alpha;	// 透明度

	std::vector<Point> vertexList;
	std::vector<Mesh> faceList;

	GLfloat ambient[4] = { 0, 0, 0, 1.0 };
	GLfloat diffuse[4] = { 0, 0, 0, 1.0 }; 
	GLfloat specular[4] = { 0, 0, 0, 1.0 }; 
	GLfloat shininess = 0.0; 
	float reflection = 0.0;
	float refraction = 0.0;
	float refractionRate = 0.0;

	void initColor(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess,
		float k_reflection, float k_refraction, float refraction_rate, float alpha);
	void initPlace(std::string filename, float size, Point base);
	int readPly(std::fstream &f);	
	void normalalization();	
};

