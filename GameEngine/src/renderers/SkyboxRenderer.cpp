
#include <gl/glew.h>

#include "SkyboxRenderer.h"

#include "../Application.h"
#include "../Location.h"

#include "../environment/Skybox.h"

const float SkyboxRenderer::cube_size = 500.f;

SkyboxRenderer::SkyboxRenderer()
{
	cube_ = Application::loader->loadToVao(vertex_positions, 3);
	shader_.setUp();

	shader_.start();
	shader_.connectTextureUnits();
	shader_.stop();
}

void SkyboxRenderer::render(const Environment& environment)
{
	shader_.start();

	glBindVertexArray(cube_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	bindTextures(environment);

	shader_.loadUniforms(*environment.getCamera());

	glDrawArrays(GL_TRIANGLES, 0, cube_.getVertexCount());

	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
}

void SkyboxRenderer::bindTextures(const Environment& environment)
{
	float min_sunlight = 0.2f;
	float max_sunlight = 0.9f;
	float sunlight = 0.f;

	time += Application::frame_delta * 1000;
	time = fmod(time, 24000.f);
	int texture_1;
	int texture_2;
	float blend_factor;
	int day = environment.getSkybox().getDayTexture();
	int night = environment.getSkybox().getNightTexture();
	// Night
	if (time >= 0 && time < 5000) {
		texture_1 = night;
		texture_2 = night;
		blend_factor = (time - 0) / (5000 - 0);
		sunlight = min_sunlight;
	}
	// sunrise
	else if (time >= 5000 && time < 8000) {
		texture_1 = night;
		texture_2 = day;
		blend_factor = (time - 5000) / (8000 - 5000);
		sunlight = min_sunlight + (blend_factor * (max_sunlight - min_sunlight));
	}
	// day
	else if (time >= 8000 && time < 21000) {
		texture_1 = day;
		texture_2 = day;
		blend_factor = (time - 8000) / (21000 - 8000);
		sunlight = max_sunlight;
	}
	// sunset
	else {
		texture_1 = day;
		texture_2 = night;
		blend_factor = (time - 21000) / (24000 - 21000);
		sunlight = max_sunlight - (blend_factor * (max_sunlight - min_sunlight));
	}
	environment.getSun()->setColor(vec3(sunlight));

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
