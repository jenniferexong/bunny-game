#pragma once

#include "Mesh.h"
#include "Texture.h"

class Shader;

/* Raw model, only a mesh, no texture or colour */
class Model {
protected:
	Mesh m_mesh;

public:
	Model(Mesh mesh) : m_mesh(mesh) {}
	Mesh getMesh() const { return m_mesh; }
};

class TexturedModel : public Model {
private:
	ModelTexture m_texture;

public:
	TexturedModel(Mesh mesh, ModelTexture texture) : Model(mesh), m_texture(texture) {}

	ModelTexture getTexture() const { return m_texture; }
};

