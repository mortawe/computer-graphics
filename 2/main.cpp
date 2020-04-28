//
// Created by master on 28.03.2020.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <math.h>

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

GLfloat color[] = {
        255/255.0f, 154/255.0f, 162/255.0f,
        255/255.0f, 154/255.0f, 162/255.0f,
        255/255.0f, 154/255.0f, 162/255.0f,
        255/255.0f, 154/255.0f, 162/255.0f,

        255/255.0f, 183/255.0f, 178/255.0f,
        255/255.0f, 183/255.0f, 178/255.0f,
        255/255.0f, 183/255.0f, 178/255.0f,
        255/255.0f, 183/255.0f, 178/255.0f,

        255/255.0f, 218/255.0f, 193/255.0f,
        255/255.0f, 218/255.0f, 193/255.0f,
        255/255.0f, 218/255.0f, 193/255.0f,
        255/255.0f, 218/255.0f, 193/255.0f,

        226/255.0f, 240/255.0f, 203/255.0f,
        226/255.0f, 240/255.0f, 203/255.0f,
        226/255.0f, 240/255.0f, 203/255.0f,
        226/255.0f, 240/255.0f, 203/255.0f,


        181/255.0f, 234/255.0f, 215/255.0f,
        181/255.0f, 234/255.0f, 215/255.0f,
        181/255.0f, 234/255.0f, 215/255.0f,
        181/255.0f, 234/255.0f, 215/255.0f,


        199/255.0f, 206/255.0f, 234/255.0f,
        199/255.0f, 206/255.0f, 234/255.0f,
        199/255.0f, 206/255.0f, 234/255.0f,
        199/255.0f, 206/255.0f, 234/255.0f,
};

GLfloat twoPointMatrix[] = {
        1, 0, 0, sin(q) / (float) d,
        0, 1, 0, 0,
        0, 0, 1, cos(q) / (float) d,
        0, 0 , 0, 1,
};


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_SPACE:
                (m == GL_LINE)? m = GL_FILL : m = GL_LINE;
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
                translationX -= speed*5;
                break;
            case GLFW_KEY_S:
                translationX += speed*5;
                break;
            case GLFW_KEY_A:
                translationY += speed*5;
                break;
            case GLFW_KEY_D:
                translationY -= speed*5;
                break;
            case GLFW_KEY_Z:
                (k >= 1.5) ? k = 1.5 : k += 0.1;
                break;
            case GLFW_KEY_X:
                (k <= 0.2) ? k = 0.1 : k -= 0.1;
                break;
        }
    }
}

void drawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength) {
    GLfloat halfSideLength = edgeLength * 0.5f;

    GLfloat vertices[] = {
            // front face
            centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
            centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top right
            centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom right
            centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

            // back face
            centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top left
            centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
            centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
            centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom left

            // left face
            centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
            centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
            centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
            centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

            // right face
            centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
            centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
            centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
            centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

            // top face
            centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
            centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
            centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // bottom right
            centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom left

            // bottom face
            centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
            centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // top right
            centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
            centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength  // bottom left
    };

    glPolygonMode(GL_FRONT_AND_BACK, m);

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, color);

    glDrawArrays(GL_QUADS, 0, 24);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int main(){
    if (!glfwInit()) exit(EXIT_FAILURE);
    auto window = glfwCreateWindow(1024, 720, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glMatrixMode(GL_PROJECTION);
    glMultMatrixf(twoPointMatrix);
    glOrtho(0, 1024, 0, 720, 0, 1000);

    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawCube(50, 50, -500, 50);

        glPushMatrix();

        glTranslatef(halfScreenWidth + translationX, halfScreenHeight + translationY, -500 - translationZ);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);
        glRotatef(rotationZ, 0, 0, 1);
        glScalef(k, k, k);
        glTranslatef(-halfScreenWidth, -halfScreenHeight, 500);

        drawCube(halfScreenWidth, halfScreenHeight, -500, 200);
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
