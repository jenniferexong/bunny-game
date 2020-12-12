﻿#pragma once

#include <memory>

#include "Scene.h"
#include "../game-manager/UserInput.h"
#include "../environment/Environment.h"
#include "../ui/MousePicker.h"

class GuiTexture;
class GuiText;

class GameScene: public Scene {
private:
	Environment environment_;
	MousePicker mouse_picker_;

	shared_ptr<GuiTexture> compass_ = nullptr;
	shared_ptr<GuiTexture> cross_hair_ = nullptr;

	shared_ptr<GuiText> frame_rate_ = nullptr;

	shared_ptr<Player> player_ = nullptr;
	weak_ptr<Entity> selected_;

	Terrain terrain_1_;
	shared_ptr<Light> sun_ = std::make_shared<Light>(vec3(0.f, 700, -1000), vec3(0.1f));
	shared_ptr<Camera> camera_;
	shared_ptr<Skybox> skybox_;
	vector<shared_ptr<Light>> lights_;
	vector<weak_ptr<Light>> close_lights_;

	map<Key, bool> move_keys_ = {
		{Key::W, false}, {Key::A, false}, {Key::S, false}, {Key::D, false}, {Key::Space, false}
	};

	map<MouseButton, bool> mouse_buttons_ = {
		{MouseButton::Left, false}, {MouseButton::Right, false}
	};

	bool first_mouse_movement_ = true;
	double previous_mouse_x_ = 0;
	double previous_mouse_y_ = 0;

	void setup();
	void makeGame();
	void makeTest();
	void pause();
	
public:
	GameScene();
	~GameScene() = default;

	glm::mat4 getProjectionMatrix() override;

	void init() override;
	bool update() override;
	void gameLogic();

	void render(bool pause);
	void renderScene(glm::vec4 clipping_plane);

	const Environment& getEnvironment() { return environment_; }

	void keyCallback(int key, int scan_code, int action, int mods) override;
	void cursorPosCallback(double x, double y) override;
	void mouseButtonCallback(int button, int action, int mods) override;
	void scrollCallBack(double x_offset, double y_offset) override;
};