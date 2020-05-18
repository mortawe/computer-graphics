//
// Created by master on 02.05.2020.
//
#include "config.h"

uint readTexture(){
    FILE* file = fopen(imagepath, "rb");
    if (!file) {
        return 0;
    }
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        return 0;
    }
    if (header[0] != 'B' || header[1] != 'M') {
        return 0;
    }
    unsigned int dataPos = *(int*)&(header[0x0A]);
    unsigned int imageSize = *(int*)&(header[0x22]);
    unsigned int width = *(int*)&(header[0x12]);
    unsigned int height = *(int*)&(header[0x16]);
//    cout << dataPos << " " << imageSize << " " << width << " " << height << endl;
    if (imageSize == 0) {
        imageSize = width * height * 3;
    }
    if (dataPos == 0) {
        dataPos = 54;
    }
    unsigned char *data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);
    uint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return textureID;
}

void materials(){
    glEnable(GL_COLOR_MATERIAL);
    GLfloat light0_specular[] = {0.633, 0.727811, 0.633};
    GLfloat light0_diffuse[] = {0.07568, 0.61424, 0.07568};
    GLfloat light0_ambient[] = {0.0215, 0.1745, 0.0215};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, light0_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, light0_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, light0_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 1 * 128);
    glShadeModel(GL_SMOOTH);
}