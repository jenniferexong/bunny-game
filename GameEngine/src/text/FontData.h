#pragma once
#include <string>
#include <unordered_map>

#include "Text.h"

class FontData {
private:
	static const int desired_padding;
	std::unordered_map<int, Character> characters_;
	double space_width_;

	std::string getToken(std::ifstream* file, const std::string& token_name);
	
public:
	FontData() = default;
	FontData(const std::string& font_name);

	double getSpaceWidth() const { return space_width_; }
	Character getCharacter(int ascii) const;
};
