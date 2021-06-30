#include "WaterShader.h"

#include "../renderers/MasterRenderer.h"
#include "../util/Log.h"
#include "../util/Maths.h"
#include "../Location.h"
#include "../environment/Environment.h"
#include "../Camera.h"

void WaterShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void WaterShader::getAllUniformLocations()
{
	INSERT_LOC(TransformationMatrix, "uTransformationMatrix");
	INSERT_LOC(ProjectionMatrix, "uProjectionMatrix");
	INSERT_LOC(ViewMatrix, "uViewMatrix");
	INSERT_LOC(CameraPosition, "uCameraPosition");
	INSERT_LOC(Reflection, "uReflection");
	INSERT_LOC(Refraction, "uRefraction");
	INSERT_LOC(DistortionMap, "uDistortionMap");
	INSERT_LOC(NormalMap, "uNormalMap");
	INSERT_LOC(DepthMap, "uDepthMap");
	INSERT_LOC(MoveFactor, "uMoveFactor");
	INSERT_LOC(LightColor, "uLightColor");
	INSERT_LOC(LightPosition, "uLightPosition");
	INSERT_LOC(NearPlane, "uNearPlane");
	INSERT_LOC(FarPlane, "uFarPlane");
	Error::glCheck("WaterShader getAllUniformLocations");
}

void WaterShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::glCheck("WaterShader setup");
}

void WaterShader::connectTextureUnits()
{
	loadInt(
		locations_.at(UniformVariable::Reflection),
		WaterTextureLocation::Reflection
	);
	loadInt(
		locations_.at(UniformVariable::Refraction), 
		WaterTextureLocation::Refraction
	);
	loadInt(
		locations_.at(UniformVariable::DistortionMap),
		WaterTextureLocation::DistortionMap
	);
	loadInt(
		locations_.at(UniformVariable::NormalMap),
		WaterTextureLocation::NormalMap
	);
	loadInt(
		locations_.at(UniformVariable::DepthMap),
		WaterTextureLocation::DepthMap
	);
	Error::glCheck("WaterShader connectTextureUnits");
}

void WaterShader::loadUniformPerFrame(const Environment& environment)
{
	// Loading projection matrix
	loadMatrix(
		locations_.at(UniformVariable::ProjectionMatrix),
		MasterRenderer::projection_matrix
	);

	// Near and far planes
	loadFloat(
		locations_.at(UniformVariable::NearPlane), MasterRenderer::near_plane
	);
	loadFloat(
		locations_.at(UniformVariable::FarPlane), MasterRenderer::far_plane
	);

	// View matrix
	Camera camera = *environment.getCamera().lock();
	glm::mat4 v_matrix = Maths::createViewMatrix(camera);
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadVector(
		locations_.at(UniformVariable::CameraPosition), camera.getPosition()
	);

	loadFloat(locations_.at(UniformVariable::MoveFactor), Water::move_factor);

	loadVector(
		locations_.at(UniformVariable::LightColor),
		environment.getSun().lock()->getColor()
	);
	loadVector(
		locations_.at(UniformVariable::LightPosition),
		environment.getSun().lock()->getPosition()
	);

	Error::glCheck("WaterShader loadUniformPerFrame");
}

void WaterShader::loadModelMatrix(const Water& water)
{
	glm::vec3 position = vec3(water.getX(), Water::height, water.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(
		position, vec3(0), Water::tile_size, glm::mat4(1.f)
	);
	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
	Error::glCheck("WaterShader loadModelMatrix");
}
