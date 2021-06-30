#pragma once

#include "../../engine/scene/Scene.h"

class PauseScene: public Scene {
private:
	void setUp();

	bool update_ = true;

	void unpause();
	void render();

public:
	PauseScene();

	void init() override;
	bool update() override;

	void mouseButtonCallback(int button, int action, int mods) override;
	void cursorPosCallback(double x, double y) override;
	void scrollCallBack(double x_offset, double y_offset) override;
	void keyCallback(int key, int scan_code, int action, int mods) override;
};
