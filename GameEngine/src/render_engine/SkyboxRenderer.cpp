
#include <gl/glew.h>

#include "SkyboxRenderer.h"

#include "../Application.h"
#include "../Location.h"

const float SkyboxRenderer::cube_size = 700.f;

SkyboxRenderer::SkyboxRenderer()
{
	printf("create skybox renderer\n");
	cube_ = Application::loader.loadToVao(vertex_positions, 3);
	sky_texture_id_ = Application::loader.loadCubeMap(sky_texture_names);
	star_texture_id_ = Application::loader.loadCubeMap(star_texture_names);
	shader_.setUp();

	shader_.start();
	shader_.connectTextureUnits();
	shader_.stop();
}

// order of textures for faces must be: right, left, top, bottom, back, front
const std::vector<std::string> SkyboxRenderer::sky_texture_names = {
	"side", "side", "top", "bottom", "side", "side"
};

const std::vector<std::string> SkyboxRenderer::star_texture_names = {
	"stars-right", "stars-left", "stars-right", "stars-left", "stars-front", "stars-back"
};

void SkyboxRenderer::render()
{
	shader_.start();

	glBindVertexArray(cube_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	bindTextures();

	shader_.loadUniforms();

	glDrawArrays(GL_TRIANGLES, 0, cube_.getVertexCount());

	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
}

void SkyboxRenderer::bindTextures()
{
	glActiveTexture(GL_TEXTURE0 + CubeMapLocation::Sky);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture_id_);

	glActiveTexture(GL_TEXTURE0 + CubeMapLocation::Stars);
	glBindTexture(GL_TEXTURE_CUBE_MAP, star_texture_id_);
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
