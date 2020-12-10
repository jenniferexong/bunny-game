#include "GuiText.h"

#include "TextMaster.h"
#include "../game-manager/Application.h"
#include "../engine/text/TextLoader.h"

int GuiText::next_id_ = 0;
TextLoader GuiText::loader_ = TextLoader();

GuiText::GuiText(const std::string& text, float font_size, std::shared_ptr<FontType> font, glm::vec2 position, float max_line_width, bool centered)
{
	// assign id
	id_ = next_id_++;
	text_ = text;
	font_size_ = font_size;
	font_ = std::move(font);
	position_ = position;
	max_line_width_ = max_line_width;
	centered_ = centered;

	character_width_ = (1.f / 50.f) * glm::log(font_size_ + 0.1f) + 0.47f;
	edge_transition_ = (1.f / 13.f) * glm::pow(1.45f, -font_size_ + 2.8f) + 0.045f;	

	TextMeshData mesh_data = loader_.createMesh(*this);
	mesh_vao_ = engine->loader->loadToVao(mesh_data.positions, mesh_data.texture_coords);
	vertex_count_ = mesh_data.getVertexCount();
}

void GuiText::updateText(const std::string& text)
{
	text_ = text;

	// delete old vao?
	engine->loader->deleteVao(mesh_vao_);
	TextMeshData mesh_data = loader_.createMesh(*this);
	mesh_vao_ = engine->loader->loadToVao(mesh_data.positions, mesh_data.texture_coords);
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



