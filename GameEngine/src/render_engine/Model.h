#pragma once

#include "Mesh.h"
#include "Texture.h"


class Model {
protected:
	Mesh m_mesh;

public:
	Model(Mesh mesh) : m_mesh(mesh) {}
	virtual void render() const;
};

class TexturedModel : public Model {
private:
	ModelTexture m_texture;

public:
	TexturedModel(Mesh mesh, ModelTexture texture) : Model(mesh), m_texture(texture) {}
	void render() const override;

	inline Mesh getMesh() { return m_mesh; }
	inline ModelTexture getTexture() { return m_texture; }
};

