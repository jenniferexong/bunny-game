#include "WaterShader.h"

#include "../renderers/MasterRenderer.h"
#include "../Utility.h"
#include "../Location.h"
#include "../Camera.h"
#include "../Environment.h"

void WaterShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void WaterShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	locations_.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	locations_.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	locations_.insert({ UniformVariable::CameraPosition, getUniformLocation("uCameraPosition") });

	locations_.insert({ UniformVariable::Reflection, getUniformLocation("uReflection") });
	locations_.insert({ UniformVariable::Refraction, getUniformLocation("uRefraction") });
	locations_.insert({ UniformVariable::DistortionMap, getUniformLocation("uDistortionMap") });
	locations_.insert({ UniformVariable::NormalMap, getUniformLocation("uNormalMap") });
	locations_.insert({ UniformVariable::DepthMap, getUniformLocation("uDepthMap") });
	locations_.insert({ UniformVariable::MoveFactor, getUniformLocation("uMoveFactor") });

	locations_.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	locations_.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });

	locations_.insert({ UniformVariable::NearPlane, getUniformLocation("uNearPlane") });
	locations_.insert({ UniformVariable::FarPlane, getUniformLocation("uFarPlane") });

	Error::gl_check("WaterShader getAllUniformLocations");
}

void WaterShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::gl_check("WaterShader setup");
}

void WaterShader::connectTextureUnits()
{
	loadInt(locations_.at(UniformVariable::Reflection), WaterTextureLocation::Reflection);
	loadInt(locations_.at(UniformVariable::Refraction), WaterTextureLocation::Refraction);
	loadInt(locations_.at(UniformVariable::DistortionMap), WaterTextureLocation::DistortionMap);
	loadInt(locations_.at(UniformVariable::NormalMap), WaterTextureLocation::NormalMap);
	loadInt(locations_.at(UniformVariable::DepthMap), WaterTextureLocation::DepthMap);

	Error::gl_check("WaterShader connectTextureUnits");
}

void WaterShader::loadUniformPerFrame(const Environment& environment)
{
	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// Near and far planes
	loadFloat(locations_.at(UniformVariable::NearPlane), MasterRenderer::near_plane);
	loadFloat(locations_.at(UniformVariable::FarPlane), MasterRenderer::far_plane);

	// View matrix
	Camera camera = *environment.getCamera().lock();
	glm::mat4 v_matrix = Maths::createViewMatrix(camera);
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadVector(locations_.at(UniformVariable::CameraPosition), camera.getPosition());

	loadFloat(locations_.at(UniformVariable::MoveFactor), Water::move_factor);

	loadVector(locations_.at(UniformVariable::LightColor), environment.getSun().lock()->getColor());
	loadVector(locations_.at(UniformVariable::LightPosition), environment.getSun().lock()->getPosition());

	Error::gl_check("WaterShader loadUniformPerFrame");
}

void WaterShader::loadModelMatrix(const Water& water)
{
	glm::vec3 position = vec3(water.getX(), Water::height, water.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(position, vec3(0), Water::tile_size, glm::mat4(1.f));
	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
	Error::gl_check("WaterShader loadModelMatrix");
}





