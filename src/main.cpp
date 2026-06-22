// #include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

#include "tgaimage.h"
#include "model.h"
#include "drawer.h"

const int width = 1024;
const int height = 1024;

const TGAColor white = TGAColor(0xFF, 0xFF, 0xFF, 0xFF);
const TGAColor black = TGAColor(0x00, 0x00, 0x00, 0xFF);
const TGAColor gray  = TGAColor(0x40, 0x40, 0x40, 0x40);
const TGAColor red   = TGAColor(0xFF, 0x00, 0x00, 0xFF);

Model* model = new Model("obj/african_head.obj");

int main(int argc, char** argv) {
    Drawer drawer(width, height);
    drawer.drawModel(model, white);
    drawer.drawWireframe(model, gray);
    delete model;
    return 0;
}

