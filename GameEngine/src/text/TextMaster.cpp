#include "TextMaster.h"

#include "../Application.h"

void TextMaster::addText(GuiText& text)
{
	TextMeshData mesh_data = loader_.createMesh(text);
	int vao = Application::loader->loadToVao(mesh_data.positions, mesh_data.texture_coords);
	text.setMeshDataInfo(vao, mesh_data.getVertexCount());

	// add to map
	auto font_type = text.getFont();
	if (texts_.find(font_type) == texts_.end()) {
		set<GuiText> set;
		texts_.insert({font_type, set });
	}
	texts_.at(font_type).insert(text);
}

void TextMaster::removeText(const GuiText& text)
{
	auto font_type = text.getFont();
	texts_.at(font_type).erase(text);

	if (texts_.at(font_type).empty()) 
		texts_.erase(font_type);
}


