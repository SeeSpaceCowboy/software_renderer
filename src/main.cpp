// #include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "tgaimage.h"
#include "model.h"
#include "drawer.h"

const int width = 1024;
const int height = 1024;

const TGAColor white = TGAColor(0xFF, 0xFF, 0xFF, 0xFF);
const TGAColor black = TGAColor(0x00, 0x00, 0x00, 0xFF);
const TGAColor gray  = TGAColor(0x40, 0x40, 0x40, 0x40);
const TGAColor red   = TGAColor(0xFF, 0x00, 0x00, 0xFF);

int main(int argc, char** argv) {
    Drawer drawer(width, height);

    std::string filepath;
    std::ifstream in("data/model_list.txt");
    std::vector<Model*> model;

    if (!in.is_open()) {
        std::cerr << "Error: model_list.txt doesn't exist!";
        return 1;
    }
    
    while (std::getline(in, filepath)) model.push_back(new Model(filepath));
    for (Model* m : model) {
        drawer.drawModel(m, white);
        // drawer.drawWireframe(m, gray);
    }
    return 0;
}

