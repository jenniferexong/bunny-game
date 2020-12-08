#include "TextLoader.h"

#include <iostream>

#include "Text.h"
#include "GuiText.h"
#include "FontData.h"
#include "FontType.h"

using std::string;

TextMeshData TextLoader::createMesh(GuiText& text)
{
	std::vector<Line> lines = structureText(text);
	auto quad_data = createQuadData(text, lines);
	return quad_data;
}

std::vector<Line> TextLoader::structureText(GuiText& gui_text)
{
	std::vector<Line> lines;
	const FontData& font_data = gui_text.getFont().lock()->getData();

	string text = gui_text.getText();
	double font_size = (double) gui_text.getFontSize();
	double max_line_width = (double) gui_text.getMaxLineWidth();
	double space_width = font_data.getSpaceWidth();

	Line current_line = Line(space_width, font_size, max_line_width);
	Word current_word = Word(font_size);

	for (char& c : text) {
		int ascii = (int)c;
		if (ascii == space_ascii) {
			// add the current word to the line, if it fits
			addWord(&current_line, current_word, lines, space_width, font_size, max_line_width);
			current_word = Word(font_size);
			continue;
		}
		else if (c == '\n') {
			addWord(&current_line, current_word, lines, space_width, font_size, max_line_width);
			lines.push_back(current_line);
			current_line = Line(space_width, font_size, max_line_width);
			current_word = Word(font_size);
			continue;
		}
		Character character = font_data.getCharacter(ascii);
		current_word.addCharacter(character);
	}

	// finishing
	addWord(&current_line, current_word, lines, space_width, font_size, max_line_width);
	lines.push_back(current_line);

	return lines;
}

void TextLoader::addWord(Line* current_line, const Word& current_word, std::vector<Line>& lines, 
	double space_width, double font_size, double max_line_width)
{
	bool added = current_line->addWord(current_word);
	if (!added) { // make new line and add word to it
		lines.push_back(*current_line);
		*current_line = Line(space_width, font_size, max_line_width);
		current_line->addWord(current_word);
	}
}

using std::vector;
TextMeshData TextLoader::createQuadData(GuiText& text, const vector<Line>& lines)
{
	text.setLineCount(lines.size());

	double cursor_x = 0;
	double cursor_y = 0;
	vector<float> positions;
	vector<float> texture_coords;
	float font_size = text.getFontSize();

	for (const auto& line: lines) {
		if (text.isCentered())
			cursor_x = (line.getMaxWidth() - line.getCurrentWidth()) / 2.0;

		for (const auto& word: line.getWords()) {
			for (const auto& letter: word.getCharacters()) {
				addVertices(letter, font_size, cursor_x, cursor_y, positions);
				addTextureCoords(letter, texture_coords);
				cursor_x += letter.getAdvance() * font_size;
			}
			// space
			cursor_x += text.getFont().lock()->getData().getSpaceWidth() * font_size; 
		}
		cursor_x = 0;
		cursor_y += line_height * font_size;
	}
	return { positions, texture_coords };
}

void TextLoader::addVertices(const Character& character, double font_size, double cursor_x, double cursor_y, std::vector<float>& positions)
{
	double x = cursor_x + (character.getOffset().x * font_size);
	double y = cursor_y + (character.getOffset().y * font_size);
	double max_x = x + (character.getSize().x * font_size);
	double max_y = y + (character.getSize().y * font_size);

	// to screen space?
	x = (2 * x) - 1;
	y = (-2 * y) + 1;
	max_x = (2 * max_x) - 1;
	max_y = (-2 * max_y) + 1;

	addData(positions, (float) x, (float) y, (float) max_x, (float) max_y);
}

void TextLoader::addTextureCoords(const Character& character, std::vector<float>& texture_coords)
{
	float x = (float) character.getTextureCoords().x;
	float y = (float) character.getTextureCoords().y;
	float max_x = (float) character.getMaxTextureCoords().x;
	float max_y = (float) character.getMaxTextureCoords().y;
	addData(texture_coords, x, y, max_x, max_y);
}

void TextLoader::addData(vector<float>& data, float x, float y, float max_x, float max_y)
{
	data.push_back(x);
	data.push_back(y);
	data.push_back(x);
	data.push_back(max_y);
	data.push_back(max_x);
	data.push_back(max_y);

	data.push_back(max_x);
	data.push_back(max_y);
	data.push_back(max_x);
	data.push_back(y);
	data.push_back(x);
	data.push_back(y);
}


