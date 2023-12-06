#include "Model.h"

void Model::Mesh::initPlace(Point a, Point b, Point c)
{
	v1 = a;
	v2 = b;
	v3 = c;
	Point l1 = v1 - v2;
	Point l2 = v2 - v3;
	float normal_x = (l1.posY() * l2.posZ()) - (l1.posZ() * l2.posY());
	float normal_y = -((l1.posX() * l2.posZ()) - (l1.posZ() * l2.posX()));
	float normal_z = (l1.posX() * l2.posY()) - (l1.posY() * l2.posX());
	normal.setPoint(normal_x, normal_y, normal_z);
	normal = normal / normal.getDist();

	center = (a + b + c) / 3;
}

void Model::Mesh::initColor(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess, float reflection, float refraction, float refraction_rate)
{
	for (int i = 0; i < 3; i++)
	{
		this->ambient[i] = ambient[i];
		this->diffuse[i] = diffuse[i];
		this->specular[i] = specular[i];
	}
	this->shininess = shininess;
	this->reflection = reflection;
	this->refraction = refraction;
	this->refractionRate = refraction_rate;
}

void Model::initColor(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess, float reflection, float refraction, float refraction_rate, float alpha)
{
	this->alpha = alpha;
	for (int i = 0; i < 3; i++)
	{
		this->ambient[i] = ambient[i];
		this->diffuse[i] = diffuse[i];
		this->specular[i] = specular[i];
	}
	this->shininess = shininess;
	this->reflection = reflection;
	this->refraction = refraction;
	this->refractionRate = refraction_rate;
	for (int i = 0; i < faceNum; i++)
	{
		faceList[i].initColor(ambient, diffuse, specular, shininess, reflection, refraction, refraction_rate);
	}
}

void Model::initPlace(std::string filename, float size, Point base)
{
	storagePath += filename;
	this->size = size;
	position = base;
	vertexNum = 0;
	faceNum = 0;

	std::fstream f(storagePath);
	int properties = readPly(f);

	for (int i = 0; i < vertexNum; i++)
	{
		float x, y, z;
		f >> x >> y >> z;
		Point nova(x, y, z);
		vertexList.push_back(nova);
		for (int i = 3; i < properties - 1; i++)
		{
			float temp;
			f >> temp;
		}
	}

	normalalization();

	for (int i = 0; i < faceNum; i++)
	{
		int num, v1_num, v2_num, v3_num;
		f >> num >> v1_num >> v2_num >> v3_num;
		Point v1 = vertexList[v1_num];
		Point v2 = vertexList[v2_num];
		Point v3 = vertexList[v3_num];
		Mesh nova;
		nova.initPlace(v1, v2, v3);
		faceList.push_back(nova);
	}
}

int Model::readPly(std::fstream& f)
{
	using namespace std;
	int properties = 0;
	while (true)
	{
		string line;
		getline(f, line);

		if (line.substr(0, 15) == "element vertex ")
		{
			string vertex_num = line.substr(15);
			vertexNum = stoi(vertex_num);
		}
		else if (line.substr(0, 13) == "element face ")
		{
			string face_num = line.substr(13);
			faceNum = stoi(face_num);
		}
		else if (line.substr(0, 8) == "property")
		{
			properties++;
		}
		else if (line == "end_header")
		{
			break;
		}

	}
	return properties;
}

void Model::normalalization()
{
	// -均值 /标准差 * 大小 +偏置 
	Point mean(0, 0, 0);
	for (int i = 0; i < vertexNum; i++)
	{
		mean = mean + vertexList[i];
	}
	mean = mean / float(vertexNum);

	float variance = 0;
	for (int i = 0; i < vertexNum; i++)
	{
		float error = (mean - vertexList[i]) * (mean - vertexList[i]);
		variance += error;
	}
	variance /= float(vertexNum);
	float standard_deviation = sqrt(variance);

	for (int i = 0; i < vertexNum; i++)
	{
		Point p = vertexList[i];
		p = p - mean;
		p = p / standard_deviation;
		p = p * size;
		p = p + position;
		vertexList[i] = p;
	}
}
