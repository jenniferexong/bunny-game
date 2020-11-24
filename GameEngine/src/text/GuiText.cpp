#include "GuiText.h"

#include "TextMaster.h"
#include "TextLoader.h"

#include "../Application.h"

int GuiText::next_id = 0;
TextLoader GuiText::loader_ = TextLoader();

GuiText::GuiText(const std::string& text, float font_size, std::shared_ptr<FontType> font, glm::vec2 position, float max_line_width, bool centered)
{
	// assign id
	id_ = next_id++;
	text_ = text;
	font_size_ = font_size;
	font_ = font;
	position_ = position;
	max_line_width_ = max_line_width;
	centered_ = centered;

	TextMeshData mesh_data = loader_.createMesh(*this);
	mesh_vao_ = Application::loader->loadToVao(mesh_data.positions, mesh_data.texture_coords);
	vertex_count_ = mesh_data.getVertexCount();
}

void GuiText::updateText(const std::string& text)
{
	text_ = text;

	// delete old vao?
	Application::loader->deleteVao(mesh_vao_);
	TextMeshData mesh_data = loader_.createMesh(*this);
	mesh_vao_ = Application::loader->loadToVao(mesh_data.positions, mesh_data.texture_coords);
	vertex_count_ = mesh_data.getVertexCount();
}

void GuiText::setMeshDataInfo(int vao, int vertex_count)
{
	mesh_vao_ = vao;
	vertex_count_ = vertex_count;
}

