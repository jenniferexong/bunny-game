#pragma once

#include <string>

#include "FontData.h"

class FontType {
private:
	int texture_atlas_ = -1;
	FontData font_data_;

public:
	FontType() = default;
	FontType(int texture_atlas, const std::string& font_name);
	FontType(const std::string& font_name);

	int getTexture() const { return texture_atlas_; }
	const FontData& getData() const { return font_data_; }

	// equality
	bool operator==(const FontType& other) const {
		return texture_atlas_ == other.texture_atlas_;
	}
};
