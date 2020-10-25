#pragma once

#include <memory>

class Entity;
class Shader;

class Renderer {
public:
	void prepare();
	void render(const Entity& entity, const Shader& shader);

};

