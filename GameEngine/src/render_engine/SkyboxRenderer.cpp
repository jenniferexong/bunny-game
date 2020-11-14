
#include <gl/glew.h>

#include "SkyboxRenderer.h"

#include "../Application.h"
#include "../Location.h"

const float SkyboxRenderer::cube_size = 500.f;

SkyboxRenderer::SkyboxRenderer()
{
	printf("create skybox renderer\n");
	cube_ = Application::loader->loadToVao(vertex_positions, 3);
	day_texture_id_ = Application::loader->loadCubeMap(sky_texture_names);
	night_texture_id_ = Application::loader->loadCubeMap(star_texture_names);
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
	"stars-right", "stars-left", "stars-top", "stars-bottom", "stars-back", "stars-front"
};


void SkyboxRenderer::render(const shared_ptr<Scene>& scene)
{
	shader_.start();

	glBindVertexArray(cube_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	bindTextures(scene);

	shader_.loadUniforms(*scene->getEnvironment().getCamera());

	glDrawArrays(GL_TRIANGLES, 0, cube_.getVertexCount());

	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
}

void SkyboxRenderer::bindTextures(const shared_ptr<Scene>& scene)
{
	// TODO: fix fog?
	float min_sunlight = 0.2f;
	float max_sunlight = 0.9f;
	float sunlight = 0.f;
	time += Application::frame_delta * 1000;
	time = fmod(time, 24000.f);
	int texture_1;
	int texture_2;
	float blend_factor;
	// Night
	if (time >= 0 && time < 5000) {
		texture_1 = night_texture_id_;
		texture_2 = night_texture_id_;
		blend_factor = (time - 0) / (5000 - 0);
		sunlight = min_sunlight;
	}
	// sunrise
	else if (time >= 5000 && time < 8000) {
		texture_1 = night_texture_id_;
		texture_2 = day_texture_id_;
		blend_factor = (time - 5000) / (8000 - 5000);
		sunlight = min_sunlight + (blend_factor * (max_sunlight - min_sunlight));
	}
	// day
	else if (time >= 8000 && time < 21000) {
		texture_1 = day_texture_id_;
		texture_2 = day_texture_id_;
		blend_factor = (time - 8000) / (21000 - 8000);
		sunlight = max_sunlight;
	}
	// sunset
	else {
		texture_1 = day_texture_id_;
		texture_2 = night_texture_id_;
		blend_factor = (time - 21000) / (24000 - 21000);
		sunlight = max_sunlight - (blend_factor * (max_sunlight - min_sunlight));
	}
	scene->getEnvironment().getSun()->setColor(vec3(sunlight));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_2);
	shader_.loadBlendFactor(blend_factor);
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
