#pragma once

#include "../../engine/scene/Scene.h"
#include "GameScene.h"

class PauseScene: public Scene {
private:

	void setUp();

	bool update_ = true;

	void unpause();
	void render();

public:
    std::weak_ptr<Scene> game_scene;

	PauseScene();

	void init() override;
	bool update() override;

	void mouseButtonCallback(int button, int action, int mods) override;
	void cursorPosCallback(double x, double y) override;
	void scrollCallBack(double x_offset, double y_offset) override;
	void keyCallback(int key, int scan_code, int action, int mods) override;
};
