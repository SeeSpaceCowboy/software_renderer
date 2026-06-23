#include <glm/glm.hpp>
#include "drawer.h"

Drawer::Drawer(int w, int h) {
	width = w;
	height = h;
	z_buffer = new int[w * h];
    for (int i = w * h - 1; i >= 0; --i) {
        z_buffer[i] = INT32_MIN;
    }
	plane = TGAImage(w, h, TGAImage::RGB);
}

Drawer::~Drawer() {
	delete[] z_buffer;
	plane.flip_vertically();
	plane.write_tga_file("output.tga");
}

/* Нужно доделать ЦДА
void lineDDA(dot from, dot to, TGAColor color) {
    float diff_x = std::abs(from.x - to.x);
    float diff_y = std::abs(from.y - to.y);
    float move = std::max(diff_x, diff_y);

    diff_x /= move;
    diff_y /= move;

    int i = 0;
    float x = from.x;
    float y = from.y;

    while (++i <= move) {
        plane.set(x, y, color);
        x += move;
        y += move;
    }
} */

void Drawer::lineBrezenham(glm::vec2 from, glm::vec2 to, TGAColor color) {
    int y;
    float t;
    bool steep = false;
    if (std::abs(from.x - to.x) < std::abs(from.y - to.y)) {
        std::swap(from.x, from.y);
        std::swap(to.x, to.y);
        steep = true;
    }
    if (from.x > to.x) std::swap(from, to);

    for (int x = from.x; x <= to.x; ++x) {
        t = (x - from.x) / (float)(to.x - from.x);
        y = from.y * (1. - t) + to.y * t;
        if (steep) {
            plane.set(y, x, color);
        } else {
            plane.set(x, y, color);
        }
    }
}
void Drawer::drawTriangle(Model* m, glm::vec3 dots[3], glm::vec2 uv[3], float intensity) {
    if (dots[0].y > dots[1].y) std::swap(dots[0], dots[1]);
    if (dots[0].y > dots[2].y) std::swap(dots[0], dots[2]);
    if (dots[1].y > dots[2].y) std::swap(dots[1], dots[2]);

    TGAColor color;
    glm::vec3 A, B, P;
    glm::vec2 uvA, uvB, uvP;

    bool half;
    int seg_height, idx;
    int total_height = dots[2].y - dots[0].y;
    float alpha, beta, phi;

    for (int i = 0; i < total_height; ++i) {
        half = (i > dots[1].y - dots[0].y) || (dots[1].y == dots[0].y);
        seg_height = (half) ? (dots[2].y - dots[1].y) : (dots[1].y - dots[0].y);

        alpha = (float)i / total_height;
        beta = (float)(i - ((half) ? (dots[1].y - dots[0].y) : 0)) / seg_height;

        A = dots[0] + ((dots[2] - dots[0]) * alpha);
        B = (half) ? (dots[1] + (dots[2] - dots[1]) * beta) : (dots[0] + (dots[1] - dots[0]) * beta);

        uvA = uv[0] + (uv[2] - uv[0]) * alpha;
        uvB = (half) ? (uv[1] + (uv[2] - uv[1]) * beta) : (uv[0] + (uv[1] - uv[0]) * beta);

        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; ++j) {
            phi = ((B.x == A.x) ? (1.) : (float)(j - A.x) / (B.x - A.x));
            P = A + (B - A) * phi;
            uvP = uvA + (uvB - uvA) * phi;

            idx = P.x + P.y * width;
            if (z_buffer[idx] < P.z) {
                z_buffer[idx] = P.z;
                color = m->diffuse(uvP);
                plane.set(P.x, P.y, color);
            }
        }
    }
}

void Drawer::drawWireframe(Model* m, TGAColor color) {
    glm::vec3 v0, v1;
    glm::vec3 from, to;
    for (int i = 0; i < m->nfaces(); ++i) {
        std::vector<int> face = m->face(i);
        for (int j = 0; j < 3; ++j) {
            v0 = m->vert(face[j]);
            v1 = m->vert(face[(j + 1) % 3]);

            from = glm::vec3((v0.x + 1.) * width / 2., (v0.y + 1.) * height / 2., v0.z);
            to = glm::vec3((v1.x + 1.) * width / 2., (v1.y + 1.) * height / 2., v1.z);

            lineBrezenham(from, to, color);
        }
    }
}
void Drawer::drawModel(Model* m, TGAColor color) {
    glm::vec3 dots[3];
    glm::vec3 n;
    glm::vec3 light = glm::normalize(glm::vec3(1, 0, -2));
    glm::vec3 world_coords[3];
    glm::vec3 coords[3];
    glm::vec2 uv[3];
    std::vector<int> face;

    for (int i = 0; i < m->nfaces(); ++i) {
        face = m->face(i);
        for (int j = 0; j < 3; ++j) {
            world_coords[j] = m->vert(face[j]);
            coords[j] = glm::uvec3((world_coords[j].x + 1.) * width / 2.,
                                        (world_coords[j].y + 1.) * height / 2.,
                                        (world_coords[j].z));
        }

        n = glm::cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        float intensity = glm::dot(light, glm::normalize(n));
        if (intensity > 0) {
            for (int k = 0; k < 3; ++k) {
                uv[k] = m->uv(i, k);
            }
            drawTriangle(m, coords, uv, intensity);
        }
    }
}