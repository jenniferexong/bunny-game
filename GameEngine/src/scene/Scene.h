#pragma once

#include <GLFW/glfw3.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../CompareTexturedModel.h"
#include "../gui/GuiTexture.h"
#include "../models/Model.h"

#include "../objects/Entity.h"
#include "../objects/Light.h"
#include "../objects/Terrain.h"
#include "../render_engine/Loader.h"

using std::string;
using std::map;
using std::shared_ptr;
using std::set;
using std::vector;

class Camera;

class Scene {
protected:
	shared_ptr<GLFWwindow*> window_;
	shared_ptr<Loader> loader_;

	map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, CompareTexturedModel> entities_;
	vector<Terrain> terrains_;
	vector<shared_ptr<GuiTexture>> guis_;
	vector<Light> lights_;

public:
	virtual const map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, CompareTexturedModel>& getEntities() { return entities_; }
	virtual const vector<Terrain>& getTerrains() { return terrains_; }
	virtual const vector<shared_ptr<GuiTexture>>& getGuis() { return guis_; }
	virtual vector<Light> getLights() { return lights_; }

	virtual void update() = 0;
	virtual void postRenderUpdate() = 0;

	// GameScene
	virtual shared_ptr<Light> getSun() = 0;
	virtual Camera getCamera() = 0;

	virtual glm::mat4 getProjectionMatrix() = 0;

	virtual void keyCallback(int key, int scan_code, int action, int mods) = 0;
	virtual void mouseButtonCallback(int button, int action, int mods) = 0;
	virtual void cursorPosCallback(double x, double y) = 0;
	virtual void scrollCallBack(double x_offset, double y_offset) = 0;
};

