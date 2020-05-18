
#include "config.h"

void light_on(int i){
    switch (i) {
        case 0: {
            GLfloat  light0[] = {1,1,1};
            GLfloat light0_position[] = {450, 950, 450.0, 1.0};
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light0);
            glLightfv(GL_LIGHT0, GL_SPECULAR, light0);

            glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
            break;
        }
        case 1: {
            GLfloat light1_diffuse[] = {1, 1, 1};
            GLfloat light1_position[] = {0.0, 0.0, 450.0, 1.0};
            glEnable(GL_LIGHT1);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
            glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
            break;
        }
        case 2: {
            GLfloat light2_diffuse[] = {1, 1, 1};
            GLfloat light2_position[] ={0.0, 900.0, 450.0, 0.0};
            glEnable(GL_LIGHT2);
            glLightfv(GL_LIGHT2, GL_SPECULAR, light2_diffuse);
            glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
            break;
        }
        case 3: {
            GLfloat light3_diffuse[] = {1, 1, 1};
            GLfloat light3_position[] =  {0.0, 0.0, 0.0, 1.0};
            glEnable(GL_LIGHT3);
            glLightfv(GL_LIGHT3, GL_AMBIENT, light3_diffuse);
            glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
            break;
        }
    }
}

void m3_per() {
    float p, q, r, c;
    c = 4.f;
    p = q = r = 1 / c;
    double M[] = {
            1, 0, 0, p,
            0, 1, 0, q,
            0, 0, 1, r,
            0, 0, 0, 1
    };
    glLoadMatrixd(M);
}

void lighting(){
    if (!on0)
        glDisable(GL_LIGHT0);
    else light_on(0);
    if (!on1)
        glDisable(GL_LIGHT1);
    else light_on(1);
    if (!on2)
        glDisable(GL_LIGHT2);
    else light_on(2);
    if (!on3)
        glDisable(GL_LIGHT3);
    else light_on(3);
}