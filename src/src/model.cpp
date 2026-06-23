#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

#include "model.h"

Model::Model(std::string filepath) : verts_(), faces_(), norms_(), uv_() {
    std::string line;
    std::ifstream in;
    in.open(filepath, std::ifstream::in);
    if (in.fail()) return;

    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            glm::vec3 v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            glm::vec3 n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            glm::vec2 uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            uv_.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<glm::uvec3> f;
            glm::uvec3 tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << filepath << " {verticles: " << verts_.size() << ", faces: " << faces_.size();
    std::cerr << ", uv: " << uv_.size() << ", normales: " << norms_.size() << "}\n";
    load_texture(filepath, "_diffuse.tga");
}

void Model::load_texture(std::string filepath, const char* suffix) {
    std::string texfile(filepath);
    size_t dot = texfile.find_last_of(".");

    if (dot != std::string::npos) {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture: " << (diffusemap_.read_tga_file(texfile.c_str()) ? "loadad" : "doesn't exist") << std::endl;
        diffusemap_.flip_vertically();
    }
}

int Model::nverts() {
    return (int)verts_.size();
}
int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i = 0; i < (int)faces_[idx].size(); i++) face.push_back(faces_[idx][i][0]);
    return face;
}
glm::vec3 Model::vert(int i) {
    return verts_[i];
}
TGAColor Model::diffuse(glm::uvec2 uv) {
    return diffusemap_.get(uv.x, uv.y);
}
glm::uvec2 Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert][1];
    return glm::uvec2(uv_[idx].x * diffusemap_.get_width(), uv_[idx].y * diffusemap_.get_height());
}
