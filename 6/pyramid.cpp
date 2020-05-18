#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include "config.h"

using namespace std;
GLfloat multy = 400;
GLfloat m1 = 1, m2 = 10;

GLfloat translationX = 0.0f;
GLfloat translationY = 0.0f;
GLfloat translationZ = 0.0f;


//
// Point
//

struct Point {
    GLfloat x, y, z;

    void draw();

    void move(Point v) {
        x += v.x;
        y += v.y;
        z += v.z;
    };

    void rotateX(Point c, GLfloat p) {
        GLfloat z2 = c.z + (z - c.z) * cos(p) - (y - c.y) * sin(p);
        GLfloat y2 = c.y + (y - c.y) * cos(p) + (z - c.z) * sin(p);
        z = z2;
        y = y2;
    }

    void rotateY(Point c, GLfloat p) {
        GLfloat z2 = c.z + (z - c.z) * cos(p) - (x - c.x) * sin(p);
        GLfloat x2 = c.x + (x - c.x) * cos(p) + (z - c.z) * sin(p);
        z = z2;
        x = x2;
    }
};

void Point::draw() {
    glVertex3f(x, y, z);
}

//
// Polygon
//

struct Polygon {
    Point p1, p2, p3;
    Point color;
    Point normal;

    void countNormal();

    void normalize();

    void drawBottom();

    void draw(int i, int j);
};

void Polygon::normalize() {
    auto x = normal.x, y = normal.y, z = normal.z;
    float length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
    normal.x = x;
    normal.y = y;
    normal.z = z;
}

void Polygon::draw(int i, int j) {
    glColor3f(color.x, color.y, color.z);

    glTexCoord2d(p1.x / (float) 400, p1.y / (float) 400);
    glNormal3f(normal.x, normal.y, normal.z);
    p1.draw();
    glTexCoord2d(p2.x / (float) 400, p2.y / (float) 400);
    glNormal3f(normal.x, normal.y, normal.z);
    p2.draw();
    glTexCoord2d(p3.x / (float) 400, p3.y / (float) 400);
    glNormal3f(normal.x, normal.y, normal.z);
    p3.draw();

}

void Polygon::drawBottom() {
    glColor3f(color.x, color.y, color.z);

    glTexCoord2d(p1.x / (float) 400, p1.z / (float) 400);
    glNormal3f(normal.x, normal.y, normal.z);
    p1.draw();

    glTexCoord2d(p2.x / (float) 400, p2.z / (float) 400);
    glNormal3f(normal.x, normal.y, normal.z);
    p2.draw();

    glTexCoord2d(p3.x / (float) 400, p3.z / (float) 400);
    glNormal3f(normal.x, normal.y, normal.z);
    p3.draw();
}

void Polygon::countNormal() {
    float x1 = p1.x, x2 = p2.x, x3 = p3.x;
    float y1 = p1.y, y2 = p2.y, y3 = p3.y;
    float z1 = p1.z, z2 = p2.z, z3 = p3.z;

    float X1 = x2 - x1, X2 = x3 - x2;
    float Y1 = y2 - y1, Y2 = y3 - y2;
    float Z1 = z2 - z1, Z2 = z3 - z2;

    float x0 = Y2 * Z1 - Z2 * Y1;
    float y0 = -X2 * Z1 + X1 * Z2;
    float z0 = X2 * Y1 - X1 * Y2;

    normal.x = x0;
    normal.y = y0;
    normal.z = z0;
    normalize();
}

//
// Pyramid
//

struct Pyramid {
    vector<Polygon> body;
    vector<Polygon> bottom;
    int vertNum, layersN;
    float totalHeight, angleDeg, r;
    Point center;
    Point speed = Point{0.1, 0.1, 0};

    void countPyramid();

    void addPolygon(Polygon p);

    void initVAO();

    vector<Point> breakPoints;

    void animate();

    void rotate(GLfloat x, GLfloat y) {
//        for (int i = 0; i < body.size(); i++) {
//            body[i].p1.rotateX(center, x);
//            body[i].p2.rotateX(center, x);
//            body[i].p3.rotateX(center, x);
//
//            body[i].p1.rotateY(center, y);
//            body[i].p2.rotateY(center, y);
//            body[i].p3.rotateY(center, y);
//        }

        for (int i = 0; i < breakPoints.size(); i++) {
            breakPoints[i].rotateX(center, x);
            breakPoints[i].rotateY(center, y);
        }
    }

    void drawBreakPoints();
};

void Pyramid::initVAO() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    for (int j = 1; j < layersN * 2 + 1; j++) {
        for (int i = 0; i < vertNum; i++) {
            body[j * vertNum + i].draw(i, j);
        }
    }
    for (int i = 0; i < vertNum; i++) {
        body[i].drawBottom();
    }
    glEnd();

}

void Pyramid::drawBreakPoints(){
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int i = 0; i < (vertNum + 1); i++) {
        glColor3f(0, 1, 0);
        glVertex3f(breakPoints[i].x, breakPoints[i].y, breakPoints[i].z);
    }
    glEnd();
}

