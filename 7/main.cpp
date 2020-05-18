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

int WINDOW_SIZE = 900;

int main(int argc, const char *argv[]) {
    if (!glfwInit()) exit(EXIT_FAILURE);
    auto window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Simple example", NULL, NULL);
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

    pyramid = new Pyramid(4, 50, 200, 35, 100);
    pyramid->count();

    Box box = Box{};
    box.init(multy);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    m3_per();

    materials();

    uint texture = readTexture();
    if (!texture) {
        cout << "Image could not be opened\n";
        return -5;
    }
//    GLfloat delta_time;
//    readConfig(pyramid);
//    if (isTextOn){
//        glEnable(GL_TEXTURE_2D);
//        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    }
//    GLfloat start_frame = glfwGetTime();
//    long long fps = 0;
    while (!glfwWindowShouldClose(window)) {
//        GLfloat current_frame = glfwGetTime();
//        if (current_frame - start_frame >= 5) {
//            break;
//        }
//        fps++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(WINDOW_SIZE / 2, WINDOW_SIZE / 2, WINDOW_SIZE / 2);
        glScalef(zoom, zoom, zoom);
        box.draw();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (isChangeView) glRotatef(90, 0, 1, 0);
        glTranslatef(WINDOW_SIZE / 2 + pyramid->translationX,
                     WINDOW_SIZE / 2 + pyramid->translationY,
                     WINDOW_SIZE / 2 + pyramid->translationZ);
        glMatrixMode(GL_MODELVIEW);
        glScalef(zoom, zoom, zoom);
        pyramid->drawBP();
        glRotatef(pyramid->rotationY, 0, -1, 0);
        glRotatef(pyramid->rotationX, 1, 0, 0);

        glRotatef(pyramid->rotationZ, 0, 0, 1);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, min(width, height), 0, min(width, height),
                -2000, 2000);
        if (isMotionOn) {
            pyramid->animate();
        }
        glCallList(pyramid->list);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (isChangeView) glRotatef(90, 0, 1, 0);
        lighting();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
//    cout << fps / 5;
    glfwDefaultWindowHints();
    glfwDestroyWindow(window);
    glfwTerminate();
    pyramid->destroy();
    delete pyramid;
    box.destroy();

    return 0;
}


