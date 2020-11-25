#include "FontData.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "TextLoader.h"
#include "../Application.h"
#include "../Location.h"

using std::cout;
using std::endl;
using std::stoi;
using std::stod;
using std::string;
using std::stringstream;

const int FontData::desired_padding = 8;

Character FontData::getCharacter(int ascii) const
{
	if (characters_.find(ascii) == characters_.end())
		return characters_.at(45);

	return characters_.at(ascii);
}
/**
 * Reads a font file, loading characters into map
 */
FontData::FontData(const std::string& font_name)
{
	int ww, wh;
	glfwGetFramebufferSize(*Application::window, &ww, &wh);
	double aspect_ratio = (double)ww / (double)wh;
	
	std::ifstream file = std::ifstream(FilePath::font_path + font_name + ".fnt");

	// read padding information (top, left, bottom, right) padding=3,3,3,3
	auto padding_data = stringstream(getToken(&file, "padding"));
	string padding_top, padding_left, padding_bottom, padding_right;
	getline(padding_data, padding_top, ',');
	getline(padding_data, padding_left, ',');
	getline(padding_data, padding_bottom, ',');
	getline(padding_data, padding_right);

	int padding_width = stoi(padding_left) + stoi(padding_right);
	int padding_height = stoi(padding_top) + stoi(padding_bottom);

	// line height
	int line_height_pixels = stoi(getToken(&file, "lineHeight")) - padding_height;
	double vertical_per_pixel_size = TextLoader::line_height / (double)line_height_pixels;
	double horizontal_per_pixel_size = vertical_per_pixel_size / aspect_ratio;

	int image_size = stoi(getToken(&file, "scaleW"));
	int char_count = stoi(getToken(&file, "count"));

	// process character data
	for (int i = 0; i < char_count; i++) {
		int char_id = stoi(getToken(&file, "id")); // ASCII

		double x = stod(getToken(&file, "x"));
		double y = stod(getToken(&file, "y"));
		int w = stoi(getToken(&file, "width"));
		int h = stoi(getToken(&file, "height"));
		double x_offset = stod(getToken(&file, "xoffset"));
		double y_offset = stod(getToken(&file, "yoffset"));
		double x_adv = stod(getToken(&file, "xadvance"));

		if (char_id == TextLoader::space_ascii) { // space character
			space_width_ = (x_adv - padding_width) * horizontal_per_pixel_size;
			continue;
		}

		dvec2 texture_coords(0); 
		dvec2 texture_size(0); 
		dvec2 offset(0);
		dvec2 quad_size(0);
		double x_advance;

		// convert to screen space, and remove padding
		texture_coords.x = (x + (stod(padding_left) - desired_padding)) / (double) image_size;
		texture_coords.y = 1.0 - (y + (stod(padding_top) - desired_padding)) / (double)image_size;
		int width = w - (padding_width - (2 * desired_padding));
		int height = h - (padding_height - (2 * desired_padding));
		texture_size.x = (double)width / image_size;
		texture_size.y = (double)height / image_size;
		quad_size.x = width * horizontal_per_pixel_size;
		quad_size.y = height * vertical_per_pixel_size;
		offset.x = (x_offset + stod(padding_left) - desired_padding) * horizontal_per_pixel_size;
		offset.y = (y_offset + stod(padding_top) - desired_padding) * vertical_per_pixel_size;
		x_advance = (x_adv - padding_width) * horizontal_per_pixel_size;

		characters_.insert({ char_id, Character(char_id, texture_coords, texture_size, offset, quad_size, x_advance) });
	}
	cout << "characters: " << characters_.size() << endl;

	file.close();
}

string FontData::getToken(std::ifstream* file, const string& token_name)
{
	string str;
	while(str.find(token_name) == string::npos)
		getline(*file, str, ' ');

	stringstream str_stream = stringstream(str);
	getline(str_stream, str, '=');
	getline(str_stream, str);

	return str;
}