void Pyramid::addPolygon(Polygon p) {
    body.push_back(p);
}

void Pyramid::
countPyramid() {
    body.clear();
    bottom.clear();
    breakPoints.clear();
    vector<vector<GLfloat>> v;
    vector<vector<GLfloat>> base;
    for (int i = 0; i < vertNum; i++) {
        base.push_back(vector<GLfloat>{
                static_cast<float>(cos(2 * M_PI / (float) vertNum * i)),
                center.y,
                static_cast<float>(sin(2 * M_PI / (float) vertNum * i))
        });
    }

    GLfloat h = totalHeight / (float) layersN;
    GLfloat angleRad = angleDeg * M_PI / 180.0;
    GLfloat scale = r / (float) layersN;
    breakPoints.push_back(Point{center.x, center.y + totalHeight, center.z});
    for (int j = 0; j < layersN + 1; j++) {
        vector<vector<GLfloat>> rv;
        for (int i = 0; i < vertNum; i++) {
            GLfloat x = (r - scale * j) * base[i][0];
            GLfloat z = (r - scale * j) * base[i][2];

            GLfloat newX = cos(angleRad * j / (float) layersN) * x - sin(angleRad * j / (float) layersN) * z;
            GLfloat newZ = sin(angleRad * j / (float) layersN) * x + cos(angleRad * j / (float) layersN) * z;

            rv.push_back(vector<GLfloat>{
                    newX + center.x,
                    center.y,
                    newZ + center.z});
        }

        if (j > 0) {
            for (int i = 0; i < vertNum; i++) {
                Point color = {static_cast<GLfloat>((cos(i) + 1) / 2), static_cast<GLfloat>((sin(i) + 1) / 2), 0.7f};
                Polygon p1 = Polygon{
                        v[i][0], center.y + j * h, v[i][2],
                        v[(i + 1) % vertNum][0], center.y + j * h, v[(i + 1) % vertNum][2],
                        rv[(i + 1) % vertNum][0], center.y + (j + 1) * h, rv[(i + 1) % vertNum][2],
                        color,
                };

                Polygon p2 = Polygon{
                        v[i][0], center.y + j * h, v[i][2],
                        rv[(i + 1) % vertNum][0], center.y + (j + 1) * h, rv[(i + 1) % vertNum][2],
                        rv[i][0], center.y + (j + 1) * h, rv[i][2],
                        color,
                };
                p1.countNormal();
                p2.countNormal();
                Point n = p1.normal;
                n.x = (p1.normal.x + p2.normal.x) / (float) 2;
                n.y = (p1.normal.y + p2.normal.y) / (float) 2;
                n.z = (p1.normal.z + p2.normal.z) / (float) 2;
                p1.normal = n;
                p2.normal = n;
                addPolygon(p1);
                addPolygon(p2);

            }

        }
        if (j == 0) {
            for (int i = 0; i < vertNum; i++) {
                Point color = {static_cast<GLfloat>((cos(i) + 1) / 2), static_cast<GLfloat>((sin(i) + 1) / 2), 0.7f};
                Polygon p1 = Polygon{
                        rv[i][0], center.y + (j + 1) * h, rv[i][2],
                        center.x, center.y + (j + 1) * h, center.z,
                        rv[(i + 1) % vertNum][0], center.y + (j + 1) * h, rv[(i + 1) % vertNum][2],
                        color,
                };
                p1.countNormal();
                addPolygon(p1);
                breakPoints.push_back(Point{rv[i][0], center.y + (j + 1) * h, rv[i][2]});
            }
        }
        v = rv;
    }
}

GLfloat EPS = 0.00001;

void Pyramid::animate() {
    for (int i = 0; i < breakPoints.size(); i++) {
        if (abs(breakPoints[i].x + translationX) >= multy) {
            speed.x *= (m1 - m2) / (float) (m1 + m2);
            if (breakPoints[i].x  + translationX < 0) {
                translationX = - breakPoints[i].x - multy;
            } else {
                translationX = - breakPoints[i].x + multy;
            }
            break;
        }
        if (abs(breakPoints[i].y + translationY) >= multy) {
            speed.y *= (m1 - m2) / (float) (m1 + m2);
            if (breakPoints[i].y  + translationY < 0) {
                translationY = - breakPoints[i].y - multy;
            } else {
                translationY = - breakPoints[i].y + multy;
            }
            break;
        }
        if (abs(breakPoints[i].z + translationZ) >= multy) {
            speed.z *= (m1 - m2) / (float) (m1 + m2);
            if (breakPoints[i].z  + translationZ < 0) {
                translationZ = - breakPoints[i].z - multy;
            } else {
                translationZ = - breakPoints[i].z + multy;
            }
            break;
        }

    }
    translationX += speed.x;
    translationY += speed.y;
    translationZ += speed.z;
}