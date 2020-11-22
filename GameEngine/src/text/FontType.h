#pragma once

#include <string>


#include "FontData.h"
#include "TextLoader.h"
#include "TextMeshData.h"

class GuiText;


class FontType {
private:
	int texture_atlas_;
	FontData font_data_;

public:
	FontType() = default;
	FontType(int texture_atlas, const std::string& font_name);

	int getTexture() const { return texture_atlas_; }

	// equality
	bool operator==(const FontType& other) const {
		return texture_atlas_ == other.texture_atlas_;
	}
};
