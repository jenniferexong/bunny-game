#pragma once

#include "Mesh.h"
#include "Texture.h"

/* Raw model, only a mesh, no texture or colour */
class Model {
protected:
	InstancedMesh mesh_;

public:
	Model() = default;
	~Model() = default;
	Model(InstancedMesh mesh) : mesh_(mesh) {}
	InstancedMesh getMesh() const { return mesh_; }
};

class TexturedModel : public Model {
private:
	ModelTexture texture_;

public:
	TexturedModel(InstancedMesh mesh, ModelTexture texture): 
		Model(mesh), texture_(texture) {}

	TexturedModel() = default;
	~TexturedModel() = default;

	ModelTexture getTexture() const { return texture_; }

	// equality
	bool operator==(const TexturedModel& other) const 
	{
		return texture_.getTextureId() == other.texture_.getTextureId();
	}
};

