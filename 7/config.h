#ifndef COMPUTER_GRAPHICS_CONFIG_H
#define COMPUTER_GRAPHICS_CONFIG_H

#include <fstream>
#include "yaml-cpp/yaml.h"
#include "pyramid.cpp"

using namespace  std;

bool on1, on2, on3 = false;
bool on0 = true;
const char *imagepath = "2.bmp";
bool isTextOn = false;
const GLfloat Speed = 5; // not
GLfloat zoom = 1;
bool isMotionOn = false;
bool isOnTwoSide = false, isModelLocal = false, isChangeView = false;

void readConfig(Pyramid *pyramid) {
    YAML::Node config = YAML::LoadFile("config.yaml");

    on0 = config["on0"].as<bool>();
    on1 = config["on1"].as<bool>();
    on2 = config["on2"].as<bool>();
    on3 = config["on3"].as<bool>();

    isTextOn = config["texture"].as<bool>();
    isOnTwoSide = config["twoSide"].as<bool>();
    isModelLocal = config["modelLocal"].as<bool>();
    isChangeView = config["changeView"].as<bool>();

    pyramid->rotationX = config["rotX"].as<float>();
    pyramid->rotationY = config["rotY"].as<float>();
    pyramid->rotationZ = config["rotZ"].as<float>();

    pyramid->translationX = config["transX"].as<float>();
    pyramid->translationY = config["transY"].as<float>();
    pyramid->translationZ = config["transZ"].as<float>();

    pyramid->speed.x = config["speedX"].as<float>();
    pyramid->speed.y = config["speedY"].as<float>();
    pyramid->speed.z = config["speedZ"].as<float>();

    pyramid->vertNum = config["vertNum"].as<int>();
    pyramid->angleDeg = config["spinAngle"].as<float>();
    pyramid->layersN = config["layers"].as<int>();

    pyramid->count();
}

void writeFile(Pyramid *pyramid){
    YAML::Node node;

    ofstream  fout("config.yaml");

    node["on0"] = on0;
    node["on1"] = on1;
    node["on2"] = on2;
    node["on3"] = on3;

    node["texture"] = isTextOn;
    node["twoSide"] = isOnTwoSide;
    node["modelLocal"] = isModelLocal;
    node["changeView"] = isChangeView;

    node["rotX"] = pyramid->rotationX;
    node["rotY"] = pyramid->rotationY;
    node["rotZ"] = pyramid->rotationZ;

    node["transX"] = pyramid->translationX;
    node["transY"] = pyramid->translationY;
    node["transZ"] = pyramid->translationZ;

    node["speedX"] = pyramid->speed.x;
    node["speedY"] = pyramid->speed.y;
    node["speedZ"] = pyramid->speed.z;

    node["vertNum"] = pyramid->vertNum;
    node["spinAngle"] = pyramid->angleDeg;
    node["layers"] = pyramid->layersN;

    fout << node;
}

#endif //COMPUTER_GRAPHICS_CONFIG_H
