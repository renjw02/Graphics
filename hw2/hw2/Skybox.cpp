#include "Skybox.h"
#include <GL/glew.h>
#include <GL/glut.h>

void Skybox::initSky(unsigned int* texture)
{
    type = 0;
    env[0] = new BMP("skybox/right.bmp");
    env[1] = new BMP("skybox/left.bmp");
    env[2] = new BMP("skybox/bottom.bmp");
    env[3] = new BMP("skybox/top.bmp");
    env[4] = new BMP("skybox/front.bmp");
    env[5] = new BMP("skybox/back.bmp");
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);
    for (int i = 0; i < 6; i++)
    {
        int target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
        glTexImage2D(target, 0, GL_RGBA, env[i]->w, env[i]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, env[i]->data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete env[0];
    delete env[1];
    delete env[2];
    delete env[3];
    delete env[4];
    delete env[5];
}

void Skybox::invert(unsigned int* texture, int type)
{
    if (type) {
        env[0] = new BMP("skybox/right-i.bmp");
        env[1] = new BMP("skybox/left-i.bmp");
        env[2] = new BMP("skybox/bottom-i.bmp");
        env[3] = new BMP("skybox/top-i.bmp");
        env[4] = new BMP("skybox/front-i.bmp");
        env[5] = new BMP("skybox/back-i.bmp");
    }
    else {
        env[0] = new BMP("skybox/right.bmp");
        env[1] = new BMP("skybox/left.bmp");
        env[2] = new BMP("skybox/bottom.bmp");
        env[3] = new BMP("skybox/top.bmp");
        env[4] = new BMP("skybox/front.bmp");
        env[5] = new BMP("skybox/back.bmp");
    }
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);
    for (int i = 0; i < 6; i++)
    {
        int target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
        glTexImage2D(target, 0, GL_RGBA, env[i]->w, env[i]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, env[i]->data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete env[0];
    delete env[1];
    delete env[2];
    delete env[3];
    delete env[4];
    delete env[5];
    this->type = 1 - this->type;
}



void Skybox::renderEnvironment(unsigned int* texture)
{
    glEnable(GL_TEXTURE_CUBE_MAP);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);
    // front
    glBegin(GL_POLYGON);
    glTexCoord3d(-1, 1, -1);
    glVertex3d(-100 / 2, -100 / 2, -100 / 2);
    glTexCoord3d(1, 1, -1);
    glVertex3d(100 / 2, -100 / 2, -100 / 2);
    glTexCoord3d(1, -1, -1);
    glVertex3d(100 / 2, 100 / 2, -100 / 2);
    glTexCoord3d(-1, -1, -1);
    glVertex3d(-100 / 2, 100 / 2, -100 / 2);
    glEnd();
    // back
    glBegin(GL_POLYGON);
    glTexCoord3d(-1, 1, 1);
    glVertex3d(-100 / 2, -100 / 2, 100 / 2);
    glTexCoord3d(1, 1, 1);
    glVertex3d(100 / 2, -100 / 2, 100 / 2);
    glTexCoord3d(1, -1, 1);
    glVertex3d(100 / 2, 100 / 2, 100 / 2);
    glTexCoord3d(-1, -1, 1);
    glVertex3d(-100 / 2, 100 / 2, 100 / 2);
    glEnd();
    // bottom
    glBegin(GL_POLYGON);
    glTexCoord3d(-1, 1, -1);
    glVertex3d(-100 / 2, -100 / 2, -100 / 2);
    glTexCoord3d(1, 1, -1);
    glVertex3d(100 / 2, -100 / 2, -100 / 2);
    glTexCoord3d(1, 1, 1);
    glVertex3d(100 / 2, -100 / 2, 100 / 2);
    glTexCoord3d(-1, 1, 1);
    glVertex3d(-100 / 2, -100 / 2, 100 / 2);
    glEnd();
    // top
    glBegin(GL_POLYGON);
    glTexCoord3d(-1, -1, -1);
    glVertex3d(-100 / 2, 100 / 2, -100 / 2);
    glTexCoord3d(1, -1, -1);
    glVertex3d(100 / 2, 100 / 2, -100 / 2);
    glTexCoord3d(1, -1, 1);
    glVertex3d(100 / 2, 100 / 2, 100 / 2);
    glTexCoord3d(-1, -1, 1);
    glVertex3d(-100 / 2, 100 / 2, 100 / 2);
    glEnd();
    // left
    glBegin(GL_POLYGON);
    glTexCoord3d(-1, 1, -1);
    glVertex3d(-100 / 2, -100 / 2, -100 / 2);
    glTexCoord3d(-1, -1, -1);
    glVertex3d(-100 / 2, 100 / 2, -100 / 2);
    glTexCoord3d(-1, -1, 1);
    glVertex3d(-100 / 2, 100 / 2, 100 / 2);
    glTexCoord3d(-1, 1, 1);
    glVertex3d(-100 / 2, -100 / 2, 100 / 2);
    glEnd();
    // right
    glBegin(GL_POLYGON);
    glTexCoord3d(1, 1, -1);
    glVertex3d(100 / 2, -100 / 2, -100 / 2);
    glTexCoord3d(1, -1, -1);
    glVertex3d(100 / 2, 100 / 2, -100 / 2);
    glTexCoord3d(1, -1, 1);
    glVertex3d(100 / 2, 100 / 2, 100 / 2);
    glTexCoord3d(1, 1, 1);
    glVertex3d(100 / 2, -100 / 2, 100 / 2);
    glEnd();
    glDisable(GL_TEXTURE_CUBE_MAP);
}
