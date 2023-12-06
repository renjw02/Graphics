#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "Pillar.h"
#include "Ball.h"
#include "Camera.h"
#include "Skybox.h"

Pillar pillar_1;
Pillar pillar_2;
Ball ball;
Camera camera(10, 10);
int fx, fy;
const float interval = 0.02;
Skybox skybox;

GLuint texture[6];

GLuint shaderProgram, framebuffer, textureColorBuffer;

// 需读入数据
Point pos1, pos2, pos3;
Point speed;
GLfloat color1[3], color2[3];

void init();
void initPillar();
void initBall();
void initSkybox();
void setCamera();
void drawScene();
void drawPolygon(Point a, Point b, Point c, Point d);
void drawPillar();
void drawBall();
void drawSkybox();
void readConfig();


void onMouseClick(int button, int state, int x, int y)
{
    fx = x;
    fy = y;
}

void onMouseMove(int x, int y) {
    camera.mouseMove(x, y, fx, fy);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, (float)w / h, 1.0f, 1000.0f); // 视角：眼睛睁开的角度
    glMatrixMode(GL_MODELVIEW);
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
    else if (key == 'l') {
        color1[0] = 1 - color1[0];
        color1[1] = 1 - color1[1];
        color1[2] = 1 - color1[2];
        color2[0] = 1 - color2[0];
        color2[1] = 1 - color2[1];
        color2[2] = 1 - color2[2];
        skybox.invert(texture, 1 - skybox.type);
    }
    camera.horizentalMove(type);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    init();
    initPillar();
    initBall();
    initSkybox();
    // 绑定显示函数
    glutDisplayFunc(drawScene);
    //绑定计时器
    glutTimerFunc(0, update, 0);
    //绑定鼠标函数
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);

    //绑定键盘
    glutKeyboardFunc(onKeyClick);

    //绑定更新函数
    glutReshapeFunc(reshape);

    
    //initShader();
    //frame();

    glutMainLoop();
    system("pause");
    return 0;
}

void init() {
    readConfig();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Scene");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE);
    
}

void initPillar()
{
    float width = 2;
    float height = 4;
    pillar_1.setPillar(pos1, width, height, color1);

    pillar_2.setPillar(pos2, width, height, color1);

    /*std::cout << pillar_1.getPoints()[7].posX() << ' ' << pillar_1.getPoints()[7].posY() << ' ' << pillar_1.getPoints()[7].posZ() << std::endl;
    std::cout << pillar_1.getPoints()[1].posX() << ' ' << pillar_1.getPoints()[1].posY() << ' ' << pillar_1.getPoints()[1].posZ() << std::endl;
    std::cout << pillar_2.getPoints()[7].posX() << ' ' << pillar_2.getPoints()[7].posY() << ' ' << pillar_2.getPoints()[7].posZ() << std::endl;
    std::cout << pillar_2.getPoints()[1].posX() << ' ' << pillar_2.getPoints()[1].posY() << ' ' << pillar_2.getPoints()[1].posZ() << std::endl;*/
}

void initBall()
{
    ball.setBall(pos3.posX(), pos3.posY(), pos3.posZ(), 1, speed.posX(), speed.posY(), speed.posZ(), color2);
    /*std::cout << ball.box_left_bottom_back.posX() << ' ' << ball.box_left_bottom_back.posY() << ' ' << ball.box_left_bottom_back.posZ() << std::endl;
    std::cout << ball.box_right_top_front.posX() << ' ' << ball.box_right_top_front.posY() << ' ' << ball.box_right_top_front.posZ() << std::endl;*/
}

void initSkybox()
{
    skybox.initSky(texture);
}


void setCamera()
{
    glLoadIdentity();
    gluLookAt(camera.getPos().posX(), camera.getPos().posY(), camera.getPos().posZ(), camera.getLookAt().posX(),
        camera.getLookAt().posY(), camera.getLookAt().posZ(), 0, 1, 0);
}

