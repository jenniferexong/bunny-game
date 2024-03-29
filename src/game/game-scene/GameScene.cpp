﻿#include "GameScene.h"

#include <memory>
#include <ostream>
#include <fstream>
#include <filesystem>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/random.hpp>

#include "Player.h"
#include "PlayerCamera.h"
#include "../../engine/Application.h"
#include "../../engine/Timer.h"
#include "../../engine/util/FilePath.h"
#include "../game-manager/Helper.h"

using namespace std;
using glm::ivec2;

GameScene::GameScene()
{
	Log::init("GameScene", false);
	setup();
	//makeGame();
	makeTest();
	Log::init("GameScene", true);
}

void GameScene::init()
{
	Log::s("changed to gamescene");
    app->enableCursor(false);
	first_mouse_movement_ = true;
}

bool GameScene::update()
{
	gameLogic();
	render(false);
	return true;
}

void GameScene::render(bool pause)
{
    app->renderer->renderShadowMap(environment_);
    app->renderer->renderWaterReflection(*this);
    app->renderer->renderWaterRefraction(*this);

    app->post_processor->startProcessing();
	renderScene(glm::vec4(0));
    app->renderer->renderWater(environment_);

	if (pause) {
        app->post_processor->blurToFbo();
		return;
	}

	//app->post_processor->antiAliasToScreen();
    app->post_processor->bloomEffect();

    app->renderer->renderGui(guis_);
    app->renderer->renderText(text_master_);
}

void GameScene::renderScene(glm::vec4 clipping_plane)
{
    app->renderer->prepare(getProjectionMatrix());
    app->renderer->renderEntities(environment_, clipping_plane);
    app->renderer->renderTerrain(environment_, clipping_plane);
    app->renderer->renderSkybox(environment_);
}

void GameScene::gameLogic()
{
    app->timer->updateTime();
    app->timer->updateFps();
	if (selected_.lock() != nullptr)
		selected_.lock()->unhighlight();
	selected_.reset();

	// only render lights that are within a distance from the player
	close_lights_.clear();
	close_lights_.push_back(sun_);
	for (const auto& l : lights_) {
		if (glm::distance(player_->getPosition(), l->getPosition()) <= 250.f)
			close_lights_.push_back(l);
	}
	environment_.setLights(close_lights_);

	skybox_->update(sun_);
	Water::updateRipples();

	player_->updatePosition(environment_, move_keys_);
    compass_.lock()->setRotation(-player_->getRotation().x);
	camera_->updateView(terrain_1_, environment_.getWater());

	// must update after camera is moved
	mouse_picker_.update(getProjectionMatrix(), *camera_); 
	environment_.updateInView();

	selected_ = mouse_picker_.selectEntity(environment_);
	if (!selected_.expired())
		selected_.lock()->highlight();

	frame_rate_.lock()->updateText(
        "FPS: " + std::to_string((int)app->timer->fps));
}

using glm::vec2;

void GameScene::setup()
{
	//TODO: Make files that you can read material properties from, 
	// and position, scale, rotation...

    gui_ = make_unique<GuiComponent>("gamescene.json");
    compass_ = gui_->getComponent("compass")->getGui();
    cross_hair_ = gui_->getComponent("cross-hair")->getGui();
    frame_rate_ = gui_->getComponent("frame-rate")->getText();

    gui_->addToBatch(guis_, text_master_);

	//guis_.push_back(shadow_test_);

	// Terrains
	auto texture_pack = Helper::makeTexturePack (
        "green.png", "light-green.png", "brown.png", "rocks.png");
	Texture blend_map = Texture(app->loader->loadTexture("river-blendmap.png"));
	Texture normal_map = Texture(app->loader->loadTexture("rocks-normal.png"));
	TerrainTexture ground_texture = TerrainTexture(
		texture_pack, blend_map, normal_map);
	terrain_1_ = Terrain(0, -1, ground_texture, "river-heightmap.png");
	environment_.addTerrain(terrain_1_);

	// Player
	Material player_material = Material();
	player_material.setGlowMap("black.png");
	auto player_model = Helper::makeModel(
		"nibbles.obj", "nibbles.png", player_material);

	// Bunny player
	float player_x = 489.295f;
	float player_z = -221.175f;
	float player_y = terrain_1_.getHeightOfTerrain(player_x, player_z);
	player_ = make_shared<Player>(
		player_model,
		vec3(player_x, player_y, player_z),
		vec3(45.f, 0, 0), 0.7f
	);
	player_->setRotationOffset(180.f, 0, 0);
	environment_.addEntity(player_);

	camera_ = make_shared<PlayerCamera>(player_);
	environment_.setCamera(camera_);
	environment_.setSun(sun_);

	// skybox
	skybox_ = make_shared<Skybox, fs::path, fs::path, fs::path>(
        "skybox-textures-day.txt", 
        "skybox-textures-night.txt", 
        "skybox-textures-dawn.txt"
    );
	environment_.setSkybox(skybox_);

	// water
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			float x = (2 * (Water::tile_size)) * i + Water::tile_size;
			float z = -(2 * (Water::tile_size)) * j - Water::tile_size;
			environment_.addWater(Water(x, z));
		}
	}
}

