#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "Scene.h"

#include "../Application.h"
#include "../UserInput.h"
#include "../objects/Camera.h"

#include "../gui/MousePicker.h"
#include "../render_engine/Loader.h"

class GameScene: public Scene {
private:
	const float fov = 70.f;
	const float near_plane = 1.f;
	const float far_plane = 1000.f;

	MousePicker mouse_picker;

	shared_ptr<Player> player = nullptr;
	shared_ptr<GuiTexture> compass = nullptr;
	shared_ptr<Entity> selected = nullptr;
	Terrain terrain_1_;
	shared_ptr<Light> sun_ = std::make_shared<Light>(vec3(0.f, 100, 1000), vec3(0.1f));
	Camera camera_;

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
	
public:
	GameScene(shared_ptr<GLFWwindow*> window, shared_ptr<Loader> loader);

	glm::mat4 getProjectionMatrix() override;
	shared_ptr<Light> getSun() override { return sun_; }
	Camera getCamera() override { return camera_; }
	vector<Light> getLights() override;

	void update() override;
	void postRenderUpdate() override;

	void keyCallback(int key, int scan_code, int action, int mods) override;
	void cursorPosCallback(double x, double y) override;
	void mouseButtonCallback(int button, int action, int mods) override;
	void scrollCallBack(double x_offset, double y_offset) override;
};
