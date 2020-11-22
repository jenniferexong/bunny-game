#pragma once
#include <string>

#include "TextMeshData.h"

class GuiText;

class TextLoader
{
public:
	static const double line_height;
	static const int space_ascii;

	TextLoader() = default;
	TextLoader(const std::string& font_name);

/*
 * Takes in an unloaded text and calculates all of the vertices
 * for the quads on which the text will be rendered.
 */ 
	TextMeshData loadData(const GuiText& text); // createTextMesh
};

