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
void Drawer::drawTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, TGAColor color) {
    if (a.y > b.y) std::swap(a, b);
    if (a.y > c.y) std::swap(a, c);
    if (b.y > c.y) std::swap(b, c);

    bool half;
    int seg_height;
    int total_height = c.y - a.y;
    float alpha, beta;
    glm::vec2 A, B;

    for (int i = 0; i < total_height; ++i) {
        half = i > b.y - a.y || b.y == a.y;
        seg_height = (half) ? (c.y - b.y) : (b.y - a.y);

        alpha = (float)i / total_height;
        beta = (float)(i - ((half) ? (b.y - a.y) : 0)) / seg_height;

        A = a + ((c - a) * alpha);
        B = (half) ? (b + (c - b) * beta) : (a + (b - a) * beta);

        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; ++j) {
            plane.set(j, a.y + i, color);
        }
    }
}

void Drawer::drawWireframe(Model* m, TGAColor color) {
    glm::vec3 v0, v1;
    glm::vec2 from, to;
    for (int i = 0; i < m->nfaces(); ++i) {
        std::vector<int> face = m->face(i);
        for (int j = 0; j < 3; ++j) {
            v0 = m->vert(face[j]);
            v1 = m->vert(face[(j + 1) % 3]);

            from = glm::vec2((v0.x + 1.) * width / 2., (v0.y + 1.) * height / 2.);
            to = glm::vec2((v1.x + 1.) * width / 2., (v1.y + 1.) * height / 2.);

            lineBrezenham(from, to, color);
        }
    }
}
void Drawer::drawModel(Model* m, TGAColor color) {
    glm::vec3 dots[3];
    glm::vec3 n;
    glm::vec3 light = glm::normalize(glm::vec3(1, 0, -1));
    glm::vec3 world_coords[3];
    glm::uvec2 screen_coords[3];
    std::vector<int> face;

    for (int i = 0; i < m->nfaces(); ++i) {
        face = m->face(i);
        for (int j = 0; j < 3; ++j) {
            world_coords[j] = m->vert(face[j]);
            screen_coords[j] = glm::vec2((world_coords[j].x + 1.) * width / 2.,
                                        (world_coords[j].y + 1.) * height / 2.);
        }

        n = glm::cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        float intensity = glm::dot(light, glm::normalize(n));
        if (intensity > 0) {
            drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], color * intensity);
        }
    }
}