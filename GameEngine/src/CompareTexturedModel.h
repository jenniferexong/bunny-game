#pragma once

#include "models/Model.h"

struct CompareTexturedModel {
	bool operator()(const std::shared_ptr<TexturedModel>& a, const std::shared_ptr<TexturedModel>& b) const {
		return a->getTexture().getTextureId() < b->getTexture().getTextureId();
	}
};
