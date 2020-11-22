#pragma once

#include <glm/vec2.hpp>

#include <vector>

class Character;
class Word;
class Line;

class Line {
private:
	std::vector<Word> words_;

	double max_width_ = 0;
	double space_width_ = 0;
	double current_line_width_ = 0;

public:

	/**
	 * @param space_size width of space character in screen space
	 * @param max_width maximum length of line in screen space
	 */
	Line(double space_size, double font_size, double max_width);

	bool addWord(const Word& word);

	double getMaxWidth() const { return max_width_; }
	double getCurrentWidth() const { return current_line_width_; }
	const std::vector<Word>& getWords() { return words_; }
};

class Word {
private:
	std::vector<Character> characters_;
	double width_ = 0;
	double font_size_ = 1;
	
public:
	Word() = default;
	Word(double font_size) : font_size_(font_size) {}

	void addCharacter(const Character& character);

	const std::vector<Character>& getCharacters() { return characters_; }
	double getWidth() const { return width_; }
};

using glm::dvec2;

class Character
{
private:
	int id_ = -1; // ASCII value of character

	// For font size 1
	dvec2 texture_coords_ = dvec2(0); // texture coordinate of top left of character in texture atlas
	dvec2 max_texture_coords_ = dvec2(0);
	dvec2 offset_ = dvec2(0); // cursor's distance from the top and left edges of the character's quad
	dvec2 quad_size_ = dvec2(0); // size of character's quad in screen space
	double x_advance_ = 0; // how far the cursor should advance after adding this character

public:
	Character() = default;

	Character(int id, dvec2 texture_coords, dvec2 texture_size, dvec2 offset, dvec2 quad_size, double x_advance);

	int getId() const { return id_; }
	dvec2 getTextureCoords() const { return texture_coords_; }
	dvec2 getMaxTextureCoords() const { return max_texture_coords_; }
	dvec2 getOffset() const { return offset_; }
	dvec2 getSize() const { return quad_size_; }
	double getAdvance() const { return x_advance_; }
};
