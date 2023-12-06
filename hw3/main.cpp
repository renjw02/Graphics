#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "main.h"


int main(int argc, char** argv) {
    std::cout << "选择功能：\n";
    std::cout << "1: phong模型    2: 光线追踪\n";
    int mode = 0;
    std::cin >> mode;

    glutInit(&argc, argv);
    init();
    initLight();
    initMeshs();

    if (mode == 1) {
        initSkybox();   
        // 绑定显示函数
        glutDisplayFunc(drawScene);

        //绑定鼠标函数
        glutMouseFunc(onMouseClick);
        glutMotionFunc(onMouseMove);

        //绑定键盘
        glutKeyboardFunc(onKeyClick);
    }
    else if (mode == 2) {
        rayTracing();
        glutDisplayFunc(rayTracingDisplay); //绑定显示函数
    }
    else {
        std::cout << "error mode!\n";
        return -1;
    }

    //绑定更新函数
    glutReshapeFunc(reshape);
    //绑定计时器
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}

void init() {
    readConfig();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Scene");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //指定混合函数
    glEnable(GL_BLEND);
}


void initSkybox()
{
    skybox.initSky(texture);
}

void initMeshs()
{
    float size = 2;
    //面片的纹理，材质，颜色
    GLfloat ambient[3] = { 0.2, 0.2, 0.6 };
    GLfloat diffuse[3] = { 0.4, 0.4, 0.4 };
    GLfloat specular[3] = { 1, 1, 1 };
    GLfloat shininess = 100;
    float k_reflection = 0.2;
    float k_refraction = 0.1;
    float refraction_rate = 1.4;
    bun.initPlace("bun.ply", size, pos1);
    bun.initColor(ambient, diffuse, specular, shininess, k_reflection, k_refraction, refraction_rate, alpha1);


    //面片的纹理，材质，颜色
    GLfloat ambient1[3] = { 0.6, 0.2, 0.2 };
    GLfloat diffuse1[3] = { 0.4, 0.4, 0.4 };
    GLfloat specular1[3] = { 0.2, 0.2, 0.2 };
    GLfloat shininess1 = 100;
    float k_reflection1 = 0.3;
    float k_refraction1 = 0.1;
    float refraction_rate1 = 1.6;
    dragon.initPlace("dragon.ply", size, pos2);
    dragon.initColor(ambient1, diffuse1, specular1, shininess1, k_reflection1, k_refraction1, refraction_rate1, alpha2);


    //面片的纹理，材质，颜色
    GLfloat ambient2[3] = { 0.6, 0.6, 0.2 };
    GLfloat diffuse2[3] = { 0.4, 0.4, 0.4 };
    GLfloat specular2[3] = { 0.2, 0.2, 0.2 };
    GLfloat shininess2 = 100;
    float k_reflection2 = 0.6;
    float k_refraction2 = 0.9;
    float refraction_rate2 = 1.8;

    happy.initPlace("happy.ply", size, pos3);
    happy.initColor(ambient2, diffuse2, specular2, shininess2, k_reflection2, k_refraction2, refraction_rate2, alpha3);
}

void initLight()
{
    //Point pos(0, 0, 20);
    light.setPlace(pos_light);

    glShadeModel(GL_SMOOTH);
    //设置初始背景色，清除颜色缓存和深度缓存
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void setCamera()
{
    glLoadIdentity();
    gluLookAt(camera.getPos().posX(), camera.getPos().posY(), camera.getPos().posZ(), camera.getLookAt().posX(),
        camera.getLookAt().posY(), camera.getLookAt().posZ(), 0, 1, 0);

}

void drawSkybox() {
    skybox.renderEnvironment(texture);
}

void drawMesh(Model& model)
{
    for (int i = 0; i < model.faceNum; i++)
    {
        Model::Mesh the_face = model.faceList[i];
        Color p = phong(light, camera, the_face);
        glColor4f(p.R, p.G, p.B, model.alpha);
        drawTriangle(the_face.v1, the_face.v2, the_face.v3);
    }
}



void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setCamera();
    drawSkybox();
    drawMesh(bun);
    drawMesh(dragon);
    drawMesh(happy);
    glutSwapBuffers();
}

void onMouseClick(int button, int state, int x, int y)
{
    fx = x;
    fy = y;
}

void onMouseMove(int x, int y) {
    //std::cout << "mouse move\n";
    camera.mouseMove(x, y, fx, fy);
}

void initCameraRayTrace(Point& x_axis, Point& y_axis, Point& z_axis)
{
    camera.getPos().setPoint(pos4);
    camera.getLookAt().setPoint(pos5);
    x_axis.setPoint(1, -1, -sqrt(2));
    y_axis.setPoint(-1, 1, -sqrt(2));
    z_axis.setPoint(-1, -1, 0);
    x_axis = x_axis / x_axis.getDist();
    y_axis = y_axis / y_axis.getDist();
    z_axis = z_axis / z_axis.getDist();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, (float)w / h, 1.0f, 1000.0f); // 视角：眼睛睁开的角度
    glMatrixMode(GL_MODELVIEW);
}

void rayTracing()
{
    //求解相机-视点对应的坐标，以及每个像素的位置
    float dist_x = lx / rx;
    float dist_y = ly / ry;
    Point x_axis;
    Point y_axis;
    Point z_axis;
    initCameraRayTrace(x_axis, y_axis, z_axis);
    Point half_x = x_axis * (lx / 2);
    Point half_y = y_axis * (ly / 2);
    Point base = camera.getPos() - half_x - half_y;

    // 遍历像素
    for (int i = 0; i < rx; i++)
    {
        Point dx = x_axis * (dist_x * i);
        for (int j = 0; j < ry; j++)      
        {
            Point dy = y_axis * (dist_y * j);
            Point start = base + dx + dy;
            Ray ray(start, z_axis, 1, 1);
            Color color = rayTracingAlgorithm(ray, 1);
            picture[i][j] = color;
        }
    }

}

