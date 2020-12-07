#pragma once

#include "Environment.h"

#include <GLFW/glfw3.h>

#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../gui/GuiTexture.h"
#include "../Loader.h"

#include "../text/TextMaster.h"

using std::string;
using std::map;
using std::shared_ptr;
using std::unordered_set;
using std::vector;

class MasterRenderer;

namespace std {
	template <>
	struct hash<shared_ptr<GuiTexture>>
	{
		size_t operator()(const shared_ptr<GuiTexture>& gui) const
		{
			return hash<int>()(gui->getTexture());
		}
	};
}

class Scene {
protected:
	shared_ptr<GLFWwindow*> window_;
	shared_ptr<Loader> loader_;
	shared_ptr<MasterRenderer> renderer_;

	Environment environment_;
	unordered_set<shared_ptr<GuiTexture>> guis_;
	TextMaster text_master_;

public:
	virtual const unordered_set<shared_ptr<GuiTexture>>& getGuis() const { return guis_; }
	virtual const Environment& getEnvironment() { return environment_; }
	virtual const TextMaster& getText() { return text_master_; }

	virtual void render() = 0;
	virtual void update() = 0;
	virtual void postRenderUpdate() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;

	virtual void keyCallback(int key, int scan_code, int action, int mods) = 0;
	virtual void mouseButtonCallback(int button, int action, int mods) = 0;
	virtual void cursorPosCallback(double x, double y) = 0;
	virtual void scrollCallBack(double x_offset, double y_offset) = 0;
};

