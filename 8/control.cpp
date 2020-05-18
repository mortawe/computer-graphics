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
            // pyramid movements
            switch (key) {
                case GLFW_KEY_F:
                    pyramid->rotationX += speed;
                    break;
                case GLFW_KEY_V:
                    pyramid->rotationX -= speed;
                    break;
                case GLFW_KEY_C:
                    pyramid->rotationY += speed;
                    break;
                case GLFW_KEY_B:
                    pyramid->rotationY -= speed;
                    break;
                case GLFW_KEY_W:
                    pyramid->translationY += speed ;
                    break;
                case GLFW_KEY_S:
                    pyramid->translationY -= speed ;
                    break;
                case GLFW_KEY_A:
                    pyramid->translationX -= speed ;
                    break;
                case GLFW_KEY_D:
                    pyramid->translationX += speed ;
                    break;
            }
            // pyramid form
            switch (key) {
                case GLFW_KEY_LEFT_BRACKET: {
                    pyramid->angleDeg += 5;
                    pyramid->count();
                    break;
                }
                case GLFW_KEY_RIGHT_BRACKET: {
                    pyramid->angleDeg -= 5;
                    pyramid->count();
                    break;
                }
                case GLFW_KEY_SEMICOLON: {
                    pyramid->layersN += 1;
                    pyramid->count();
                    break;
                }
                case GLFW_KEY_APOSTROPHE: {
                    if (pyramid->layersN > 1 ) {
                        pyramid->layersN -= 1;
                        pyramid->count();
                    }
                    break;
                }
                case GLFW_KEY_COMMA: {
                    if (pyramid->vertNum > 3 ) {
                        pyramid->vertNum -= 1;
                        pyramid->count();
                    }
                    break;
                }
                case GLFW_KEY_PERIOD: {
                    pyramid->vertNum += 1;
                    pyramid->count();
                    break;
                }
            }

            switch (key) {
                case GLFW_KEY_F5: {
                    writeFile(pyramid);
                    break;
                }
                case GLFW_KEY_F6: {
                    if (isTextOn) {
                        glDisable(GL_TEXTURE_2D);
                    }
                    readConfig(pyramid);
                    if (isTextOn){
                        glEnable(GL_TEXTURE_2D);
                        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                    }
                    break;
                }
            }
        }
        // navigation
        switch (key) {
            case GLFW_KEY_EQUAL: {
                zoom += 0.1;
                break;
            }
            case GLFW_KEY_MINUS: {
                (zoom <= 0.2) ? zoom = 0.1 : zoom -= 0.1;
                break;
            }
            case GLFW_KEY_SPACE: {
                isChangeView = !isChangeView;
                break;
            }
        }

        // light options
        switch (key) {
            case GLFW_KEY_1: {
                on0 = !on0;
                break;
            }
            case GLFW_KEY_2: {
                on1 = !on1;
                break;
            }
            case GLFW_KEY_3: {
                on2 = !on2;
                break;
            }
            case GLFW_KEY_4: {
                on3 = !on3;
                break;
            }
            case GLFW_KEY_5: {
                if (!isOnTwoSide) {
                    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
                } else {
                    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
                }
                isOnTwoSide = !isOnTwoSide;
                break;
            }
            case GLFW_KEY_6: {
                GLfloat c[] = {rand() % 100 / (float) 500, rand() % 100 / (float) 500, rand() % 100 / (float) 500, 1};
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, c);
                break;
            }
            case GLFW_KEY_7: {
                if (!isModelLocal) {
                    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
                } else {
                    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
                }
                isModelLocal = !isModelLocal;
                break;
            }
        }

        // features
        switch (key) {
            case GLFW_KEY_T: {
                isTextOn = !isTextOn;
                if (!isTextOn) {
                    glDisable(GL_TEXTURE_2D);
                } else {
                    glEnable(GL_TEXTURE_2D);
                    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                }
                break;
            } // texture
            case GLFW_KEY_M: {
                isMotionOn = !isMotionOn;
                if (isMotionOn) {
                    pyramid->rotateX(-pyramid->rotationX * M_PI / (float) 180);
                    pyramid->rotateY(-pyramid->rotationY * M_PI / (float) 180);
                } else {
                    pyramid->rotateY(pyramid->rotationY * M_PI / (float) 180);
                    pyramid->rotateX(pyramid->rotationX * M_PI / (float) 180);
                }
                break;
            } // motion
        }

    }
}

#endif //COMPUTER_GRAPHICS_CONTROL_CPP