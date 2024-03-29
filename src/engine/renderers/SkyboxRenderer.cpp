﻿#include <GL/glew.h>

#include "SkyboxRenderer.h"

#include "../Application.h"
#include "../environment/Environment.h"
#include "../Location.h"
#include "../util/Log.h"

const float SkyboxRenderer::cube_size = 500.f;

SkyboxRenderer::SkyboxRenderer()
{
	Log::init("SkyboxRenderer", false);

	cube_ = app->loader->loadToVao(vertex_positions, 3);
	shader_.setUp();

	shader_.start();
	shader_.connectTextureUnits();
	shader_.stop();

	Log::init("SkyboxRenderer", true);
}

void SkyboxRenderer::render(const Environment &environment)
{
	shader_.start();

	glBindVertexArray(cube_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	bindTextures(*environment.getSkybox().lock());

	shader_.loadUniforms(*environment.getCamera().lock());

	glDrawArrays(GL_TRIANGLES, 0, cube_.getVertexCount());

	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
	Error::glCheck(name_);
}

void SkyboxRenderer::bindTextures(const Skybox &skybox)
{
	int texture_1 = skybox.getTextures().x;
	int texture_2 = skybox.getTextures().y;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_2);
	shader_.loadBlendFactor(skybox.getBlendFactor());
}

const std::vector<float> SkyboxRenderer::vertex_positions = {
	-cube_size, cube_size, -cube_size,
	-cube_size, -cube_size, -cube_size,
	cube_size, -cube_size, -cube_size,
	cube_size, -cube_size, -cube_size,
	cube_size, cube_size, -cube_size,
	-cube_size, cube_size, -cube_size,

	-cube_size, -cube_size, cube_size,
	-cube_size, -cube_size, -cube_size,
	-cube_size, cube_size, -cube_size,
	-cube_size, cube_size, -cube_size,
	-cube_size, cube_size, cube_size,
	-cube_size, -cube_size, cube_size,

	cube_size, -cube_size, -cube_size,
	cube_size, -cube_size, cube_size,
	cube_size, cube_size, cube_size,
	cube_size, cube_size, cube_size,
	cube_size, cube_size, -cube_size,
	cube_size, -cube_size, -cube_size,

	-cube_size, -cube_size, cube_size,
	-cube_size, cube_size, cube_size,
	cube_size, cube_size, cube_size,
	cube_size, cube_size, cube_size,
	cube_size, -cube_size, cube_size,
	-cube_size, -cube_size, cube_size,

	-cube_size, cube_size, -cube_size,
	cube_size, cube_size, -cube_size,
	cube_size, cube_size, cube_size,
	cube_size, cube_size, cube_size,
	-cube_size, cube_size, cube_size,
	-cube_size, cube_size, -cube_size,

	-cube_size, -cube_size, -cube_size,
	-cube_size, -cube_size, cube_size,
	cube_size, -cube_size, -cube_size,
	cube_size, -cube_size, -cube_size,
	-cube_size, -cube_size, cube_size,
	cube_size, -cube_size, cube_size};
