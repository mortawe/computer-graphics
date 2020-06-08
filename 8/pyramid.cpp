#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include "config.h"
#include "memory.h"
#include "config.h"

using namespace std;
GLfloat multy = 3;
GLfloat m1 = 1, m2 = 10;

//
// Pyramid
//

class Pyramid {
public:
    unsigned int VBO, VAO;
    int vertNum, layersN;
    float totalHeight, angleDeg, r;
    float scale = 0.5;
    GLuint list;
    GLfloat *vertices= NULL;
    GLfloat *bp= NULL; // break points
    GLfloat *normals = NULL;
    GLfloat *texCoord = NULL;

    Pyramid(int vertNum, int layersN, float totalH, float angle, float r) :
            vertNum(vertNum), layersN(layersN), totalHeight(totalH), angleDeg(angle), r(r) {};

    GLfloat translationX = 0.0f;
    GLfloat translationY = 0.0f;
    GLfloat translationZ = 0.0f;
    GLfloat rotationX = 0.0f;
    GLfloat rotationY = 0.0f;
    GLfloat rotationZ = 0.0f;
    struct speed {
        GLfloat x, y, z;
    } speed{0.005, 0.001, 0.002};

    void count();

    void destroy();

    void animate();

    void rotateX(GLfloat p);

    void rotateY(GLfloat p);

    void drawBP(){
        glBegin(GL_POINTS);
        glPointSize(10);
        glColor3f(0,1,0);
        for (int i = 0; i < vertNum + 1; i++ ) {
            glVertex3f(bp[i * 3 + 0],bp[i * 3 + 1],bp[i * 3 + 2]);
        }
        glEnd();
    }


    void countNormals();

    void countTexCoords();

    void initVAO();


    void initVAO(bool isTextOn);
};


void Pyramid::initVAO(bool isTextOn) {
    GLfloat *v = (GLfloat*)malloc( layersN * vertNum * 2 * 3 * 5 * sizeof(GLfloat));

    for (int i = 0; i < layersN * vertNum * 2 * 3; i++) {
        v[i * 5 + 0] = vertices[i * 3 + 0];
        v[i * 5 + 1] = vertices[i * 3 + 1];
        v[i * 5 + 2] = vertices[i * 3 + 2];
        v[i * 5 + 3] = texCoord[i * 2 + 0];
        v[i * 5 + 4] = texCoord[i * 2 + 1];
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, layersN * vertNum * 2 * 3 * 5 * sizeof(GLfloat), v, GL_STATIC_DRAW);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
    if (isTextOn) {
        glEnableVertexAttribArray(2);
    }
    free(v);
}

void Pyramid::destroy() {
    if (vertices != NULL) {
        free(vertices);
    }
    if (bp != NULL) {
        free(bp);
    }
    if (normals != NULL) {
        free(normals);
    }
    if (texCoord != NULL) {
        free(texCoord);
    }
}

