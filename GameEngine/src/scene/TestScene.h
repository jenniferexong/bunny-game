#pragma once

#include "Scene.h"

class TestScene final : public Scene {
public:
	TestScene(shared_ptr<MasterRenderer> renderer);

	void update() override {}
	void postRenderUpdate() override {}
	void render() override;

	glm::mat4 getProjectionMatrix() override { return glm::mat4(1.f); }
	void cursorPosCallback(double x, double y) override {}
	void keyCallback(int key, int scan_code, int action, int mods) override {}
	void mouseButtonCallback(int button, int action, int mods) override {}
	void scrollCallBack(double x_offset, double y_offset) override {}
};
