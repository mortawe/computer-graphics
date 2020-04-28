
#include <iostream>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

int screenWidth = 1280, screenHeight = 720;
int curHeight = 0;

struct Point {
    int x, y;
};

struct Pixel {
    float r, g, b, alpha;
};

Pixel *buffer = NULL, *aBuffer = NULL;
vector<Point> curve;
vector<vector<Point>> edges;
vector<double> dx;

void clearBuffer() {
    Pixel clearColor{0, 0, 0, 0};
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        buffer[i] = clearColor;
    }
}

Pixel fillColor{0, 1, 0, 1};
Pixel unfillColor{0, 0, 0, 1};
Pixel lineColor{1, 1, 1, 1};
Pixel fillColorBorder{0, 1, 0, 1};
bool isPolygonInputMode = true,  isPostfiltrationMode = false, isRasterMode = false;

Pixel *getPixel(Point point) {
    return &buffer[point.y * screenWidth + point.x];
}

Pixel *getPixel2nd(Point point) {
    return &aBuffer[point.y * screenWidth + point.x];
}

void setPixel(Point point, Pixel pixel) {
    Pixel *p = getPixel(point);
    *p = pixel;
}

bool compSAR(int left, int right) {
    return (edges[left][0].x + dx[left] * (curHeight - edges[left][0].y) <=
            edges[right][0].x + dx[right] * (curHeight - edges[right][0].y));
}

void rasterContour() {
    for (curHeight = 0; curHeight < screenHeight; curHeight++) {
        vector<int> sar;
        for (int j = 0; j < edges.size(); j++) {
            if (edges[j][0].y <= curHeight && edges[j][1].y > curHeight) {
                sar.push_back(j);
            }
        }
        sort(sar.begin(), sar.end(), compSAR);
        for (int j = 0; j < sar.size(); j++) {
            int k = (int) (floor)(edges[sar[j]][0].x + (dx[sar[j]] * (curHeight - edges[sar[j]][0].y)));
            if ( k < screenWidth) {
                setPixel(Point{k, curHeight}, fillColorBorder);
            }

            if (j % 2 == 0 && j < sar.size() - 1) {
                for (int k = (int) (edges[sar[j]][0].x + (dx[sar[j]] * (curHeight - edges[sar[j]][0].y)));
                     k <= edges[sar[j + 1]][0].x + (dx[sar[j + 1]] * (curHeight - edges[sar[j + 1]][0].y))
                     && k < screenWidth; k++) {
                    setPixel(Point{k, curHeight}, fillColor);
                }
            }

        }
    }
}

void closingContour() {
    curve.push_back(curve[0]);
}

void addEdge(Point p1, Point p2) {
    if (p1.y <= p2.y) {
        edges.push_back(vector<Point>{p1, p2});
        if (p2.y - p1.y == 0) {
            dx.push_back(0);
        } else {
            dx.push_back((p2.x - p1.x) / double(p2.y - p1.y));
        }
    } else {
        edges.push_back(vector<Point>{p2, p1});
        if (p1.y - p2.y == 0) {
            dx.push_back(0);
        } else {
            dx.push_back((p1.x - p2.x) / double(p1.y - p2.y));
        }
    }
}

void makeContour() {
    for (int i = 0; i < curve.size() - 1; i++) {
        addEdge(curve[i], curve[i + 1]);
    }
}

void filter() {
    float intensityMask[9]{1, 2, 1,
                           2, 3, 2,
                           1, 2, 1};
    for (int i = 1; i < screenWidth - 1; i++) {
        for (int j = 1; j < screenHeight - 1; j++) {
            Pixel *target = getPixel2nd(Point{i, j});
            *target = *getPixel(Point{i, j});
            for (int xMov = i - 1, k = 0; xMov <= i + 1; xMov++) {
                for (int yMov = j - 1; yMov <= j + 1; yMov++, k++) {
                    Pixel *v = getPixel(Point{xMov, yMov});
                    target->alpha += v->alpha * intensityMask[k] / 3;
                    if (target->alpha >= 1) {
                        target->alpha = 1;
                    }
                    target->r += v->r * intensityMask[k] / 6;
                    target->g += v->g * intensityMask[k] / 6;
                    target->b += v->b * intensityMask[k] / 6;
                }
            }
        }
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_SPACE:
                if (curve.size() > 2) {
                    if (isPolygonInputMode) {
                        closingContour();
                    }
                    isPolygonInputMode = !isPolygonInputMode;
                }
                break;
            case GLFW_KEY_BACKSPACE:
                clearBuffer();
                curve.clear();
                edges.clear();
                dx.clear();
                isPolygonInputMode = true;
                isPostfiltrationMode = false;
                isRasterMode = false;
                break;
            case GLFW_KEY_R:
                if (!isPolygonInputMode && !isRasterMode) {
                    isRasterMode = !isRasterMode;
                    if (isRasterMode) {
                        clearBuffer();
                        makeContour();
                        rasterContour();
                        filter();
                    }
                }
                break;
            case GLFW_KEY_P:
                isPostfiltrationMode = !isPostfiltrationMode;
                cout << "POSTFILTRATION : " << isPostfiltrationMode << endl;
        }
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (isPolygonInputMode) {
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            curve.push_back(Point{int(round(xPos)), int(round(float(screenHeight) - yPos))});
        }
    }
}

void drawCurve() {
    if (curve.size() == 1) {
        glBegin(GL_POINTS);
        glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.alpha);
        Point point = curve[0];
        glVertex2f(float(point.x), float(point.y));
        glEnd();
    } else if (curve.size() > 1) {
        glBegin(GL_LINE_STRIP);
        glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.alpha);
        for (int i = 0; i < curve.size(); i++) {
            Point point = curve[i];
            glVertex2f(float(point.x), float(point.y));
        }
        glEnd();
    }
}

void onResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    if (buffer != NULL) {
        delete[] buffer;
        delete[] aBuffer;
    }
    buffer = new Pixel[screenWidth * screenHeight];
    aBuffer = new Pixel[screenWidth * screenHeight];
    if (isRasterMode) {
        clearBuffer();
        edges.clear();
        makeContour();
        rasterContour();
        filter();
    }
}

int main(int argc, const char *argv[]) {
    GLFWwindow *window;

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(screenWidth, screenHeight, "Lab4", NULL, NULL);
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glEnable(GL_ALPHA_TEST);

    buffer = new Pixel[screenWidth * screenHeight];
    aBuffer = new Pixel[screenWidth * screenHeight];
    clearBuffer();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    while (!glfwWindowShouldClose(window)) {
        glRasterPos2i(0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        if (!isRasterMode) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, screenWidth, 0, screenHeight, -screenWidth, screenWidth);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            drawCurve();
        } else {
            glDrawBuffer(GL_BACK);
            if (isPostfiltrationMode) {
                glDrawPixels(screenWidth, screenHeight, GL_RGBA, GL_FLOAT, &aBuffer[0]);
            } else {
                glDrawPixels(screenWidth, screenHeight, GL_RGBA, GL_FLOAT, &buffer[0]);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    if (buffer != NULL) {
        delete[] buffer;
        delete[] aBuffer;
    }
    glfwTerminate();

    return 0;
}