//void initShader() {
//    GLuint vertexShader, fragmentShader;
//    GLint vertCompiled, fragCompiled;
//
//    // 创建顶点着色器
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    const char* vertexShaderSource = R"(
//        #version 330 core
//        layout (location = 0) in vec2 aPos;
//        layout (location = 1) in vec2 aTexCoords;
//
//        out vec2 TexCoords;
//
//        void main()
//        {
//            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
//            TexCoords = aTexCoords;
//        }
//    )";
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//    glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &vertCompiled);
//
//    // 创建片段着色器
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    const char* fragmentShaderSource = R"(
//        #version 330 core
//        out vec4 FragColor;
//
//        in vec2 TexCoords;
//
//        uniform sampler2D screenTexture;
//
//        void main()
//        { 
//            //FragColor = texture(screenTexture, TexCoords);
//            FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
//        }
//    )";
//
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // 创建着色器程序
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//}
//
//void frame() {
//    // 创建和绑定帧缓冲对象
//    glGenFramebuffers(1, &framebuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//
//    // 创建和绑定纹理附件
//    glGenTextures(1, &textureColorBuffer);
//    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
//
//    unsigned int rbo;
//    glGenRenderbuffers(1, &rbo);
//    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
//
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//        std::cerr << "Framebuffer is not complete!" << std::endl;
//    }
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//}

void drawPolygon(Point a, Point b, Point c, Point d)
{
    glColor3f(color1[0], color1[1], color1[2]);
    glBegin(GL_POLYGON);
    glVertex3f(a.posX(), a.posY(), a.posZ());
    glVertex3f(b.posX(), b.posY(), b.posZ());
    glVertex3f(c.posX(), c.posY(), c.posZ());
    glVertex3f(d.posX(), d.posY(), d.posZ());
    glEnd();
}

void drawPillar() {
    Point* points = pillar_1.getPoints();
    drawPolygon(points[0], points[1], points[2], points[3]);
    drawPolygon(points[0], points[4], points[7], points[3]);
    drawPolygon(points[4], points[5], points[6], points[7]);
    drawPolygon(points[1], points[5], points[4], points[0]);
    drawPolygon(points[3], points[7], points[6], points[2]);
    drawPolygon(points[2], points[6], points[5], points[1]);

    points = pillar_2.getPoints();
    drawPolygon(points[0], points[1], points[2], points[3]);
    drawPolygon(points[0], points[4], points[7], points[3]);
    drawPolygon(points[4], points[5], points[6], points[7]);
    drawPolygon(points[1], points[5], points[4], points[0]);
    drawPolygon(points[3], points[7], points[6], points[2]);
    drawPolygon(points[2], points[6], points[5], points[1]);
}

void drawBall() {
    ball.move(interval);
    ball.collision(pillar_1);
    ball.collision(pillar_2);

    glColor3f(color2[0], color2[1], color2[2]);
    glPushMatrix();
    glTranslatef(ball.getPos().posX(), ball.getPos().posY(), ball.getPos().posZ());
    glutSolidSphere(ball.getRadius(), 40, 40);
    glPopMatrix();

}

void drawSkybox() {
    skybox.renderEnvironment(texture);
}

void readConfig()
{
    std::ifstream fin;
    fin.open("./config.txt");
    if (!fin.is_open()) {
        exit(-1);
    }

    float tmp[18];
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
    speed.setPoint(tmp[9], tmp[10], tmp[11]);
    color1[0] = tmp[12];
    color1[1] = tmp[13];
    color1[2] = tmp[14];
    color2[0] = tmp[15];
    color2[1] = tmp[16];
    color2[2] = tmp[17];
}

void drawScene() {
    // 渲染原始场景到帧缓冲对象
    //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    setCamera();
    drawPillar();
    drawBall();
    drawSkybox();

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //// 应用滤镜效果到纹理附件
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glUseProgram(shaderProgram);

    // 渲染帧缓冲对象的内容到屏幕

    // 使用一个屏幕填充的四边形来显示帧缓冲对象的内

    // 设置四边形的顶点数据
    // 例如，使用glBegin/glEnd或VAO/VBO来绘制一个占满整个窗口的四边形
    //float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    //    // positions   // texCoords
    //    -1.0f,  1.0f,  0.0f, 1.0f,
    //    -1.0f, -1.0f,  0.0f, 0.0f,
    //     1.0f, -1.0f,  1.0f, 0.0f,

    //    -1.0f,  1.0f,  0.0f, 1.0f,
    //     1.0f, -1.0f,  1.0f, 0.0f,
    //     1.0f,  1.0f,  1.0f, 1.0f
    //};


    //unsigned int quadVAO;
    //glGenVertexArrays(1, &quadVAO);
    //glBindVertexArray(quadVAO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(0);

    //glDisable(GL_DEPTH_TEST);
    //glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //
    //glUseProgram(0);

    glutSwapBuffers();
    //glFlush();
}