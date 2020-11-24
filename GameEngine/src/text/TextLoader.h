#pragma once
#include <string>

#include "GuiText.h"
#include "Text.h"

struct TextMeshData {
	std::vector<float> positions;
	std::vector<float> texture_coords;

	int getVertexCount() const { return (int)positions.size() / 2; }
};

class TextLoader {
private:
	std::vector<Line> structureText(GuiText& gui_text);
	void addWord(Line* current_line, const Word& current_word, std::vector<Line>& lines, 
		double space_width, double font_size, double max_line_width);

	TextMeshData createQuadData(GuiText& text, const std::vector<Line>& lines);
	void addVertices(const Character& character, double font_size, double cursor_x, double cursor_y, std::vector<float>& positions);
	void addTextureCoords(const Character& character, std::vector<float>& texture_coords);
	void addData(std::vector<float>& data, float x, float y, float max_x, float max_y);
		
public:
	static const double line_height;
	static const int space_ascii;

	//TextLoader(const std::string& font_name);

/*
 * Takes in an unloaded text and calculates all of the vertices
 * for the quads on which the text will be rendered.
 */ 
	TextMeshData createMesh(GuiText& text); // createTextMesh
};
