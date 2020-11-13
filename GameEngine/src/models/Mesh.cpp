
#include "Mesh.h"

void InstancedMesh::setVbos(int model_matrix, int model_brightness)
{
	model_matrix_vbo_ = model_matrix;
	model_brightness_vbo_ = model_brightness;
}

