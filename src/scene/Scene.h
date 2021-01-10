#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../ui/GuiComponent.h"
#include "../ui/GuiTexture.h"
#include "../ui/TextMaster.h"
#include "../game-manager/UserInput.h"

using std::string;
using std::weak_ptr;
using std::vector;

class MasterRenderer;

class Scene {
protected:
	static std::map<Key, bool> move_keys_;

    std::unique_ptr<GuiComponent> gui_;
	vector<weak_ptr<GuiTexture>> guis_;
	TextMaster text_master_;

public:
	virtual ~Scene() = default;

	virtual const TextMaster& getText() { return text_master_; }
	virtual const vector<weak_ptr<GuiTexture>>& getGuis() const { return guis_; }

	virtual bool update() = 0;
	virtual void init() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;

	virtual void keyCallback(int key, int scan_code, int action, int mods) = 0;
	virtual void mouseButtonCallback(int button, int action, int mods) = 0;
	virtual void cursorPosCallback(double x, double y) = 0;
	virtual void scrollCallBack(double x_offset, double y_offset) = 0;
};

