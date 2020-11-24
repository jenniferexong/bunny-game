#pragma once

#include "Environment.h"

#include <GLFW/glfw3.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../gui/GuiTexture.h"
#include "../Loader.h"

#include "../text/TextMaster.h"

using std::string;
using std::map;
using std::shared_ptr;
using std::set;
using std::vector;

class MasterRenderer;

class Scene {
protected:
	shared_ptr<GLFWwindow*> window_;
	shared_ptr<Loader> loader_;
	shared_ptr<MasterRenderer> renderer_;

	Environment environment_;
	vector<shared_ptr<GuiTexture>> guis_;
	TextMaster text_master_;

public:
	virtual const vector<shared_ptr<GuiTexture>>& getGuis() const { return guis_; }
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

