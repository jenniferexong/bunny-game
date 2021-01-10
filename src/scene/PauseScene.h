#pragma once

#include "Scene.h"

class PauseScene: public Scene {
private:
	void setUp();

	//std::shared_ptr<GuiText> pause_menu_;
	//GuiComponent* background_;

	bool update_ = true;

	void unpause();
	void render();

public:
	PauseScene();

	void init() override;
	bool update() override;

	glm::mat4 getProjectionMatrix() override { return glm::mat4(1); }

	void mouseButtonCallback(int button, int action, int mods) override;
	void cursorPosCallback(double x, double y) override;
	void scrollCallBack(double x_offset, double y_offset) override;
	void keyCallback(int key, int scan_code, int action, int mods) override;
};
