#pragma once

#include "Mesh.h"
#include "Texture.h"

class Shader;

/* Raw model, only a mesh, no texture or colour */
class Model {
protected:
	Mesh mesh_;

public:
	Model(Mesh mesh) : mesh_(mesh) {}
	Mesh getMesh() const { return mesh_; }
};

class TexturedModel : public Model {
private:
	ModelTexture texture_;

public:
	TexturedModel(Mesh mesh, ModelTexture texture) : Model(mesh), texture_(texture) {}

	ModelTexture getTexture() const { return texture_; }
};

