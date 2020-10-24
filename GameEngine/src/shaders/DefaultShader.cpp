
#include "DefaultShader.h"

#include "../AttributeLocation.h"

const std::string DefaultShader::s_vertex_file = "res/shaders/default_vert.glsl";
const std::string DefaultShader::s_fragment_file = "res/shaders/default_frag.glsl";

void DefaultShader::bindAttributes() {
	bindAttribute(ePosition, "position");
	bindAttribute(eTexture, "textureCoords");
}

