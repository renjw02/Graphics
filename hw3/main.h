#pragma once
#include "Skybox.h"
#include "Camera.h"
#include "Model.h"
#include "Color.h"
#include "Ray.h"
#include "Light.h"
#include "utils.h"

int fx, fy;
const float interval = 0.02;
Skybox skybox;
Light light;
Camera camera(10, 10);
Model bun, dragon, happy;


// 光线追踪生成图片分辨率
const int rx = 150;
const int ry = 150;
// 图像在摄像机坐标中的大小
const float lx = 30;
const float ly = 30;
const int maxDepth = 4;
Color picture[100][100];

// 需读入数据
Point pos1, pos2, pos3, pos4, pos5;
Point pos_light;
float alpha1 = 1, alpha2 = 1, alpha3;
GLfloat color1[3], color2[3];
GLuint texture;

void readConfig();

void init();
void initSkybox();
void initMeshs();
void initLight();
void setCamera();
void drawScene();
void drawSkybox();
void drawMesh(Model &model);

void initCameraRayTrace(Point& x_axis, Point& y_axis, Point& z_axis);
void rayTracing();
void rayTracingDisplay();
Color rayTracingAlgorithm(Ray& ray, int depth);

void onMouseClick(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void reshape(int w, int h);
void update(int value);
void onKeyClick(unsigned char key, int x, int y);