void GameScene::makeGame()
{
	Material flower_material = Material(false, false);
	Material carrot_material = Material(10.f, 20.f);

	auto flower_model = Helper::makeModel("flower.obj", "flower.png", flower_material);
	auto carrot_model = Helper::makeModel("carrot.obj", "carrot.png", carrot_material);

	set<shared_ptr<Entity>> flower_set;

	Helper::loadPositionsFromFile(
		terrain_1_, environment_, carrot_model, "carrot-positions.txt", vec3(0), 0.02f);
	Helper::loadPositionsFromFileToSet(
		terrain_1_, flower_set, flower_model, 
		"flower-positions.txt", vec3(0, -90.f, 0), 0.15f
	);
	environment_.addEntitySet(flower_set);

	// Position the lights above flowers
	vec3 color = vec3(1.f, 1, 1);
	for (const auto& flower: flower_set) {
		vec3 flower_pos = flower->getPosition();
		vec3 terrain_normal = terrain_1_.getNormalOfTerrain(
			flower_pos.x, flower_pos.z);
		vec3 light_pos = flower_pos + (15.f * terrain_normal);
		lights_.emplace_back(
			make_shared<Light>(light_pos, color, Light::point_light_attenuation));
	}
}

void GameScene::makeTest()
{
	Material material = Material();
	material.setGlowMap("flower-glow.png");
	auto flower_model = Helper::makeModel("flower.obj", "flower.png", material);
	auto teapot_model = Helper::makeModel("teapot.obj", "white.png", Material(10.f, 20.f));

	Helper::loadPositionsFromFile(
		terrain_1_, environment_, flower_model, 
        {"test-flowers-positions.txt"}, vec3(0, -90.f, 0), 0.15f
	);
	//Helper::loadPositionsFromFile(
		//terrain_1_, environment_, teapot_model, 
		//"teapot", vec3(0, 0.f, 0), 1.f
	//);
}

void GameScene::pause()
{
	render(true);
	app->changeScene(pause_scene);
}

glm::mat4 GameScene::getProjectionMatrix()
{
	// Setting the projection matrix
	return glm::perspective(
		MasterRenderer::fov, app->aspect_ratio, 
		MasterRenderer::near_plane, MasterRenderer::far_plane
	);
}

void GameScene::keyCallback(int key, int scan_code, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			pause();
		break;
	case GLFW_KEY_W: 
		if (action == GLFW_PRESS)
			move_keys_[Key::W] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::W] = false;
		break;
	case GLFW_KEY_A: 
		if (action == GLFW_PRESS)
			move_keys_[Key::A] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::A] = false;
		break;
	case GLFW_KEY_S: 
		if (action == GLFW_PRESS)
			move_keys_[Key::S] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::S] = false;
		break;
	case GLFW_KEY_D: 
		if (action == GLFW_PRESS)
			move_keys_[Key::D] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::D] = false;
		break;
	case GLFW_KEY_SPACE: 
		if (action == GLFW_PRESS)
			move_keys_[Key::Space] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::Space] = false;
		break;
	case GLFW_KEY_L: 
		break;
		if (action == GLFW_RELEASE) {
			string file_name = "teapot-positions.txt";

			// Create and open a text file
			std::ofstream positions(
				FilePath::get({file_name}, FileType::Data),
				std::ios::app
			);
			auto teapot_model = Helper::makeModel(
				"teapot", "white", Material(10.f, 20.f));
			Helper::spawnEntity(
				player_, environment_, teapot_model,
				file_name, vec3(0, 0.f, 0), 1.f
			);
		}
		break;
	default:
		break;
	}
}

void GameScene::cursorPosCallback(double x, double y)
{
	if (first_mouse_movement_) {
		first_mouse_movement_ = false;
		previous_mouse_x_ = x;
		previous_mouse_y_ = y;
		return;
	}

	const float sensitivity = 0.07f;
	float x_offset = float(x - previous_mouse_x_) * sensitivity;
	float y_offset = float(y - previous_mouse_y_) * sensitivity;
	player_->changeDirection(x_offset);
	camera_->changePitch(y_offset);

	previous_mouse_x_ = x;
	previous_mouse_y_ = y;
}

void GameScene::mouseButtonCallback(int button, int action, int mods)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) 
			mouse_buttons_[MouseButton::Left] = true;
		else if (action == GLFW_RELEASE)
			mouse_buttons_[MouseButton::Left] = false;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS) 
			mouse_buttons_[MouseButton::Right] = true;
		else if (action == GLFW_RELEASE)
			mouse_buttons_[MouseButton::Right] = false;
		break;
	default:
		break;
	}
}

void GameScene::scrollCallBack(double x_offset, double y_offset)
{
	camera_->zoom(float(y_offset), environment_.getWater());
}
