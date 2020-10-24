#pragma once

#include <memory>

#include "render_engine/Model.h"

class Entity {
private:
	std::shared_ptr<Model> m_model; // pointer for run time polymorphism

public:
	Entity(std::shared_ptr<Model> model) : m_model(model) {}
	void render();
};
