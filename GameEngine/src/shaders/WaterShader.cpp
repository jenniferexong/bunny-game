#include "WaterShader.h"

#include "../renderers/MasterRenderer.h"
#include "../Location.h"

const std::string WaterShader::vertex_file = "res/shaders/water-vert.glsl";
const std::string WaterShader::fragment_file = "res/shaders/water-frag.glsl";

void WaterShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void WaterShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	locations_.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	locations_.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
}

void WaterShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

void WaterShader::loadUniformPerFrame(const Camera& camera)
{
	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(camera);
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
}

void WaterShader::loadModelMatrix(const Water& water)
{
	glm::vec3 position = vec3(water.getX(), water.getHeight(), water.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(position, vec3(0), Water::tile_size, glm::mat4(1.f));
	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
}





