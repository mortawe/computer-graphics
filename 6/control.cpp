#ifndef COMPUTER_GRAPHICS_CONTROL_CPP
#define COMPUTER_GRAPHICS_CONTROL_CPP

#include <iostream>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include "light.cpp"
#include "config.h"

int screenWidth = 800, screenHeight = 800;

Pyramid *pyramid;

void onResize(GLFWwindow *pWwindow, int i, int i1) {
    glfwGetFramebufferSize(pWwindow, &screenWidth, &screenHeight);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (!isMotionOn) {
            switch (key){
                case GLFW_KEY_F:
                    rotationX += speed;
//                    pyramid->rotate(5*M_PI/(float) 180, 0);
                    break;
                case GLFW_KEY_V:
//                    pyramid->rotate(-5*M_PI/(float) 180, 0);
                    rotationX -= speed;
                    break;
                case GLFW_KEY_C:
//                    pyramid->rotate(0, 5*M_PI/(float) 180);
                    rotationY += speed;
                    break;
                case GLFW_KEY_B:
//                    pyramid->rotate(0, -5*M_PI/(float) 180);
                    rotationY -= speed;
                    break;
                case GLFW_KEY_W:
                    translationY += speed * 5;
                    break;
                case GLFW_KEY_S:
                    translationY -= speed * 5;
                    break;
                case GLFW_KEY_A:
                    translationX -= speed * 5;
                    break;
                case GLFW_KEY_D:
                    translationX += speed * 5;
                    break;
            }
        }
        switch (key) {
            case GLFW_KEY_EQUAL:
                (zoom >= 1.5) ? zoom = 1.5 : zoom += 0.1;
                break;
            case GLFW_KEY_MINUS:
                (zoom <= 0.2) ? zoom = 0.1 : zoom -= 0.1;
                break;
            case GLFW_KEY_I:
                if (pyramid->vertNum < 50) pyramid->vertNum++;
                pyramid->countPyramid();
                break;
            case GLFW_KEY_K:
                if (pyramid->vertNum > 3) pyramid->vertNum--;
                pyramid->countPyramid();
                break;
            case GLFW_KEY_J:
                if (pyramid->layersN < 200) pyramid->layersN++;
                pyramid->countPyramid();
                break;
            case GLFW_KEY_L:
                if (pyramid->layersN > 5) pyramid->layersN--;
                pyramid->countPyramid();
                break;
            case GLFW_KEY_N:
                if (pyramid->angleDeg > 0) pyramid->angleDeg--;
                pyramid->countPyramid();
                break;
            case GLFW_KEY_M:
                if (pyramid->angleDeg < 1000) pyramid->angleDeg++;
                pyramid->countPyramid();
                break;
            case GLFW_KEY_1:
                on0 = !on0;
                break;
            case GLFW_KEY_2:
                on1 = !on1;
                break;
            case GLFW_KEY_3:
                on2 = !on2;
                break;
            case GLFW_KEY_4:
                on3 = !on3;
                break;
            case GLFW_KEY_5:
                if (!isOnTwoSide) {
                    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
                } else {
                    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
                }
                isOnTwoSide = !isOnTwoSide;
                break;
            case GLFW_KEY_7: {
                if (!isModelLocal) {
                    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
                } else {
                    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
                }
                isModelLocal = !isModelLocal;
                break;
            }
            case GLFW_KEY_6: {
                GLfloat c[] = {rand() % 100 / (float) 500, rand() % 100 / (float) 500, rand() % 100 / (float) 500, 1};
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, c);
                break;
            }
            case GLFW_KEY_SPACE: {
                isChangeView = !isChangeView;
                break;
            }
            case GLFW_KEY_Z: {
                isMotionOn = !isMotionOn;
                if (isMotionOn){
                        pyramid->rotate(-M_PI * rotationX / (float) 180, -M_PI * rotationY / (float) 180);
                }
                if (!isMotionOn) {
                        pyramid->rotate(M_PI * rotationX / (float) 180,  M_PI * rotationY / (float) 180);
                }
                break;
            }
            case GLFW_KEY_LEFT_SHIFT: {
                isChangeView = !isChangeView;
                break;
            }
            case GLFW_KEY_T: {
                isTextOn = !isTextOn;
                if (!isTextOn) {
                    glDisable(GL_TEXTURE_2D);
                } else {
                    glEnable(GL_TEXTURE_2D);
                    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                }
                break;
            }
            case GLFW_KEY_LEFT_CONTROL:{
                writeFile(pyramid);
                break;
            }
            case GLFW_KEY_X:{
                if (isTextOn) {
                    glDisable(GL_TEXTURE_2D);
                }
                readConfig(pyramid);
                if (isTextOn) {
                    glEnable(GL_TEXTURE_2D);
                    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                }
                pyramid->countPyramid();
                break;
            }
        }
    }
}

#endif //COMPUTER_GRAPHICS_CONTROL_CPP