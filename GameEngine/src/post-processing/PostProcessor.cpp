#include <GL/glew.h>

#include "PostProcessor.h"

#include "../Location.h"
#include "../Application.h"

const std::vector<float> PostProcessor::positions = { -1, 1, -1, -1, 1, 1, 1, -1 };
Mesh PostProcessor::quad = Mesh();

PostProcessor::PostProcessor()
{
	quad = Application::loader->loadToVao(positions, 2);
}

void PostProcessor::process()
{
	fbo_.unbind();
	glBindVertexArray(quad.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);
	glDisable(GL_DEPTH_TEST);

	contrast_.render(fbo_.getColorTexture());

	glEnable(GL_DEPTH_TEST);	
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);
}


