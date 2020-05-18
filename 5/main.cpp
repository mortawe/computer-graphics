#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include "polygon.cpp"
int screenWidth = 800, screenHeight = 800;
bool isPolygonInputMode = true, isClipperInputMode = false;
bool isShowPolygon = true, isShowClipper = true, isShowCliped = false;
float CLIPPER_COLOR[] = {1, 0, 1}, COLOR[] = {0, 1, 1},
        CLIPED_COLOR[] = {0, 0, 1};

Polygon *polygon, *clipper, *cliped;
SuthHodg *suthHodg;

void mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (isPolygonInputMode) {
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            polygon->addVertex(Point{float(xPos), float(float(min(screenWidth, screenHeight)) - yPos)});
        }
        if (isClipperInputMode) {
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            clipper->addVertex(Point{float(xPos), float(float(min(screenWidth, screenHeight)) - yPos)});
        }
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ENTER:
                if (isPolygonInputMode) {
                    if (polygon->size() > 1) {
                        isPolygonInputMode = false;
                        isClipperInputMode = true;
                        cout << "INFO: draw the clipper in clockwise order and then press right SHIFT\n";
                        polygon->close();
                    } else {
                        cout << "ERROR: you have to enter at least 3 vertices\n";
                    }
                }
                break;
            case GLFW_KEY_RIGHT_SHIFT:
                if (isClipperInputMode) {
                    if (clipper->size() > 1) {
                        isClipperInputMode = false;
                        clipper->close();
                        suthHodg->clip(false);
                    } else {
                        cout << "ERROR: you have to enter at least 3 vertices\n";
                    }
                }
                break;
            case GLFW_KEY_BACKSPACE:
                isPolygonInputMode = true;
                isClipperInputMode = false;
                isShowPolygon = true;
                isShowCliped = true;
                isShowClipper = true;
                polygon->clear();
                clipper->clear();
                cliped->clear();
                break;
            case GLFW_KEY_C:
                if (!isPolygonInputMode && !isClipperInputMode) {
                    isShowCliped = !isShowCliped;
                } else {
                    cout << "ERROR: you have to enter Polygon and Clipper first\n";
                }
                break;
            case GLFW_KEY_R:
                if (!isPolygonInputMode && !isClipperInputMode) {
                    isShowClipper = !isShowClipper;
                } else {
                    cout << "ERROR: you have to enter Polygon and Clipper first\n";
                }
                break;
            case GLFW_KEY_P:
                if (!isPolygonInputMode && !isClipperInputMode) {
                    isShowPolygon= !isShowPolygon;
                } else {
                    cout << "ERROR: you have to enter Polygon and Clipper first\n";
                }
                break;

        }
    }
}

void onResize(GLFWwindow *pWwindow, int i, int i1){
    glfwGetFramebufferSize(pWwindow, &screenWidth, &screenHeight);
}

int main(int argc, const char * argv[]) {
    GLFWwindow *window;

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(screenWidth, screenHeight, "Lab5", NULL, NULL);
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    glfwSetFramebufferSizeCallback(window,onResize);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    if (!window) {
        glfwTerminate();
        return -1;
    }
    polygon = new Polygon{};
    clipper = new Polygon{};
    cliped = new Polygon{};
    suthHodg = new SuthHodg{polygon, clipper, cliped};
    glfwMakeContextCurrent(window);
    cout << "INFO: draw the polygon in clockwise order and then press ENTER\n";
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glViewport(0, 0, min(screenWidth, screenHeight), min(screenWidth, screenHeight));
        glLoadIdentity();
        glOrtho(0, min(screenWidth, screenHeight), 0, min(screenWidth, screenHeight), -min(screenWidth, screenHeight), min(screenWidth, screenHeight));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (isShowPolygon) polygon->drawPolygon(COLOR);
        if (isShowClipper) clipper->drawPolygon(CLIPPER_COLOR);
        if (isShowCliped) cliped->drawPolygon(CLIPED_COLOR);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}