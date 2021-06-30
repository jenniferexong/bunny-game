#include "GuiText.h"

#include "TextMaster.h"
#include "../Engine.h"
#include "../text/TextLoader.h"
#include "GuiBound.h"

int GuiText::next_id_ = 0;
TextLoader GuiText::loader_ = TextLoader();

/**
 * Creates text, loads its mesh into a VAO, adds text to the screen.
 * Positions and widths in screen size, where the range is [0, 1]
 *	- top left (0, 0)
 *	- bottom right (1, 1)
 */
GuiText::GuiText(
	const std::string& text, float font_size,
	const std::string& font, glm::vec2 position,
	float max_line_width, bool centered)
{
	// assign id
	id_ = next_id_++;
	text_ = text;
	font_size_ = font_size;
	font_ = Engine::instance->getFont(font);
	position_ = position;
	max_line_width_ = max_line_width;
	centered_ = centered;

	init();
}

/**
 * Creates a text and positions it relative to bound
 */
GuiText::GuiText(
	const std::string& text, float font_size,
	const std::string& font, glm::vec2 position,
	float max_line_width, bool centered, const GuiBound& bound)
{
	// assign id
	id_ = next_id_++;
	text_ = text;
	font_size_ = font_size;
	font_ = Engine::instance->getFont(font);
	centered_ = centered;

	// calculate position and size
	max_line_width_ = max_line_width * bound.getScale().x;
	vec2 bound_position = bound.getTopLeft();
	bound_position.x = (bound_position.x + 1.f) / 2.f;
	bound_position.y = (bound_position.y - 1.f) / -2.f;

	position_ = bound_position + (position * bound.getScale().x);
	init();
}

void GuiText::init() 
{
	character_width_ = 
		(1.f / 50.f) * glm::log(font_size_ + 0.1f) + 0.47f;
	edge_transition_ = 
		(1.f / 13.f) * glm::pow(1.45f, -font_size_ + 2.8f) + 0.045f;	

	TextMeshData mesh_data = loader_.createMesh(*this);
	mesh_vao_ = Engine::instance->loader->loadToVao(
		mesh_data.positions, 
		mesh_data.texture_coords
	);
	vertex_count_ = mesh_data.getVertexCount();
}

void GuiText::updateText(const std::string& text)
{
	text_ = text;

	// delete old vao?
    Engine::instance->loader->deleteVao(mesh_vao_);
	TextMeshData mesh_data = loader_.createMesh(*this);
	mesh_vao_ = Engine::instance->loader->loadToVao(
		mesh_data.positions,
		mesh_data.texture_coords
	);
	vertex_count_ = mesh_data.getVertexCount();
}

void GuiText::setMeshDataInfo(int vao, int vertex_count)
{
	mesh_vao_ = vao;
	vertex_count_ = vertex_count;
}

void GuiText::setOutline(glm::vec3 color)
{
	border_color_ = color;
	border_width_ = character_width_ + (character_width_ / 8.f);
	border_edge_transition_ = edge_transition_;
}

void GuiText::setGlow(glm::vec3 color)
{
	border_color_ = color;
	border_width_ = character_width_ + (character_width_  / 10.f);
	border_edge_transition_ = character_width_ * 2.f;
}
