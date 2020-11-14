#pragma once

#include "models/Model.h"

struct TexturedModelHash {
	size_t operator() (const std::shared_ptr<TexturedModel>& model) const
	{
		return std::hash<int>{}(model->getTexture().getTextureId());
	}
};

struct TexturedModelEquality {
	bool operator()(const std::shared_ptr<TexturedModel>& a, const std::shared_ptr<TexturedModel>& b) const {
		return a->getTexture().getTextureId() == b->getTexture().getTextureId();
	}
};
