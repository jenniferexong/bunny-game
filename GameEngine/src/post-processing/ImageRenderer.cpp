#include <GL/glew.h>

#include "ImageRenderer.h"
#include "../Helper.h"

void ImageRenderer::renderQuad()
{
	if (fbo_.intialised())
		fbo_.bind();

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // single quad

	if (fbo_.intialised())
		fbo_.unbind();
}