void update(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, update, 1);
}

void onKeyClick(unsigned char key, int x, int y)
{
    int type = -1;
    //std::cout << x << ',' << y << std::endl;
    if (key == 'w')
    {
        type = 0;
    }
    else if (key == 'a')
    {
        type = 1;
    }
    else if (key == 's')
    {
        type = 2;
    }
    else if (key == 'd')
    {
        type = 3;
    }
    camera.horizentalMove(type);
}

void rayTracingDisplay()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, 18, 0, 1, 0);
    Point base = Point(-lx / 2, -ly / 2, 18);
    float dist_x = lx / rx;
    float dist_y = ly / ry;

    for (int i = 0; i < rx; i++)
    {
        for (int j = 0; j < ry; j++)
        {
            Point v1 = Point(base.posX() + i * dist_x, base.posY() + j * dist_y, base.posZ());
            Point v2 = Point(v1.posX() + dist_x, v1.posY(), v1.posZ());
            Point v3 = Point(v1.posX() + dist_x, v1.posY() + dist_y, v1.posZ());
            Point v4 = Point(v1.posX(), v1.posY() + dist_y, v1.posZ());
            Color color = picture[i][j];
            glColor3f(color.R, color.G, color.B);
            drawRectangle(v1, v2, v3, v4);
        }
    }
    glutSwapBuffers();
}

void readConfig()
{
    std::ifstream fin;
    fin.open("./config.txt");
    if (!fin.is_open()) {
        exit(-1);
    }

    float tmp[20];
    std::string input;
    int k = 0;
    while (fin >> input) {
        tmp[k++] = std::stof(input);
        //std::cout << tmp[k - 1] << ' ';
    }
    fin.close();

    pos1.setPoint(tmp[0], tmp[1], tmp[2]);
    pos2.setPoint(tmp[3], tmp[4], tmp[5]);
    pos3.setPoint(tmp[6], tmp[7], tmp[8]);
    alpha3 = tmp[9];
    pos_light.setPoint(tmp[10], tmp[11], tmp[12]);
    pos4.setPoint(tmp[13], tmp[14], tmp[15]);
    pos5.setPoint(tmp[16], tmp[17], tmp[18]);
    //color1[0] = tmp[12];
    //color1[1] = tmp[13];
    //color1[2] = tmp[14];
    //color2[0] = tmp[15];
    //color2[1] = tmp[16];
    //color2[2] = tmp[17];
}

Color rayTracingAlgorithm(Ray& ray, int depth)
{
    if (depth > maxDepth || ray.intensity <= 0.01) {
        return Color(0, 0, 0);
    }

    // 寻找光线与物体的交点
    std::vector<float> intersections;
    int i_bunny = -1;
    float t_bunny = -1;
    ray.getIntersection(bun, i_bunny, t_bunny);
    intersections.push_back(t_bunny);

    int i_dragon = -1;
    float t_dragon = -1;
    ray.getIntersection(dragon, i_dragon, t_dragon);
    intersections.push_back(t_dragon);

    int i_happy = -1;
    float t_happy = -1;
    ray.getIntersection(happy, i_happy, t_happy);
    intersections.push_back(t_happy);

    
    //for (auto it = intersections.begin(); it != intersections.end(); ++it) {
    //    if (*it > 0)
    //        std::cout << *it << ' '<< distance(intersections.begin(), it) << std::endl;
    //}
    

    int min_id = getSmallestNum(intersections);
    if (min_id < 0)
    {
        return Color(0.0, 0.0, 0.0);
    }

    //求交点，反射光线，折射光线，颜色
    Point intersection;
    Ray reflection;
    Ray refraction;
    Color color;
    float k_reflection = 0.0;
    float k_refraction = 0.0;
    if (min_id == 0)
    {
        Model::Mesh triangle = bun.faceList[i_bunny];
        k_reflection = triangle.reflection;
        k_refraction = triangle.refraction;
        color = phong(light, camera, triangle);
        reflection = ray.getReflection(bun, i_bunny, t_bunny);
        refraction = ray.getRefraction(bun, i_bunny, t_bunny);
        intersection = reflection.startPlace;
    }

    else if (min_id == 1)
    {
        Model::Mesh triangle = dragon.faceList[i_dragon];
        k_reflection = triangle.reflection;
        k_refraction = triangle.refraction;
        color = phong(light, camera, triangle);
        reflection = ray.getReflection(dragon, i_dragon, t_dragon);
        refraction = ray.getRefraction(dragon, i_dragon, t_dragon);
        intersection = reflection.startPlace;
    }
    else if (min_id == 2)
    {
        Model::Mesh triangle = happy.faceList[i_happy];
        k_reflection = triangle.reflection;
        k_refraction = triangle.refraction;
        color = phong(light, camera, triangle);
        reflection = ray.getReflection(happy, i_happy, t_happy);
        refraction = ray.getRefraction(happy, i_happy, t_happy);
        intersection = reflection.startPlace;
    }

    Color color_reflection = rayTracingAlgorithm(reflection, depth + 1);
    Color color_refraction = rayTracingAlgorithm(refraction, depth + 1);
    Color result = color + color_reflection * k_reflection + color_refraction * k_refraction;
    return result;

}