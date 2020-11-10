#pragma once

#include "Mesh.h"
#include "Texture.h"

class Shader;

/* Raw model, only a mesh, no texture or colour */
class Model {
protected:
	InstancedMesh mesh_;

public:
	Model(InstancedMesh mesh) : mesh_(mesh) {}
	InstancedMesh getMesh() const { return mesh_; }
};

class TexturedModel : public Model {
private:
	ModelTexture texture_;

public:
	TexturedModel(InstancedMesh mesh, ModelTexture texture) : Model(mesh), texture_(texture) {}

	ModelTexture getTexture() const { return texture_; }
};