void Pyramid::count() {
    destroy();

    // filling the base
    GLfloat base[vertNum * 3];
    vertices = (GLfloat *) malloc(sizeof(GLfloat) * 2 * vertNum * 3 * 3 * layersN);
    for (int i = 0; i < vertNum; i++) {
        base[i * 3 + 0] = static_cast<float>(cos(2 * M_PI / (float) vertNum * (i + 1)));
        base[i * 3 + 1] = 0;
        base[i * 3 + 2] = static_cast<float>(sin(2 * M_PI / (float) vertNum * (i + 1)));
    }
    // helping values
    GLfloat h = totalHeight / (float) layersN; // height per one layer
    GLfloat angleRad = angleDeg * M_PI / 180.0; // angle in radians
    GLfloat scale = r / (float) layersN; // diam reduce

    // adding the top of the list to break points
    bp = (GLfloat *) malloc(sizeof(GLfloat) * (vertNum + 1) * 3);
    bp[0] = 0;
    bp[1] = totalHeight;
    bp[2] = 0;
    int idx = 0;
    GLfloat *v = NULL;

    for (int j = 0; j < layersN + 1; j++) {
        GLfloat *rv = (GLfloat *) malloc(sizeof(GLfloat) * vertNum * 3); // rotated vertices
        for (int i = 0; i < vertNum; i++) {
            GLfloat x = (r - scale * j) * base[i * 3 + 0];
            GLfloat z = (r - scale * j) * base[i * 3 + 2];

            // rotated x and z
            GLfloat newX = cos(angleRad * j / (float) layersN) * x - sin(angleRad * j / (float) layersN) * z;
            GLfloat newZ = sin(angleRad * j / (float) layersN) * x + cos(angleRad * j / (float) layersN) * z;

            rv[i * 3 + 0] = newX;
            rv[i * 3 + 1] = 0; // computes later
            rv[i * 3 + 2] = newZ;
        }
        if (j == layersN) {
            for (int i = 0; i < vertNum; i++) {
                vertices[idx + 0] = 0;
                vertices[idx + 1] = j * h;
                vertices[idx + 2] = 0;
                idx += 3;

                vertices[idx + 0] = v[i * 3 + 0];
                vertices[idx + 1] = (j - 1) * h;
                vertices[idx + 2] = v[i * 3 + 2];
                idx += 3;

                vertices[idx + 0] = v[(i + 1) % vertNum * 3 + 0];
                vertices[idx + 1] = (j - 1) * h;
                vertices[idx + 2] = v[(i + 1) % vertNum * 3 + 2];
                idx += 3;
            }
        } else {
            if (j > 0) {
                j--;
                for (int i = 0; i < vertNum; i++) {
                    // first triangle
                    vertices[idx + 0] = v[i * 3 + 0];
                    vertices[idx + 1] = j * h;
                    vertices[idx + 2] = v[i * 3 + 2];
                    idx += 3;

                    vertices[idx + 0] = v[(i + 1) % vertNum * 3 + 0];
                    vertices[idx + 1] = j * h;
                    vertices[idx + 2] = v[(i + 1) % vertNum * 3 + 2];
                    idx += 3;

                    vertices[idx + 0] = rv[(i + 1) % vertNum * 3 + 0];
                    vertices[idx + 1] = (j + 1) * h;
                    vertices[idx + 2] = rv[(i + 1) % vertNum * 3 + 2];
                    idx += 3;

                    // second triangle
                    vertices[idx + 0] = v[i * 3 + 0];
                    vertices[idx + 1] = j * h;
                    vertices[idx + 2] = v[i * 3 + 2];
                    idx += 3;

                    vertices[idx + 0] = rv[(i + 1) % vertNum * 3 + 0];
                    vertices[idx + 1] = (j + 1) * h;
                    vertices[idx + 2] = rv[(i + 1) % vertNum * 3 + 2];
                    idx += 3;

                    vertices[idx + 0] = rv[i * 3 + 0];
                    vertices[idx + 1] = (j + 1) * h;
                    vertices[idx + 2] = rv[i * 3 + 2];
                    idx += 3;
                }
                j++;
            }
        }
        if (j == 0) {
            for (int i = 0; i < vertNum; i++) {
                vertices[idx + 0] = rv[i * 3 + 0];
                vertices[idx + 1] = rv[i * 3 + 1];
                vertices[idx + 2] = rv[i * 3 + 2];
                idx += 3;

                vertices[idx + 0] = 0;
                vertices[idx + 1] = 0;
                vertices[idx + 2] = 0;
                idx += 3;

                vertices[idx + 0] = rv[(i + 1) % vertNum * 3 + 0];
                vertices[idx + 1] = rv[(i + 1) % vertNum * 3 + 1];
                vertices[idx + 2] = rv[(i + 1) % vertNum * 3 + 2];
                idx += 3;

                bp[(i + 1) * 3 + 0] = rv[i * 3 + 0];
                bp[(i + 1) * 3 + 1] = rv[i * 3 + 1];
                bp[(i + 1) * 3 + 2] = rv[i * 3 + 2];
            }
        }
        if (v != NULL) {
            free(v);
        }
        v = rv;
    }
    free(v);

    countNormals();
    countTexCoords();
    initVAO(true);
}

