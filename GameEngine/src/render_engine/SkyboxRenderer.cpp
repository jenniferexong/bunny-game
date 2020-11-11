
#include <gl/glew.h>

#include "SkyboxRenderer.h"

#include "../Application.h"
#include "../Location.h"

const float SkyboxRenderer::cube_size = 500.f;

SkyboxRenderer::SkyboxRenderer()
{
	printf("create skybox renderer\n");
	cube_ = Application::loader.loadToVao(vertex_positions, 3);
	texture_id_ = Application::loader.loadCubeMap(texture_names);
	shader_.setUp();
}

// order of textures for faces must be: right, left, top, bottom, back, front
const std::vector<std::string> SkyboxRenderer::texture_names = {
	"side", "side", "top", "bottom", "side", "side"
};

void SkyboxRenderer::render()
{
	shader_.start();
	//glDepthMask(GL_FALSE);
	//glDepthRange(1.0, 1.0);

	glBindVertexArray(cube_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);

	shader_.loadUniforms();

	glDrawArrays(GL_TRIANGLES, 0, cube_.getVertexCount());

	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	//glDepthRange(0.0, 1.0);
	//glDepthMask(GL_TRUE);

	shader_.stop();
}


const std::vector<float> SkyboxRenderer::vertex_positions = {
		-cube_size,  cube_size, -cube_size,
		-cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,
		 cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,

		-cube_size, -cube_size,  cube_size,
		-cube_size, -cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size,  cube_size,
		-cube_size, -cube_size,  cube_size,

		 cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,

		-cube_size, -cube_size,  cube_size,
		-cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size, -cube_size,  cube_size,
		-cube_size, -cube_size,  cube_size,

		-cube_size,  cube_size, -cube_size,
		 cube_size,  cube_size, -cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		-cube_size,  cube_size,  cube_size,
		-cube_size,  cube_size, -cube_size,

		-cube_size, -cube_size, -cube_size,
		-cube_size, -cube_size,  cube_size,
		 cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,
		-cube_size, -cube_size,  cube_size,
		 cube_size, -cube_size,  cube_size
};
