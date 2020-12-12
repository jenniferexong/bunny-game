#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../ui/GuiTexture.h"
#include "../ui/TextMaster.h"

using std::string;
using std::map;
using std::weak_ptr;
using std::unordered_set;
using std::vector;

class MasterRenderer;

class Scene {
protected:
	unordered_set<weak_ptr<GuiTexture>> guis_;
	TextMaster text_master_;

public:
	virtual const unordered_set<weak_ptr<GuiTexture>>& getGuis() const { return guis_; }
	virtual const TextMaster& getText() { return text_master_; }
	virtual ~Scene() = default;

	virtual bool update() = 0;
	virtual void init() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;

	virtual void keyCallback(int key, int scan_code, int action, int mods) = 0;
	virtual void mouseButtonCallback(int button, int action, int mods) = 0;
	virtual void cursorPosCallback(double x, double y) = 0;
	virtual void scrollCallBack(double x_offset, double y_offset) = 0;
};
