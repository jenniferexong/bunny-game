#pragma once

#include "Mesh.h"
#include "Texture.h"

class Shader;

/* Raw model, only a mesh, no texture or colour */
class Model {
protected:
	Mesh _mesh;

public:
	Model(Mesh mesh) : _mesh(mesh) {}
	Mesh getMesh() const { return _mesh; }
};

class TexturedModel : public Model {
private:
	ModelTexture _texture;

public:
	TexturedModel(Mesh mesh, ModelTexture texture) : Model(mesh), _texture(texture) {}

	ModelTexture getTexture() const { return _texture; }
};

