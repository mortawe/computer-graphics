#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include "control.cpp"
#include "box.cpp"
#include "texture.cpp"
#include "config.h"
#include "unistd.h"

using namespace std;

int main(int argc, const char *argv[]) {
    if (!glfwInit()) exit(EXIT_FAILURE);
    auto window = glfwCreateWindow(900, 900, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    pyramid = new Pyramid{
            vector<Polygon>{},
            vector<Polygon>{},
            4, 50, 200, 20, 100,
            Point{0, 0, 0},
    };
    pyramid->countPyramid();

    Box box = Box{};

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    m3_per();

    materials();

    uint texture = readTexture();
    if (!texture){
//        cout << "Image could not be opened\n";
        return -5;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(zoom, zoom, zoom);
        glTranslatef(450, 450, 450);
        box.draw();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (isChangeView) glRotatef(90, 0, 1, 0);
        glScalef(zoom, zoom, zoom);
        glTranslatef(450 + translationX, 450 + translationY, 450 + translationZ);
        pyramid->drawBreakPoints();
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);
        glRotatef(rotationZ, 0, 0, 1);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, min(width, height), 0, min(width, height), -2000, 2000);
        if (isMotionOn) {
            pyramid->animate();
        }
        pyramid->draw();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (isChangeView) glRotatef(90, 0, 1, 0);
        lighting();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDefaultWindowHints();
    glfwDestroyWindow(window);
    glfwTerminate();
    delete pyramid;
    return 0;
}