void Pyramid::countNormals() {
    normals = (GLfloat *) malloc(sizeof(GLfloat) * 2 * vertNum * 3 * 3 * layersN);
    for (int i = 0; i < 2 * vertNum * layersN * 3; i += 3) {
        float x1 = vertices[i * 3 + 0], x2 = vertices[(i + 1) * 3 + 0], x3 = vertices[(i + 2) * 3 + 0];
        float y1 = vertices[i * 3 + 1], y2 = vertices[(i + 1) * 3 + 1], y3 = vertices[(i + 2) * 3 + 1];
        float z1 = vertices[i * 3 + 2], z2 = vertices[(i + 1) * 3 + 2], z3 = vertices[(i + 2) * 3 + 2];

        float X1 = x2 - x1, X2 = x3 - x2;
        float Y1 = y2 - y1, Y2 = y3 - y2;
        float Z1 = z2 - z1, Z2 = z3 - z2;

        float x0 = Y2 * Z1 - Z2 * Y1;
        float y0 = -X2 * Z1 + X1 * Z2;
        float z0 = X2 * Y1 - X1 * Y2;

        float length = sqrt(x0 * x0 + y0 * y0 + z0 * z0);

        x0 /= length;
        y0 /= length;
        z0 /= length;

        for (int j = 0; j < 3; j++) {
            normals[(i + j) * 3 + 0] = x0;
            normals[(i + j) * 3 + 1] = y0;
            normals[(i + j) * 3 + 2] = z0;
        }
    }
    for (int i = vertNum + (vertNum % 3 * 2 ); i < 2 * vertNum * 3 * layersN - vertNum * 3; i += 6) {
        float x, y, z;
        x = (normals[i * 3 + 0] + normals[((i + 3) * 3) + 0]) / (float) 2;
        y = (normals[i * 3 + 1] + normals[((i + 3) * 3) + 1]) / (float) 2;
        z = (normals[i * 3 + 2] + normals[((i + 3) * 3) + 2]) / (float) 2;
//        x = normals[i * 3 + 0];
//        y = normals[i * 3 + 1];
//        z = normals[i * 3 + 2];
        for (int j = 0; j < 3; j++) {
            normals[(i + j) * 3 + 0] = x;
            normals[((i + j + 3) * 3) + 0] = x;

            normals[(i + j) * 3 + 1] = y;
            normals[((i + j + 3) * 3) + 1] = y;

            normals[(i + j) * 3 + 2] = z;
            normals[((i + j + 3) * 3) + 2] = z;
        }
    }

}

void Pyramid::countTexCoords() {
    texCoord = (GLfloat *) malloc(sizeof(GLfloat) * 2 * vertNum * 3 * 2 * layersN);
    for (int i = 0; i < 2 * vertNum * 3 * layersN; i += 3) {
        texCoord[i * 2 + 0] = 0;
        texCoord[i * 2 + 1] = -1;

        texCoord[(i + 1) * 2 + 0] = 0;
        texCoord[(i + 1) * 2 + 1] = 0;

        texCoord[(i + 2) * 2 + 0] = 1;
        texCoord[(i + 2) * 2 + 1] = 0;
    }
}

void Pyramid::animate() {
    for (int i = 0; i < vertNum + 1; i++) {
        if (abs(bp[i * 3 + 0] + translationX) >= multy) {
            speed.x *= (m1 - m2) / (float) (m1 + m2);
            if (bp[i * 3 + 0] + translationX < 0) {
                translationX = -bp[i * 3 + 0] - multy;
            } else {
                translationX = -bp[i * 3 + 0] + multy;
            }
            break;
        }
        if (abs(bp[i * 3 + 1] + translationY) >= multy) {
            speed.y *= (m1 - m2) / (float) (m1 + m2);
            if (bp[i * 3 + 1] + translationY < 0) {
                translationY = -bp[i * 3 + 1] - multy;
            } else {
                translationY = -bp[i * 3 + 1] + multy;
            }
            break;
        }
        if (abs(bp[i * 3 + 2] + translationZ) >= multy) {
            speed.z *= (m1 - m2) / (float) (m1 + m2);
            if (bp[i * 3 + 2] + translationZ < 0) {
                translationZ = -bp[i * 3 + 2] - multy;
            } else {
                translationZ = -bp[i * 3 + 2] + multy;
            }
            break;
        }

    }
    translationX += speed.x;
    translationY += speed.y;
    translationZ += speed.z;
}

void Pyramid::rotateX(GLfloat p) {
    for (int i = 0; i < vertNum + 1; i++) {
        GLfloat z2 = bp[i * 3 + 2] * cos(p) - bp[i * 3 + 1] * sin(p);
        GLfloat y2 = bp[i * 3 + 1] * cos(p) + bp[i * 3 + 2] * sin(p);
        bp[i * 3 + 2] = z2;
        bp[i * 3 + 1] = y2;
    }
}

void Pyramid::rotateY(GLfloat p) {
    for (int i = 0; i < vertNum + 1; i++) {
        GLfloat z2 = bp[i * 3 + 2] * cos(p) - bp[i * 3 + 0] * sin(p);
        GLfloat x2 = bp[i * 3 + 0] * cos(p) + bp[i * 3 + 2] * sin(p);
        bp[i * 3 + 2] = z2;
        bp[i * 3 + 0] = x2;
    }
}
