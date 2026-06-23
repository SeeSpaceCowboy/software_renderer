#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <glm/glm.hpp>
#include "tgaimage.h"

class Model {
private:
	std::vector<glm::vec3> verts_;
	std::vector<std::vector<glm::uvec3>> faces_;
	std::vector<glm::vec3> norms_;
	std::vector<glm::vec2> uv_;
	TGAImage diffusemap_;

	void load_texture(std::string filename, const char* suffix);
public:
	Model(std::string filepath);

	int nverts();
	int nfaces();
	glm::vec3 vert(int i);
	std::vector<int> face(int idx);
	TGAColor diffuse(glm::uvec2 uv);
	glm::uvec2 uv(int iface, int nvert);
};

#endif //__MODEL_H__
