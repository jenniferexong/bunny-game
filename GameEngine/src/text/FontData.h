#pragma once
#include <string>
#include <unordered_map>

#include "Text.h"

class FontData {
private:
	static const int desired_padding;

	
	std::unordered_map<int, Character> characters_;

	std::string getToken(std::ifstream* file, const std::string& token_name);
	
public:
	FontData() = default;

	FontData(const std::string& font_name);
};
