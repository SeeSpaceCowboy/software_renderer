#ifndef __DRAWER_H__
#define __DRAWER_H__

#include <iostream>
#include <glm/glm.hpp>

#include "tgaimage.h"
#include "model.h"

class Drawer {
private:
	int width;
	int height;
    int* z_buffer;
	TGAImage plane;
public:
	Drawer(int w, int h);
    ~Drawer();

    void lineBrezenham(glm::vec2 from, glm::vec2 to, TGAColor color);
    void drawTriangle(Model* m, glm::vec3 dot[3], glm::vec2 uv[3], float intensity);

    void drawWireframe(Model* m, TGAColor color);
    void drawModel(Model* m, TGAColor color);
};

#endif // __DRAWER_H__