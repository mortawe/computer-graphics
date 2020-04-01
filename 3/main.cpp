
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <math.h>
#include <vector>

using namespace std;
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat rotationZ = 0.0f;
GLfloat translationX = 0.0f;
GLfloat translationY = 0.0f;
GLfloat translationZ = 0.0f;
const GLfloat speed = 5;
GLenum m = GL_LINE;
GLfloat k = 1;


GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;

GLfloat q = 75, d = 0.75;

GLfloat twoPointMatrix[] = {
        1, 0, 0, sin(q) / (float) d,
        0, 1, 0, 0,
        0, 0, 1, cos(q) / (float) d,
        0, 0, 0, 1,
};

int detailization = 50;
int vertNum = 4;
int spinningAngle = 20;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_SPACE:
                (m == GL_LINE) ? m = GL_FILL : m = GL_LINE;
                break;
            case GLFW_KEY_UP:
                rotationX -= speed;
                break;
            case GLFW_KEY_DOWN:
                rotationX += speed;
                break;
            case GLFW_KEY_RIGHT:
                rotationY += speed;
                break;
            case GLFW_KEY_LEFT:
                rotationY -= speed;
                break;
            case GLFW_KEY_W:
                translationX -= speed * 5;
                break;
            case GLFW_KEY_S:
                translationX += speed * 5;
                break;
            case GLFW_KEY_A:
                translationY += speed * 5;
                break;
            case GLFW_KEY_D:
                translationY -= speed * 5;
                break;
            case GLFW_KEY_Z:
                (k >= 1.5) ? k = 1.5 : k += 0.1;
                break;
            case GLFW_KEY_X:
                (k <= 0.2) ? k = 0.1 : k -= 0.1;
                break;
            case GLFW_KEY_KP_8:
                if (vertNum < 50) vertNum++;
                break;
            case GLFW_KEY_KP_2:
                if (vertNum > 3) vertNum--;
                break;
            case GLFW_KEY_KP_4:
                if (detailization < 80) detailization++;
                break;
            case GLFW_KEY_KP_6:
                if (detailization > 5) detailization--;
                break;
            case GLFW_KEY_KP_1:
                if (spinningAngle > 0) spinningAngle--;
                break;
            case GLFW_KEY_KP_3:
                if (spinningAngle < 1000) spinningAngle++;
                break;
        }
    }
}

void
drawPyramid(GLdouble xPos, GLdouble yPos, GLdouble zPos, int vertNum, GLdouble r, int layersN, GLdouble totalHeight,
            int angleDeg, bool isPyramid) {

    vector<vector<GLdouble>> v;
    vector<vector<GLdouble>> base;
//    GLdouble side = r / (double) (2 * sin(M_PI / (double) vertNum));
    for (int i = 0; i < vertNum; i++) {
        base.push_back(vector<GLdouble>{
                cos(2 * M_PI / (double) vertNum * i),
                yPos,
                sin(2 * M_PI / (double) vertNum * i)
        });
    }

    glPolygonMode(GL_FRONT_AND_BACK, m);

    GLdouble h = totalHeight / (double) layersN;

    GLdouble angleRad = angleDeg * M_PI / 180.0;
    GLdouble scale = isPyramid * r / (float) layersN;


    for (int j = 0; j < layersN + 1; j++) {
        vector<vector<GLdouble>> rv;
        for (int i = 0; i < vertNum; i++) {
            GLdouble x = (r - scale * j) * base[i][0];
            GLdouble z = (r - scale * j) * base[i][2];

            GLdouble newX = cos(angleRad * j / (double) layersN) * x - sin(angleRad * j / (double) layersN) * z;
            GLdouble newZ = sin(angleRad * j / (double) layersN) * x + cos(angleRad * j / (double) layersN) * z;

            rv.push_back(vector<GLdouble>{
                    newX + xPos,
                    yPos,
                    newZ + zPos});
        }

        if (j > 0) {
            for (int i = 0; i < vertNum; i++) {

                if (!isPyramid) {
                    glBegin(GL_QUADS);
                } else {
                    glBegin(GL_TRIANGLE_FAN);
                }
                glColor3f((cos(i) + 1) / 2, (sin(i) + 1) / 2, 0.7f);
                glVertex3f(v[i][0], yPos + j * h, v[i][2]);
                glVertex3f(v[(i + 1) % vertNum][0], yPos + j * h, v[(i + 1) % vertNum][2]);
                glVertex3f(rv[(i + 1) % vertNum][0], yPos + (j + 1) * h, rv[(i + 1) % vertNum][2]);
                glVertex3f(rv[i][0], yPos + (j + 1) * h, rv[i][2]);
                glEnd();
            }
        }
        v = rv;
        if (!isPyramid && (j == 0 || j == layersN)) {
            glBegin(GL_QUADS);
            glColor3f(1.0f, 0.0f, 0.0f);
            for (int i = 0; i < vertNum; i++) {
                glVertex3f(v[i][0], yPos + (j + 1) * h, v[i][2]);
            }
            glEnd();
        }
    }
}

int main() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    auto window = glfwCreateWindow(900, 900, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    glMatrixMode(GL_PROJECTION);
    glMultMatrixf(twoPointMatrix);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glOrtho(0, width, 0, height, 0, 1000);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &width, &height);
        glLoadIdentity();
        glMultMatrixf(twoPointMatrix);
        glViewport(0, 0, min(width, height), min(width, height));
        glOrtho(0, min(width, height), 0, min(width, height), 0, 1000);

        drawPyramid(50, 50, -500, 4, 100, 1, 100, 0, 0);

        glPushMatrix();
        glTranslatef((min(width, height)) / 2 + translationX, min(width, height) / 2 + translationY,
                     -500 - translationZ);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);
        glRotatef(rotationZ, 0, 0, 1);
        glScalef(k, k, k);
        glTranslatef(-(min(width, height)) / 2, -(min(width, height)) / 2, 500);

        drawPyramid((min(width, height)) / 2, 250, -500, vertNum, 200, detailization, 500, spinningAngle, 1);

        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
