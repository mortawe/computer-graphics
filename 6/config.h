#ifndef COMPUTER_GRAPHICS_CONFIG_H
#define COMPUTER_GRAPHICS_CONFIG_H

#include <fstream>
#include "yaml-cpp/yaml.h"
#include "pyramid.cpp"

using namespace  std;

bool on1, on2, on3 = false;
bool on0 = true;
const char *imagepath = "../6/1.bmp";
bool isTextOn = true;
const GLfloat Speed = 5; // not
GLfloat zoom = 1;
bool isMotionOn = false;
bool isOnTwoSide = false, isModelLocal = false, isChangeView = false;

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat rotationZ = 0.0f;


void readConfig(Pyramid *pyramid) {
    YAML::Node config = YAML::LoadFile("../7/config.yaml");

    on0 = config["on0"].as<bool>();
    on1 = config["on1"].as<bool>();
    on2 = config["on2"].as<bool>();
    on3 = config["on3"].as<bool>();

    isTextOn = config["texture"].as<bool>();
    isMotionOn = config["motion"].as<bool>();
    isOnTwoSide = config["twoSide"].as<bool>();
    isModelLocal = config["modelLocal"].as<bool>();
    isChangeView = config["changeView"].as<bool>();

    rotationX = config["rotX"].as<float>();
    rotationY = config["rotY"].as<float>();
    rotationZ = config["rotZ"].as<float>();

    translationX = config["transX"].as<float>();
    translationY = config["transY"].as<float>();
    translationZ = config["transZ"].as<float>();

    pyramid->speed.x = config["speedX"].as<float>();
    pyramid->speed.y = config["speedY"].as<float>();
    pyramid->speed.z = config["speedZ"].as<float>();

    pyramid->vertNum = config["vertNum"].as<int>();
    pyramid->angleDeg = config["spinAngle"].as<float>();
    pyramid->layersN = config["layers"].as<int>();

    pyramid->countPyramid();
}

void writeFile(Pyramid *pyramid){
    YAML::Node node;

    ofstream  fout("config.yaml");

    node["on0"] = on0;
    node["on1"] = on1;
    node["on2"] = on2;
    node["on3"] = on3;

    node["texture"] = isTextOn;
    node["motion"] = isMotionOn;
    node["twoSide"] = isOnTwoSide;
    node["modelLocal"] = isModelLocal;
    node["changeView"] = isChangeView;

    node["rotX"] = rotationX;
    node["rotY"] = rotationY;
    node["rotZ"] = rotationZ;

    node["transX"] = translationX;
    node["transY"] = translationY;
    node["transZ"] = translationZ;

    node["speedX"] = pyramid->speed.x;
    node["speedY"] = pyramid->speed.y;
    node["speedZ"] = pyramid->speed.z;

    node["vertNum"] = pyramid->vertNum;
    node["spinAngle"] = pyramid->angleDeg;
    node["layers"] = pyramid->layersN;

    fout << node;
}

#endif //COMPUTER_GRAPHICS_CONFIG_H
